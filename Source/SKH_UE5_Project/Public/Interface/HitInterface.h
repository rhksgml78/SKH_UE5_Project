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
	// �������� �����Ҽ��ֵ��� ��ũ�� ����
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void GetHit(const FVector& ImpactPoint, AActor* Hitter); 
};
