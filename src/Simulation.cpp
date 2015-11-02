/**
 * Copyright (c) 2015 Alexandre Vaillancourt. See full MIT license at the root of the repository.
 */
#include "Simulation.h"

#include "DirtMap.h"
#include "Global.h"
#include "Graph.h"
#include "Arc.h"
#include "Node.h"

#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Texture.hpp>

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
  : mFrameRate( FRAME_RATE )
  , mFrameTime( 1.0f / FRAME_RATE )
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
  mDirtMap = std::make_unique<DirtMap>(
    mWorldWidth, mWorldHeight,
    mCanvaWidth, mCanvaHeight,
    0.01f );

  mDirtImage = std::make_unique<sf::Image>();
  mDirtImage->create(
    static_cast<unsigned int>( mCanvaWidth ),
    static_cast<unsigned int>( mCanvaHeight ),
    mDirtMap->getDirtMapPix() );
  mDirtTexture = std::make_unique<sf::Texture>();
  mDirtTexture->loadFromImage( *mDirtImage.get() );

  sprite.setTexture( *mDirtTexture.get() );
}


void
Simulation::simulateOneStep()
{
  mDirtMap->simulateOneStep();
  for ( int i = 0; i < mPatrolCars.size(); ++i )
  {
    mPatrolCars[i]->simulateOneStep();
    float x = 0.0f;
    float y = 0.0f;
    mPatrolCars[i]->putWorldPosition( x, y );
    mDirtMap->patrol( x, y, mPatrolCars[i]->mPatrolRadius );
  }
}


void
Simulation::draw( sf::RenderTarget& aRenderTarget )
{
  sf::Vector2f canvasSize( static_cast<float>( mCanvaWidth ), static_cast<float>( mCanvaHeight ) );
  sf::Vector2f worldSize( mWorldWidth, mWorldHeight );
  sf::Vector2f worldToCanvasScale( canvasSize.x / worldSize.x, canvasSize.y / worldSize.y );

  drawBackground( aRenderTarget );
  drawStreets( aRenderTarget );
  drawDirt( aRenderTarget );
  // Lights
  drawPatrolCars( aRenderTarget );
}

void 
Simulation::drawBackground( sf::RenderTarget& aRenderTarget )
{
  sf::Vector2f canvasSize( static_cast<float>( mCanvaWidth ), static_cast<float>( mCanvaHeight ) );

  sf::RectangleShape background( canvasSize );
  background.setFillColor( sf::Color( COLOUR_BACKGROUND ) );
  aRenderTarget.draw(background);
}


void 
Simulation::drawStreets( sf::RenderTarget& aRenderTarget )
{
  sf::Vector2f canvasSize( static_cast<float>( mCanvaWidth ), static_cast<float>( mCanvaHeight ) );
  sf::Vector2f worldSize( mWorldWidth, mWorldHeight );
  sf::Vector2f worldToCanvasScale( canvasSize.x / worldSize.x, canvasSize.y / worldSize.y );

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


void 
Simulation::drawDirt( sf::RenderTarget& aRenderTarget )
{
  mDirtTexture->update( mDirtMap->getDirtMapPix() );
  aRenderTarget.draw( sprite );
  //sf::Vector2f pixelSquare( 1.0f, 1.0f );
  //const float* dirtMap = mDirtMap->getDirtMap();
  //sf::RectangleShape pixel( pixelSquare );
  //for ( int y = 0; y < mCanvaHeight; ++y )
  //{
  //  for ( int x = 0; x < mCanvaWidth; ++x )
  //  {
  //    sf::Vector2f position( x, y );
  //      
  //    float ratio = dirtMap[y * mCanvaWidth + x];
  //    pixel.setPosition( position );
  //    pixel.setFillColor( sf::Color( ratio, ratio, ratio, 0.5f ) );
  //    aRenderTarget.draw( pixel );
  //  }
  //}
}


void 
Simulation::drawPatrolCars( sf::RenderTarget& aRenderTarget )
{
  sf::Vector2f canvasSize( static_cast<float>( mCanvaWidth ), static_cast<float>( mCanvaHeight ) );
  sf::Vector2f worldSize( mWorldWidth, mWorldHeight );
  sf::Vector2f worldToCanvasScale( canvasSize.x / worldSize.x, canvasSize.y / worldSize.y );

  for ( int i = 0; i < mPatrolCars.size(); ++i )
  {
    PatrolCar& patrolCar = *mPatrolCars[i].get();

    sf::Vector2f position;
    patrolCar.putWorldPosition( position.x, position.y );

    position.x = position.x * worldToCanvasScale.x;
    position.y = position.y * worldToCanvasScale.y;

    sf::RectangleShape car( sf::Vector2f(1.0f, 1.0f ) );
    car.setPosition( position );
    car.setFillColor( sf::Color( COLOUR_POLICE_R ) );
    aRenderTarget.draw( car );
  }
}


int
Simulation::rand( int aMax )
{
  return ::rand() % aMax;
}