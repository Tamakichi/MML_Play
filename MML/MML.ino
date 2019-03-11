#include "MML.h"
#include "src/LIB/sound.h"

const char * mmltext =

  "?t200v5l16d+c+r8f+rf+rd+c+r8f+rf+rd+c+l8rf+r"
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

void setup() {
  Serial.begin(115200);
  Serial.println("Start.");

  MML mml;
  mml.init(dev_toneInit, dev_tone, dev_notone);
  mml.play((char *)mmltext);
}

void loop() {
  // put your main code here, to run repeatedly:

}
