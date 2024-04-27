#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HealthBar.generated.h"

UCLASS()
class SKH_UE5_PROJECT_API UHealthBar : public UUserWidget
{
	GENERATED_BODY()
	
public:
	// 위젯과바인드할때 객체의 이름이 꼭 같아야한다.
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* HealthBar;
};
