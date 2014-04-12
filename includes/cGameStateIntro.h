#ifndef GAME_STATE_INTRO_H
#define GAME_STATE_INTRO_H
#include <iostream>
using std::cout;
using std::endl;

#include "cgamestateobject.h"
#include "Graphics.h"
#include "CAudioManager.h"
#include "CAudioConstants.h"
#include "CTimer.h"
#include <sstream>
#include <string>
#include "keystatus.h"

class cGameStateIntro :  public cGameStateObject{
public:
  cGameStateIntro(void);
  ~cGameStateIntro(void);
  void initialize();
  void activate();
  cGameStateObject* update();
  void render();
  void processEvent(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
  void deactivate();
  void resume();
  void pause();
  void save();
  void addTransitionEvent(int event, cGameStateObject* p_Next);

private:
  std::vector<TRANSITION_EVENT> m_TE; //stores all transition events
  CTimer m_timer;
  bool m_bDone;
  int m_event;
  bool m_bSplashComplete;

};

#endif