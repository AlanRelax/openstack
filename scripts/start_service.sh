bin/glance-registry --config-file=etc/glance-registry.conf &
bin/glance-api --config-file=etc/glance-api.conf &
bin/nova-api &
bin/nova-scheduler &
./utils/nova-wsproxy.py --flagfile ../nova/bin/nova.conf --web . 6080 &
