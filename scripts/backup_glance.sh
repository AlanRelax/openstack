#!/bin/bash

cd /data/openstack && rsync -avz glance 10.200.200.3:/data/backup
