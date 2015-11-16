/**
 * Copyright (c) 2015 Alexandre Vaillancourt. See full MIT license at the root of the repository.
 */
#pragma once

#include <SFML/Graphics/Color.hpp>

#include <stdint.h>


class PatrolCarLight
{
public:
  PatrolCarLight( 
    float aRadius, 
    int aRadiusInt, 
    const sf::Color& aColourState0, 
    const sf::Color& aColourState1,
    int aTotalFrameInState );

  ~PatrolCarLight();

  PatrolCarLight( PatrolCarLight&& aOther );

  const uint8_t*  getLightPix() const { return (mCurrentState == 0) ? mIntArrayState0 : mIntArrayState1; }

  void simulateOneStep();

  int getLightSize() const { return mRadiusInt * 2 + 1; };


private:

  float mRadius;
  int   mRadiusInt;
  uint8_t* mIntArrayState0;
  uint8_t* mIntArrayState1;
  int mTotalFrameInState;
  int mFramesInThisState;
  int mCurrentState;

  sf::Color mColourState0;
  sf::Color mColourState1;

};
