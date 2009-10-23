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


// Envelope.cpp: implementation of the Envelope class.
//
//////////////////////////////////////////////////////////////////////

#pragma warning(disable:4786)

#include "Envelope.h"
#include "Action.h"

#include "dlmalloc.h"

Envelope::Envelope()
{
	m_KeyFrame = 0;
	m_TotalFrame = 0;
	
	memset(f_ini, 0, sizeof(ChanVec));
	
	nChannels = 1;

	keys = NULL;

	motion.keylist = NULL;
	motion.keys = 0;
	motion.steps = 0;
}

Envelope::~Envelope()
{
	if(keys)
		delete [] keys;
}

int Envelope::KeyNumber(int frame)
{	
	if(keys)
	{
		for(int i=0;i<m_KeyFrame;i++)
		{
			if(frame == keys[i].step)
			{
				return i;
			}
		}
	}

	return 0;
}

int Envelope::GetTotalFrames()
{
	return m_TotalFrame;
}

int Envelope::GetTotalKeys()
{
	return m_KeyFrame;
}

int Envelope::GetFrame(int nKey)
{
	if(keys && nKey >= 0 && nKey < m_KeyFrame)
	{
		return keys[nKey].step;
	}

	return 0;
}

void Envelope::SetLinear(int key, bool bLinear)
{
	if(keys && key >= 0 &&  key < m_KeyFrame)
	{
		keys[key].linear = bLinear?1:0;		
	}
}

bool Envelope::RemoveKey(int key)
{
	if(!keys || m_KeyFrame < 3 || key == 0) return false;

	m_KeyFrame--;

	if(key < m_KeyFrame)
	{
		memcpy(&keys[key], &keys[key+1], (m_KeyFrame - key)*sizeof(KeyFrame));
	}	
	else
	{
		m_TotalFrame = keys[m_KeyFrame-1].step;
	}

	/*MotionCalcStep(keys[key+1].cv, keys[key+1].step);
	MotionCalcStep(keys[key-1].cv, keys[key-1].step);*/
	return true;
}

void Envelope::Value(ChanVec f, int frame)
{
	//Final constante
	
	if(keys)
	{
		if(frame > keys[m_KeyFrame-1].step) frame = keys[m_KeyFrame-1].step;
		else if(frame < 0) frame = 0;
		
		MotionCalcStep(f, frame);
	}
}


void Envelope::Hermite (
						double			 t,
						double			*h1,
						double			*h2,
						double			*h3,
						double			*h4)
{
/*
* Compute Hermite spline coeficients for t, where 0 <= t <= 1.
    */
	
	double			 t2, t3, z;
	
	t2 = t * t;
	t3 = t * t2;
	z = 3.0 * t2 - t3 - t3;
	
	*h1 = 1.0 - z;
	*h2 = z;
	*h3 = t3 - t2 - t2 + t;
	*h4 = t3 - t2;
}

void Envelope::MotionCalcStep(	  ChanVec			 resVec, /* results */
							  double			 step) /* frame */
{
/*
* Compute the motion channel vector for the given step.  Step can be
* fractional but values correspond to frames.
	*/
	
	KeyFrame		*key0 = NULL, *key1 = NULL;
	double			 t = 0.0, h1 = 0.0, h2 = 0.0, h3 = 0.0, h4 = 0.0, res = 0.0, d10 = 0.0;
	double			 dd0a = 0.0, dd0b = 0.0, ds1a = 0.0, ds1b = 0.0;
	double			 adj0 = 0.0, adj1 = 0.0, dd0 = 0.0, ds1 = 9.9;
	int			 tlength = 0, i1 = 0;
	int i = 0;
	
	/*
	* If there is but one key, the values are constant.
	*/
	if (motion.keys == 1) {
		for (i = 0; i < nChannels; i++)
			resVec[i] = motion.keylist[0].cv[i];
		return;
	}
	
	/*
	* Get keyframe pair to evaluate.  This should be within the range
	* of the motion or this will raise an illegal access.
	*/
	key1 = key0 = motion.keylist;
	while (step > key0[i1 + 1].step)
		i1++;
	/* key1[i1 + 1] = key0[i1]; */
	step -= key0[i1].step;
	
	/*
	* Get tween length and fractional tween position.
	*/
	tlength = key1[i1+1].step -  key0[i1].step;
	t = step / tlength; /* entre 0 e 1 */
	
						/*
						* Precompute spline coefficients.
	*/
	if (!key1[i1+1].linear) {
		Hermite (t, &h1, &h2, &h3, &h4);
		dd0a = (1.0 - key0[i1].tens) * (1.0 + key0[i1].cont)
			* (1.0 + key0[i1].bias);
		dd0b = (1.0 - key0[i1].tens) * (1.0 - key0[i1].cont)
			* (1.0 - key0[i1].bias);
		ds1a = (1.0 - key1[i1+1].tens) * (1.0 - key1[i1+1].cont)
			* (1.0 + key1[i1+1].bias);
		ds1b = (1.0 - key1[i1+1].tens) * (1.0 + key1[i1+1].cont)
			* (1.0 - key1[i1+1].bias);
		
		if (key0[i1].step != 0)
			adj0 = tlength / (double)(key1[i1+1].step - key0[i1-1].step);
		if (key1[i1+1].step != motion.steps)
			adj1 = tlength / (double)(key1[i1+2].step - key0[i1].step);
	}
	
	/*
	* Compute the channel components.
	*/
	for (i = 0; i < nChannels; i++) {
		d10 = key1[i1+1].cv[i] - key0[i1].cv[i];
		
		if (!key1[i1+1].linear) {
			if (key0[i1].step == 0)
				dd0 = .5 * (dd0a + dd0b) * d10;
			else
				dd0 = adj0 * (dd0a
				* (key0[i1].cv[i] - key0[i1-1].cv[i])
				+ dd0b * d10);
			
			if (key1[i1+1].step == motion.steps)
				ds1 = .5 * (ds1a + ds1b) * d10;
			else
				ds1 = adj1 * (ds1a * d10 + ds1b
				* (key1[i1+2].cv[i] - key1[i1+1].cv[i]));
			
			res = key0[i1].cv[i] * h1 + key1[i1+1].cv[i] * h2
				+ dd0 * h3 + ds1 * h4;
		} else
			res = key0[i1].cv[i] + t * d10;
		
		resVec[i] = res;
	}
}


void Envelope::AddKeyFrame(double value, int frame, int iChannel, double tens, double cont, double bias)
{
	int i, i1, i2;
	ChanVec f;
	
	// Verifica adição no mesmo frame
	if(keys)
	{
		for(i=0;i<m_KeyFrame;i++)
		{
			if(frame == keys[i].step)
			{
				// Muda o value
				keys[i].cv[iChannel] = value;
				return;
			}
		}
	}
	
	// Aloca memória	
	KeyFrame *key_aux = new KeyFrame[m_KeyFrame+1];
	
	if(keys)
	{
		for(i1=0;i1<m_KeyFrame;i1++) *(key_aux+i1) = keys[i1];
	}

	if(m_KeyFrame > 0) delete keys;
	keys = key_aux;
	
	
	
	
	keys[m_KeyFrame].step = 9999;
	for(i=0;i<=m_KeyFrame; i++) 
	{
		if(frame < keys[i].step)
		{	//Inserir key frame
			key_aux = new KeyFrame[m_KeyFrame+1];
			
			//Copia 1a parte
			for(i1=0; i1<i; i1++) key_aux[i1] = keys[i1];
			
			//Insere
			if(i != m_KeyFrame) 
			{
				motion.keylist = keys;
				motion.keys = m_KeyFrame;
				motion.steps = m_TotalFrame;
				MotionCalcStep(f, frame);
				for(i2 = 0; i2 < nChannels; i2++) 
					key_aux[i].cv[i2] = f[i2];
			}
			else
			{
				for(i2 = 0; i2 < nChannels; i2++) 
					key_aux[i].cv[i2] = 0.0;
			}
			
			key_aux[i].cv[iChannel] = value;
			
			key_aux[i].tens = tens;
			key_aux[i].cont = cont;
			key_aux[i].bias = bias;
			key_aux[i].linear = 0;
			
			key_aux[i].step = frame;
			
			//Copia 1a parte
			for(i1=i+1; i1<m_KeyFrame+1; i1++) key_aux[i1] = keys[i1-1];
			
			delete [] keys;
			keys = key_aux;
			
			i = m_KeyFrame+1;
		}
	}
	
	
	if(!m_KeyFrame)
	{
		for(i = 0; i < nChannels; i++)
		{
			keys[0].cv[i] = f_ini[i];
		}
		
		value = f_ini[0];
	}
	
	
	m_TotalFrame = (m_TotalFrame > frame)?m_TotalFrame:frame;
	
	++m_KeyFrame;
	motion.keylist = keys;
	motion.keys = m_KeyFrame;
	motion.steps = m_TotalFrame;
}

#ifdef GP2X
void Envelope::KeyFrame::ToGP2X()
{
	tens = GP2X_Double(tens).getValue();
	cont = GP2X_Double(cont).getValue();
	bias = GP2X_Double(bias).getValue();

	cv[0] = GP2X_Double(cv[0]).getValue();
	cv[1] = GP2X_Double(cv[1]).getValue();
}
#endif
