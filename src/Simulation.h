/**
 * Copyright (c) 2015 Alexandre Vaillancourt. See full MIT license at the root of the repository.
 */
#pragma once

#include "Graph.h"
#include "PatrolCar.h"

#include <memory>
#include <vector>

namespace sf
{
  class RenderTarget;
}

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

  float mTileToWorldScale; // 
  float mFrameRate;
  float mFrameTime;
  float mWorldWidth;
  float mWorldHeight;
  int mCanvaHeight;
  int mCanvaWidth;

  ~Simulation();
  Simulation();
private:

  std::unique_ptr<Graph> mGraph;

  std::vector< std::unique_ptr<PatrolCar> > mPatrolCars;

  static std::unique_ptr<Simulation> Instance;
  static bool                        IsFromTerminate;;
};