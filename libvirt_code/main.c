/**
 * connect kvm hypervisor
 * close connection
 * usage:
 * author: livemoon
 */


#include "mylib.h"



int main(int argc, char **argv) {

    link p;
    virConnectPtr conn;
    char *name, *password;

    argv++;
    name = *argv++;
    password = *argv;
    db_init(name, password);
    while ((p = pop_node()) != NULL ) {
        conn_init(p->host, &conn);
        printf("%s", virConnectGetSysinfo(conn,0));
        conn_close(&conn);
    }
//    virDomainPtr dom = NULL;   /* the domain being checked */
//    virDomainInfo info;        /* the information being fetched */
/*    int ret;

    int id = 6;



    virConnectPtr conn;
    conn_init("10.11.3.63", &conn);
    dom = virDomainLookupByID(conn, id);
    ret = virDomainGetInfo(dom, &info);
    printf("Domains %d: State is %d , %d CPUs, %ld memory\n", id, info.state, info.nrVirtCpu, info.memory);
    if (dom != NULL)
        virDomainFree(dom);

    conn_close(&conn);
*/
}

