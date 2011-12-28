#!/bin/bash

echo "Killing nova-compute ..."
for i in `ps aux | awk '/[n]ova-compute$/ {print $2}'`
do
    sudo kill $i
done
ps aux | grep [n]ova-compute > /dev/null
if [ $? -eq 1 ];then
    echo "Restarting libvirt-bin ..."
    sudo /etc/init.d/libvirt-bin restart
    if [ "$?" -eq 0 ]; then
        sleep 5
        echo "Starting nova-compute ..."
        cd $HOME/nova && (sudo su -c "sg libvirtd bin/nova-compute > /dev/null 2>&1 &" stack)
    fi
fi
