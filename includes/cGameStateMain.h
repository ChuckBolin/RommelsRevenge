#ifndef GAME_STATE_MAIN_H
#define GAME_STATE_MAIN_H
#include "cgamestateobject.h"
#include "CTimer.h"
//#include "..\resource.h"
#include "Graphics.h"
#include "CAudioManager.h"
#include "CAudioConstants.h"
#include <iostream>
#include <mmsystem.h>

class cGameStateMain :  public cGameStateObject{
public:
  cGameStateMain(void);
  ~cGameStateMain(void);
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
  int m_event;
  CTimer m_timer;
  int m_selection;
};

#endif