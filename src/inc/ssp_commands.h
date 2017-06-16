#ifndef SSP_COMMANDS_H_INCLUDED
#define SSP_COMMANDS_H_INCLUDED

#include "../inc/SSPComs.h"


void _ssp_setup_command_structure(SSP_COMMAND_SETUP * setup, SSP_COMMAND * sspC);

SSP_RESPONSE_ENUM _ssp_return_values(SSP_PORT port,SSP_COMMAND * sspC);

SSP_RESPONSE_ENUM ssp_setup_encryption(SSP_COMMAND_SETUP * setup,const unsigned long long fixedkey);

SSP_RESPONSE_ENUM ssp_reset(SSP_COMMAND_SETUP setup);

SSP_RESPONSE_ENUM ssp_host_protocol(SSP_COMMAND_SETUP setup,const unsigned char host_protocol);

SSP_RESPONSE_ENUM ssp_poll(SSP_COMMAND_SETUP setup,SSP_POLL_DATA * poll_response);

SSP_RESPONSE_ENUM ssp_get_serial(SSP_COMMAND_SETUP setup,unsigned long * serial );

SSP_RESPONSE_ENUM ssp_sync(SSP_COMMAND_SETUP setup);

SSP_RESPONSE_ENUM ssp_disable(SSP_COMMAND_SETUP setup);

SSP_RESPONSE_ENUM ssp_enable(SSP_COMMAND_SETUP setup);

SSP_RESPONSE_ENUM ssp_set_inhibits(SSP_COMMAND_SETUP setup,const unsigned char lowchannels, const unsigned char highchannels);

SSP_RESPONSE_ENUM ssp_display_off(SSP_COMMAND_SETUP setup);

SSP_RESPONSE_ENUM ssp_display_on(SSP_COMMAND_SETUP setup);

SSP_RESPONSE_ENUM ssp_reject_note(SSP_COMMAND_SETUP setup);

SSP_RESPONSE_ENUM ssp_hold_note(SSP_COMMAND_SETUP setup);

SSP_RESPONSE_ENUM ssp_enable_higher_protocol_events(SSP_COMMAND_SETUP setup);

SSP_RESPONSE_ENUM ssp_unit_data(SSP_COMMAND_SETUP setup, SSP_UNIT_DATA * sud);

SSP_RESPONSE_ENUM ssp_channel_value_data(SSP_COMMAND_SETUP setup, SSP_CHANNEL_DATA * scd);

SSP_RESPONSE_ENUM ssp_channel_security_data(SSP_COMMAND_SETUP setup, SSP_CHANNEL_DATA * scd);

SSP_RESPONSE_ENUM ssp_last_reject(SSP_COMMAND_SETUP setup, unsigned char * last_reject_reason);

SSP_RESPONSE_ENUM ssp_setup_request(SSP_COMMAND_SETUP setup, SSP_SETUP_REQUEST_DATA * setup_request_data);

SSP_RESPONSE_ENUM ssp_payout_amount(SSP_COMMAND_SETUP setup, const unsigned long amount);

SSP_RESPONSE_ENUM ssp_set_coin_amount(SSP_COMMAND_SETUP setup, const unsigned long value, const unsigned short amount);

SSP_RESPONSE_ENUM ssp_get_coin_amount(SSP_COMMAND_SETUP setup, const unsigned long value, unsigned short * amount);

SSP_RESPONSE_ENUM ssp_halt_payout(SSP_COMMAND_SETUP setup);

SSP_RESPONSE_ENUM ssp_set_routing(SSP_COMMAND_SETUP setup,const unsigned long value, const unsigned char route);

SSP_RESPONSE_ENUM ssp_get_routing(SSP_COMMAND_SETUP setup,const unsigned long value,  unsigned char * route);

SSP_RESPONSE_ENUM ssp_float_hopper(SSP_COMMAND_SETUP setup,const unsigned long value,  const unsigned long minimum_payout);

SSP_RESPONSE_ENUM ssp_float_payout(SSP_COMMAND_SETUP setup,const unsigned long value,  const unsigned long minimum_payout);

SSP_RESPONSE_ENUM ssp_minimum_payout(SSP_COMMAND_SETUP setup,unsigned long * value);

SSP_RESPONSE_ENUM ssp_empty(SSP_COMMAND_SETUP setup);

SSP_RESPONSE_ENUM ssp_enable_payout_device(SSP_COMMAND_SETUP setup);

SSP_RESPONSE_ENUM ssp_disable_payout_device(SSP_COMMAND_SETUP setup);

SSP_RESPONSE_ENUM ssp_set_host_serial(SSP_COMMAND_SETUP setup,const unsigned long serial);

SSP_RESPONSE_ENUM ssp_set_coin_inhibit(SSP_COMMAND_SETUP setup,const unsigned long value,const unsigned char state);

SSP_RESPONSE_ENUM ssp_setup_request_hopper(SSP_COMMAND_SETUP setup, SSP_SETUP_REQUEST_DATA * setup_request_data);

#endif // SSP_COMMANDS_H_INCLUDED
