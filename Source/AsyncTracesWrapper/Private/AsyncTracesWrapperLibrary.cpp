#include "AsyncTracesWrapperLibrary.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

// Static storage for delegates to keep them alive
static TMap<FTraceHandle, FAsyncLineTraceDelegate> PendingDelegates;

void UAsyncTracesWrapperLibrary::AsyncLineTraceByChannel(
    UObject* WorldContextObject,
    FVector Start,
    FVector End,
    ETraceTypeQuery TraceChannel,
    bool bTraceComplex,
    const TArray<AActor*>& ActorsToIgnore,
    EDrawDebugTrace::Type DrawDebugType,
    FAsyncLineTraceDelegate OnComplete
)
{
    if (!WorldContextObject) return;

    UWorld* World = GEngine->GetWorldFromContextObjectChecked(WorldContextObject);
    if (!World) return;

    FCollisionQueryParams Params(SCENE_QUERY_STAT(AsyncLineTrace), bTraceComplex);
    Params.AddIgnoredActors(ActorsToIgnore);

    // Create a static delegate that will be stored in the FTraceDatum
    FTraceDelegate* StaticDelegate = new FTraceDelegate();
    *StaticDelegate = FTraceDelegate::CreateStatic(
        [](const FTraceHandle& Handle, FTraceDatum& TraceDatum)
        {
            // Find our Blueprint delegate
            if (FAsyncLineTraceDelegate* BlueprintDelegate = PendingDelegates.Find(Handle))
            {
                if (BlueprintDelegate->IsBound())
                {
                    bool bHit = TraceDatum.OutHits.Num() > 0 && TraceDatum.OutHits[0].bBlockingHit;
                    FVector ImpactPoint = bHit ? TraceDatum.OutHits[0].ImpactPoint : FVector::ZeroVector;
                    FHitResult HitResult = bHit ? TraceDatum.OutHits[0] : FHitResult();

                    BlueprintDelegate->ExecuteIfBound(bHit, ImpactPoint, HitResult);
                }

                // Clean up
                PendingDelegates.Remove(Handle);
            }
        }
    );

    // Start the async trace
    FTraceHandle Handle = World->AsyncLineTraceByChannel(
        EAsyncTraceType::Single,
        Start,
        End,
        UEngineTypes::ConvertToCollisionChannel(TraceChannel),
        Params,
        FCollisionResponseParams::DefaultResponseParam,
        StaticDelegate,
        0
    );

    // Store the Blueprint delegate for later execution
    if (OnComplete.IsBound())
    {
        PendingDelegates.Add(Handle, OnComplete);
    }
}
