#include "..\includes\CTank.h"

//extern CAudioManager g_Audio;

CTank::CTank(){
  m_turretElev = 100;
  m_tankWidth = 40;//38;
  m_tankHeight = 50;//48;
}

CTank::~CTank(){}

void CTank::Initialize(){
  m_radius = 25;
  m_bAlive = true;
  m_bTankFiresRound = false;
  m_secondTimer.initialize();
  m_secondCount = 0;
  m_fireTime = 0;
  m_bTankFired = false;
  m_tankStateStep = 0;
  m_type = C_TYPE_ENEMY;
  m_tankX = m_wp[0].x;
  m_tankY = m_wp[0].y;
  m_tankDir = m_wp[0].dir;//
  m_wp[0].count = m_wp[0].initialCount;
  m_tankState = C_TANK_STATE_PATROL;
  m_time.initialize();
  m_timeUp = 0;
  m_bReadyToFire = false;
}

float CTank::GetWaypointX(int id){
  if(id < 0 || id > m_wp.size())
    return 0;
  return m_wp[id].x;
}

float CTank::GetWaypointY(int id){
  if(id < 0 || id > m_wp.size())
    return 0;
  return m_wp[id].y;
}


void CTank::Initialize(float x, float y, float dir, float vel){
  m_tankX = x;
  m_tankY = y;
  m_tankDir = dir;
  m_tankVel = vel;
  m_turretAz = m_tankDir;
  m_radius = 25;
  m_length = 50;
  m_width = 40;
  m_bAlive = true;
  m_bTankFiresRound = false;
  m_secondTimer.initialize();
  m_secondCount = 0;
  m_fireTime = 0;
  m_bTankFired = false;
  m_tankState = C_TANK_STATE_WAIT;
  m_bFirstTime = true;
  m_bReadyToFire = true;
}

void CTank::SetTankPosition(float x, float y){
  m_tankX = x;
  m_tankY = y;
}

void CTank::SetTankDir(float dir){
  m_tankDir = dir;
}

void CTank::SetTankVel(float vel){
  m_tankVel = vel;
}

void CTank::SetTurretPosition(float x, float y){
  m_turretX = x;
  m_turretY = y;
}

void CTank::SetTurretLimits(float cw, float ccw){
  m_turretCW = cw;
  m_turretCCW =ccw;
}

void CTank::SetTurretAzimuth(float az){
  m_turretAz = az;
}
void CTank::SetTurretElevation(float elev){
  m_turretElev = elev;
}

float CTank::GetTankX(){
  return m_tankX;
}

float CTank::GetTankY(){
  return m_tankY;
}

void CTank::Update(){
  float dx;
  float dy;
  int nextStep = 0;
  VECTOR2D vecWaypoint;
  VECTOR2D vecNextWaypoint;
  float angle;
  CPhysics m_Physics;
  float distance;

  if(m_bAlive == false)
    return;

  if(m_type == C_TYPE_PLAYER){
    //eliminate jitter if going real slow
    //if(m_tankVel < 0.1)
    //  m_tankVel = 0.0f;

    //reset acceleration
    if(m_bMoveForward == false && m_bMoveReverse == false){
      m_tankAcc = 0;
    }

    //increase acceleration
    if(m_bMoveForward == true){
      m_tankAcc += 0.01f;
      if(m_tankAcc > C_TANK_MAX_ACC)
        m_tankAcc = C_TANK_MAX_ACC;
      m_tankVel += m_tankAcc;
    }

    //decrease acceleration
    if(m_bMoveReverse == true){
      m_tankAcc -= 0.01f;
      if(m_tankAcc < C_TANK_MIN_ACC)
        m_tankAcc = C_TANK_MIN_ACC;
      m_tankVel += m_tankAcc;
    }

    if(m_tankVel > C_TANK_MAX_VEL)
      m_tankVel = C_TANK_MAX_VEL;
    if(m_tankVel < C_TANK_MIN_VEL)
      m_tankVel = C_TANK_MIN_VEL;

    //adjust turret azimuth and elevation
    if(m_bMoveTurretLeft == true){
      m_turretAz -= 0.035f;
    }

    if(m_bMoveTurretRight == true){
      m_turretAz += 0.035f;
    }

    if(m_bMoveTurretUp == true){
      m_turretElev += 2.5f;
      if(m_turretElev > C_TANK_MAX_ELEV)
        m_turretElev = C_TANK_MAX_ELEV;
    }

    if(m_bMoveTurretDown == true){
      m_turretElev -= 2.5f;
      if(m_turretElev < C_TANK_MIN_ELEV)
        m_turretElev = C_TANK_MIN_ELEV;
    }

    //adjust tank direction
    if(m_bMoveLeft == true && m_bMoveRight == true){
      m_tankSteer = 0.0f;
    }

    if(m_bMoveLeft == true){
      m_tankSteer -= 0.025f;
      if(m_tankSteer < C_TANK_MIN_STEER)
        m_tankSteer = C_TANK_MIN_STEER;
          
      m_tankDir += m_tankSteer;
      m_turretAz -= 0.025f;
    }

    if(m_bMoveRight == true){
      m_tankSteer += 0.025f;
      if(m_tankSteer > C_TANK_MAX_STEER)
        m_tankSteer = C_TANK_MAX_STEER;
    
      m_tankDir += m_tankSteer;
      m_turretAz += 0.025f;
    }    

    //update tank position
    dx = -cos(m_tankDir + 1.57f) * m_tankVel;
    dy = -sin(m_tankDir + 1.57f) * m_tankVel;
    m_tankX += dx;
    m_tankY += dy;

    //update target position
    dx = cos(m_turretAz - 1.57f) * m_turretElev;
    dy = sin(m_turretAz - 1.57f) * m_turretElev;
    m_targetX = m_tankX + dx;
    m_targetY = m_tankY + dy;
  
    //reset control bools
    m_bMoveForward = false;
    m_bMoveReverse = false;
    m_bMoveLeft = false;
    m_bMoveRight = false;
    m_bMoveTurretLeft = false;
    m_bMoveTurretRight = false;
    m_bMoveTurretUp = false;
    m_bMoveTurretDown = false;
  }
  else{  //enemy tank ///////////////////////////////////////////////////////////////////////////////

    //patrol state
    if(m_tankState == C_TANK_STATE_PATROL && m_bAlive == true){
      nextStep = m_tankStateStep + 1;
      if(nextStep > m_wp.size() - 1)
        nextStep = 0;

      if(m_bFirstTime == true){
        m_bFirstTime = false;
        m_tankVel = 0;
        m_tankAcc = m_wp[m_tankStateStep].acc;
        angle = m_Physics.GetAngle(m_tankX, m_tankY, m_wp[nextStep].x, m_wp[nextStep].y);
      }

      m_tankAcc = m_wp[m_tankStateStep].acc;// 0.01f;

      //enemy tank at next waypoint  
      if(m_Physics.GetDistance(m_tankX, m_tankY, m_wp[nextStep].x, m_wp[nextStep].y) < (m_radius)){
        m_tankStateStep++;
        if(m_tankStateStep > m_wp.size()){
          m_tankStateStep = 0;
          nextStep = m_tankStateStep + 1;
          if(nextStep > m_wp.size())
            nextStep = 0;
        }
        m_tankVel = 0;
        m_tankAcc = m_wp[m_tankStateStep].acc;
      }
      m_tankAcc = m_wp[m_tankStateStep].acc;

      if(m_tankAcc > C_TANK_MAX_ACC)
        m_tankAcc = C_TANK_MAX_ACC;
      if(m_tankAcc < C_TANK_MIN_ACC)
        m_tankAcc = C_TANK_MIN_ACC;
      m_tankVel += m_tankAcc;
      if(m_tankVel > C_TANK_MAX_VEL)
        m_tankVel = C_TANK_MAX_VEL;
      if(m_tankVel < C_TANK_MIN_VEL)
        m_tankVel = C_TANK_MIN_VEL;
    }

    angle = m_Physics.GetAngle(m_tankX, m_tankY, m_wp[nextStep].x, m_wp[nextStep].y);
    m_tankSteer = (angle - m_tankDir) * 0.2f;
    if(m_tankSteer > C_TANK_MAX_STEER)
      m_tankSteer = C_TANK_MAX_STEER;
    if(m_tankSteer < C_TANK_MIN_STEER)
      m_tankSteer = C_TANK_MIN_STEER;

    m_tankDir += m_tankSteer;
    
    if(m_tankDir > 2 * PI)
      m_tankDir = 0;
    if(m_tankDir < 0)
      m_tankDir = 2 * PI;

    dx = cos(7.85 - m_tankDir) * m_tankVel;
    dy = -sin(7.85 - m_tankDir) * m_tankVel;
    m_tankX += dx;
    m_tankY += dy;

    //update target position
    dx = cos(m_turretAz - 1.57f) * m_turretElev;
    dy = sin(m_turretAz - 1.57f) * m_turretElev;
    //dx = cos(7.85 - m_turretAz) * m_turretElev * C_TANK_MAX_ELEV ;
    //dy = -sin(7.85 - m_turretAz) * m_turretElev * C_TANK_MAX_ELEV;
    m_targetX = m_tankX + dx;
    m_targetY = m_tankY + dy;

    //auto firing
    if(m_secondTimer.getTimer(0.1)){
      m_secondCount++;
      if(m_bReadyToFire == true){
        if(m_ammoCount > 0 && m_bTargetInRange == true){
          m_ammoCount--;
          m_fireTime = m_secondCount;
          m_bTankFiresRound = true; 
          m_bReadyToFire = false;

        }
      }
      else if(m_bReadyToFire == false && (m_secondCount > m_fireTime + m_firingInterval)){
        m_bReadyToFire = true;
        m_bExplosion = false;
        m_bTankFiresRound = false;
      }
    }//if(m_secondTimer
  }//if(m_type == C_TYPE_PLAYER){
}

void CTank::IncrementTankStateStep(){
   m_tankStateStep++;
   if(m_tankStateStep > m_wp.size())
     m_tankStateStep = 0;
}

void CTank::ClearWaypoints(){
  m_wp.clear();
}
void CTank::AddWaypoint(float x, float y, float acc, float maxVel, int initialCount, int count, float dir){
  WAYPOINT wp;
  wp.x = x;
  wp.y = y;
  wp.count = count;
  wp.initialCount = initialCount;
  wp.maxVel = maxVel;
  wp.acc = acc;
  wp.dir = dir;
  m_wp.push_back(wp);
  
}

void CTank::SetType(int type){
  m_type = type;
}
void CTank::SetStatus(int status){
  m_status = status;
}
int CTank::GetStatus(){
  return m_status;
}
int CTank::GetType(){
  return m_type;
}
