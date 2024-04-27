#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "PalyerHUD.generated.h"

UCLASS()
class SKH_UE5_PROJECT_API APalyerHUD : public AHUD
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, Category = Player)
	TSubclassOf<class UPlayerOverlay> PlayerOverlayClass;
};
