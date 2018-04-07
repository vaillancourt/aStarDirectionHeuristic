/**
 * Copyright (c) 2015 Alexandre Vaillancourt. See full MIT license at the root of the repository.
 */
#include "Graph.h"

#include "Arc.h"
#include "Node.h"

#include <glm/vec2.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/epsilon.hpp>

#include <algorithm>
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
Graph::plot( int aStart, int aEnd, bool aShouldOutputDebug )
{
  if ( aShouldOutputDebug )
    std::cout << "new plot" << std::endl;

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

  fScore[aStart] = getHeuristicBetween( aStart, aEnd, aStart );
  if ( aShouldOutputDebug )
  {
    std::cout << "plot cost between " << aStart << " and " << aEnd << ": " << fScore[aStart] << std::endl;
  }

  while (openSet.size() > 0)
  {
    int current = -1;
    float currentMinVal = std::numeric_limits<float>::infinity();
    if ( aShouldOutputDebug )
    {
      std::cout << "OpenSet:";
      for ( auto nodeIndex : openSet )
      {
        std::cout << " " << nodeIndex;
      }
      std::cout << std::endl;
    }

    for ( auto nodeIndex : openSet )
    {
      if ( aShouldOutputDebug )
      {
        std::cout << "Processing node " << nodeIndex << std::endl;
      }

      auto epsilonLessThan = []( float aLhs, float aRhs, float aEpsilon ) {
        if ( std::abs( aLhs - aRhs ) < std::abs( aEpsilon ) )
          return false;
        return aLhs < aRhs;
      };

      if ( epsilonLessThan( fScore[nodeIndex], currentMinVal, 0.00005f) )
      {
        current = nodeIndex;
        currentMinVal = fScore[nodeIndex];
        if ( aShouldOutputDebug )
        {
          std::cout << "  new current selected with MinVal " << currentMinVal << std::endl;
        }
      }
      else if ( glm::epsilonEqual( fScore[nodeIndex], currentMinVal, 0.00005f ) )
      {
        auto goalNode        = mNodes.find( aEnd );
        auto selectedNode    = mNodes.find( current );
        auto replacementNode = mNodes.find( nodeIndex ); 

        assert( goalNode != mNodes.end() && selectedNode != mNodes.end() && replacementNode != mNodes.end() );
        auto goalNodePosition        = glm::vec2( goalNode       ->second->getX(), goalNode       ->second->getY() );
        auto selectedNodePosition    = glm::vec2( selectedNode   ->second->getX(), selectedNode   ->second->getY() );
        auto replacementNodePosition = glm::vec2( replacementNode->second->getX(), replacementNode->second->getY() );

        auto deltaSelected = selectedNodePosition - goalNodePosition;
        auto deltaReplacement = replacementNodePosition - goalNodePosition;

        // We prefer going in y first
        if ( std::abs( deltaReplacement.y ) < std::abs( deltaSelected.y ) )
        {
          // we prefer that. 
          current = nodeIndex;
          currentMinVal = fScore[nodeIndex];
          if ( aShouldOutputDebug )
          {
            std::cout << "  replaced new current with " << current << " with MinVal " << currentMinVal << std::endl;
          }
        }
        else
        {
          if ( aShouldOutputDebug )
          {
            std::cout << "  no replacement" << std::endl;
          }
        }
      }
      else
      {
        if ( aShouldOutputDebug )
        {
          std::cout << "  node not considered (fScore[nodeIndex]; currentMinVal) " << fScore[nodeIndex] << " " << currentMinVal << std::endl;
        }
      }
    }

    if ( current == aEnd )
      return reconstructPath(cameFrom, aEnd);

    openSet.erase( current );
    closedSet.insert( current );

    //struct SortableOutArc
    //{
    //  std::shared_ptr<Arc> mArc;
    //  glm::vec2 mDistanceToGo;
    //};

    //std::vector<SortableOutArc> sortableOutArcs;

    auto goalNode = mNodes.find( aEnd );
    assert( goalNode != mNodes.end() );

    //for ( auto outArc : mNodes[current]->getOutArcs() )
    //{
    //  auto arcNode = outArc->getNodeTo();
    //  SortableOutArc sortableOutArc;
    //  sortableOutArc.mArc = outArc;
    //  sortableOutArc.mDistanceToGo = 
    //    glm::vec2( arcNode->getX(), arcNode->getY() ) - 
    //    glm::vec2( goalNode->second->getX(), goalNode->second->getY() );

    //  sortableOutArcs.push_back( sortableOutArc );
    //}

    //std::sort( sortableOutArcs.begin(), sortableOutArcs.end(), []( const SortableOutArc& a, const SortableOutArc& b ) {
    //  if ( glm::epsilonEqual( a.mDistanceToGo.x, b.mDistanceToGo.x, 0.000005f ) )
    //  {
    //    return ( a.mDistanceToGo.y < b.mDistanceToGo.y );
    //  }
    //  if ( glm::epsilonEqual( a.mDistanceToGo.y, b.mDistanceToGo.y, 0.000005f ) )
    //  {
    //    return ( a.mDistanceToGo.x < b.mDistanceToGo.x );
    //  }
    //  return std::abs( a.mDistanceToGo.y ) > std::abs( b.mDistanceToGo.y );
    //} );

    for ( auto outArc : mNodes[current]->getOutArcs() )
    {
    //for ( auto& outArcSorted : sortableOutArcs )
    //{
      //auto outArc = outArcSorted.mArc;
      int neighbourIndex = outArc->getNodeTo()->getId();
      if ( closedSet.find(neighbourIndex) != closedSet.end() )
        continue;
      float tentativeGscore = gScore[current] + outArc->getInfluencedDistance();
      if ( openSet.find(neighbourIndex) == openSet.end() )
        openSet.insert(neighbourIndex);
      else if ( tentativeGscore >= gScore[neighbourIndex] )
        continue;

      cameFrom[neighbourIndex] = current;
      gScore[neighbourIndex] = tentativeGscore;
      //fScore[neighbourIndex] = gScore[neighbourIndex] + getHeuristicBetween( neighbourIndex, aEnd );
      float heuristicCost = getHeuristicBetween( neighbourIndex, aEnd, aStart );
      fScore[neighbourIndex] = gScore[neighbourIndex] + heuristicCost;
      if ( aShouldOutputDebug )
      {
        std::cout << "cost between " << neighbourIndex << " and " << aEnd << ": " << heuristicCost << std::endl;
      }
    }
  }
}

float 
Graph::getHeuristicBetween( int aFromNode, int aToNode ) const
{
  auto nodeStartIt = mNodes.find( aFromNode );
  auto nodeEndIt   = mNodes.find( aToNode );

  if ( nodeStartIt == mNodes.end() || nodeEndIt == mNodes.end() )
    return std::numeric_limits<float>::infinity();

  //if ( nodeEndIt->second->getCarBoundToVisit() != -1 )
  //  return std::numeric_limits<float>::infinity();

  glm::vec2 s( nodeStartIt->second->getX(), nodeStartIt->second->getY() );
  glm::vec2 e( nodeEndIt->second->getX(), nodeEndIt->second->getY() );

  float dx = std::abs( nodeEndIt->second->getX() - nodeStartIt->second->getX() );
  float dy = std::abs( nodeEndIt->second->getY() - nodeStartIt->second->getY() );

  float length = 
    (   glm::vec2( nodeStartIt->second->getX(), nodeStartIt->second->getY() )
      - glm::vec2( nodeEndIt->second->getX()  , nodeEndIt->second->getY() ) ).length();

  //if ( dx > dy )
  //{
  //  return length * 0.1;
  //}
  return length;
}

float 
Graph::getHeuristicBetween( int aFromNode, int aToNode, int aDepartureNode ) const
{
  if ( aFromNode == aToNode )
    return 0;
  auto nodeStartIt = mNodes.find( aFromNode );
  auto nodeEndIt   = mNodes.find( aToNode );
  auto nodeDepartureIt = mNodes.find( aDepartureNode );

  if ( nodeStartIt == mNodes.end() || nodeEndIt == mNodes.end() || nodeDepartureIt == mNodes.end() )
    return std::numeric_limits<float>::infinity();

  //if ( nodeEndIt->second->getCarBoundToVisit() != -1 )
  //  return std::numeric_limits<float>::infinity();

  glm::vec2 startPosition( nodeStartIt->second->getX(), nodeStartIt->second->getY() );
  glm::vec2 endPosition( nodeEndIt->second->getX(), nodeEndIt->second->getY() );
  glm::vec2 departurePosition( nodeDepartureIt->second->getX(), nodeDepartureIt->second->getY() );

  float dx = std::abs( nodeEndIt->second->getX() - nodeStartIt->second->getX() );
  float dy = std::abs( nodeEndIt->second->getY() - nodeStartIt->second->getY() );

  //float hackedLenght = std::sqrt( dx*dx*1.1 + dy * dy );
  //if ( aDepartureNode == aToNode )
    return dy + dx;
  
  //if ( dx < 0.0002f )
  //  return dy + dx;

  //float hackedLenght = dy + dx;
  //return hackedLenght;

  ////float length = glm::distance( 
  ////  glm::vec2( nodeStartIt->second->getX(), nodeStartIt->second->getY() ),
  ////  glm::vec2( nodeEndIt->second->getX(), nodeEndIt->second->getY() ) );

  ////float lenghtFromDepartureToEnd = glm::distance( endPosition, departurePosition );

  ////if ( dx > dy )
  ////{
  ////  float distanceRatio = length / lenghtFromDepartureToEnd;
  ////  return length * ( 1.0f + distanceRatio );
  ////}
  ////return length;
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
