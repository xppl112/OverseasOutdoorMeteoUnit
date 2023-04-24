#pragma once

#include <ArduinoJson.h>

struct GetData {
    bool dataReceived;
    unsigned int sleepDurationSeconds;

    static GetData constructFromJson(String json){
        GetData returnValue {.dataReceived = false};
        StaticJsonDocument<100> responseJson;
        deserializeJson(responseJson, json);

        if(responseJson.containsKey("sds")){
            returnValue.dataReceived = true;
            returnValue.sleepDurationSeconds = responseJson["sds"];
        }
        return returnValue;
    }
};