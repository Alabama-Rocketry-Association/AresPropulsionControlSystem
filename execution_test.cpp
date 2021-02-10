#include "exec.h"
#include <iostream>
#include <stdlib.h>
#include <errno.h>

#define MSG "WORKING CORRECTLY\n" // const char *

using namespace execution;

int dummy(int mode, const char *msg){
    //dummy pass function
    printf("%d\t%s", mode, msg);
    return 0;
}


int main(){
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
