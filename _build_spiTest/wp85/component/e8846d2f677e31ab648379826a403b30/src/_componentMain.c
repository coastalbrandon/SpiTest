/*
 * AUTO-GENERATED _componentMain.c for the spiTestComponent component.

 * Don't bother hand-editing this file.
 */

#include "legato.h"
#include "../src/eventLoop.h"
#include "../src/log.h"

#ifdef __cplusplus
extern "C" {
#endif

extern const char* _spiTestComponent_le_spi_ServiceInstanceName;
const char** le_spi_ServiceInstanceNamePtr = &_spiTestComponent_le_spi_ServiceInstanceName;
void le_spi_ConnectService(void);
// Component log session variables.
le_log_SessionRef_t spiTestComponent_LogSession;
le_log_Level_t* spiTestComponent_LogLevelFilterPtr;

// Component initialization function (COMPONENT_INIT).
void _spiTestComponent_COMPONENT_INIT(void);

// Library initialization function.
// Will be called by the dynamic linker loader when the library is loaded.
__attribute__((constructor)) void _spiTestComponent_Init(void)
{
    LE_DEBUG("Initializing spiTestComponent component library.");

    // Connect client-side IPC interfaces.
    le_spi_ConnectService();

    // Register the component with the Log Daemon.
    spiTestComponent_LogSession = log_RegComponent("spiTestComponent", &spiTestComponent_LogLevelFilterPtr);

    //Queue the COMPONENT_INIT function to be called by the event loop
    event_QueueComponentInit(_spiTestComponent_COMPONENT_INIT);
}


#ifdef __cplusplus
}
#endif
