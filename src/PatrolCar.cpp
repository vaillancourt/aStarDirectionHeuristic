/**
 * Copyright (c) 2015 Alexandre Vaillancourt. See full MIT license at the root of the repository.
 */
#include "PatrolCar.h"

#include "Simulation.h"
#include "Graph.h"
#include "Arc.h"
#include "Node.h"

#include <SFML/System/Vector2.hpp> // DEVEL-AV remove from core classes. 

#include <iostream>

PatrolCar::PatrolCar( int aId )
  : mCurrentArc( -1 )
  , mCurrentNode( -1 )
  , mTravelDist( 0.0f )
  , mTravelSpeedKPH( 1.0f )
  , mPatrolRadius( 0.05f )
  , mWorldPositionX( 0.0f )
  , mWorldPositionY( 0.0f )
  , mId( aId )
{}


void
PatrolCar::simulateOneStep()
{
  Simulation& sim = Simulation::GetInstance();
  Graph&      graph = sim.getGraph();

  if ( mCurrentArc == -1 )
  {
    selectNewArc();
  }
  else // mCurrentArc != -1
  {
    PatrolCar::travelOnCurrentArc();
  }

  updateWorldPosition();
}


void 
PatrolCar::selectNewArc()
{
  Simulation& sim = Simulation::GetInstance();
  Graph&      graph = sim.getGraph();

  auto currNodeIt = graph.getNodes().find( mCurrentNode );

  if ( currNodeIt != graph.getNodes().end() )
  {
    auto currentNode = currNodeIt->second;

    if ( currentNode->getOutArcs().size() == 1 )
    {
      mCurrentArc = currentNode->getOutArcs()[0]->getId();
      currentNode->getOutArcs()[0]->getNodeTo()->setCarBoundtoVisit( mId );
    }
    else if ( currentNode->getOutArcs().size() > 1 )
    {
      int nodeWithMaxDirt = evaluateAndSelectDestinationNode();

      std::vector<int> path = graph.plot( mCurrentNode, nodeWithMaxDirt );

      int destNodeIndex = path.size() > 1 ? path[path.size() - 2] : -1;

      for ( auto arcIt : currentNode->getOutArcs() )
      {
        if ( destNodeIndex == arcIt->getNodeTo()->getId() )
        {
          mCurrentArc = arcIt->getId();
          arcIt->getNodeTo()->setCarBoundtoVisit( mId );
        }
      }
    }

    if ( mCurrentArc != -1 )
    {
      mCurrentNode = -1;
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


void 
PatrolCar::updateWorldPosition()
{
  // This method could probably be integrated into the simulateOneStep
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

void
PatrolCar::travelOnCurrentArc()
{
  Simulation& sim = Simulation::GetInstance();

  auto arcIt = sim.getGraph().getArcs().find( mCurrentArc );
  if ( arcIt != sim.getGraph().getArcs().end() )
  {
    mTravelDist += mTravelSpeedKPF;
    if ( mTravelDist >= arcIt->second->getDistance() )
    {
      mTravelDist = 0.0;
      mCurrentNode = arcIt->second->getNodeTo()->getId();
      arcIt->second->getNodeTo()->setCarBoundtoVisit( -1 );
      mCurrentArc = -1;
    }
  }
}

int 
PatrolCar::evaluateAndSelectDestinationNode()
{
  float maxScore = std::numeric_limits<float>::lowest();
  int nodeWithMaxScore = -1;
  float maxRadiusSquare = Simulation::GetInstance().getMaxRadiusSquare();

  // find the dirtiest node
  for ( auto it : Simulation::GetInstance().getGraph().getNodes() )
  {
    if ( it.second->getId() == mCurrentNode || it.second->getCarBoundToVisit() != -1 )
      continue;

    float distToNodeSquare =
      ( mWorldPositionX - it.second->getX() ) * ( mWorldPositionX - it.second->getX() ) +
      ( mWorldPositionY - it.second->getY() ) * ( mWorldPositionY - it.second->getY() );

    float distRatio = distToNodeSquare / maxRadiusSquare;
    float dirtRatio = static_cast<float>( it.second->getDirtLevelScaled() ) / 255.0f;
    // Closer the better
    // Dirtier the better
    float thisNodesScore = dirtRatio * ( 1.0f - distRatio );

    if ( thisNodesScore > maxScore )
    {
      maxScore = thisNodesScore;
      nodeWithMaxScore = it.first;
    }
  }

  return nodeWithMaxScore;
}

