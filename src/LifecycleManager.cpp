#include "LifecycleManager.h"
#include "config.h"

extern void safeDelay(unsigned int ms);
//#define DEBUG

LifecycleManager::LifecycleManager() {
    _powerManager = new PowerManager();
    _sensors = new Sensors();
    _webClient = new WebClient();

    pinMode(CONTROL_LED_PIN, OUTPUT);digitalWrite(CONTROL_LED_PIN, HIGH);
    initialize();
}

void LifecycleManager::initialize(){
    #ifdef DEBUG
    Serial.println("LifecycleManager::initialize");
    #endif

    _systemState = SystemState{
        .isFirstRun = true
    };
    _systemState.wifiErrors = 0;

    _settings = DEFAULT_SETTINGS;

    blinkControlLed(1);
}

void LifecycleManager::iterate() {
    #ifdef DEBUG
    Serial.println("LifecycleManager::iterate");
    #endif

    measure();
    sendData();
    updateSystemState();
    sleep();
}

void LifecycleManager::measure(){
    #ifdef DEBUG
    Serial.println("LifecycleManager::measure");
    #endif

    _currentWeather = _sensors->getWeather();
    _currentPowerLevels = _sensors->getPowerLevels();
}

void LifecycleManager::sendData(){
    #ifdef DEBUG
    Serial.println("LifecycleManager::sendData");
    #endif

    PostData data = {
        .timestamp = _systemState.timestamp,

        .temperature = _currentWeather.temperature,
        .humidity = _currentWeather.humidity,
        .pressure = _currentWeather.pressure,
        .raindropLevel = _currentWeather.raindropLevel,
        .uvLevel = _currentWeather.uvLevel,
        .lightLevel = _currentWeather.lightLevel,
        .windLevel = _currentWeather.windLevel,

        .batteryVoltage = _currentPowerLevels.batteryVoltage,
        .batteryCurrentIdle = _currentPowerLevels.batteryCurrent
    };

    for(int i = WIFI_CONNECTION_RETRY_ATTEMPTS_COUNT; i > 0 ; i--){
        if(!_webClient->connect()){
            _systemState.wifiErrors++;
            if(i == 1){
                blinkControlLed(4);
                return;
            }
        }
        else break;
    }

    //Measure current with wifi connected
    _currentPowerLevels = _sensors->getPowerLevels();
    data.batteryCurrentWifiOn = _currentPowerLevels.batteryCurrent;
    data.wifiSignalLevel = _webClient->lastSignalLevel;
    data.wifiErrorsCount = _systemState.wifiErrors;

    GetData gdata;
    for(int i = WIFI_SEND_DATA_ATTEMPTS_COUNT; i > 0; i--){
        if(!_webClient->postData(data, &gdata))_systemState.wifiErrors++;
        else {
            if(gdata.dataReceived && gdata.sleepDurationSeconds > 0 && gdata.sleepDurationSeconds < 10000){
                _settings.sleepDurationSeconds = gdata.sleepDurationSeconds;
                if(_systemState.isFirstRun)blinkControlLed(2);
                break;
            }
        }
        data.wifiErrorsCount = _systemState.wifiErrors;
    }
    _webClient->disconnect();
}

void LifecycleManager::updateSystemState(){    
    _systemState.timestamp = millis() / 1000;
    _systemState.isFirstRun = false;
}

void LifecycleManager::sleep(){
    _powerManager->deepSleep(_settings.sleepDurationSeconds);
}

void LifecycleManager::blinkControlLed(int count){
    for(int i=0;i<count;i++){
        if(i!=0)safeDelay(300);
        digitalWrite(CONTROL_LED_PIN, LOW);
        safeDelay(300);
        digitalWrite(CONTROL_LED_PIN, HIGH);
    }
}