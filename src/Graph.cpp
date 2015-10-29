/**
 * Copyright (c) 2015 Alexandre Vaillancourt. See full MIT license at the root of the repository.
 */
#include "Graph.h"

#include "Arc.h"
#include "Node.h"

#include <iostream>
#include <fstream>
#include <cmath>

Graph::Graph( const std::string& aNodeFilePath, const std::string& aArcFilePath )
{
  {
    std::ifstream myfile( aNodeFilePath );
    if ( myfile.is_open() )
    {
      int id;
      float x;
      float y;
      int count = 0;

      myfile >> count;
      for ( int i = 0; i < count; ++i )
      {
        myfile >> id >> x >> y;
        auto newNode = std::make_shared<Node>( id, x, y );

        // DEVEL make sure the node is not there twice. 
        mNodes[newNode->getId()] = newNode;
      }
    }
    else
    {
      std::cout << "Can't open node file at '" << aNodeFilePath << "." << std::endl;
    }
  }
  {
    std::ifstream myfile( aArcFilePath );
    if ( myfile.is_open() )
    {
      int id;
      int fromId;
      int toId;
      int count = 0;

      myfile >> count;
      for ( int i = 0; i < count; ++i )
      {
        myfile >> id >> fromId >> toId;
        auto fromNodeIt = mNodes.find( fromId );
        if ( fromNodeIt == mNodes.end() )
        {
          std::cout << "could not find 'from' node with ID '" << fromId << "'.";
          continue;
        }

        auto toNodeIt = mNodes.find( toId );
        if ( toNodeIt == mNodes.end() )
        {
          std::cout << "could not find 'to' node with ID '" << toId << "'.";
          continue;
        }


        auto newArc = std::make_shared<Arc>( id, fromNodeIt->second , toNodeIt->second );
        newArc->setTravelCost(
          std::sqrt(
          std::pow( ( newArc->getNodeTo()->getX() - newArc->getNodeFrom()->getX() ), 2 ) +
          std::pow( ( newArc->getNodeTo()->getY() - newArc->getNodeFrom()->getY() ), 2 )
          )
          );
        // DEVEL make sure there the arc is not there twice
        mArcs[newArc->getId()] = newArc;
        fromNodeIt->second->addOutArc( newArc );
        toNodeIt  ->second->addInArc( newArc );
      }

    }
    else
    {
      std::cout << "Can't open arc file at '" << aArcFilePath << "." << std::endl;
    }
  }
}