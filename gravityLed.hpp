#include <stdlib.h>

#define W 50
#define H 50
#define GRAN 100 // granularity (precision) of underlying simulation, currently also affects speed (TODO needs to be fixed)
#define NB_BALLS 30
#define ATTENUATION 0.8
#define MAX_SPEED 50

typedef struct vec{
  float x, y;
} vec;

class Ball{
public :
  vec pos;
  float mass;// more like gravity sensibility
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

    if(inBounds(this->speed.x + dir.x, -MAX_SPEED, MAX_SPEED))
      this->speed.x += dir.x;

    if(inBounds(this->speed.y + dir.y, -MAX_SPEED, MAX_SPEED))
      this->speed.y += dir.y;

  }
  
  void tick(bool** occupancy, Ball* balls, int myIndex, float x, float y){

    //gravity
    vec g;
    g.x = x;
    g.y = y;
    updateForceDir(g);

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

	
	if(myGridXNext == otherGridX && myGridY == otherGridY)
	  this->speed.x = -this->speed.x*ATTENUATION;

	if(myGridYNext == otherGridY && myGridX == otherGridX)
	  this->speed.y = -this->speed.y*ATTENUATION;

      }
    }


    this->pos.x += this->speed.x*mass;
    this->pos.y += this->speed.y*mass;    


    // Last check
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

    for(int i = 0 ; i < NB_BALLS ; i++){
      //balls[i] = Ball(((i%W)*GRAN), (i/W)*GRAN, 10);// init with same gravity sensibility
      balls[i] = Ball(((i%W)*GRAN), (i/W)*GRAN, 0.9*(i+1));// init with different gravity sensibility
    }
     
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
