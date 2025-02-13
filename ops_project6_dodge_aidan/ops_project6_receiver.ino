#include <Adafruit_Sensor.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include <NRF24L01_W.h>

LiquidCrystal_I2C lcd(0x27,16,2);  //instance of LiquidCrystal_I2C
NRF24L01_W radio(7, 8);

WeatherReport incomingReport;
int payload;
int temperature;
int humidity;
int tod;

void lcd_init(){
  //initializing lcd
  lcd.init();                      
  lcd.backlight();
  //printing test message
  lcd.setCursor(3,0);
  lcd.print("Waiting for");
  lcd.setCursor(0,1);
  lcd.print("Weather Report..");
}

void radio_init(){
  radio.begin();
  radio.setReceiveAddress("RPORT");
  radio.setTransmitAddress("SENDR");
  radio.setChannel(50); //matches channel of transmitting NRF
}


void set_weather(){
  temperature = incomingReport.temperature;
  humidity = incomingReport.humidity;
  tod = incomingReport.light;
}

void radio_transmit(){
  while(!radio.available()){}
  incomingReport = radio.read();
  
  delay(100);
}

void print_to_lcd(){
  lcd.clear();

  //top row
  lcd.setCursor(0,0);
  lcd.print("Temp: ");
  lcd.setCursor(6, 0);
  lcd.print(temperature);
  lcd.setCursor(8,0);
  lcd.print("C");

  //bottom row
  lcd.setCursor(0,1);
  lcd.print("Hum: ");
  lcd.setCursor(5, 1);
  lcd.print(humidity);
  lcd.setCursor(7, 1);
  lcd.print("%, ");
  lcd.setCursor(10,1);

  //time of day
  if (tod == 0) {lcd.print("Day");}
  else {lcd.print("Night");}

  delay(2000);
}

void setup()
{
  radio_init();
  lcd_init();
}

void loop()
{
  radio_transmit();
  set_weather();
  print_to_lcd();
}