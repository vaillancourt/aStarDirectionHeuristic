/**
 * Copyright (c) 2015 Alexandre Vaillancourt. See full MIT license at the root of the repository.
 */
#include "Node.h"

#include "DirtMap.h"
#include "Simulation.h"

Node::Node( int aId, float aX, float aY, float aDirtLevel )
  : mId( aId )
  , mX( aX )
  , mY( aY )
  , mDirtLevel( aDirtLevel )
{}

void 
Node::addInArc( std::shared_ptr<Arc> aArc )
{
  // DEVEL check if the arc is already in there
  mArcsIn.push_back( aArc );
}

void 
Node::addOutArc( std::shared_ptr<Arc> aArc )
{
  // DEVEL check if the arc is already in there
  mArcsOut.push_back( aArc );
}

float
Node::getDirtLevel() const
{
  float dirtLevel = Simulation::GetInstance().getDirtMap().getDirtLevel( mX, mY );

  return dirtLevel;
}