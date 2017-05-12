/**
 * Copyright (c) 2015 Alexandre Vaillancourt. See full MIT license at the root of the repository.
 */
#pragma once

#include <memory>

class Node;

class Arc
{
public:
  Arc( int aId, std::shared_ptr<Node> aNodeFrom, std::shared_ptr<Node> aNodeTo );

  int getId() const { return mId; }
  float getTravelCost() const { return mTravelCost; }
  void  setTravelCost( float mValue ) { mTravelCost = mValue; }
  Node* getNodeFrom() { return mNodeFrom.get(); }
  Node* getNodeTo() { return mNodeTo.get(); }

  float getDistance() const { return mDistance; }
  float getInfluencedDistance() const { return mInfluencedDistance; }

private:
  int mId;
  float mTravelCost;
  float mDistance;
  float mInfluencedDistance;
  std::shared_ptr<Node> mNodeFrom;
  std::shared_ptr<Node> mNodeTo;
};
