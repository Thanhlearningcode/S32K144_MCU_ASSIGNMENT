#include "S32K144.h"
#include "boot_manager.h"


static volatile uint32_t heartbeat = 0;

int main(void)
{
  
    while (1) 
    {
        boot_manager_run();
    }
  
    return 0;
}

