/**
 * Copyright (c) 2015 Alexandre Vaillancourt. See full MIT license at the root of the repository.
 */
#pragma once

#include <memory>
#include <vector>

#include <SFML/Graphics/Sprite.hpp>

namespace sf
{
  class Image;
  class RenderTarget;
  class Texture;
}

class DirtMap;
class Graph;
class PatrolCar;
class PatrolCarLight;

class Simulation
{
  Simulation(const Simulation& that) = delete;
  Simulation& operator=(const Simulation&) = delete;
public:

  enum class Entity
  {
    BACKGROUND,
    ROADS,
    DIRT,
    LIGHTS,
    CAR
  };

  static void InitInstance();
  static void TerminateInstance();
  static Simulation& GetInstance();



  void simulateOneStep();
  void draw( sf::RenderTarget& aRenderTarget );

  int rand( int aMax );

  void populate();

  Graph& getGraph() { return *mGraph.get(); }
  DirtMap& getDirtMap() { return *mDirtMap.get(); }

  float mFrameRate;
  float mFrameTime;
  float mWorldWidth;
  float mWorldHeight;
  int mCanvaHeight;
  int mCanvaWidth;

  float getMaxRadius() const { return mMaxRadius; }
  float getMaxRadiusSquare() const { return mMaxRadiusSquare; }
  float getWorldToGraphicRatio() const { return mWorldToGraphicRatio; }

  ~Simulation();
  Simulation();

private:

  std::unique_ptr<Graph> mGraph;
  std::unique_ptr<DirtMap> mDirtMap;

  void drawBackground( sf::RenderTarget& aRenderTarget );
  void drawStreets( sf::RenderTarget& aRenderTarget );
  void drawDirt( sf::RenderTarget& aRenderTarget );
  void drawPatrolCars( sf::RenderTarget& aRenderTarget );
  void drawLigts( sf::RenderTarget& aRenderTarget );

  std::vector< std::unique_ptr<PatrolCar> > mPatrolCars;
  std::vector< std::unique_ptr<PatrolCarLight> > mPatrolCarLights;


  float mMaxRadius;
  float mMaxRadiusSquare;
  const float mWorldToGraphicRatio;

  std::unique_ptr<sf::Image> mDirtImage;
  std::unique_ptr<sf::Texture> mDirtTexture;
  sf::Sprite mDirtMapSprite;

  std::vector<std::unique_ptr< sf::Image > >   mPatrolCarLightsImages;
  std::vector<std::unique_ptr< sf::Texture > > mPatrolCarLightsTextures;
  std::vector<sf::Sprite>                      mPatrolCarLightsSprites;

  static std::unique_ptr<Simulation> Instance;
  static bool                        IsFromTerminate;;
};