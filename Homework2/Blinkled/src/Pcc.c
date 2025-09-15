#include <Pcc.h>

PCC_status configure_ClockPort (PCC_PortIndex port_index)
{
	if  (port_index >= 0 && port_index < (sizeof(pcc_ports) / sizeof(pcc_ports[0])) )
	{
		   pcc_ports[port_index]->CGC |= PCC_PCCn_CGC_MASK;
		   return PCC_ERROR_NONE;
	}
	else
	{
	        return PCC_ERROR_INVALID_PORT;
	 }
}
