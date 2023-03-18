#ifndef TIMER_h
#define TIMER_h

class TIMER
{
private:
    int milliDivsecs;
    unsigned long currmillis;
    unsigned long prevMillis;
    int decrement;

public:
    int secs;
    int mins;
    TIMER(int sec, int min);
    void updateTime();
    void penalty();
    void boom();
};
#endif
