/**
 * Copyright (c) 2015 Alexandre Vaillancourt. See full MIT license at the root of the repository.
 */
#pragma once

class PatrolCar
{
public:
  PatrolCar();

  void simulateOneStep();

  int mCurrentArc;
  int mCurrentNode;
  float mTravelDist;

  void setTravelSpeedKPH( float aValue );

  void putWorldPosition( float& aX, float& aY ) const { aX = mWorldPositionX; aY = mWorldPositionY; }

  float mPatrolRadius;

private:

  void updateWorldPosition();

  float mTravelSpeedKPH;
  float mTravelSpeedKPF; /// K per frame


  float mWorldPositionX;
  float mWorldPositionY;
};