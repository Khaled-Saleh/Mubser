#include <math.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

unsigned int period=90000;
char path [256];
FILE *fp;



static volatile int exit_loop = 0;


void gpio_init(int gpio, const char* direction)
{


     // Export GPIO in the filesystem
     snprintf (path, 256, "/sys/class/gpio/export");
     if ((fp = fopen (path, "w")) == NULL) {
         perror (path);
         exit (EXIT_FAILURE);
     }
     fprintf (fp,"%d", gpio);
     fclose (fp);

     // Toggle the GPIO output
     snprintf (path, 256, "/sys/class/gpio/gpio%d/direction", gpio);
     if ((fp = fopen (path, "w")) == NULL) {
         perror (path);
         exit (EXIT_FAILURE);
     }
     fprintf (fp, direction);
     fclose (fp);

}




void pwm_start(int gpio, int duty)
{



     // Write the value of the GPIO alternately
     snprintf (path, 256, "/sys/class/gpio/gpio%d/value", gpio);
     
  if ((fp = fopen (path, "w")) == NULL) {
         perror (path);
         exit (EXIT_FAILURE);
     	}
    
         fprintf (fp, "%d", 1);
         fflush (fp);
         usleep((useconds_t)(period*duty*.01));
         
	fprintf (fp, "%d",0);
         fflush (fp);
        usleep((useconds_t)(period-period*duty*.01));
	
	fclose(fp);

}



void gpio_deinit(int gpio)
{
	 if ((fp = fopen ("/sys/class/gpio/unexport", "w")) == NULL) {
         perror ("/sys/class/gpio/unexport");
         exit (EXIT_FAILURE);
     }
     fprintf (fp,"%d", gpio);
     fclose (fp);



}

void handler_signal (int unused)
{
     exit_loop = 1;
}



int main ()
{

unsigned int gpio=39;
int i;


gpio_init(gpio,"out");


signal (SIGINT, handler_signal);

while (!exit_loop)
{

for (i=0;i<=100;i+=5)
pwm_start(gpio,i);

}

gpio_deinit(gpio);

return 0;
}
