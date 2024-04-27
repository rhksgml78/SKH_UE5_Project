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
	// ó�� nullptr ���� �����ϱ����� ��ũ�� ����
	UPROPERTY()
	class UHealthBar* HealthBarWidget;
};
