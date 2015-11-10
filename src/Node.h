/**
 * Copyright (c) 2015 Alexandre Vaillancourt. See full MIT license at the root of the repository.
 */
#pragma once

#include <memory>
#include <vector>

class Arc;

class Node
{
public:
  Node( int aId, float aX, float aY, float aDirtLevel = 0.0f );
  void addInArc( std::shared_ptr<Arc> aArc );
  void addOutArc( std::shared_ptr<Arc> aArc );

  int getId() const { return mId; }
  float getX()  const { return mX; }
  float getY()  const { return mY; }
  uint8_t getDirtLevelScaled() const;
  
  std::vector< std::shared_ptr<Arc> >& getOutArcs() { return mArcsOut; }

  int getCarBoundToVisit() const { return mCarBoundToVisit; }
  void setCarBoundtoVisit( int aValue ) { mCarBoundToVisit = aValue; }

private:
  float mDirtLevel;
  int mId;
  float mX;
  float mY;

  int mCarBoundToVisit;

  std::vector< std::shared_ptr<Arc> > mArcsIn;
  std::vector< std::shared_ptr<Arc> > mArcsOut;
};

