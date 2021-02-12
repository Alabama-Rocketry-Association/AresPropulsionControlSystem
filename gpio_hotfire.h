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
    std::vector<void(*)(pin_io::pin*, unsigned)> functions;        //function type int(*func)(params types) pointers
    std::vector<std::tuple<int, const char*>> args;
};



//using kernel api for GPIO
#define GPIO_EXPORT_K "/sys/class/gpio/export"
#define GPIO_UNEXPORT_K "/sys/class/gpio/unexport"
#define GPIO_BASE_DEV_K "/sys/class/gpio/gpio" // gpio#/direction gpio#/value
#define GPIO_BASE_VALUE_K "/value"
#define GPIO_BASE_DIRECTION_K "/direction"



namespace pin_io {
    /*

    from https://www.kernel.org/doc/Documentation/gpio/sysfs.txt

        GPIO signals have paths like /sys/class/gpio/gpio42/ (for GPIO #42)
        and have the following read/write attributes:

    /sys/class/gpio/gpioN/

	"direction" ... reads as either "in" or "out". This value may
		normally be written. Writing as "out" defaults to
		initializing the value as low. To ensure glitch free
		operation, values "low" and "high" may be written to
		configure the GPIO as an output with that initial value.

		Note that this attribute *will not exist* if the kernel
		doesn't support changing the direction of a GPIO, or
		it was exported by kernel code that didn't explicitly
		allow userspace to reconfigure this GPIO's direction.

	"value" ... reads as either 0 (low) or 1 (high). If the GPIO
		is configured as an output, this value may be written;
		any nonzero value is treated as high.

		If the pin can be configured as interrupt-generating interrupt
		and if it has been configured to generate interrupts (see the
		description of "edge"), you can poll(2) on that file and
		poll(2) will return whenever the interrupt was triggered. If
		you use poll(2), set the events POLLPRI and POLLERR. If you
		use select(2), set the file descriptor in exceptfds. After
		poll(2) returns, either lseek(2) to the beginning of the sysfs
		file and read the new value or close the file and re-open it
		to read the value.

	"edge" ... reads as either "none", "rising", "falling", or
		"both". Write these strings to select the signal edge(s)
		that will make poll(2) on the "value" file return.

		This file exists only if the pin can be configured as an
		interrupt generating input pin.

	"active_low" ... reads as either 0 (false) or 1 (true). Write
		any nonzero value to invert the value attribute both
		for reading and writing. Existing and subsequent
		poll(2) support configuration via the edge attribute
		for "rising" and "falling" edges will follow this
		setting.
    */
    struct pin {
        char *name;
        unsigned pin;
        char *value;
        char *mode;
    };

    class GPIOHypervisor {
        private:
            std::vector<pin> PINS;
        public:
            GPIOHypervisor(std::vector<std::tuple<std::string, unsigned, char*>> init_values); // pin name, pin number, direction MODE;
            void GPIOHypervisorSetPinByName(std::string, const char*);
            void GPIOHypervisorSetPinByNumber(unsigned, const char *);
            void GPIOHypervisorSetDirectionByName(std::string, const char*);
            void GPIOHypervisorSetDirectionbyNumber(unsigned, const char*);
    };

    namespace gpio_util {
        
        char *path(pin *target, const char* fname){
            //generates path
            char *buff = (char*)malloc(32 * sizeof(char));  //allocating memory for a character string using pointers
            sprintf(buff, "%s", GPIO_BASE_DEV_K);
            sprintf(buff, "%d", target->pin);
            sprintf(buff, "%s", fname);
            return buff;
        };

        void init(pin *target, const char* MODE){
            int export_file = open(GPIO_EXPORT_K, O_WRONLY);
            char *buff = (char*)malloc(8*sizeof(char));
            int w = sprintf(buff, "%d", target->pin);
            int wd = write(export_file, buff, w);
            if (w != wd){
                perror("Error Init:\t");
                return;
            }
            free(buff);
            close(export_file);
            char *buff = (char*)malloc(8*sizeof(char));
            char *target_direction = path(target, GPIO_BASE_DIRECTION_K);
            int dir_file = open(target_direction, O_WRONLY);
            int w = sprintf(buff, "%s", MODE);
            int wd = write(dir_file, buff, w);
            if (w != wd){
                perror("Error Init:\t");
                return;
            }
            sprintf(target->mode, "%s", MODE);
            free(buff);
            free(target_direction);
            close(dir_file);
        };

        void set_value(pin *target, const char* VALUE){
            char *path_target = path(target, GPIO_BASE_VALUE_K);
            int value_file = open(path_target, O_WRONLY);
            int wd = write(value_file, VALUE, strlen(VALUE));
            if (wd != strlen(VALUE)){
                perror("Cannot Set Value:\t");
                return;
            }
            free(target->value);                            //reseting the memory of target.value because sprintf concat's c-strings
            target->value = (char*)malloc(3*sizeof(char));
            sprintf(target->value, "%s", VALUE);
            close(value_file);
            free(path_target); //freeing memory allocated by data allocated by path()
        };

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