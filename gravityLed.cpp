#include <iostream>
#include <stdlib.h>
#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>

#define W 8
#define H 16
#define GRAN 100 // granularité simu
#define NB_BALLS 15

enum directions {UP=0, DOWN=1, RIGHT=2, LEFT=3};

typedef struct vec{
  float x, y;
} vec;

class Ball{
public :
  vec pos;
  float mass;  //  kgs
  vec speed;
  
  Ball(float x, float y, float mass){
    this->pos.x = x;
    this->pos.y = y;
    this->mass = mass;
    this->speed.x = 0.;
    this->speed.y = 0.;
  }

  void move(int dir){
    switch(dir){
    case UP : 
      
      break;
    case DOWN : 

      break;
    case LEFT : 

      break;
    case RIGHT : 

      break;
    }
  }

  bool inBounds(float val, float low, float up){
    return (val > low && val < up);
  }

  void updateForceDir(vec dir){

    if(inBounds(this->speed.x + dir.x, -50, 50))
      this->speed.x += dir.x;

    if(inBounds(this->speed.y + dir.y, -50, 50))
      this->speed.y += dir.y;

  }

  
  void tick(bool** occupancy){

    vec g;
    g.x = 0.;
    g.y = 0.2;

    this->speed.x += g.x;
    this->speed.y += g.y;

    if(this->pos.x + this->speed.x >= W*GRAN || this->pos.x + this->speed.x <= 0)
      this->speed.x = -this->speed.x*0.8;    

    if(this->pos.y + this->speed.y >= H*GRAN || this->pos.y + this->speed.y <= 0)
      this->speed.y = -this->speed.y*0.8;


    this->pos.x += this->speed.x*mass;
    this->pos.y += this->speed.y*mass;



    // int gridXNext = (int)((this->pos.x+this->speed.x)/(GRAN*1.));
    // int gridYNext = (int)((this->pos.y+this->speed.y)/(GRAN*1.));

    // int gridX = (int)((this->pos.x)/(GRAN*1.));
    // int gridY = (int)((this->pos.y)/(GRAN*1.));

    // std::cout << "GridX : " << gridX << std::endl;
    // std::cout << "GridY : " << gridY << std::endl;

    // std::cout << "GridXNext : " << gridXNext << std::endl;
    // std::cout << "GridYNext : " << gridYNext << std::endl;

    // if(inBounds(this->pos.x+this->speed.x, 0, (W)*GRAN)){
    //   if(gridXNext == gridX)
    // 	this->pos.x += this->speed.x;
    //   else
    // 	if(!occupancy[gridXNext][gridY])
    // 	  this->pos.x += this->speed.x;
    // }

    // if(inBounds(this->pos.y+this->speed.y, 0, (H)*GRAN)){
    //   if(gridYNext == gridY)
    // 	this->pos.y += this->speed.y;
    //   else
    // 	if(!occupancy[gridYNext][gridY])
    // 	  this->pos.y += this->speed.y;
    // }

    // std::cout << "pos x : "  << this->pos.x << std::endl;
    // std::cout << "pos y : "  << this->pos.y << std::endl;
    
  }
  
};

void updateOccupancy(bool** occupancy, Ball* balls){

  for(int i = 0 ; i < W ; i++)
    for(int j = 0 ; j < H ; j++)
      occupancy[i][j] = false;
  
  for(int i = 0 ; i < NB_BALLS ; i++){

      int gridX = (int)((balls[i].pos.x)/(GRAN*1.));
      int gridY = (int)((balls[i].pos.y)/(GRAN*1.));

      occupancy[gridX][gridY] = true;   
  }    

}

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

  bool** occupancy;
  occupancy = (bool**)malloc(H*sizeof(bool*));
  for(int i = 0 ; i < H ; i++){
    occupancy[i] = (bool*)malloc(W*sizeof(bool));
    for(int j = 0 ; j < W ; j++)
      occupancy[i][j] = false;
  }

  Ball* balls;
  balls = (Ball*)malloc(NB_BALLS*sizeof(Ball));

  for(int i = 0 ; i < NB_BALLS ; i++)
    balls[i] = Ball(((i%W)*GRAN), (i/W)*GRAN, 0.5*(i+1));


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

    c = wgetch(w);
    if(c == KEY_UP){
      
      v.x =  0;
      v.y = -10;

      for(int i = 0 ; i < NB_BALLS ; i++)
      	balls[i].updateForceDir(v);
    }
    if(c == KEY_DOWN){
      v.x =  0;
      v.y = 10;

      for(int i = 0 ; i < NB_BALLS ; i++)
      	balls[i].updateForceDir(v);
    }
    if(c == KEY_LEFT){
      v.x =  -10;
      v.y = 0;

      for(int i = 0 ; i < NB_BALLS ; i++)
      	balls[i].updateForceDir(v);
    }
    if(c == KEY_RIGHT){
      v.x =  10;
      v.y = 0;

      for(int i = 0 ; i < NB_BALLS ; i++)
      	balls[i].updateForceDir(v);
    }

    for(int i = 0 ; i < NB_BALLS ; i++){
      balls[i].tick(occupancy);
    }
    
    updateOccupancy(occupancy, balls);
    

    clear();
    display(occupancy);
    usleep(10000);
  }

  endwin();

  

  return EXIT_SUCCESS;
}
