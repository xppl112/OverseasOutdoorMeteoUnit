#pragma once

#include <ArduinoJson.h>

struct PostData {
    unsigned long timestamp;

    float temperature;
    unsigned int humidity;
    float pressure;
    
    unsigned int raindropLevel;
    float uvLevel;
    unsigned int lightLevel;
    unsigned int windLevel;

    float batteryVoltage;
    float batteryCurrentIdle;
    float batteryCurrentWifiOn;

    int wifiSignalLevel;
    int wifiErrorsCount;

    String toJson(){
        DynamicJsonDocument doc(2048);

        doc["ts"] = timestamp;

        doc["t"] = temperature;
        doc["h"] = humidity;
        doc["p"] = pressure;

        doc["rd"] = raindropLevel;
        doc["uv"] = uvLevel;
        doc["ll"] = lightLevel;
        doc["w"] = windLevel;

        doc["bv"] = batteryVoltage;
        doc["bci"] = batteryCurrentIdle;
        doc["bcw"] = batteryCurrentWifiOn;

        doc["wsl"] = wifiSignalLevel;
        doc["we"] = wifiErrorsCount;

        String output;
        serializeJson(doc, output);
        doc.clear();
        doc.garbageCollect();

        return output;
    }
};