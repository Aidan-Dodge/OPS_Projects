#include <Adafruit_Sensor.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include <NRF24L01_W.h>

DHT weatherSensor(4, DHT11); //instance of DHT
NRF24L01_W radio(7, 8);

WeatherReport report;
int payload;

int humidity;
int temperature;

const int a2 = 16;
int sensor; //integer value for a2
int tod = 0; //0 if day, 1 if night

void radio_init(){
  radio.begin();
  radio.setReceiveAddress("SENDR");
  radio.setTransmitAddress("RPORT");
  radio.setChannel(50); //matches channel of receiving NRF`
}

void get_weather(){
  humidity = weatherSensor.readHumidity();
  delay(500); //small delay to make sure proper values are read
  temperature = weatherSensor.readTemperature();
}

void get_tod(){
  sensor = analogRead(a2);

  if (sensor < 500) //night
  {
    tod = 1;
  }else { //day
    tod = 0;
  }
}

void set_weather(){
  report.temperature = temperature;
  report.humidity = humidity;
  report.light = tod;
}

void radio_transmit(){
  //delay is 10 seconds between each radio send
  radio.write(report);
}


void setup()
{
  weatherSensor.begin();
  radio_init();
}

/* Get weather from DHT.
* Get time of day from photo resistor.
* Set weather report to match values.
* Transmit reporty over radio.
*/

void loop()
{
  get_weather();
  get_tod();
  set_weather();
  radio_transmit();
  delay(9500);
}