/**
 * connect kvm hypervisor
 * close connection
 * usage:
 * author: livemoon
 */


#include "mylib.h"



int main() {

    entry *host;

    db_init(&host);
    printf("%s ",get_host(host));
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

