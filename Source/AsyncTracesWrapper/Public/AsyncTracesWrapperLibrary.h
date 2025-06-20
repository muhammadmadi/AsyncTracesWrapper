#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Engine/EngineTypes.h"
#include "Kismet/KismetSystemLibrary.h"
#include "AsyncTracesWrapperLibrary.generated.h"

DECLARE_DYNAMIC_DELEGATE_ThreeParams(FAsyncLineTraceDelegate, bool, bHit, FVector, ImpactPoint, FHitResult, HitResult);

UCLASS()
class ASYNCTRACESWRAPPER_API UAsyncTracesWrapperLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    /** Perform an asynchronous line trace using the specified channel. */
    UFUNCTION(BlueprintCallable, Category = "AsyncTracesWrapperLibrary", meta = (WorldContext = "WorldContextObject"))
    static void AsyncLineTraceByChannel(
        UObject* WorldContextObject,
        FVector Start,
        FVector End,
        ETraceTypeQuery TraceChannel,
        bool bTraceComplex,
        const TArray<AActor*>& ActorsToIgnore,
        EDrawDebugTrace::Type DrawDebugType,
        FAsyncLineTraceDelegate OnComplete
    );
};
