#ifndef TIMER_h
#define TIMER_h

class TIMER
{
private:
    int milliDivsecs;
    unsigned long currmillis;
    unsigned long prevMillis;
    int timer_step;

public:
    int secs;
    int mins;
    TIMER(int sec, int min);
    void updateTime();
    void increase_min();
    void decrease_min();
    void increase_sec();
    void decrease_sec();
    void penalty();
    void boom();
};
#endif
