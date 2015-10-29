/**
 * Copyright (c) 2015 Alexandre Vaillancourt. See full MIT license at the root of the repository.
 */
#pragma once

#include <string>
#include <memory>
#include <map>

class Node;
class Arc;

class Graph
{
public:
  Graph() {}
  Graph( const std::string& aNodeFilePath, const std::string& aArcFilePath );

  std::map< int, std::shared_ptr<Node> >& getNodes() { return mNodes; }
  std::map< int, std::shared_ptr<Arc> >& getArcs() { return mArcs; }

private:

  std::map< int, std::shared_ptr<Node> > mNodes;
  std::map< int, std::shared_ptr<Arc> > mArcs;


};