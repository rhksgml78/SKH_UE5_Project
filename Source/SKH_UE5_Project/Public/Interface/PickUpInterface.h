#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PickUpInterface.generated.h"

UINTERFACE(MinimalAPI)
class UPickUpInterface : public UInterface
{
	GENERATED_BODY()
};

class SKH_UE5_PROJECT_API IPickUpInterface
{
	GENERATED_BODY()


public:
	virtual void SetOverlappingItem(class AItem* Item);
	virtual void AddSouls(class ASoul* Soul);
	virtual void AddGold(class ATreasure* Treasure);
	virtual void AddHealth(class AHealthPotion* HealthPotion);
};
