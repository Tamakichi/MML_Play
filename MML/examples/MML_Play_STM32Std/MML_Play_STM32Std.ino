//
// MML演奏サンプル Arduino STN32 標準Tone()関数利用版
//

#include "MML.h"
#include "TimerEvent.h"

#define   TonePin PB9 // 圧電スピーカー接続ピン
#define   StopBtn PB8 // フォアグランド演奏中断ボタン

TimerEvent ticker;   // タイマー割り込み管理
MML mml;             // MML文演奏管理

// デバイス初期化関数
void dev_toneInit() {
}

// 単音出力関数
void dev_tone(uint16_t freq, uint16_t tm, uint16_t vol) {
  tone(TonePin,freq);
  if (tm) {
    delay(tm);
    noTone(TonePin);
  }
}

// 単音出力停止関数
void dev_notone() {
  noTone(TonePin);
}

// バックグラウンド演奏割り込み
void handle_timer() {
  if (mml.isBGMPlay())
    if (mml.available()) 
      mml.playTick();    
}

// 猫ふんじゃった
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
  ;

// デバッグ出力用
void debug(uint8_t c) {
  Serial.write(c);
}

// フォアグランド演奏の停止
void OnStopkey() {
  if (mml.isPlay()) {
    mml.stop();
    Serial.println("Stop foreground playing");
  }
}

void setup() {
  Serial.begin(115200);

  // フォアグランド演奏停止用ボタンの設定
  pinMode(StopBtn,INPUT_PULLUP);
  attachInterrupt(StopBtn, OnStopkey, FALLING);

  Serial.println("MML library sample. Hit any key to start.");    
      
  // キーボード入力待ち
  while (!Serial.available())
    continue;
  
  // MML初期化、デバイス依存関数の登録
  mml.init(dev_toneInit, dev_tone, dev_notone, debug); 

  // フォアグランド演奏
  Serial.println("Now foreground playing ..");
  mml.setText(mmltext);
  mml.play();
  detachInterrupt(StopBtn);

  // タイマー割り込み設定
  ticker.set(10, handle_timer);
  ticker.setPriority(14);
  ticker.start();

  delay(1000);

  // バックグラウンド演奏 
  Serial.println();
  Serial.println("Now Background playing ..");
  Serial.println("Menu: e:end, r:resume,  s:start"); 
  mml.playBGM();
}

void loop() {
   // バックグラウンド演奏中の操作 
  if (Serial.available()) {
    uint8_t c = Serial.read();
    if (c == 'e' || c== 'E') {        // 演奏停止
      mml.stop();
    } else if (c == 'r' || c== 'R') { // 演奏再開
      mml.resume();
    } else if (c == 's' || c== 'S') { // 最初から演奏
      mml.stop();
      delay(15);
      mml.playBGM();
    }
  }
}
