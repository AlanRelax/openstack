#!/usr/bin/perl

use strict;
use 5.010001;
#use Config::IniFiles;
#use DBI;


#sub parse {
#    my $cfg = Config::IniFiles->new( -file => "./config.ini", -default => "DEFAULT" );
#    my @value = $cfg->val( 'MYSQL', 'USER' );
#    print "$_\n" for @value;
#}

sub install_common {
    system "apt-get", "install", "-y", "ubuntu-cloud-keyring";
    system "echo", "deb http://ubuntu-cloud.archive.canonical.com/ubuntu precise-updates/grizzly main", ">> /etc/apt/sources.list.d/grizzly.list";
    SYSTEM "apt-get", "update";
    system "apt-get", "-y", "upgrade";
    system "apt-get", "-y", "dist-upgrade";
    system "apt-get", "-y", "install", "ntp" if ( system "dpkg", "-l", "ntp");
    system "apt-get", "-y", "install", "vlan", "bridge-utils";
    system "sed", "-i", "'s/#net.ipv4.ip_forward=1/net.ipv4.ip_forward=1/'", "/etc/sysctl.conf";
    system "sysctl", "net.ipv4.ip_forward=1";
}   

sub install_nova {
    system "apt-get", "install", "-y", "kvm", "libvirt-bin", "pm-utils";
    system "virsh", "net-destroy", "default";
    system "virsh", "net-undefine", "default";
    system "apt-get", "-y", "install", "quantum-plugin-openvswitch-agent";
    system "apt-get", "install", "-y", "nova-compute-kvm";
}


&install_common;
&install_nova;
