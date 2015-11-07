/**
 * Copyright (c) 2015 Alexandre Vaillancourt. See full MIT license at the root of the repository.
 */
#include "Graph.h"

#include "Arc.h"
#include "Node.h"

#include <glm/vec2.hpp>

#include <iostream>
#include <fstream>
#include <cmath>
#include <set>
#include <limits>

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


std::vector<int> 
Graph::plot( int aStart, int aEnd )
{
  std::set<int> closedSet;
  std::set<int> openSet = {aStart};

  std::map<int, int> cameFrom;
 
  std::map<int, float> gScore;
  for ( auto node : mNodes )
    gScore[node.first] = std::numeric_limits<float>::infinity();

  gScore[aStart] = 0.0f;
  std::map<int, float> fScore;
  for ( auto node : mNodes )
    fScore[node.first] = std::numeric_limits<float>::infinity();

  fScore[aStart] = gScore[aStart] + getHeuristicBetween( aStart, aEnd );

  while (openSet.size() > 0)
  {
    int current = -1;
    float currentMinVal = std::numeric_limits<float>::infinity();
    for ( auto nodeIndex : openSet )
    {
      if ( fScore[nodeIndex] < currentMinVal )
      {
        current = nodeIndex;
        currentMinVal = fScore[nodeIndex];
      }
    }

    if ( current == aEnd )
      return reconstructPath(cameFrom, aEnd);

    openSet.erase( current );
    closedSet.insert( current );
    for ( auto outArc : mNodes[current]->getOutArcs() )
    {
      int neighbourIndex = outArc->getNodeTo()->getId();
      if ( closedSet.find(neighbourIndex) != closedSet.end() )
        continue;
      float tentativeGscore = gScore[current] + outArc->getDistance();
      if ( openSet.find(neighbourIndex) == openSet.end() )
        openSet.insert(neighbourIndex);
      else if ( tentativeGscore >= gScore[neighbourIndex] )
        continue;

      cameFrom[neighbourIndex] = current;
      gScore[neighbourIndex] = tentativeGscore;
      fScore[neighbourIndex] = gScore[neighbourIndex] + getHeuristicBetween( neighbourIndex, aEnd );
    }
  }
}

float 
Graph::getHeuristicBetween( int aFromNode, int aToNode ) const
{
  auto nodeStartIt = mNodes.find( aFromNode );
  auto nodeEndIt   = mNodes.find( aToNode );
  if ( nodeStartIt == mNodes.end() || nodeEndIt == mNodes.end() )
    return -1.0f;
  return 
    (   glm::vec2( nodeStartIt->second->getX(), nodeStartIt->second->getY() )
      - glm::vec2( nodeEndIt->second->getX()  , nodeEndIt->second->getY() ) ).length();
}

std::vector<int> 
Graph::reconstructPath( const std::map<int, int>& aCameFrom, int aCurrent )
{
  std::vector<int> path;
  path.push_back( aCurrent );
  while ( aCameFrom.find(aCurrent) != aCameFrom.end() )
  {
    aCurrent = aCameFrom.at(aCurrent);
    path.push_back( aCurrent );
  }
  return path;
}
