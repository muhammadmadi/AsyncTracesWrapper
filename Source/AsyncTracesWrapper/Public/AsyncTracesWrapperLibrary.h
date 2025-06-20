#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "Engine/EngineTypes.h"
#include "Kismet/KismetSystemLibrary.h"
#include "AsyncTracesWrapperLibrary.generated.h"


DECLARE_DYNAMIC_DELEGATE_ThreeParams(FAsyncLineTraceSingleResultDelegate, bool, bBlockingHit, FVector, ImpactPoint,
                                     FHitResult, HitResult);

UCLASS()
class ASYNCTRACESWRAPPER_API UAsyncTracesWrapperLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    /** Perform an asynchronous line trace using the specified channel. */
    UFUNCTION(BlueprintCallable, Category = "Async Trace", meta = (WorldContext = "WorldContextObject", BlueprintInternalUseOnly = "true"))
    static void AsyncLineTraceByChannel(
        UObject* WorldContextObject,
        FVector Start,
        FVector End,
        ETraceTypeQuery TraceChannel,
        bool bTraceComplex,
        const TArray<AActor*>& ActorsToIgnore,
        EDrawDebugTrace::Type DrawDebugType,
        FAsyncLineTraceSingleResultDelegate Callback
    );
};
