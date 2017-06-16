#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "common.h"
#include "billing.h"
#include "validator.h"
#include "smart_payout.h"

int _is_sess_processing = 1;

int SetupCommand(SSP_PORT port, int ssp_address, SSP_COMMAND_SETUP *ssp_setup)
{
	ssp_setup->port = port;
	ssp_setup->Timeout = 1000;
	ssp_setup->RetryLevel = 3;
	ssp_setup->SSPAddress = ssp_address;
	ssp_setup->EncryptionStatus = NO_ENCRYPTION;

	return 1;
}

void StartDevicePolling(SSP_COMMAND_SETUP ssp_setup)
{
    SSP_POLL_DATA poll;

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
    SSP_COMMAND_SETUP ssp_setup;

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
    printf("SSP ADDRESS: %d\n", ssp_address);

    SetupCommand(port, ssp_address, &ssp_setup);

    // setup connected devices
    SetupValidator(ssp_setup);
    SetupPayout(ssp_setup);

    // start polling process
    StartDevicePolling(ssp_setup);

	CloseSSPPort(port);
	return 0;
}
