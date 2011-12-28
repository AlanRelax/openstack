ps aux | grep [n]ova-copmute> /dev/null
if [ $? -eq 1 ];then
echo OK
fi
