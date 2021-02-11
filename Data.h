#ifndef DATA_H
#define DATA_H

#include <iostream>
#include <stdlib.h>
#include "exec.h"
#include "HOT2021.h"
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <vector>
#include <tuple>
#include <future>
#include <fstream>

namespace Data {
    
    int write_csv(std::string fname, datafeed feed){
        int written = 0;
        
        std::ofstream csv(fname);
        const char delimiter = ',';


        return written;
    }

    struct datapacket {
        std::time_t timestamp;
        std::vector<std::tuple<std::string, double>> data;
        int id;
    };

    struct datafeed {
        std::time_t start_time;
        std::string name;
        std::vector<datapacket> feed;
    };

    union hypervisor {
        std::time_t start_time;
        std::string name;
        std::vector<std::tuple<std::string, datafeed>> feeds;
    };

    class DataHyperVisor{
            hypervisor *hypervisor;
        public:
            DataHyperVisor(std::vector<std::tuple<std::string, int>> feed_name_pin_list);
            datapacket *get_current(std::string datafeed_key);
            datapacket *get_from_time(std::string, std::time_t start, std::time_t end);      
            void *update(std::string feed);
            void to_csv_file(std::string fname);
    };
};


Data::DataHyperVisor::DataHyperVisor(std::vector<std::tuple<std::string, int>> feed_name_pin_list){
    spdlog::info("Starting Data Hypervisor");
    
}

#endif