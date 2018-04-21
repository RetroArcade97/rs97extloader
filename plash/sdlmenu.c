#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

const char default_splash[] = "/usr/share/splash/splash.jpg";
const char default_font[] = "font.ttf";


int main(int argc, char* argv[])
{
  const char *splash_img = default_splash;
  const char *font = default_font;
  TTF_Font *ttf = NULL;
  SDL_Event event;
  int i, nopts, copt = 0;
  char **options;
  Uint32 timeout=0;
  SDL_Surface *screen, *layer, *png, *msgbmp;

  options = (char **)malloc(sizeof(char *)*argc);//
  if (options == NULL) {
    perror("malloc");
    exit(__LINE__);
  }
  nopts = 0;
  for (i=1;i <argc;i++) {
    if (argv[i][0] == '-') {
      switch (argv[i][1]) {
      case 'c':
	copt = atoi(argv[i]+2);
	break;
      case 'f':
	splash_img = argv[i][2] == '=' ? argv[i]+3 : argv[i]+2;
	break;
      case 'F':
	font = argv[i][2] == '=' ? argv[i]+3 : argv[i]+2;
	break;
      case 't':
	timeout = atoi(argv[i]+2);
	break;
      default:
	fprintf(stderr,"Ignoring option: %s\n",argv[i]);
      }
    } else {
      options[nopts++] = argv[i];
    }
  }
  options[nopts] = NULL;
  if (!nopts) {
    fputs("No menu options defined!\n",stderr);
    exit(__LINE__);
  }
  if(SDL_Init(SDL_INIT_VIDEO) != 0){
    fprintf(stderr,"%s, failed to SDL_Init\n", __func__);
    exit(__LINE__);
  }
  SDL_ShowCursor(0);
  if (timeout) timeout += SDL_GetTicks();
 
  screen = SDL_SetVideoMode(320, 480, 16, SDL_HWSURFACE);
  layer = SDL_CreateRGBSurface(SDL_SWSURFACE, 320, 240, 16, 0, 0, 0, 0);
  if(screen == NULL){
    fprintf(stderr,"%s, failed to SDL_SetVideMode\n", __func__);
    exit(__LINE__);
  }
  if(layer == NULL){
    fprintf(stderr,"%s, failed to SDL_CreateRGBSurface\n", __func__);
    exit(__LINE__);
  }
  if (TTF_Init() == -1) {
    perror("TTF_Init");
    exit(__LINE__);
  }
  png = IMG_Load(splash_img);
  if(png == NULL){
    fprintf(stderr,"%s, failed to IMG_Load\n", splash_img);
    exit(__LINE__);
  }
  ttf = TTF_OpenFont(font,14);
  if (!ttf) {
    fprintf(stderr,"Unable to load font: %s\n",font);
    exit(__LINE__);
  }
  
  for (;;) {
    SDL_BlitSurface(png, NULL, layer, NULL);
    for (i=0;i<nopts;i++) {
      static SDL_Color textColor = { 255,255,255 };
      static SDL_Color selColor = {0, 255, 0};
      msgbmp = TTF_RenderText_Solid(ttf, options[i], i == copt ? selColor : textColor);
      if (msgbmp) {
	SDL_Rect offset;
	offset.x = 10;
	offset.y= 5 + i*15;
	SDL_BlitSurface(msgbmp,NULL,layer,&offset);
	SDL_FreeSurface(msgbmp);
      }
    }
    // 
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
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
      case SDL_KEYDOWN:
	switch (event.key.keysym.sym) {
	case SDLK_UP:
	  copt = (copt == 0 ? nopts : copt)-1;
	  goto next;
	case SDLK_ESCAPE:	// btn_select
	case SDLK_DOWN:
	  copt = (copt + 1)%nopts;
	  goto next;
	case SDLK_LEFT:
	  fputs("key_left\n",stderr);
	  break;
	case SDLK_RIGHT:
	  fputs("key_right\n",stderr);
	  break;
	case SDLK_RETURN:	// btn_start
	  printf("START %d %s\n", copt, options[copt]);
	  goto end;
	case SDLK_LCTRL:	// btn_a
	  printf("BTN_A %d %s\n", copt, options[copt]);
	  goto end;
	case SDLK_LALT:		// btn_b
	  printf("BTN_B %d %s\n", copt, options[copt]);
	  goto end;
	case SDLK_SPACE:	// btn_x	
	  printf("BTN_X %d %s\n", copt, options[copt]);
	  goto end;
	case SDLK_LSHIFT:	// btn_y	
	  printf("BTN_Y %d %s\n", copt, options[copt]);
	  goto end;
	case SDLK_TAB:		// Left_Shoulder
	  printf("LEFT %d %s\n", copt, options[copt]);
	  goto end;
	case SDLK_BACKSPACE:	// Right Shoulder
	  printf("RIGHT %d %s\n", copt, options[copt]);
	  goto end;
	case SDLK_3:		// btn_backlight	
	  printf("BACKLIGHT %d %s\n", copt, options[copt]);
	  goto end;
	case SDLK_END:		// btn_power
	  printf("POWER %d %s\n", copt, options[copt]);
	  goto end;
	default: 
	  fprintf(stderr,"%d\n",event.key.keysym.sym);
	}
	break;
      case SDL_QUIT:
	goto end;
	break;
      }
      //fputs("bottom of inner loop\n",stderr);
    }
next:
    //fprintf(stderr,"Timeout; %d/%d %d\n",SDL_GetTicks(),timeout,timeout && SDL_GetTicks() > timeout);
    if (timeout && SDL_GetTicks() > timeout) {
      puts("TIMEOUT");
      break;
    }
    SDL_Delay(100);
  }
end:
  SDL_FreeSurface(png);
  SDL_FreeSurface(layer);
  SDL_Quit();
  return 0;    
}

