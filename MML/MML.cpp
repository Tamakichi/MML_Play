//
// MMLクラスライブラリ V1.0
//

#include "MML.h"

#define c_putch(c) Serial.write(c)
#define isBreak()  (false)

// note定義
const PROGMEM  uint16_t mml_scale[] = {
  4186,  // C
  4435,  // C#
  4699,  // D
  4978,  // D#
  5274,  // E
  5588,  // F
  5920,  // F#
  6272,  // G
  6643, // G#
  7040, // A
  7459, // A#
  7902, // B
};

// mml_scaleテーブルのインデックス
#define MML_C_BASE  0
#define MML_CS_BASE 1
#define MML_D_BASE  2
#define MML_DS_BASE 3
#define MML_E_BASE  4
#define MML_F_BASE  5
#define MML_FS_BASE 6
#define MML_G_BASE  7
#define MML_GS_BASE 8
#define MML_A_BASE  9
#define MML_AS_BASE 10
#define MML_B_BASE  11

const uint8_t mml_scaleBase[] = {
  MML_A_BASE, MML_B_BASE, MML_C_BASE, MML_D_BASE, MML_E_BASE, MML_F_BASE, MML_G_BASE,
};

void MML::init() {
  if (func_init != 0)
    func_init();
}

// TONE 周波数 [,音出し時間 [,ボリューム]]
// freq   周波数
// tm     音出し時間
// vol    音出し時間
void MML::tone(int16_t freq, int16_t tm,int16_t vol) {
  //dev_tone(freq, tm, vol);
  if (func_tone != 0) 
    func_tone(freq, tm, vol);
}

//　NOTONE
void MML::notone() {
  //dev_notone()
  if (func_notone != 0)
    func_notone();
}

// TEMPO テンポ
void MML::tempo(int16_t tempo) {
  if ( (tempo < MML_MIN_TMPO) || (tempo > MML_MAX_TMPO) ) 
    return;
  common_tempo = tempo;
}

// 長さ引数の評価
//  引数 
//   ptr(IN)  : 対象文字列格納アドレス
//   len(OUT) : 評価した長さ
//              正常  0,1,2,4,8,16,32,64 (0は長さ指定なし)
//              異常  -1 (数値エラー)
//  戻り値
//   次の文字列へのポインタ
//
const char* MML::getParamLen(const char *ptr, int16_t& len) {
  int16_t tmpLen = 0;
  const char* tmpPtr = ptr;
  while(isdigit(*ptr)) {
     if (flgdbug) c_putch(*ptr); // デバッグ
     tmpLen*= 10;
     tmpLen+= *ptr - '0';
     ptr++;
  }
  if (tmpPtr != ptr) {
    // 長さ引数ありの場合、長さを評価
    if ( !((tmpLen==1)||(tmpLen==2)||(tmpLen==4)||(tmpLen==8)||
         (tmpLen==16)||(tmpLen==32)||(tmpLen==64)) ) {
      // 長さ指定エラー
      tmpLen = -1;
      err = ERR_MML; 
    }
  }
  len = tmpLen;
  return ptr;
}

// PLAY 文字列
const char*  MML::play(const char* ptr, uint8_t flgTick) {
  uint16_t freq;                     // 周波数
  flgdbug = 0;                       // デバッグフラグ
  uint16_t local_len = common_len ;  // 個別長さ
  uint8_t  local_oct = common_oct ;  // 個別高さ
  
  int8_t  scale = 0;                 // 音階
  uint32_t duration;                 // 再生時間(msec)
  uint8_t flgExtlen = 0;
  uint8_t c;                         // 1文字取り出し用
  int16_t tmpLen;                    // 音の長さ評価用

  // MMLの評価
  while(*ptr) {
    if (flgdbug) c_putch(*ptr); // デバッグ
    
    flgExtlen = 0;
    local_len = common_len;
    local_oct = common_oct;
    
    //強制的な中断の判定
    if (isBreak()) {
      err = 0; 
      break;
      //return;
    }
    
    // 空白はスキップ    
    if (*ptr == ' '|| *ptr == '&') {
      ptr++;
      continue;
    }

    // デバッグコマンド
    if (*ptr == '?') {
      flgdbug = 1;
      ptr++;
      continue;
    }   

     c = toUpperCase(*ptr);
    if ( ((c >= 'A') && (c <= 'G')) || (c == 'R') ) {
      //**** 音階記号 A - Z,R 
      uint8_t flgR = 0;
      if (c == 'R') {
        flgR = 1;
        ptr++;
      } else {
        scale = pgm_read_byte(&mml_scaleBase[c-'A']); // 音階コードの取得        
        ptr++;
        if (*ptr == '#' || *ptr == '+') {
          //** 個別の音階半音上げ # or +
          if (flgdbug) c_putch(*ptr); // デバッグ
          // 半音上げる
          if (scale < MML_B_BASE) {
            scale++;
          } else {
            if (local_oct < MML_MAX_OCT) {
              scale = MML_B_BASE;
              local_oct++;
            }
          }
          ptr++;        
        } else if (*ptr == '-') {
          //** 個別の音階半音下げ # or +
          if (flgdbug) c_putch(*ptr); // デバッグ
          // 半音下げる
          if (scale > MML_C_BASE) {
            scale--;
          } else {
            if (local_oct > 1) {
              scale = MML_B_BASE;
              local_oct--;
            }
          }                
          ptr++;      
        } 
      }
      
      //** 個別の長さの指定 
      ptr = getParamLen(ptr, tmpLen);
      if (tmpLen < 0) {
        //return;
        err = ERR_MML; 
        break;
      } else if (tmpLen > 0) {
        local_len = tmpLen;
      }
      
      //** 半音伸ばし
      if (*ptr == '.') {
        if (flgdbug) c_putch(*ptr); // デバッグ
        ptr++;
        flgExtlen = 1;
      } 

      //** 音階の再生
      duration = 240000/common_tempo/local_len;  // 再生時間(msec)
      if (flgExtlen)duration += duration>>1;

      if (flgR) {
        // 休符
        if (flgTick) {
          playduration = duration;
          endTick = millis()+duration;
          break;
        } else {
          delay(duration); 
        }
      } else {
        // 音符
        freq = pgm_read_word(&mml_scale[scale])>>(MML_MAX_OCT-local_oct); // 再生周波数(Hz);  
        if (flgTick) {         
          playduration = duration;
          endTick = millis()+duration;
          tone(freq, 0, common_vol);                      // 音の再生(時間指定なし）
          break;
        } else {
          tone(freq, (uint16_t)duration, common_vol);     // 音の再生   
        }
      }
      
    } else if (c == 'L') {  // グローバルな長さの指定     
      //**** 省略時長さ指定 L[n][.] ***********************************************
      ptr++;
      ptr = getParamLen(ptr, tmpLen);
      if (tmpLen < 0) {
        //return;
        err = ERR_MML; 
        break;
      } else if (tmpLen > 0) {
        local_len = tmpLen;
        common_len = tmpLen;
      
        //** 半音伸ばし
        if (*ptr == '.') {
         if (flgdbug) c_putch(*ptr); // デバッグ
          ptr++;
          common_len += common_len>>1;
          local_len =  common_len;
        } 
      } else {
        // 引数省略時は、デフォルトを設定する
        common_len = MML_len;
        local_len =  MML_len;              
      }
    //**** ボリューム指定 Vn ***********************************************
    } else if (c == 'V') {  // グローバルなボリュームの指定     
      ptr++;
      uint16_t tmpVol =0;
      while(isdigit(*ptr)) {
         if (flgdbug) c_putch(*ptr); // デバッグ
         tmpVol*= 10;
         tmpVol+= *ptr - '0';
         ptr++;
      }
      if (tmpVol < 0 || tmpVol > MML_MAX_VOL) {
        err = ERR_MML; 
        //return;
        break;
      }
      common_vol = tmpVol;     
    //**** 音の高さ指定 On ***********************************************
    } else if (c == 'O') { // グローバルなオクターブの指定
      ptr++;
      uint16_t tmpOct =0;
      while(isdigit(*ptr)) {
         if (flgdbug) c_putch(*ptr); // デバッグ
         tmpOct*= 10;
         tmpOct+= *ptr - '0';
         ptr++;
      }
      if (tmpOct < 1 || tmpOct > MML_MAX_OCT) {
        err = ERR_MML; 
        //return;
        break;
      }
      common_oct = tmpOct;
      local_oct = tmpOct;
    } else if (c == '>') { // グローバルな1オクターブ上げる
      if (common_oct < MML_MAX_OCT) {
        common_oct++;
      }
      ptr++;
    //**** 1オクターブ下げる < ***********************************************
    } else if (c == '<') { // グローバルな1オクターブ下げる
      if (common_oct > 1) {
        common_oct--;
      }
      ptr++;
    //**** テンポ指定 Tn ***********************************************
    } else if (c == 'T') { // グローバルなテンポの指定
      ptr++;      
      //** 長さの指定
      uint32_t tmpTempo =0;
      const char* tmpPtr = ptr;
      while(isdigit(*ptr)) {
        if (flgdbug) c_putch(*ptr); // デバッグ
         tmpTempo*= 10;
         tmpTempo+= *ptr - '0';
         ptr++;
      }
      if (tmpPtr == ptr) {
        err = ERR_MML; 
        //return;
        break;
      }
      if (tmpTempo < MML_MIN_TMPO || tmpTempo > MML_MAX_TMPO) {
        err = ERR_MML; 
        return 0;                
      }
      common_tempo = tmpTempo;      
    } else {
      err = ERR_MML; 
      //return;
      break;
    }
  }
  return ptr;
}
