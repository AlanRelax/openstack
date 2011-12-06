/**
 * connect kvm hypervisor
 * close connection
 * usage:
 * author: livemoon
 */

#include "mylib.h"

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

void conn_init(char *ip, virConnectPtr *conn)  {
    *conn = NULL; /* the hypervisor connection */
    int ret;
    char *p;
    p = (char *)malloc(35*sizeof(char));

    /* NULL means connect to local Xen hypervisor */
    getRemoteUrl(ip, &p);
printf("%s\n", p);
    *conn = virConnectOpenReadOnly(p);
    free(p);
    if (*conn == NULL) {
        fprintf(stderr, "Failed to connect to hypervisor\n");
    }

}

void conn_close(virConnectPtr *conn) {
    if (*conn != NULL)
        virConnectClose(*conn);
}

