/**
 * Copyright (c) 2015 Alexandre Vaillancourt. See full MIT license at the root of the repository.
 */
#include "PatrolCar.h"

#include "Simulation.h"
#include "Graph.h"
#include "Arc.h"
#include "Node.h"

#include <SFML/System/Vector2.hpp> // DEVEL-AV remove from core classes. 


PatrolCar::PatrolCar()
  : mCurrentArc( -1 )
  , mCurrentNode( -1 )
  , mTravelDist( 0.0f )
  , mTravelSpeedKPH( 1.0f )
  , mPatrolRadius( 0.05f )
  , mWorldPositionX( 0.0f )
  , mWorldPositionY( 0.0f )
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

  updateWorldPosition();
}

void 
PatrolCar::setTravelSpeedKPH( float aValue )
{
  mTravelSpeedKPH = aValue;

  Simulation& sim = Simulation::GetInstance();
  
  mTravelSpeedKPF = mTravelSpeedKPH / 60.0f / 60.0f / sim.mFrameRate;
}


void 
PatrolCar::updateWorldPosition()
{
  // This mehtod could probably be integrated into the simulateOneStep
  mWorldPositionX = 0.0f;
  mWorldPositionY = 0.0f;

  Simulation& sim = Simulation::GetInstance();
  Graph&      graph = sim.getGraph();
  

  if ( mCurrentNode != -1 )
  {
    auto nodeIt = graph.getNodes().find( mCurrentNode );
    if ( nodeIt != graph.getNodes().end() )
    {
      mWorldPositionX = nodeIt->second->getX();
      mWorldPositionY = nodeIt->second->getY();
    }
    // DEVEL-AV else?
  }
  else
  {
    auto arcIt = graph.getArcs().find( mCurrentArc );
    if ( arcIt != graph.getArcs().end() )
    {
      sf::Vector2f from( arcIt->second->getNodeFrom()->getX(), arcIt->second->getNodeFrom()->getY() );
      sf::Vector2f to  ( arcIt->second->getNodeTo  ()->getX(), arcIt->second->getNodeTo  ()->getY() );
      sf::Vector2f vect = to - from;
      float ratio = mTravelDist / arcIt->second->getDistance();

      sf::Vector2f position = from + (vect * ratio);
      mWorldPositionX = position.x;
      mWorldPositionY = position.y;
    }
    // DEVEL-AV else?
  }
}