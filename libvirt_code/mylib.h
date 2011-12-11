#include <stdio.h>
#include <libvirt/libvirt.h>
#include <string.h>
#include <malloc.h>
#include <mysql/mysql.h>
#include <stdlib.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <netdb.h>

#define FALSE 1
#define TRUE 0

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
void list_id_domain(virConnectPtr);
int get_num_domain(virConnectPtr);
void list_info_domain(virDomainPtr);
void list_cpu_domain();
void list_disk_domain(virDomainPtr);
int list_network_domain(virDomainPtr);

/* myxml.c */
