/*
native_midi:  Hardware Midi support for the SDL_mixer library
Copyright (C) 2000,2001  Florian 'Proff' Schulze

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Library General Public
  License as published by the Free Software Foundation; either
  version 2 of the License, or (at your option) any later version.
  
    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.
	
	  You should have received a copy of the GNU Library General Public
	  License along with this library; if not, write to the Free
	  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
	  
		Florian 'Proff' Schulze
		florian.proff.schulze@gmx.net
*/

/* everything below is currently one very big bad hack ;) Proff */

#ifndef _WIN32_WCE //maks

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <windowsx.h>
#include <mmsystem.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "native_midi.h"
#include "native_midi_common.h"

#include "../../../gameEngine/dlmalloc.h" //maks

struct _NativeMidiSong {
	int MusicLoaded;
	int MusicPlaying;
	MIDIHDR MidiStreamHdr;
	MIDIEVENT *NewEvents;
	Uint16 ppqn;
	int Size;
	int NewPos;
};

static UINT MidiDevice=MIDI_MAPPER;
static HMIDISTRM hMidiStream;
static NativeMidiSong *currentsong;

static int BlockOut(NativeMidiSong *song)
{
	MMRESULT err;
	int BlockSize;
	
	if ((song->MusicLoaded) && (song->NewEvents))
	{
		// proff 12/8/98: Added for savety
		midiOutUnprepareHeader((HMIDIOUT)hMidiStream,&song->MidiStreamHdr,sizeof(MIDIHDR));
		if (song->NewPos>=song->Size)
			return 0;

		BlockSize=(song->Size-song->NewPos);

		if (BlockSize<=0)
			return 0;

		if (BlockSize>36000)
			BlockSize=36000;

		song->MidiStreamHdr.lpData=(void *)((unsigned char *)song->NewEvents+song->NewPos);
		song->NewPos+=BlockSize;
		song->MidiStreamHdr.dwBufferLength=BlockSize;
		song->MidiStreamHdr.dwBytesRecorded=BlockSize;
		song->MidiStreamHdr.dwFlags=0;
		err=midiOutPrepareHeader((HMIDIOUT)hMidiStream,&song->MidiStreamHdr,sizeof(MIDIHDR));
		if (err!=MMSYSERR_NOERROR)
			return 0;
		err=midiStreamOut(hMidiStream,&song->MidiStreamHdr,sizeof(MIDIHDR));
		return 0;
	}
	return 1;
}

static void MIDItoStream(NativeMidiSong *song, MIDIEvent *evntlist)
{
	int eventcount;
	MIDIEvent *event;
	MIDIEVENT *newevent;
	
	eventcount=0;
	event=evntlist;
	while (event)
	{
		eventcount++;
		event=event->next;
	}
	song->NewEvents=malloc(eventcount*3*sizeof(DWORD));
	if (!song->NewEvents)
		return;
	memset(song->NewEvents,0,(eventcount*3*sizeof(DWORD)));
	
	eventcount=0;
	event=evntlist;
	newevent=song->NewEvents;
	while (event)
	{
		int status = (event->status&0xF0)>>4;
		switch (status)
		{
		case MIDI_STATUS_NOTE_OFF:
		case MIDI_STATUS_NOTE_ON:
		case MIDI_STATUS_AFTERTOUCH:
		case MIDI_STATUS_CONTROLLER:
		case MIDI_STATUS_PROG_CHANGE:
		case MIDI_STATUS_PRESSURE:
		case MIDI_STATUS_PITCH_WHEEL:
			newevent->dwDeltaTime=event->time;
			newevent->dwEvent=(event->status|0x80)|(event->data[0]<<8)|(event->data[1]<<16)|(MEVT_SHORTMSG<<24);
			newevent=(MIDIEVENT*)((char*)newevent+(3*sizeof(DWORD)));
			eventcount++;
			break;
			
		case MIDI_STATUS_SYSEX:
			if (event->status == 0xFF && event->data[0] == 0x51) /* Tempo change */
			{
				int tempo = (event->extraData[0] << 16) |
					(event->extraData[1] << 8) |
					event->extraData[2];
				newevent->dwDeltaTime=event->time;
				newevent->dwEvent=(MEVT_TEMPO<<24) | tempo;
				newevent=(MIDIEVENT*)((char*)newevent+(3*sizeof(DWORD)));
				eventcount++;
			}
			break;
		}
		
		event=event->next;
	}
	
	song->Size=eventcount*3*sizeof(DWORD);
	
	{
		int time;
		int temptime;
		
		song->NewPos=0;
		time=0;
		newevent=song->NewEvents;
		while (song->NewPos<song->Size)
		{
			temptime=newevent->dwDeltaTime;
			newevent->dwDeltaTime-=time;
			time=temptime;
			if ((song->NewPos+12)>=song->Size)
				newevent->dwEvent |= MEVT_F_CALLBACK;
			newevent=(MIDIEVENT*)((char*)newevent+(3*sizeof(DWORD)));
			song->NewPos+=12;
		}
	}
	song->NewPos=0;
	song->MusicLoaded=1;
}

void CALLBACK MidiProc( HMIDIIN hMidi, UINT uMsg, DWORD dwInstance,
					   DWORD dwParam1, DWORD dwParam2 )
{
    switch( uMsg )
    {
    case MOM_DONE:
		if ((currentsong->MusicLoaded) && ((DWORD)dwParam1 == (DWORD)&currentsong->MidiStreamHdr))
			BlockOut(currentsong);
		break;
    case MOM_POSITIONCB:
		if ((currentsong->MusicLoaded) && ((DWORD)dwParam1 == (DWORD)&currentsong->MidiStreamHdr))
			currentsong->MusicPlaying=0;
		break;
    default:
		break;
    }
}

int native_midi_detect()
{
	MMRESULT merr;
	HMIDISTRM MidiStream;
	
	merr=midiStreamOpen(&MidiStream,&MidiDevice,1,(DWORD)&MidiProc,0,CALLBACK_FUNCTION);
	if (merr!=MMSYSERR_NOERROR)
		MidiStream=0;
	midiStreamClose(MidiStream);
	if (!MidiStream)
		return 0;
	else
		return 1;
}

NativeMidiSong *native_midi_loadsong(char *midifile)
{
	NativeMidiSong *newsong;
	MIDIEvent		*evntlist = NULL;
	
	newsong=malloc(sizeof(NativeMidiSong));
	if (!newsong)
		return NULL;
	memset(newsong,0,sizeof(NativeMidiSong));
	
	/* Attempt to load the midi file */
	evntlist = CreateMIDIEventList(midifile, &newsong->ppqn);
	if (!evntlist)
	{
		free(newsong);
		return NULL;
	}
	
	MIDItoStream(newsong, evntlist);
	
	FreeMIDIEventList(evntlist);
	
	return newsong;
}

void native_midi_freesong(NativeMidiSong *song)
{
	if (hMidiStream)
	{
		midiStreamStop(hMidiStream);
		midiStreamClose(hMidiStream);
	}
	if (song)
	{
		if (song->NewEvents)
			free(song->NewEvents);
		free(song);
	}
}

void native_midi_pause(int pauseon) //maks
{
	static int pause = 0;
	if(hMidiStream)
	{
		if(pauseon && !pause)
		{
			midiStreamPause(hMidiStream);
			pause = 1;
		}
		else if(!pauseon && pause)
		{
			midiStreamRestart(hMidiStream);
			pause = 0;
		}
	}
}

void native_midi_start(NativeMidiSong *song)
{
	MMRESULT merr;
	MIDIPROPTIMEDIV mptd;
	
	native_midi_stop();
	if (!hMidiStream)
	{
		merr=midiStreamOpen(&hMidiStream,&MidiDevice,1,(DWORD)&MidiProc,0,CALLBACK_FUNCTION);
		if (merr!=MMSYSERR_NOERROR)
		{
			hMidiStream=0;
			return;
		}
		//midiStreamStop(hMidiStream);
		currentsong=song;
		currentsong->NewPos=0;
		currentsong->MusicPlaying=1;
		mptd.cbStruct=sizeof(MIDIPROPTIMEDIV);
		mptd.dwTimeDiv=currentsong->ppqn;
		merr=midiStreamProperty(hMidiStream,(LPBYTE)&mptd,MIDIPROP_SET | MIDIPROP_TIMEDIV);
		BlockOut(song);
		merr=midiStreamRestart(hMidiStream);
	}
}

void native_midi_stop()
{
	if (!hMidiStream)
		return;
	midiStreamStop(hMidiStream);
	midiStreamClose(hMidiStream);
	currentsong=NULL;
	hMidiStream = 0;
}

int native_midi_active()
{
	return currentsong->MusicPlaying;
}

void native_midi_setvolume(int volume) //maks
{
	//maks:
	//Some devices use 16 bit to represend left and right channel
	//So, using only MAKEWORD(calcVolume , calcVolume) results on a 0x0000VVVV
	//format, thats causes midi pan set to left, always.
	//To solve this problem, copy the low WORD to high word make the format 0xVVVVVVVV

	int calcVolume = volume << 1;
	unsigned long i = 0;
	DWORD dwVolume, longVolume; 

	if(calcVolume > 255)
	{
		calcVolume = 255;
	}
	else if (calcVolume < 0)
	{
		calcVolume = 0;
	}


	dwVolume = MAKEWORD(calcVolume , calcVolume);
	longVolume = MAKELONG(dwVolume , dwVolume);
	midiOutSetVolume((HMIDIOUT)hMidiStream, longVolume);
}

char *native_midi_error()
{
	return "";
}

#endif /* Windows native MIDI support */

#else //_WIN32_WCE

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <wfmtmidi.h>
#include "native_midi.h"
#include "native_midi_common.h"

struct _NativeMidiSong {
	int MusicLoaded;
	int MusicPlaying;
	WAVEHDR MidiStreamHdr;
	WAVEFORMAT_MIDI_MESSAGE *NewEvents;
	Uint16 ppqn;
	int Size;
	int NewPos;
};

static HWAVEOUT hMidiStream;
static NativeMidiSong *currentsong;
static WAVEFORMAT_MIDI wfm;

static int BlockOut(NativeMidiSong *song)
{
	MMRESULT err;
	int BlockSize;
	
	if ((song->MusicLoaded) && (song->NewEvents))
	{
		// proff 12/8/98: Added for savety
		waveOutUnprepareHeader((HWAVEOUT)hMidiStream,&song->MidiStreamHdr,sizeof(WAVEHDR));
		if (song->NewPos>=song->Size)
			return 0;

		BlockSize=(song->Size-song->NewPos);

		if (BlockSize<=0)
			return 0;

		if (BlockSize>300*sizeof(WAVEFORMAT_MIDI_MESSAGE))
			BlockSize=300*sizeof(WAVEFORMAT_MIDI_MESSAGE);

		song->MidiStreamHdr.lpData=(void *)((unsigned char *)song->NewEvents+song->NewPos);
		song->MidiStreamHdr.dwBufferLength=BlockSize;
		//song->MidiStreamHdr.dwBytesRecorded=BlockSize;
		song->MidiStreamHdr.dwFlags=0; //WHDR_BEGINLOOP|WHDR_ENDLOOP;
		//song->MidiStreamHdr.dwLoops = 1;

		song->NewPos+=BlockSize;

		err=waveOutPrepareHeader((HWAVEOUT)hMidiStream,&song->MidiStreamHdr,sizeof(WAVEHDR));
		if (err!=MMSYSERR_NOERROR)
			return 0;
		err=waveOutWrite(hMidiStream,&song->MidiStreamHdr,sizeof(WAVEHDR));
		return 0;
	}
	return 1;
}

int note2freq(int note)
{
	//Dada a nota, retorna a frequencia em Hz
	int freq = (440 / 32.0) * (pow(2, ((note - 9) / 12.0)));
	
	return freq;
}

static void MIDItoStream(NativeMidiSong *song, MIDIEvent *evntlist)
{
	int eventcount;
	MIDIEvent *event;
	WAVEFORMAT_MIDI_MESSAGE *newevent;
	
	eventcount=0;
	event=evntlist;
	while (event)
	{
		eventcount++;
		event=event->next;
	}
	song->NewEvents=malloc(eventcount*sizeof(WAVEFORMAT_MIDI_MESSAGE));
	if (!song->NewEvents)
		return;
	memset(song->NewEvents,0,(eventcount*sizeof(WAVEFORMAT_MIDI_MESSAGE)));
	
	eventcount=0;
	event=evntlist;
	newevent=song->NewEvents;
	while (event)
	{
		int status = (event->status&0xF0)>>4;
		switch (status)
		{
		case MIDI_STATUS_NOTE_OFF:
		case MIDI_STATUS_NOTE_ON:
		case MIDI_STATUS_AFTERTOUCH:
		case MIDI_STATUS_CONTROLLER:
		case MIDI_STATUS_PROG_CHANGE:
		case MIDI_STATUS_PRESSURE:
		case MIDI_STATUS_PITCH_WHEEL:
			newevent->DeltaTicks=event->time;
			newevent->MidiMsg=(event->status|0x80) | (event->data[0]<<8) | (event->data[1]<<16) /*| (MEVT_SHORTMSG<<24)*/;
			/*newevent->DeltaTicks=3;
			newevent->MidiMsg=0x203F0000 | 480;*/
			
			newevent=(WAVEFORMAT_MIDI_MESSAGE*)((char*)newevent+sizeof(WAVEFORMAT_MIDI_MESSAGE));
			eventcount++;
			break;
			
		/*case MIDI_STATUS_SYSEX:
			if (event->status == 0xFF && event->data[0] == 0x51) // Tempo change 
			{
				int tempo = (event->extraData[0] << 16) |
					(event->extraData[1] << 8) |
					event->extraData[2];
				newevent->DeltaTicks=event->time;				
				newevent->MidiMsg=(MIDI_MESSAGE_UPDATETEMPO<<24) | tempo;				

				newevent=(WAVEFORMAT_MIDI_MESSAGE*)((char*)newevent+sizeof(WAVEFORMAT_MIDI_MESSAGE));
				eventcount++;
			}
			break;*/
		}
		
		event=event->next;
	}
	
	song->Size=eventcount*sizeof(WAVEFORMAT_MIDI_MESSAGE);
	
	{
		int time;
		int temptime;
		
		song->NewPos=0;
		time=0;
		newevent=song->NewEvents;
		while (song->NewPos<song->Size)
		{
			temptime=newevent->DeltaTicks;
			newevent->DeltaTicks-=time;
			time=temptime;
			
			/*if ((song->NewPos+12)>=song->Size)
				newevent->MidiMsg |= MEVT_F_CALLBACK;*/

			newevent=(WAVEFORMAT_MIDI_MESSAGE*)((char*)newevent+sizeof(WAVEFORMAT_MIDI_MESSAGE));
			song->NewPos+=sizeof(WAVEFORMAT_MIDI_MESSAGE);
		}
	}
	song->NewPos=0;
	song->MusicLoaded=1;
}

void CALLBACK MidiProc(
  HWAVEOUT hwo,
  UINT uMsg,
  DWORD dwInstance,
  DWORD dwParam1,
  DWORD dwParam2
)
{
	switch( uMsg )
    {
    case WOM_DONE:
		if ((currentsong->MusicLoaded) && ((DWORD)dwParam1 == (DWORD)&currentsong->MidiStreamHdr))
			BlockOut(currentsong);
		break;
    /*case WOM_POSITIONCB:
		if ((currentsong->MusicLoaded) && ((DWORD)dwParam1 == (DWORD)&currentsong->MidiStreamHdr))
			currentsong->MusicPlaying=0;
		break;*/
    default:
		break;
    }

    return;
}


int native_midi_detect()
{
	MMRESULT merr;
	HWAVEOUT MidiStream;

	memset(&wfm,0,sizeof(wfm));
	wfm.wfx.wFormatTag=WAVE_FORMAT_MIDI;
	wfm.wfx.nChannels=1;
	wfm.wfx.nSamplesPerSec=32;
	wfm.wfx.nAvgBytesPerSec=32;
	wfm.wfx.nBlockAlign=sizeof(WAVEFORMAT_MIDI_MESSAGE);
	wfm.wfx.wBitsPerSample=16;
	wfm.wfx.cbSize=WAVEFORMAT_MIDI_EXTRASIZE;
	//wfm.USecPerQuarterNote=9600000; // Force each tick to be 1/10 sec
	//wfm.TicksPerQuarterNote = 96;

	


	merr = waveOutOpen(&MidiStream, WAVE_MAPPER, (LPWAVEFORMATEX)&wfm, (DWORD)MidiProc, 0, CALLBACK_FUNCTION); 

	if (merr!=MMSYSERR_NOERROR)
		MidiStream=0;

	waveOutClose(MidiStream);
	if (!MidiStream)
		return 0;
	else
		return 1;
}

NativeMidiSong *native_midi_loadsong(char *midifile)
{
	NativeMidiSong *newsong;
	MIDIEvent		*evntlist = NULL;
	
	newsong=malloc(sizeof(NativeMidiSong));
	if (!newsong)
		return NULL;
	memset(newsong,0,sizeof(NativeMidiSong));
	
	/* Attempt to load the midi file */
	evntlist = CreateMIDIEventList(midifile, &newsong->ppqn);
	if (!evntlist)
	{
		free(newsong);
		return NULL;
	}
	
	MIDItoStream(newsong, evntlist);
	
	FreeMIDIEventList(evntlist);
	
	return newsong;
}

void native_midi_freesong(NativeMidiSong *song)
{
	if (hMidiStream)
	{
		waveOutReset(hMidiStream);
		waveOutClose(hMidiStream);
	}
	if (song)
	{
		if (song->NewEvents)
			free(song->NewEvents);
		free(song);
	}
}

void native_midi_pause(int pauseon) //maks
{
	static int pause = 0;
	if(hMidiStream)
	{
		if(pauseon && !pause)
		{
			waveOutPause(hMidiStream);
			pause = 1;
		}
		else if(!pauseon && pause)
		{
			waveOutRestart(hMidiStream);
			pause = 0;
		}
	}
}

void native_midi_start(NativeMidiSong *song)
{
	MMRESULT merr;
	
	memset(&wfm,0,sizeof(wfm));
	wfm.wfx.wFormatTag=WAVE_FORMAT_MIDI;
	wfm.wfx.nChannels=1;
	wfm.wfx.nSamplesPerSec=32;
	wfm.wfx.nAvgBytesPerSec=32;
	wfm.wfx.nBlockAlign=sizeof(WAVEFORMAT_MIDI_MESSAGE);
	wfm.wfx.wBitsPerSample=16;
	wfm.wfx.cbSize=WAVEFORMAT_MIDI_EXTRASIZE;
	wfm.USecPerQuarterNote=9600000; // Force each tick to be 1/10 sec
	//wfm.TicksPerQuarterNote = 96;
	
	wfm.USecPerQuarterNote=263157; //+-
    wfm.TicksPerQuarterNote=800;//480;

	/*wfm.USecPerQuarterNote=500000; //Wrong
    wfm.TicksPerQuarterNote=4;*/

	

	native_midi_stop();
	if (!hMidiStream)
	{
		merr = waveOutOpen(&hMidiStream, WAVE_MAPPER, (LPWAVEFORMATEX)&wfm, (DWORD)MidiProc, 0, CALLBACK_FUNCTION); 
		
		if (merr!=MMSYSERR_NOERROR)
		{
			hMidiStream=0;
			return;
		}
		//midiStreamStop(hMidiStream);
		currentsong=song;
		currentsong->NewPos=0;
		currentsong->MusicPlaying=1;
		/*mptd.cbStruct=sizeof(MIDIPROPTIMEDIV);
		mptd.dwTimeDiv=currentsong->ppqn;
		merr=midiStreamProperty(hMidiStream,(LPBYTE)&mptd,MIDIPROP_SET | MIDIPROP_TIMEDIV);*/
		BlockOut(song);
		merr=waveOutReset(hMidiStream);
	}
}

void native_midi_stop()
{
	if (!hMidiStream)
		return;
	waveOutReset(hMidiStream);
	waveOutClose(hMidiStream);
	currentsong=NULL;
	hMidiStream = 0;
}

int native_midi_active()
{
	return currentsong->MusicPlaying;
}

void native_midi_setvolume(int volume)
{
	int calcVolume = volume << 1;
	if(calcVolume > 255)
	{
		calcVolume = 255;
	}
	else if (calcVolume < 0)
	{
		calcVolume = 0;
	}
	waveOutSetVolume((HWAVEOUT)hMidiStream, MAKEWORD(calcVolume , calcVolume));
}

char *native_midi_error()
{
	return "";
}

#endif /* PocketPC native MIDI support */
