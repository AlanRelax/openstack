#!/bin/bash


ps aux | awk '/[n]ova-compute$/ {print $2}'| xargs echo
sudo /etc/init.d/libvirt-bin restart
if [ "$?" -eq 0 ]; then
    sleep 10
    cd $HOME/nova && sudo sg libvirtd bin/nova-compute &
fi
