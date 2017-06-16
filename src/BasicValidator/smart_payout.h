#ifndef SMART_PAYOUT_H_INCLUDED
#define SMART_PAYOUT_H_INCLUDED

void SetupPayout(SSP_COMMAND_SETUP ssp_setup);
void ProcessStoredNotes(SSP_COMMAND_SETUP ssp_setup, unsigned int nChannels);

#endif // SMART_PAYOUT_H_INCLUDED
