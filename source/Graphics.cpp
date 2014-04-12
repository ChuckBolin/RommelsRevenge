/**********************************************************
 * GRAPHICS.CPP     Copyright (c) 2003 Eamonn Doherty *
 * Direct3D code                                         
 
 Save sprites with ImageComposer as PNG, check Transparent
 color, set threshold to 127
 *********************************************************/
#include "..\includes\Graphics.h"
#include "..\includes\Log.h"
extern Log g_Log;

//Initialize Direct3D
int CDXGraphics::InitD3D (int resWidth, int resHeight, D3DFORMAT resFormat, HWND g_hWnd, BOOL bWindowedMode)
{
    HRESULT hr;
    d3d = Direct3DCreate9(D3D_SDK_VERSION); //Make Direct3D object

    //Make sure NULL pointer was not returned
    if (!d3d){
      g_Log.Write("DirectX9 creation failed!");
      return FALSE;
    }
    g_Log.Write("DirectX9 creation successful!");
    g_Log.Write("D3D_SDK_VERSION",D3D_SDK_VERSION);

    //Get device capabilities
    ZeroMemory (&d3dCaps, sizeof(d3dCaps));
    if (FAILED(d3d->GetDeviceCaps (D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &d3dCaps))){
      g_Log.Write("GetDeviceCaps failed!");
      return FALSE;
    }
    g_Log.Write("GetDeviceCaps succeeded!");

    //Setup present parameters
    ZeroMemory(&d3dPresent,sizeof(d3dPresent));
    d3dPresent.SwapEffect = D3DSWAPEFFECT_FLIP;
    d3dPresent.hDeviceWindow = g_hWnd;
    d3dPresent.BackBufferCount = 1;

    //Check if windowed
    if (bWindowedMode)
    {
        D3DDISPLAYMODE d3ddm;
        RECT rWindow;

        //Get display mode
        d3d->GetAdapterDisplayMode (D3DADAPTER_DEFAULT, &d3ddm);

        //Get window bounds
        GetClientRect (g_hWnd, &rWindow);

        //Setup screen dimensions
        resWidth = rWindow.right - rWindow.left;
        resHeight = rWindow.bottom - rWindow.top;
        g_Log.Write("Windowed Mode");
        g_Log.Write("WindowWidth",resWidth);
        g_Log.Write("WindowHeight",resHeight);

        //Setup backbuffer
        d3dPresent.Windowed = true;
        d3dPresent.BackBufferFormat = d3ddm.Format;
        d3dPresent.BackBufferWidth = rWindow.right - rWindow.left;
        d3dPresent.BackBufferHeight = rWindow.bottom - rWindow.top;
        g_Log.Write("BackBuffer setup");
    }
    else
    {        
        d3dPresent.Windowed = false;
        d3dPresent.BackBufferWidth = resWidth;
        d3dPresent.BackBufferHeight = resHeight;
        d3dPresent.BackBufferFormat = resFormat;
    }

    //Check if hardware vertex processing is available
    if (d3dCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
    {    
        //Create device with hardware vertex processing
        hr = d3d->CreateDevice(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL, g_hWnd,
            D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dPresent, &d3dDevice);        
    }
    else
    {
        //Create device with software vertex processing
        hr = d3d->CreateDevice(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL, g_hWnd,
            D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dPresent, &d3dDevice);
    }

    //Make sure device was created
    if (FAILED(hr)){
      g_Log.Write("Device creation failure!");
      return FALSE;
    }
    g_Log.Write("Device creation succeeded!");

    //Set vertex shader
    d3dDevice->SetVertexShader(NULL);
    d3dDevice->SetFVF (D3DFVF_TLVERTEX);

    //Create vertex buffer and set as stream source
    d3dDevice->CreateVertexBuffer(sizeof(TLVERTEX) * 4, NULL, D3DFVF_TLVERTEX, D3DPOOL_MANAGED,
                                  &vertexBuffer, NULL);
    d3dDevice->SetStreamSource (0, vertexBuffer, 0, sizeof(TLVERTEX));

    //Setup rendering states
    d3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
    d3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    d3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    d3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
    d3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

    //load 12 different fonts for Arial and Verdana, 12-16, normal and bold
    D3DXCreateFont(d3dDevice, 12, 0, FW_NORMAL, 0, FALSE, DEFAULT_CHARSET, 
      OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 
      DEFAULT_PITCH | FF_DONTCARE, TEXT("Arial"), &m_Font[F_A12] );
    D3DXCreateFont(d3dDevice, 12, 0, FW_BOLD, 0, FALSE, DEFAULT_CHARSET, 
      OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 
      DEFAULT_PITCH | FF_DONTCARE, TEXT("Arial"), &m_Font[F_A12B] );
    D3DXCreateFont(d3dDevice, 14, 0, FW_NORMAL, 0, FALSE, DEFAULT_CHARSET, 
      OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 
      DEFAULT_PITCH | FF_DONTCARE, TEXT("Arial"), &m_Font[F_A14] );
    D3DXCreateFont(d3dDevice, 14, 0, FW_BOLD, 0, FALSE, DEFAULT_CHARSET, 
      OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 
      DEFAULT_PITCH | FF_DONTCARE, TEXT("Arial"), &m_Font[F_A14B] );
    D3DXCreateFont(d3dDevice, 16, 0, FW_NORMAL, 0, FALSE, DEFAULT_CHARSET, 
      OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 
      DEFAULT_PITCH | FF_DONTCARE, TEXT("Arial"), &m_Font[F_A16] );
    D3DXCreateFont(d3dDevice, 16, 0, FW_BOLD, 0, FALSE, DEFAULT_CHARSET, 
      OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 
      DEFAULT_PITCH | FF_DONTCARE, TEXT("Arial"), &m_Font[F_A16B] );
    D3DXCreateFont(d3dDevice, 12, 0, FW_NORMAL, 0, FALSE, DEFAULT_CHARSET, 
      OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 
      DEFAULT_PITCH | FF_DONTCARE, TEXT("Verdana"), &m_Font[F_V12] );
    D3DXCreateFont(d3dDevice, 12, 0, FW_BOLD, 0, FALSE, DEFAULT_CHARSET, 
      OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 
      DEFAULT_PITCH | FF_DONTCARE, TEXT("Verdana"), &m_Font[F_V12B] );
    D3DXCreateFont(d3dDevice, 14, 0, FW_NORMAL, 0, FALSE, DEFAULT_CHARSET, 
      OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 
      DEFAULT_PITCH | FF_DONTCARE, TEXT("Verdana"), &m_Font[F_V14] );
    D3DXCreateFont(d3dDevice, 14, 0, FW_BOLD, 0, FALSE, DEFAULT_CHARSET, 
      OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 
      DEFAULT_PITCH | FF_DONTCARE, TEXT("Verdana"), &m_Font[F_V14B] );
    D3DXCreateFont(d3dDevice, 16, 0, FW_NORMAL, 0, FALSE, DEFAULT_CHARSET, 
      OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 
      DEFAULT_PITCH | FF_DONTCARE, TEXT("Verdana"), &m_Font[F_V16] );    
    D3DXCreateFont(d3dDevice, 16, 0, FW_BOLD, 0, FALSE, DEFAULT_CHARSET, 
      OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 
      DEFAULT_PITCH | FF_DONTCARE, TEXT("Verdana"), &m_Font[F_V16B] );

    D3DXCreateFont(d3dDevice, 20, 0, FW_NORMAL, 0, FALSE, DEFAULT_CHARSET, 
      OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 
      DEFAULT_PITCH | FF_DONTCARE, TEXT("Verdana"), &m_Font[F_V20] );
    D3DXCreateFont(d3dDevice, 20, 0, FW_BOLD, 0, FALSE, DEFAULT_CHARSET, 
      OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 
      DEFAULT_PITCH | FF_DONTCARE, TEXT("Verdana"), &m_Font[F_V20B] );


   // m_screenWidth = ::GetSystemMetrics(SM_CXFULLSCREEN);
   // m_screenHeight = ::GetSystemMetrics(SM_CYFULLSCREEN);
    //Successfully initalized Direct3D
    return TRUE;
}

void CDXGraphics::Draw2DText(std::string sText, int fontType, int posX, int posY, int red, int green, int blue, int alpha){
  D3DCOLOR fontColor = D3DCOLOR_ARGB(alpha, red, green, blue);
  int width = 0;
  int fontHeight = 0;
  //if(fontType < 0 || fontType > C_NUMBER_FONTS )
  //  fontType = 2;

  if(fontType == 0 || fontType == 1 || fontType == 6 || fontType == 7)
    fontHeight = 12;
  else if(fontType == 2 || fontType == 3 || fontType == 8 || fontType == 9)
    fontHeight = 14;
  else if(fontType == 4 || fontType == 5 || fontType == 10 || fontType == 11)
    fontHeight = 16;
  else if(fontType == 12 || fontType == 13)
    fontHeight = 20;

  width = sText.size() * fontHeight;
  RECT rct;
  rct.left = posX;
  rct.top = posY;
  rct.right = rct.left + width;
  rct.bottom = rct.top + fontHeight;
  m_Font[fontType]->DrawText(NULL, sText.c_str(), -1, &rct, 0, fontColor);  
}

void CDXGraphics::Draw2DTextValue(std::string sText, float value,int fontType, int posX, int posY, int red, int green, int blue, int alpha){
  std::ostringstream oss;
  std::string text;
  oss << sText << ":" << value;
  text = oss.str();
  D3DCOLOR fontColor = D3DCOLOR_ARGB(alpha, red, green, blue);
  int width = 0;
  int fontHeight = 0;
  //if(fontType < 0 || fontType > 11)
  //  fontType = 0;

  if(fontType == 0 || fontType == 1 || fontType == 6 || fontType == 7)
    fontHeight = 12;
  else if(fontType == 2 || fontType == 3 || fontType == 8 || fontType == 9)
    fontHeight = 14;
  else if(fontType == 4 || fontType == 5 || fontType == 10 || fontType == 11)
    fontHeight = 16;
  else if(fontType == 12 || fontType == 13)
    fontHeight = 20;

  width = sText.size() * fontHeight * 20;
  RECT rct;
  rct.left = posX;
  rct.top = posY;
  rct.right = rct.left + width;
  rct.bottom = rct.top + fontHeight;
  m_Font[fontType]->DrawText(NULL, text.c_str(), -1, &rct, 0, fontColor);  
}


void CDXGraphics::Draw2DValue(float value,int fontType, int posX, int posY, int red, int green, int blue, int alpha){
  std::ostringstream oss;
  std::string text;
  oss << value;
  text = oss.str();
  D3DCOLOR fontColor = D3DCOLOR_ARGB(alpha, red, green, blue);
  int width = 0;
  int fontHeight = 0;
  if(fontType < 0 || fontType > 11)
    fontType = 0;

  if(fontType == 0 || fontType == 1 || fontType == 6 || fontType == 7)
    fontHeight = 12;
  else if(fontType == 2 || fontType == 3 || fontType == 8 || fontType == 9)
    fontHeight = 14;
  else if(fontType == 4 || fontType == 5 || fontType == 10 || fontType == 11)
    fontHeight = 16;

  width = text.size() * fontHeight;
  RECT rct;
  rct.left = posX;
  rct.top = posY;
  rct.right = rct.left + width;
  rct.bottom = rct.top + fontHeight;
  m_Font[fontType]->DrawText(NULL, text.c_str(), -1, &rct, 0, fontColor);  
}

void CDXGraphics::Draw2DValue(int value,int fontType, int posX, int posY, int red, int green, int blue, int alpha){
  std::ostringstream oss;
  std::string text;
  oss << value;
  text = oss.str();
  D3DCOLOR fontColor = D3DCOLOR_ARGB(alpha, red, green, blue);
  int width = 0;
  int fontHeight = 0;
  if(fontType < 0 || fontType > 11)
    fontType = 0;

  if(fontType == 0 || fontType == 1 || fontType == 6 || fontType == 7)
    fontHeight = 12;
  else if(fontType == 2 || fontType == 3 || fontType == 8 || fontType == 9)
    fontHeight = 14;
  else if(fontType == 4 || fontType == 5 || fontType == 10 || fontType == 11)
    fontHeight = 16;

  width = text.size() * fontHeight;
  RECT rct;
  rct.left = posX;
  rct.top = posY;
  rct.right = rct.left + width;
  rct.bottom = rct.top + fontHeight;
  m_Font[fontType]->DrawText(NULL, text.c_str(), -1, &rct, 0, fontColor);  
}


//Close Direct3D
int CDXGraphics::CloseD3D()
{
    for(int i = 0; i < 12; i++){
      if(m_Font[i])
        m_Font[i]->Release();
    }

    //Release vertex buffer
    if (vertexBuffer)
        vertexBuffer->Release ();

    //Release device
    if (d3dDevice)
        d3dDevice->Release();

    //Release d3d object
    if (d3d)
        d3d->Release();

    if(m_sprite)
      m_sprite->Release();

    //Successfully closed Direct3D
    return TRUE;
}

//draws circle
void CDXGraphics::DrawCircle(float x, float y, float radius){

  const int NUMPOINTS = 24;
  const float PI = 3.14159;
  struct line_vertex{
      float x, y, z, rhw;
      DWORD color;       
  };
  line_vertex circle[NUMPOINTS + 1];
  const DWORD line_fvf=D3DFVF_XYZRHW|D3DFVF_DIFFUSE;  
  float theta;
  float wedgeAngle;
  wedgeAngle = (float)((2 * PI)/NUMPOINTS);
  for (int i = 0; i <= NUMPOINTS; i++){
    theta = i * wedgeAngle;
    circle[i].x = (float)(x + radius * cos(theta));
    circle[i].y = (float)(y - radius * sin(theta));
    circle[i].z = 0;
    circle[i].rhw = 1.0f;
    circle[i].color = D3DCOLOR_XRGB(0,255,0);
  }

  d3dDevice->SetFVF(line_fvf);
  d3dDevice->SetTexture(0, NULL);
  d3dDevice->DrawPrimitiveUP(D3DPT_LINESTRIP, NUMPOINTS, &circle[0],sizeof(circle[0]));
  
}

void CDXGraphics::RenderSprite(float angle){
  RECT rect;
  rect.left = 3;
  rect.top = 2;
  rect.right = 27;
  rect.bottom = 27;
  RECT turr;
  turr.left = 128;
  turr.top = 32;
  turr.right = 142;
  turr.bottom = 59;
  float scale = 1.0f;
  m_sprite->Begin(D3DXSPRITE_ALPHABLEND);
  d3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);

  ::D3DXVECTOR2 pos(120, 50);
  ::D3DXVECTOR2 scaling(scale, scale);
  ::D3DXVECTOR2 rotcenter(13.0f, 13.0f);
  ::D3DXMATRIX mat;
  ::D3DXMatrixTransformation2D(&mat, NULL, 0.0, &scaling, &rotcenter, 0, &pos);//angle
  m_sprite->SetTransform(&mat);  
  m_sprite->Draw(m_texture, &rect, NULL, NULL, D3DCOLOR_ARGB(255, 255, 255, 255));

  pos.x = 120 + (4.5 * scale);
  pos.y = 50 - (7 * scale);
  scaling.x = scale;
  scaling.y = scale;
  rotcenter.x = 7.5f * scale;
  rotcenter.y = 20.0f * scale;
  //::D3DXMATRIX mat;
  ::D3DXMatrixTransformation2D(&mat, NULL, 0.0, &scaling, &rotcenter, angle, &pos);//angle
  m_sprite->SetTransform(&mat);  

  m_sprite->Draw(m_texture, &turr, NULL, NULL, D3DCOLOR_ARGB(255, 255, 255, 255));

  D3DXMatrixIdentity(&mat);
  m_sprite->SetTransform(&mat);
  m_sprite->End();
}

void CDXGraphics::RenderTank(CTank tank, CTank player){
  RECT rect;
  RECT turr;
  RECT shadowBody;
  RECT shadowTurret;
  int shadowOffsetX = 3;
  int shadowOffsetY = 4;
  float scale = 1.0f;

  shadowBody = LoadRect(361, 59, 401,111);
  shadowTurret = LoadRect(367, 0, 392, 49);

  if(tank.m_bAlive == false){//destroyed tank
    rect = LoadRect(404, 59, 444, 111);
    turr = LoadRect(410, 0, 435, 49);
  }
  else if(tank.GetType() == C_TYPE_PLAYER){
    rect = LoadRect(317, 59, 357, 111);
    turr = LoadRect(323, 0, 348, 49);
  }
  else{//tank may be British or American
    if(tank.m_countryFlag == C_COUNTRY_US){
      rect = LoadRect(274, 59, 314, 111);
      turr = LoadRect(280, 0, 305, 49);
    }
    else{
      rect = LoadRect(230, 59, 271, 110);
      turr = LoadRect(237, 5, 261, 50);
    }
  }

  m_sprite->Begin(D3DXSPRITE_ALPHABLEND);
  d3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
 
  float posX;
  float posY;
  ::D3DXMATRIX mat;
  ::D3DXVECTOR2 pos;//(posX, posY);//  120, 50);
  ::D3DXVECTOR2 scaling;//(scale, scale);
  ::D3DXVECTOR2 rotcenter;

  //tank body - German
  if(tank.GetType() == C_TYPE_PLAYER){
    posX = m_screenWidth/2 + (1.0 * scale);// + (player.GetTankX() - tank.GetTankX());
    posY = m_screenHeight/2  - (1.0 * scale);// + (player.GetTankY() - tank.GetTankY());
    pos.x = posX + shadowOffsetX;
    pos.y = posY + shadowOffsetY;
    scaling.x = scale;
    scaling.y = scale;
    rotcenter.x = 19.0f * scale;
    rotcenter.y = 22.0f * scale;
    ::D3DXMatrixTransformation2D(&mat, NULL, 0.0, &scaling, &rotcenter, tank.GetTankDir(), &pos);//angle
    m_sprite->SetTransform(&mat);  
    m_sprite->Draw(m_texture, &shadowBody, NULL, NULL, D3DCOLOR_ARGB(205, 255, 255, 255)); //shadow //70

    pos.x = posX;
    pos.y = posY;
    scaling.x = scale;
    scaling.y = scale;
    rotcenter.x = 19.0f * scale;
    rotcenter.y = 22.0f * scale;
    ::D3DXMatrixTransformation2D(&mat, NULL, 0.0, &scaling, &rotcenter, tank.GetTankDir(), &pos);//angle
    m_sprite->SetTransform(&mat);  
    m_sprite->Draw(m_texture, &rect, NULL, NULL, D3DCOLOR_ARGB(255, 255, 255, 255));

  }
  else{ //enemy tank
    posX = m_screenWidth/2 + (1.0 * scale) + (tank.GetTankX() - player.GetTankX());
    posY = m_screenHeight/2  - (1.0 * scale) + (tank.GetTankY() - player.GetTankY());
    pos.x = posX + shadowOffsetX;
    pos.y = posY + shadowOffsetY;
    scaling.x = scale;
    scaling.y = scale;
    rotcenter.x = 19.0f * scale;//19.0f * scale;
    rotcenter.y = 22.0f * scale;// 24.0f * scale;
    ::D3DXMatrixTransformation2D(&mat, NULL, 0.0, &scaling, &rotcenter, tank.GetTankDir(), &pos);//angle
    m_sprite->SetTransform(&mat);  
    m_sprite->Draw(m_texture, &shadowBody, NULL, NULL, D3DCOLOR_ARGB(205, 255, 255, 255));//enemy shadow

    pos.x = posX;
    pos.y = posY;
    scaling.x = scale;
    scaling.y = scale;
    rotcenter.x = 19.0f * scale;//19.0f * scale;
    rotcenter.y = 22.0f * scale;// 24.0f * scale;
    ::D3DXMatrixTransformation2D(&mat, NULL, 0.0, &scaling, &rotcenter, tank.GetTankDir(), &pos);//angle
    m_sprite->SetTransform(&mat);  
    m_sprite->Draw(m_texture, &rect, NULL, NULL, D3DCOLOR_ARGB(255, 255, 255, 255));

  }

  //German turret
  if(tank.GetType() == C_TYPE_PLAYER){
    pos.x =  m_screenWidth/2 + (8 * scale);//tank.GetTankX()
    pos.y =  m_screenHeight/2 - (14 * scale);//tank.GetTankY()
    scaling.x = scale;
    scaling.y = scale;
    rotcenter.x = 12.0f * scale + shadowOffsetX;
    rotcenter.y = 35.0f * scale + shadowOffsetY;
    ::D3DXMatrixTransformation2D(&mat, NULL, 0.0, &scaling, &rotcenter,tank.GetTurretAz() , &pos);//angle
    m_sprite->SetTransform(&mat);  
    //m_sprite->Draw(m_texture, &shadowTurret, NULL, NULL, D3DCOLOR_ARGB(70, 255, 255, 255));

    scaling.x = scale;
    scaling.y = scale;
    rotcenter.x = 12.0f * scale;
    rotcenter.y = 35.0f * scale;
    ::D3DXMatrixTransformation2D(&mat, NULL, 0.0, &scaling, &rotcenter,tank.GetTurretAz() , &pos);//angle
    m_sprite->SetTransform(&mat);  
    m_sprite->Draw(m_texture, &turr, NULL, NULL, D3DCOLOR_ARGB(255, 255, 255, 255));

  }
  else{//enemy turret
    pos.x =  m_screenWidth/2 + (8 * scale) + (tank.GetTankX() - player.GetTankX());
    pos.y =  m_screenHeight/2 - (14 * scale) + (tank.GetTankY() - player.GetTankY());
    scaling.x = scale;
    scaling.y = scale;
    rotcenter.x = 12.0f * scale + shadowOffsetX;
    rotcenter.y = 35.0f * scale + shadowOffsetY;
    ::D3DXMatrixTransformation2D(&mat, NULL, 0.0, &scaling, &rotcenter,tank.GetTurretAz() , &pos);//angle
    m_sprite->SetTransform(&mat);  
    //m_sprite->Draw(m_texture, &shadowTurret, NULL, NULL, D3DCOLOR_ARGB(70, 255, 255, 255));

    scaling.x = scale;
    scaling.y = scale;
    rotcenter.x = 12.0f * scale;
    rotcenter.y = 35.0f * scale;
    ::D3DXMatrixTransformation2D(&mat, NULL, 0.0, &scaling, &rotcenter,tank.GetTurretAz() , &pos);//angle
    m_sprite->SetTransform(&mat);  
    m_sprite->Draw(m_texture, &turr, NULL, NULL, D3DCOLOR_ARGB(255, 255, 255, 255));

  }

  D3DXMatrixIdentity(&mat);
  m_sprite->SetTransform(&mat);
  m_sprite->End();
}

void CDXGraphics::RenderTile(RECT source, RECT dest, float scale, float angle){
  //RECT src = LoadRect(1, 182, 33, 214);
  m_sprite->Begin(D3DXSPRITE_ALPHABLEND);
  d3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
  ::D3DXVECTOR2 pos(dest.left, dest.top);
  ::D3DXVECTOR2 scaling(scale, scale);
  ::D3DXVECTOR2 rotcenter(dest.right - dest.left, dest.top - dest.bottom);
  ::D3DXMATRIX mat;
  ::D3DXMatrixTransformation2D(&mat, NULL, 0.0, &scaling, &rotcenter, angle, &pos);
  m_sprite->SetTransform(&mat);  
  m_sprite->Draw(m_texture, &source, NULL, NULL, D3DCOLOR_ARGB(255, 255, 255, 255));

  D3DXMatrixIdentity(&mat);
  m_sprite->SetTransform(&mat);
  m_sprite->End();
}

void CDXGraphics::RenderTileMore(RECT source, RECT dest, float scale, float angle){
  m_spriteMore->Begin(D3DXSPRITE_ALPHABLEND);
  d3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
  ::D3DXVECTOR2 pos(dest.left, dest.top);
  ::D3DXVECTOR2 scaling(scale, scale);
  ::D3DXVECTOR2 rotcenter(dest.right - dest.left, dest.top - dest.bottom);
  ::D3DXMATRIX mat;
  ::D3DXMatrixTransformation2D(&mat, NULL, 0.0, &scaling, &rotcenter, angle, &pos);
  m_spriteMore->SetTransform(&mat);  
  m_spriteMore->Draw(m_pTextureMore, &source, NULL, NULL, D3DCOLOR_ARGB(255, 255, 255, 255));

  D3DXMatrixIdentity(&mat);
  m_spriteMore->SetTransform(&mat);
  m_spriteMore->End();
}


void CDXGraphics::LoadSprite(std::string sFilename){
  HRESULT hr;
  hr = ::D3DXCreateTextureFromFile(d3dDevice, sFilename.c_str(), &m_texture);
  hr = ::D3DXCreateSprite(d3dDevice, &m_sprite);
}

void CDXGraphics::LoadSpriteMore(std::string sFilename){
  HRESULT hr;
  hr = ::D3DXCreateTextureFromFile(d3dDevice, sFilename.c_str(), &m_pTextureMore);
  hr = ::D3DXCreateSprite(d3dDevice, &m_spriteMore);
}

//Clear backbuffer
int CDXGraphics::ClearBuffer (int red, int green, int blue, int alpha){ 
  return d3dDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(alpha, red, green, blue), 0.0f, 0);
}

//Present
int CDXGraphics::Present () { return d3dDevice->Present (NULL, NULL, NULL, NULL); }
   
//Begin drawing
int CDXGraphics::BeginDrawing () { return d3dDevice->BeginScene (); }

//End drawing
int CDXGraphics::EndDrawing () { return d3dDevice->EndScene (); }


//Load a texture
//Supported formats: BMP, PPM, DDS, JPG, PNG, TGA, DIB
IDirect3DTexture9 * CDXGraphics::LoadTexture(char *fileName)
{
    IDirect3DTexture9 *d3dTexture;
    D3DXIMAGE_INFO SrcInfo;			//Optional

    //Use a magenta colorkey
    D3DCOLOR colorkey = 0xFFFF00FF;

    // Load image from file
    if (FAILED(D3DXCreateTextureFromFileEx (d3dDevice, fileName, 0, 0, 1, 0, 
          D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_DEFAULT, 
          colorkey, &SrcInfo, NULL, &d3dTexture)))
    {
        return NULL;
    }

	//Return the newly made texture
    return d3dTexture;
}

IDirect3DTexture9 * CDXGraphics::LoadTextureSplash(char *fileName)
{
    IDirect3DTexture9 *d3dTexture;
    D3DXIMAGE_INFO SrcInfo;			//Optional

    //Use a magenta colorkey
    D3DCOLOR colorkey = 0xFFFF00FF;

    // Load image from file
    if (FAILED(D3DXCreateTextureFromFileEx (d3dDevice, fileName, 0, 0, 1, 0, 
          D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_DEFAULT, 
          colorkey, &SrcInfo, NULL, &d3dTexture)))
    {
        return NULL;
    }

	//Return the newly made texture
    return d3dTexture;
}


void CDXGraphics::DrawSprite (IDirect3DTexture9 *texture, RECT *rDest,RECT *rSource, 
                 float rotate, D3DCOLOR vertexcolor)
{
	TLVERTEX* vertices;
  TLVERTEX bufferVertices[4];
  int index = 0;

	//Lock the vertex buffer
	vertexBuffer->Lock(0, 0, (void **)&vertices, NULL);

	//Setup vertices
	//A -0.5f modifier is applied to vertex coordinates to match texture and screen coords
	//Some drivers may compensate for this automatically, but on others texture alignment errors are introduced
	//More information on this can be found in the Direct3D 9 documentation
	rSource->left = 0.75f;
  rSource->top = 0.0f;
  rSource->right = 1.0f;
  rSource->bottom = 1.0f;
  
  vertices[0].color = vertexcolor;
	vertices[0].x = (float) rDest->left - 0.5f;
	vertices[0].y = (float) rDest->top - 0.5f;
	vertices[0].z = 0.0f;
	vertices[0].rhw = 1.0f;
	vertices[0].u = rSource->left;
	vertices[0].v = rSource->top;

	vertices[1].color = vertexcolor;
	vertices[1].x = (float) rDest->right - 0.5f;
	vertices[1].y = (float) rDest->top - 0.5f;
	vertices[1].z = 0.0f;
	vertices[1].rhw = 1.0f;
	vertices[1].u = rSource->right;
	vertices[1].v = rSource->top;

	vertices[2].color = vertexcolor;
	vertices[2].x = (float) rDest->right - 0.5f;
	vertices[2].y = (float) rDest->bottom - 0.5f;
	vertices[2].z = 0.0f;
	vertices[2].rhw = 1.0f;
	vertices[2].u = rSource->right;
	vertices[2].v = rSource->bottom;

	vertices[3].color = vertexcolor;
	vertices[3].x = (float) rDest->left - 0.5f;
	vertices[3].y = (float) rDest->bottom - 0.5f;
	vertices[3].z = 0.0f;
	vertices[3].rhw = 1.0f;
	vertices[3].u = rSource->left;
	vertices[3].v = rSource->bottom;

	//Unlock the vertex buffer
	vertexBuffer->Unlock();

	//Set texture
	d3dDevice->SetTexture (0, texture);

	//Draw image
	d3dDevice->DrawPrimitive (D3DPT_TRIANGLEFAN, 0, 2);


}


//Draw a textured quad on the back-buffer
void CDXGraphics::BlitD3D (IDirect3DTexture9 *texture, RECT *rDest, D3DCOLOR vertexcolor, float rotate)
{
	TLVERTEX* vertices;
  TLVERTEX bufferVertices[4];
  int index = 0;

	//Lock the vertex buffer
	vertexBuffer->Lock(0, 0, (void **)&vertices, NULL);

	//Setup vertices
	//A -0.5f modifier is applied to vertex coordinates to match texture and screen coords
	//Some drivers may compensate for this automatically, but on others texture alignment errors are introduced
	//More information on this can be found in the Direct3D 9 documentation
	vertices[0].color = vertexcolor;
	vertices[0].x = (float) rDest->left - 0.5f;
	vertices[0].y = (float) rDest->top - 0.5f;
	vertices[0].z = 0.0f;
	vertices[0].rhw = 1.0f;
	vertices[0].u = 0.0f;
	vertices[0].v = 0.0f;

	vertices[1].color = vertexcolor;
	vertices[1].x = (float) rDest->right - 0.5f;
	vertices[1].y = (float) rDest->top - 0.5f;
	vertices[1].z = 0.0f;
	vertices[1].rhw = 1.0f;
	vertices[1].u = 1.0f;
	vertices[1].v = 0.0f;

	vertices[2].color = vertexcolor;
	vertices[2].x = (float) rDest->right - 0.5f;
	vertices[2].y = (float) rDest->bottom - 0.5f;
	vertices[2].z = 0.0f;
	vertices[2].rhw = 1.0f;
	vertices[2].u = 1.0f;
	vertices[2].v = 1.0f;

	vertices[3].color = vertexcolor;
	vertices[3].x = (float) rDest->left - 0.5f;
	vertices[3].y = (float) rDest->bottom - 0.5f;
	vertices[3].z = 0.0f;
	vertices[3].rhw = 1.0f;
	vertices[3].u = 0.0f;
	vertices[3].v = 1.0f;

  //Handle rotation
  if (rotate != 0)
  {
      RECT rOrigin;
      float centerX, centerY;

      //Find center of destination rectangle
      centerX = (float)(rDest->left + rDest->right) / 2;
      centerY = (float)(rDest->top + rDest->bottom) / 2;

      //Translate destination rect to be centered on the origin
      rOrigin.top = rDest->top - (int)(centerY);
      rOrigin.bottom = rDest->bottom - (int)(centerY);
      rOrigin.left = rDest->left - (int)(centerX);
      rOrigin.right = rDest->right - (int)(centerX);

      //Rotate vertices about the origin
      bufferVertices[index].x = rOrigin.left * cosf(rotate) -
                                rOrigin.top * sinf(rotate);
      bufferVertices[index].y = rOrigin.left * sinf(rotate) +
                                rOrigin.top * cosf(rotate);

      bufferVertices[index + 1].x = rOrigin.right * cosf(rotate) -
                                    rOrigin.top * sinf(rotate);
      bufferVertices[index + 1].y = rOrigin.right * sinf(rotate) +
                                    rOrigin.top * cosf(rotate);

      bufferVertices[index + 2].x = rOrigin.right * cosf(rotate) -
                                    rOrigin.bottom * sinf(rotate);
      bufferVertices[index + 2].y = rOrigin.right * sinf(rotate) +
                                    rOrigin.bottom * cosf(rotate);

      bufferVertices[index + 3].x = rOrigin.left * cosf(rotate) -
                                    rOrigin.bottom * sinf(rotate);
      bufferVertices[index + 3].y = rOrigin.left * sinf(rotate) +
                                    rOrigin.bottom * cosf(rotate);

      //Translate vertices to proper position
      bufferVertices[index].x += centerX;
      bufferVertices[index].y += centerY;
      bufferVertices[index + 1].x += centerX;
      bufferVertices[index + 1].y += centerY;
      bufferVertices[index + 2].x += centerX;
      bufferVertices[index + 2].y += centerY;
      bufferVertices[index + 3].x += centerX;
      bufferVertices[index + 3].y += centerY;
  }

	//Unlock the vertex buffer
	vertexBuffer->Unlock();

	//Set texture
	d3dDevice->SetTexture (0, texture);

	//Draw image
	d3dDevice->DrawPrimitive (D3DPT_TRIANGLEFAN, 0, 2);
}


//Draw a textured quad on the back-buffer
void CDXGraphics::BlitExD3D (IDirect3DTexture9 *texture, RECT *rDest, D3DCOLOR *vertexcolors /* -> D3DCOLOR[4] */,
             float rotate)
{
	TLVERTEX* vertices;
  TLVERTEX bufferVertices[4];
  int index = 0;

	//Lock the vertex buffer
	vertexBuffer->Lock(0, 0, (void **)&vertices, NULL);

	//Setup vertices
	//A -0.5f modifier is applied to vertex coordinates to match texture and screen coords
	//Some drivers may compensate for this automatically, but on others texture alignment errors are introduced
	//More information on this can be found in the Direct3D 9 documentation
	vertices[0].color = vertexcolors[0];
	vertices[0].x = (float) rDest->left - 0.5f;
	vertices[0].y = (float) rDest->top - 0.5f;
	vertices[0].z = 0.0f;
	vertices[0].rhw = 1.0f;
	vertices[0].u = 0.0f;
	vertices[0].v = 0.0f;

	vertices[1].color = vertexcolors[1];
	vertices[1].x = (float) rDest->right - 0.5f;
	vertices[1].y = (float) rDest->top - 0.5f;
	vertices[1].z = 0.0f;
	vertices[1].rhw = 1.0f;
	vertices[1].u = 1.0f;
	vertices[1].v = 0.0f;

	vertices[2].color = vertexcolors[2];
	vertices[2].x = (float) rDest->right - 0.5f;
	vertices[2].y = (float) rDest->bottom - 0.5f;
	vertices[2].z = 0.0f;
	vertices[2].rhw = 1.0f;
	vertices[2].u = 1.0f;
	vertices[2].v = 1.0f;

	vertices[3].color = vertexcolors[3];
	vertices[3].x = (float) rDest->left - 0.5f;
	vertices[3].y = (float) rDest->bottom - 0.5f;
	vertices[3].z = 0.0f;
	vertices[3].rhw = 1.0f;
	vertices[3].u = 0.0f;
	vertices[3].v = 1.0f;

  //Handle rotation
  if (rotate != 0)
  {
      RECT rOrigin;
      float centerX, centerY;

      //Find center of destination rectangle
      centerX = (float)(rDest->left + rDest->right) / 2;
      centerY = (float)(rDest->top + rDest->bottom) / 2;

      //Translate destination rect to be centered on the origin
      rOrigin.top = rDest->top - (int)(centerY);
      rOrigin.bottom = rDest->bottom - (int)(centerY);
      rOrigin.left = rDest->left - (int)(centerX);
      rOrigin.right = rDest->right - (int)(centerX);

      //Rotate vertices about the origin
      bufferVertices[index].x = rOrigin.left * cosf(rotate) -
                                rOrigin.top * sinf(rotate);
      bufferVertices[index].y = rOrigin.left * sinf(rotate) +
                                rOrigin.top * cosf(rotate);

      bufferVertices[index + 1].x = rOrigin.right * cosf(rotate) -
                                    rOrigin.top * sinf(rotate);
      bufferVertices[index + 1].y = rOrigin.right * sinf(rotate) +
                                    rOrigin.top * cosf(rotate);

      bufferVertices[index + 2].x = rOrigin.right * cosf(rotate) -
                                    rOrigin.bottom * sinf(rotate);
      bufferVertices[index + 2].y = rOrigin.right * sinf(rotate) +
                                    rOrigin.bottom * cosf(rotate);

      bufferVertices[index + 3].x = rOrigin.left * cosf(rotate) -
                                    rOrigin.bottom * sinf(rotate);
      bufferVertices[index + 3].y = rOrigin.left * sinf(rotate) +
                                    rOrigin.bottom * cosf(rotate);

      //Translate vertices to proper position
      bufferVertices[index].x += centerX;
      bufferVertices[index].y += centerY;
      bufferVertices[index + 1].x += centerX;
      bufferVertices[index + 1].y += centerY;
      bufferVertices[index + 2].x += centerX;
      bufferVertices[index + 2].y += centerY;
      bufferVertices[index + 3].x += centerX;
      bufferVertices[index + 3].y += centerY;
  }

	//Unlock the vertex buffer
	vertexBuffer->Unlock();

	//Set texture
	d3dDevice->SetTexture (0, texture);

	//Draw image
	d3dDevice->DrawPrimitive (D3DPT_TRIANGLEFAN, 0, 2);
}

RECT CDXGraphics::LoadRect(float left, float top, float right, float bottom){
  RECT temp;
  temp.left = (long)left;
  temp.top = (long)top;
  temp.right = (long)right;
  temp.bottom = (long)bottom;
  return temp; 
}
IDirect3DTexture9 * CDXGraphics::GetTexturePointer(){
  return m_pTexture;
}
IDirect3DTexture9 * CDXGraphics::GetTexturePointerMore(){
  return m_pTextureMore;
}

IDirect3DTexture9 * CDXGraphics::GetTextureSplashPointer(){
  return m_pTextureSplash;
}


int CDXGraphics::GetScreenCenterX(){
  return m_screenCenterX;
}
int CDXGraphics::GetScreenCenterY(){
  return m_screenCenterY;
}
int CDXGraphics::GetScreenWidth(){
  return m_screenWidth;
}
int CDXGraphics::GetScreenHeight(){
  return m_screenHeight;
}

void CDXGraphics::SetScreenWidthHeight(int w, int h){
 m_screenWidth = w;
 m_screenHeight = h;
 m_screenCenterX = m_screenWidth/2;
 m_screenCenterY = m_screenHeight/2;
}