//
// MML演奏サンプル for M5Stack 2020/01/25
//

#include <M5Stack.h>
#include "MML.h"

MML mml;             // MML文演奏管理

// デバイス初期化関数
void dev_toneInit() {
}

// toneEx
//  引数 
//   frequency (Hz)
//   vol (0 ～ 9、0:無音 9:最大)
void toneEx(uint16_t frequency, uint16_t vol) {
  vol=1;
  ledcSetup(TONE_PIN_CHANNEL, frequency, 10);
  ledcWrite(TONE_PIN_CHANNEL,0x1FF>>(9-vol));
}

void noToneEx() {
  ledcWriteTone(TONE_PIN_CHANNEL, 0);
  digitalWrite(SPEAKER_PIN, 0);
}

// 単音出力関数
void dev_tone(uint16_t freq, uint16_t tm, uint16_t vol) {
   toneEx(freq, vol);
  if (tm) {
    delay(tm);
    noToneEx();
  }
}

// 単音出力停止関数
void dev_notone() {
  noToneEx();
}

// 猫ふんじゃった
const char * mmltext =
  "v1?O6t160v15l16d+c+r8f+rf+rd+c+r8f+rf+rd+c+l8rf+r"
  "f+rl16frfrd+c+r8frfrd+c+r8frfrd+c+l8"
  "rfrfrl16f+rf+rd+c+r8f+rf+rd+c+r8f+r"
  "f+rd+c+l8rf+rf+rl16frfrd+c+r8frfrd+c+r8fr"
  "frd+c+l8rfrfrl16f+rf+rd+c+l8rf+rf+rf+r"
  "f+rf+rf+rl16frfrd+c+l8rfrfrfrfrfrfrl16"
  "f+rf+rd+c+r8f+rf+rd+c+r8f+rf+rd+c+l8r"
  "f+rf+rl16frfrd+c+r8frfrd+c+r8frfrd+c+l8"
  "rfrfrl16f+rf+r8.f+rc+c+d8c+8.frf+?"
  ;
  
// デバッグ出力用
void debug(uint8_t c) {
  Serial.write(c);
}

void setup() {
  M5.begin(true, false, true);
  M5.Power.begin();
  Serial.begin(115200);
    
  ledcSetup(TONE_PIN_CHANNEL, 0, 10);
  ledcAttachPin(SPEAKER_PIN, TONE_PIN_CHANNEL);
      
  // MML初期化、デバイス依存関数の登録
  mml.init(dev_toneInit, dev_tone, dev_notone, debug); 
  mml.setText(mmltext);

  M5.Lcd.clear(BLACK);
  M5.Lcd.setTextSize(3);
  M5.Lcd.setCursor(40, 10);  M5.Lcd.println("MML play");
  M5.Lcd.setCursor(50, 36);  M5.Lcd.println("[A] Start");
  M5.Lcd.setCursor(50, 62);  M5.Lcd.println("[B] Stop");
  M5.Lcd.setCursor(50, 88);  M5.Lcd.println("[C] Resume");
}

void loop() {
  M5.update(); 
  
  if (M5.BtnA.wasReleased()) { // 左ボタン
     // 最初から演奏
     mml.stop();
     delay(15);
     mml.playBGM();
  }
  if (M5.BtnB.wasReleased()) { // 中ボタン
     // 演奏停止
     mml.stop();
  }
  if (M5.BtnC.wasReleased()) { // 右ボタン
     // 演奏再開
     mml.resume();
  }

  if (mml.isBGMPlay()) {
    // 演奏状態で演奏継続可能なら１音再生
    if (mml.available()) 
      mml.playTick();    
  }
}
