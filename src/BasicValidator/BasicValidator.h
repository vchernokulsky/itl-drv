#ifndef BASICVALIDATOR_H_INCLUDED
#define BASICVALIDATOR_H_INCLUDED

#define PORT_TO_USE "/dev/ttyUSB0"

//filename defines
#define SESSION_END_FILE "sess_end"
#define SESSION_PAYOUT_FILE "sess_payout"
#define SESSION_BILLING_FILE "billing_session"

#define NOTE_100_RUB_COUNT 20
#define NOTE_500_RUB_COUNT 4

#define DEFAULT_INHIBITS 0x1E
#define INHIBIT_500_RUB  0xF7
#define ALLOW_500_RUB    0x08
#define INHIBIT_1000_RUB 0xEF
#define ALLOW_1000_RUB   0x10

#endif // BASICVALIDATOR_H_INCLUDED
