#ifndef CSHELLIMPACT_H
#define CSHELLIMPACT_H
#include <windows.h>
#include <string>
#include <vector>
#include <deque>

struct EXPLOSION{
  POINT pt;       //coordinates of impact point
  int type;       //type of explosion
  int frameCount; //0-17...indicates frame
  bool active;    //if true..animate...false...show crater
  int crater;
  int frameOffset;//0, 1, 2
};

class CShellImpact{
public:
  CShellImpact();
  RECT GetImageSource(int id);
  RECT GetImpactDestination(int id);
  void Update(int id);
  void MouseClick(const POINT p, int type);
  void ImpactExplosion(const POINT p, int type);
  int Size();
  void Clear();
  float GetImpactX(int id);
  float GetImpactY(int id);

private:
  RECT m_imageRect[20];
  //std::deque<EXPLOSION> m_exp;
  std::vector<EXPLOSION> m_exp;
  int m_count;
  int m_maxCount;
  
};

#endif