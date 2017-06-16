#include "validator.h"
#include <stdio.h>

void SetupValidator(SSP_COMMAND_SETUP ssp_setup)
{
	unsigned int ssp_response = 0x00;
    //setup the required information
    /*
	ssp_setup.port = port;
	ssp_setup.Timeout = 1000;
	ssp_setup.RetryLevel = 3;
	ssp_setup.SSPAddress = ssp_address;
	ssp_setup.EncryptionStatus = NO_ENCRYPTION;
    */

    //check validator is present
    ssp_response = ssp_sync(ssp_setup);
	if (ssp_response != SSP_RESPONSE_OK)
	{
	    printf("NO VALIDATOR FOUND\n");
	    printf("SSP_RESPONSE: 0x%02X\n", ssp_response);
	    return;
	}
	printf ("Validator Found\n");

    //try to setup encryption using the default key
	if (ssp_setup_encryption(&ssp_setup,(unsigned long long)0x123456701234567LL) != SSP_RESPONSE_OK)
        printf("Encryption Failed\n");
    else
        printf("Encryption Setup\n");

    //enable the unit
    ssp_response = ssp_enable(ssp_setup);
	if (ssp_response != SSP_RESPONSE_OK)
	{
	    printf("Enable Failed\n");
        printf("SSP_RESPONSE: 0x%02X\n", ssp_response);
        return;
	}

    ssp_response = ssp_enable_higher_protocol_events(ssp_setup);
    if (ssp_response != SSP_RESPONSE_OK)
    {
        printf("Higher Protocol Failed\n");
        printf("SSP_RESPONSE: 0x%02X\n", ssp_response);
        return;
    }

    //set the inhibits (enable all note acceptance)
    ssp_response = ssp_set_inhibits(ssp_setup, 0xFF, 0xFF);
	if (ssp_response != SSP_RESPONSE_OK)
	{
	    printf("Inhibits Failed\n");
        printf("SSP_RESPONSE: 0x%02X\n", ssp_response);
        return;
	}
}
