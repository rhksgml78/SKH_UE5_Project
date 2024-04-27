#include "HUD/HealthBarComponent.h"
#include "HUD/HealthBar.h"
#include "Components/ProgressBar.h"

void UHealthBarComponent::SetHealthPercent(float Percent)
{
	// UWidgetComponent�� ������ ȣ�� �Լ� �� ȣ���ϴµ� ĳ���� �ؾ���
	if (HealthBarWidget == nullptr)
	{
		// �켱 ������ ��ü�� ��ó��(���������϶�)�ѹ��� ĳ�����ؼ� �ּҸ� �־��ش�. ĳ������ ����� ��α⶧���� �����ϸ� �ѹ��� ĳ�����Ͽ� ����ϴ°��� ����. �Լ��� ȣ��ɶ����� ĳ�������Ѵٸ� �ſ� ��δ�.
		HealthBarWidget = Cast<UHealthBar>(GetUserWidgetObject());
	}


	// ĳ������ ����������
	if (HealthBarWidget && HealthBarWidget->HealthBar)
	{
		HealthBarWidget->HealthBar->SetPercent(Percent);
	}
}
