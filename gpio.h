#ifndef GPIO_H

#endif // !GPIO_H

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>

#define GPIO     "/sys/class/gpio"
#define EXPORT   "/sys/class/gpio/export"
#define UNEXPORT "/sys/class/gpio/unexport"
#define PIN      "/gpio"
#define CHIP     "/gpio_chip"
#define INPUT    "in"
#define OUTPUT   "out"
#define LOW      "low"
#define HIGH     "high"

#define LEN(x) (int)(sizeof(x)/sizeof(x[0]))        //macros for find length of dynamic array
#define sleep(t) usleep(t*1000)                     //sleep for t milliseconds


/*
    Linux sysfs provides the gpio api for the linux kernel

    linux kernel documentation for advanced usage
    https://www.kernel.org/doc/Documentation/gpio/sysfs.txt

        Paths in Sysfs
        --------------
        There are three kinds of entries in /sys/class/gpio:

   -	Control interfaces used to get userspace control over GPIOs;

   -	GPIOs themselves; and

   -	GPIO controllers ("gpio_chip" instances).


*/
char *get_path(int pin, char *path, const char* descriptor)
/*
GPIO signals have paths like /sys/class/gpio/gpio42/ (for GPIO #42)
and have the following read/write attributes:

    /sys/class/gpio/gpioN/

*/
{
    char *buff = (char*)malloc(128 * sizeof(char));
    strcpy(buff, GPIO);
    strcat(buff, descriptor);
    sprintf(buff, "%d", pin);
    strcat(buff, path);
    return buff;
}

int gpio_setup(int *pins)
{
    int fd = open(EXPORT, O_WRONLY);                        //open /sys/class/gpio/export
    int sz = LEN(pins);                                     //get length of dynamic array containing pins to export
    for (int i = 0; i < sz; i++)                            
    {
        char *buff = (char*)malloc(3 * sizeof(char));       //initialize buffer for copying c strings into and sprint'ing int -> char*
        int amt = sprintf(buff, "%d", pins[i]);
        int out = write(fd, buff ,amt);
        if (out != amt)                                     //check is buffer was written to file if err then throw error
        {
            perror("\n[!!] Pin Initialization Error:\t");
            return -1;
        }
        else
        {
            printf("\r[##] Exported Pin %d", pins[i]);
        }
        printf("\r[##] Successfully Initialized All Pins!\n");
        free(buff);
        close(fd);
        return -1;
    }   
    close(fd);
    return 0;
}

int PinMode(int pin, const char* mode, const char* descriptor)
/*
"direction" ... reads as either "in" or "out". This value may
		normally be written. Writing as "out" defaults to
		initializing the value as low. To ensure glitch free
		operation, values "low" and "high" may be written to
		configure the GPIO as an output with that initial value.

		Note that this attribute *will not exist* if the kernel
		doesn't support changing the direction of a GPIO, or
		it was exported by kernel code that didn't explicitly
		allow userspace to reconfigure this GPIO's direction.


        This will be useful for switching solenoids by changing low, high
*/
{
   char *path = get_path(pin, "/direction", descriptor);
   int fd = open(path, O_WRONLY);
   char *buff = (char*)malloc(3 * sizeof(char));
   int amt = sprintf(buff, "%d", pin);
   int out = write(fd, mode, amt);
   free(path);
   free(buff);
   close(fd);
   if(out != amt)
   {
       perror("\n[!!] Pin Write Error:\t");
       close(fd);
       return -1;
   }
   close(fd);
   return 0;
}

int PinWrite(int pin, int value, const char* descriptor)
/*
        "value" ... reads as either 0 (low) or 1 (high). If the GPIO
		is configured as an output, this value may be written;
		any nonzero value is treated as high.
*/
{
    char *path = get_path(pin, "/value", descriptor);
    int fd = open(path, O_WRONLY);
    char *buff = (char*)malloc(20 * sizeof(char));
    int amt = sprintf(buff, "%d", value);
    int out = write(fd, buff, amt);
    if (out != amt)
    {
        close(fd);
        perror("[!!]Pin Write Error Data:\t");
        return -1;
    }
    close(fd);
    return 0;
}
