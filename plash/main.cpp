#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

const char default_splash[] = "/usr/share/splash/splash.jpg";
const char default_font[] = "font.ttf";


int main(int argc, char* argv[])
{
  int delay = 1500;
  const char *splash_img = default_splash;
  const char *font = default_font;
  const char *txt = NULL;
  bool quit = false;
  bool instream = false;
  Uint32 start;
  TTF_Font *ttf = NULL;
  //The event structure that will be used
  SDL_Event event;

  {
    int i;
    for (i=1;i <argc;i++) {
      if (argv[i][0] == '-') {
        switch (argv[i][1]) {
	case 'd':
	  delay = atoi(argv[i]+2);
	  break;
	case 'D':
	  delay = -atoi(argv[i]+2);
	  break;
	case 'f':
	  splash_img = argv[i][2] == '=' ? argv[i]+3 : argv[i]+2;
	  break;
	case 'm':
	  txt = argv[i]+2;
	  instream = false;
	  break;
	case 'F':
	  font = argv[i][2] == '=' ? argv[i]+3 : argv[i]+2;
	  break;
	case 'i':
	  instream = true;
	  txt = NULL;
	  break;
	default:
	  fprintf(stderr,"Ignoring option: %s\n",argv[i]);
        }
      } else {
	fprintf(stderr,"Ignoring argument: %s\n", argv[i]);
      }
    }
  }
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
  //Initialize SDL_ttf
  if((txt || instream) &&  TTF_Init() == -1 )
  {
        return false;    
  }
 
 
  SDL_Surface* png = IMG_Load(splash_img);
  FILE *fp = NULL;
  if(png == NULL){
    printf("%s, failed to IMG_Load\n", __func__);
    return -1;
  }
  if (txt || instream){
    ttf = TTF_OpenFont(font,14);
    if (!ttf) fprintf(stderr,"Unable to load font: %s\n",font);
    if (instream) {
      fp = fdopen(0,"r");
      if (fp == NULL) {
        perror("fdopen");
	exit(1);
      }
    } 
  }
  
  
  char buffer[1024]; buffer[(sizeof buffer)-1] = 0; // NUL terminated...
  for (;;) {
    SDL_BlitSurface(png, NULL, layer, NULL);
    if (txt) {
      static SDL_Color textColor = { 255,255,255 };
      SDL_Surface *msg;
      if (ttf) {
	fprintf(stderr,"Rendering: %s\n",txt);
        msg = TTF_RenderText_Solid(ttf, txt, textColor);
        fprintf(stderr,"MSG: %llx\n",(long long)msg);
        if (msg) {
	  SDL_Rect offset;
	  offset.x = 10;
	  offset.y = 30;
	  SDL_BlitSurface(msg,NULL,layer,&offset);
	  SDL_FreeSurface(msg);
        }
      }
    }
    int x, y;
    uint32_t *s = (uint32_t*)layer->pixels;
    uint32_t *d = (uint32_t*)screen->pixels;
    for(y=0; y<240; y++){
      for(x=0; x<160; x++){
        *d++ = *s++;
      }
      d+= 160;
    }
    fprintf(stderr,"Ready to flip\n");
    SDL_Flip(screen);
    fprintf(stderr,"FLIPPED\n");
    if (instream) {
      fprintf(stderr,"fgets: %d\n",(sizeof buffer)-1);
      txt = fgets(buffer,(sizeof buffer)-1,fp);
      if (txt) {
	fprintf(stderr,"READ: %s\n",txt);
	int ln = strlen(txt);
	if (ln && txt[ln-1] == '\n') buffer[ln-1] = 0; // Remove EOL...
      } else {
        fprintf(stderr,"EOF\n");
	break;
      }
    } else {
      fprintf(stderr,"Leaving loop\n");
      break;
    }
  }
  if (delay > 0) {
    fprintf(stderr,"Wait for %d\n",delay);
    SDL_Delay(delay);
  } else {
    fprintf(stderr,"Entering event loop: %d\n",quit);
    start = SDL_GetTicks();
    while (!quit) {
      while (SDL_PollEvent(&event)) {
        switch (event.type) {
	case SDL_KEYDOWN:
	  switch (event.key.keysym.sym) {
	  case SDLK_UP:
	    puts("key_up");
	    break;
	  case SDLK_DOWN:
	    puts("key_down");
	    break;
	  case SDLK_LEFT:
	    puts("key_left");
	    break;
	  case SDLK_RIGHT:
	    puts("key_right");
	    break;
	  case SDLK_SPACE:
	    puts("key_space modify btn_x");
	    break;
	  case SDLK_LCTRL:
	    puts("key_lctrl confirm btn_a");
	    break;
	  case SDLK_LALT:
	    puts("key_lalt cancel btn_b");
	    break;
	  case SDLK_LSHIFT:
	    puts("key_lshift manual btn_y");
	    break;
	  case SDLK_TAB:
	    puts("key_tab prev left_shoulder");
	    break;
	  case SDLK_BACKSPACE:
	    puts("key_backspace next right_shoulder");
	    break;
	  case SDLK_RETURN:
	    puts("key_return settings start");
	    break;
	  case SDLK_ESCAPE:
	    puts("key_esc menu select");
	    break;
	  case SDLK_3:
	    puts("key_3 backlight");
	    break;
	  case SDLK_END:
	    puts("key_end power");
	    break;
	  default: 
	    printf("%d\n",event.key.keysym.sym);
          }
	case SDL_QUIT:
	  quit = true;
	  break;
	}
      }
      if (delay < 0) {
	//fprintf(stderr,"Timer: %d (%d)\n",SDL_GetTicks() - start,-delay);
	if (SDL_GetTicks() - start > -delay) quit = true;
      }

    }
  }

  SDL_FreeSurface(png);
  SDL_FreeSurface(layer);
  SDL_Quit();
  return 0;    
}

