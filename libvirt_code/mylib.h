#include <stdio.h>
#include <libvirt/libvirt.h>
#include <string.h>
#include <malloc.h>
#include <mysql/mysql.h>
#include <stdlib.h>

#define FALSE 0

typedef struct entry {
    struct entry *link;
    char host[12];
} entry;


/* connect_base.c */
void getRemoteUrl(char *, char **);    // input IP and set it to "qemu+ssh://$IP/system"
void conn_init(char *, virConnectPtr *);    // init libvirt connection
void conn_close(virConnectPtr *);    // close libvirt connection

/* db_control.c */
int db_init(entry **);    // init db,store computing host from mysqldb into a list
char *get_host(entry *);    // get host in list
