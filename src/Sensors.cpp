#include "Sensors.h"
#include "config.h"

//#define DEBUG
extern void safeDelay(unsigned int ms);

Sensors::Sensors(){
    _bmp = new BMP280Sensor(BMP_I2C_ADDR);
    _ina = new INASensor(INA_I2C_ADDRESS);
    
    pinMode(VCC_I2C_PIN, OUTPUT);digitalWrite(VCC_I2C_PIN, LOW);
    pinMode(VCC_LIGHT_SENSOR_PIN, OUTPUT);digitalWrite(VCC_LIGHT_SENSOR_PIN, LOW);
    //pinMode(VCC_RAIN_PIN, OUTPUT);digitalWrite(VCC_RAIN_PIN, LOW);
    pinMode(VCC_UV_PIN, OUTPUT);digitalWrite(VCC_UV_PIN, LOW);
    pinMode(VCC_WIND_PIN, OUTPUT);digitalWrite(VCC_WIND_PIN, LOW);
}

Weather Sensors::getWeather(){
    Weather data;

    //I2C
    digitalWrite(VCC_I2C_PIN, HIGH);
    safeDelay(SENSOR_WARMUP_DELAY_MS);
    _bmp->connect();
    
    auto bmeData = _bmp->readData();
    if(bmeData.isDataReceived){
        data.humidity = 0;
        data.temperature = bmeData.temperatureCelsium;
        data.pressure = bmeData.pressureInHPascals;
    }
    
    //RAIN
    //safeDelay(SENSOR_WARMUP_DELAY_MS);
    data.raindropLevel = (1024 - getDataFromAnalogPin(ANALOG_INPUT_PIN)) * 100 / 1024;
    digitalWrite(VCC_I2C_PIN, LOW);//Fix. Use I2C power for rain sensor

    //LIGHT
    digitalWrite(VCC_LIGHT_SENSOR_PIN, HIGH);
    safeDelay(100);
    data.lightLevel = getDataFromAnalogPin(ANALOG_INPUT_PIN) * 100 / 1024;
    digitalWrite(VCC_LIGHT_SENSOR_PIN, LOW);

    //UV
    digitalWrite(VCC_UV_PIN, HIGH);
    safeDelay(SENSOR_WARMUP_DELAY_MS * 3);
    data.uvLevel = getDataFromAnalogPin(ANALOG_INPUT_PIN) / 1024 * 33;
    digitalWrite(VCC_UV_PIN, LOW);

    //WIND
    digitalWrite(VCC_WIND_PIN, HIGH);
    safeDelay(SENSOR_WARMUP_DELAY_MS);
    data.windLevel = getDataFromAnalogPin(ANALOG_INPUT_PIN) * 100 / 1024;
    digitalWrite(VCC_WIND_PIN, LOW);

    return data;
}

PowerLevels Sensors::getPowerLevels(){
    PowerLevels data;

    //I2C
    digitalWrite(VCC_I2C_PIN, HIGH);
    safeDelay(SENSOR_WARMUP_DELAY_MS);
    _ina->connect();
    auto inaData = _ina->readData();

    if(inaData.isDataReceived){
        data.batteryCurrent = inaData.current;
        data.batteryVoltage = inaData.voltage;
    }
    digitalWrite(VCC_I2C_PIN, LOW);

    return data;
}

float Sensors::getDataFromAnalogPin(uint8_t pin){
    float Vvalue=0.0;
    for(unsigned int i=0;i<10;i++){
        Vvalue=Vvalue+analogRead(pin); 
        delay(5);
    }
    return Vvalue=(float)Vvalue/10.0;
}