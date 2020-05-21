#/bin/sh
# UTF-8 改行=LF
# .binをESP32にアップロードする
# PORTを装置のusbシリアルに合わせて設定
# APPにプログラム名を設定
# 
PORT=$1
APP=$2

python ./esptool.py --port /dev/${PORT} --baud 921600 \
    --chip esp32  --before default_reset --after hard_reset \
    write_flash -z --flash_mode dio --flash_freq 80m --flash_size detect \
    0x10000 ./${APP}.ino.esp32.bin
