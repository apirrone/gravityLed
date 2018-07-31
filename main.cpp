#include "gravityLed.hpp"
#include <iostream>
#include <stdlib.h>
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

  vec v;
  v.x = 0;
  v.y = 0;

  WINDOW* w = initscr();
  cbreak();
  nodelay(w, TRUE);
  keypad(stdscr, TRUE);

  start_color();

  init_pair(1, COLOR_WHITE, COLOR_WHITE);//background
  init_pair(2, COLOR_RED, COLOR_RED);//led
  
  int c;
  while(true){

    g.step(10000);
    bool** occupancy = g.getMatrix();
    
    c = wgetch(w);
    if(c == KEY_UP){
      v.x =  0;
      v.y = -2;
      g.updateForceDir(v);
    }
    if(c == KEY_DOWN){
      v.x =  0;
      v.y = 2;
      g.updateForceDir(v);
    }
    if(c == KEY_LEFT){
      v.x =  -2;
      v.y = 0;
      g.updateForceDir(v);
    }
    if(c == KEY_RIGHT){
      v.x = 2;
      v.y = 0;
      g.updateForceDir(v);
    }
    
    clear();
    display(occupancy);
  }

  endwin();

  return EXIT_SUCCESS;
}
