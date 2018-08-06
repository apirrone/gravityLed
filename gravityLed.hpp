#include <stdlib.h>

#define GRAN 100 // granularity (precision) of underlying simulation, currently also affects speed (TODO needs to be fixed)
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
  
  void tick(bool** obstacles, Ball* balls, int nbBalls, int myIndex, float x, float y, int w, int h){

    //gravity
    vec g;
    g.x = x;
    g.y = y;
    updateForceDir(g);

    int myGridX = (int)((this->pos.x)/(GRAN*1.));
    int myGridY = (int)((this->pos.y)/(GRAN*1.));	

    int myGridXNext = (int)((this->pos.x+this->speed.x*mass)/(GRAN*1.));
    int myGridYNext = (int)((this->pos.y+this->speed.y*mass)/(GRAN*1.));	

    //Borders handling
    if(this->pos.x + this->speed.x >= w*GRAN || this->pos.x + this->speed.x <= 0)
      this->speed.x = -this->speed.x*ATTENUATION;    

    if(this->pos.y + this->speed.y >= h*GRAN || this->pos.y + this->speed.y <= 0)
      this->speed.y = -this->speed.y*ATTENUATION;

    //Obstacle handling
    if((myGridXNext < w && myGridXNext > 0) &&  obstacles[myGridXNext][myGridY])
      this->speed.x = -this->speed.x*ATTENUATION;

    if((myGridYNext < h && myGridYNext > 0) && obstacles[myGridX][myGridYNext])
      this->speed.y = -this->speed.y*ATTENUATION;

    //checking collisions with other balls (exept me, duh)
    for(int i = 0 ; i < nbBalls ; i++){
      if(i != myIndex){
	
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
    if(this->pos.x >= w*GRAN)
      this->pos.x = w*GRAN-1;
    if(this->pos.x < 0)
      this->pos.x = 0*GRAN;

    if(this->pos.y >= h*GRAN)
      this->pos.y = h*GRAN-1;
    if(this->pos.y < 0)
      this->pos.y = 0;

  }
  
};


class gravityLed{

  public:

  gravityLed(int w, int h, int nbBalls){
    this->w = w;
    this->h = h;
    this->nbBalls = nbBalls;
    
    occupancy = (bool**)malloc(h*sizeof(bool*));
    for(int i = 0 ; i < h ; i++){
      occupancy[i] = (bool*)malloc(w*sizeof(bool));
      for(int j = 0 ; j < w ; j++)
	occupancy[i][j] = false;
    }

    obstacles = (bool**)malloc(h*sizeof(bool*));
    for(int i = 0 ; i < h ; i++){
      obstacles[i] = (bool*)malloc(w*sizeof(bool));
      for(int j = 0 ; j < w ; j++){
	if(i > 5 && j > 5 && i == j)
	  obstacles[i][j] = true;
	else
	  obstacles[i][j] = false;
      }
    }

    balls = (Ball*)malloc(nbBalls*sizeof(Ball));

    for(int i = 0 ; i < nbBalls ; i++){
      // balls[i] = Ball(((i%w)*GRAN), (i/w)*GRAN, 10);// init with same gravity sensibility
      balls[i] = Ball(((i%w)*GRAN), (i/w)*GRAN, 0.9*(i+1));// init with different gravity sensibility
    }    
  }

  ~gravityLed(){}
  
  void step(float x, float y){
    for(int i = 0 ; i < nbBalls ; i++)
      balls[i].tick(obstacles, balls, nbBalls, i, x, y, w, h);

    updateOccupancy(occupancy, balls);
  }

  bool** getMatrix(){
    return occupancy;
  }

  void updateForceDir(vec d){
    for(int i = 0 ; i < nbBalls ; i++)
      balls[i].updateForceDir(d);
  }

  void setObstacle(int i, int j, bool val){
    obstacles[i][j] = val;
  }

  private:

  Ball* balls;  
  bool** occupancy;
  bool** obstacles;
  int w, h;
  int nbBalls;

  void updateOccupancy(bool** occupancy, Ball* balls){

    for(int i = 0 ; i < w ; i++)
      for(int j = 0 ; j < h ; j++){
	if(obstacles[i][j])
	  occupancy[i][j] = true;
	else
	  occupancy[i][j] = false;
      }
  
    for(int i = 0 ; i < nbBalls ; i++){
      int gridX = (int)((balls[i].pos.x)/(GRAN*1.));
      int gridY = (int)((balls[i].pos.y)/(GRAN*1.));
      occupancy[gridX][gridY] = true;   
    }
    
  }

};
