/**
 * Copyright (c) 2015 Alexandre Vaillancourt. See full MIT license at the root of the repository.
 */
#pragma once

#include <vector>

class PatrolCar
{
public:
  PatrolCar( int aId );

  void simulateOneStep();

  int mCurrentArc;
  int mCurrentNode;
  float mTravelDist;
  int mNextNode;

  void setTravelSpeedKPH( float aValue );

  void putWorldPosition( float& aX, float& aY ) const { aX = mWorldPositionX; aY = mWorldPositionY; }
  void putDestinationWorldPosition( float& aX, float& aY );

  float getPatrolRadius() const { return mPatrolRadius; }

private:

  void updateWorldPosition();

  void selectNewArc();
  void travelOnCurrentArc();
  int  evaluateAndSelectDestinationNode();


  float mPatrolRadius;
  float mTravelSpeedKPH;
  float mTravelSpeedKPF; /// K per frame

  int mId;

  std::vector<int> mPath;


  float mWorldPositionX;
  float mWorldPositionY;

  float mDestinationWorldPositionX;
  float mDestinationWorldPositionY;
};