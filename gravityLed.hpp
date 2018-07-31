#include <iostream>
#include <stdlib.h>
#include <stdlib.h>
#include <unistd.h>

#define W 5
#define H 8
#define GRAN 100 // granularité simu (précision)
#define NB_BALLS 10
#define ATTENUATION 0.6

enum directions {UP=0, DOWN=1, RIGHT=2, LEFT=3};

typedef struct vec{
  float x, y;
} vec;

class Ball{
public :
  vec pos;
  float mass;
  vec speed;
  
  Ball(float x, float y, float mass){
    this->pos.x = x;
    this->pos.y = y;
    this->mass = mass;
    this->speed.x = 0.;
    this->speed.y = 0.;
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
  
  void tick(bool** occupancy, Ball* balls, int myIndex){

    vec g;
    g.x = 0;
    g.y = 0.2;

    this->speed.x += g.x;
    this->speed.y += g.y;

    if(this->pos.x + this->speed.x >= W*GRAN || this->pos.x + this->speed.x <= 0)
      this->speed.x = -this->speed.x*ATTENUATION;    

    if(this->pos.y + this->speed.y >= H*GRAN || this->pos.y + this->speed.y <= 0)
      this->speed.y = -this->speed.y*ATTENUATION;


    //checking collisions with other balls (exept me, duh)
    for(int i = 0 ; i < NB_BALLS ; i++){
      if(i != myIndex){
	int myGridX = (int)((this->pos.x)/(GRAN*1.));
	int myGridY = (int)((this->pos.y)/(GRAN*1.));	

	int myGridXNext = (int)((this->pos.x+this->speed.x*mass)/(GRAN*1.));
	int myGridYNext = (int)((this->pos.y+this->speed.y*mass)/(GRAN*1.));	

	int otherGridX = (int)((balls[i].pos.x)/(GRAN*1.));
	int otherGridY = (int)((balls[i].pos.y)/(GRAN*1.));	

	int otherGridXNext = (int)((balls[i].pos.x+balls[i].speed.x*balls[i].mass)/(GRAN*1.));
	int otherGridYNext = (int)((balls[i].pos.y+balls[i].speed.y*balls[i].mass)/(GRAN*1.));

	if(myGridXNext == otherGridX && myGridY == otherGridY)
	  this->speed.x = -this->speed.x*ATTENUATION;

	if(myGridYNext == otherGridY && myGridX == otherGridX)
	  this->speed.y = -this->speed.y*ATTENUATION;

      }
    }


    this->pos.x += this->speed.x*mass;
    this->pos.y += this->speed.y*mass;    


    // Garde fou
    if(this->pos.x >= W*GRAN)
      this->pos.x = W*GRAN-1;
    if(this->pos.x < 0)
      this->pos.x = 0*GRAN;

    if(this->pos.y >= H*GRAN)
      this->pos.y = H*GRAN-1;
    if(this->pos.y < 0)
      this->pos.y = 0;

  }
  
};


class gravityLed{

  public:

  gravityLed(){
    init();
  }

  ~gravityLed(){}
  
  void step(double microseconds){
    for(int i = 0 ; i < NB_BALLS ; i++)
      balls[i].tick(occupancy, balls, i);

    updateOccupancy(occupancy, balls);
    usleep(microseconds);
  }

  bool** getMatrix(){
    return occupancy;
  }

  void updateForceDir(vec d){
    for(int i = 0 ; i < NB_BALLS ; i++)
      balls[i].updateForceDir(d);
  }


  private:

  Ball* balls;  
  bool** occupancy;

  void init(){

    occupancy = (bool**)malloc(H*sizeof(bool*));
    for(int i = 0 ; i < H ; i++){
      occupancy[i] = (bool*)malloc(W*sizeof(bool));
      for(int j = 0 ; j < W ; j++)
	occupancy[i][j] = false;
    }

    balls = (Ball*)malloc(NB_BALLS*sizeof(Ball));

    for(int i = 0 ; i < NB_BALLS ; i++)
      balls[i] = Ball(((i%W)*GRAN), (i/W)*GRAN, 3*(i+1));    
    
  }

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

};
     
