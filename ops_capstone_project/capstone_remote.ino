#include <NRF24L01_W.h>
/*
Initially my RF24 library had updated and I tried to run this with the RF24
class since the NRF24L01_W was not working, but I switched back to this older
version. Leaving the old code for now incase I want to use it later. It should
all be commented out.
*/

//RF24 radio(7, 8);
NRF24L01_W radio(7,8);
int left_thrshld = 450;
int down_thrshld = 450;
int right_thrshld = 750;
int up_thrshld = 750;

uint8_t address[2][6] = {"REMOTE", "RCCAR"};
RemoteData message;

void radio_init(){
  if (!radio.begin()) {
    Serial.println("notready");
    //wait until radio ready
  while (1) {} 
}
  radio.setReceiveAddress("Remot");
  radio.setTransmitAddress("RCcar");
  // radio.openWritingPipe(address[0]);
  //radio.openReadingPipe(0, address[1]);
  radio.setChannel(44); 
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  radio_init();

}

RemoteData readJoyStick(){
  int y = analogRead(A0);
  int x = analogRead(A1);
  /*
  Serial.print("x ");
  Serial.print(x);
  Serial.print(" y ");
  Serial.println(y);
  */
  char command;
  RemoteData data;
  data.vx =x;
  data.vy =y;

  /*
  if (x < left_thrshld){
    command = 'l';
  }
  else if (x > right_thrshld){
    command = 'r';
  }

  else if (y < down_thrshld){
    command = 'd';
  }
  else if (y > up_thrshld){
    command = 'u';
  }
  else{
    command = 'X';
  }
  */

  return data;
}

void sendMessage(RemoteData r){
  Serial.print("x: ");
  Serial.println(r.vx);
  Serial.print("y: ");
  Serial.println(r.vy);
  radio.write(r);
}
void loop() {
  // put your main code here, to run repeatedly:
  message = readJoyStick();
  sendMessage(message);

}
