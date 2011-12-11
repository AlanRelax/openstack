/**
 * connect kvm hypervisor
 * close connection
 * usage:
 * author: livemoon
 */


#include "mylib.h"



int main() {

//    link p;
    virConnectPtr conn;
//    char *name, *password;

//    argv++;
//   name = *argv++;
//    password = *argv;
//    db_init(name, password);
//    while ((p = pop_node()) != NULL ) {
    char hostname[15]="uec11-003";
    //gethostname(hostname, sizeof(hostname));
    conn_init(hostname, &conn);
  
    int ids[10];
    int maxids = 10;
    int i, num;
    FILE *fp;
    num = virConnectListDomains(conn, ids, maxids);
    for(i = 0;i < num;i++) {
        virDomainPtr dom = NULL;
        dom = virDomainLookupByID(conn, ids[i]);
    //    printf("%s:\n", virDomainGetName(dom));
//list_disk_domain(dom);
        char *xmldesc;
        xmldesc = virDomainGetXMLDesc(dom, 0);
        if ((fp = fopen(virDomainGetName(dom), "w")) == NULL) {
            printf("Cannot open file test\n");
        }
        fprintf(fp,xmldesc);
        fclose(fp);
        free(xmldesc);
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

