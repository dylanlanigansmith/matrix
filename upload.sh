host="http://matrix.local/update"
file=/home/dylan/code/matrix/matrix/.pio/build/esp32dev/firmware.bin
md5=($(md5sum $file))
mode=firmware

curl --digest --compressed -L -X POST -F "MD5=$md5" -F "name=$mode" -F "data=@$file;filename=$mode" "$host/update"