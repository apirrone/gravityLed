#include "gravityLed.hpp"
#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>

void display(bool** tab){

  for(int j = 0 ; j < H ; j++){
    for(int i = 0 ; i < W ; i++){
      
      if(tab[i][j]){
	attron(COLOR_PAIR(2));	
	printw(" ");//LED
      }
      else{
	attron(COLOR_PAIR(1));
	printw(" ");//background
      }
      
      addch(' ');      
    }
    addch('\n');
  }
}

int main(){
  gravityLed g;

  WINDOW* w = initscr();
  cbreak();
  nodelay(w, TRUE);
  keypad(stdscr, TRUE);

  start_color();

  init_pair(1, COLOR_WHITE, COLOR_WHITE);//background
  init_pair(2, COLOR_RED, COLOR_RED);//led
  init_pair(3, COLOR_BLACK, COLOR_RED);//txt

  vec gr;
  gr.x = 0.;
  gr.y = 0.;
  
  int c;
  while(true){
    
    g.step(gr.x, gr.y);
    bool** occupancy = g.getMatrix();
    
    c = wgetch(w);
    if(c == KEY_UP)
      gr.y -= 0.1;
    if(c == KEY_DOWN)
      gr.y += 0.1;
    if(c == KEY_LEFT)
      gr.x -= 0.1;
    if(c == KEY_RIGHT)
      gr.x += 0.1;
    
    clear();
    display(occupancy);
    attron(COLOR_PAIR(3));	      
    move(W, H);
    printw("g.x : %f\n", gr.x);
    move(W+1, H);
    printw("g.y : %f", gr.y);    
    usleep(10000);
  }

  endwin();

  return EXIT_SUCCESS;
}

// int main(){
//   gravityLed g;

//   vec v;
//   v.x = 0;
//   v.y = 0;

//   WINDOW* w = initscr();
//   cbreak();
//   nodelay(w, TRUE);
//   keypad(stdscr, TRUE);

//   start_color();

//   init_pair(1, COLOR_WHITE, COLOR_WHITE);//background
//   init_pair(2, COLOR_RED, COLOR_RED);//led
  
//   int c;
//   while(true){

//     g.step(0., 0.2);
//     bool** occupancy = g.getMatrix();
    
//     c = wgetch(w);
//     if(c == KEY_UP){
//       v.x =  0;
//       v.y = -2;
//       g.updateForceDir(v);
//     }
//     if(c == KEY_DOWN){
//       v.x =  0;
//       v.y = 2;
//       g.updateForceDir(v);
//     }
//     if(c == KEY_LEFT){
//       v.x =  -2;
//       v.y = 0;
//       g.updateForceDir(v);
//     }
//     if(c == KEY_RIGHT){
//       v.x = 2;
//       v.y = 0;
//       g.updateForceDir(v);
//     }
    
//     clear();
//     display(occupancy);
    
//     usleep(10000);
//   }

//   endwin();

//   return EXIT_SUCCESS;
// }
