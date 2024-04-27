#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "PlayerHUD.generated.h"

class UPlayerOverlay;

UCLASS()
class SKH_UE5_PROJECT_API APlayerHUD : public AHUD
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly, Category = Player)
	TSubclassOf<UPlayerOverlay> PlayerOverlayClass;

	UPROPERTY()
	UPlayerOverlay* PlayerOverlay;

public:
	FORCEINLINE UPlayerOverlay* GetPlayerOverlay() const { return PlayerOverlay; }
};
