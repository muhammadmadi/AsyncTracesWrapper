#include "AsyncTracesWrapperModule.h"
#include "Modules/ModuleManager.h"

IMPLEMENT_MODULE(FAsyncTracesWrapperModule, AsyncTracesWrapper)

void FAsyncTracesWrapperModule::StartupModule()
{
    // Module startup logic here if needed
}

void FAsyncTracesWrapperModule::ShutdownModule()
{
    // Module shutdown logic here if needed
    // Ensure any pending async operations are cleaned up
}
