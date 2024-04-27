#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HitInterface.generated.h"

UINTERFACE(MinimalAPI)
class UHitInterface : public UInterface
{
	GENERATED_BODY()
};

class SKH_UE5_PROJECT_API IHitInterface
{
	GENERATED_BODY()


public:
	// 블프에서 정의할수있도록 매크로 지정
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void GetHit(const FVector& ImpactPoint, AActor* Hitter); 
};
