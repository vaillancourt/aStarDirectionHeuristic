/**
 * Copyright (c) 2015 Alexandre Vaillancourt. See full MIT license at the root of the repository.
 */
#include "DirtMap.h"
#include "Simulation.h"

#include <iostream>
#include <iomanip>
#include <cstring>

DirtMap::DirtMap( 
  float aWorldWidth, 
  float aWorldHeight, 
  int   aCanvasWidth,
  int   aCanvasHeight,
  float aDirtRate )
  : mWorldWidth     ( aWorldWidth )
  , mWorldHeight    ( aWorldHeight )
  , mCanvasWidth    ( aCanvasWidth )
  , mCanvasHeight   ( aCanvasHeight )
  , mDirtRate       ( aDirtRate )
  , mSizeToGridScale( aWorldWidth / static_cast<float>( aCanvasWidth ))
  , mGridTileCount  ( mCanvasWidth * mCanvasHeight )
  , mDirtMap        ( nullptr )
  , mIntMap         ( nullptr )
{
  mDirtMap = new float[mGridTileCount];
  for ( int i = 0; i < mGridTileCount; ++i )
    mDirtMap[i] = 0.0f;

  mDirtRatePerFrame = aDirtRate / Simulation::GetInstance().mFrameRate;

  mIntMap = new uint8_t[mGridTileCount * 4];
  memset( mIntMap, 0, sizeof( uint32_t ) * mGridTileCount );
}

DirtMap::~DirtMap()
{
  if ( mDirtMap )
    delete[] mDirtMap;

  if ( mIntMap )
    delete[] mIntMap;
}

void 
DirtMap::simulateOneStep()
{
  for ( int y = 0; y < mCanvasHeight; ++y )
  {
    for ( int x = 0; x < mCanvasWidth; ++x )
    {
      bool debug = false;
      if ( x == 0 && y == 0 )
        debug = false;

      int index = y * mCanvasWidth + x;
      mDirtMap[index] += mDirtRatePerFrame;
      if ( mDirtMap[index] > 1.0f )
        mDirtMap[index] = 1.0f;

      uint8_t value = static_cast<uint8_t>( mDirtMap[index] * 255.0f );
      if ( value > 200 )
        value = 200u;

      //value = 255 - value;
      mIntMap[index * 4    ] = 0;
      mIntMap[index * 4 + 1] = 0;
      mIntMap[index * 4 + 2] = 0;
      mIntMap[index * 4 + 3] = value;
      //static_cast<uint32_t>(
      //  value << 24 | 
      //  value << 16 | 
      //  value <<  8 | 
      //  static_cast<uint8_t>(127)  );
    }
  }
}


void 
DirtMap::patrol( float aX, float aY, float aRadius )
{
  float f = aRadius / mSizeToGridScale;
  int tileRadiusSq = static_cast<int>(f) * static_cast<int>(f);
  int originX = aX / mSizeToGridScale;
  int originY = aY / mSizeToGridScale;

  for ( int y = 0; y < mCanvasHeight; ++y )
  {
    for ( int x = 0; x < mCanvasWidth; ++x )
    {
      if ( (originX - x) * (originX - x) + (originY - y) * (originY - y) < tileRadiusSq )
        mDirtMap[y * mCanvasWidth + x] = 0.0f;
    }
  }
}


float 
DirtMap::getDirtLevel( float aX, float aY )
{
  int originX = aX / mSizeToGridScale;
  int originY = aY / mSizeToGridScale;

  return mDirtMap[originY * mCanvasWidth + originX];
}