#include "..\includes\CGameStatePlay.h"
#include "..\includes\keystatus.h"


extern Log g_Log;
extern CDXGraphics g_con;
extern CAudioManager g_Audio;//
CShellImpact g_Impact;
int g_gameLevel;
extern bool g_bResetGame;

CGameStatePlay::CGameStatePlay(void){
  m_bPaused = false;
  srand(time(0));
}

CGameStatePlay::~CGameStatePlay(void){
  delete[] m_pObject;
}

//called to start each level
void CGameStatePlay::InitializeNextLevel(){

  g_Log.Write("Loading level",m_level);

  for(int i = 0; i < m_maxEnemyTanks + 1; i++)
    m_pObject[i].ClearWaypoints();

  g_Log.Write("Waypoints cleared");

  g_gameLevel = m_level;

  switch(m_level){
    case 1:
      m_file.loadLevel("levels\\level1.dat");
      m_validFile = m_file.isValid();
      if(m_validFile == true)
        addLevel(1);
      break;
    case 2:
      m_file.loadLevel("levels\\level2.dat");
      m_validFile = m_file.isValid();
      if(m_validFile == true)
        addLevel(2);
      break;
    case 3:
      m_file.loadLevel("levels\\level3.dat");
      m_validFile = m_file.isValid();
      if(m_validFile == true)
        addLevel(3);
      break;
    case 4:
      m_file.loadLevel("levels\\level4.dat");
      m_validFile = m_file.isValid();
      if(m_validFile == true)
        addLevel(4);
      break;
    case 5:
      m_file.loadLevel("levels\\level5.dat");
      m_validFile = m_file.isValid();
      if(m_validFile == true)
        addLevel(5);
      break;
  }
  
  if(m_validFile == true)
    g_Log.Write("File is valid");
  else
    g_Log.Write("File is invalid");

  //initialize for each level
  m_pObject[0].SetStatus(C_STATUS_OK);
  m_pObject[0].SetType(C_TYPE_PLAYER);
  m_pObject[0].SetAmmoCount(C_TANK_INITIAL_AMMO_PLAYER);
  m_pObject[0].SetTurretElevation(300);

  for(int i = 1; i < m_maxEnemyTanks + 1; i++){
    m_pObject[i].m_firingInterval = ((6 - m_level) * 10) + (rand() % 30);
    m_pObject[i].SetStatus(C_STATUS_OK);
    m_pObject[i].SetType(C_TYPE_ENEMY);
    m_pObject[i].SetTankState(C_TANK_STATE_PATROL);
    m_pObject[i].Initialize();
    m_pObject[i].SetAmmoCount(C_TANK_INITIAL_AMMO_ENEMY);
    if(i < 6)
      m_pObject[i].m_countryFlag = C_COUNTRY_US;
    else
      m_pObject[i].m_countryFlag = C_COUNTRY_UK;
  }

  m_secondCount = 0;
  m_kills = 0;
  m_fireTime = 0;
  m_bTankFired = false;
  m_levelComplete = false;
  m_EndLevel = 0;
  g_Impact.Clear();
  g_bResetGame = false;
  m_gameOver = false;
  m_bWinner = false;
  m_ironCross = 1;
  deathCount = 0;
}

void CGameStatePlay::initialize(){ 
  m_maxEnemyTanks = C_PLAY_ENEMYCOUNT_LEVEL;//130;
  m_pObject = new CTank[C_PLAY_ENEMYCOUNT_LEVEL + 1];//[131];
  m_secondTimer.initialize();
  m_LevelTimer.initialize();
  m_bKeyTPressed = false;
  m_gameOver = false;
  m_bWinner = false;
  g_gameLevel = 1;
  m_level = 1;
  //m_bPauseReleased = true;
  m_pauseTimer.initialize();
  m_bOneshot = false;
  //InitializeNextLevel();
  m_winTimer.initialize();
  deathCount = 0;
}

void CGameStatePlay::activate(){
}

/*************************************  U P D A T E *****************************************/
cGameStateObject* CGameStatePlay::update(){
  m_event = GO_NO_WHERE;
  float randomOffset = 0;
  POINT tempPoint;
  
  if(g_bResetGame == true){
    if(g_gameLevel == 1)
      m_level = 1;
    else
      m_level = g_gameLevel;
    InitializeNextLevel();
  }

  if(m_LevelTimer.getTimer(1) == true){//called every second
    if(m_levelComplete == true){
      m_EndLevel++;
      if(m_level == C_PLAY_MAX_LEVEL){
        m_bWinner = true;
        g_Log.Write("Player Wins");
      }
      else if(m_EndLevel  == 5){
        m_level++;
        //g_Impact.Clear();
        g_Log.Write("Initialize Next Level");
        InitializeNextLevel();
        m_levelComplete = false;
      }
    }
  }

  if(keyDown(VK_ESCAPE) == true ){
    m_event = GO_MAIN;
  }

  //game state transition code...very important
  for(unsigned int i=0; i< m_TE.size(); i++){
    if (m_TE[i].event == m_event){
      return m_TE[i].p_gso;
    }
  }

  //this adds all sorts of fire effects to destroyed
  //players tank
  if(m_gameOver == true){
    if(g_Audio.IsPlaying(C_AUDIO_MUSIC_PLAY) == true)
      g_Audio.StopSoundClip(C_AUDIO_MUSIC_PLAY);

    deathCount++;
    if(deathCount < 100){
      tempPoint.x = m_pObject[0].GetTankX() + rand() % 32; //16
      tempPoint.y = m_pObject[0].GetTankY() + rand() % 32; //8
      g_Impact.ImpactExplosion(tempPoint, 0);
    }
    return this;
  }

  if(m_bWinner == true){
    return this;
  }

  if(m_pObject[0].GetAmmoCount() < 1 && m_kills < C_PLAY_ENEMYCOUNT_LEVEL){
    m_gameOver = true;
  }

  if(m_kills == C_PLAY_ENEMYCOUNT_LEVEL && m_levelComplete == false && m_gameOver == false){
    m_levelComplete = true;
    m_EndLevel = 0;
    return this;
  }
  else if(m_levelComplete == true)
    return this;

  POINT impactPoint;
 
  //process keys
  if(keyDown(VK_LEFT)){
    m_pObject[0].MoveTurretLeft();
    if(g_Audio.IsPlaying(C_AUDIO_TURRET) == false){
      g_Audio.SetVolume(C_AUDIO_TURRET, 60);    
      g_Audio.PlaySoundClip(C_AUDIO_TURRET, 0);
    }
  }
  else if(keyDown(VK_RIGHT)){
    m_pObject[0].MoveTurretRight();
    if(g_Audio.IsPlaying(C_AUDIO_TURRET) == false){
      g_Audio.SetVolume(C_AUDIO_TURRET, 255);
      g_Audio.PlaySoundClip(C_AUDIO_TURRET, 0);
    }
  }

  if(keyDown(VK_UP)){
    m_pObject[0].MoveTurretUp();
    if(g_Audio.IsPlaying(C_AUDIO_TURRET) == false)
      g_Audio.PlaySoundClip(C_AUDIO_TURRET, 0);

  }
  else if(keyDown(VK_DOWN)){
    m_pObject[0].MoveTurretDown();
    if(g_Audio.IsPlaying(C_AUDIO_TURRET) == false)
      g_Audio.PlaySoundClip(C_AUDIO_TURRET, 0);
  }

  if(keyDown('A')){
    m_pObject[0].MoveLeft();
  }

  if(keyDown('D')){
    m_pObject[0].MoveRight();
  }

  if(keyDown('W')){
    m_pObject[0].MoveForward();
    if(g_Audio.IsPlaying(C_AUDIO_TANK_ACC) == false){
      if(g_Audio.IsPlaying(C_AUDIO_TANK_DRIVE) == true)
        g_Audio.StopSoundClip(C_AUDIO_TANK_DRIVE);
      if(g_Audio.IsPlaying(C_AUDIO_TANK_DEACC) == true)
        g_Audio.StopSoundClip(C_AUDIO_TANK_DEACC);
      g_Audio.PlaySoundClip(C_AUDIO_TANK_ACC, 0);
    }
  }

  if(keyDown('S')){
    m_pObject[0].MoveReverse();
    if(g_Audio.IsPlaying(C_AUDIO_TANK_DEACC) == false){
      if(g_Audio.IsPlaying(C_AUDIO_TANK_DRIVE) == true)
        g_Audio.StopSoundClip(C_AUDIO_TANK_DRIVE);
      if(g_Audio.IsPlaying(C_AUDIO_TANK_ACC) == true)
        g_Audio.StopSoundClip(C_AUDIO_TANK_ACC);
      g_Audio.PlaySoundClip(C_AUDIO_TANK_DEACC, 0);
    }
  }

  //pause code
  if(m_pauseTimer.getTimer(0.1) == true)    
    m_pauseCount++;

  if(keyDown('P')== true && m_bPaused == false && m_pauseCount > 5){
    m_bPaused = true;
    m_pauseCount = 0;
  }
  else if(keyDown('P') == false && m_bPaused == true){
    m_bOneshot = true;
  }
  else if(keyDown('P') == true && m_bPaused == true && m_bOneshot == true && m_pauseCount > 5){
    m_bOneshot = false;
    m_bPaused = false;
    m_pauseCount = 0;
  }

  if(keyDown('T')){
    if(m_bKeyTPressed == false)
      m_bKeyTPressed = true;
    else
      m_bKeyTPressed = false;
  }

  //1 second timer - controls minimum firing interval for player
  if(m_secondTimer.getTimer(1)){
    m_secondCount++;

    //allow player to fire if time has elapsed between rounds
    if(m_bTankFired == true && m_secondCount > m_fireTime + 1){// 4){
      m_bTankFired = false;
    }
  }

  if(m_bPaused == false){
    //player shoots...uses 1 second timer to monitor firing rate
    if(keyDown(VK_SPACE) && m_gameOver == false){
      if(m_bTankFired == false){
        if(m_pObject[0].GetAmmoCount() > 0){
          if(m_pObject[0].GetTargetX() > CWORLD_LEFT_LIMIT &&
             m_pObject[0].GetTargetX() < CWORLD_RIGHT_LIMIT &&
             m_pObject[0].GetTargetY() > CWORLD_TOP_LIMIT &&
             m_pObject[0].GetTargetY() < CWORLD_BOTTOM_LIMIT){
              m_pObject[0].DecAmmoCount();
              m_fireTime = m_secondCount;
              m_bTankFired  = true;
              m_pObject[0].m_bTankFiresRound = true;
              tempPoint.x = m_pObject[0].GetTargetX() + 16;
              tempPoint.y = m_pObject[0].GetTargetY() + 8;
              g_Impact.ImpactExplosion(tempPoint, 0);
              
              g_Audio.PlaySoundClip(C_AUDIO_TANK_SHOOT, 0);
              g_Audio.PlaySoundClip(C_AUDIO_EXPLOSION_DISTANT, 0);
             }
        }
      }
    }
    else if(!keyDown(VK_SPACE)){
      m_pObject[0].m_bTankFiresRound = false;
    }

    //updates elevation and azimuth of turrent
    //to move target reticle near Tiger tank
    for(int k = 1; k < m_maxEnemyTanks + 1; k++){
      m_Physics.UpdateTankTarget(m_pObject[0], m_pObject[k]);
    }

    //update player and enemy tanks
    for(int k = 0; k < m_maxEnemyTanks + 1; k++){
      m_pObject[k].Update();
    }

    if(m_pObject[0].m_tankDir > 2 * PI)
      m_pObject[0].m_tankDir = 0;
    else if(m_pObject[0].m_tankDir < 0)
      m_pObject[0].m_tankDir = 2 * PI;

    if(m_pObject[0].m_turretAz > 2 * PI)
      m_pObject[0].m_turretAz = 0;
    else if(m_pObject[0].m_turretAz < 0)
      m_pObject[0].m_turretAz = 2 * PI;

    //add impact if enemy tank fires round
    for(int k = 1; k < m_maxEnemyTanks + 1; k++){
      if(m_pObject[k].m_bTankFiresRound == true){
        if(m_pObject[k].m_bExplosion == false){
          m_pObject[k].m_bExplosion = true;
          tempPoint.x = m_pObject[k].GetTargetX() + 16;
          tempPoint.y = m_pObject[k].GetTargetY() + 8;
          g_Impact.ImpactExplosion(tempPoint, 0);
          if(g_Audio.IsPlaying(C_AUDIO_EXPLOSION_DISTANT) == false)
            g_Audio.PlaySoundClip(C_AUDIO_EXPLOSION_DISTANT, 0);
        }
      }      
    }

    for(int i = 0; i < g_Impact.Size(); i++){
      g_Impact.Update(i);  //updates animated frame for explosion
    }

    //physics...boundary enforcement
    for(int k = 0; k < m_maxEnemyTanks + 1; k++)
      m_Physics.CheckWorldBoundary(m_pObject[k]);

    //physics...collision detection
    for(int j = 0; j < m_maxEnemyTanks + 1; j++){
      for(int k = 0; k < m_maxEnemyTanks + 1; k++){
        if(j != k){
          m_Physics.CheckCollision(m_pObject[j], m_pObject[k]);
          
          //player shoots enemy tanks
          if(m_Physics.CheckAShootsB(m_pObject[j], m_pObject[k]) == true){
            if(j == 0){
              if(g_Audio.IsPlaying(C_AUDIO_EXPLOSION_NEAR) == false)
                g_Audio.PlaySoundClip(C_AUDIO_EXPLOSION_NEAR, 0);
                //g_Audio.PlaySoundClip(C_AUDIO_EXPLOSION_DISTANT, 0);
              m_kills++;
            }
            if(k==0){
              //g_Audio.PlaySoundClip(C_AUDIO_MUSIC_PLAY, 0);
              g_Audio.StopSoundClip(C_AUDIO_MUSIC_PLAY);
              g_Audio.PlaySoundClip(C_AUDIO_TANK_EXPLODE, 0);
              g_Audio.PlaySoundClip(C_AUDIO_WAR_ENDS, 0);
              
              m_gameOver = true;
            }
          }
        }
      }
    }

    //audio looping
    if(m_pObject[0].m_tankAcc > 0.01){//acc
      g_Audio.StopSoundClip(C_AUDIO_TANK_DRIVE);
      g_Audio.StopSoundClip(C_AUDIO_TANK_IDLE);
      g_Audio.StopSoundClip(C_AUDIO_TANK_DEACC);
      if(g_Audio.IsPlaying(C_AUDIO_TANK_ACC) == false)
        g_Audio.PlaySoundClip(C_AUDIO_TANK_ACC, 0);
    }
    else if(m_pObject[0].m_tankVel < -0.01){//play deacc
      g_Audio.StopSoundClip(C_AUDIO_TANK_DRIVE);
      g_Audio.StopSoundClip(C_AUDIO_TANK_IDLE);
      g_Audio.StopSoundClip(C_AUDIO_TANK_ACC);
      if(g_Audio.IsPlaying(C_AUDIO_TANK_DEACC) == false)
        g_Audio.PlaySoundClip(C_AUDIO_TANK_DEACC, 0);
    }
    else if(m_pObject[0].m_tankVel > 0.3 || m_pObject[0].m_tankVel < -0.3){//play tank_driving
      g_Audio.StopSoundClip(C_AUDIO_TANK_ACC);
      g_Audio.StopSoundClip(C_AUDIO_TANK_IDLE);
      g_Audio.StopSoundClip(C_AUDIO_TANK_DEACC);
      if(g_Audio.IsPlaying(C_AUDIO_TANK_DRIVE) == false)
        g_Audio.PlaySoundClip(C_AUDIO_TANK_DRIVE, 0);
    }
    else{//tank idle
      g_Audio.StopSoundClip(C_AUDIO_TANK_DRIVE);
      g_Audio.StopSoundClip(C_AUDIO_TANK_ACC);
      g_Audio.StopSoundClip(C_AUDIO_TANK_DEACC);
      if(g_Audio.IsPlaying(C_AUDIO_TANK_IDLE) == false)
        g_Audio.PlaySoundClip(C_AUDIO_TANK_IDLE, 0);
    }

    if(g_Audio.IsPlaying(C_AUDIO_MUSIC_PLAY) == false && m_bWinner == false)
      g_Audio.PlaySoundClip(C_AUDIO_MUSIC_PLAY, 0);

  }
  return 0;
}

/************************************** R E N D E R ***************************************/
void CGameStatePlay::render(){
  int dx = 0; //used to convert object coordinates to screen coordinates
  int dy = 0;
  POINT tempPoint;
  IDirect3DTexture9 * pTexture = g_con.GetTexturePointer();
  IDirect3DTexture9 * pTextureMore = g_con.GetTexturePointerMore();

  //Clear buffer and draw graphics
  g_con.ClearBuffer(0, 0, 0, 0);
  g_con.BeginDrawing();

  //this is stationary background
  RECT source = g_con.LoadRect(0,0,511,511);//511,511);
  RECT dest;
  for(int i = 0; i < 4; i++){
    for(int j = 0; j < 4; j++){
      dest = g_con.LoadRect( -(m_pObject[0].GetTankX() - g_con.GetScreenCenterX()) + (i * 512),
                              -(m_pObject[0].GetTankY() - g_con.GetScreenCenterY()) + (j * 512),
                              -(m_pObject[0].GetTankX() - g_con.GetScreenCenterX()) + 512 + (i * 512),
                              -(m_pObject[0].GetTankY() - g_con.GetScreenCenterY()) + 512 + (j * 512));

      g_con.DrawSprite(pTexture, &dest, &source, 0);
    }
  }

  //g_con.DrawCircle(1600, 1600, 25);//m_pObject[1].GetTankX() + 100, m_pObject[1].GetTankY() + 100, 25.0f);

  //try to render explosions
  if(m_gameOver == false){
    for(int i = 0; i < g_Impact.Size(); i++){
      dest =  g_Impact.GetImpactDestination(i);
      source = g_Impact.GetImageSource(i);
      dx = dest.left - m_pObject[0].GetTankX();
      dy = dest.top - m_pObject[0].GetTankY();
      dest.left = g_con.GetScreenCenterX() + dx;
      dest.right = dest.left + 32;
      dest.top = g_con.GetScreenCenterY() + dy;
      dest.bottom = dest.top + 32;
      g_con.RenderTile(source, dest, 1.0f, 0);
    }
  }

  //render enemy tanks
  for(int i = 1; i < m_maxEnemyTanks + 1; i++)
    g_con.RenderTank(m_pObject[i], m_pObject[0]);

  g_con.RenderTank(m_pObject[0], m_pObject[0]);
  if(m_gameOver == true){ //used only when player tank is destroyed
    for(int i = 0; i < g_Impact.Size(); i++){
      dest =  g_Impact.GetImpactDestination(i);
      source = g_Impact.GetImageSource(i);
      dx = dest.left - m_pObject[0].GetTankX();
      dy = dest.top - m_pObject[0].GetTankY();
      dest.left = g_con.GetScreenCenterX() + dx;
      dest.right = dest.left + 32;
      dest.top = g_con.GetScreenCenterY() + dy;
      dest.bottom = dest.top + 32;
      g_con.RenderTile(source, dest, 1.0f, 0);
    }
  }

  VECTOR2D impactData;
  CIRCLE2D playerData;
  playerData.center.x = m_pObject[0].GetTankX();
  playerData.center.y = m_pObject[0].GetTankY();
  playerData.radius = m_pObject[0].GetRadius() * 1.65;
 
  //render targeting circle     
  if(m_gameOver == false){
    for(int i = 0; i < m_maxEnemyTanks + 1; i++){
      
      if(i == 0){
        source.left = 1; source.top = 1; source.right = 49; source.bottom = 47;
      }
      else{
        source.left = 49; source.top = 1; source.right = 98; source.bottom = 47;
      }
      dest.left = m_pObject[i].GetTargetX();// - 24;
      dest.top = m_pObject[i].GetTargetY();// - 23;
      dest.right = dest.left + 48;
      dest.bottom = dest.top + 46;
      dx = dest.left - m_pObject[0].GetTankX() + 8;
      dy = dest.top - m_pObject[0].GetTankY();
      dest.left = g_con.GetScreenCenterX() + dx;
      dest.right = dest.left + 32;
      dest.top = g_con.GetScreenCenterY() + dy;
      dest.bottom = dest.top + 32;
      if(m_pObject[i].IsAlive() == true)
        g_con.RenderTile(source, dest, 1.0f, 0);
    }
  }

  if(m_bKeyTPressed == true){
    for(int i = 0; i < m_maxEnemyTanks + 1; i++){
      dx = g_con.GetScreenCenterX() + (m_pObject[i].GetTankX() - m_pObject[0].GetTankX());
      dy = g_con.GetScreenCenterY() + (m_pObject[i].GetTankY() - m_pObject[0].GetTankY());
      g_con.Draw2DTextValue("ID",(float)i, F_V12, dx + m_pObject[i].GetRadius()*3, dy - m_pObject[i].GetRadius(), 255,0,0,255); 
      g_con.Draw2DTextValue("X",(float)m_pObject[i].GetTankX(), F_V12, dx + m_pObject[i].GetRadius()*3, dy - m_pObject[i].GetRadius() + 14, 255,0,0,255); 
      g_con.Draw2DTextValue("Y",(float)m_pObject[i].GetTankY(), F_V12, dx + m_pObject[i].GetRadius()*3, dy - m_pObject[i].GetRadius() + 28, 255,0,0,255); 
      g_con.Draw2DTextValue("DIR",m_pObject[i].GetTankDir(), F_V12, dx + m_pObject[i].GetRadius()*3, dy - m_pObject[i].GetRadius() + 42, 255,0,0,255); 
      g_con.Draw2DTextValue("Step",(float)m_pObject[i].GetTankStateStep(), F_V12, dx + m_pObject[i].GetRadius()*3, dy - m_pObject[i].GetRadius() + 56, 255,0,0,255); 
      g_con.Draw2DTextValue("ACC",m_pObject[i].m_tankAcc, F_V12, dx + m_pObject[i].GetRadius()*3, dy - m_pObject[i].GetRadius() + 98, 255,0,0,255);   
      g_con.Draw2DTextValue("VEL",m_pObject[i].m_tankVel, F_V12, dx + m_pObject[i].GetRadius()*3, dy - m_pObject[i].GetRadius() + 112, 255,0,0,255);   
    }
  }
  
  if(m_bPaused == true){
    g_con.Draw2DText("Paused",F_V20B,g_con.GetScreenCenterX() - 12, g_con.GetScreenCenterY() - 40,0, 130,0,255);
  }

  //used for effects at winning
  if(m_winTimer.getTimer(0.1)== true && m_bWinner == true){
    m_ironCross += 1;
    if(m_ironCross > 50)
      m_ironCross = 50;
  }

  if(m_gameOver == true){
    g_con.Draw2DText("Game Over! Press ESC to Continue.",F_V20,g_con.GetScreenCenterX() - 120, g_con.GetScreenCenterY() - 40, 0, 130,0,255);
  }
  else if(m_bWinner == true){
    if(g_Audio.IsPlaying(C_AUDIO_MUSIC_PLAY) == true)
      g_Audio.StopSoundClip(C_AUDIO_MUSIC_PLAY);
    if(g_Audio.IsPlaying(C_AUDIO_TANK_DRIVE) == true)
      g_Audio.StopSoundClip(C_AUDIO_TANK_DRIVE);
    if(g_Audio.IsPlaying(C_AUDIO_PANZER) == false)
      g_Audio.PlaySoundClip(C_AUDIO_PANZER,0);

    g_con.Draw2DText("Congratulations! You Win! Press ESC to Continue.",F_V20,g_con.GetScreenCenterX() - 240, g_con.GetScreenCenterY() - (6 * m_ironCross), 0, 130,0,255);

    source = g_con.LoadRect(399, 194,496, 289);
    dest = g_con.LoadRect(g_con.GetScreenCenterX() - 4 * m_ironCross, g_con.GetScreenCenterY() - 4 * m_ironCross,//+ 50,
                          g_con.GetScreenCenterX() + 8 * m_ironCross, g_con.GetScreenCenterY() + 8 * m_ironCross);
    g_con.RenderTile(source, dest, 4.0f * (float)(m_ironCross)/50.0f, 0.0f);

  }  
  else if(m_levelComplete == true){
    g_con.Draw2DTextValue("Level complete!  Loading next level......",5 - m_EndLevel, F_V20,g_con.GetScreenCenterX() - 150, g_con.GetScreenCenterY() - 40, 0, 130,0,255);
  }

  if(m_bKeyTPressed == true){
    g_con.Draw2DTextValue("Vel",m_pObject[0].m_tankVel, F_V20,50, 620, 255, 0, 255,255);
    g_con.Draw2DTextValue("Dir",m_pObject[0].m_tankDir * 180/PI, F_V20,50, 640, 255, 0, 255,255);
    g_con.Draw2DTextValue("Turret Az",m_pObject[0].m_turretAz * 180/PI, F_V20,50, 660, 255, 0, 255,255);
    g_con.Draw2DTextValue("Turret Elev",m_pObject[0].m_turretElev, F_V20,50, 680, 255, 0, 255,255);
  }

 

  //render control panel
  source = g_con.LoadRect(259, 5, 408, 294);
  dest = g_con.LoadRect(25, 25, 149, 289);
  g_con.RenderTileMore(source, dest, 1.0f, 0);
  g_con.Draw2DValue(m_kills, F_V16B, 90, 56, 255,0,0,255); 
  g_con.Draw2DValue(m_pObject[0].GetAmmoCount(), F_V16B, 90, 140, 255,255,0,255); 
  g_con.Draw2DValue(m_level, F_V16B, 90, 223, 0,255,255,255); 

  //render map
  float mapX = 50;
  float mapY = 345;
  float mx = 0;
  float my = 0;
  for(int i = 0; i < m_maxEnemyTanks + 1; i++){
    if(i == 0){ //player
      source = g_con.LoadRect(133, 432, 140, 438);//gray
    }
    else{
      source = g_con.LoadRect(133, 425, 140, 431);//red
    }

    mx = mapX + ((m_pObject[i].GetTankX()/2048) * 100.0f);
    my = mapY + ((m_pObject[i].GetTankY()/2048) * 100.0f);
    dest = g_con.LoadRect(mx, my, mx+7,my+7);
    if(m_pObject[i].IsAlive() == true)//only render map item if alive
      g_con.RenderTileMore(source, dest, 1.0f, 0);
  }

  source = g_con.LoadRect(285, 298, 431, 485);
  dest = g_con.LoadRect(25, 325, 200, 289);
  g_con.RenderTileMore(source, dest, 1.0f, 0);

  g_con.EndDrawing ();
  g_con.Present();
}

//loads entities to be displayed on the level
void CGameStatePlay::addLevel(int level){
  if(m_validFile == false)
    return;

  if(level < 1 || level > C_PLAY_MAX_LEVEL)
    return; //legal levels are 1, 2 and 3
  
  g_Log.Write("addLevel", level);

  m_level = level;
  int tank;
  for(int i = 0; i < m_file.size(); i++){
    if(m_file.getType(i) == 0){
      m_pObject[0].Initialize(m_file.getPosX(i), m_file.getPosY(i), 0, 0);
    }
    else if(m_file.getType(i) > 0 && m_file.getType(i) < C_PLAY_ENEMYCOUNT_LEVEL + 1){
      tank = m_file.getType(i);
      if(tank > 0 && tank < C_PLAY_ENEMYCOUNT_LEVEL + 1)
        m_pObject[tank].AddWaypoint((float)m_file.getPosX(i), (float)m_file.getPosY(i),0.01f,2,0,0,0);
    }
  }
}


void CGameStatePlay::processEvent(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam){
}
void CGameStatePlay::deactivate(){

}
void CGameStatePlay::resume(){
}
void CGameStatePlay::pause(){
}
void CGameStatePlay::save(){
}

void CGameStatePlay::addTransitionEvent(int event, cGameStateObject* p_Next){
  TRANSITION_EVENT te;
  te.event=event;
  te.p_gso = p_Next;

  m_TE.push_back(te);
}
