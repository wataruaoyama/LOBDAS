:: (Shift-JIS)
:: .bin��ESP32�ɃA�b�v���[�h����
:: PORT�𑕒u��usb�V���A���ɍ��킹�Đݒ�
:: APP�Ƀv���O��������ݒ�
::
set PORT=%1
set APP=%2

.\esptool.exe --port %PORT% --baud 961600 ^
    --chip esp32  --before default_reset --after hard_reset ^
    write_flash -z --flash_mode dio --flash_freq 80m --flash_size detect ^
    0x10000 ./%APP%.ino.esp32.bin

pause
