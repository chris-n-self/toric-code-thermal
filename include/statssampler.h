//
//  statssampler.h
//
//  Created by Chris Self on 12/05/2014.
//  Copyright (c) 2014 Chris Self. All rights reserved.
//

#ifndef ColorCodeDynamicsSimulation_StatsSampler_h
#define ColorCodeDynamicsSimulation_StatsSampler_h

class statssampler
{
private:
    long int n;
    double mean;
    double M2;
    double max;
    
public:
    statssampler();
    
    void addNewData( double x_ );
    
    long getN() { return n; }
    double getMean() { return mean; }
    double getVariance() { return (n<2) ? 0 : M2/(n-1); }
    double getMax() { return max; }
    
    void reset();
};

#endif
