/**
 * Copyright (c) 2015 Alexandre Vaillancourt. See full MIT license at the root of the repository.
 */
#include "Arc.h"

#include "Node.h"

Arc::Arc( int aId, std::shared_ptr<Node> aNodeFrom, std::shared_ptr<Node> aNodeTo )
  : mId( aId )
  , mNodeFrom( aNodeFrom )
  , mNodeTo( aNodeTo )
  , mTravelCost( 0.0f )
  , mDistance( 1.0f )
{
  mDistance = std::sqrt( 
    std::powf( aNodeTo->getX() - aNodeFrom->getX(), 2 ) + 
    std::powf( aNodeTo->getY() - aNodeFrom->getY(), 2 ) );
}
