# MML_Play

Arduino、Arduino STM32 MML文演奏クラスライブラリ  

## ライブラリ名称

MML (ヘッダーファイル MML.h)  

主な機能

- 単音MML文の演奏
- フォアグランド演奏(play)
- バックグラウンド演奏支援機能(playBGM、playTick)
- 支援ライブラリ

## インストール

本パッケージをダウンロードし、解凍したフォルダ内の **MML**フォルダを  
各自のArduino環境のライブラリ配置フォルダ **libraries** に配置して下さい。  
( **環境設定** - **スケッチの保存場所** に設定のフォルダ の  **libraries** に配置）

## ライブラリリファレンス

本ライブラリはクラスライブラリとして実装しています。  

### クラス名

MML

### ヘッダーファイル

`#include "MML.h"`  

### パブリックメンバー関数

#### ■ コンストラクタ

【書式】  
`MML()`  

【引数】  
なし

【戻り値】  
なし

【説明】  
**MML** クラスのインスタンスを作成します。  
コンストラクタでは、ハードウェア資源の確保・初期化は行いません。  
別途、`init()`関数にてハードウェア資源の確保・初期化を行う必要があります。  

【記述例】  
`MML mml;`  

#### ■ 初期化

【書式】  
`void init(`  
`void (*func_init)(void),`  
`void (*func_tone(uint16_t freq, uint16_t tm, uint16_t vol),`  
`void (*func_notone)(void),`  
`void (*func_debug)(uint8_t c)=0)`  
`)`

【引数】  
`func_init`：  単音出力デバイス初期化関数へのポインタ  
`func_tone`：  単音出力関数へのポインタ  
`func_notone`：単音出力停止関数へのポインタ  
`func_debug`： デバッグ出力関数へのポインタ  

【戻り値】  
なし

【説明】  
単音出力制御を行う関数登録、単音出力デバイスの初期化を行います。  
`func_init`は、単音出力を行うためのデバイス初期化を行う関数へのポインタです。  

`init()`関数では、デバイス初期化処理としてこの`func_init`を呼び出します。  
`func_tone`は単音出力を行う関数へのポインタです。  
単音出力時に引数として、
`freq`:周波数(Hz)、`tm`：音長（ミリ秒）`vol`:音量(0～15)  
が渡されます。  

`func_notone`は単音出力を停止する関数へのポインタです。  

`func_debug`はデバッグ出力関数へのポインタです。引数`c`はMML文処理時の文字数が  
渡されます。本関数の登録は省略可能です。  

【記述例】  
Arduinoの`tone()`、`notone()`関数を使った単音出力初期化実装例  

```CPP:sample.cpp
#define   TonePin 8
MML mml;


// デバイス初期化関数
void dev_init() {

}

// 単音出力関数
void dev_tone(uint16_t freq, uint16_t tm, uint16_t vol) {
    tone(TonePin,freq,tm);
}

// 単音出力停止関数
void dev_notone() {
    noTone(TonePin);
}

// デバッグ出力関数
void dev_debug(uint8_t c) {
    Serial.write(c);
}

// MML初期化、デバイス依存関数の登録
mml.init(dev_init, dev_tone, dev_notone, dev_debug);  
```

#### ■ MML文の登録

【書式】  
`void setText(const char* text)`

【引数】  
`text`：MML文文字列へのポインタ  

【戻り値】  
 なし

【説明】  
演奏するMML文を登録します。  
演奏中にMML文を登録した場合は、動作を保証しません。  

MML文には次のコマンドを利用することが出来ます。  

- 音階記号 `[#|+|-][長さ][.]`  
音階指定：C、D、E、F、G、A、B または c、d、e、f、g、a、b  
順番に ド、レ、ミ、ファ、ソ、ラ、シ、ド の音階に対応します。  

  - `#`：半音上げる（省略可能）
  - `+`：半音上げる（省略可能）
  - `-`：半音下げる（省略可能）
    - 長さ：1、2、4、8、16、32、64  
    (省略時はLによる長さ、デフォルト値 ４)、省略可能  
    1は全音符、２は2分音符、4は四分音符、64は64分の一音符  
    - `.`：長さを半分伸ばす

- `R[長さ][.]`：休符
  - 長さ：1、2、4、8、16、32、64  
  (省略時はLによる長さ、デフォルト値 ４)、省略可能  
  1は全音符、２は2分音符、4は四分音符、64は64分の一音符  
    - `.`：長さを半分伸ばす  

- `L<長さ>[.]` ： 音の長さ指定  
音階記号の長さ省略時の長さの設定。設定前初期値は4(四分音符)
  - 長さ： 1、2、4、8、16、32、64  
  (省略時はLによる長さ、デフォルト値 ４)、省略可能  
  1は全音符、２は2分音符、4は四分音符、64は64分の一音符  
  - `.`：長さを半分伸ばす

- `O<オクターブ>`: 音の高さを指定  
オクターブ：1～8 初期値は4  
- `<`：１オクターブ下げる  
- `>`：１オクターブ上げる  
- `T<テンポ>`：テンポを指定  
初期値は120、32～255

- `V<音の大きさ>`：ボリューム設定  
初期値は15、0(無音) ～ 15(最大)  
PWMのパルス幅で音の大きさを簡易的に再現しています。  
- `?`：デバッグON  
演奏中の音階記号を表示します。  
PLAYコマンド事に指定する必要があります。  
- 空白文字  
何もせずスキップします。

【記述例】  

```CPP:sample.cpp
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

MML mml;  
<中略>
void setup() {
    <中略>
    mml.setText(mmltext);
    <中略>
}
```

#### ■ 演奏テンポの設定

【書式】  
`void tempo(int16_t tempo)`  

【引数】  
`tempo`：テンポ 32～255（デフォルト 120）  

【戻り値】  
なし

【説明】  
演奏テンポを設定します。  
ここで指定するテンポとは、1分間に4分音符を演奏する回数です。  
120の場合は、1分間に4分音符を120回演奏する速度となります。  
値が大きいほど、演奏が早くなります。  
テンポはMML文の`T`コマンドでも設定できます。

【記述例】  

```CPP:sample.cpp
mml.tempo(150);
```

#### ■ MML文１音演奏

【書式】  
`void playTick(uint8_t flgTick = true)`  

【引数】  
`flgTick`：再生モード  

- `false` １音再生完了待ちを行う  
- `true`  １音再生完了待ちを行わない  

【戻り値】  
なし

【説明】  
バックグラウンドモードでの演奏のための、１音分の演奏を行います。  
バックグラウンドモードは、`playBGM()`で行います。  
`playTick()`を呼び出す都度、MML文から１音づつ演奏することが出来ます。  
割り込み処理から、本関数を呼び出すことで、バックグラウンド演奏を可能にします。

登録したMML文のコマンド解釈位置からコマンドを取り出し、１音分演奏します。  
演奏後は、コマンド解釈位置を次に移動します。  
コマンド解釈位置が文末に達している場合は、再生を行いません。  

引数`flgTick`に`false`を指定している場合は、１音演奏完了待ちを行います。  

引数`flgTick`に`true`を指定している場合は、１音演奏完了待ちを行いません。  
この場合、演奏完了を待たずに即時復帰します。  
次の音を演奏する場合は`available()`関数にて演奏状態をチェックしてから演奏します。  

【記述例】  

```CPP:sample.cpp
#include "MML.h"

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

// デバイス初期化関数
void dev_init() { }

// 単音出力関数
void dev_tone(uint16_t freq, uint16_t tm, uint16_t vol) {
    tone(freq,tm);
}

// 単音出力停止関数
void dev_notone() {
    notone();
}

void setup() {
    MML mml;
    mml.init(dev_toneInit, dev_tone, dev_notone);
    mml.setText(mmltext);
    mml.playBGM();

    // 1音づつ再生
    while(mml.isBGMPlay()) {
        if (mml.available()) {
        mml.playTick();
        }
    }
}

void loop() {}
```

#### ■ MML文の演奏開始

【書式】  
`void play(uint8_t mode = 0)`  

【引数】  
`mode`：再生モード、下位3ビットにて演奏モードを指定します。  

- 第0ビット
  - 0:フォアグランド演奏
  - 1 or `MML_BGM` :バックグラウンド演奏
- 第1ビット
  - 0:先頭から
  - 1 or MML_RESUME:中断途中から  
- 第2ビット
  - 0:リピートなし
  - 1 or MML_REPEAT:リピートあり(バックグラウンド演奏時のみ）  

【戻り値】  
なし

【説明】  
登録したMML文の演奏を開始します。  
引数の指定を省略した場合は、フォアグランド演奏を行います。  

#### ■ MML文のバックグラウンド演奏開始

【書式】  
`void playBGM(uint8_t mode = 0)`  

【引数】  
`mode`：再生モード、下位3ビットにて演奏モードを指定します。  
`play()`関数と同じですが、第0ビットが1固定となります。

- 第0ビット
  - 0:フォアグランド演奏
  - 1 or `MML_BGM` :バックグラウンド演奏

  本設定は無視され、常に1（バックグラウンド演奏）となります。  

- 第1ビット
  - 0:先頭から
  - 1 or MML_RESUME:中断途中から  
- 第2ビット
  - 0:リピートなし
  - 1 or MML_REPEAT:リピートあり(バックグラウンド演奏時のみ）  

【戻り値】  
なし

【説明】  
登録したMML文のバックグラウンド演奏を開始します。  
内部的に`play()`を呼び出しています。

#### ■ バックグラウンド演奏状態の確認

【書式】  
`uint8_t isBGMPlay()`  

【引数】  
なし

【戻り値】  
演奏中の場合は`1`、そうでない場合は`0`を返します。

【説明】  

#### ■ フォアグランド演奏状態の確認

【書式】  
`uint8_t isPlay()`  

【引数】  
なし

【戻り値】  
演奏中の場合は`1`、そうでない場合は`0`を返します。

【説明】  

#### ■ １音演奏可能の確認

【書式】  
`uint8_t available()`  

【引数】  
なし

【戻り値】  
演奏可能な場合は`1`、そうでない場合は`0`を返します。

【説明】  

#### ■ 演奏の中断

【書式】  
`void stop()`  

【引数】  
なし

【戻り値】  
なし

【説明】  
演奏を中断します。  
`resume()`関数にて演奏の再開が可能です。  

#### ■ 演奏の再開

【書式】  
`void resume()`  

【引数】  
なし

【戻り値】  
なし

【説明】  
`stop()`関数にて中断した演奏を再開します。  

## サンプルプログラム

### Arduino Uno用サンプルプログラム

#### MML_PLAY.ino

フォアグランド演奏後、バックグラウンド演奏を行うサンプルプログラムです。  
スケッチのコンパイルには、**TimerOne**ライブラリが別途必要です。  

```CPP:MML_PLAY.ino
//
// MML演奏サンプル Arduino版
//

#include <TimerOne.h>
#include "MML.h"

#define   TonePin 8 // 圧電スピーカー接続ピン
#define   StopBtn 3 // フォアグランド演奏中断ボタン

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
void OnStopPkay() {
  if (mml.isPlay()) {
    mml.stop();
    Serial.println("Stop foreground playing");
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(13, OUTPUT);

  // フォアグランド演奏停止用ボタンの設定
  pinMode(StopBtn,INPUT_PULLUP);
  attachInterrupt(StopBtn, OnStopPkay, FALLING);
  
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
  Timer1.initialize(10000);
  Timer1.attachInterrupt(handle_timer);

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
```

### Arduino STM32用サンプルプログラム

#### MML_Play_STM32Std.ino

Blue Pillボード（STM32F103C8T6搭載)用のサンプルプログラムです。
Arduino Uno版と同等の機能です。  

```CPP:MML_Play_STM32Std.ino
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
  detachInterrupt(PB8);

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
```
#### MML_Play_STM32Ex.ino

Blue Pillボード（STM32F103C8T6搭載)用のサンプルプログラムです。
Arduino Uno版と同等の機能ですが、tone()関数は音の大きさ指定対応のものに置き換えています。  
詳細については、スケッチを参照して下さい。  


