//
//  TCDecoderBox.h
//  ToricCodeThermal
//
//  Created by Chris Self on 07/08/2014.
//  Copyright (c) 2014 Chris Self. All rights reserved.
//

#ifndef ToricCodeThermal_TCDecoderBox_h
#define ToricCodeThermal_TCDecoderBox_h

class TCDecoderBox {
private:
    //  total charge of all the particles in the box
    bool contains_odd_parity;
    
    //  the dimensions of the box -- box is fully rectangular
    std::pair<int,int> upper_right;
    std::pair<int,int> lower_left;
    
    //  list of the particles in the box
    std::set<std::pair<int,int>> contained_particles;
    
    //  flags used for garbage collection by decoder
    bool has_been_absorbed;
    bool has_been_neutralised;
    
    //  flags for flux affecting the log ops
    bool might_affect_hori;
    bool might_affect_vert;
    
public:
    // constructor
    TCDecoderBox( int at_x_, int at_y_ );
    
    // access methods
    bool hasOddParity() { return contains_odd_parity; };
    std::pair<int,int> getUpperRight() { return upper_right; }
    std::pair<int,int> getLowerLeft() { return lower_left; }
    std::set<std::pair<int,int>>* getContainedParticles() { return &contained_particles; }
    bool isRedundant() { return has_been_absorbed; }
    bool isNeutral() { return has_been_neutralised; }
    bool mightAffectHori() { return might_affect_hori; }
    bool mightAffectVert() { return might_affect_vert; }
    
    bool isNewlyNeutral(); //  ask if this box became neutral at this RG level
    bool combineBoxes( TCDecoderBox& other_box_, int repeat_x_, int repeat_y_ ); //  absorb another box into this box
    void makeRedundant(); //  signal this box has been absorbed by another
    bool coordInsideBox( std::pair<int,int> coord ); //  ask if a given coord is inside the box
    void addNewParticlePositions( std::set<std::pair<int,int>>& newParticles ); //  add a list of particles to this box
    
};

#endif
