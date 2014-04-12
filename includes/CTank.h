#ifndef CTANK_H
#define CTANK_H
#include <cmath>
#include <vector>
#include "CObject.h"
#include "CTimer.h"
#include "CPhysics.h"

class CPhysics;

const int C_TANK_LEVEL_WIDTH = 2056;
const int C_TANK_LEVEL_HEIGHT = 2056;
const float C_TANK_MAX_ELEV = 500;
const float C_TANK_MIN_ELEV = 75;
const float C_TANK_MAX_VEL = 3.5;
const float C_TANK_MIN_VEL = -2;
const float C_TANK_MAX_ACC = 0.05f;
const float C_TANK_MIN_ACC = -0.05f;
const float C_TANK_MAX_STEER = 0.03f;
const float C_TANK_MIN_STEER = -0.03f;
const float C_TANK_MAX_CW = 1.57f;
const float C_TANK_MAX_CCW = -1.57f;
const int C_TYPE_PLAYER = 1;
const int C_TYPE_ENEMY = 2;
const int C_STATUS_DEAD = -1;
const int C_STATUS_OK = 0;
const int C_STATUS_COLLISION = 1;
const int C_STATUS_HIT = 2;
const int C_TANK_STATE_WAIT = 0;
const int C_TANK_STATE_PATROL = 1;
const int C_TANK_STATE_ATTACK = 2;
const int C_TANK_STATE_EVADE = 3;
const int C_TANK_INITIAL_AMMO_PLAYER = 25;
const int C_TANK_INITIAL_AMMO_ENEMY = 25;
const int C_COUNTRY_US = 0;
const int C_COUNTRY_UK = 1;

struct WAYPOINT{
  float x;
  float y;
  float acc;
  float maxVel;
  int initialCount;
  int count;
  float dir;
};

class CTank{
public:
  CTank::CTank();
  CTank::~CTank();
  void Initialize();
  void Initialize(float x, float y, float dir, float vel);
  float GetTankX();
  float GetTankY();
  float GetTankDir(){return m_tankDir;}
  float SetTankVel(){return m_tankVel;}
  float GetTurretX(){return m_turretX;}
  float GetTurretY(){return m_turretY;}
  float GetTurretAz(){return m_turretAz;}
  float GetTurretElev(){return m_turretElev;}
  float GetTurretCW(){return m_turretCW;}
  float GetTurretCCW(){return m_turretCCW;}
  float GetTargetX(){return m_targetX;}
  float GetTargetY(){return m_targetY;}
  void SetTankPosition(float x, float y);
  void SetTankDir(float dir);
  void SetTankVel(float vel);
  void SetTankMaxVel(float max){m_tankMaxVel = max;}
  void SetTurretPosition(float x, float y);
  void SetTurretLimits(float cw, float ccw);
  void SetTurretAzimuth(float az);
  void SetTurretElevation(float elev);
  void Update();
  void MoveLeft(){m_bMoveLeft = true;}
  void MoveRight(){m_bMoveRight = true;}
  void MoveForward(){m_bMoveForward = true;}
  void MoveReverse(){m_bMoveReverse = true;}
  void MoveTurretLeft(){m_bMoveTurretLeft = true;}
  void MoveTurretRight(){m_bMoveTurretRight = true;}
  void MoveTurretUp(){m_bMoveTurretUp = true;}
  void MoveTurretDown(){m_bMoveTurretDown = true;}
  void SetType(int type);
  void SetStatus(int status);
  int GetStatus();
  int GetType();
  float GetRadius(){return m_radius;}
  void SetAcc(float acc){m_tankAcc = acc;}
  void AddWaypoint(float x, float y, float acc, float maxVel, int initialCount, int count, float dir);
  void SetTankState(int state){m_tankState = state;}
  int GetTankState(){return m_tankState;}
  float GetWaypointX(int id);
  float GetWaypointY(int id);
  int GetTankStateStep(){return m_tankStateStep;}
  void IncrementTankStateStep();
  bool IsAlive(){return m_bAlive;}
  void SetAmmoCount(int count){m_ammoCount = count;}
  int GetAmmoCount(){return m_ammoCount;}
  void DecAmmoCount(){m_ammoCount--;}
  void ClearWaypoints();

  float m_tankX;
  float m_tankY;
  float m_tankDir;
  float m_tankSteer; 
  float m_tankVel;
  float m_tankMaxVel;
  float m_tankAcc;
  float m_turretX;
  float m_turretY;
  float m_turretAz;
  float m_turretElev;
  float m_turretCW;
  float m_turretCCW;
  float m_targetX;
  float m_targetY;
  float m_range;
  float m_radius;
  float m_length;
  float m_width;
  bool m_bAlive;
  bool m_bTankFiresRound;
  float m_tankWidth;
  float m_tankHeight;
  bool m_bReadyToFire;
  float m_randomX;
  float m_randomY;
  int m_firingInterval;
  bool m_bTargetInRange;
  bool m_bExplosion;
  int m_countryFlag;

private:
  std::vector<WAYPOINT> m_wp; //stores waypoint data
  int m_type;  //0=player, 1=enemy tank, 2=obstacle
  int m_status;  //-1 = dead, 0 = ok, 1=collides, 2=hit
  CTimer m_secondTimer;
  CTimer m_time;
  int m_timeUp;
  int m_secondCount;
  int m_fireTime;
  bool m_bTankFired;
  bool m_bMoveForward;
  bool m_bMoveReverse;
  bool m_bMoveLeft;
  bool m_bMoveRight;
  bool m_bMoveTurretLeft;
  bool m_bMoveTurretRight;
  bool m_bMoveTurretUp;
  bool m_bMoveTurretDown;
  int m_tankState;
  int m_tankStateStep;
  bool m_bFirstTime;
  int m_ammoCount;


};

#endif