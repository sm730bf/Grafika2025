#include <stdbool.h>
#include <math.h>
#include <SDL2/SDL.h>
#include "circle.h"

#define SCREEN_W 800
#define SCREEN_H 600

typedef enum { USE_SEGMENTS, USE_DTHETA, USE_MAXLEN } Mode;

Mode   drawMode = USE_SEGMENTS;
int    segments = 60;         // 1. param
double dtheta   = M_PI/30;    // 2. param
double maxlen   = 5.0;        // 3. param

#define MAX_CIRCLES 100
Circle circles[MAX_CIRCLES];
int    circleCount = 0;
int    dragIndex   = -1;
int    prevMx, prevMy;

// segéd: szegmensekkel
void drawCircle(SDL_Renderer* r, const Circle* c){
    SDL_SetRenderDrawColor(r, c->color.r, c->color.g, c->color.b, 255);
    double cx=c->x, cy=c->y, R=c->radius;
    if(drawMode==USE_SEGMENTS){
        for(int i=0;i<segments;i++){
            double t1=2*M_PI*i/segments;
            double t2=2*M_PI*(i+1)/segments;
            SDL_RenderDrawLine(r,
                cx+R*cos(t1), cy+R*sin(t1),
                cx+R*cos(t2), cy+R*sin(t2)
            );
        }
    } else if(drawMode==USE_DTHETA){
        for(double t=0;t<2*M_PI; t+=dtheta){
            double t2 = t+dtheta;
            SDL_RenderDrawLine(r,
                cx+R*cos(t), cy+R*sin(t),
                cx+R*cos(t2),cy+R*sin(t2)
            );
        }
    } else {
        double dtheta = 2*acos(1 - maxlen/R);
        for(double t=0;t<2*M_PI;t+=dtheta){
            double t2 = t+dtheta;
            SDL_RenderDrawLine(r,
                cx+R*cos(t), cy+R*sin(t),
                cx+R*cos(t2),cy+R*sin(t2)
            );
        }
    }
}

// + jel a kör közepén
void drawCross(SDL_Renderer* r, double x, double y){
  int s=10;
  SDL_RenderDrawLine(r, x-s,y, x+s,y);
  SDL_RenderDrawLine(r, x,y-s, x,y+s);
}

int main(int argc, char* argv[]){
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window*  win = SDL_CreateWindow("Circles",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,SCREEN_W,SCREEN_H,0);
    SDL_Renderer* ren=SDL_CreateRenderer(win,-1,SDL_RENDERER_ACCELERATED);

    bool quit=false; SDL_Event ev;
    while(!quit){
      while(SDL_PollEvent(&ev)){
        int mx=ev.button.x, my=ev.button.y;
        switch(ev.type){
          case SDL_QUIT: quit=true; break;
          case SDL_KEYDOWN:
            if(ev.key.keysym.sym=='1') drawMode=USE_SEGMENTS;
            if(ev.key.keysym.sym=='2') drawMode=USE_DTHETA;
            if(ev.key.keysym.sym=='3') drawMode=USE_MAXLEN;
            break;
          case SDL_MOUSEBUTTONDOWN:
            if(circleCount<MAX_CIRCLES){
              // SHIFT: új kör
              if(ev.button.button==SDL_BUTTON_LEFT && (SDL_GetModState() & KMOD_SHIFT)){
                Circle c; set_circle_data(&c,mx,my,50);
                // random color
                set_circle_color(&c, rand()%256, rand()%256, rand()%256);
                circles[circleCount++]=c;
              } else {
                // kiválasztás drag-hoz
                for(int i=0;i<circleCount;i++){
                  double d=hypot(mx-circles[i].x,my-circles[i].y);
                  if(d<circles[i].radius){ dragIndex=i; break; }
                }
              }
            }
            prevMx=mx; prevMy=my;
            break;
          case SDL_MOUSEBUTTONUP:
            dragIndex=-1; break;
          case SDL_MOUSEMOTION:
            if(dragIndex>=0){
              circles[dragIndex].x += mx-prevMx;
              circles[dragIndex].y += my-prevMy;
            }
            prevMx=mx; prevMy=my;
            break;
        }
      }

      // render
      SDL_SetRenderDrawColor(ren,255,255,255,255);
      SDL_RenderClear(ren);

      for(int i=0;i<circleCount;i++){
        drawCircle(ren,&circles[i]);
        // hover?
        int mx, my; SDL_GetMouseState(&mx,&my);
        if(hypot(mx-circles[i].x, my-circles[i].y) < circles[i].radius){
          drawCross(ren, circles[i].x, circles[i].y);
        }
      }

      SDL_RenderPresent(ren);
      SDL_Delay(16);
    }

    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}