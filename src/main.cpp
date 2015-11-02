/**
 * Copyright (c) 2015 Alexandre Vaillancourt. See full MIT license at the root of the repository.
 */
#include "Global.h"
#include "Simulation.h"

#include <SFML/Graphics.hpp>
#include <string>
#include <chrono>
#include <thread>


int 
main(void)
{
  Simulation::InitInstance();
  Simulation::GetInstance().populate();

  sf::RenderWindow window(sf::VideoMode(256, 192), "SFML works!");

  
  std::chrono::duration<int, std::ratio<1, FRAME_RATE> > frameDuration;

  while (window.isOpen())
  {
    auto startTimePoint = std::chrono::high_resolution_clock::now();
    auto endTimePoint   = startTimePoint + frameDuration;

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

    std::this_thread::sleep_until( endTimePoint );
  }

  Simulation::TerminateInstance();
  return 0;
}