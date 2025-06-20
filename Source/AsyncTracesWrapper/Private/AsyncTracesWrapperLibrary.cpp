#include "AsyncTracesWrapperLibrary.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

void UAsyncTracesWrapperLibrary::AsyncLineTraceByChannel(
    UObject* WorldContextObject,
    FVector Start,
    FVector End,
    ETraceTypeQuery TraceChannel,
    bool bTraceComplex,
    const TArray<AActor*>& ActorsToIgnore,
    EDrawDebugTrace::Type DrawDebugType,
    FAsyncLineTraceSingleResultDelegate Callback
)
{
    if (!WorldContextObject) return;
    UWorld* World = GEngine->GetWorldFromContextObjectChecked(WorldContextObject);
    if (!World) return;

    FCollisionQueryParams Params(SCENE_QUERY_STAT(AsyncLineTrace), bTraceComplex);
    Params.AddIgnoredActors(ActorsToIgnore);

    // Create a heap-allocated delegate to avoid dangling pointer issues
    FTraceDelegate* HeapDelegate = new FTraceDelegate();
    *HeapDelegate = FTraceDelegate::CreateLambda(
        [Callback, HeapDelegate](const FTraceHandle&, FTraceDatum& Datum)
        {
            bool bHit = Datum.OutHits.Num() > 0 && Datum.OutHits[0].bBlockingHit;
            FVector Impact = bHit ? Datum.OutHits[0].ImpactPoint : FVector::ZeroVector;
            FHitResult Hit = bHit ? Datum.OutHits[0] : FHitResult();

            // Execute the callback
            Callback.ExecuteIfBound(bHit, Impact, Hit);

            // Clean up the heap-allocated delegate
            delete HeapDelegate;
        }
    );

    World->AsyncLineTraceByChannel(
        EAsyncTraceType::Single,
        Start,
        End,
        UEngineTypes::ConvertToCollisionChannel(TraceChannel),
        Params,
        FCollisionResponseParams::DefaultResponseParam,
        HeapDelegate,  // Use heap-allocated delegate
        0
    );
}
