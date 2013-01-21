#!/usr/bin/perl

use strict;
use 5.010001;
use Config::IniFiles;



#sub parse {
#    my $cfg = Config::IniFiles->new( -file => "./config.ini", -default => "DEFAULT" );
#    my @value = $cfg->val( 'MYSQL', 'USER' );
#    print "$_\n" for @value;
#}

$^I = ".bak";

sub parse {
    my %ini;
    tie %ini, 'Config::IniFiles', ( -file => "./config.ini", -default => "DEFAULT" );
    $ini{$_[0]}{$_[1]};
}

sub install_common {
    system "apt-get", "update";
    system "apt-get", "-y", "upgrade";
    system "apt-get", "-y", "dist-upgrade";
    system "apt-get", "-y", "install", "ntp" if ( system "dpkg", "-l", "ntp");
    @ARGV = qw ! /etc/ntp.conf !;
    $^I = ".bak";
    my $flag;
    while( <> ) {
	given( $_ ) {
	    when ( s/\A(server ntp.ubuntu.com)\Z/$1 iburst/ ) { print; }
	    when ( /\Aserver 127.127.1.0\Z/ ) { $flag += 1; print; }
	    when ( /\Afudeg\s+127\.127\.1\.0\s+stratum\s+10\Z/) { $flag += 2; print }
	    default { print; }
	}
    }
    print $flag;
}   

sub install_mysql {
    system "apt-get", "install", "-y", "python-mysqldb", "mysql-server";
    @ARGV = qw ( /etc/mysql/my.cnf );
    $^I = ".bak";
    while( <> ) {
	given( $_ ) {
	    when ( s/\Abind-address = (\d+\.){3}\d+\Z/bind-address = 0.0.0.0/ ) { print; }
	    default { print; }
	}
    }
    system "service", "mysql", "restart";
}

sub install_rabbitmq {
    system "apt-get", "install", "-y", "rabbitmq-server";
    system "rabbitmqctl", "change_password", "guest", $_[0];
    system "service", "rabbitmq-server", "restart";
}

sub install_keystone {
    system "apt-get", "install", "-y", "keystone", "python-keystone", "python-keystoneclient";
    @ARGV = qw ! /etc/keystone/keystone.conf/ !;
    while( <> ) {
	
}

#&install_common;
&install_mysql if ( system "dpkg", "-l", "mysql-server");
&install_rabbitmq("openstack") if ( system "dpkg", "-l", "rabbitmq-server");
&install_keystone;

my $result = &parse("MYSQL", "USER");
print $result;
