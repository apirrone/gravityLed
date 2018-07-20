#include <iostream>
#include <stdlib.h>
#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>

#define W 8
#define H 16
#define GRAN 100 // granularité simu
#define NB_BALLS 2
typedef struct vec{
  float x, y;
} vec;

class Ball{
public :
  float x, y;  //  m
  float mass;  //  kgs
  vec speed; //  m/s
  
  Ball(float x, float y, float mass){
    this->x = x;
    this->y = y;
    this->mass = mass;
    
    this->speed.x = 0;
    this->speed.y = 0;
  }
  
  void applyForce(vec f){
    speed.x = GRAN*f.x*mass;
    speed.y = GRAN*f.y*mass;
  }

  void tick(bool** occupancy){

    int gridX;
    int gridY;
    if((x > 0 && x < (W-1)*GRAN))
      gridX = (int)((x+speed.x)/(GRAN*1.));
    else
      gridX = (int)((x)/(GRAN*1.));


    if((y > 0 && y < (H-1)*GRAN))
      gridY = (int)((y+speed.y)/(GRAN*1.));
    else
      gridY = (int)((y)/(GRAN*1.));
      

    if( (x > 0 && x < (W-1)*GRAN) && !occupancy[gridX][gridY]){
      x += speed.x;
    }

    if( (y > 0 && y < (H-1)*GRAN) && !occupancy[gridX][gridY]){
      y += speed.y;
    }
    
    // std::cout << "x : "  << x << std::endl;
    // std::cout << "y : "  << y << std::endl;
    
  }
  
};

void updateOccupancy(bool** occupancy, Ball* balls){

  for(int i = 0 ; i < W ; i++)
    for(int j = 0 ; j < H ; j++)
      occupancy[i][j] = false;
  
  for(int i = 0 ; i < NB_BALLS ; i++){

      int gridX = (int)((balls[i].x)/(GRAN*1.));
      int gridY = (int)((balls[i].y)/(GRAN*1.));

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
  for(int i = 0 ; i < NB_BALLS ; i++){
    balls[i] = Ball((i+1)*GRAN, (i+1)*GRAN, 1);// TODO bien faire init avec NB_BALLS plus grand que W
  }

  vec v;
  v.x = 0;
  v.y = 0;
  for(int i = 0 ; i < NB_BALLS ; i++){
    balls[i].applyForce(v);
  }
  
  WINDOW* w = initscr();
  cbreak();
  nodelay(w, TRUE);
  keypad(stdscr, TRUE);

  start_color();

  init_pair(1, COLOR_WHITE, COLOR_WHITE);//background
  init_pair(2, COLOR_RED, COLOR_RED);//led
  
  bool updateForces = false;
  int c;
  while(true){

    c = wgetch(w);
    if(c == KEY_UP){
      v.y = -1;
      updateForces = true;
    }
    if(c == KEY_DOWN){
      v.y = 1;
      updateForces = true;
    }
    if(c == KEY_LEFT){
      v.x = -1;
      updateForces = true;
    }
    if(c == KEY_RIGHT){
      v.x = 1;
      updateForces = true;
    }

    if(updateForces){
      updateForces = false;
      for(int i = 0 ; i < NB_BALLS ; i++){
	balls[i].applyForce(v);
      }      
    }

    for(int i = 0 ; i < NB_BALLS ; i++){
      balls[i].tick(occupancy);
    }
    
    updateOccupancy(occupancy, balls);
    

    clear();
    display(occupancy);
    usleep(100000);
  }

  endwin();

  

  return EXIT_SUCCESS;
}
