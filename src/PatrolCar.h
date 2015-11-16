/**
 * Copyright (c) 2015 Alexandre Vaillancourt. See full MIT license at the root of the repository.
 */
#pragma once

class PatrolCar
{
public:
  PatrolCar( int aId );

  void simulateOneStep();

  int mCurrentArc;
  int mCurrentNode;
  float mTravelDist;

  void setTravelSpeedKPH( float aValue );

  void putWorldPosition( float& aX, float& aY ) const { aX = mWorldPositionX; aY = mWorldPositionY; }

  float getPatrolRadius() const { return mPatrolRadius; }
  float mPatrolRadius;

private:

  void updateWorldPosition();

  void selectNewArc();
  void travelOnCurrentArc();
  int  evaluateAndSelectDestinationNode();


  float mTravelSpeedKPH;
  float mTravelSpeedKPF; /// K per frame

  int mId;


  float mWorldPositionX;
  float mWorldPositionY;
};