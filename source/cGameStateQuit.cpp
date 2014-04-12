//cGameStateQuit.cpp
#include "..\includes\cgamestatequit.h"
#include "..\includes\keystatus.h"

extern CDXGraphics g_con;
extern CAudioManager g_Audio;
extern Log g_Log;
extern BOOL g_bRunning;
cGameStateQuit::cGameStateQuit(void)
{
}

cGameStateQuit::~cGameStateQuit(void)
{
}

void cGameStateQuit::initialize(){
  std::string text;
  text = "Rommel's Fantasy Revenge"; m_msg.push_back(text);
  text = "Created by";                m_msg.push_back(text);
  text = "Chuck Bolin";               m_msg.push_back(text);
  text = "October 13, 2007";          m_msg.push_back(text);
  text = "";                          m_msg.push_back(text);
  text = ""; m_msg.push_back(text);
  text = "Official Entry";            m_msg.push_back(text);
  text = "Game Challenge Number 3";   m_msg.push_back(text);
  text = "Top-Down Shooter 2D";       m_msg.push_back(text);
  text = "www.gameinstitute.com";     m_msg.push_back(text);
  text = "";                          m_msg.push_back(text);
  text = ""; m_msg.push_back(text);
  text = "Special Thanks To......";   m_msg.push_back(text);
  text = "";                          m_msg.push_back(text);
  text = "Grace Bolin....................World War II Historian"; m_msg.push_back(text);
  text = ""; m_msg.push_back(text);
  text = "Grace Bolin......For her unfailing support and encouragement in this and all endeavors"; m_msg.push_back(text);
  text = ""; m_msg.push_back(text);
  text = "Sara and Sam..............For living with a disengaged father for four weeks"; m_msg.push_back(text);
  text = ""; m_msg.push_back(text);
  text = "Zaid.................................Original Tank Sprites";  m_msg.push_back(text);
  text = ""; m_msg.push_back(text);
  text = "Eamonn Doherty........................DirectX Texture Sample Code"; m_msg.push_back(text);
  text = ""; m_msg.push_back(text);
  text = "Steve Jones...........................Tip for creating tank shadow"; m_msg.push_back(text);
  text = ""; m_msg.push_back(text);
  text = "Myself................................Bugs, bugs and more bugs"; m_msg.push_back(text);
  text = ""; m_msg.push_back(text);
  text = "www.fmod.org..........................Sound Library"; m_msg.push_back(text);
  text = ""; m_msg.push_back(text);
  text = "www.deusx.com/freemusicloops.html.....Music Loops"; m_msg.push_back(text);
  text = ""; m_msg.push_back(text);
  text = "http://cooltext.com...................Splash Screen Fonts"; m_msg.push_back(text);
  text = ""; m_msg.push_back(text);
  text = "www.sounddogs.com.....................Tank Sounds"; m_msg.push_back(text);
  text = ""; m_msg.push_back(text);
  text = "LPaul...............................Music Loop Intro"; m_msg.push_back(text);
  text = ""; m_msg.push_back(text);
  text = "Boss Bat-Sir Shir...................Music Loop Play"; m_msg.push_back(text);
  text = ""; m_msg.push_back(text);
  text = "Metal_Me - Fatal_Si.................Music Loop Credits"; m_msg.push_back(text);
  text = ""; m_msg.push_back(text);
  text = "GI Students.........................Encouragement"; m_msg.push_back(text);
  text = ""; m_msg.push_back(text);
  text = "GI Judges (Jon and Drew)........You guys ROCK! =)"; m_msg.push_back(text);
  text = ""; m_msg.push_back(text);
  text = "Sam Bolin...........................Alpha Tester"; m_msg.push_back(text);
  text = ""; m_msg.push_back(text);
  text = "Isaac Deloach...........................Alpha Tester"; m_msg.push_back(text);
  text = ""; m_msg.push_back(text);
  text = "Game Institute Instructors..............Training in the Basics"; m_msg.push_back(text);
  text = ""; m_msg.push_back(text);
  text = "  Frank Luna...........................C++"; m_msg.push_back(text);
  text = ""; m_msg.push_back(text);
  text = "  Gary Simmons..............DirectX"; m_msg.push_back(text);
  text = ""; m_msg.push_back(text);
  text = "  Adam Hoult................DirectX"; m_msg.push_back(text);
  text = ""; m_msg.push_back(text);  
  text = "Neighbors......I haven't cut my lawn in 4 weeks =("; m_msg.push_back(text);
  text = ""; m_msg.push_back(text);
  text = "Written and compiled with Microsoft Visual Studio.net 2003"; m_msg.push_back(text);
  text = ""; m_msg.push_back(text);
  text = "Graphics - DirectX 9"; m_msg.push_back(text);
  text = ""; m_msg.push_back(text);
  text = "Physics (my original code...can't you tell? )"; m_msg.push_back(text);
  text = ""; m_msg.push_back(text);
  text = "Artificial Intelligence (my code...can you say waypoints?)"; m_msg.push_back(text);
  text = ""; m_msg.push_back(text);
  text = ""; m_msg.push_back(text);
  text = ""; m_msg.push_back(text);
  text = ""; m_msg.push_back(text);
  text = "Aren't scrolling credits awesome? "; m_msg.push_back(text);
  
  m_timer.initialize(); 
  m_scrollValue = 0;

}

void cGameStateQuit::activate(){
  //if(g_Audio.IsPlaying(C_AUDIO_MUSIC_PLAY) == true)
   m_scrollValue = 0;
   //g_Audio.KillAll();
   if(g_Audio.IsPlaying(C_AUDIO_MUSIC_CREDITS) == false)
     g_Audio.PlaySoundClip(C_AUDIO_MUSIC_CREDITS,0);
    
}


cGameStateObject* cGameStateQuit::update(){
  m_event = GO_NO_WHERE;
  //g_Audio.PauseAll();
   //g_Audio.KillAll();

  if(keyDown(VK_SPACE) && m_scrollValue > 50){
    if(g_Audio.IsPlaying(C_AUDIO_MUSIC_CREDITS) == true)
     g_Audio.StopSoundClip(C_AUDIO_MUSIC_CREDITS);
    else{
      m_event = GO_QUIT;
      g_bRunning = false;
    }
  }

  //1/10th of a second
  if(m_timer.getTimer(0.01) == true){
    m_scrollValue += 2;  
  }

  for(unsigned int i=0; i< m_TE.size(); i++){
    if (m_TE[i].event == m_event){
      return m_TE[i].p_gso;
    }
  }
  return 0;
}

void cGameStateQuit::render(){
 //Clear buffer and draw graphics
  g_con.ClearBuffer(0, 0, 0, 0);
  g_con.BeginDrawing();
  
  int posY = 0;

  for(int i = 0; i < m_msg.size(); i++){
    posY = m_scrollValue + ((int)m_msg.size() - i * 20);
    if(posY < g_con.GetScreenHeight() + 20){
      g_con.Draw2DText(m_msg[i], F_V20, 200, g_con.GetScreenHeight() - posY,0,255,255);
    }
  }

  if(m_scrollValue > 50)
    g_con.Draw2DText("Press SPACEBAR to Quit", F_V20,g_con.GetScreenCenterX() + 300 , g_con.GetScreenCenterY(),255,255,255);
  
  g_con.EndDrawing ();
  g_con.Present();
}

void cGameStateQuit::deactivate(){
}
void cGameStateQuit::resume(){
}
void cGameStateQuit::pause(){
}
void cGameStateQuit::save(){
}

void cGameStateQuit::addTransitionEvent(int event, cGameStateObject* p_Next){
  TRANSITION_EVENT te;
  te.event=event;
  te.p_gso = p_Next;

  m_TE.push_back(te);
}