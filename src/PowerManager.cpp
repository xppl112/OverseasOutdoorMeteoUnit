#include <Arduino.h>
#include <PowerManager.h>
#include <config.h>

//#define DEBUG

PowerManager::PowerManager(){
    
}

void PowerManager::deepSleep(int seconds){
    #ifdef DEBUG
    Serial.print("PowerManager::deepSleep ");Serial.println(seconds);
    #endif

    ESP.deepSleep(seconds * 1000000);
    //safeDelay(1000 * (seconds));
}

void PowerManager::safeDelay(unsigned int ms){
	for(unsigned int i = 0; i < ms; i += 1000){
        //power.sleepDelay(ms - i > 1000 ? 1000 : (ms - i));
        delay(ms - i > 1000 ? 1000 : (ms - i));
		//Watchdog.reset();
	}
}

void PowerManager::wakeUp(){
    //power.wakeUp();
}