/**
 * Copyright (c) 2015 Alexandre Vaillancourt. See full MIT license at the root of the repository.
 */
#include "PatrolCarLight.h"

#include <cstring>

PatrolCarLight::PatrolCarLight( 
  float aRadius, 
  int aRadiusInt, 
  const sf::Color& aColorState0, 
  const sf::Color& aColorState1,
  int aTotalFrameInState )
  : mColourState0( aColorState0 )
  , mColourState1( aColorState1 )
  , mRadius( aRadius )
  , mRadiusInt( aRadiusInt )
  , mIntArrayState0( nullptr )
  , mIntArrayState1( nullptr )
  , mTotalFrameInState(aTotalFrameInState)
  , mFramesInThisState( 0 )
  , mCurrentState( 0 )
{
  int side = getLightSize();
  int surface = side * side;

  mIntArrayState0 = new uint8_t[surface * 4];
  memset( mIntArrayState0, 0, sizeof( uint8_t ) * surface * 4 );

  mIntArrayState1 = new uint8_t[surface * 4];
  memset( mIntArrayState1, 0, sizeof( uint8_t ) * surface * 4 );

  int tileRadiusSq = mRadiusInt * mRadiusInt;
  int originX = mRadiusInt;
  int originY = mRadiusInt;

  for ( int y = 0; y < side; ++y )
  {
    for ( int x = 0; x < side; ++x )
    {
      if ( ( originX - x ) * ( originX - x ) + ( originY - y ) * ( originY - y ) < tileRadiusSq )
      {
        //uint8_t* mIntArrayState0;
        //uint8_t* mIntArrayState1;

        int index = y * ( side ) + x;

        if ( x < y )
        {
          mIntArrayState0[index * 4]     = mColourState0.r;
          mIntArrayState0[index * 4 + 1] = mColourState0.g;
          mIntArrayState0[index * 4 + 2] = mColourState0.b;
          mIntArrayState0[index * 4 + 3] = mColourState0.a / 4 * 3;

          mIntArrayState1[index * 4]     = mColourState1.r;
          mIntArrayState1[index * 4 + 1] = mColourState1.g;
          mIntArrayState1[index * 4 + 2] = mColourState1.b;
          mIntArrayState1[index * 4 + 3] = mColourState1.a / 4 * 3;
        }
        else
        {
          mIntArrayState0[index * 4]     = mColourState1.r;
          mIntArrayState0[index * 4 + 1] = mColourState1.g;
          mIntArrayState0[index * 4 + 2] = mColourState1.b;
          mIntArrayState0[index * 4 + 3] = mColourState1.a / 4 * 3;

          mIntArrayState1[index * 4]     = mColourState0.r;
          mIntArrayState1[index * 4 + 1] = mColourState0.g;
          mIntArrayState1[index * 4 + 2] = mColourState0.b;
          mIntArrayState1[index * 4 + 3] = mColourState0.a / 4 * 3;
        }
      }
    }
  }
}

PatrolCarLight::PatrolCarLight( PatrolCarLight&& aOther )
  : mRadius( aOther.mRadius)
  , mRadiusInt( aOther.mRadiusInt )
  , mTotalFrameInState( aOther. mTotalFrameInState)
  , mFramesInThisState( aOther.mFramesInThisState )
  , mCurrentState( aOther.mCurrentState )
  , mColourState0( aOther.mColourState0 )
  , mColourState1( aOther.mColourState1 )
  , mIntArrayState0( aOther.mIntArrayState0 )
  , mIntArrayState1( aOther.mIntArrayState1 )
{
  aOther.mIntArrayState0 = nullptr;
  aOther.mIntArrayState1 = nullptr;
}


PatrolCarLight::~PatrolCarLight()
{
  if ( mIntArrayState0 )
    delete[] mIntArrayState0;

  if ( mIntArrayState1 )
    delete[] mIntArrayState1;
}


void 
PatrolCarLight::simulateOneStep()
{
   ++mFramesInThisState; 
   if ( mFramesInThisState == mTotalFrameInState ) 
   {
     mFramesInThisState = 1;
     mCurrentState = ( mCurrentState == 0 ) ?
       1 :
       0 ;
   }
}

