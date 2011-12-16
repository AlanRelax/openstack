#include "mylib.h"

int main() {

    virConnectPtr conn;
    char hostname[256] = "uec11-003";
    conn_init(hostname, &conn);

/*    char *p;
    virNetworkPtr net;
    net = virNetworkLookupByName(conn, "default");
    p = virNetworkGetXMLDesc(net,0);
    printf("%s\n", p);

    virNetworkFree(net);
*/
    //virNetworkPtr network;
    virNodeInfoPtr info;
    info = malloc(sizeof(virNodeInfo));
    virNodeGetInfo(conn, info);
    printf("%u\n", info->cpus);
    printf("%s\n", info->model);
    free(info);
    conn_close(&conn);
}
