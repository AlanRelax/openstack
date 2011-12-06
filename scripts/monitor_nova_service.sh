#!/bin/bash

for i in `sudo /data/stack/nova/bin/nova-manage service list | awk '/nova-compute.*enable.*XXX/ {print $2}'`
do
    ssh $i "sudo su -c \"/data/stack/openstack/scripts/restart_nova-compute.sh\" stack"
done
