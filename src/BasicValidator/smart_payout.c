#include <stdio.h>

#include "../inc/ssp_commands.h"

// internal variables
const unsigned int _notes_define[] = {10, 50, 100, 500, 1000, 5000};

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
        printf("%i RUB - %i\n", _notes_define[i], amount);
        fprintf(file, "%i;%i\n", _notes_define[i], amount);
    }
    fclose(file);
}


void SetupPayout(SSP_COMMAND_SETUP ssp_setup)
{
	unsigned int ssp_response = 0x00;
	SSP_SETUP_REQUEST_DATA ssp_setup_req;

	unsigned long value = 5000;
	unsigned char route = 0;

    ssp_response = ssp_enable_payout_device(ssp_setup);
	if(ssp_response != SSP_RESPONSE_OK)
	{
		//TODO: process payout fail
		printf("SmartPayout Failed\n");
        printf("SSP_RESPONSE: 0x%02X\n", ssp_response);
		return;
	}

	ssp_response = ssp_setup_request(ssp_setup, &ssp_setup_req);
    if(ssp_response != SSP_RESPONSE_OK)
    {
        printf("SmartPayout setup request error\n");
		printf("SSP_RESPONSE: 0x%02X\n", ssp_response);
		return;
    }

	printf("Read stored notes\n");
	ProcessStoredNotes(ssp_setup, 6);

	ssp_response = ssp_set_routing(ssp_setup, 10000, 0x00);
	if(ssp_response != SSP_RESPONSE_OK)
	{
		printf("SmartPayout routing failed\n");
		printf("SSP_RESPONSE: 0x%02X\n", ssp_response);
		return;
	}

    //SaveUnitSettings(ssp_setup_req);

    //printf("Read stored notes\n");
    ProcessStoredNotes(ssp_setup, 6);
}
