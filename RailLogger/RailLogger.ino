//--------------------------------------------------------------------------------
//
// Train Rail Logger スケッチ
// Copyright(C)'2021 Ayanosuke(Maison de DCC)
// [RaillLogger.ino]
//
// 線路の電圧を波形化します
//
// Arduino NANO に SSD1306 SPI 4pinバージョン(CSなし)を接続
//
// ライブラリを管理から以下２つのライブラリをインストール
// Adafruit GFX ライブラリを使用
// Adafruit SSD1306 ライブラリを使用
//
// http://maison-dcc.sblo.jp/ http://dcc.client.jp/ http://ayabu.blog.shinobi.jp/
// https://twitter.com/masashi_214
//
// DCC電子工作連合のメンバーです
// https://desktopstation.net/tmi/ https://desktopstation.net/bb/index.php
//
// This software is released under the MIT License.
// http://opensource.org/licenses/mit-license.php
//
// 2021/08/22 初版作成
//--------------------------------------------------------------------------------

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for SSD1306 display connected using software SPI (default case):
#define OLED_MOSI   9
#define OLED_CLK    8
#define OLED_DC    12
#define OLED_CS    10     // Dummy
#define OLED_RESET 11

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

#define AIN         5     // Rail電圧測定ポート

void setup() {
  Serial.begin(115200);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  VoltLogger();
}

//
// loop() は使いません
//
void loop() {
}

void VoltLogger(void) {
  unsigned char dot[128];
  unsigned char i;

  display.clearDisplay();

  for(;;){
    dot[127] = 64 - ( analogRead( AIN ) / 16);
//  Serial.println(dot[127]);

    // 前回の波形を削除(バッファ内書き換え）
    for( i = 0 ; i <= 126 ; i++ ){
      display.drawLine( i, dot[i], i+1, dot[i+1], BLACK );
      dot[i] = dot[i+1];
    }
    // 最新の波形を描画(バッファ内書き換え）
    for( i = 0 ; i <= 125 ; i++ ){
      display.drawLine( i, dot[i], i+1, dot[i+1], WHITE );
    }
    // OLEDを最新状態に書き換え
    display.display();
  }
}
