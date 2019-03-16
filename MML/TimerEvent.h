//
// file : TimerEvent.h
// タイマーイベントライブラリ
// 作成日 2019/03/16 by たま吉さん
// 

#ifndef TimerEvent_h_
#define TimerEvent_h_

#include "Arduino.h"

class TimerEvent {
 private:
  uint32_t interval = 0;
  void (*handler)(void) = 0;
  uint8_t flgRun = 0;
  HardwareTimer* pTimer;

 public:
 
  // 初期化
  TimerEvent(HardwareTimer& pt) { setTimer(pt); };
  TimerEvent() { setTimer(Timer1);};

  // ハードウェアタイマーの設定
  void setTimer(HardwareTimer& pt) {
    pTimer = &pt;
    init();
  };

// ハードウェアタイマーの参照
  HardwareTimer*  getTimer(HardwareTimer* pt) {
    return pTimer;
  }

  // 初期化
  void init() {
      pTimer->pause();  // タイマー停止
  }
  // 設定
  void set(uint32_t tm, void (*func)(void) ); 
  // 中断
  void stop() {  pTimer->pause();  flgRun = 0; };
  // 開始
  int16_t start();
  // 再開
  void resume() {  pTimer->resume();  flgRun = 1; };
  // 実行状態
  uint8_t isRun() { return flgRun;};

  // 割り込み優先レベル設定
  void setPriority(uint8_t);
};

#endif
