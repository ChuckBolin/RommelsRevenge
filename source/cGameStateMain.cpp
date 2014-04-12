#include "..\includes\cgamestatemain.h"
#include "..\includes\keystatus.h"

extern CDXGraphics g_con;
extern CAudioManager g_Audio;//
extern int g_gameLevel;
bool g_bResetGame;

cGameStateMain::cGameStateMain(void)
{
}

cGameStateMain::~cGameStateMain(void)
{
}

void cGameStateMain::initialize(){
  m_selection = 0;
  m_timer.initialize();
}

void cGameStateMain::activate(){

}

cGameStateObject* cGameStateMain::update(){
  m_event = GO_NO_WHERE;
  
  if(g_Audio.IsPlaying(C_AUDIO_WAR_ENDS) == true)
    g_Audio.StopSoundClip(C_AUDIO_WAR_ENDS);
  if(g_Audio.IsPlaying(C_AUDIO_MUSIC_PLAY) == true)
    g_Audio.StopSoundClip(C_AUDIO_MUSIC_PLAY);
  if(g_Audio.IsPlaying(C_AUDIO_PANZER) == true)
    g_Audio.StopSoundClip(C_AUDIO_PANZER);
  if(g_Audio.IsPlaying(C_AUDIO_TANK_DRIVE) == true)
    g_Audio.StopSoundClip(C_AUDIO_TANK_DRIVE);  

  if(m_timer.getTimer(0.1)==true){
    if(keyDown(VK_UP)){
      m_selection--;
      if(m_selection < 0)
        m_selection = 5;
    }

    if(keyDown(VK_DOWN)){
      m_selection++;
      if(m_selection > 5)
        m_selection = 0;
    }
  }

  if(keyDown(VK_RETURN)){
    switch(m_selection){
      case 0://New Mission
        m_event = GO_PLAY;
        g_gameLevel = 1;
        g_bResetGame = true;
        break;
      case 1://Replay Level
        //don't change g_gameLevel
        m_event = GO_PLAY;
        g_bResetGame = true;
        break;
      case 2://Instructions
        m_event = GO_HELP;
        break;
      case 3://High Score
        //m_event = GO_SCORE;
        break;
      case 4://Config

        break;
      case 5://Quit
        m_event = GO_QUIT;
        break;
    }
  }

  for(int i=0; i< m_TE.size(); i++){
    if (m_TE[i].event == m_event){
        return m_TE[i].p_gso;
    }
  }

  return 0;
}

void cGameStateMain::render(){
 //Clear buffer and draw graphics
  g_con.ClearBuffer(0, 0, 0, 0);
  g_con.BeginDrawing();

  RECT source;
  RECT dest;

  //new
  source = g_con.LoadRect(9, 313,241, 369);
  dest = g_con.LoadRect(g_con.GetScreenCenterX() - 116, g_con.GetScreenCenterY() - 181, g_con.GetScreenCenterX() - 130 + 232, g_con.GetScreenCenterY() -181 + 52);
  g_con.RenderTile(source, dest, 1.0f, 0);
  
  //replay
  source = g_con.LoadRect(4, 372,248, 424);
  dest = g_con.LoadRect(g_con.GetScreenCenterX() - 122, g_con.GetScreenCenterY() - 119, g_con.GetScreenCenterX() - 130 + 244, g_con.GetScreenCenterY() -181 + 52);
  g_con.RenderTile(source, dest, 1.0f, 0);

  //instruction
  source = g_con.LoadRect(261, 371,470, 423);
  dest = g_con.LoadRect(g_con.GetScreenCenterX() - 105, g_con.GetScreenCenterY() - 62, g_con.GetScreenCenterX() - 130 + 209, g_con.GetScreenCenterY() - 181 + 52);
  g_con.RenderTile(source, dest, 1.0f, 0);

  //high score
  source = g_con.LoadRect(249, 315,476, 367);
  dest = g_con.LoadRect(g_con.GetScreenCenterX() - 114, g_con.GetScreenCenterY() + 5, g_con.GetScreenCenterX() - 130 + 227, g_con.GetScreenCenterY() -181 + 52);
  g_con.RenderTile(source, dest, 1.0f, 0);

  //config
  source = g_con.LoadRect(3, 426,244, 478);
  dest = g_con.LoadRect(g_con.GetScreenCenterX() - 120, g_con.GetScreenCenterY() + 67, g_con.GetScreenCenterX() - 130 + 241, g_con.GetScreenCenterY() -181 + 52);
  g_con.RenderTile(source, dest, 1.0f, 0);

  //quit
  source = g_con.LoadRect(261, 428,464, 480);
  dest = g_con.LoadRect(g_con.GetScreenCenterX() - 102, g_con.GetScreenCenterY() + 129, g_con.GetScreenCenterX() - 130 + 203, g_con.GetScreenCenterY() -181 + 52);
  g_con.RenderTile(source, dest, 1.0f, 0);

  //selection outline
  source = g_con.LoadRect(170, 122, 440, 180);
  dest = g_con.LoadRect(g_con.GetScreenCenterX() - 134, g_con.GetScreenCenterY() - 186 + (m_selection * 62), g_con.GetScreenCenterX() - 130 + 270, g_con.GetScreenCenterY() - 244 + (m_selection * 62));
  g_con.RenderTile(source, dest, 1.0f, 0);


/*
  g_con.Draw2DText("<<<<<<<< RFR Main Menu >>>>>>>>", F_V16B, g_con.GetScreenCenterX() - 100, g_con.GetScreenCenterY() - 40,255,0,0);
  g_con.Draw2DText("Press S for High Score", F_V16B, g_con.GetScreenCenterX() - 100, g_con.GetScreenCenterY() - 20,255,0,0);
  g_con.Draw2DText("Press H for Help", F_V16B, g_con.GetScreenCenterX() - 100, g_con.GetScreenCenterY(),255,0,0);
  g_con.Draw2DText("Press ENTER to Play", F_V16B, g_con.GetScreenCenterX() - 100, g_con.GetScreenCenterY() + 20,255,0,0);
  g_con.Draw2DText("Press ESC to Quit", F_V16B, g_con.GetScreenCenterX() - 100, g_con.GetScreenCenterY() + 40,255,0,0);
  */      
  g_con.EndDrawing ();
  g_con.Present();
}

void cGameStateMain::processEvent(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam){

}

void cGameStateMain::deactivate(){
}
void cGameStateMain::resume(){
}
void cGameStateMain::pause(){
}
void cGameStateMain::save(){
}

void cGameStateMain::addTransitionEvent(int event, cGameStateObject* p_Next){
  TRANSITION_EVENT te;
  te.event=event;
  te.p_gso = p_Next;

  m_TE.push_back(te);
}