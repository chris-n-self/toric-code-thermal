//
//  TCSystem.h
//  ToricCodeThermal
//
//  Created by Chris Self on 07/08/2014.
//  Copyright (c) 2014 Chris Self. All rights reserved.
//

#include "TCSpin.h"
#include "TCStabiliser.h"
#include "TCDecoderBox.h"

#ifndef ToricCodeThermal_TCSystem_h
#define ToricCodeThermal_TCSystem_h

class TCSystem {
    
private:
    /*
     member variables
     */
    // spins, plaquettes and anyons
    int sys_size;
    std::vector<std::vector<TCStabiliser>> stabilisers;
    std::vector<std::vector<std::map<int,TCSpin>>> spins;
    std::set<std::pair<int,int>> anyon_positions;
    
    // logical state
    std::pair<bool,bool> logical_state;
    
    // decoder resources
    std::map<int,TCDecoderBox> decoder_boxes;
    int number_charged_boxes;
    std::map<int,std::list<std::pair<int,int>>> decoder_lookup_table;
    
    // noise
    //
    // random number generator
    static MTRand_closed rand_gen;
    //
    // thermal
    double simulation_time;
    double total_rate;
    std::map<int,int> rates_class_counts;
    
    /*
     functions
     */
    // lattice geometry
    std::list<std::pair<int,int>> getNeighbours( int from_x_, int from_y_ );
    std::vector<std::pair<int,int>> getLeveliNeighbours( int level_i_, int from_x_, int from_y_ );
    
    // internal decoder functions
    std::pair<bool,bool> guessLogicalStateFromDecoder();
    bool testDecoderSuccess();
    bool boxClusters();
    bool handleBoxCombinations( int box_id_, int other_box_id_, std::set<std::pair<int,int>>& temp_particle_pos_ );
    
    // interal thermal noise functions
    void setThermalRates();
    static std::map<int,double> createRatesMap();
    void incrementTime();
    void thermalSpinFlip();
    void handleStabiliserChange( int x_, int y_ );
    void updateLocalRates( int x_, int y_, int id_ );
    void updateSpinRate( int x_, int y_, int id_ );
    
public:
    /*
     member variables
     */
    // thermal resources
    static const double beta;
    std::map<int,double> thermal_rates;
    
    /*
     functions
     */
    TCSystem( int L_ );
    int L() { return sys_size; }
    void buildDecoderLookupTable();
    
    // system processes
    void evaluateSyndromes();
    void findAnyons();
    void measureLogicalState();
    
    bool runDecoder();
    
    // noise
    //
    // generic
    void resetSystem();
    void flipSpin( int at_x_, int at_y_, int at_id_ );
    //
    // iid
    void sufferIIDNoise( double error_rate_ );
    double calcIIDSuccessRate( double error_rate_, long num_trials_ );
    //
    // thermal
    void thermalEvolution( double end_time_ );
    //
    // FailFast99 uses Binomial hypothesis testing every 1/10*num_trials to reject end times whose success
    // rate < 0.9 with 95% confidence level
    // FailFast90 also uses Binomial hypothesis testing every 1/10*num_trials to reject end times whose success
    // are rate < 0.85 with 95% confidence level or rate > 0.95 with 95% confidence level
    double calcThermalSuccessRate( double end_time_, long int num_trials_ );
    double calcThermalSuccessRateFailFast99( double end_time_, long int num_trials_ );
    double calcThermalSuccessRateFailFast90( double end_time_, long int num_trials_ );
    //
    // RETURNS a time in the success rate interval [0.992,0.994] and a time in the interval [0.986,0.988]
    std::pair<double,double> find99PercentBracketingTimes( long int num_trials_ );
    //
    // RETURNS a time in the success rate interal [0.901,0.899]
    double find90PercentSuccessTime( long int num_trials_ );
    
    // drawing
    std::pair<int,int> convertToDrawCoord( std::pair<int,int> coord_ );
    void drawAll( std::ofstream& dout );
    void drawEmptyLattice( std::ofstream& dout );
    void drawQubits( std::ofstream& dout );
    void drawQubitsRates( std::ofstream& dout );
    void drawAnyons( std::ofstream& dout );
    void drawErrors( std::ofstream& dout );
    void drawBoxes( std::ofstream& dout );
    void drawAnimationHeader( std::ofstream& dout );
    void drawAnimationFrame( std::ofstream& dout, int flipped_x_, int flipped_y_, int flipped_id_ );
    void drawAnimationFooter( std::ofstream& dout );
    
};

#endif
