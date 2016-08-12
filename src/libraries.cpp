//
//  libraries.cpp
//
//  Created by Chris Self on 07/10/2014.
//  Copyright (c) 2014 Chris Self. All rights reserved.
//

#include "libraries.h"

//  function to calculate the distance between two points on a ring
int distanceOnTorus( int a_, int b_, int period_ ) {
    return (period_+(b_-a_))%period_;
}
