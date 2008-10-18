//[*]------------------------------------------------------------------------------------------[*]
/*
    SDL - Simple DirectMedia Layer
    Copyright (C) 1997-2004 Sam Lantinga

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

    Sam Lantinga
    slouken@libsdl.org
*/
//[*]------------------------------------------------------------------------------------------[*]
#ifdef SAVE_RCSID
static char rcsid =
 "@(#) $Id: SDL_sysjoystick.c,v 1.19 2004/09/11 17:48:41 slouken Exp $";
#endif
//[*]------------------------------------------------------------------------------------------[*]
#include <stdlib.h>
#include <stdio.h>		/* For the definition of NULL */
#include <sys/types.h>	// For Device open
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>		// For Device read
#include <memory.h> //maks


#include "SDL_error.h"
#include "SDL_joystick.h"
#include "SDL_sysjoystick.h"
#include "SDL_joystick_c.h"
#include "SDL_keysym.h" //maks
#include "SDL_events_c.h" //maks

//[*]------------------------------------------------------------------------------------------[*]
#define MAX_JOYSTICKS	19		/* only 2 are supported in the multimedia API */
#define MAX_AXES		0		/* each joystick can have up to 2 axes */
#define MAX_BUTTONS		19		/* and 6 buttons                      */
#define	MAX_HATS		0

#define	JOYNAMELEN		7

/* GPKEY IOCTL Commands */
#if 0
#define IOCTL_GP2X_BACK_LIGHT    _IOR('v', 0x00 ,unsigned long)

#else
#define IOCTL_GP2X_CONTROL_LIB    _IOR('v', 0x00 ,unsigned long)
#define BACK_LIGHT_OFF		0
#define BACK_LIGHT_ON		1
#define BATT_LED_ON			2
#define BATT_LED_OFF		3

#define FCLK_200			10
#define FCLK_166			11
#define FCLK_133			12
#define FCLK_100			13	
#define FCLK_78				14
#define FCLK_64				15
#define FCLK_DEFUALT		16


#endif


#if 0
enum {
	VK_UP			,/* 0 */	
	VK_UP_LEFT		,/* 1 */	
	VK_LEFT			,/* 2 */
	VK_DOWN_LEFT	,/* 3 */	
	VK_DOWN			,/* 4 */
	VK_DOWN_RIGHT	,/* 5 */	
	VK_RIGHT		,/* 6 */	
	VK_UP_RIGHT		,/* 7 */
	VK_START		,/* 8 */	
	VK_SELECT		,/* 9 */	
	VK_FL			,/* 10*/	
	VK_FR			,/* 11 */	
	VK_FA			,/* 12 */	
	VK_FB			,/* 13 */	
	VK_FX			,/* 14 */	
	VK_FY			,/* 15 */	
	VK_VOL_UP		,/* 16 */	
	VK_VOL_DOWN		,/* 17 */	
	VK_TAT			/* 18 */	
}MAP_KEY;	
#endif	

#if 1
/* function 2 */
#define	GPX_VK_UP				(1<<0)	
#define	GPX_VK_UP_LEFT			(1<<1)	
#define	GPX_VK_LEFT				(1<<2)	
#define	GPX_VK_DOWN_LEFT		(1<<3)	
#define	GPX_VK_DOWN				(1<<4)	
#define	GPX_VK_DOWN_RIGHT		(1<<5)	
#define	GPX_VK_RIGHT			(1<<6)	
#define	GPX_VK_UP_RIGHT			(1<<7)	
/* function 1 */
#define	GPX_VK_START			(1<<8)	
#define	GPX_VK_SELECT			(1<<9)	
#define	GPX_VK_FL				(1<<10)	
#define	GPX_VK_FR				(1<<11)	
#define	GPX_VK_FA				(1<<12)	
#define	GPX_VK_FB				(1<<13)	
#define	GPX_VK_FX				(1<<14)	
#define	GPX_VK_FY				(1<<15)	

#define	GPX_VK_VOL_UP			(1<<16)	
#define	GPX_VK_VOL_DOWN			(1<<17)	
#define GPX_VK_TAT				(1<<18)
#endif


static char *dev_name = "/dev/GPIO";

//[*]------------------------------------------------------------------------------------------[*]
struct joystick_hwdata
{
	int fd;
	int prev_buttons;
};
//[*]------------------------------------------------------------------------------------------[*]
/* Function to scan the system for joysticks.
 * This function should set SDL_numjoysticks to the number of available
 * joysticks.  Joystick 0 should be the system default joystick.
 * It should return 0, or -1 on an unrecoverable fatal error.
 */
int SDL_SYS_JoystickInit(void)
{
	printf("SDL_SYS_JoystickInit\n");
	return 1;
}
//[*]------------------------------------------------------------------------------------------[*]
/* Function to get the device-dependent name of a joystick */
const char *SDL_SYS_JoystickName(int index)
{
	return "PEP Joy";
}

//[*]------------------------------------------------------------------------------------------[*]
/* Gp2x back light function */

void SDL_SYS_JoysticBacklightControl(SDL_Joystick *joystick,int onoffFlag)
{
#if 0	
	switch(onoffFlag)
	{
		case 0:		/* off */
			ioctl(joystick->hwdata->fd,IOCTL_GP2X_BACK_LIGHT,0);
			break;
		case 1:		/* on */
			ioctl(joystick->hwdata->fd,IOCTL_GP2X_BACK_LIGHT,1);
			break;
	}
#else
	ioctl(joystick->hwdata->fd,IOCTL_GP2X_CONTROL_LIB,onoffFlag);
	
#endif
}


void SDL_SYS_JoystickGp2xSys(SDL_Joystick *joystick,int cmd)
{
	ioctl(joystick->hwdata->fd,IOCTL_GP2X_CONTROL_LIB,cmd);
}

	
//[*]------------------------------------------------------------------------------------------[*]
/* Function to open a joystick for use.
   The joystick to open is specified by the index field of the joystick.
   This should fill the nbuttons and naxes fields of the joystick structure.
   It returns 0, or -1 if there is an error.
 */
int SDL_SYS_JoystickOpen(SDL_Joystick *joystick)
{
	int fd = 0;

	fd = open(dev_name, O_RDWR | O_NDELAY );
	if(fd < 0){      	
        printf( "GPIO OPEN FAIL\n"); 
        return -1;
	}
	
	joystick->hwdata = (struct joystick_hwdata *) malloc(sizeof(*joystick->hwdata));
	if (joystick->hwdata == NULL)
	{
		SDL_OutOfMemory();
		return -1;
	}
	joystick->hwdata->prev_buttons=0;

	// fill nbuttons, naxes, and nhats fields
	joystick->nbuttons = MAX_BUTTONS;
	joystick->naxes = MAX_AXES;
	joystick->nhats = MAX_HATS;
	
	joystick->hwdata->fd = fd;
	joystick->hwdata->prev_buttons = 0;

	return(0);
}
//[*]------------------------------------------------------------------------------------------[*]
/* Function to update the state of a joystick - called as a device poll.
 * This function shouldn't update the joystick structure directly,
 * but instead should call SDL_PrivateJoystick*() to deliver events
 * and update joystick device state.
 */
void SDL_SYS_JoystickUpdate(SDL_Joystick *joystick) //maks
{		
	static Uint32 lastCheck = 0;
	
	if(SDL_GetSystemTicks() - lastCheck > 15) //avoid time consume loops
	{
		int ret=0;
		unsigned long buff=0,prev_buttons=0, changed=0,buttons=0;

		ret = read(joystick->hwdata->fd, &buff, 4); 

		prev_buttons 	= joystick->hwdata->prev_buttons;
		changed 		= buff^prev_buttons; 

		if(changed)
		{
			//In GP2X Joystick behave like a keyboard, so, send keyboard messages
			SDL_keysym keysym;

			keysym.scancode = 0;		
			keysym.mod = 0;
			keysym.unicode = 0;
			
			if(changed & GPX_VK_UP			) {keysym.sym = SDLK_GP2X_BUTTON_UP;			SDL_PrivateKeyboard((buff & GPX_VK_UP)				? SDL_PRESSED : SDL_RELEASED, &keysym);}
			if(changed & GPX_VK_UP_LEFT		) {keysym.sym = SDLK_GP2X_BUTTON_UPLEFT;		SDL_PrivateKeyboard((buff & GPX_VK_UP_LEFT)			? SDL_PRESSED : SDL_RELEASED, &keysym);}
			if(changed & GPX_VK_LEFT		) {keysym.sym = SDLK_GP2X_BUTTON_LEFT;			SDL_PrivateKeyboard((buff & GPX_VK_LEFT)			? SDL_PRESSED : SDL_RELEASED, &keysym);}
			if(changed & GPX_VK_DOWN_LEFT	) {keysym.sym = SDLK_GP2X_BUTTON_DOWNLEFT;		SDL_PrivateKeyboard((buff & GPX_VK_DOWN_LEFT)		? SDL_PRESSED : SDL_RELEASED, &keysym);}
			if(changed & GPX_VK_DOWN		) {keysym.sym = SDLK_GP2X_BUTTON_DOWN;			SDL_PrivateKeyboard((buff & GPX_VK_DOWN)			? SDL_PRESSED : SDL_RELEASED, &keysym);}
			if(changed & GPX_VK_DOWN_RIGHT	) {keysym.sym = SDLK_GP2X_BUTTON_DOWNRIGHT;		SDL_PrivateKeyboard((buff & GPX_VK_DOWN_RIGHT)		? SDL_PRESSED : SDL_RELEASED, &keysym);}
			if(changed & GPX_VK_RIGHT		) {keysym.sym = SDLK_GP2X_BUTTON_RIGHT;			SDL_PrivateKeyboard((buff & GPX_VK_RIGHT)			? SDL_PRESSED : SDL_RELEASED, &keysym);}
			if(changed & GPX_VK_UP_RIGHT	) {keysym.sym = SDLK_GP2X_BUTTON_UPRIGHT;		SDL_PrivateKeyboard((buff & GPX_VK_UP_RIGHT)		? SDL_PRESSED : SDL_RELEASED, &keysym);}
			
			if(changed & GPX_VK_START		) {keysym.sym = SDLK_GP2X_BUTTON_START;			SDL_PrivateKeyboard((buff & GPX_VK_START)			? SDL_PRESSED : SDL_RELEASED, &keysym);}
			if(changed & GPX_VK_SELECT		) {keysym.sym = SDLK_GP2X_BUTTON_SELECT;		SDL_PrivateKeyboard((buff & GPX_VK_SELECT)			? SDL_PRESSED : SDL_RELEASED, &keysym);}
			if(changed & GPX_VK_FL			) {keysym.sym = SDLK_GP2X_BUTTON_L;				SDL_PrivateKeyboard((buff & GPX_VK_FL)				? SDL_PRESSED : SDL_RELEASED, &keysym);}
			if(changed & GPX_VK_FR			) {keysym.sym = SDLK_GP2X_BUTTON_R;				SDL_PrivateKeyboard((buff & GPX_VK_FR)				? SDL_PRESSED : SDL_RELEASED, &keysym);}
			if(changed & GPX_VK_FA			) {keysym.sym = SDLK_GP2X_BUTTON_A;				SDL_PrivateKeyboard((buff & GPX_VK_FA)				? SDL_PRESSED : SDL_RELEASED, &keysym);}
			if(changed & GPX_VK_FB			) {keysym.sym = SDLK_GP2X_BUTTON_B;				SDL_PrivateKeyboard((buff & GPX_VK_FB)				? SDL_PRESSED : SDL_RELEASED, &keysym);}
			if(changed & GPX_VK_FX			) {keysym.sym = SDLK_GP2X_BUTTON_X;				SDL_PrivateKeyboard((buff & GPX_VK_FX)				? SDL_PRESSED : SDL_RELEASED, &keysym);}
			if(changed & GPX_VK_FY			) {keysym.sym = SDLK_GP2X_BUTTON_Y;				SDL_PrivateKeyboard((buff & GPX_VK_FY)				? SDL_PRESSED : SDL_RELEASED, &keysym);}
			
			if(changed & GPX_VK_VOL_UP		) {keysym.sym = SDLK_GP2X_BUTTON_VOLUP;			SDL_PrivateKeyboard((buff & GPX_VK_VOL_UP)			? SDL_PRESSED : SDL_RELEASED, &keysym);}
			if(changed & GPX_VK_VOL_DOWN	) {keysym.sym = SDLK_GP2X_BUTTON_VOLDOWN;		SDL_PrivateKeyboard((buff & GPX_VK_VOL_DOWN)		? SDL_PRESSED : SDL_RELEASED, &keysym);}
			if(changed & GPX_VK_TAT			) {keysym.sym = SDLK_GP2X_BUTTON_CLICK;			SDL_PrivateKeyboard((buff & GPX_VK_TAT)				? SDL_PRESSED : SDL_RELEASED, &keysym);}
				
			/*if(changed & GPX_VK_UP			) SDL_PrivateJoystickButton(joystick, GP2X_BUTTON_UP, 
														(buff & GPX_VK_UP			) ? SDL_PRESSED : SDL_RELEASED);
			if(changed & GPX_VK_UP_LEFT		) SDL_PrivateJoystickButton(joystick, GP2X_BUTTON_UPLEFT, 
														(buff & GPX_VK_UP_LEFT		) ? SDL_PRESSED : SDL_RELEASED);
			if(changed & GPX_VK_LEFT		) SDL_PrivateJoystickButton(joystick, GP2X_BUTTON_LEFT, 
														(buff & GPX_VK_LEFT			) ? SDL_PRESSED : SDL_RELEASED);
			if(changed & GPX_VK_DOWN_LEFT	) SDL_PrivateJoystickButton(joystick, GP2X_BUTTON_DOWNLEFT, 
														(buff & GPX_VK_DOWN_LEFT	) ? SDL_PRESSED : SDL_RELEASED);
			if(changed & GPX_VK_DOWN		) SDL_PrivateJoystickButton(joystick, GP2X_BUTTON_DOWN, 
														(buff & GPX_VK_DOWN		) ? SDL_PRESSED : SDL_RELEASED);
			if(changed & GPX_VK_DOWN_RIGHT	) SDL_PrivateJoystickButton(joystick, GP2X_BUTTON_DOWNRIGHT, 
														(buff & GPX_VK_DOWN_RIGHT	) ? SDL_PRESSED : SDL_RELEASED);
			if(changed & GPX_VK_RIGHT		) SDL_PrivateJoystickButton(joystick, GP2X_BUTTON_RIGHT, 
														(buff & GPX_VK_RIGHT		) ? SDL_PRESSED : SDL_RELEASED);
			if(changed & GPX_VK_UP_RIGHT	) SDL_PrivateJoystickButton(joystick, GP2X_BUTTON_UPRIGHT, 
														(buff & GPX_VK_UP_RIGHT	) ? SDL_PRESSED : SDL_RELEASED);
			
			if(changed & GPX_VK_START		) SDL_PrivateJoystickButton(joystick, GP2X_BUTTON_START, 
														(buff & GPX_VK_START		) ? SDL_PRESSED : SDL_RELEASED);
			if(changed & GPX_VK_SELECT		) SDL_PrivateJoystickButton(joystick, GP2X_BUTTON_SELECT, 
														(buff & GPX_VK_SELECT		) ? SDL_PRESSED : SDL_RELEASED);
			if(changed & GPX_VK_FL			) SDL_PrivateJoystickButton(joystick, GP2X_BUTTON_L, 
														(buff & GPX_VK_FL			) ? SDL_PRESSED : SDL_RELEASED);
			if(changed & GPX_VK_FR			) SDL_PrivateJoystickButton(joystick, GP2X_BUTTON_R, 
														(buff & GPX_VK_FR			) ? SDL_PRESSED : SDL_RELEASED);
			if(changed & GPX_VK_FA			) SDL_PrivateJoystickButton(joystick, GP2X_BUTTON_A, 
														(buff & GPX_VK_FA			) ? SDL_PRESSED : SDL_RELEASED);
			if(changed & GPX_VK_FB			) SDL_PrivateJoystickButton(joystick, GP2X_BUTTON_B, 
														(buff & GPX_VK_FB			) ? SDL_PRESSED : SDL_RELEASED);
			if(changed & GPX_VK_FX			) SDL_PrivateJoystickButton(joystick, GP2X_BUTTON_X, 
														(buff & GPX_VK_FX			) ? SDL_PRESSED : SDL_RELEASED);
			if(changed & GPX_VK_FY			) SDL_PrivateJoystickButton(joystick, GP2X_BUTTON_Y, 
														(buff & GPX_VK_FY			) ? SDL_PRESSED : SDL_RELEASED);
			
			if(changed & GPX_VK_VOL_UP		) SDL_PrivateJoystickButton(joystick, GP2X_BUTTON_VOLUP, 
														(buff & GPX_VK_VOL_UP		) ? SDL_PRESSED : SDL_RELEASED);
			if(changed & GPX_VK_VOL_DOWN	) SDL_PrivateJoystickButton(joystick, GP2X_BUTTON_VOLDOWN, 
														(buff & GPX_VK_VOL_DOWN		) ? SDL_PRESSED : SDL_RELEASED);
			if(changed & GPX_VK_TAT			) SDL_PrivateJoystickButton(joystick, GP2X_BUTTON_CLICK, 
		
													(buff & GPX_VK_TAT			) ? SDL_PRESSED : SDL_RELEASED);*/
			joystick->hwdata->prev_buttons = buff;
		}	

		lastCheck = SDL_GetSystemTicks();
	}	
}
//[*]------------------------------------------------------------------------------------------[*]
/* Function to close a joystick after use */
void SDL_SYS_JoystickClose(SDL_Joystick *joystick)
{
	if (joystick->hwdata != NULL) 
	{
		close(joystick->hwdata->fd);
		// free system specific hardware data
		free(joystick->hwdata);
	}
}
//[*]------------------------------------------------------------------------------------------[*]
/* Function to perform any system-specific joystick related cleanup */
void SDL_SYS_JoystickQuit(void)
{
	return;
}
//[*]------------------------------------------------------------------------------------------[*]

