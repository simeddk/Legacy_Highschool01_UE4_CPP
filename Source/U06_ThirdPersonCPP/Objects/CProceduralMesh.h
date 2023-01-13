#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CProceduralMesh.generated.h"

UCLASS()
class U06_THIRDPERSONCPP_API ACProceduralMesh : public AActor
{
	GENERATED_BODY()
	
public:	
	ACProceduralMesh();

protected:
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& Transform) override;

private:
	UPROPERTY(VisibleDefaultsOnly)
		class USceneComponent* Scene;

	UPROPERTY(VisibleDefaultsOnly)
		class UStaticMeshComponent* StaticMesh;

	UPROPERTY(VisibleDefaultsOnly)
		class UProceduralMeshComponent* ProcMesh;

};
