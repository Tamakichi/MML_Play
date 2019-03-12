#include "MML.h"
#include "src/LIB/sound.h"

const char * mmltext =

  "?O6t180v15l16d+c+r8f+rf+rd+c+r8f+rf+rd+c+l8rf+r"
  "f+rl16frfrd+c+r8frfrd+c+r8frfrd+c+l8"
  "rfrfrl16f+rf+rd+c+r8f+rf+rd+c+r8f+r"
  "f+rd+c+l8rf+rf+rl16frfrd+c+r8frfrd+c+r8fr"
  "frd+c+l8rfrfrl16f+rf+rd+c+l8rf+rf+rf+r"
  "f+rf+rf+rl16frfrd+c+l8rfrfrfrfrfrfrl16"
  "f+rf+rd+c+r8f+rf+rd+c+r8f+rf+rd+c+l8r"
  "f+rf+rl16frfrd+c+r8frfrd+c+r8frfrd+c+l8"
  "rfrfrl16f+rf+r8.f+rc+c+d8c+8.frf+?"

 // "L1CL2DL4EL8FL16GL32AL64BL4C"
  ;

void debug(uint8_t c) {
  Serial.write(c);
}

void setup() {
  Serial.begin(115200);
    while (!Serial.available())
        continue;
  
  Serial.println("Start.");
  char *ptr = (char *)mmltext;

  MML mml;
  mml.init(dev_toneInit, dev_tone, dev_notone, debug);
  mml.setText(mmltext);
  mml.play();
  
/*  
  do {
    if (!mml.isPlay()) 
      ptr = (char *)mml.play((char *)ptr,1);
    else 
      delay(10);
  } while (*ptr || mml.isPlay());
*/
  Serial.println("End.");
}

void loop() {
  // put your main code here, to run repeatedly:

}
