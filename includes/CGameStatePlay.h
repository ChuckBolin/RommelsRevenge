#ifndef CGAMESTATEPLAY_H
#define CGAMESTATEPLAY_H
#include <vector>

#include "CGameStateObject.h"
#include <Windows.h>
#include "CShellImpact.h"
#include "Graphics.h"
#include "CAudioManager.h"
#include "CAudioConstants.h"
#include "CTank.h"
#include "Keystatus.h"
#include "CPhysics.h"
#include "CTimer.h"
#include "Log.h"
#include "ReadLevelFile.h"

const int C_PLAY_ENEMYCOUNT_LEVEL = 10;//10
const int C_PLAY_MAX_LEVEL = 5;//5

class CGameStatePlay: public cGameStateObject{
public:
  CGameStatePlay(void);
  ~CGameStatePlay(void);
  void initialize();
  void activate();
  cGameStateObject* update();
  void render();
  void deactivate();
  void resume();
  void pause();
  void save();
  void addTransitionEvent(int event, cGameStateObject* p_Next);
  void processEvent(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
  void InitializeNextLevel();
  void addLevel(int level);
  
private:
  std::vector<TRANSITION_EVENT> m_TE; //stores all transition events
  int m_event;
  std::vector<CObject> m_Object;
  CTank* m_pObject;
  int m_maxEnemyTanks;
  CPhysics m_Physics;
  bool m_bDataOneShot;
  bool m_bKeyTPressed;
  CTimer m_LevelTimer;
  int m_EndLevel;  
  CTimer m_winTimer;
  int m_ironCross;
  int deathCount;
  
  //firing timer and vars for player
  CTimer m_secondTimer;
  int m_secondCount;
  int m_fireTime;
  bool m_bTankFired;
  bool m_bPaused;
  int m_timeRemaining;
  int m_kills;
  int m_level;
  bool m_levelComplete;
  bool m_gameOver;
  CFile m_file;
  bool m_validFile;
  bool m_bPauseReleased;
  CTimer m_pauseTimer;
  int m_pauseCount;
  bool m_bOneshot;
  bool m_bWinner;
};

#endif