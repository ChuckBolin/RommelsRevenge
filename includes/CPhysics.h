#ifndef CPHYSICS_H
#define CPHYSICS_H
#include <cmath>
#include "CTank.h"
#include "Graphics.h"

//constants and structures
//boundaries of the world
const int CWORLD_LEFT_LIMIT = 0;
const int CWORLD_TOP_LIMIT  = 0;
const int CWORLD_RIGHT_LIMIT = 2015;
const int CWORLD_BOTTOM_LIMIT = 2015;
const float PI = 3.14159; 
const float CTANK_RANGE = 400;

class CTank;

struct VECTOR2D{ 
  float x; 
  float y; 
}; 

struct QUAD2D{ 
  VECTOR2D v1, v2, v3, v4; //4 verticies of quad 
}; 

struct CIRCLE2D{ 
  VECTOR2D center; 
  float radius; 
}; 

class CPhysics{
public:
  void CheckWorldBoundary(CTank &tank1);
  void CheckCollision(CTank &tank1, CTank &tank2);
  void CheckCollisionCircle(CTank &tank1, CTank &tank2);
  bool CheckAShootsB(CTank &tank1, CTank &tank2);
  float GetDistance(float x1, float y1, float x2, float y2);
  float GetAngle(float x1, float y1, float x2, float y2);
  float GetVectorMagnitude(VECTOR2D &vec); 
  float GetDotProduct(VECTOR2D &vec1, VECTOR2D &vec2); 
  float GetAngleTwoVectors(VECTOR2D &vec1, VECTOR2D &vec2); 
  bool IsPointInsideQuad(QUAD2D &q, VECTOR2D &ref); //
  bool QuadQuadIntersection(QUAD2D &q1, QUAD2D &q2); 
  bool IsPointInsideCircle(CIRCLE2D &c, VECTOR2D &v); //
  VECTOR2D CreatePointOnLine(VECTOR2D &p0, float t, VECTOR2D &v); 
  VECTOR2D RotateVector2D(VECTOR2D &pt, float angle); 
  QUAD2D RotateQuad(QUAD2D &q, VECTOR2D &refPt, float angle); 
  VECTOR2D NormalizeVector2D(VECTOR2D &v);
  void UpdateTankTarget(CTank &tank1, CTank &tank2);
  float GetBearing(float x1, float y1, float x2, float y2);
  int QuadOverlapQuad(QUAD2D &q1, QUAD2D &q2);
  //void SetGameLevel(int level){m_gameLevel = level;}
  

private:
  //m_gameLevel;
};

#endif