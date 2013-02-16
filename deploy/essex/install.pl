#!/usr/bin/perl

use strict;
use 5.010001;
use Config::IniFiles;
use DBI;


#sub parse {
#    my $cfg = Config::IniFiles->new( -file => "./config.ini", -default => "DEFAULT" );
#    my @value = $cfg->val( 'MYSQL', 'USER' );
#    print "$_\n" for @value;
#}

$^I = ".bak";
my $ini_file = "./config.ini";

sub exist_app {
    system "dpkg", "-l", @_;
}

sub config_app {
    my %ini;
    my %writeini;
    tie %ini, 'Config::IniFiles', ( -file => $_[0] );
    tie %writeini, 'Config::IniFiles', ( -file => $_[1] );
    foreach (keys %ini) {
	my $section =$_;
	my @keys = keys %{$ini{$section}};
	foreach (@keys) {
	    $writeini{$section}{$_} = $ini{$section}{$_};
	}
    }
    tied(%writeini)->WriteConfig($_[1]) or die "Cannot write .";
}

sub get_ini {
    my $cfg = Config::IniFiles->new( -file => $_[0], -default => "DEFAULT" );
    my $value = $cfg->val($_[1], $_[2]);
    $value =~ s/,\s+/,/g;
    split(",", $value);
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
    my $root_pass = (&get_ini($ini_file, "MYSQL", "root_password"))[0];
    my $host = (&get_ini($ini_file, "MYSQL", "host"))[0];
    my $db_pass = (&get_ini($ini_file, "MYSQL", "db_password"))[0];
    my $dbh = DBI->connect("DBI:mysql:mysql:$host:3306", "root", $root_pass, { RaiseError => 1, AutoCommit => 1 });
    foreach (&get_ini($ini_file, "MYSQL", "db")) {
	$dbh->do(qq{CREATE DATABASE $_});
	my $db_admin = $_."dbadmin";
#	$dbh->do(qq{GRANT ALL PRIVILEGES ON $_.* TO "$dbadmin"@'%' IDENTIFIED BY "$db_pass"});
	my $query = qq{GRANT ALL PRIVILEGES ON $_.* TO ?\@'%' IDENTIFIED BY ?};
	dbh->do($query, undef, $db_admin, $db_pass);
	my $query = qq{GRANT ALL PRIVILEGES ON $_.* TO ?\@'localhost' IDENTIFIED BY ?};
	dbh->do($query, undef, $db_admin, $db_pass);
    }
    $dbh->disconnect();
    print $root_pass;
}

sub install_rabbitmq {
    system "apt-get", "install", "-y", "rabbitmq-server";
    system "rabbitmqctl", "change_password", "guest", $_[0];
    system "service", "rabbitmq-server", "restart";
}

sub install_keystone {
    system "apt-get", "install", "-y", "keystone", "python-keystone", "python-keystoneclient" if &exist_app("keystone");
    &config_app("./config/keystone/keystone.conf", "/etc/keystone/keystone.conf");
    system "service", "keystone", "restart";
    system "keystone-manage", "db_sync";
    my $token = (&get_ini("./config/keystone.conf", "DEFAULT", "admin_token"))[0];
    my $endpoint = (&get_ini($ini_file, "KEYSTONE", "endpoint"))[0];
    $ENV{SERVICE_TOKEN} = $token;
    $ENV{SERVICE_ENDPOINT} = $endpoint;
    my $db_pass = (&get_ini($ini_file, "MYSQL", "db_password"))[0];
    my $region = (&get_ini($ini_file, "KEYSTONE", "region"))[0];
    my $dbh = DBI->connect("DBI:mysql:keystone:$host:3306", "keystonedbadmin", $db_pass, { RaiseError => 1, AutoCommit => 1});
    foreach (&get_ini($ini_file, "KEYSTONE", "service")) {
	my @service = &get_ini($ini_file, "KEYSTONE_SERVICE", $_);
	system "keystone", "service-create", "--name", $_ , "--type", $service[0], "--description", $service[1], "\n";
	my $sth = $dbh->prepare('SELECT ID FROM service WHERE type=?');
	$sth->bind_param(1, $service[0]);
	$sth->execute();
	my $result = $sth->fetchrow_hashref();
	my @endpoint = &get_ini($ini_file, "KEYSTONE_ENDPOINT", $service[0]);
	system "keystone", "endpoint-create", "--region", $endpoint[0], "--service_id", $result->{id}, "--publicurl", $endpoint[1], "--adminurl", $endpoint[2], "--internalurl", $endpoint[3];
    }
    $dbh->disconnect();
}

sub install_glance {
    system "apt-get", "install", "-y", "glance", "glance-api", "glance-client", "glance-common", "glance-registry", "python-glance" if &exit_app("glance");
    &config_app("./config/glance/glance-api-paste.ini", "/etc/glance/glance-api-paste.ini");
    &config_app("./config/glance/glance-registry-paste.ini", "/etc/glance/glance-registry-paste.ini");
    &config_app("./config/glance/glance-api.conf", "/etc/glance/glance-api.conf");
    &config_app("./config/glance/glance-registry.conf", "/etc/glance/glance-registry.conf");
    system "glance-manage", "version_control", "0";
    system "glance-manage", "db_sync";
    system "service", "glance-api", "restart";
    system "service", "glance-registry", "restart";
    system "wget";
}

sub install_nova {
#my @nova_pkg = qq/apt-get install -y nova-api nova-cert nova-common/;
#system(@nova_pkg);
#&config_app("./config/nova/nova.conf", "/etc/nova/nova.conf");
    my @nova_service = &get_ini($ini_file, "NOVA", "service");
    foreach (@nova_service) {
	print "service", $_, "stop";
    }
    sleep(10);
    foreach (@nova_service) {
	print "service", $_, "start";
    }
    sleep(10);
    print "nova-manage", "db", "sync";
    foreach (@nova_service) {
	print "service", $_, "restart";
    }
}

#&install_common;
#&install_mysql if ( system "dpkg", "-l", "mysql-server");
#&install_rabbitmq("openstack") if ( system "dpkg", "-l", "rabbitmq-server");
#&install_keystone;
foreach (&get_ini($ini_file, "KEYSTONE", "service")) {
    my @service = &get_ini($ini_file, "KEYSTONE_SERVICE", $_);
    print "keystone", "service-create", "--name", $_ , "--type", $service[0], "--description", $service[1], "\n";
}
