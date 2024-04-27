#include "HUD/HealthBarComponent.h"
#include "HUD/HealthBar.h"
#include "Components/ProgressBar.h"

void UHealthBarComponent::SetHealthPercent(float Percent)
{
	// UWidgetComponent에 구성된 호출 함수 를 호출하는데 캐스팅 해야함
	if (HealthBarWidget == nullptr)
	{
		// 우선 포인터 객체에 맨처음(널포인터일때)한번만 캐스팅해서 주소를 넣어준다. 캐스팅은 비용이 비싸기때문에 가능하면 한번만 캐스팅하여 사용하는것이 좋다. 함수가 호출될때마다 캐스팅을한다면 매우 비싸다.
		HealthBarWidget = Cast<UHealthBar>(GetUserWidgetObject());
	}


	// 캐스팅이 성공했을때
	if (HealthBarWidget && HealthBarWidget->HealthBar)
	{
		HealthBarWidget->HealthBar->SetPercent(Percent);
	}
}
