/*
	GrammarShape.h
	Core functionality for shape grammar
	Includes rules for subdividing, repeating, etc
*/

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Actor.h"
#include "GrammarShape.generated.h"

UCLASS()
class ARCHIPLEAGOALPHA_API UGrammarShape : public UStaticMeshComponent {
	GENERATED_BODY()

public:
	UGrammarShape();
	void expand();

	int spaces;
	int space_dir;
	int rule;

	TSubclassOf<class AActor> LightBlueprint;
	TSubclassOf<class AActor> ChairBlueprint;
	TSubclassOf<class AActor> BedBlueprint;

private:
	void rule_rooms(int count);
	void rule_walls();
	void rule_roof();
	void construct_wall(int dir);
};
