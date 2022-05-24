#ifndef _STATES_H_
#define _STATES_H_

typedef enum {
    INITIAL_STATE = 0,
    WAITING_TRANSACTION = 1,
    PROVIDER_SET = 2,
    PROVIDER_CHECKED = 3,
    TRANSACTION_RECEIVED = 4,
    SIGNATURE_CHECKED = 5,
    TO_ADDR_CHECKED = 6,
    WAITING_USER_VALIDATION = 7,
    WAITING_SIGNING = 8,
    STATE_UPPER_BOUND
} state_e;

#endif  //_STATES_H_