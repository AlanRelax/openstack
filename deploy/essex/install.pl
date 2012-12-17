#!/usr/bin/perl

use strict;
use 5.010;
use Config::IniFiles;



#sub parse {
#    my $cfg = Config::IniFiles->new( -file => "./config.ini", -default => "DEFAULT" );
#    my @value = $cfg->val( 'MYSQL', 'USER' );
#    print "$_\n" for @value;
#}

sub parse {
    my %ini;
    tie %ini, 'Config::IniFiles', ( -file => "./config.ini", -default => "DEFAULT" );
    $ini{$_[0]}{$_[1]};
}

sub install_common {
    system "apt-get", "update";
    system "apt-get", "-y", "upgrade";
    system "apt-get", "-y", "dist-upgrade";
    system "apt-get", "-y", "ntp" if ( system "dpkg", "-l", "ntp");
    open my $fh, "+<", "/etc/ntp.conf"
	or die "Can't open /etc/ntp.conf: $!";
    while ( <$fh> ) {
}

&install_common;

my $result = &parse("MYSQL", "USER");
print $result;
