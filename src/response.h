#ifndef RESPONSE_H
#define RESPONSE_H

#include "transaction.h"

void handle_response(Transaction *t);
void forward_response(Transaction *t);

#endif // !RESPONSE_H
