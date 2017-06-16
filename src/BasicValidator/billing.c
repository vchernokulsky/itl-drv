#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#include "billing.h"
#include "common.h"

// externals declarations
extern int _is_sess_processing;

// internal variables
unsigned long _sess_amount = 0;
unsigned long _sess_payout_amount = 0;

void StartPayout(SSP_COMMAND_SETUP ssp_setup)
{
	FILE *file;
	char buffer[128];
	unsigned int payout_amount;

	unsigned int ssp_response = 0x00;


	memset(buffer, 0, 128);

	file = fopen(SESSION_PAYOUT_FILE, "r");
	fscanf(file, "%s", buffer);
	fclose(file);

	payout_amount = atoi(buffer);
	printf("BasicValidator: payout amount = %i\n", payout_amount);

	ssp_response = ssp_payout_amount(ssp_setup, payout_amount*100);
	if(ssp_response != SSP_RESPONSE_OK)
	{
	    printf("BasicValidator: payout process error\n");
	    printf("SSP_RESPONSE: 0x%02X\n", ssp_response);
	}
	else
    {
		printf("BasicValidator: payout process started\n");
		remove(SESSION_PAYOUT_FILE);
	}
}

void ProcessBilling(unsigned long event_data)
{
	FILE *file;
	char buffer[128];

	memset(buffer, 0, 128);
	printf("Process credits\n");
	switch(event_data)
	{
		case 1:
			_sess_amount += 10;
			break;
		case 2:
			_sess_amount += 50;
			break;
		case 3:
			_sess_amount += 100;
			break;
		case 4:
			_sess_amount += 500;
			break;
		case 5:
			_sess_amount += 1000;
			break;
		case 6:
			_sess_amount += 5000;
			break;

	}
	printf("Session amount: %lu\n", _sess_amount);
	file = fopen(SESSION_BILLING_FILE, "w");
	fprintf(file, "%lu\n", _sess_amount);
	fclose(file);
}

void ParsePoll(SSP_POLL_DATA * poll)
{
	int i;
	for (i = 0; i < poll->event_count; ++i)
	{
		switch(poll->events[i].event)
		{
		case SSP_POLL_RESET:
			printf("Unit Reset\n");
			break;
		case SSP_POLL_READ:
			if (poll->events[i].data > 0)
				printf("Note Read %ld\n",poll->events[i].data);
			break;
		case SSP_POLL_CREDIT:
			printf("Credit %ld\n",poll->events[i].data);
			ProcessBilling(poll->events[i].data);
			break;
		case SSP_POLL_REJECTING:
			break;
		case SSP_POLL_REJECTED:
			printf("Note Rejected\n");
			break;
		case SSP_POLL_STACKING:
			break;
		case SSP_POLL_STACKED:
			printf("Stacked\n");
			break;
		case SSP_POLL_DISPENSING:
			// answer for whole note despensing process
			//printf("Dispensing\n");
			break;
		case SSP_POLL_DISPENSED:
			_is_sess_processing = 0;
			printf("Dispensed\n");
			break;
		case SSP_POLL_SAFE_JAM:
			printf("Safe Jam\n");
			break;
		case SSP_POLL_UNSAFE_JAM:
			printf("Unsafe Jam\n");
			break;
		case SSP_POLL_DISABLED:
			printf("DISABLED\n");
			break;
		case SSP_POLL_FRAUD_ATTEMPT:
			printf("Fraud Attempt %ld\n",poll->events[i].data);
			break;
		case SSP_POLL_STACKER_FULL:
			printf("Stacker Full\n");
			break;
        case SSP_POLL_CASH_BOX_REMOVED:
            printf("Cashbox Removed\n");
            break;
        case SSP_POLL_CASH_BOX_REPLACED:
            printf("Cashbox Replaced\n");
            break;
        case SSP_POLL_CLEARED_FROM_FRONT:
            printf("Cleared from front\n");
            break;
        case SSP_POLL_CLEARED_INTO_CASHBOX:
            printf("Cleared Into Cashbox\n");
            break;
		}
	}
}
