#ifndef HOT2021_H
#define HOT2021_H

#include "exec.h"
#include <unistd.h> //usleep
#include <chrono>

#define LOG_PIN_MSG_INIT "Pin Initialized"
#define LOG_PIN_MSG_ON "Pin On"
#define LOG_PIN_MSG_OFF "Pin OFF"
#define NONE "NONE"                 //const char* to be passed to functions without a need


#define _sleep(t) usleep(t * 1000) //sleep macro for t millisecond wait usleep -> microseconds 1000 micro==milli

/*
Author: Jonathan Martini 2021
*/


namespace Data {
    struct datapacket {
        std::time_t timestamp;
        std::vector<std::tuple<std::string, double>> data;
        int id;
    };

    union datafeed {
        std::time_t start_time;
        std::string name;
        std::vector<datapacket> feed;
    };

    class DataHyperVisor{

    };
};


namespace HotFireHardwareSpecific2021{
    namespace RelayCMD{
        int cmd_sleep(int t, const char *) { //follows command_sequence function pointer type formatting
            // milliseconds
            _sleep(t);
            return 0;
        };
        
        int RelayINIT(int pin, const char *MODE){
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