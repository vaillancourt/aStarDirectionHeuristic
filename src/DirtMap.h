/**
 * Copyright (c) 2015 Alexandre Vaillancourt. See full MIT license at the root of the repository.
 */
#pragma once

#include <stdint.h>

class DirtMap
{
public:
  
  DirtMap(
    float aWorldWidth, 
    float aWorldHeight, 
    int   aCanvasWidth,
    int   aCanvasHeight,
    float aDirtRate);
  ~DirtMap();
  void simulateOneStep();
  void patrol( float aX, float aY, float aRadius );
  const float*    getDirtMap()    const { return mDirtMap; }
  const uint8_t*  getDirtMapPix() const { return mIntMap; }
private:

  float mWorldWidth;
  float mWorldHeight;
  float mDirtRate;
  float* mDirtMap;
  uint8_t* mIntMap;
  float mDirtRatePerFrame;
  float mSizeToGridScale;

  int mCanvasWidth;
  int mCanvasHeight;
  int mGridTileCount;

};
