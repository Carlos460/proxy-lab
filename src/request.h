#ifndef REQUEST_H
#define REQUEST_H

#include "transaction.h"

void handle_request(Transaction *t);
void forward_request(Transaction *t);

#endif // !REQUEST_H
