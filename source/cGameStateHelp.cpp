//cGameStateHelp.cpp
#include "..\includes\cgamestatescore.h"
#include "..\includes\keystatus.h"
#include "..\includes\CGameStateHelp.h"

extern CDXGraphics g_con;
extern CAudioManager g_Audio;

cGameStateHelp::cGameStateHelp(void)
{
}

cGameStateHelp::~cGameStateHelp(void)
{
}

void cGameStateHelp::initialize(){
}

void cGameStateHelp::activate(){
}


cGameStateObject* cGameStateHelp::update(){
  m_event = GO_NO_WHERE;

  if(keyDown(VK_SPACE)){
    m_event = GO_MAIN;
  }

  for(int i=0; i< m_TE.size(); i++){
    if (m_TE[i].event == m_event)
      return m_TE[i].p_gso;
  }
  return 0;
}

void cGameStateHelp::processEvent(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam){

}


void cGameStateHelp::render(){
 //Clear buffer and draw graphics
  g_con.ClearBuffer(0, 90, 90, 0);
  g_con.BeginDrawing();

  g_con.Draw2DText("Instructions for Rommel's Fantasy Revenge", F_V20, 50,20,255,255,0);
  g_con.Draw2DText("Field Marshall Rommel has been ordered to return to Deutschland by Der Fuhrer.", F_V20, 50,60,20,255,0);
  g_con.Draw2DText("Rommel longs for the day that he can return to North Africa to defeat the Allies.", F_V20, 50,80,20,255,0);
  g_con.Draw2DText("Each day before his death, he fantasizes returning to Africa and single-", F_V20, 50,100,20,255,0);
  g_con.Draw2DText("handedly destroying the British and the American mechanized forces.", F_V20, 50,120,20,255,0);
  g_con.Draw2DText("YOU are Field Marshall Rommel.", F_V20, 50,140,255,0,0);
  g_con.Draw2DText("Drive your tank through 5 regions (levels) of Africa. Destroy all", F_V20, 50,160,20,255,0);
  g_con.Draw2DText("tanks. There are 10 tanks in each level.", F_V20, 50,180,20,255,0);
  g_con.Draw2DText("BE CAREFUL! As each level progresses, the tank crews you will be fighting", F_V20, 50,200,20,255,0);
  g_con.Draw2DText("will be more challenging. They are more accurate at shooting and they reload faster.", F_V20, 50,220,20,255,0);
  g_con.Draw2DText("Use W,A,S,D keys to drive the tank.", F_V20, 50,300,20,255,0);
  g_con.Draw2DText("Use the Arrow keys to control the turret.", F_V20, 50,320,20,255,0);
  g_con.Draw2DText("Use SPACEBAR for shooting.", F_V20, 50,340,20,255,0);
  g_con.Draw2DText("The game can be Paused by pressing 'P'.  ", F_V20, 50,360,20,255,0);
  g_con.Draw2DText("For more data, press the 'T' key.", F_V20, 50,380,20,255,0);
  g_con.Draw2DText("Press ESCAPE to exit the game and return to the Main Menu.", F_V20, 50,400,20,255,0);
  g_con.Draw2DText("If your tank is destroyed in levels 2 through 5, you may", F_V20, 50,420,20,255,0);
  g_con.Draw2DText("return to that level from the Main Menu.", F_V20, 50,440,20,255,0);

  g_con.Draw2DText("TIPS: Avoid enemy red circles.", F_V20, 50,500,255,0,0);
  g_con.Draw2DText("You can accelerate, deaccelerate or turn to escape the red circles.", F_V20, 50,520,20,255,0);
  g_con.Draw2DText("In lower levels, up to 8 seconds may elapse between reloads.", F_V20, 50,540,20,255,0);

  g_con.Draw2DText("Press SPACEBAR to Continue", F_V20, g_con.GetScreenCenterX() - 75,g_con.GetScreenHeight() - 150,255,255,255);      
  g_con.EndDrawing ();
  g_con.Present();
 }

void cGameStateHelp::deactivate(){
}
void cGameStateHelp::resume(){
}
void cGameStateHelp::pause(){
}
void cGameStateHelp::save(){
}

void cGameStateHelp::addTransitionEvent(int event, cGameStateObject* p_Next){
  TRANSITION_EVENT te;
  te.event=event;
  te.p_gso = p_Next;

  m_TE.push_back(te);
}