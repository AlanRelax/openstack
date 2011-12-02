#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>


db_init() {

    int r,t;
    char *query;
    MYSQL mysql, *conn_ptr;
    MYSQL_RES *res;
    MYSQL_ROW row;
    mysql_init(&mysql);

    if (!conn_ptr) {
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

        printf("%s ", row[0]);
    }

    mysql_close(conn_ptr);
}

int main() {

    db_init();
    
    return 0;
}
