#pragma once

class PowerManager
{
public:
    PowerManager();
    void deepSleep(int seconds);
    void wakeUp();

private:
    void safeDelay(unsigned int ms);
};