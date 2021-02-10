#include "exec.h"
#include <iostream>
#include <stdlib.h>
#include <errno.h>
#include <spdlog/spdlog.h>



#define MSG "WORKING CORRECTLY\n" // const char *

/*

Author: Jonathan Martini 2021

*/


using namespace execution;



int dummy(int mode, const char *msg){
    //dummy pass function
    char buffer[64];
    sprintf(buffer, "%d, %s", mode, msg);
    spdlog::debug(buffer);
    return 0;
}


int main(){
    spdlog::set_level(spdlog::level::debug);
    command_sequence test_seq = {
        .functions{
            &dummy,
            &dummy,
            &dummy,
            &dummy
        },
        .args{
            {1, MSG},
            {2, MSG},
            {3, MSG},
            {4, MSG}
        }
    };
    int res = execution::execute_command_sequence(&test_seq);
    std::cout << res << std::endl;
    return 0;
}
