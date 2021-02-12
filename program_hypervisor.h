#ifndef PROGRAM_HYPERVISOR_H
#define PROGRAM_HYPERVISOR_H

#include <stdio.h>
#include <iostream>
#include <vector>
#include <assert.h>
#include <tuple>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string>
#include <cstring>
#include <assert.h>
#include <future> //asynchronous threads
#include "gpio_hotfire.h"


#define TUPVEC(x, i) std::get<i>(x)     //macros for cycling thru tuples embedded in vector lists

namespace DATAFEED{
    
    union current_data {
        time_t timestamp;
        std::vector<std::tuple<std::string, double>> now_data;
    };


    double get_value_current_data(current_data* feed, std::string key){
        auto i = feed->now_data.begin();
        bool exit = false;
        while(!exit){
            std::string cmp = TUPVEC(*i, 0);
            if (cmp.compare(key) == 0){
                exit = true;
                return TUPVEC(*i, 1);
            }
        return -1.0;
        }

    };
}


namespace RUNTIME {
    union RUNTIME_FLAGS {
        std::vector<std::tuple<std::string, bool>> flags;
        std::string logging_fname;
        std::string data_csv_fname;
    };

    double get_flag_value(RUNTIME_FLAGS* flags, std::string key){
        auto i = flags->flags.begin();
        bool exit = false;
        while(!exit){
            std::string cmp = TUPVEC(*i, 0);
            if (cmp.compare(key) == 0){
                exit = true;
                return TUPVEC(*i, 1);
            }
        return -1.0;
        };

};

class ProgramHypervisor {
    private:
        //data unions to store across async loops
        DATAFEED::current_data now;
        RUNTIME::RUNTIME_FLAGS flags;
        pin_io::RUNTIME_PINS pins;
    public:
    ProgramHypervisor();
    bool CheckFlag(std::string flag);
    double GetCurrentData(std::string key);
    double UpdateCurrentData(std::string key, double value);
    double LogDataFile(); //exporting the current_data->now_data vector to csv log
    
};



#endif // !PROGRAM_HYPERVISOR_H
