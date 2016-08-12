//
//  TCSpin.h
//  ToricCodeThermal
//
//  Created by Chris Self on 07/08/2014.
//  Copyright (c) 2014 Chris Self. All rights reserved.
//

#ifndef ToricCodeThermal_TCSpin_h
#define ToricCodeThermal_TCSpin_h

class TCSpin {
private:
    bool state;
    
    // coordinates
    int xcoord;
    int ycoord;
    int id_num;
    
    // thermal property
    int rate_key;
    
public:
    TCSpin();
    TCSpin( int x_, int y_, int id_num_ );
    bool getState() { return state; }
    void setState( bool new_state_ ) { state = new_state_; }
    void flipState() { state = !state; }
    int x() { return xcoord; }
    int y() { return ycoord; }
    int idNum() { return id_num; }
    
    // thermal functions
    int getRateKey() { return rate_key; }
    void setRateKey( int new_key_ ) { rate_key = new_key_; }
    
};

#endif
