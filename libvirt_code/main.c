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
//    char hostname[256] = "uec11-003";
    char hostname[15];
    gethostname(hostname, sizeof(hostname));
        conn_init(hostname, &conn);
//        printf("%s:\n", virConnectGetHostname(conn));

    int num = virConnectNumOfDomains(conn);
    int ids[num];
    int i;
    virConnectListDomains(conn, ids, num);
    for(i = 0;i < num;i++) {
        char *vnet;
        vnet = malloc(10*sizeof(char));
        virDomainPtr dom = NULL;
        dom = virDomainLookupByID(conn, ids[i]);
        get_vnet(virDomainGetName(dom), &vnet);
        printf("%s|", virDomainGetName(dom));
        get_time();
        printf("|");
        list_info_domain(dom, list_num_core(conn));
        printf("|");
        list_disk_domain(dom);
        printf("|");
        list_network_domain(dom, vnet);
        printf("\n");
//char *xmldesc;
//xmldesc = virDomainGetXMLDesc(dom, 0);
//printf("%s\n", xmldesc);
//free(xmldesc);
//        get_vnet(virDomainGetName(dom), &vnet);
//        list_network_domain(dom, vnet);
        virDomainFree(dom);
    }

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

