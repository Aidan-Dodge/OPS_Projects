#include <Timer.h>
#include <TM1637Display.h>

Timer timer;
TM1637Display display(12, 11);

void setup() {
  pinMode(2, INPUT_PULLUP); // start/pause/resume button
  pinMode(3, INPUT_PULLUP); // stop button

  attachInterrupt(digitalPinToInterrupt(2), ISR_D2, FALLING);
  attachInterrupt(digitalPinToInterrupt(3), ISR_D3, FALLING);

  display.clear();
  display.setBrightness(4);
  display.showNumberDecEx(0, 0x40, true);
}

void ISR_D2() //start/pause/resume
{
  switch(timer.state())
  {
    case RUNNING : timer.pause();
                    break;

    case PAUSED : timer.resume();
                    break;
    case STOPPED : timer.start();
                    break;
  }

  tone(6, 523, 250); //C5

}

void ISR_D3() //stop
{
  timer.stop();
  tone(6, 659, 300); // E5
}
void loop() {
  while (timer.state() == RUNNING)
  {
    uint32_t  time_ms = timer.read();
    uint32_t  time_s = floor(time_ms / 1000);
    uint32_t  time_min = floor(time_s / 60);
    uint32_t  time_leftover_s = floor(time_s % 60);

    if(time_s % 60 == 0)
    {
      tone(6, 783, 650); // G5
    }

    uint32_t  timer_send = (time_min * 100) + time_leftover_s;

    //check if timer reaches max value
    if (timer_send == 9999)
    {
      timer.stop();
      timer.start();
    }
    else{
      display.showNumberDecEx(timer_send, 0x40, true);
    }
  }

}
