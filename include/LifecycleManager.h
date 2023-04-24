#pragma once
#include "PowerManager.h"
#include "Sensors.h"
#include "WebClient.h"
#include "Models/SystemState.h"
#include "Models/Settings.h"

class LifecycleManager
{
public:
    LifecycleManager();
    void iterate();

    void initialize();
    void measure();
    void sendData();
    void getUpdates();

private:
    Settings _settings;
    SystemState _systemState;

    PowerManager* _powerManager;
    Sensors* _sensors;
    WebClient* _webClient;

    unsigned int _sendDataIterationCounter = 0;
    Weather _currentWeather;
    PowerLevels _currentPowerLevels;

    void updateSystemState();
    void sleep();
    void blinkControlLed(int count);
};