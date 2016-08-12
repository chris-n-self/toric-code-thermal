//
//  TCSystem_Drawing.cpp
//  ToricCodeThermal
//
//  Created by Chris Self on 11/08/2014.
//  Copyright (c) 2014 Chris Self. All rights reserved.
//

#include "libraries.h"
#include "TCSystem.h"

std::pair<int,int> TCSystem::convertToDrawCoord( std::pair<int,int> coord_ ) {
    return std::make_pair( 1+coord_.first-(sys_size/2), coord_.second-(sys_size/2) );
}

void TCSystem::drawAll( std::ofstream &dout ) {
    drawEmptyLattice(dout);
    drawQubits(dout);
    drawAnyons(dout);
    drawErrors(dout);
    drawBoxes(dout);
}

void TCSystem::drawEmptyLattice( std::ofstream &dout ) {
    dout << std::boolalpha << "import ToricCodeHeader;\n" << '\n'
    << "setL( " << sys_size << " );" << '\n'
    << "drawAxes( " << -sys_size/2 << ", " << 1+sys_size/2 << ", " << -sys_size/2 << ", " << 1+sys_size/2 << " );"  << '\n';
    for ( auto stabil_row : stabilisers ) {
        for ( auto stabil : stabil_row ) {
            auto shifted_coord = convertToDrawCoord(std::make_pair(stabil.x(),stabil.y()));
            dout << std::boolalpha << "labelCoord( " << shifted_coord.first << ", " << shifted_coord.second << ", " << "\"" << shifted_coord.first << ", " << shifted_coord.second << "\"" << " );\n";
        }
    }
}

void TCSystem::drawQubits( std::ofstream &dout ) {
    dout << std::boolalpha << "loadQubits( " << -sys_size/2 << ", " << 1+sys_size/2 << ", " << -sys_size/2 << ", " << 1+sys_size/2 << " );" << '\n';
}

void TCSystem::drawAnyons( std::ofstream &dout ) {
    for ( auto anyon : anyon_positions ) {
        auto shifted_anyon = convertToDrawCoord(anyon);
        dout << std::boolalpha << "placeAnyon( " << shifted_anyon.first << ", " << shifted_anyon.second << " );";
    }
    dout << '\n';
}

void TCSystem::drawErrors( std::ofstream &dout ) {
    for ( auto spin_row : spins ) {
        for ( auto spin_plaq : spin_row ) {
            for ( auto spin : spin_plaq ) {
                if ( spin.second.getState() ) {
                    auto shifted_spin = convertToDrawCoord(std::make_pair(spin.second.x(),spin.second.y()));
                    if ( (spin.second.y()==(sys_size-1)) && (spin.second.idNum()==1) ) {
                        shifted_spin = std::make_pair(shifted_spin.first, shifted_spin.second-sys_size);
                    }
                    dout << std::boolalpha << "placeError( " << shifted_spin.first << ", " << shifted_spin.second << ", " << spin.second.idNum() << " );\n";
                }
            }
        }
    }
    dout << '\n';
}

void TCSystem::drawQubitsRates( std::ofstream &dout ) {
    for ( auto spin_row : spins ) {
        for ( auto spin_plaq : spin_row ) {
            for ( auto spin : spin_plaq ) {
                if ( spin.second.getState() ) {
                    auto shifted_spin = convertToDrawCoord(std::make_pair(spin.second.x(),spin.second.y()));
                    if ( (spin.second.y()==(sys_size-1)) && (spin.second.idNum()==1) ) {
                        shifted_spin = std::make_pair(shifted_spin.first, shifted_spin.second-sys_size);
                    }
                    dout << std::boolalpha << "labelQubitRate( " << shifted_spin.first << ", " << shifted_spin.second << ", " << spin.second.idNum() << ", \"" << spin.second.getRateKey() << "\"" << " );\n";
                }
            }
        }
    }
    dout << '\n';
}

void TCSystem::drawBoxes( std::ofstream &dout ) {
    for ( auto box : decoder_boxes ) {
        auto shifted_lower_left = convertToDrawCoord(box.second.getLowerLeft());
        auto shifted_upper_right = convertToDrawCoord(box.second.getUpperRight());
        dout << std::boolalpha << "drawBox( (" << shifted_lower_left.first << "," << shifted_lower_left.second << "), (" << shifted_upper_right.first << "," << shifted_upper_right.second << ") );\n";
        for ( auto particle : *box.second.getContainedParticles() ) {
            auto shifted_particle = convertToDrawCoord(particle);
            dout << std::boolalpha << "labelAnyonBox( " << shifted_particle.first << ", " << shifted_particle.second << ", " << "\"" << stabilisers[particle.first][particle.second].getBoxNumber() << "\"" << " );\n";
        }
    }
    dout << '\n';
}

void TCSystem::drawAnimationHeader( std::ofstream& dout ) {
    dout << "usepackage(\"animate\");\n" << "settings.tex=\"pdflatex\";\n";
    dout << "import animation;\n";
    dout << "animation a;\n" << '\n';
    
    drawEmptyLattice(dout);
    drawQubits(dout);
    
    dout << "a.add();\n" << '\n';
}

//void TCSystem::drawAnimationFrame( std::ofstream& dout, int flipped_x_, int flipped_y_, int flipped_id_ ) {
//    dout << "save();\n";
//    drawAnyons(dout);
//
//    // draw the spin that changed state
//    auto shifted_spin = convertToDrawCoord(std::make_pair(flipped_x_,flipped_y_));
//    if ( (flipped_y_==(sys_size-1)) && (flipped_id_==1) ) {
//        shifted_spin = std::make_pair(shifted_spin.first, shifted_spin.second-sys_size);
//    }
//    if ( spins[flipped_x_][flipped_y_][flipped_id_].getState() ) {
//        dout << std::boolalpha << "placeError( " << shifted_spin.first << ", " << shifted_spin.second << ", " << flipped_id_ << " );\n";
//    } else {
//        dout << std::boolalpha << "placeQubit( " << shifted_spin.first << ", " << shifted_spin.second << ", " << flipped_id_ << " );\n";
//    }
//
//    dout << "a.add();\n";
//    dout << "restore();\n" << '\n';
//
//    // add changed spin to base image
//    if ( spins[flipped_x_][flipped_y_][flipped_id_].getState() ) {
//        dout << std::boolalpha << "placeError( " << shifted_spin.first << ", " << shifted_spin.second << ", " << flipped_id_ << " );\n";
//    } else {
//        dout << std::boolalpha << "placeQubit( " << shifted_spin.first << ", " << shifted_spin.second << ", " << flipped_id_ << " );\n";
//    }
//}

void TCSystem::drawAnimationFrame( std::ofstream& dout, int flipped_x_, int flipped_y_, int flipped_id_ ) {
    
    // draw the spin that changed state
    auto shifted_spin = convertToDrawCoord(std::make_pair(flipped_x_,flipped_y_));
    if ( (flipped_y_==(sys_size-1)) && (flipped_id_==1) ) {
        shifted_spin = std::make_pair(shifted_spin.first, shifted_spin.second-sys_size);
    }
    if ( spins[flipped_x_][flipped_y_][flipped_id_].getState() ) {
        dout << std::boolalpha << "placeError( " << shifted_spin.first << ", " << shifted_spin.second << ", " << flipped_id_ << " );\n";
    } else {
        dout << std::boolalpha << "placeQubit( " << shifted_spin.first << ", " << shifted_spin.second << ", " << flipped_id_ << " );\n";
    }
    
    // draw the changed anyons
    std::pair<int,int> anyon_1;
    std::pair<int,int> anyon_2;
    if( flipped_id_==1 ) {
        anyon_1 = std::make_pair(flipped_x_,flipped_y_); // stabiliser (x,y)
        anyon_2 = std::make_pair(flipped_x_,(flipped_y_+1)%sys_size); // stabiliser (x,y+1)
    } else {
        anyon_1 = std::make_pair(flipped_x_,flipped_y_); // stabiliser (x,y)
        anyon_2 = std::make_pair((flipped_x_+(sys_size-1))%sys_size,flipped_y_); // stabiliser (x-1,y)
    }
    if ( stabilisers[anyon_1.first][anyon_1.second].getOccupation() ) {
        dout << std::boolalpha << "placeAnyon( " << convertToDrawCoord(anyon_1).first << ", " << convertToDrawCoord(anyon_1).second << " );\n";
    } else {
        dout << std::boolalpha << "deleteAnyon( " << convertToDrawCoord(anyon_1).first << ", " << convertToDrawCoord(anyon_1).second << " );\n";
    }
    if ( stabilisers[anyon_2.first][anyon_2.second].getOccupation() ) {
        dout << std::boolalpha << "placeAnyon( " << convertToDrawCoord(anyon_2).first << ", " << convertToDrawCoord(anyon_2).second << " );\n";
    } else {
        dout << std::boolalpha << "deleteAnyon( " << convertToDrawCoord(anyon_2).first << ", " << convertToDrawCoord(anyon_2).second << " );\n";
    }
    
    dout << "a.add();\n\n";
}

void TCSystem::drawAnimationFooter( std::ofstream& dout ) {
    dout << '\n' << "erase();\n";
    // dout << "a.movie(BBox(0.25cm),loops=10,delay=250);\n";
    dout << "label(a.pdf(BBox(0.25cm),delay=500,\"controls\",multipage=false));";
}
