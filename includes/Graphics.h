/*************************************************************************************************
 * GRAPHICS.H                                                  Copyright (c) 2003 Eamonn Doherty *
 *                                                                                               *
 * Direct3D code                                                                                 *
 *************************************************************************************************/
//#pragma once
#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <string>
#include <sstream>
#include "CTank.h"

//Custom vertex format
const DWORD D3DFVF_TLVERTEX = D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1;

/* Vertices:

    v0               v1
    |---------------|
    |\              |
    |  \            |
    |    \          |
    |      \        |
    |        \      |
    |          \    |
    |            \  |
    |              \|
    |---------------|
    v3               v2

    z = 0
    rhw = 1
*/

//Custom vertex
struct TLVERTEX
{
    float x;
    float y;
    float z;
    float rhw;
    D3DCOLOR color;
    float u;
    float v;
};

//font constants
const int F_A12 = 0;  //Arial
const int F_A12B = 1;
const int F_A14 = 2;
const int F_A14B = 3;
const int F_A16 = 4;
const int F_A16B = 5;
const int F_V12 = 6;  //Verdana
const int F_V12B = 7;
const int F_V14 = 8;
const int F_V14B = 9;
const int F_V16 = 10;
const int F_V16B = 11;
const int F_V20 = 12;
const int F_V20B = 13;
const int C_NUMBER_FONTS = 15;

class CTank;


class CDXGraphics{
public:
  int InitD3D (int resWidth, int resHeight, D3DFORMAT resFormat, HWND g_hWnd, BOOL bWindowedMode = false);
  int CloseD3D ();
  int ClearBuffer (int red, int green, int blue, int alpha = 255);
  int Present();
  int BeginDrawing ();
  int EndDrawing ();
  IDirect3DTexture9 *LoadTexture(char *fileName);
  IDirect3DTexture9 *LoadTextureSplash(char *fileName);
  void BlitD3D (IDirect3DTexture9 *texture, RECT *rDest, D3DCOLOR vertexcolor = 0xFFFFFFFF,float fRotate = 0);
  void DrawSprite (IDirect3DTexture9 *texture, RECT *rDest, RECT *rSource, float fRotate = 0, D3DCOLOR vertexcolor = 0xFFFFFFFF);
  void BlitExD3D (IDirect3DTexture9 *texture, RECT *rDest, D3DCOLOR *vertexcolors, float rotate = 0);
  void Draw2DText(std::string sText, int fontType, int posX, int posY, int red, int green, int blue, int alpha = 255);
  void Draw2DTextValue(std::string sText, float value, int fontType, int posX, int posY, int red, int green, int blue, int alpha = 255);
  void LoadSprite(std::string sFilename);
  void LoadSpriteMore(std::string sFilename);
  void RenderSprite(float angle);
  void RenderTile(RECT dest,RECT source, float scale, float angle);
  void RenderTileMore(RECT dest,RECT source, float scale, float angle);
  void RenderTank(CTank tank, CTank player);
  void DrawCircle(float x, float y, float radius);
  RECT LoadRect(float left, float top, float right, float bottom);
  IDirect3DTexture9 * GetTexturePointer();
  IDirect3DTexture9 * GetTexturePointerMore();
  IDirect3DTexture9 * GetTextureSplashPointer();
  void SetTexturePointer(IDirect3DTexture9 * pTexture){m_pTexture = pTexture;}
  void SetTextureSplashPointer(IDirect3DTexture9 * pTexture){m_pTextureSplash = pTexture;}
  int GetScreenCenterX();
  int GetScreenCenterY();
  int GetScreenWidth();
  int GetScreenHeight();
  void SetScreenWidthHeight(int w, int h);
  void Draw2DValue(int value,int fontType, int posX, int posY, int red, int green, int blue, int alpha);
  void Draw2DValue(float value,int fontType, int posX, int posY, int red, int green, int blue, int alpha);
  
  //Log m_Log;
private:
    IDirect3D9* d3d;//Direct3D 9 interface
    IDirect3DDevice9* d3dDevice;//Graphics adapter interface
    D3DCAPS9 d3dCaps;//Capabilities of graphics adapter
    D3DPRESENT_PARAMETERS d3dPresent;//Direct3D present parameters
    IDirect3DVertexBuffer9* vertexBuffer;//Vertex buffer for drawing quads
    ID3DXFont *m_Font[C_NUMBER_FONTS];
    IDirect3DTexture9* m_texture;//<
    IDirect3DTexture9* m_pTextureMore;
    IDirect3DTexture9* m_texture2;
    IDirect3DTexture9* m_pTextureSplash;
    ID3DXSprite* m_sprite;
    ID3DXSprite* m_spriteMore;
    int m_screenWidth;
    int m_screenHeight;
    int m_screenCenterX;
    int m_screenCenterY;
    IDirect3DTexture9 * m_pTexture;

};

#endif