#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "HealthBarComponent.generated.h"

UCLASS()
class SKH_UE5_PROJECT_API UHealthBarComponent : public UWidgetComponent
{
	GENERATED_BODY()
public:
	void SetHealthPercent(float Percent);

private:
	// 처음 nullptr 임을 증명하기위한 매크로 적용
	UPROPERTY()
	class UHealthBar* HealthBarWidget;
};
