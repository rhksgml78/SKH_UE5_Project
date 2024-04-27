#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/HitInterface.h"
#include "BreakableActor.generated.h"

// 모듈을 포함해야한다! 빌드.cs 파일에 추가!
class UGeometryCollectionComponent;

UCLASS()
class SKH_UE5_PROJECT_API ABreakableActor : public AActor, public IHitInterface
{
	GENERATED_BODY()
	
public:	
	ABreakableActor();
	virtual void Tick(float DeltaTime) override;

		// 인터페이스 상속 재정의 함수
	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UGeometryCollectionComponent* GeometryCollection;

	// 충돌용 캡슐 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UCapsuleComponent* Capsule;

private:

	// Treasure 형태로 제작된 아이템만 배열 등록가능
	UPROPERTY(EditAnywhere, Category = "Breakable Property")
	TArray<TSubclassOf<class ATreasure>> TreasureClasses;

	bool bBroken = false;
};
