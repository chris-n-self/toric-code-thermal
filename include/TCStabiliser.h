//
//  TCStabiliser.h
//  ToricCodeThermal
//
//  Created by Chris Self on 07/08/2014.
//  Copyright (c) 2014 Chris Self. All rights reserved.
//

#ifndef ToricCodeThermal_TCStabiliser_h
#define ToricCodeThermal_TCStabiliser_h

class TCStabiliser {
private:
    bool occupied;
    int xcoord;
    int ycoord;
    
    // decoder information
    int box_number;
    
public:
    TCStabiliser();
    TCStabiliser( int x_, int y_ );
    int x() { return xcoord; }
    int y() { return ycoord; }
    bool getOccupation() { return occupied; }
    void setOccupation( bool state_ ) { occupied = state_; }
    void flipStabiliser() { occupied = !occupied; }
    
    // decoder methods
    int getBoxNumber() { return box_number; }
    void setBoxNumber( int new_box_number_ ) { box_number = new_box_number_; }
    
};

#endif
