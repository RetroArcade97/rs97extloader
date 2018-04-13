#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_image.h>
 
int main(int argc, char* args[])
{
  if(SDL_Init(SDL_INIT_VIDEO) != 0){
    printf("%s, failed to SDL_Init\n", __func__);
    return -1;
  }
  SDL_ShowCursor(0);
 
  SDL_Surface* screen;
  SDL_Surface* layer;
  screen = SDL_SetVideoMode(320, 480, 16, SDL_HWSURFACE);
  layer = SDL_CreateRGBSurface(SDL_SWSURFACE, 320, 240, 16, 0, 0, 0, 0);
  if(screen == NULL){
    printf("%s, failed to SDL_SetVideMode\n", __func__);
    return -1;
  }
  if(layer == NULL){
    printf("%s, failed to SDL_CreateRGBSurface\n", __func__);
    return -1;
  }
 
 
  SDL_Surface* png = IMG_Load("/usr/share/splash/splash.jpg");
  if(png == NULL){
    printf("%s, failed to IMG_Load\n", __func__);
    return -1;
  }

  SDL_BlitSurface(png, NULL, layer, NULL);
  int x, y;
  uint32_t *s = (uint32_t*)layer->pixels;
  uint32_t *d = (uint32_t*)screen->pixels;
  for(y=0; y<240; y++){
    for(x=0; x<160; x++){
      *d++ = *s++;
    }
    d+= 160;
  }
  SDL_Flip(screen);
  SDL_Delay(1500);
  SDL_FreeSurface(png);
  SDL_FreeSurface(layer);
  SDL_Quit();
  return 0;    
}

