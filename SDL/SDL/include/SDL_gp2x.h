/*
 * Non-standard SDL header.
 *
 * This header gives access to a few extra features of the GP2X hardware
 * that I added to the video driver.
 */

#ifndef SDL_GP2X__H
#define SDL_GP2X__H

#ifdef __cplusplus
extern "C" {
#endif

void SDL_GP2X_SetMonoColours(int background, int foreground);
int  SDL_GP2X_GetPhysicalScreenSize(SDL_Rect *size);
void SDL_GP2X_Display(SDL_Rect *area);

#ifdef __cplusplus
}
#endif

#endif
