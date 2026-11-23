
#include "transaction.h"
#include "globals.h"

Transaction create_transaction(int fd) {
    Transaction t;
    t.client_fd = fd;

    memset(t.req_buf, 0, sizeof(t.req_buf));
    memset(t.res_buf, 0, sizeof(t.res_buf));

    Getnameinfo(g_clientp, g_clientlen, t.hostname, MAXLINE, t.port, MAXLINE,
                0);
    return t;
}
