#ifndef EMBEDDED_H

#endif // !EMBEDDED_H

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
#define INPUT    "in"
#define OUTPUT   "out"

#define LEN(x) (int)(sizeof(x)/sizeof(x[0]))        //macros for find length of dynamic array
#define sleep(t) usleep(t*1000)                     //sleep for t milliseconds

char *get_path(int pin, char *path)
{
    char *buff = (char*)malloc(128 * sizeof(char));
    strcpy(buff, GPIO);
    strcat(buff, PIN);
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

int PinMode(int pin, const char* mode)
{
   char *path = get_path(pin, "/direction");
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

int PinWrite(int pin, int payload)
{
    char *path = get_path(pin, "/value");
    int fd = open(path, O_WRONLY);
    char *buff = (char*)malloc(20 * sizeof(char));
    int amt = sprintf(buff, "%d", payload);
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
