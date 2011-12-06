
#include "mylib.h"


static link head = NULL;

int db_init(char *name,char *password) {

    int r,t;
    char *query;
    MYSQL mysql, *conn_ptr;
    MYSQL_RES *res;
    MYSQL_ROW row;
    link current;
printf("%s%s", name,password);

    mysql_init(&mysql);

    if (! &mysql) {
        fprintf(stderr, "mysql_init failed\n");
        exit(1);
    }

    conn_ptr = mysql_real_connect(&mysql, "localhost", name, password, "nova", 0, NULL, 0);

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
        current = make_node(row[0]);
        insert(current);
    }

    mysql_close(conn_ptr);

    return 1;
}

link pop_node() {

    if (head == NULL)
        return NULL;
    else {
        link p = head;
        head = head->next;
        return p;
    }
}

void free_node(link p) {
    free(p);
}

link make_node(char *item) {

    link p = malloc(sizeof(*p));


    if (p == NULL)
        return FALSE;
    strncpy(p->host, item, 10);
    p->next = NULL;

    return p;
}

void insert(link p) {

    p->next = head;
    head = p;
}
    
