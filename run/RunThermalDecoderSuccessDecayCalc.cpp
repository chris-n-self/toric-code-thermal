//
//  RunDecoderThresholdCalc.cpp
//  ToricCodeThermal
//
//  Created by Chris Self on 07/08/2014.
//  Copyright (c) 2014 Chris Self. All rights reserved.
//

#include "libraries.h"
#include "TCSystem.h"

using namespace std;

int main(void) {
    
    // test parameters
    int L = 24;//InputSysSize;
	long num_trials = static_cast<long>(1E4);
    double run_number = 1;//InputRNum;
    
	// initialise file output junk
	ofstream outputf;
	std::string s; std::stringstream outstring;
    outstring.str("");
    outstring.clear();
    outstring << "TCThermal_" << L << "_" << run_number << ".txt";
    s=outstring.str();
    const char * c = s.c_str();
    outputf.open (c, std::ofstream::out | std::ofstream::trunc);
    
    TCSystem system = (L);
    
    // find data range:  run search algorithm to find an estimate of the interval containing the coherence time
	double decoder_90Percent_time = system.find90PercentSuccessTime( num_trials );
    
	// take data:  plot 10 points across the range around the coherence time
	double end_time;
    double success_rate;
	for ( int i=0; i<100; i++ ) {
		end_time  = i*1.0*(decoder_90Percent_time)/19.0;
        success_rate =  system.calcThermalSuccessRate(end_time,num_trials);
		cout << TCSystem::beta << '\t' << end_time << '\t' << success_rate << '\n';
		outputf << TCSystem::beta << '\t' << end_time << '\t' << success_rate << '\n';
	}
    
	return 0;
}
