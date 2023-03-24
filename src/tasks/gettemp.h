#ifndef TASK_GET_TEMP
#define TASK_GET_TEMP

#include <Arduino.h>

#include "../config/config.h"
#include "../config/enums.h"

#include "HTTPClient.h"

HTTPClient httpdimmer;

extern DisplayValues gDisplayValues;
extern Config config; 

void GetDImmerTemp(void * parameter){
  for (;;){ 

#if DALLAS
    extern Dallas dallas ;
    if ( !dallas.detect) {
#endif // DALLAS

      // create get request 
      httpdimmer.begin(String(config.dimmer), 80, "/state");   
      int httpResponseCode = httpdimmer.GET();

      //  read request return
      if (httpResponseCode > 0) {
        String dimmerstate = httpdimmer.getString();
        dimmerstate = dimmerstate.substring( dimmerstate.indexOf(";") + 1);
        gDisplayValues.temperature = dimmerstate.substring( 0, dimmerstate.indexOf(";") ) ; 
        if (logging.serial) {
          Serial.println("gettemp HTTP Response code: " + httpResponseCode);
          Serial.println("recu :" + dimmerstate );
          Serial.println("temperature " + gDisplayValues.temperature );
        }
      } else {
        if (logging.serial) {
          Serial.println("Get Dimmer temp : Error code: " + httpResponseCode);
        }
      }
      // Free resources
      httpdimmer.end();

#if DALLAS
    }
#endif  // DALLAS

// refresh every GETTEMPREFRESH seconds 
vTaskDelay(pdMS_TO_TICKS(15000));
  }
}

#endif