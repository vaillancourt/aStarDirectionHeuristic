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

private:
  float mTravelSpeedKPH;
  float mTravelSpeedKPF; /// K per frame
};