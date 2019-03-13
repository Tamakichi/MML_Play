#ifndef TimerEvent_h_
#define TimerEvent_h_

#include "Arduino.h"

// タイマーイベントクラス

class TimerEvent {
 private:
  uint32_t interval = 0;
  void (*handler)(void) = 0;
  uint8_t flgRun = 0;
  HardwareTimer* pTimer;

 public:
  // 初期化
  TimerEvent() { pTimer = &Timer1;};

  // ハードウェアタイマーの設定
  void setTimer(HardwareTimer* pt) {
    pTimer = pt;
  };

// ハードウェアタイマーの参照
  HardwareTimer*  getTimer(HardwareTimer* pt) {
    return pTimer;
  }
  
  void init(); 
  // 設定
  void set(uint32_t tm, void (*func)(void) ); 
  // 中断
  void stop();                            
  // 開始
  int16_t start();
  // 再開
  void resume();
  // 実行状態
  uint8_t isRun() { return flgRun;};
  
};

#endif
