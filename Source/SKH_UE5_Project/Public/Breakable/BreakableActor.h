#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/HitInterface.h"
#include "BreakableActor.generated.h"

// ����� �����ؾ��Ѵ�! ����.cs ���Ͽ� �߰�!
class UGeometryCollectionComponent;

UCLASS()
class SKH_UE5_PROJECT_API ABreakableActor : public AActor, public IHitInterface
{
	GENERATED_BODY()
	
public:	
	ABreakableActor();
	virtual void Tick(float DeltaTime) override;

		// �������̽� ��� ������ �Լ�
	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UGeometryCollectionComponent* GeometryCollection;

	// �浹�� ĸ�� ������Ʈ
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UCapsuleComponent* Capsule;

private:

	// Treasure ���·� ���۵� �����۸� �迭 ��ϰ���
	UPROPERTY(EditAnywhere, Category = "Breakable Property")
	TArray<TSubclassOf<class ATreasure>> TreasureClasses;

	bool bBroken = false;
};
