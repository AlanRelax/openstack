/**
 * section: Informations
 * synopsis: Extract information about Xen domain 0
 * purpose: Demonstrate the basic use of the library to connect to the
 *          hypervisor and extract domain information.
 * usage: info1
 * test: info1
 * author: Daniel Veillard
 * copy: see Copyright for the status of this software.
 */

#include <stdio.h>
#include <libvirt/libvirt.h>
#include <string.h>
#include <malloc.h>

/**
 * getDomainInfo:
 * @id: the id of the domain
 *
 * extract the domain 0 information
 */
//static void


void getRemoteUrl(char *ip, char **p) {
    char s[35] = "";
    if (strlen(ip) > 12 ) {
        fprintf(stderr, "ip length is too long\n");
    }
    else {
        strcat(s, "qemu+ssh://root@");
        strcat(s, ip);
        strcat(s, "/system");
    }
    strcpy(*p, s);
}


void getDomainInfo(int id, char *ip) {
    virConnectPtr conn = NULL; /* the hypervisor connection */
    virDomainPtr dom = NULL;   /* the domain being checked */
    virDomainInfo info;        /* the information being fetched */
    int ret;
    char *p;
    p = (char *)malloc(35*sizeof(char));

    /* NULL means connect to local Xen hypervisor */
    getRemoteUrl(ip, &p);
    conn = virConnectOpenReadOnly(p);
    free(p);
    if (conn == NULL) {
        fprintf(stderr, "Failed to connect to hypervisor\n");
        goto error;
    }

    /* Find the domain of the given id */
    dom = virDomainLookupByID(conn, id);
    if (dom == NULL) {
        fprintf(stderr, "Failed to find Domain %d\n", id);
        goto error;
    }

    /* Get the information */
    ret = virDomainGetInfo(dom, &info);
    if (ret < 0) {
        fprintf(stderr, "Failed to get information for Domain %d\n", id);
        goto error;
    }

    printf("Domains %d: State is %d , %d CPUs, %ld memory\n", id, info.state, info.nrVirtCpu, info.memory);

error:
    if (dom != NULL)
        virDomainFree(dom);
    if (conn != NULL)
        virConnectClose(conn);
}




int main() {


    getDomainInfo(6, "uec11-002");

    return(0);
}
