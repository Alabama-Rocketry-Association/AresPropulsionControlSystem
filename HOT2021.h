#ifndef HOT2021_H
#define HOT2021_H

#include "exec.h"


/*
Author: Jonathan Martini 2021


*/




namespace HotFireHardwareSpecific2021{
    namespace RelayCommands{

    };

    namespace DefinedCommandSequences{
        /*
            All commands are defined in the HotFireHardwareSpecific2021::RelayCommands
            all functions passed must have a return type int and take params {int, const char*} 
            if change, create new header file and redefine command_sequence data structure

        */
        command_sequence tank_pressurization = {
            .functions {

            },
            .args {

            }
        };
        command_sequence tank_depressurization = {
            .functions {

            },
            .args {

            }
        };
        command_sequence engine_start = {
            .functions {

            },
            .args {

            }
        }
    };

};


#endif