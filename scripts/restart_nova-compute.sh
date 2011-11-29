#!/bin/bash

echo "Killing nova-compute ..."
ps aux | awk '/[n]ova-compute$/ {print $2}'| xargs echo
echo "Restarting libvirt-bin ..."
sudo /etc/init.d/libvirt-bin restart
if [ "$?" -eq 0 ]; then
    sleep 5
    echo "Starting nova-compute ..."
    cd $HOME/nova && (sudo sg libvirtd bin/nova-compute &)
fi
