#include <stdio.h>
#include <time.h>
#include <signal.h>

timer_t gTimerid;

void start_timer(void)
{


struct itimerspec value;

value.it_value.tv_sec = 1;
value.it_value.tv_nsec = 0;

value.it_interval.tv_sec = 1;
value.it_interval.tv_nsec = 0;

timer_create (CLOCK_REALTIME, NULL, &gTimerid);

timer_settime (gTimerid, 0, &value, NULL);

}

void stop_timer(void)
{


struct itimerspec value;

value.it_value.tv_sec = 0;
value.it_value.tv_nsec = 0;

value.it_interval.tv_sec = 0;
value.it_interval.tv_nsec = 0;

timer_settime (gTimerid, 0, &value, NULL);


}


void timer_callback(int sig) {

 printf(" Catched timer signal: %d ... !!\n", sig);

}

int main(int ac, char **av)
{
    (void) signal(SIGALRM, timer_callback);
    start_timer();
    while(1);
}