/**
 * Copyright (c) 2015 Alexandre Vaillancourt. See full MIT license at the root of the repository.
 */
#include "PatrolCar.h"

#include "Simulation.h"
#include "Graph.h"
#include "Arc.h"
#include "Node.h"


PatrolCar::PatrolCar()
  : mCurrentArc( -1 )
  , mCurrentNode( -1 )
  , mTravelDist( 0.0f )
  , mTravelSpeedKPH( 1.0f )
{}


void
PatrolCar::simulateOneStep()
{
  Simulation& sim = Simulation::GetInstance();
  if ( mCurrentArc == -1 )
  {
    auto currNodeIt = sim.getGraph().getNodes().find( mCurrentNode );

    if ( currNodeIt != sim.getGraph().getNodes().end() )
    {
      auto node = currNodeIt->second;
      int newArcIdx = -1;

      if ( node->getOutArcs().size() == 1 )
        newArcIdx = 0;
      else if ( node->getOutArcs().size() > 1 )
        newArcIdx = sim.rand( node->getOutArcs().size() );

      if ( newArcIdx != -1 )
      {
        auto newArc = node->getOutArcs()[newArcIdx];
        mCurrentNode = -1;
        mCurrentArc = newArc->getId();
      }
    }
  }
  else // mCurrentArc != -1
  {
    auto arcIt = sim.getGraph().getArcs().find( mCurrentArc );
    if ( arcIt != sim.getGraph().getArcs().end() )
    {
      mTravelDist += mTravelSpeedKPF;
      if ( mTravelDist >= arcIt->second->getDistance() )
      {
        mTravelDist = 0.0;
        mCurrentNode = arcIt->second->getNodeTo()->getId();
        mCurrentArc = -1;
      }
    }
  }
}

void 
PatrolCar::setTravelSpeedKPH( float aValue )
{
  mTravelSpeedKPH = aValue;

  Simulation& sim = Simulation::GetInstance();
  
  mTravelSpeedKPF = mTravelSpeedKPH / 60.0f / 60.0f / sim.mFrameRate;
}