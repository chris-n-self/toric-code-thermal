//
//  TCSpin.cpp
//  ToricCodeThermal
//
//  Created by Chris Self on 08/08/2014.
//  Copyright (c) 2014 Chris Self. All rights reserved.
//

#include "libraries.h"
#include "TCSpin.h"

TCSpin::TCSpin() {
    state = false;
    xcoord = 0;
    ycoord = 0;
    id_num = 0;
    rate_key = 0;
}

TCSpin::TCSpin( int x_, int y_, int id_num_ ) {
    state = false;
    xcoord = x_;
    ycoord = y_;
    id_num = id_num_;
    rate_key = 0;
}