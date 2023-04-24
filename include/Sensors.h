#pragma once
#include <Models/Weather.h>
#include <Models/PowerLevels.h>
#include <Wire.h>
#include "INASensor.h"
#include "BMP280Sensor.h"

class Sensors
{
public:
    Sensors();
    
    Weather getWeather();
    PowerLevels getPowerLevels();

private:
    BMP280Sensor* _bmp;
    INASensor* _ina;

    float getDataFromAnalogPin(uint8_t pin);
};