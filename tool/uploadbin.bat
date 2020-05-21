:: (Shift-JIS)
:: .binをESP32にアップロードする
:: PORTを装置のusbシリアルに合わせて設定
:: APPにプログラム名を設定
::
set PORT=%1
set APP=%2

.\esptool.exe --port %PORT% --baud 961600 ^
    --chip esp32  --before default_reset --after hard_reset ^
    write_flash -z --flash_mode dio --flash_freq 80m --flash_size detect ^
    0x10000 ./%APP%.ino.esp32.bin

pause
