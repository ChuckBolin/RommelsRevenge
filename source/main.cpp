/**********************************************************************************
 main.cpp - Written by Chuck Bolin, August 2007
 Special Thanks to ...Original DX Texture code: Eamonn Doherty
 Tutorial: www.gamedev.net/reference/articles/article1972.asp
 Remember to link:
   d3d9.lib, d3dx9.lib winmm.lib
**********************************************************************************/
#include <Windows.h>
#include "..\includes\Graphics.h"
#include <string>
#include <sstream>
#include "..\resource.h"
#include "..\includes\CTimer.h"
#include "..\includes\CTank.h"
#include "..\includes\Keystatus.h"
#include "..\includes\CShellImpact.h"
#include "..\includes\CAudioManager.h"
#include "..\includes\CAudioConstants.h"
#include "..\includes\cGameStateObject.h"
#include "..\includes\CGameStatePlay.h"
#include "..\includes\cGameStateQuit.h"
#include "..\includes\cGameStateIntro.h"
#include "..\includes\cGameStateMain.h"
#include "..\includes\cGameStateScore.h"
#include "..\includes\cGameStateHelp.h"
#include "..\includes\Log.h"

HWND g_hWnd;      //Handle to the window the app is running in
BOOL g_bRunning;  //Always set when app is running. App will exit if flag becomes false
CTimer g_Timer; //used for FPS calculation
POINTS g_mouse; //Mouse g_mouse
CDXGraphics g_con; ////manages display to the console screen
CTimer g_LoopTimer;//
CAudioManager g_Audio;//
int g_SecondCount = 0;
int g_FireTime = 0;
bool g_TankFired = false;
Log g_Log;
CTimer g_FPS_Timer;
int g_FPS_Counter;

//manages game states
cGameStateObject* g_pStatePlay = new CGameStatePlay;
cGameStateObject* g_pStateHelp = new cGameStateHelp;
cGameStateObject* g_pStateMain = new cGameStateMain;
cGameStateObject* g_pStateIntro = new cGameStateIntro;
cGameStateObject* g_pStateScore = new cGameStateScore;
cGameStateObject* g_pStateQuit = new cGameStateQuit;
cGameStateObject* g_pCurrent = 0;
cGameStateObject* g_pLast = 0; 
cGameStateObject* g_pNext = 0;

//********************************
//* Function Prototypes          *
//********************************

//Make a window
int MakeWindow(HINSTANCE hInstance);

//Window procedure
LRESULT CALLBACK WindowProcedure(HWND hWindow,UINT uMessage,WPARAM wParam,LPARAM lParam);

//Entry point
int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
  
  MSG msg;    
  g_bRunning = true;//Start program execution

  //Make a window and initialize DirectX in windowed mode
  MakeWindow(hInstance);

  //map game state information
  g_pStateIntro->addTransitionEvent(GO_MAIN, g_pStateMain);//
  g_pStateMain->addTransitionEvent(GO_PLAY, g_pStatePlay);//
  g_pStateMain->addTransitionEvent(GO_HELP, g_pStateHelp);//
  g_pStateMain->addTransitionEvent(GO_QUIT, g_pStateQuit);
  //g_pStateMain->addTransitionEvent(GO_SCORE, g_pStateScore);//  
  g_pStateScore->addTransitionEvent(GO_MAIN, g_pStateMain);//
  g_pStatePlay->addTransitionEvent(GO_MAIN, g_pStateMain);//
  g_pStateHelp->addTransitionEvent(GO_MAIN, g_pStateMain);//
  g_Log.Write("Game state transitions events");
  g_Log.Write("g_pStateIntro",g_pStateIntro);
  g_Log.Write("g_pStateMain", g_pStateMain);
  g_Log.Write("g_pStatePlay", g_pStatePlay);
  g_Log.Write("g_pStateScore",g_pStateScore);
  g_Log.Write("g_pStateHelp", g_pStateHelp);

  g_Timer.initialize();
  g_LoopTimer.initialize();
  g_FPS_Timer.initialize();
  g_con.InitD3D (0, 0, D3DFMT_A8R8G8B8, g_hWnd, true);
  
  IDirect3DTexture9 * pTexture =  g_con.LoadTexture("assets\\artwork\\sand3.png");
  IDirect3DTexture9 * pSplashTexture =  g_con.LoadTextureSplash("assets\\artwork\\splash1.jpg");
  g_con.SetTexturePointer(pTexture);
  g_con.SetTextureSplashPointer(pSplashTexture);

  g_con.LoadSprite("assets\\artwork\\tank1.png");
  g_Log.Write("Sprite (tank1.png) loaded!");
  g_con.LoadSpriteMore("assets\\artwork\\buttons2.png");
  g_Log.Write("Sprite (buttons2.png) loaded!");

  //used for framerate display
  std::ostringstream oss;
  std::string sFramerate;
  std::string sText;
  int nDisplayFPSCount = 0;
  int dx = 0; //used to convert object coordinates to screen coordinates
  int dy = 0;

  //initialize audio manager
  g_Audio.LoadSample(C_AUDIO_TANK_IDLE, "assets\\sounds\\tank_idle.wav");
  g_Audio.LoadSample(C_AUDIO_TANK_ACC,"assets\\sounds\\tank_acc.wav");
  g_Audio.LoadSample(C_AUDIO_TANK_DRIVE, "assets\\sounds\\tank_driving.wav");
  g_Audio.LoadSample(C_AUDIO_TANK_DEACC, "assets\\sounds\\tank_deacc.wav");
  g_Audio.LoadSample(C_AUDIO_TANK_SHOOT, "assets\\sounds\\tank_shoot.mp3");
  g_Audio.LoadSample(C_AUDIO_TURRET, "assets\\sounds\\tank_turret.mp3");
  g_Audio.LoadSample(C_AUDIO_ENEMY_DRIVE, "assets\\sounds\\enemy.mp3");
  g_Audio.LoadSample(C_AUDIO_TANK_EXPLODE, "assets\\sounds\\tank_explode.wav");
  g_Audio.LoadSample(C_AUDIO_MUSIC_INTRO, "assets\\sounds\\intro.mp3");
  g_Audio.LoadSample(C_AUDIO_MUSIC_PLAY, "assets\\sounds\\play.mp3");
  g_Audio.LoadSample(C_AUDIO_MUSIC_CREDITS, "assets\\sounds\\credits.mp3");
  g_Audio.LoadSample(C_AUDIO_EXPLOSION_NEAR, "assets\\sounds\\explode_close.mp3");
  g_Audio.LoadSample(C_AUDIO_WAR_ENDS, "assets\\sounds\\war_ends.wav");
  g_Audio.LoadSample(C_AUDIO_P51, "assets\\sounds\\51.wav");
  g_Audio.LoadSample(C_AUDIO_EXPLOSION_DISTANT, "assets\\sounds\\explosion_distant2.wav");
  g_Audio.LoadSample(C_AUDIO_PANZER, "assets\\sounds\\panzerlied8.mp3");
  
  for(int i = 0; i < g_Audio.Size(); i++){
    g_Log.Write(g_Audio.GetFilename(i), i); //write filename and file index to log file
  }
  g_Log.Write("Audio samples loaded!");
  g_Log.Write("Main game loop entered!");

  //Main application loop
  while (g_bRunning)
  {
      //Handle messages
      if (PeekMessage (&msg,NULL,0,0,PM_REMOVE)){
          TranslateMessage (&msg);
          DispatchMessage (&msg);
      }        
      else{
        if(g_LoopTimer.getTimer(0.025)){
          g_pLast = g_pCurrent;
          g_pNext = g_pCurrent->update();
          if(g_pNext == NULL){
            //no change
          }
          else{
            g_pCurrent->deactivate();
            g_pCurrent = g_pNext;
            g_pCurrent->activate();
          }

        }
        g_pCurrent->render();
  
        if(g_FPS_Timer.getTimer(1))
          g_FPS_Counter++;

        //display frame rate in caption
        nDisplayFPSCount++;
        if(nDisplayFPSCount > 30){
          nDisplayFPSCount = 0;
          oss.str(""); //clear oss
          oss << (int) (30/g_Timer.getTimeDifference());
          if((g_FPS_Counter % 15) == 0)
            g_Log.Write("FPS",oss.str());
            sFramerate = "Rommel\'s Fantasy Revenge, v. 0.41";//,  " + oss.str() + " FPS";
          ::SetWindowText(g_hWnd, sFramerate.c_str());
        }//if(nDisplayFPS...
      }//else
  }//while(...
  
  g_con.CloseD3D();//Close DirectX
  g_Log.Write("DirectX closed!");
  g_Audio.KillAll();
  ::Sleep(2000);
  ::ShowCursor(true); //turn mouse cursor back on
  g_Log.Write("Audio sounds are all killed!");
  g_Log.Write("***************************");
  g_Log.Write("Program ended!");
  g_Log.Write("***************************");
  return 0;//(int)msg.wParam;  //returns exit code to operating system
} 
  
/*************************************************************************
Window Procedure - Process Windows events

*************************************************************************/
LRESULT CALLBACK WindowProcedure (HWND g_hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam)
{
    //Check message
    switch(uMessage)
    {
    case WM_CREATE:
      g_pStateIntro->initialize();
      g_pStateMain->initialize();
      g_pStatePlay->initialize();    
      g_pStateQuit->initialize();
      g_pCurrent = g_pStateIntro;
      g_pStateIntro->activate();
      g_Log.Write("Game states initialized!");
      g_Log.Write("Game State = Intro");

      return 0;

    case WM_DESTROY:
        g_bRunning = false;
        delete g_pStatePlay;
        delete g_pCurrent;
        delete g_pLast;
        delete g_pNext;
        delete g_pStateQuit;
        delete g_pStateHelp;
        delete g_pStateMain;
        delete g_pStateIntro;
        delete g_pStateScore;
        g_Log.Write("Game state pointers deleted!");
        break;
    
    case WM_KEYDOWN:
    case WM_KEYUP:
      //switch(wParam){
      //  case VK_SPACE:
       //   if(g_pCurrent == g_pStateQuit)        
       //     g_bRunning = false;
      //    break;
      //}
      break;

    case WM_LBUTTONDOWN:
        break;	

    case WM_LBUTTONUP:
        break;

    case WM_RBUTTONDOWN:
        break;

    case WM_RBUTTONUP:
        break;	
    }

    return DefWindowProc(g_hWnd,uMessage,wParam,lParam);
}

/*************************************************************************
MakeWindow - Creates window
*************************************************************************/
int MakeWindow (HINSTANCE hInstance)
{
    WNDCLASSEX wc;

    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WindowProcedure;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = NULL;
    wc.hCursor = ::LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CURSOR1));   //NULL;//(HCURSOR)IDC_CROSS;
    wc.hbrBackground = (HBRUSH) GetStockObject (BLACK_BRUSH);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = "RFR";
    wc.hIconSm = LoadIcon (hInstance, MAKEINTRESOURCE(IDI_ICON1)); 
    
    if(!RegisterClassEx(&wc))
        return false;

    //screen data
    int cxScreen = ::GetSystemMetrics(SM_CXFULLSCREEN);
    int cyScreen = ::GetSystemMetrics(SM_CYFULLSCREEN);
    g_con.SetScreenWidthHeight(cxScreen, cyScreen);
    g_Log.Write("*************************************");
    g_Log.Write("Rommel's Fantasy Revenge...Starting...");
    g_Log.WriteDate();
    g_Log.WriteTime();
    g_Log.Write("*************************************");
    g_Log.Write("ScreenWidth",cxScreen);
    g_Log.Write("ScreenHeight",cyScreen);
    
    //Create window
    g_hWnd = CreateWindowEx (NULL, "RFR", " ",
        WS_CAPTION | WS_VISIBLE | WS_SYSMENU, 0, 0, g_con.GetScreenWidth(), g_con.GetScreenHeight(), //800, 600,
        NULL, NULL, hInstance, NULL);
    g_Log.Write("CreateWindowEx");

    if (!g_hWnd){//Make sure window was created properly
      g_Log.Write("Window failed to create!");
      return false;
    }
    g_Log.Write("Window creation successful!");
    ::ShowCursor(false);
    return true;//Success
}

