//
// MMLクラスライブラリ V1.0
//

#ifndef ___MML_h___
#define ___MML_h___

#include "Arduino.h"

// エラーメッセージ定義
#define ERR_MML 1

// デフォルト設定値
#define MML_tempo     120 // テンポ(32～255)
#define MML_len       4   // 長さ(1,2,4,8,16,32)
#define MML_oct       4   // 音の高さ(1～8)
#define MML_vol       15  // 音の大きさ(1～15)

// 引数範囲
#define MML_MAX_VOL  15   // 音の大きさ(1～15)
#define MML_MAX_OCT  8    // 音の高さ(1～8)
#define MML_MAX_TMPO 255  //  テンポ(32～255)
#define MML_MIN_TMPO 32   //  テンポ(32～255)

class MML {
  private:
    uint8_t   err;                          // Error message index
    uint16_t  common_tempo = MML_tempo ;    // 共通テンポ
    uint16_t  common_len   = MML_len   ;    // 共通長さ
    uint8_t   common_oct   = MML_oct   ;    // 共通高さ
    uint8_t   common_vol   = MML_vol   ;    // 音の大きさ
    uint8_t   flgdbug;                      // デバッグ出力フラグ

    char      *text;                        // MML文テキスト
    char      *mml_ptr;                     // 再生位置
    uint16_t  playduration;                 // 処理中の音符・休符の長さ（ミリ秒）
    uint32_t  endTick;                      // 再生終了システム時間（ミリ秒）

    void  (*func_tone)(uint16_t freq, uint16_t tm, uint16_t vol);
    void  (*func_notone)(void);
    void  (*func_init)(void);
  
  private:
    const char* getParamLen(const char *ptr, int16_t& len);
  public:
    void  attach_init(void (*func)(void)) { func_init = func; };
    void  attach_tone(void (*func)(uint16_t freq, uint16_t tm, uint16_t vol)) { func_tone = func; };
    void  attach_notone(void (*func)(void)) { func_notone = func; }
    void init();
    void init(void (*f1)(void),void (*f2)(uint16_t freq, uint16_t tm, uint16_t vol),void (*f3)(void)) 
    { func_init = f1; func_tone = f2; func_notone = f3; init(); }; 
   
    void tone(int16_t freq, int16_t tm = 0,int16_t vol=15);
    void notone();
    void tempo(int16_t tempo);
    const char* play(const char* ptr, uint8_t flgTick = 0);
    void setText(const char* text);
    uint8_t isPlay();

};

#endif
