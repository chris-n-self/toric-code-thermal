//
//  TCStabiliser.cpp
//  ToricCodeThermal
//
//  Created by Chris Self on 08/08/2014.
//  Copyright (c) 2014 Chris Self. All rights reserved.
//

#include "TCStabiliser.h"

TCStabiliser::TCStabiliser() {
    occupied = false;
    xcoord = 0;
    ycoord = 0;
    box_number = 0;
}

TCStabiliser::TCStabiliser( int x_, int y_ ) {
    occupied = false;
    xcoord = x_;
    ycoord = y_;
    box_number = 0;
}