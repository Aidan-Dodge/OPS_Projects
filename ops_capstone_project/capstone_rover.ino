#include <NRF24L01_W.h>

/*
Initially my RF24 library had updated and I tried to run this with the RF24
class since the NRF24L01_W was not working, but I switched back to this older
version. Leaving the old code for now incase I want to use it later. It should
all be commented out.
*/

NRF24L01_W radio(7, 8);
RemoteData command;


uint8_t address[2][6] = {"REMOTE", "RCCAR"};

int left_thrshld = 0;
int down_thrshld = 0;
int right_thrshld = 255;
int up_thrshld = 255;

int motor1A = 5;
int motor2A = 6;
int motor3A = 10;
int motor4A = 9;

void radio_init(){
  if (!radio.begin()) {
  //Serial.println(F("radio hardware not responding!"));
  while (1) {} 
}
  radio.setTransmitAddress("Remot");
  radio.setReceiveAddress("RCcar");
  // radio.openWritingPipe(address[0]);
  //radio.openReadingPipe(0, address[1]);
  radio.setChannel(44); 
}
void setup() {
  //Serial.begin(9600);
  //Serial.println("Starting");
  radio_init();
  command.vx = 25;
  command.vy = 6;
  pinMode(motor1A, OUTPUT);
  pinMode(motor2A, OUTPUT);
  pinMode(motor3A, OUTPUT);
  pinMode(motor4A, OUTPUT);
}

char readController(){
  /*
  char receiving_package = '\n';
  if (radio.available()){
    uint8_t package_bytes = radio.getDynamicPayloadSize();
    radio.read(&receiving_package, package_bytes);
    Serial.println(receiving_package);
  }
  return receiving_package;
  */
}

void controlCar(char c){
  switch(c){ 
    case 'l':
      //turn left
      analogWrite(motor1A, 0);
      analogWrite(motor2A, 0);
      analogWrite(motor3A, 255);
      analogWrite(motor4A, 0);
      delay(250);
      break;

    case 'r':
    //move right
      analogWrite(motor1A, 255);
      analogWrite(motor2A, 0);
      analogWrite(motor3A, 0);
      analogWrite(motor4A, 0);
      delay(250);    
      break;

    case 'u':
    //move forward
      analogWrite(motor1A, 255);
      analogWrite(motor2A, 0);
      analogWrite(motor3A, 255);
      analogWrite(motor4A, 0);
      delay(250);
      break;

    case 'd':
    //reverse
      analogWrite(motor1A, 0);
      analogWrite(motor2A, 255);
      analogWrite(motor3A, 0);
      analogWrite(motor4A, 255);
      delay(250);
      break;
 
    default:
   //do nothing
      analogWrite(motor1A, 0);
      analogWrite(motor2A, 0);
      analogWrite(motor3A, 0);
      analogWrite(motor4A, 0);
      break; 

  }
}

void processCommand(RemoteData data){
  if (data.vx == left_thrshld){
    //Serial.println('l');
    controlCar('l');
    
  }
  else if (data.vx == right_thrshld){
    //Serial.println('r');
    controlCar('r');
    
  }

  if (data.vy == down_thrshld){
    //Serial.println('d');
    controlCar('d');
  } 
  else if (data.vy == up_thrshld){
    //Serial.println('u');
    controlCar('u');
    
  }
  
  //stop car
  controlCar('x');
  
}

void printData(RemoteData r){
  //Serial.print("x: ");
  //Serial.println(r.vx);
  //Serial.print("y: ");
  //Serial.println(r.vy);
}
void loop() {
  if (radio.available()){
    RemoteData command = radio.readRemoteData();
    //printData(command);
    processCommand(command);
  }
   
}
