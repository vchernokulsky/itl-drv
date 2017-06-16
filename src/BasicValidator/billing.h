#ifndef BILLING_H_INCLUDED
#define BILLING_H_INCLUDED

#include "../inc/SSPComs.h"

void ParsePoll(SSP_POLL_DATA * poll);
void StartPayout(SSP_COMMAND_SETUP ssp_setup);
void ProcessBilling(unsigned long event_data);

#endif // BILLING_H_INCLUDED
