/*
 *******************************
 *
 * REVISION HISTORY
 * Version 1.0 - Alex Arent
 *
 * DESCRIPTION
 * Sensor is used in my Kitchen. It was based
 * off the Motion sensor MySensors example
 */

// Enable debug prints
#define MY_DEBUG

// Enable and select radio type attached
#define MY_RADIO_RF24
#define MY_SPLASH_SCREEN_DISABLED
#define MY_RF24_PA_LEVEL RF24_PA_MAX
#define MY_RF24_IRQ_PIN 2
#define MY_RF24_CHANNEL 125
#define MY_NODE_ID 4

#include <MySensors.h>
#include <DHT.h>
#include <HCSR04.h>

uint32_t SLEEP_TIME = 120000; // Sleep time between reports (in milliseconds)
                              // 21600000 = 6 hours
                              
#define DHT_DATA_PIN 4
#define HCSR04_TRIGGER 5
#define HCSR04_ECHO 6

#define CHILD_ID_TEMP 1
#define CHILD_ID_HUM 2
#define CHILD_ID_DISTANCE 3

#define TX_RX_DELAY 500

// Initialize messages
MyMessage msgTemp(CHILD_ID_TEMP,V_TEMP);
MyMessage msgHum(CHILD_ID_HUM,V_HUM);
MyMessage msgDistance(CHILD_ID_DISTANCE,V_DISTANCE);

bool txRx = false;

DHT dht;
HCSR04 hc(HCSR04_TRIGGER,HCSR04_ECHO);//initialisation class HCSR04 (trig pin , echo pin)

void setup()
{ 
  dht.setup(DHT_DATA_PIN); // set data pin of DHT sensor

  wait(dht.getMinimumSamplingPeriod());
}

void presentation()
{
	// Send the sketch version information to the gateway and Controller
	sendSketchInfo("Simple Node", "Version 1.0");
  
	// Register all sensors to gw (they will be created as child devices)
  present(CHILD_ID_TEMP,S_TEMP);
  present(CHILD_ID_HUM,S_HUM);
  present(CHILD_ID_DISTANCE,S_DISTANCE);
}

void loop()
{
	//Read state of sensors
  float humidity = dht.getHumidity();
  float temperature = dht.toFahrenheit(dht.getTemperature());
  float distance = hc.dist();

  txRx = false;
  while(!txRx){
    send(msgDistance.set(distance,1),true);
    wait(TX_RX_DELAY);
  }

  txRx = false;
  while(!txRx){
    send(msgTemp.set(temperature,1),true);
    wait(TX_RX_DELAY);
  }

  txRx = false;
  while(!txRx){
    send(msgHum.set(humidity,1),true);
    wait(TX_RX_DELAY);
  }
  wait(2000);
  }



void receive(const MyMessage &message) { 
  #ifdef MY_DEBUG
    Serial.print("Incoming message for child: ");
    Serial.println(message.sensor);
  #endif
   txRx = true;
  switch(message.sensor){
    case CHILD_ID_TEMP:
      Serial.println("Temp Received at Gateway");
      break;
    case CHILD_ID_HUM:
      Serial.println("Humidity Received at Gateway");
      break;
    case CHILD_ID_DISTANCE:
      Serial.println("Distance Received at Gateway");
    default:
      break;
  }
}
