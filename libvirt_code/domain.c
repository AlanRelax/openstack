/*
 * This program resolve domain processing
*/

#include "mylib.h"

int list_id_domain(virConnectPtr conn) {

    int ids[10];
    int maxids=10;
    int num, i;

    num = virConnectListDomains(conn, ids, maxids);

    for(i = 0;i < num;i++) {
        printf("%d\n",ids[i]);
    }
    return num;
}

int list_num_domain(virConnectPtr conn) {

    int num;
    num = virConnectNumOfDomains(conn);

    return num;
}
