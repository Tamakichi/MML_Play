#include "TimerEvent.h"

void TimerEvent::init() {
  pTimer->pause();                   // タイマー停止
}

// ハンドラの設定
// 引数
//  tm   : 割り込み間隔(msec)
//  func : ハンドラ(関数のポインタ)
void TimerEvent::set(uint32_t tm, void (*func)(void) ) {
  interval = tm;
  handler = func;
}

int16_t TimerEvent::start() {
  if (!handler) 
    return 1; // ハンドラ未登録
  if (!interval)
    return 0; // 間隔が0
  
  pTimer->attachInterrupt(           // 割り込みハンドラの登録
    TIMER_UPDATE_INTERRUPT,         // 呼び出し条件は、カウンターオーバーフロー更新時
    handler                         // 呼び出す関数
  );    
  pTimer->setPrescaleFactor(720000); // システムクロック 72MHzを1ｋHzに分周
  pTimer->setOverflow(interval);     // 割り込み間隔（msec)
  pTimer->setCount(0);               // カウンタを0に設定
  pTimer->refresh();                 // タイマ更新
  pTimer->resume();                  // タイマースタート
  flgRun = 1;
  return 0;
}

// タイマー停止
void TimerEvent::stop() { 
    pTimer->pause();                 
    flgRun = 0; 
}

// タイマ再開
void TimerEvent::resume() { 
  pTimer->resume();
  flgRun = 1;
}
