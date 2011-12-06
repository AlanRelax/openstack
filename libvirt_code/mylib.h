#include <stdio.h>
#include <libvirt/libvirt.h>
#include <string.h>
#include <malloc.h>
#include <mysql/mysql.h>
#include <stdlib.h>

#define FALSE 0
#define TRUE 1

typedef struct NODE *link;
struct NODE {
    char host[12];
    link next;
};


/* connect_base.c */
void getRemoteUrl(char *, char **);    // input IP and set it to "qemu+ssh://$IP/system"
void conn_init(char *, virConnectPtr *);    // init libvirt connection
void conn_close(virConnectPtr *);    // close libvirt connection

/* db_control.c */
int db_init();    // init db,store computing host from mysqldb into a list
link pop_node();
void free_node(link);
link make_node(char *);
void insert(link);

/* domain.c */
int list_domain(virConnectPtr);
int list_num_domain(virConnectPtr);
