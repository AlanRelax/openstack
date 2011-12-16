/**
 * connect kvm hypervisor
 * close connection
 * usage:
 * author: livemoon
 */


#include "mylib.h"



int main() {

    virConnectPtr conn;
    int i, num;

    char hostname[15]="uec11-003";
    //gethostname(hostname, sizeof(hostname));
    conn_init(hostname, &conn);
    num = get_num_domain(conn); 

    if (num > 0) {

        int ids[num];
        FILE *fp;
        virConnectListDomains(conn, ids, num);
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

