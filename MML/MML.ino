#include "MML.h"
#include "src/LIB/sound.h"
#include "TimerEvent.h"

TimerEvent ticker;
MML mml;

#define LED_PIN  PC13

uint8_t sw = LOW;
void handle_timer() {
  if (mml.isPlay())
    if (mml.available()) 
      mml.playTick();    
}

const char * mmltext =
  "O6t180v15l16d+c+r8f+rf+rd+c+r8f+rf+rd+c+l8rf+r"
  "f+rl16frfrd+c+r8frfrd+c+r8frfrd+c+l8"
  "rfrfrl16f+rf+rd+c+r8f+rf+rd+c+r8f+r"
  "f+rd+c+l8rf+rf+rl16frfrd+c+r8frfrd+c+r8fr"
  "frd+c+l8rfrfrl16f+rf+rd+c+l8rf+rf+rf+r"
  "f+rf+rf+rl16frfrd+c+l8rfrfrfrfrfrfrl16"
  "f+rf+rd+c+r8f+rf+rd+c+r8f+rf+rd+c+l8r"
  "f+rf+rl16frfrd+c+r8frfrd+c+r8frfrd+c+l8"
  "rfrfrl16f+rf+r8.f+rc+c+d8c+8.frf+"

 // "L1CL2DL4EL8FL16GL32AL64BL4C"
  ;

void debug(uint8_t c) {
  Serial.write(c);
}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(115200);
  while (!Serial.available())
    continue;
  Serial.println("Start.");

  // フォアグランド演奏
  mml.init(dev_toneInit, dev_tone, dev_notone, debug);
  mml.setText(mmltext);
  //mml.play();

  // バックグラウンド演奏 
  mml.playBGM();
  ticker.init();
  ticker.set(10, handle_timer);
  ticker.start();
  Serial.println("End.");
}

void loop() {
  if (Serial.available()) {
    uint8_t c = Serial.read();
    if (c == 'e') {
      mml.stop();
    } else if (c == 'r') {
      mml.resume();
    } else if (c == 's') {
      mml.stop();
      delay(15);
      mml.playBGM();
    }
  }
}
