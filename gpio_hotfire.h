#include <stdio.h>
#include <iostream>
#include <vector>
#include <assert.h>
#include <tuple>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string>

/*
Author: Jonathan Martini 2021
*/
#define LOW 0
#define HIGH 1
#define ON "in"
#define OFF "off"
#define PtrToStr(x) (*static_cast<std::string*>(static_cast<void *>(x))).c_str() //for logging debugging data addresses of wanted datastructures and function
                    // void* -> std::string* -> std::string type sequence
#define itoa(x) std::to_string(x)
#define GPIO_MODE "gpio mode "
#define GPIO_EXPORT "gpio export"
#define GPIO_TOGGLE "gpio toggle"
#define GPIO_WRITE "gpio write"
#define GPIO_READ "gpio read"
#define LOW "0"
#define HIGH "1"

struct command_sequence {
    std::vector<int(*)(unsigned, const char *)> functions;        //function type int(*func)(params types) pointers
    std::vector<std::tuple<int, const char*>> args;
};

struct command_sequence {
    std::vector<int(*)(unsigned, const char *)> functions;        //function type int(*func)(params types) pointers
    std::vector<std::tuple<int, const char*>> args;
};




namespace pin_io {
    struct pin {
        unsigned pin;
        unsigned value;
        char* mode;
    };

    void set_value(pin *target, unsigned value){
        
    };

}

namespace pin_io_hacky{
    std::string execute_shell(std::string command) {
        // copy pasted from stack overflow
        char buffer[128];
        std::string result = "";
        // Open pipe to file
        FILE* pipe = popen(command.c_str(), "r");
        if (!pipe) {
            return "popen failed!";
        }
        // read till end of process:
        while (!feof(pipe)) {
            // use buffer to read and add to result
            if (fgets(buffer, 128, pipe) != NULL)
                result += buffer;
        }
        pclose(pipe);
        return result;
        }

    int writePIN(int pin, const char* Mode){
        std::string pin = itoa(pin);
        auto cmd = GPIO_WRITE + pin + std::string(Mode);
        system(cmd.c_str());
        return 0;
        };
    int readPIN(int pin, const char* MODE){
        std::string pin = itoa(pin);
        int out = std::stoi(pin_io_hacky::execute_shell(GPIO_READ + pin));
        return out;
        };
    int initPIN(int pin, const char* MSG){
        std::string pin = itoa(pin);
        system(GPIO_EXPORT + pin);
        return 0;
        };
}