#include "..\includes\CPhysics.h"

extern int g_gameLevel;
extern CDXGraphics g_con;

//return 0 if no pts overlap other quads
//return 1 if q1 pt in q2
//return 2 if q2 pt in q1
int CPhysics::QuadOverlapQuad(QUAD2D &q1, QUAD2D &q2){
  //look for quad 1 pts in quad 2
  if(IsPointInsideQuad(q2, q1.v1) == true)
    return 1;
  else if(IsPointInsideQuad(q2, q1.v2) == true)
    return 1;
  else if(IsPointInsideQuad(q2, q1.v3) == true)
    return 1;
  else if(IsPointInsideQuad(q2, q1.v4) == true)
    return 1;

  //look for quad 2 pt in quad 1
  else if(IsPointInsideQuad(q1, q2.v1) == true)
    return 1;
  else if(IsPointInsideQuad(q1, q2.v2) == true)
    return 1;
  else if(IsPointInsideQuad(q1, q2.v3) == true)
    return 1;
  else if(IsPointInsideQuad(q1, q2.v4) == true)
    return 1;

  return 0;
}

void CPhysics::CheckCollision(CTank &tank1, CTank &tank2){
  float radius = tank1.GetRadius();
  float x1 = tank1.GetTankX();
  float y1 = tank1.GetTankY();
  float x2 = tank2.GetTankX();
  float y2 = tank2.GetTankY();
  int ret = 0; //return value
  QUAD2D quad1;
  quad1.v1.x = (tank1.m_tankX - tank1.m_width/2);
  quad1.v2.x = (tank1.m_tankX + tank1.m_width/2);
  quad1.v3.x = (tank1.m_tankX + tank1.m_width/2);
  quad1.v4.x = (tank1.m_tankX - tank1.m_width/2);
  quad1.v1.y = (tank1.m_tankY - tank1.m_length/2);
  quad1.v2.y = (tank1.m_tankY + tank1.m_length/2);
  quad1.v3.y = (tank1.m_tankY + tank1.m_length/2);
  quad1.v4.y = (tank1.m_tankY - tank1.m_length/2);
  QUAD2D quad2;
  quad2.v1.x = (tank2.m_tankX - tank1.m_width/2);
  quad2.v2.x = (tank2.m_tankX + tank1.m_width/2);
  quad2.v3.x = (tank2.m_tankX + tank1.m_width/2);
  quad2.v4.x = (tank2.m_tankX - tank1.m_width/2);
  quad2.v1.y = (tank2.m_tankY - tank1.m_length/2);
  quad2.v2.y = (tank2.m_tankY + tank1.m_length/2);
  quad2.v3.y = (tank2.m_tankY + tank1.m_length/2);
  quad2.v4.y = (tank2.m_tankY - tank1.m_length/2);

  VECTOR2D vec1;
  vec1.x = tank1.GetTankX();
  vec1.y = tank1.GetTankY();
  VECTOR2D vec2;
  vec1.x = tank2.GetTankX();
  vec1.y = tank2.GetTankY();

  //look for overlap in bounding circles first
  if(CPhysics::GetDistance(x1,y1,x2,y2) < (2 * radius)){
    quad1 = RotateQuad(quad1, vec1, tank1.GetTankDir());
    quad2 = RotateQuad(quad2, vec2, tank2.GetTankDir());  
    ret = QuadOverlapQuad(quad1, quad2);
    if(ret == 0)
      return; //no overlap of quad

    //if(ret == 1){    
    if(abs(tank1.m_tankDir - tank2.m_tankDir) > 2.0){
      tank1.m_tankVel = -tank1.m_tankVel;
      tank2.m_tankVel = -tank2.m_tankVel;
      tank1.m_tankAcc = - tank1.m_tankAcc;
      tank2.m_tankAcc = - tank2.m_tankAcc;
    }
    else if(ret == 1){ //tank1 crossed over into tank2
      if(GetBearing(tank1.m_tankX, tank1.m_tankY, tank2.m_tankX, tank2.m_tankY) < PI)
        tank1.m_tankX -= tank1.m_tankVel;
      else
        tank1.m_tankX += tank1.m_tankVel;
      if(GetBearing(tank1.m_tankX, tank1.m_tankY, tank2.m_tankX, tank2.m_tankY) < 4.71 
        && GetBearing(tank1.m_tankX, tank1.m_tankY, tank2.m_tankX, tank2.m_tankY) > 1.57)
        tank1.m_tankY -= tank1.m_tankVel;
      else
        tank1.m_tankY += tank1.m_tankVel;

      tank1.m_tankVel = -tank1.m_tankVel;
      //tank2.m_tankVel = -tank2.m_tankVel;
      tank1.m_tankAcc = - tank1.m_tankAcc;
      //tank2.m_tankAcc = - tank2.m_tankAcc;
    }
    else if(ret == 2){ //tank2 crossed over into tank1
      if(GetBearing(tank2.m_tankX, tank2.m_tankY, tank1.m_tankX, tank1.m_tankY) < PI)
        tank2.m_tankX -= tank2.m_tankVel;
      else
        tank2.m_tankX += tank2.m_tankVel;
      if(GetBearing(tank2.m_tankX, tank2.m_tankY, tank1.m_tankX, tank1.m_tankY) < 4.71 
        && GetBearing(tank2.m_tankX, tank2.m_tankY, tank1.m_tankX, tank1.m_tankY) > 1.57)
        tank2.m_tankY -= tank2.m_tankVel;
      else
        tank2.m_tankY += tank2.m_tankVel;

      //tank1.m_tankVel = -tank1.m_tankVel;
      tank2.m_tankVel = -tank2.m_tankVel;
      //tank1.m_tankAcc = - tank1.m_tankAcc;
      tank2.m_tankAcc = - tank2.m_tankAcc;
    }
  }//if(CPhysics::GetDistance....
  
}

void CPhysics::CheckCollisionCircle(CTank &tank1, CTank &tank2){
  float radius = tank1.GetRadius();
  float x1 = tank1.GetTankX();
  float y1 = tank1.GetTankY();
  float x2 = tank2.GetTankX();
  float y2 = tank2.GetTankY();

  if(CPhysics::GetDistance(x1,y1,x2,y2) < (1.8 * radius)){
    if(tank1.GetType() == C_TYPE_PLAYER){
      tank1.m_tankVel = -0.3;//tank1.m_tankVel;
      tank1.m_tankAcc = - tank1.m_tankAcc;
      return;///
    }
    else if(tank2.GetType() == C_TYPE_PLAYER){
      tank2.m_tankVel = -0.3;//tank1.m_tankVel; //-0.3
      tank2.m_tankAcc = - tank2.m_tankAcc;
      return;///
    }

    //determine tank with greatest CW direction value
    //this one will stop and wait for range to open up
    //allows other tank to manuever
    float t1 = 0;
    float t2 = 0;
    t1 = tank1.m_tankDir;
    t2 = tank2.m_tankDir;
    if(t1 - t2 < 0.1  || t2 - t1 < 0.1){
      tank1.m_tankVel = -.5;
      tank2.m_tankVel = .5;
    }
    else if(tank1.m_tankDir > tank2.m_tankDir){
      if(tank1.IsAlive() == true && tank2.IsAlive() == false){ //this tank can move
        tank1.m_tankVel = - 0.3;//tank1.m_tankVel ;//.3;//1;                     //stop quickly
        tank1.m_tankAcc = tank1.m_tankAcc * -1;  //reverse acceleration
        tank1.IncrementTankStateStep();
      }
      else if(tank1.IsAlive() == true && tank2.IsAlive() == true){ //this tank can move
        tank1.m_tankVel = - 0.3;//-tank1.m_tankVel ;//-.3;//1;                     //stop quickly
        tank1.m_tankAcc = tank1.m_tankAcc * -1;  //reverse acceleration
      }
      else if(tank1.IsAlive() == false &&tank2.IsAlive() == true){
        tank2.m_tankVel = - 0.3;//-tank1.m_tankVel ;//-.3;//1;                     //stop quickly
        tank2.m_tankAcc = tank2.m_tankAcc * -1;  //reverse acceleration
        tank2.IncrementTankStateStep();
      }
    }
    else{
      if(tank2.IsAlive() == true && tank1.IsAlive() == false){ //this tank can move
        tank2.m_tankVel = - 0.3;//-tank1.m_tankVel ;//-.3;//1;                     //stop quickly
        tank2.m_tankAcc = tank2.m_tankAcc * -1;  //reverse acceleration
        tank2.IncrementTankStateStep();
      }
      else if(tank2.IsAlive() == true && tank1.IsAlive() == true){ //this tank can move
        tank2.m_tankVel = - 0.3;//-tank1.m_tankVel ;//-.3;//1;                     //stop quickly
        tank2.m_tankAcc = tank2.m_tankAcc * -1;  //reverse acceleration
      }
      else if(tank2.IsAlive() == false &&tank1.IsAlive() == true){
        tank1.m_tankVel = - 0.3;//-tank1.m_tankVel ;//-.3;//1;                     //stop quickly
        tank1.m_tankAcc = tank1.m_tankAcc * -1;  //reverse acceleration
        tank1.IncrementTankStateStep();
      }
    }
  }
  
  else if(CPhysics::GetDistance(x1,y1,x2,y2) < (radius)){
    if(tank1.IsAlive() == true){
      tank1.m_tankVel = -2;
      tank1.m_tankAcc = -0.05;
      tank2.m_tankSteer -= .01;
    }
    else if(tank2.IsAlive() ==true){
      tank2.m_tankVel = -2;
      tank2.m_tankAcc = -.05;
      tank2.m_tankSteer += .01;
    }
  }
  
}

void CPhysics::CheckWorldBoundary(CTank &tank1){
  
  if(tank1.m_tankX < CWORLD_LEFT_LIMIT + tank1.m_radius/2){
    tank1.m_tankX = CWORLD_LEFT_LIMIT + tank1.m_radius/2 + 2;//,tank1.m_tankX;
    tank1.m_tankVel = -tank1.m_tankVel;//-2;
    tank1.IncrementTankStateStep();
  }

  if(tank1.m_tankX > CWORLD_RIGHT_LIMIT - tank1.m_radius/2){
    tank1.m_tankX = CWORLD_RIGHT_LIMIT - tank1.m_radius/2 - 2;
    tank1.m_tankVel = -tank1.m_tankVel;//-2;
    tank1.IncrementTankStateStep();
  }

  if(tank1.m_tankY < CWORLD_TOP_LIMIT + tank1.m_radius/2){
    tank1.m_tankY = CWORLD_TOP_LIMIT + tank1.m_radius/2 + 2;
    tank1.m_tankVel = -tank1.m_tankVel;//-2;
    tank1.IncrementTankStateStep();
  }

  if(tank1.m_tankY > CWORLD_BOTTOM_LIMIT - tank1.m_radius/2){
    tank1.m_tankY = CWORLD_BOTTOM_LIMIT - tank1.m_radius/2 - 2;
    tank1.m_tankVel = -tank1.m_tankVel;//2;
    tank1.IncrementTankStateStep();
  }
}

//determine if A shoots...and if so does it hit tank2
bool CPhysics::CheckAShootsB(CTank &tank1, CTank &tank2){
  if(tank1.m_bAlive == false) //tank1 is okay
    return false;

  if(tank1.m_bTankFiresRound == false)  //didn't fire
    return false;  

  //tank1 fired...does it hit tank2?
  QUAD2D quad;
  quad.v1.x = tank2.m_tankX - (tank2.m_tankWidth/2);
  quad.v1.y = tank2.m_tankY - (tank2.m_tankHeight/2);
  quad.v2.x = tank2.m_tankX + (tank2.m_tankWidth/2);
  quad.v2.y = tank2.m_tankY - (tank2.m_tankHeight/2);
  quad.v3.x = tank2.m_tankX + (tank2.m_tankWidth/2);
  quad.v3.y = tank2.m_tankY + (tank2.m_tankHeight/2);
  quad.v4.x = tank2.m_tankX - (tank2.m_tankWidth/2);
  quad.v4.y = tank2.m_tankY + (tank2.m_tankHeight/2);
  VECTOR2D vec;
  vec.x = tank2.m_tankX;
  vec.y = tank2.m_tankY;
  QUAD2D temp = RotateQuad(quad,vec,tank2.m_tankDir);

  VECTOR2D pt;
  pt.x = tank1.GetTargetX();
  pt.y = tank1.GetTargetY();
  
  ////
  if(tank1.GetType() == C_TYPE_ENEMY)
    tank1.m_bTankFiresRound = false;

  if(IsPointInsideQuad(temp, pt) == true && tank2.m_bAlive == true){
    tank2.m_bAlive = false;
    tank2.m_tankVel = 0;
    return true;
  }

  return false;
}

//updates an enemy tanks azimuth and elevation
//to move target
//tank1 = player, tank2 is enemy AI
void CPhysics::UpdateTankTarget(CTank &tank1, CTank &tank2){
  if(tank2.IsAlive() == false)
    return;

  float distance = GetDistance(tank1.m_tankX, tank1.m_tankY, tank2.m_tankX, tank2.m_tankY);
  float targetDistance = GetDistance(tank2.m_targetX, tank2.m_targetY, tank1.m_tankX, tank1.m_tankY);
  float targetDistance2 = GetDistance(tank2.m_targetX, tank2.m_targetY, tank2.m_tankX, tank2.m_tankY);  
  
  //this is too fast
  float angle = GetBearing(tank2.m_tankX, tank2.m_tankY, tank1.m_tankX, tank1.m_tankY);
  
  tank2.m_turretAz = angle;
  if(tank2.m_turretAz > 2 * PI)
    tank2.m_turretAz = 0.0f;
  if(tank2.m_turretAz < 0.0f)
    tank2.m_turretAz = 2 * PI;
  
  if(distance < C_TANK_MAX_ELEV + 100){
    if(distance < targetDistance2) //target to far out
      //tank2.m_turretElev -= 1;
      tank2.m_turretElev -= 1;// + ((float)5/10));//g_gameLevel/4));
    else if(targetDistance2 < distance)
      //tank2.m_turretElev += 1;
      tank2.m_turretElev += 1;// + ((float)5/10);//g_gameLevel/4);
    
    if(tank2.m_turretElev > C_TANK_MAX_ELEV)
      tank2.m_turretElev = C_TANK_MAX_ELEV;
    if(tank2.m_turretElev < C_TANK_MIN_ELEV)
      tank2.m_turretElev = C_TANK_MIN_ELEV;
  }
/*
  if(distance < 600){//C_TANK_MAX_ELEV + 100){ 
    if(targetDistance > 0 )
      tank2.m_turretElev += 66;//1.0f;
    else if(targetDistance < 0 )
      tank2.m_turretElev -= 66;//1.0;
    
    if(tank2.m_turretElev > C_TANK_MAX_ELEV)
      tank2.m_turretElev = C_TANK_MAX_ELEV;
    if(tank2.m_turretElev < C_TANK_MIN_ELEV)
      tank2.m_turretElev = C_TANK_MIN_ELEV;
  }
  */

  if(targetDistance  < tank2.GetRadius() * (1 + 6 - g_gameLevel))// && tank2.m_bTankFiresRound == false)
    tank2.m_bTargetInRange = true;
  else
    tank2.m_bTargetInRange = false; 
}

float CPhysics::GetDistance(float x1, float y1, float x2, float y2){
  return sqrt( (x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1));
}

float CPhysics::GetAngle(float x1, float y1, float x2, float y2){
  float angle = 0;
  float dx;
  float dy;

  dy = y2 - y1;
  dx = x2 - x1;
  if(dx == 0){

  }
  else{
    angle = atan(-dy/dx);
  }
  if(dy > 0 && dx > 0){  //bottom-right OK
    angle = PI/2 - angle;
  }
  else if(dy > 0 && dx < 0) {//bottom-left OK
    angle = 5*PI/2 -(angle + PI);
  }
  else if(dy < 0 && dx < 0){ //top-left OK
    angle = 3*PI/2 - angle;
  }
  else if(dy < 0 && dx > 0){ //top-right
    angle = 5*PI/2 -(angle + 2*PI);
  }
  
  return angle;
}

float CPhysics::GetBearing(float x1, float y1, float x2, float y2){
  float angle = 0;
  float dx;
  float dy;

  dy = y2 - y1;
  dx = x2 - x1;
  if(dx == 0){

  }
  else{
    angle = atan(-dy/dx);
  }

  if(dy > 0 && dx > 0){  //bottom-right OK
    angle = PI/2 - angle;
  }
  else if(dy > 0 && dx < 0) {//bottom-left OK
    angle = 5*PI/2 -(angle + PI);
  }
  else if(dy < 0 && dx < 0){ //top-left OK
    angle = 3*PI/2 - angle;
  }
  else if(dy < 0 && dx > 0){ //top-right
    angle = 5*PI/2 -(angle + 2*PI);
  }

  if(angle > 2 * PI)
    angle = 0.0f;
  if(angle < 0.0f)
    angle = 2 * PI;

  return angle;
}


//returns magnitude of vector 
float CPhysics::GetVectorMagnitude(VECTOR2D &vec){ 
  return sqrt( (vec.x * vec.x) + (vec.y * vec.y)); 
} 

//calcs and returns dot product 
float CPhysics::GetDotProduct(VECTOR2D &vec1, VECTOR2D &vec2){ 
  return vec1.x * vec2.x + vec1.y * vec2.y; 
} 

//returns angle between two vectors 
float CPhysics::GetAngleTwoVectors(VECTOR2D &vec1, VECTOR2D &vec2){ 
  return (180/PI) * acos( GetDotProduct(vec1, vec2)/(GetVectorMagnitude(vec1)* 
                          GetVectorMagnitude(vec2))); 
} 

//returns true if point is inside quad 
bool CPhysics::IsPointInsideQuad(QUAD2D &q, VECTOR2D &ref){ 
  float angle = 0; 
  VECTOR2D a, b; 

  a.x = q.v1.x - ref.x; 
  a.y = q.v1.y - ref.y; 
  b.x = q.v2.x - ref.x; 
  b.y = q.v2.y - ref.y; 
  angle = GetAngleTwoVectors(a, b); 

  a.x = q.v2.x - ref.x; 
  a.y = q.v2.y - ref.y; 
  b.x = q.v3.x - ref.x; 
  b.y = q.v3.y - ref.y; 
  angle += GetAngleTwoVectors(a, b); 

  a.x = q.v3.x - ref.x; 
  a.y = q.v3.y - ref.y; 
  b.x = q.v4.x - ref.x; 
  b.y = q.v4.y - ref.y; 
  angle += GetAngleTwoVectors(a, b); 

  a.x = q.v4.x - ref.x; 
  a.y = q.v4.y - ref.y; 
  b.x = q.v1.x - ref.x; 
  b.y = q.v1.y - ref.y; 

  angle += GetAngleTwoVectors(a, b); 

  if(angle < 359.9) 
    return false; 
  if(angle > 360.1) 
    return false; 
  
  return true; 
} 

bool CPhysics::QuadQuadIntersection(QUAD2D &q1, QUAD2D &q2){ 
  if(IsPointInsideQuad(q1, q2.v1)) 
    return true; 
  if(IsPointInsideQuad(q1, q2.v2)) 
    return true; 
  if(IsPointInsideQuad(q1, q2.v3)) 
    return true; 
  if(IsPointInsideQuad(q1, q2.v4)) 
    return true; 
  if(IsPointInsideQuad(q2, q1.v1)) 
    return true; 
  if(IsPointInsideQuad(q2, q1.v2)) 
    return true; 
  if(IsPointInsideQuad(q2, q1.v3)) 
    return true; 
  if(IsPointInsideQuad(q2, q1.v4)) 
    return true; 

  return false; 
} 

bool CPhysics::IsPointInsideCircle(CIRCLE2D &c, VECTOR2D &v){ 
  VECTOR2D vec; 
  vec.x = c.center.x - v.x; 
  vec.y = c.center.y - v.y; 

  float distance = GetVectorMagnitude(vec); 

  if(distance < c.radius) 
    return true; 

  return false; 
} 

//given point p0, and directin vector v, and increment t 
//project line based upon t..used for ray casting 
VECTOR2D CPhysics::CreatePointOnLine(VECTOR2D &p0, float t, VECTOR2D &v){ 
  VECTOR2D temp; 
  temp.x = p0.x + (t * v.x); 
  temp.y = p0.y + (t * v.y); 
  return temp; 
} 

VECTOR2D CPhysics::NormalizeVector2D(VECTOR2D &v){ 
  VECTOR2D vec; 
  float mag = GetVectorMagnitude(v); 
  vec.x = v.x/mag; 
  vec.y = v.y/mag; 
  return vec; 
} 

//rotates point about reference point 
VECTOR2D CPhysics::RotateVector2D(VECTOR2D &pt, float angle){ 
  VECTOR2D vec; 
  vec.x = pt.x * cos(angle) - pt.y * sin(angle); 
  vec.y = pt.x * sin(angle) + pt.y * cos(angle); 
  return vec; 
} 

//rotates quad about reference point 
QUAD2D CPhysics::RotateQuad(QUAD2D &q, VECTOR2D &refPt, float angle){ 
  QUAD2D quad; 
  QUAD2D temp; 
  temp.v1.x = q.v1.x - refPt.x; 
  temp.v1.y = q.v1.y - refPt.y; 
  quad.v1 = RotateVector2D(temp.v1, angle); 
  quad.v1.x += refPt.x;
  quad.v1.y += refPt.y;
  temp.v2.x = q.v2.x - refPt.x; 
  temp.v2.y = q.v2.y - refPt.y; 
  quad.v2 = RotateVector2D(temp.v2, angle); 
  quad.v2.x += refPt.x;
  quad.v2.y += refPt.y;
  temp.v3.x = q.v3.x - refPt.x; 
  temp.v3.y = q.v3.y - refPt.y; 
  quad.v3 = RotateVector2D(temp.v3, angle); 
  quad.v3.x += refPt.x;
  quad.v3.y += refPt.y;
  temp.v4.x = q.v4.x - refPt.x; 
  temp.v4.y = q.v4.y - refPt.y; 
  quad.v4 = RotateVector2D(temp.v4, angle); 
  quad.v4.x += refPt.x;
  quad.v4.y += refPt.y;
  
  return quad; 
} 
