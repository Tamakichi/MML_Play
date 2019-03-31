//
// MML演奏サンプル
//

#include "MML.h"
#include "sound.h"
#include "TimerEvent.h"

TimerEvent ticker;   // タイマー割り込み管理
MML mml;             // MML文演奏管理

// バックグラウンド演奏割り込み
void handle_timer() {
  if (mml.isBGMPlay())
    if (mml.available()) 
      mml.playTick();    
}

// 猫ふんじゃった
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
  ;

// デバッグ出力用
void debug(uint8_t c) {
  Serial.write(c);
}

// フォアグランド演奏の停止
void OnStopPkay() {
  if (mml.isPlay()) {
    mml.stop();
    Serial.println("Stop foreground playing");
  }
}

void setup() {
  Serial.begin(115200);

  // フォアグランド演奏停止用ボタンの設定
  pinMode(PB8,INPUT_PULLUP);
  attachInterrupt(PB8, OnStopPkay, FALLING);
    
  // キーボード入力待ち
  while (!Serial.available())
    continue;
  
  // MML初期化、デバイス依存関数の登録
  mml.init(dev_toneInit, dev_tone, dev_notone, debug); 

  // タイマー割り込み設定
  ticker.set(10, handle_timer);
  ticker.setPriority(14);
  ticker.start();

  // フォアグランド演奏
  Serial.println("Now foreground playing ..");
  mml.setText(mmltext);
  mml.play();
  detachInterrupt(PB8);
  delay(1000);

  // バックグラウンド演奏 
  Serial.println("Now Background playing ..");
  Serial.println("Menu: e:end, r:resume,  s:start"); 
  mml.playBGM();
}

void loop() {
   // バックグラウンド演奏中の操作 
  if (Serial.available()) {
    uint8_t c = Serial.read();
    if (c == 'e') {        // 演奏停止
      mml.stop();
    } else if (c == 'r') { // 演奏再開
      mml.resume();
    } else if (c == 's') { // 最初から演奏
      mml.stop();
      delay(15);
      mml.playBGM();
    }
  }
}
