//#include "../inc/SSPComs.h"
#include "../inc/ssp_commands.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "BasicValidator.h"


unsigned long _sess_amount = 0;
unsigned long _sess_payout_amount = 0;
unsigned int  _is_sess_processing = 1;

const unsigned int _notes_define[] = {10, 50, 100, 500, 1000, 5000};
unsigned int _notes_table[12];

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
	if(ssp_response == SSP_RESPONSE_OK)
	{
		printf("BasicValidator: payout process started\n");
		remove(SESSION_PAYOUT_FILE);
	}
}

void ProcessStoredNotes(SSP_COMMAND_SETUP ssp_setup, unsigned int nChannels)
{
    FILE *file;

    int i;
    unsigned short mul = 100;

    unsigned short amount;
    unsigned long  note_value;

    file = fopen("notes_table", "w");
    for(i = 0; i < nChannels; ++i)
    {
        note_value = _notes_define[i] * mul;
        if(ssp_get_coin_amount(ssp_setup, note_value, &amount) != SSP_RESPONSE_OK)
        {
            printf("Can't read note[%i] amount\n", _notes_define[i]);
            fclose(file);
            return;

        }
        _notes_table[i] = amount;
        printf("%i RUB - %i\n", _notes_define[i], amount);
        fprintf(file, "%i;%i\n", _notes_define[i], amount);
    }
    fclose(file);
}

void SaveUnitSettings(SSP_SETUP_REQUEST_DATA data)
{
    unsigned int nChannelsCount;
    unsigned int nMultiplier;
    unsigned int i;

    nChannelsCount = data.ChannelValues.NumberOfChannels;
    nMultiplier = data.ValueMultiplier;

    printf("CHANNELS COUNT: %i  |  MULTIPLIER: %i\n", nChannelsCount, nMultiplier);
    for(i = 0; i<16; ++i)
    {
        printf("CH%i: %i\n", i, data.ChannelValues.ChannelData[i]);
    }
}

void RunValidator(SSP_PORT port, const unsigned char ssp_address)
{
	unsigned int ssp_response = 0x00;
	unsigned char route = 0x00;

    SSP_COMMAND_SETUP ssp_setup;
    SSP_POLL_DATA poll;
	SSP_SETUP_REQUEST_DATA ssp_setup_req;
    //setup the required information
	ssp_setup.port = port;
	ssp_setup.Timeout = 1000;
	ssp_setup.RetryLevel = 3;
	ssp_setup.SSPAddress = ssp_address;
	ssp_setup.EncryptionStatus = NO_ENCRYPTION;

    //check validator is present
	if (ssp_sync(ssp_setup) != SSP_RESPONSE_OK)
	{
	    printf("NO VALIDATOR FOUND\n");
	    return;
	}
	printf ("Validator Found\n");

    //try to setup encryption using the default key
	if (ssp_setup_encryption(&ssp_setup,(unsigned long long)0x123456701234567LL) != SSP_RESPONSE_OK)
        printf("Encryption Failed\n");
    else
        printf("Encryption Setup\n");

    //enable the unit
	if (ssp_enable(ssp_setup) != SSP_RESPONSE_OK)
	{
	    printf("Enable Failed\n");
        return;
	}

    if (ssp_enable_higher_protocol_events(ssp_setup) != SSP_RESPONSE_OK)
    {
        printf("Higher Protocol Failed\n");
        return;
    }

    //set the inhibits (disable: 10, 5000 RUB notes)
	if (ssp_set_inhibits(ssp_setup,0x1E,0xFF) != SSP_RESPONSE_OK)
	{
	    printf("Inhibits Failed\n");
        return;
	}

	// payout setup code
	if(ssp_enable_payout_device(ssp_setup) != SSP_RESPONSE_OK)
	{
		//TODO: process payout fail
		printf("SmartPayout Failed\n");
		return;
	}

	ssp_response = ssp_setup_request(ssp_setup, &ssp_setup_req);
    if(ssp_response != SSP_RESPONSE_OK)
    {
        printf("SmartPayout setup request error\n");
		printf("SSP_RESPONSE: 0x%02X\n", ssp_response);
		return;
    }
    //SaveUnitSettings(ssp_setup_req);

    printf("Read stored notes\n");
    ProcessStoredNotes(ssp_setup, 6);

    // route 100, 500 -> SmartPayout
	ssp_response = ssp_set_routing(ssp_setup, 50*100, 0x01);
	if(ssp_response != SSP_RESPONSE_OK)
	{
		printf("SmartPayout routing failed\n");
		printf("SSP_RESPONSE: 0x%02X\n", ssp_response);
		return;
	}

    route = _notes_table[2] >= NOTE_100_RUB_COUNT ? 0x01 : 0x00;
	ssp_response = ssp_set_routing(ssp_setup, 100*100, route);
	if(ssp_response != SSP_RESPONSE_OK)
	{
		printf("SmartPayout routing failed\n");
		printf("SSP_RESPONSE: 0x%02X\n", ssp_response);
		return;
	}

    route = _notes_table[3] >= NOTE_500_RUB_COUNT ? 0x01 : 0x00;
	ssp_response = ssp_set_routing(ssp_setup, 500*100, route);
	if(ssp_response != SSP_RESPONSE_OK)
	{
		printf("SmartPayout routing failed\n");
		printf("SSP_RESPONSE: 0x%02X\n", ssp_response);
		return;
	}

	ssp_response = ssp_set_routing(ssp_setup, 1000*100, 0x01);
	if(ssp_response != SSP_RESPONSE_OK)
	{
		printf("SmartPayout routing failed\n");
		printf("SSP_RESPONSE: 0x%02X\n", ssp_response);
		return;
	}


	//interrupt loop by global flag
	while (_is_sess_processing)
	{
		if(access(SESSION_END_FILE, F_OK) != -1) {
			printf("BasicValidator: session end by external signal\n");
			return;
		}
        if(access(SESSION_PAYOUT_FILE, F_OK) != -1) {
			StartPayout(ssp_setup);
		}
	    //poll the unit
	    if (ssp_poll(ssp_setup, &poll) != SSP_RESPONSE_OK)
		{
			printf("SSP_POLL_ERROR\n");
            return;
		}
	    ParsePoll(&poll);
        usleep(500000); //500 ms delay between polls
	}
	if(!_is_sess_processing)
		printf("BasicValidator: session end by payout complete\n");
}

int main(int argc, char *argv[])
{
    int ssp_address;
	SSP_PORT port;

	memset(_notes_table, 0x00, sizeof(unsigned int) * 12);

    //check for command line arguments - first is port, second is ssp address
	if (argc <= 2)
	{
	    printf("Usage: BasicValidator <port> <sspaddress>\n");
	    return 1;
    }

    //open the com port
    printf("PORT: %s\n",argv[1]);
    port = OpenSSPPort(argv[1]);
    if (port == -1)
	{
	    printf("Port Error\n");
        return 1;
	}

    //set the ssp address
    ssp_address = atoi(argv[2]);
    printf("SSP ADDRESS: %d\n",ssp_address);


    //run the validator
	RunValidator(port,ssp_address);
	CloseSSPPort(port);
	return 0;
}
