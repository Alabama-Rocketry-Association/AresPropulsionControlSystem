#ifndef EXEC_H
#define EXEC_H

#include <stdio.h>
#include <iostream>
#include <vector>
#include <assert.h>
#include <tuple>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <spdlog/spdlog.h>

/*
Author: Jonathan Martini 2021

*/
#define PtrToStr(x) static_cast<std::string>(static_cast<void *>(x)) //for logging debugging data addresses of wanted datastructures and function
                    // void* -> std::string* -> std::string type sequence

struct command_sequence {
    std::vector<int(*)(int, const char *)> functions;        //function type int(*func)(params types) pointers
    std::vector<std::tuple<int, const char*>> args;
};


namespace execution {
    int execute_command_sequence(command_sequence *seq){
        assert (seq->functions.size() == seq->args.size()); //assert that the size of the function list matches that of the argument list
        int sz = seq->functions.size();
        for (int i = 0; i < sz; i++){
            int res = seq->functions[i](std::get<0>(seq->args[i]), std::get<1>(seq->args[i]));      //passes and executes function with arguments
            if (res != -1) continue;
            else {
                perror("Error:\t"); 
                return -1;
            }
        }
        return 0;
    }

};


#endif
