//
//  RunSimulation.cpp
//  ToricCodeThermal
//
//  Created by Chris Self on 07/08/2014.
//  Copyright (c) 2014 Chris Self. All rights reserved.
//

#include "libraries.h"
#include "TCSystem.h"

using namespace std;

int main(int argc, const char * argv[])
{
    // data output streams
	ofstream drawing;
	std::string s; std::stringstream outstring;
    // test drawing
	outstring.str("");
	outstring.clear();
	outstring << "test_drawing3.asy";
	s=outstring.str();
	const char * c1 = s.c_str();
	drawing.open (c1, std::ofstream::out | std::ofstream::trunc);
    
    TCSystem system(8);
    
    system.sufferIIDNoise(0.06);
    // system.flipSpin(0,19,1);
    // system.flipSpin(0,19,2);
    // system.evaluateSyndromes();
    system.runDecoder();
    system.drawAll(drawing);
    
    // double succ_rate = system.calcIIDSuccessRate(mt_rand_generator, 0.05, 1000);
    // std::cout << succ_rate ;
    
    // system.drawAnimationHeader(drawing);
    // system.thermalEvolution(200);
    // system.drawAnimationFooter(drawing);
    
    return 0;
}