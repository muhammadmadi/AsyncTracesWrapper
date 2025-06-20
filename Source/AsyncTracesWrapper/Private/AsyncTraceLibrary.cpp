#include "AsyncTraceLibrary.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

void UAsyncTraceLibrary::AsyncLineTraceByChannel(
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

    FTraceDelegate Delegate = FTraceDelegate::CreateLambda(
        [Callback](const FTraceHandle&, FTraceDatum& Datum)
        {
            bool bHit = Datum.OutHits.Num() > 0 && Datum.OutHits[0].bBlockingHit;
            FVector Impact = bHit ? Datum.OutHits[0].ImpactPoint : FVector::ZeroVector;
            FHitResult Hit = bHit ? Datum.OutHits[0] : FHitResult();
            Callback.ExecuteIfBound(bHit, Impact, Hit);
        }
    );

    World->AsyncLineTraceByChannel(
        EAsyncTraceType::Single,                                         // Correct trace type
        Start,
        End,
        UEngineTypes::ConvertToCollisionChannel(TraceChannel),
        Params,
        FCollisionResponseParams::DefaultResponseParam,
        &Delegate,                                                       // Pointer to static delegate
        0                                                                
    );
}
