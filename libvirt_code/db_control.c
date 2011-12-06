
#include "mylib.h"


int db_init(entry **p) {

    int r,t;
    char *query;
    MYSQL mysql, *conn_ptr;
    MYSQL_RES *res;
    MYSQL_ROW row;

    mysql_init(&mysql);

    entry *new;
    entry *current;
    current = (entry *)malloc(sizeof(entry));
    *p = current;

    if (! &mysql) {
        fprintf(stderr, "mysql_init failed\n");
        exit(1);
    }

    conn_ptr = mysql_real_connect(&mysql, "localhost", "openstack", "0Penstack", "nova", 0, NULL, 0);

    if (conn_ptr) {
        printf("Connection success\n");
    }
    else {
        printf("Connection failed\n");
        exit(1);
    }

    query = "select host from services where topic='compute'";

    t = mysql_real_query(conn_ptr, query, (unsigned int)strlen(query));

    res = mysql_store_result(conn_ptr);

    for(r = 0; r < mysql_num_rows(res); r++)
    {
        row = mysql_fetch_row(res);

        if (row < 0)
            break;

        new = (entry *)malloc(sizeof(entry));
        if (new == NULL)
            return FALSE;
        current->link = new;
        strncpy(new->host, row[0],10);
        new->link = NULL;
        current = new;
    }

    mysql_close(conn_ptr);

    return 1;
}

char *get_host(entry *p) {
    

printf("%s ",p->host);
    return p->host;
}
