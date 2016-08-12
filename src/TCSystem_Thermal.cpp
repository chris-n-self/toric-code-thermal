//
//  TCSystem_Thermal.cpp
//  ToricCodeThermal
//
//  Created by Chris Self on 16/08/2014.
//  Copyright (c) 2014 Chris Self. All rights reserved.
//

#include "libraries.h"
#include "TCSystem.h"

// function to calculate nCr
//
double CombinationsIter(long iN, long iR) {
    if (iR < 0 || iR > iN) {
        return 0;
    }
    double iComb = 1.0;
    long i = 0;
    while (i < iR) {
        ++i;
        iComb *= static_cast<double>(iN - i + 1);
        iComb /= static_cast<double>(i);
    }
    return iComb;
}

// out of line definitions of thermal variables
//
double gammaFunction( double omega_ );
const double TCSystem::beta = static_cast<double>( /*InputRNum*/1 )/10.0;

/*
 ---------------
 initialisations
 ---------------
 */

void TCSystem::setThermalRates() {
    thermal_rates[0] = gammaFunction(-2.0);
    thermal_rates[1] = 1.0/TCSystem::beta;
    thermal_rates[2] = gammaFunction(2.0);
}

double gammaFunction( double omega_ ) {
    return omega_/( 1.0 - exp(-TCSystem::beta*omega_) );
}

/*
 ------------------------------
 steps of the thermal evolution
 ------------------------------
 */

void TCSystem::incrementTime() {
    
    // this check avoids a problem I had once with negative time steps
    /* double time_step = -log( rand_gen() )/total_rate;
     while ( time_step < 0 ) {
     time_step = -log( rand_gen() )/total_rate;
     }
     simulation_time += time_step; */
    
    simulation_time += (-log( rand_gen() )/total_rate);
}

void TCSystem::thermalSpinFlip() {
    /*
     then we iterate over the spins keeping a cumulative tally of the rate, when the next spin would push
     the tally over the random # we flip that spin
     */
    double rand_rate_target = total_rate*rand_gen();
    double ratetally = 0.0;
    for ( auto& spin_row : spins ) {
        for ( auto& spin_plaq : spin_row ) {
            for ( auto& spin : spin_plaq ) {
                ratetally += thermal_rates[ spin.second.getRateKey() ];
                if ( ratetally > rand_rate_target ) {
                    spin.second.flipState();
                    // std::cout << "Flipped spin " << spin.second.x() << ", " << spin.second.y() << ", " << spin.second.idNum() << " to " << spin.second.getState() << '\n';
                    
                    // update syndromes changed by this flip
                    int x = spin.second.x();
                    int y = spin.second.y();
                    if ( spin.second.idNum() == 1 ) {
                        // spin_plaq.first -> y & y+1
                        handleStabiliserChange(x,y);
                        handleStabiliserChange(x,(y+1)%sys_size);
                    } else {
                        // spin_plaq.second -> x & x-1
                        handleStabiliserChange(x,y);
                        handleStabiliserChange((x+sys_size-1)%sys_size,y);
                    }
                    
                    // drawAnimationFrame(dout,spin.second.x(),spin.second.y(),spin.second.idNum());
                    
                    // update thermal rates
                    updateLocalRates(spin.second.x(),spin.second.y(),spin.second.idNum());
                    total_rate = 0.0;
                    for ( auto rates_class : rates_class_counts ) {
                        total_rate += rates_class.second*thermal_rates[rates_class.first];
                    }
                    
                    return;
                }
            }
        }
    }
}

void TCSystem::handleStabiliserChange( int x_, int y_ ) {
    stabilisers[x_][y_].flipStabiliser();
    if( stabilisers[x_][y_].getOccupation() ) {
        anyon_positions.insert( std::make_pair(x_,y_) );
    } else {
        anyon_positions.erase( std::make_pair(x_,y_) );
    }
}

void TCSystem::updateLocalRates( int x_, int y_, int id_ ) {
    
    /*
     update rates of spins in (x,y) plaquette
     */
    updateSpinRate( x_, y_, 1 ); // <--- (x,y,#1)
    updateSpinRate( x_, y_, 2 ); // <--- (x,y,#2)
    updateSpinRate( x_, (y_+(sys_size-1))%sys_size, 1 ); // <--- (x,y-1,#1)
    updateSpinRate( (x_+1)%sys_size, y_, 2 ); // <--- (x+1,y,#2)
    
    if ( id_ == 1 ) {
        /*
         update rates of spins in (x,y+1) plaquette
         */
        updateSpinRate( x_, (y_+1)%sys_size, 1 ); // <--- (x,y+1,#1)
        updateSpinRate( x_, (y_+1)%sys_size, 2 ); // <--- (x,y+1,#2)
        updateSpinRate( (x_+1)%sys_size, (y_+1)%sys_size, 2 ); // <--- (x+1,y+1,#2)
        
        return;
    } else {
        /*
         update rates of spins in (x-1,y) plaquette
         */
        updateSpinRate( (x_+(sys_size-1))%sys_size, y_, 1 ); // <--- (x-1,y,#1)
        updateSpinRate( (x_+(sys_size-1))%sys_size, y_, 2 ); // <--- (x-1,y,#2)
        updateSpinRate( (x_+(sys_size-1))%sys_size, (y_+(sys_size-1))%sys_size, 1 ); // <--- (x-1,y-1,#1)
        
        return;
    }
}

void TCSystem::updateSpinRate( int x_, int y_, int id_ ) {
    
    rates_class_counts[ spins[x_][y_][id_].getRateKey() ]--;
    
    if( id_==1 ) {
        spins[x_][y_][id_].setRateKey( static_cast<int>(stabilisers[x_][y_].getOccupation())+static_cast<int>(stabilisers[x_][(y_+1)%sys_size].getOccupation()) );
    } else {
        spins[x_][y_][id_].setRateKey( static_cast<int>(stabilisers[x_][y_].getOccupation())+static_cast<int>(stabilisers[(x_+(sys_size-1))%sys_size][y_].getOccupation()) );
    }
    // std::cout << "spin rate " << x_ << ", " << y_ << ", " << id_ << " to " << spins[x_][y_][1].getRateKey() << '\n';
    
    rates_class_counts[ spins[x_][y_][id_].getRateKey() ]++;
}

/*
 ---------------------------
 thermal evolution algorithm
 ---------------------------
 */

void TCSystem::thermalEvolution( double end_time_ ) {
    incrementTime();
    while( simulation_time < end_time_ ) {
        thermalSpinFlip();
        incrementTime();
    }
}

/*
 -----------------------
 thermal noise functions
 -----------------------
 */

double TCSystem::calcThermalSuccessRate( double end_time_, long int num_trials_ ) {
    long int success_count = 0;
    
    for ( long int i=0; i<num_trials_; i++ ) {
        resetSystem();
        thermalEvolution(end_time_);
        if ( runDecoder() ) { success_count++; }
    }
    
    return (static_cast<double>(success_count))/(static_cast<double>(num_trials_));
}

double TCSystem::calcThermalSuccessRateFailFast99( double end_time_, long num_trials_ ) {
    long success_count = 0;
    
    for ( long block=0; block<10; block++ ) {
        for ( long i=0; i<(num_trials_/10); i++ ) {
            resetSystem();
            thermalEvolution(end_time_);
            if ( runDecoder() ) { success_count++; }
        }
        
        // HYPOTHESIS TEST
        // calculate probability of observed at least this number of fails assuming p=0.9
        // if this prob < 0.05 fail fast.
        //
        long number_so_far = (block+1)*(num_trials_/10);
        double lower_hyp_test_prob = 0.0;
        for ( int j=0; j<(success_count+1); ++j ) {
            lower_hyp_test_prob += static_cast<double>(CombinationsIter( number_so_far, j ))*pow(0.9,static_cast<double>(j))*pow(0.1,static_cast<double>(number_so_far-j));
        }
        if (lower_hyp_test_prob<0.05) {
            std::cout << "@ " << end_time_ << " and N=" << number_so_far << " could say with confidence " << (1-lower_hyp_test_prob) << " that p<0.9\n";
            return 0.9;
        }
    }
    
    double success_rate = (static_cast<double>(success_count))/(static_cast<double>(num_trials_));
    std::cout << "@ " << end_time_ << '\t' << success_rate << '\n';
    return success_rate;
}

double TCSystem::calcThermalSuccessRateFailFast90( double end_time_, long num_trials_ ) {
    long success_count = 0;
    
    for ( long block=0; block<10; block++ ) {
        for ( long i=0; i<(num_trials_/10); i++ ) {
            resetSystem();
            thermalEvolution(end_time_);
            if ( runDecoder() ) { success_count++; }
        }
        
        // HYPOTHESIS TEST
        // LOWER: calculate probability of observed no more than this number of successes assuming p=0.85 (H0: p>0.85)
        // if this prob < 0.05 fail fast.
        // UPPER: calculate probability of observed more than this number of successes assuming p=0.95 (H0: p<=0.95)
        // if this prob < 0.05 fail fast.
        //
        long number_so_far = (block+1)*(num_trials_/10);
        double lower_hyp_test_prob = 0.0;
        double upper_hyp_test_prob = 1.0;
        for ( int j=0; j<(success_count+1); ++j ) {
            lower_hyp_test_prob += static_cast<double>(CombinationsIter( number_so_far, j ))*pow(0.85,static_cast<double>(j))*pow(0.15,static_cast<double>(number_so_far-j));
            upper_hyp_test_prob -= static_cast<double>(CombinationsIter( number_so_far, j ))*pow(0.95,static_cast<double>(j))*pow(0.05,static_cast<double>(number_so_far-j));
        }
        // add back on the j = success_count case to the upper hypothesis tests
        upper_hyp_test_prob += static_cast<double>(CombinationsIter( number_so_far, success_count ))*pow(0.95,static_cast<double>(success_count))*pow(0.05,static_cast<double>(number_so_far-success_count));

        // check if either hypothesis test has been failed
        //
        if (lower_hyp_test_prob<0.01) {
            std::cout << "@ " << end_time_ << " and N=" << number_so_far << " could say with confidence " << (1-lower_hyp_test_prob) << " that p<0.85\n";
            return 0.85;
        } else if (upper_hyp_test_prob<0.01) {
            std::cout << "@ " << end_time_ << " and N=" << number_so_far << " could say with confidence " << (1-upper_hyp_test_prob) << " that p>0.95\n";
            return 0.95;
        }
    }
    
    double success_rate = (static_cast<double>(success_count))/(static_cast<double>(num_trials_));
    std::cout << "@ " << end_time_ << '\t' << success_rate << '\n';
    return success_rate;
}

std::pair<double,double> TCSystem::find99PercentBracketingTimes( long int number_trials_ ) {
    double end_time = 0.0;
    // std::cout.precision(10);
    // std::cout.setf( std::ios::fixed, std:: ios::floatfield );
    
    /*
     test the initial time success rate to see if the search needs to begin by decreasing or increasing the end time
     */
    double success_rate;
    success_rate = calcThermalSuccessRateFailFast99( end_time, number_trials_/10 );
    
    double interval_start, interval_end;
    /*
     search to find the scale that the coherence time is on
     */
    int scale = -3;
    for ( int i = -3; i<30; ++i ) {
        scale = i;
        end_time = 1.0*pow( 10.0, scale*1.0 );
        success_rate = calcThermalSuccessRateFailFast99( end_time, number_trials_/10 );
        
        /*
         stop condition is the search has the scale of the coherence time
         */
        if ( success_rate<(1.0-(1.0/(number_trials_/10))) ) {
            break;
        }
    }
    
    /*
     if the scale search has overshot the coherence time we need to search backward first
     */
    double search_multiplier = 5.0;
    if ( success_rate<0.994 ) {
        scale--;
    }
    while ( success_rate<0.994 ) {
        end_time -= search_multiplier*1.0*pow( 10.0, scale*1.0 );
        success_rate = calcThermalSuccessRateFailFast99( end_time, number_trials_/10 );
        search_multiplier = search_multiplier/2.0;
    }
    
    /*
     find the 99.4-99.6% lower bound of the interval
     */
    interval_start = end_time;
    search_multiplier = 1.0;
    while ( (success_rate>0.996) || (success_rate<0.994) ) /* while outside the  */ {
        end_time += search_multiplier*1.0*pow( 10.0, scale*1.0 );
        success_rate = calcThermalSuccessRateFailFast99( end_time, number_trials_/10 );
        if ( !(success_rate<0.994) ) /* if greater than or equal to the lower bound */ {
            interval_start = end_time;
            search_multiplier = search_multiplier*2.0;
        } else {
            end_time -= search_multiplier*1.0*pow( 10.0, scale*1.0 );
            search_multiplier = search_multiplier/2.0;
        }
    }
    std::cout << "interval_start set @ " << end_time << '\n';
    
    /*
     find the 98.4-98.6% upper bound of the interval
     */
    interval_end = end_time;
    if ( search_multiplier < 1.0 ) {
        search_multiplier = 1.0;
    }
    while ( (success_rate>0.986) || (success_rate<0.984) ) {
        end_time += search_multiplier*1.0*pow( 10.0, scale*1.0 );
        success_rate = calcThermalSuccessRateFailFast99( end_time, number_trials_/10 );
        if ( !(success_rate<0.984) ) /* if greater than or equal to the lower bound */ {
            interval_end = end_time;
            search_multiplier = search_multiplier*2.0;
        } else {
            end_time -= search_multiplier*1.0*pow( 10.0, scale*1.0 );
            search_multiplier = search_multiplier/2.0;
        }
    }
    std::cout << "interval_end set @ " << end_time << '\n';
    
    std::cout << "return interval " << interval_start << '\t'  << interval_end << '\n' << '\n';
    return std::make_pair( interval_start, interval_end );
}

double TCSystem::find90PercentSuccessTime( long int number_trials_ ) {
    double end_time = 0.0;
    // std::cout.precision(10);
    // std::cout.setf( std::ios::fixed, std:: ios::floatfield );
    
    /*
     test the initial time success rate to see if the search needs to begin by decreasing or increasing the end time
     */
    double success_rate;
    success_rate = calcThermalSuccessRateFailFast90( end_time, number_trials_/10 );
    
    double target_time;
    /*
     search to find the scale that the coherence time is on
     */
    int scale = -3;
    for ( int i = -3; i<30; ++i ) {
        scale = i;
        end_time = 1.0*pow( 10.0, scale*1.0 );
        success_rate = calcThermalSuccessRateFailFast90( end_time, number_trials_/10 );
        
        /*
         stop condition is the search has the scale of the coherence time
         */
        if ( success_rate<0.94 ) {
            break;
        }
    }
    
    /*
     if the scale search has overshot the coherence time we need to search backward first
     */
    double search_multiplier = 5.0;
    if ( success_rate<0.89 ) {
        scale--;
    }
    while ( success_rate<0.89 ) {
        end_time -= search_multiplier*1.0*pow( 10.0, scale*1.0 );
        success_rate = calcThermalSuccessRateFailFast90( end_time, number_trials_/10 );
        search_multiplier = search_multiplier/2.0;
    }
    
    /*
     find the 89-91% bound
     */
    target_time = end_time;
    search_multiplier = 1.0;
    while ( (success_rate>0.91) || (success_rate<0.89) ) /* while outside the  */ {
        end_time += search_multiplier*1.0*pow( 10.0, scale*1.0 );
        success_rate = calcThermalSuccessRateFailFast90( end_time, number_trials_/10 );
        if ( !(success_rate<0.89) ) /* if greater than or equal to the lower bound */ {
            target_time = end_time;
            search_multiplier = search_multiplier*2.0;
        } else {
            end_time -= search_multiplier*1.0*pow( 10.0, scale*1.0 );
            search_multiplier = search_multiplier/2.0;
        }
    }
    std::cout << "found estimate of 90% point @ " << target_time << '\n';
    
    return target_time;
}

