#!/bin/bash

file=$1

for i in `cat $file`
do
    ssh $i "sudo su -c \"(cd && [ ! -e openstack ] && git clone git://github.com/livemoon/openstack) || (cd /data/stack/openstack && git pull)\" stack"
done
