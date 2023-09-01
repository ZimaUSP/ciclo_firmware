#include "SimpleTimer.h"

// Create a first timer and specify its interval in milliseconds
// Create a second timer
SimpleTimer secondTimer;
SimpleTimer firstTimer;

// A flag indicates, that a first timer is ready


void setup()
{
    Serial.begin(9600);

    // Set an interval to 3 secs for the second timer
    secondTimer.setInterval(121000);
    firstTimer.setInterval(1000);
}

void loop()
{
    if (firstTimer.isReady()) {                    // Check is ready a second timer
      Serial.println(secondTimer.getRestTime()*0.001);
      Serial.print("Secondtimer: ");
      Serial.print(secondTimer.getMinutes());
      Serial.print(":");
      Serial.println(secondTimer.getSeconds());
      
        firstTimer.reset();                        // Reset a second timer
    }

    if (secondTimer.isReady()) {                    // Check is ready a second timer
        Serial.println("Called every 100 sec");
      
        secondTimer.reset();                        // Reset a second timer
    }
}
