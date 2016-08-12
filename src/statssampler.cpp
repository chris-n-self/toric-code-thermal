//
//  statssampler.cpp
//
//  Created by Chris Self on 12/05/2014.
//  Copyright (c) 2014 Chris Self. All rights reserved.
//

#include "libraries.h"
#include "statssampler.h"

statssampler::statssampler() {
    n = 0;
    mean = 0.0;
    M2 = 0.0;
    max = 0;
}

/*
 this is an implementation of the (online) incremental algorithm due to Knuth that makes
 calculation of the variance robust to loss of precision from numerical instability
 */
void statssampler::addNewData( double x_ ) {
    n++;
    double delta = x_ - mean;
    //
    mean = mean + delta/n;
    M2 = M2 + delta*(x_ - mean);
    //
    if ( x_ > max ) max = x_;
}

void statssampler::reset() {
    n = 0;
    mean = 0.0;
    M2 = 0.0;
    max = 0;
}