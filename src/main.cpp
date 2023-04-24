#include <Arduino.h>
#include <LifecycleManager.h>

//#define DEBUG

LifecycleManager* lifecycleManager;


void setup() {
   #ifdef DEBUG
   Serial.begin(9600);
   Serial.println("START");
   #endif
   //Watchdog.enable(RESET_MODE, WDT_PRESCALER_1024);
   lifecycleManager = new LifecycleManager();
}

void loop() {  
	lifecycleManager->iterate();
  //Watchdog.reset();
}

void safeDelay(unsigned int ms){
	for(unsigned int i = 0; i < ms; i += 1000){
		delay(ms - i > 1000 ? 1000 : (ms - i));
		//Watchdog.reset();
	}
}