LODAS(LOgic Board for Digital Audio System)のESP-WROOM-32モジュールにバイナリファイルを書き込む為のファイルです。
esptool.exe	: Windows用ESP32シリアルブートローダユーティリティ
uploadbin.bat	: 書き込み用バッチファイル
esptool.py	: PythonベースのESP32シリアルブートローダユーティリティ
uploadbin.sh	: 書き込み用シェルスクリプト

・USBシリアル変換基板をUSB接続し、LOBDAS基板のJ1にシリアル接続する．
・シリアルポートを確認する．
・Windows10はデバイスマネージャのポート（COMとLPT）をクリックして、macOSは ls -l /dev/tty* をUbuntu 18.04 は ls -l /dev/serial/by-id/ を実行する．
・バッチファイル（Windows10）またはシェルスクリプト（macOS,Ubuntu18.04）を実行する．実行する場合、第一引数にポートを、第二引数に書き込むバイナリファイル（****.ino.esp32.）

例）
書き込むバイナリファイルはLOBDAS.ino.esp32.binとする

windows10
C:¥Users¥hoge¥dokosoko¥uploadbin.bat COM3 LOBDAS

macOS
macbook:dokosoko hoge$ sh ./uploadbin.sh tty.SLAB_USBtoUART LOBDAS

Ubuntu 18.04
hoge@ubuntu:~$ sudo chmod 666 /dev/ttyUSB0
hoge@ubuntu:~$ sh ./uploadbin.sh ttyUSB0 LOBDAS