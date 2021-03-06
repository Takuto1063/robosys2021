# robosys2021 kadai1
ロボットシステム学の課題で作成・改良したデバイスドライバ
# 概要
・数字1～5の入力に応じてLEDをそれぞれ光らせる

・数字6を入力した場合、赤色のLEDから順々にLEDを光らせる
# 環境・使用器具
[Ubuntu 18.04]

・Raspberry Pi 4 

・ブレッドボード

・LED 赤 ×1  白 ×4

・抵抗（200Ω) ×5

・ジャンパー線　×12
# 回路
![画像名](https://github.com/Takuto1063/robosys2021/blob/main/%E3%83%AD%E3%83%9C%E3%82%B7%E3%82%B9%E8%AA%B2%E9%A1%8C1.jpg)

（gpio25,21,22,23,26 - 抵抗 - LED - GND) 
# 実行方法
**インストール**

以下の手順で操作を行う

`$ git clone https://github.com/Takuto1063/robosys2021.git`

`$ make`

`$ sudo insmod myled.ko`

`$ sudo chmod 666 /dev/myled0`

**LEDの点灯・消灯**

LEDをそれぞれ点灯させる

`$ echo 1 > /dev/myled0`

`$ echo 2 > /dev/myled0`

`$ echo 3 > /dev/myled0`

`$ echo 4 > /dev/myled0`

`$ echo 5 > /dev/myled0`

LEDを順々に点灯させる

`$ echo 6 > /dev/myled0`

LEDを消灯させる

`$ echo > /dev/myled0`

**アンインストール**

以下の手順で操作を行う

`$ sudo rmmod myled`
`$ make clean`

# 動画URL
[https://youtu.be/v1frLeisvNY](https://youtu.be/v1frLeisvNY)
# ライセンス
[GNU General Public License v3.0](https://github.com/Takuto1063/robosys2021/blob/main/COPYING)
