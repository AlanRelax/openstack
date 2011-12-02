#include <stdlib.h>
#include <stdio.h>
#include <libvirt/libvirt.h>

static virConnectPtr conn = NULL;

static int
checkDomainState(virDomainPtr dom) {
    virDomainInfo info;
    int ret;

    ret = virDomainGetInfo(dom, &info);
    if (ret < 0 ) {
        return(-1);
    }
    return(info.state);
}

int main(int argc, char **argv) {
    int id = 0;
    conn = virConnectOpenReadOnly(NULL);
    if (conn == NULL) {
        fprintf(stderr, "Failed to connect to hypervisor\n");
        goto error;
    }

    if (argc > 1) {
        id = atoi(argv[1]);
    }
    if (id == 0) {
        int i, j, ids[10];
        i = virConnectListDomains(conn, &ids[0], 10);
        if (i<0) {
            fprintf(stderr, "Failed to list the domains\n");
            goto error;
        }
        for (j = 0;j < i;j++) {
            if (ids[j] != 0) {
                id = ids[j];
                break;
            }
        }
    }
    if (id == 0) {
        fprintf(stderr, "Failed find a running guest domain\n");
        goto error;
   }

error:
    if (conn != NULL)
        virConnectClose(conn);
    return(0);
}
