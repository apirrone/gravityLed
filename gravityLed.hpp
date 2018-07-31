#include <iostream>
#include <stdlib.h>
#include <stdlib.h>
#include <math.h>

#define W 5
#define H 8
#define GRAN 100 // granularité simu (précision)
#define NB_BALLS 1
#define ATTENUATION 0.6

enum directions {UP=0, DOWN=1, RIGHT=2, LEFT=3};

// typedef struct vec{
//   float x, y;
//   float dist(vec v1, vec v2){
//     return 0.;
//   }
// } vec;

class vec{
public:
  float x;
  float y;
  float dist(vec v){
    return sqrt( ((this->x - v.x)*(this->x - v.x)) + ((this->y - v.y)*(this->y - v.y)));
  }
};

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
  
  void tick(int** occupancy, Ball* balls, int myIndex, float x, float y){
    
    this->speed.x += x;
    this->speed.y += y;

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
  
  void step(float x, float y){
    for(int i = 0 ; i < NB_BALLS ; i++)
      balls[i].tick(occupancy, balls, i, x, y);

    updateOccupancy(occupancy, balls);
  }

  int** getMatrix(){
    return occupancy;
  }

  void updateForceDir(vec d){
    for(int i = 0 ; i < NB_BALLS ; i++)
      balls[i].updateForceDir(d);
  }


  private:

  Ball* balls;  
  int** occupancy;

  void init(){

    occupancy = (int**)malloc(H*sizeof(int*));
    for(int i = 0 ; i < H ; i++){
      occupancy[i] = (int*)malloc(W*sizeof(int));
      for(int j = 0 ; j < W ; j++)
	occupancy[i][j] = 0;
    }

    balls = (Ball*)malloc(NB_BALLS*sizeof(Ball));

    for(int i = 0 ; i < NB_BALLS ; i++)
      balls[i] = Ball(((i%W)*GRAN), (i/W)*GRAN, 0.9*(i+1));
    
  }

  void updateOccupancy(int** occupancy, Ball* balls){

    for(int i = 0 ; i < W ; i++)
      for(int j = 0 ; j < H ; j++)
	occupancy[i][j] = 0;
  
    for(int i = 0 ; i < NB_BALLS ; i++){
      int gridX = (int)((balls[i].pos.x)/(GRAN*1.));
      int gridY = (int)((balls[i].pos.y)/(GRAN*1.));

      vec ledCenter;
      ledCenter.x = (gridX*GRAN+(GRAN/2));
      ledCenter.y = (gridY*GRAN+(GRAN/2));

      vec centeredBallPos;
      centeredBallPos.x = balls[i].pos.x + (GRAN/2);
      centeredBallPos.y = balls[i].pos.y + (GRAN/2);
      
      float d = ledCenter.dist(centeredBallPos)/(GRAN*1.);
      occupancy[gridX][gridY] = (int)(d*255);   
    }    
  }

};
