/**
 * Copyright (c) 2015 Alexandre Vaillancourt. See full MIT license at the root of the repository.
 */
#pragma once

#include <string>
#include <memory>
#include <map>
#include <vector>

class Node;
class Arc;

class Graph
{
public:
  Graph() {}
  Graph( const std::string& aNodeFilePath, const std::string& aArcFilePath );

  std::map< int, std::shared_ptr<Node> >& getNodes() { return mNodes; }
  std::map< int, std::shared_ptr<Arc> >& getArcs() { return mArcs; }

  std::vector<int> plot( int aStart, int aEnd, bool aShouldOutputDebug = false);

  float getHeuristicBetween( int aFromNode, int aToNode ) const;
  float getHeuristicBetween( int aFromNode, int aToNode, int aDeparturNode ) const;

private:

  std::vector<int> reconstructPath( const std::map<int, int>& aCameFrom, int aCurrent );

  std::map< int, std::shared_ptr<Node> > mNodes;
  std::map< int, std::shared_ptr<Arc> > mArcs;


};