#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HealthBar.generated.h"

UCLASS()
class SKH_UE5_PROJECT_API UHealthBar : public UUserWidget
{
	GENERATED_BODY()
	
public:
	// ���������ε��Ҷ� ��ü�� �̸��� �� ���ƾ��Ѵ�.
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* HealthBar;
};
