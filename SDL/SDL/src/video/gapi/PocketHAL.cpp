/**************************************************************************

Game Editor - The Cross Platform Game Creation Tool
Copyright (C) 2009  Makslane Araujo Rodrigues, http://game-editor.com

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

If GPL v3 does not work for you, we offer a "non-GPL" license 
More info at http://game-editor.com/License
Be a Game Editor developer: http://game-editor.com/Sharing_Software_Revenues_in_Open_Source

***************************************************************************/




#include "../../../../../PocketHAL/PocketHAL/pockethal.h"
#include "SDL_keyboard.h"
#include "SDL.h"



#if defined(_WIN32_WCE)

#ifndef DISP_CHANGE_SUCCESSFUL
#define DISP_CHANGE_SUCCESSFUL 0
#endif
#ifndef CDS_TEST
#define CDS_TEST            0x00000002
#endif
#ifndef DM_DISPLAYORIENTATION
#define DM_DISPLAYORIENTATION 0x00800000L
#endif
#ifndef DM_DISPLAYQUERYORIENTATION 
#define DM_DISPLAYQUERYORIENTATION 0x01000000L
#endif
#ifndef DMDO_0
#define DMDO_0      0
#endif
#ifndef DMDO_90
#define DMDO_90     1
#endif
#ifndef DMDO_180
#define DMDO_180    2
#endif
#ifndef DMDO_270
#define DMDO_270    4
#endif

#endif

//direction flags
#define	DIR_SWAPXY			0x001
#define	DIR_MIRRORLEFTRIGHT	0x002
#define	DIR_MIRRORUPDOWN	0x004


static int screenOrientation = -1;
extern "C" int QueryOrientation()
{
	//Returns the real system orientation


	screenOrientation = -1; //if (Orientation < 0)
	{
		HKEY Key;
		char Buffer[256];
		DEVMODE* Mode = (DEVMODE*)Buffer;
		//int i,No;
		OSVERSIONINFO Ver;
		int version = 0;

		Ver.dwOSVersionInfoSize = sizeof(Ver);
		GetVersionEx(&Ver);

		version = Ver.dwMajorVersion*100 + Ver.dwMinorVersion;

		//node* Node;

		Mode->dmSize = 192;
		Mode->dmFields = DM_DISPLAYQUERYORIENTATION;

		HMODULE CoreDLL = LoadLibrary(TEXT("coredll.dll"));
		
		if (CoreDLL)
		{
			LONG (WINAPI* FuncChangeDisplaySettingsEx)(LPCTSTR,LPDEVMODE,HWND,DWORD,LPVOID) = NULL;
			*(FARPROC*)&FuncChangeDisplaySettingsEx = GetProcAddress(CoreDLL,TEXT("ChangeDisplaySettingsEx"));		
			
			
			if (version >= 421 && // we don't trust this method on pre wm2003se systems
				FuncChangeDisplaySettingsEx &&
				FuncChangeDisplaySettingsEx(NULL, Mode, NULL, CDS_TEST, NULL) == DISP_CHANGE_SUCCESSFUL)
			{
				Mode->dmFields = DM_DISPLAYORIENTATION;
				FuncChangeDisplaySettingsEx(NULL, Mode, NULL, CDS_TEST, NULL);
				
				switch ((&Mode->dmDisplayFrequency)[1]) //(Mode->dmDisplayOrientation)
				{
				case DMDO_0: screenOrientation = PHAL::ORIENTATION_NORMAL; break;
				case DMDO_90: screenOrientation = PHAL::ORIENTATION_ROTATE90CW; break;
				case DMDO_270: screenOrientation = PHAL::ORIENTATION_ROTATE90CCW; break;
				case DMDO_180: screenOrientation = PHAL::ORIENTATION_ROTATE180; break;
				}
			}
			
			
			FreeLibrary(CoreDLL);
		}

		/*if (screenOrientation < 0)
			for (No=0;(Node = NodeEnum(VOUT_CLASS,No))!=NULL;++No)
				if (Node->Get(Node,VOUT_ORIENTATION,&i,sizeof(int))==ERR_NONE)
				{
					screenOrientation = i;
					break;
				}*/

		if (screenOrientation < 0 && RegOpenKeyEx( HKEY_LOCAL_MACHINE, TEXT("System\\GDI\\ROTATION"), 0, 0, &Key ) == ERROR_SUCCESS)
		{
			DWORD Value;
			DWORD RegSize = sizeof(Value);
			DWORD RegType;

			if (RegQueryValueEx(Key, TEXT("Angle"), 0, &RegType, (LPBYTE) &Value, &RegSize) == ERROR_SUCCESS)
				switch (Value)
				{
				case 0: screenOrientation = PHAL::ORIENTATION_NORMAL; break;
				case 90: screenOrientation = PHAL::ORIENTATION_ROTATE90CW; break;
				case 270: screenOrientation = PHAL::ORIENTATION_ROTATE90CCW; break;
				case 180: screenOrientation = PHAL::ORIENTATION_ROTATE180; break;
				}

			RegCloseKey( Key );
		}
	}

	return screenOrientation;
}

/////////////////////////////////////////////////////////////////



using namespace PHAL;
static Display *disp = NULL;
int mouse_orientation = ORIENTATION_NORMAL;
bool bNeedAdjustMouse = false;

extern int GetFlipPocketPCScreen();
extern void disable_dlmalloc();
extern void enable_dlmalloc();
extern "C" int IsHPC();




extern "C" void halClose()
{
	disable_dlmalloc();
	delete disp;
	enable_dlmalloc();

	disp = NULL;
}

extern "C" void halCreate(HWND hwnd, int width, int height)
{
	DisplayParameters parameters;
	bNeedAdjustMouse = false;

	if(disp)
	{
		halClose();
	}

	TCHAR szOEM[64];

    SystemParametersInfo(SPI_GETOEMINFO, sizeof(szOEM), szOEM, 0);

	int sysScreenW = GetSystemMetrics(SM_CXSCREEN),
		sysScreenH = GetSystemMetrics(SM_CYSCREEN);

	

	QueryOrientation();

	//Write a info file if the file deviceinfo.txt exists
	FILE *f = fopen("deviceinfo.txt", "r");
	if(f)
	{
		//File exists, get the device info
		fclose(f);

		f = fopen("deviceinfo.txt", "w");
		if(f)
		{
			fwprintf(f, L"Device name: %s\n", szOEM);
			fprintf(f, "Game resolution: %ld x %ld\n", width, height);
			fprintf(f, "Screen: %ld x %ld\n", sysScreenW, sysScreenH);
			fprintf(f, "Flip the screen: %ld\n", GetFlipPocketPCScreen());
			
			fclose(f);
		}
	}

	//Check the screen. Some system have 320x240 screen
	if (!_wcsicmp(szOEM, _T("MotoQ")) || !_wcsicmp(szOEM, _T("Gizmondo")) 
		|| (sysScreenW == 320 && sysScreenH == 240) //Solve the BlackJack II screen problem (http://game-editor.com/forum/viewtopic.php?p=33788)
		)
	{
		/*
		That is correct, the Motorola screen has a 240x320 screen with
		orientation 90CCW. Unfortunately it isn't reported as such.

		If you want a 320x240 screen, you will need to specify
		ORIENTATION_NORMAL. Earlier versions of PocketHAL didn't have this fix
		for the Motorola Q.

		Regards,
		Thierry
		*/

		if(width > height) 
		{
			if(GetFlipPocketPCScreen())
			{
				parameters.m_orientation = ORIENTATION_ROTATE180;
			}
			else
			{
				parameters.m_orientation = ORIENTATION_NORMAL;
			}

			mouse_orientation = parameters.m_orientation;
		}
		else
		{
			//Rotate frame buffer
			if(GetFlipPocketPCScreen())
			{
				//90
				parameters.m_orientation = ORIENTATION_ROTATE90CW;
			}
			else
			{
				//270
				parameters.m_orientation = ORIENTATION_ROTATE90CCW;
			}

			//Need to rotate the mouse coordinates?
			if(sysScreenW > sysScreenH)
			{
				//Yes
				mouse_orientation = parameters.m_orientation;
			}
			else
			{
				//System rotated screen
				bNeedAdjustMouse = true;
			}
		}
		
		
	}
	else if(
		(width == 640 && height == 240) ||		
		(width == 800 && height == 480) ||
		(width == 800 && height == 600)
	  )
	{
		//May be is a HPC
		//No rotation
		parameters.m_orientation = ORIENTATION_NORMAL;
		mouse_orientation = parameters.m_orientation;
	}
	else
	{
		if(width > height) 
		{
			//Rotate frame buffer
			if(GetFlipPocketPCScreen())
			{
				//90
				parameters.m_orientation = ORIENTATION_ROTATE90CW;
			}
			else
			{
				//270
				parameters.m_orientation = ORIENTATION_ROTATE90CCW;
			}

			//Need to rotate the mouse coordinates?
			if(sysScreenW < sysScreenH)
			{
				//Yes
				mouse_orientation = parameters.m_orientation;
			}
			else
			{
				//System rotated screen
				bNeedAdjustMouse = true;
			}
		}
		else
		{
			if(GetFlipPocketPCScreen())
			{
				parameters.m_orientation = ORIENTATION_ROTATE180;
			}
			else
			{
				parameters.m_orientation = ORIENTATION_NORMAL;
			}

			mouse_orientation = parameters.m_orientation;
		}
	}

	//MediaQ seams not to copy the backbuffer to frontbuffer. Instead it swap the pointers
	//This causes the flickering like using flip in DirectX (solved with SDL_CREATE_SHADOW)
	//Use SWAP_COPY here

	parameters.m_swapEffect = SWAP_COPY; //30fps in fps benchmark.ged

	disable_dlmalloc();
	disp = Display::Create(hwnd, parameters);
	enable_dlmalloc();
}



extern "C" void *halGetBuffer()
{
	uint16_t *buf = NULL;
	
	if(disp->BeginScene()) 
	{
		Surface *surf = disp->GetBackBuffer();
		if(surf)
		{
			buf = disp->GetBackBuffer()->GetPixels();
		}
	}

	return buf;
}

bool CanOpenSIP()
{
	if(disp)
	{
		DisplayParameters parameters = disp->GetParameters();
		return parameters.m_orientation == ORIENTATION_NORMAL;
	}

	return false;
}

extern "C" HWND sipHwnd;
HBITMAP hTargetBitmap = NULL;
void *pBuffer = NULL;
RECT rect;
BITMAP bm;


void CopySIP(uint16_t *screenBuffer)
{
	//Copy the SIP image to screen buffer
	if(!sipHwnd) 
	{
		if(hTargetBitmap)
		{
			DeleteObject(hTargetBitmap);
			hTargetBitmap = NULL;
			pBuffer = NULL;
		}

		return;
	}


	DisplayParameters parameters = disp->GetParameters();

	if(!hTargetBitmap)
	{
		HDC hdcSrc, dc;
		HGDIOBJ hOld;
		HBITMAP hSourceBitmap = NULL, dstBitmap = NULL;
		
		
		// get te hDC of the target window    
		dc = GetWindowDC(sipHwnd);
		if(!dc) return;
		
		hdcSrc = CreateCompatibleDC(dc);		
		GetWindowRect(sipHwnd, &rect);
		
		
		//1. Get the source bitmap
		hSourceBitmap = CreateCompatibleBitmap(hdcSrc, rect.right - rect.left, rect.bottom - rect.top); 
		if(!hSourceBitmap)
		{
			ReleaseDC(sipHwnd, hdcSrc);
			return;
		}
		
		hOld = SelectObject(hdcSrc, hSourceBitmap);
		
		BitBlt(hdcSrc, 0, 0, rect.right - rect.left, rect.bottom - rect.top, dc, 0, 0, SRCCOPY);
		
		SelectObject(hdcSrc,hOld);
		
		//2. Getting bitmap size.
		
		GetObject(hSourceBitmap, sizeof(BITMAP), &bm);
		
		
		//3. Creating new bitmap and receive pointer to it's bits.
		
		
		//3.1 Initilize DIBINFO structure
		BITMAPINFO  dibInfo;
		
		dibInfo.bmiHeader.biSize = sizeof(BITMAPINFO);
		dibInfo.bmiHeader.biBitCount = 16;
		dibInfo.bmiHeader.biClrImportant = 0;
		dibInfo.bmiHeader.biClrUsed = 0;
		dibInfo.bmiHeader.biCompression = BI_BITFIELDS;
		dibInfo.bmiHeader.biWidth = bm.bmWidth;
		dibInfo.bmiHeader.biHeight = -bm.bmHeight;
		dibInfo.bmiHeader.biPlanes = 1;	
		dibInfo.bmiHeader.biSizeImage = bm.bmWidth*bm.bmHeight*2;		
		dibInfo.bmiHeader.biXPelsPerMeter = 3780;
		dibInfo.bmiHeader.biYPelsPerMeter = 3780;

		*(DWORD*)dibInfo.bmiColors			= 0xf800; //(red)
		*((DWORD*)dibInfo.bmiColors + 1)	= 0x07e0; //(green)
		*((DWORD*)dibInfo.bmiColors + 2)	= 0x001f; //(blue)
		
		//3.2 Create bitmap and receive pointer to points into pBuffer
		HDC hDC = ::GetDC(NULL);
		ASSERT(hDC);
		hTargetBitmap = CreateDIBSection(
			hDC,
			(const BITMAPINFO*)&dibInfo,
			DIB_RGB_COLORS,
			(void**)&pBuffer,
			NULL,
			0);
		
		::ReleaseDC(NULL, hDC);
		
		
		//4. Copy source bitmap into the target bitmap.
		
		//4.1 Create 2 device contexts 
		HDC memDc = CreateCompatibleDC(NULL),
			targetDc = CreateCompatibleDC(NULL);
		
		
		//4.2 Select source bitmap into one DC, target into another
		HBITMAP hOldBitmap1 = (HBITMAP)::SelectObject(memDc, hSourceBitmap);
		HBITMAP hOldBitmap2 = (HBITMAP)::SelectObject(targetDc, hTargetBitmap);
		
		//4.3 Copy source bitmap into the target one
		BitBlt(targetDc, 0, 0, bm.bmWidth, bm.bmHeight, memDc, 0, 0, SRCCOPY);
		
		//4.4 Restore device contexts
		::SelectObject(memDc, hOldBitmap1);
		::SelectObject(targetDc, hOldBitmap2);
		DeleteDC(memDc);
		DeleteDC(targetDc);
		
		// clean up 
		//DeleteDC(hdcDest);
		ReleaseDC(sipHwnd, hdcSrc);
		
		DeleteObject(hSourceBitmap);
	}

	//Here we can bitmap bits: pBuffer. Note:
	// 1. pBuffer contains 2 bytes per point
	// 2. Lines ane from the bottom to the top!
	// 3. Points in the line are from the left to the right
	// 4. Bytes in one point are BGR (blue, green, red) not RGB
	// 5. Don't delete pBuffer, it will be automatically deleted 
	//    when delete hTargetBitmap

	if(pBuffer)
	{
		memcpy(screenBuffer + rect.top*parameters.m_width, pBuffer, bm.bmWidth*bm.bmHeight*2);
	}
}

extern "C" void halSwap(void *backBufer)
{
	//Don't use vsync now
	//Slow down the game loop
	//Use when split game loop and render loop

	
	
	if(backBufer)
	{
		CopySIP((uint16_t *)backBufer);

		//20 fps (fps benchmark.ged)
		DisplayParameters parameters = disp->GetParameters();				
		
		if (disp->BeginScene())
		{
			// Render one frame
			FastCopy(disp->GetBackBuffer()->GetPixels(), backBufer, (parameters.m_width*parameters.m_height) << 1);
			
			disp->Swap(SWAP_VSYNC | SWAP_FULL_UPDATE);
		}
		
		return;
	}

	CopySIP((uint16_t *)halGetBuffer());

	//20 fps with SWAP_VSYNC (fps benchmark.ged)

	//Tests in hp 2210 (400MHz) (m_swapEffect = SWAP_COPY):
	// SWAP_VSYNC: 72 fps
	// SWAP_VSYNC | SWAP_FULL_UPDATE: 33 fps

	//SWAP_VSYNC solve flick in vsync/pongtest.ged
	//SWAP_FULL_UPDATE better visual results in hp 2210
	disp->Swap(SWAP_VSYNC | SWAP_FULL_UPDATE);	
}


extern "C" void halGetDimensions(int *width, int *height)
{
	if(!disp) return;

	DisplayParameters parameters = disp->GetParameters();

	*width = parameters.m_width;
	*height = parameters.m_height;
}

extern "C" void setViewPort(RECT rect)
{
	if(disp)
	{
		PHAL::Rect viewPort;
		viewPort.left = rect.left;
		viewPort.top = rect.top;
		viewPort.right = rect.right;
		viewPort.bottom = rect.bottom;

		disp->SetViewport(&viewPort);
	}
}

extern "C" void InitPocketPCKeys(SDLKey *keymap)
{
	if(!disp) return;

	KeyList defaultKeys;
	DisplayParameters parameters = disp->GetParameters();

	if(GetKeyList(&defaultKeys, parameters.m_orientation))
	{
		keymap[defaultKeys.vkUp]		= SDLK_POCKET_UP;
		keymap[defaultKeys.vkDown]		= SDLK_POCKET_DOWN;
		keymap[defaultKeys.vkLeft]		= SDLK_POCKET_LEFT;
		keymap[defaultKeys.vkRight]	= SDLK_POCKET_RIGHT;
		
		keymap[defaultKeys.vkA]		= SDLK_POCKET_A;
		keymap[defaultKeys.vkB]		= SDLK_POCKET_B;
		keymap[defaultKeys.vkC]		= SDLK_POCKET_C;
		keymap[defaultKeys.vkStart]	= SDLK_POCKET_START; 
		
		keymap[defaultKeys.vkAux1]	= SDLK_POCKET_AUX1; 
		keymap[defaultKeys.vkAux2]	= SDLK_POCKET_AUX2;
		keymap[defaultKeys.vkAux3]	= SDLK_POCKET_AUX3;
		keymap[defaultKeys.vkAux4]	= SDLK_POCKET_AUX4;
		keymap[defaultKeys.vkAux5]	= SDLK_POCKET_AUX5;
		keymap[defaultKeys.vkAux6]	= SDLK_POCKET_AUX6;
		keymap[defaultKeys.vkAux7]	= SDLK_POCKET_AUX7;
		keymap[defaultKeys.vkAux8]	= SDLK_POCKET_AUX8;		
	}

	//Change direction keys
	/*switch(parameters.m_orientation)
	{
	case ORIENTATION_ROTATE90CCW:
		parameters.m_orientation = ORIENTATION_ROTATE90CW;
		break;

	case ORIENTATION_ROTATE90CW:
		parameters.m_orientation = ORIENTATION_ROTATE90CCW;
		break;

	case ORIENTATION_ROTATE180:
		keymap[VK_UP]		= SDLK_DOWN;
		keymap[VK_DOWN]		= SDLK_UP;
		keymap[VK_RIGHT]	= SDLK_LEFT;
		keymap[VK_LEFT]		= SDLK_RIGHT;
		break;
	}*/
}

extern "C" void AdjustMouseCoords(Sint16 *x, Sint16 *y)
{
	if(!disp) return;

	Sint16 xOri = *x, yOri = *y;
	DisplayParameters parameters = disp->GetParameters();	
	
	if(bNeedAdjustMouse)
	{
		if(screenOrientation < 0 || screenOrientation == parameters.m_orientation)
		{
			//Game and system with same orientation. Don't change the mouse orientation
			mouse_orientation = ORIENTATION_NORMAL;
		}
		else
		{
			mouse_orientation = ORIENTATION_ROTATE180;
		}
	}

	switch(mouse_orientation)
	{
	case ORIENTATION_ROTATE90CCW:
		*x = parameters.m_width - yOri;
		*y = xOri;
		break;

	case ORIENTATION_ROTATE180:
		*x = parameters.m_width - xOri;
		*y = parameters.m_height - yOri;
		break;

	case ORIENTATION_ROTATE90CW:
		*x = yOri;
		*y = parameters.m_height - xOri;
		break;
	}
}

