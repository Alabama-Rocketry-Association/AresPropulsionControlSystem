#ifndef HOT2021_H
#define HOT2021_H

#include "exec.h"
#include <unistd.h> //usleep


#define _sleep(t) usleep(t * 1000) //sleep macro for t millisecond wait usleep -> microseconds 1000 micro==milli

/*
Author: Jonathan Martini 2021


*/




union datafeed {

};

namespace HotFireHardwareSpecific2021{
    namespace RelayCMD{
        int cmd_sleep(int t, const char *) { //follows command_sequence function pointer type formatting
            // milliseconds
            _sleep(t);
            return 0;
        };
    };

    namespace DefinedCMDSequences{
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
        };
    };

    namespace ManualInput {
        
    };

    class ControlSystemHandler {
        public:
            bool set_thresholds(std::vector<std::tuple<std::string, double>>);      //initizialization of pressure thresholds
            void run();                                                             //main program thread
            void record(const char *);
    };
};


#endif