/**
 * Copyright (c) 2015 Alexandre Vaillancourt. See full MIT license at the root of the repository.
 */
#include "Simulation.h"

#include "Global.h"
#include "Graph.h"
#include "Arc.h"
#include "Node.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

#include <iostream>
#include <stdlib.h>

bool Simulation::IsFromTerminate = false;
std::unique_ptr<Simulation> Simulation::Instance;

void 
Simulation::InitInstance()
{
  if ( Instance )
    std::cout << "Simulation::InitInstance() already called." << std::endl;
  else
    Instance = std::make_unique<Simulation>();
}

void 
Simulation::TerminateInstance()
{
  IsFromTerminate = true;
  Instance.reset();
}

Simulation& 
Simulation::GetInstance()
{
  return *(Instance.get());
}


Simulation::Simulation()
  : mTileToWorldScale( 4.0f )
  , mFrameRate( 30.0f )
  , mFrameTime( 1.0f / 30.0f )
  , mWorldWidth( 1.6f )
  , mWorldHeight( 1.6f / 4.0f * 3.0f )
  , mCanvaHeight( 192 )
  , mCanvaWidth( 256 )
{
}


Simulation::~Simulation()
{
  if ( !IsFromTerminate )
    std::cout << "Kill the simulation via Simulation::TerminateInstance()" << std::endl;
}

void
Simulation::populate()
{
  mGraph.reset( new Graph( "..\\data\\nodes.txt", "..\\data\\arcs.txt" ) );
  auto car = std::make_unique<PatrolCar>();
  car->mCurrentNode = 11;
  car->setTravelSpeedKPH( 100.0f );
  mPatrolCars.push_back( std::move( car ) );
  srand( 0 );
}

void
Simulation::simulateOneStep()
{
  for ( int i = 0; i < mPatrolCars.size(); ++i )
  {
    mPatrolCars[i]->simulateOneStep();
  }
}

void
Simulation::draw( sf::RenderTarget& aRenderTarget )
{
  sf::Vector2f canvasSize( static_cast<float>( mCanvaWidth ), static_cast<float>( mCanvaHeight ) );
  sf::Vector2f worldSize( mWorldWidth, mWorldHeight );
  sf::Vector2f worldToCanvasScale( canvasSize.x / worldSize.x, canvasSize.y / worldSize.y );

  // Background
  sf::RectangleShape background( canvasSize );
  background.setFillColor( sf::Color( COLOUR_BACKGROUND ) );
  aRenderTarget.draw(background);

  // Streets
  {
    auto arcs = mGraph->getArcs();
    for ( auto arcPair : arcs )
    {
      auto arc = arcPair.second;

      sf::Vector2f from( arc->getNodeFrom()->getX(), arc->getNodeFrom()->getY() );
      sf::Vector2f to( arc->getNodeTo()->getX(), arc->getNodeTo()->getY() );
      sf::Vector2f size;
      if ( from.x == to.x )
      {
        // Vertical
        size.x = 1.0;
        size.y = std::abs( from.y - to.y ) * worldToCanvasScale.y;
      }
      else
      {
        // Horizontal
        size.x = std::abs( from.x - to.x ) * worldToCanvasScale.x;
        size.y = 1.0;
      }

      //size.x = size.x * worldToCanvasScale.x;
      //size.y = size.y * worldToCanvasScale.y;

      sf::Vector2f origin(
        std::min(from.x, to.x) * worldToCanvasScale.x,
        std::min(from.y, to.y) * worldToCanvasScale.y);
      sf::RectangleShape street( size );
      street.setPosition( origin );
      street.setFillColor( sf::Color( COLOUR_ROAD ) );
      aRenderTarget.draw( street );
    }
  }
  // Dirt

  // Lights

  // Cars
  {
    for ( int i = 0; i < mPatrolCars.size(); ++i )
    {
      PatrolCar& patrolCar = *mPatrolCars[i].get();

      sf::Vector2f position( 0.0f, 0.0f );
      if ( patrolCar.mCurrentNode != -1 )
      {
        auto nodeIt = mGraph->getNodes().find( patrolCar.mCurrentNode );
        if ( nodeIt != mGraph->getNodes().end() )
        {
          position.x = nodeIt->second->getX();
          position.y = nodeIt->second->getY();
        }
      }
      else
      {
        auto arcIt = mGraph->getArcs().find( patrolCar.mCurrentArc );
        if ( arcIt != mGraph->getArcs().end() )
        {
          sf::Vector2f from( arcIt->second->getNodeFrom()->getX(), arcIt->second->getNodeFrom()->getY() );
          sf::Vector2f to  ( arcIt->second->getNodeTo  ()->getX(), arcIt->second->getNodeTo  ()->getY() );
          sf::Vector2f vect = to - from;
          float ratio = patrolCar.mTravelDist / arcIt->second->getDistance();

          position = from + (vect * ratio);
        }
      }
      position.x = position.x * worldToCanvasScale.x;
      position.y = position.y * worldToCanvasScale.y;

      sf::RectangleShape car( sf::Vector2f(1.0f, 1.0f ) );
      car.setPosition( position );
      car.setFillColor( sf::Color( COLOUR_POLICE_R ) );
      aRenderTarget.draw( car );
    }
  }

}

int
Simulation::rand( int aMax )
{
  return ::rand() % aMax;
}
