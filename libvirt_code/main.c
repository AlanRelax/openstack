/**
 * connect kvm hypervisor
 * close connection
 * usage:
 * author: livemoon
 */


#include "mylib.h"



int main(int argc, char **argv) {

//    link p;
    virConnectPtr conn;
//    char *name, *password;

//    argv++;
//   name = *argv++;
//    password = *argv;
//    db_init(name, password);
//    while ((p = pop_node()) != NULL ) {
        conn_init("uec11-003", &conn);
        printf("uec11-003:\n");
/*
int ids[5];
int maxids = 5;
int i, num;
num = virConnectListDomains(conn, ids, maxids);
    for(i = 0;i < num;i++) {
        virDomainPtr dom = NULL;
        dom = virDomainLookupByID(conn, ids[i]);
    printf("%s:\n", virDomainGetName(dom));
*/
        virDomainPtr dom = NULL;
        dom = virDomainLookupByID(conn, 7);
        list_info_domain(dom);
//list_disk_domain(dom);
//char *xmldesc;
//xmldesc = virDomainGetXMLDesc(dom, 0);
//printf("%s\n", xmldesc);
//free(xmldesc);
list_network_domain(dom);
        virDomainFree(dom);
 //   }

        conn_close(&conn);
//    }
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

