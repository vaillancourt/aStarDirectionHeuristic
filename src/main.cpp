/**
 * Copyright (c) 2015 Alexandre Vaillancourt. See full MIT license at the root of the repository.
 */
#include "Global.h"
#include "Simulation.h"

#include <SFML/Graphics.hpp>
#include <string>
#include <chrono>
#include <thread>
#include <iostream>


int 
main(void)
{
  Simulation::InitInstance();
  Simulation::GetInstance().populate();

  sf::RenderWindow window(sf::VideoMode(256, 192), "SFML works!");

  std::chrono::milliseconds frameDurationMs( 1000 / FRAME_RATE );

  int frameCount = 0;

  auto secondEnd = std::chrono::high_resolution_clock::now() + std::chrono::seconds( 1 );

  while (window.isOpen())
  {
    auto startTimePoint = std::chrono::high_resolution_clock::now();
    auto endTimePoint   = startTimePoint + frameDurationMs;

    sf::Event event;
    while (window.pollEvent(event))
    {
      if (event.type == sf::Event::Closed)
        window.close();
    }

    Simulation::GetInstance().simulateOneStep();
    window.clear();
    Simulation::GetInstance().draw( window );
    window.display();
    
    auto now = std::chrono::high_resolution_clock::now();
    if ( now < endTimePoint )
    {
      std::this_thread::sleep_until( endTimePoint );
    }
    ++frameCount;
    if ( std::chrono::high_resolution_clock::now() > secondEnd )
    {
      //std::cout << frameCount << std::endl;
      frameCount = 0;
      secondEnd = std::chrono::high_resolution_clock::now() + std::chrono::seconds( 1 );
    }
  }

  Simulation::TerminateInstance();
  return 0;
}