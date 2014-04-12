#include "..\includes\cgamestateintro.h"

extern CDXGraphics g_con;
extern CAudioManager g_Audio;
extern Log g_Log;

cGameStateIntro::cGameStateIntro(void){
}

cGameStateIntro::~cGameStateIntro(void){
}

void cGameStateIntro::initialize(){
  m_timer.initialize();
  m_bSplashComplete = false;
}

void cGameStateIntro::activate(){
    g_con.SetScreenWidthHeight(GetSystemMetrics(SM_CXFULLSCREEN),
                               GetSystemMetrics(SM_CYFULLSCREEN));

}


cGameStateObject* cGameStateIntro::update(){
  m_event = GO_NO_WHERE;
  static bool bOneShot = false;
   
  if(g_Audio.IsPlaying(C_AUDIO_MUSIC_INTRO)==false && bOneShot == false){
    g_Audio.PlaySoundClip(C_AUDIO_MUSIC_INTRO, 1);
    bOneShot = true;
  }

  if(keyDown(VK_SPACE) && m_bSplashComplete == true){
    m_event = GO_MAIN;
  }

  for(unsigned int i=0; i< m_TE.size(); i++){
    if (m_TE[i].event == m_event){
      return m_TE[i].p_gso;
    }
  }
  return 0;
}

void cGameStateIntro::render(){
 //Clear buffer and draw graphics
  g_con.ClearBuffer(0,0,0,0);
  g_con.BeginDrawing();
  
  static int nCount = 0;
  IDirect3DTexture9 * pTexture = g_con.GetTextureSplashPointer();

  if(m_timer.getTimer(0.01) == true){
    nCount ++;
    if(nCount >  g_con.GetScreenHeight()/2 ){
      nCount =  g_con.GetScreenHeight()/2;
      m_bSplashComplete = true;
    }
  }

  //this is stationary background
  RECT source = g_con.LoadRect(0,0,511,511);//511,511);
  RECT dest;
  dest = g_con.LoadRect(g_con.GetScreenCenterX() + 350 - nCount * 2,//200,//256,
                        g_con.GetScreenCenterY() - nCount,//,//256,
                        g_con.GetScreenCenterX() + 350 + nCount * 2,//,//256, 
                        g_con.GetScreenCenterY() + nCount);//256);
  g_con.DrawSprite(pTexture, &dest, &source,D3DCOLOR_RGBA(127,127,127,255));    //0);
 
  
  //g_con.Draw2DText("Music loop courtesy of http://www.deusx.com/freemusicloops.html", F_V12, 30, g_con.GetScreenHeight() - 60, 255,0,0);
  //g_con.Draw2DText("Splash fonts courtesy of http://cooltext.com", F_V12, 30, g_con.GetScreenHeight() - 40, 255,0,0);
  
  if(m_bSplashComplete == true)
    g_con.Draw2DText("Press SPACEBAR to continue...", F_V16B, g_con.GetScreenCenterX() - 125, g_con.GetScreenHeight() - 80, 0,0,0);
  
  g_con.EndDrawing ();
  g_con.Present();
}


void cGameStateIntro::processEvent(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam){
}

void cGameStateIntro::deactivate(){
  g_Audio.KillAll();
}
void cGameStateIntro::resume(){
}
void cGameStateIntro::pause(){
}
void cGameStateIntro::save(){
}

void cGameStateIntro::addTransitionEvent(int event, cGameStateObject* p_Next){
  TRANSITION_EVENT te;
  te.event=event;
  te.p_gso = p_Next;

  m_TE.push_back(te);
}