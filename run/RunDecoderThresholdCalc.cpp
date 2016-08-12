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
    int L = 10; // InputSysSize;
	long num_trials = static_cast<long>(1E4);
    double run_number = 0; // InputRNum;
    double rate_step_size = 0.00001;
    //
    double error_rate = rate_step_size*run_number;
    
	// initialise file output junk
	ofstream outputf;
	std::string s; std::stringstream outstring;
    outstring.str("");
    outstring.clear();
    outstring << "dec_thresh_" << L << "_" << run_number << ".txt";
    s=outstring.str();
    const char * c = s.c_str();
    outputf.open (c, std::ofstream::out | std::ofstream::app);
    
    TCSystem system = (L);
    double success_rate = system.calcIIDSuccessRate(error_rate,num_trials);
    
    cout << error_rate << '\t' << success_rate << '\n';
    outputf << error_rate << '\t' << success_rate << '\n';
    
	return 0;
}
