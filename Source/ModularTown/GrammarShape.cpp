// GrammarShape.cpp

#include "GrammarShape.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Runtime/CoreUObject/Public/UObject/UObjectGlobals.h"
#include <iostream>
#include <cstring>
#include <string>

const int grid_size = 600;

UGrammarShape::UGrammarShape() {
 	rule = 0;
 	spaces = 1;
 	space_dir = 0;

 	static ConstructorHelpers::FObjectFinder<UBlueprint> ItemBlueprint1(TEXT("Blueprint'/Game/MPMeleeCombat/Blueprints/BP_InteractiveTorch.BP_InteractiveTorch'"));
    if (ItemBlueprint1.Object){
        LightBlueprint = (UClass*)ItemBlueprint1.Object->GeneratedClass;
    }
    static ConstructorHelpers::FObjectFinder<UBlueprint> ItemBlueprint2(TEXT("/Game/AIBehaviorToolkit/Blueprints/BP_WorkSit"));
    if (ItemBlueprint2.Object){
        ChairBlueprint = (UClass*)ItemBlueprint2.Object->GeneratedClass;
    }
    static ConstructorHelpers::FObjectFinder<UBlueprint> ItemBlueprint3(TEXT("/Game/AIBehaviorToolkit/Blueprints/BP_WorkSleep"));
    if (ItemBlueprint3.Object){
        BedBlueprint = (UClass*)ItemBlueprint3.Object->GeneratedClass;
    }
}

void UGrammarShape::expand() {
	switch(rule) {
		case 0: //create room foundations (1-5 rooms)
			rule_rooms(rand()%5);
			break;
		case 1: //add walls to room (terminal)
			rule_walls();
			break;
		case 2: //add roof to room (terminal)
			rule_roof();
			break;
	}
}

void UGrammarShape::rule_rooms(int count) {
	for(int i = 0; i <= count; i++) {
		UGrammarShape* shape = NewObject<UGrammarShape>(this, UGrammarShape::StaticClass(), *(FString::Printf(TEXT("Room_%i"), i)));
		static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObj(TEXT("/Game/EternalTemple/Environment/Bases/Thin/Base_Floor_01"));
 		shape->SetStaticMesh(MeshObj.Object);
 		shape->SetCollisionObjectType(ECC_WorldStatic);
 		shape->rule = 1;

 		switch(i) {
 			case 0:
 				shape->spaces = count;
 				shape->space_dir = 3;
 				if(count == 0) shape->SetRelativeScale3D(FVector(2,2,2));
 				break;
			case 1:
				shape->SetRelativeLocation(FVector(grid_size, 0, 0));
				break;
			case 2:
				shape->SetRelativeLocation(FVector(0, grid_size, 0));
				shape->space_dir = 1;
				break;
			case 3:
				shape->SetRelativeLocation(FVector(0, -1*grid_size, 0));
				shape->space_dir = 2;
				break;
			case 4:
				shape->SetRelativeLocation(FVector(-1*grid_size, 0, 0));
				shape->space_dir = 3;
				break;
 		}
 		shape->SetupAttachment(this);
 		/*
 		int interior = rand() % 5;
 		switch(interior) {
 			case 0: {
 				UChildActorComponent* bed = NewObject<UChildActorComponent>(this, UChildActorComponent::StaticClass(), *(FString::Printf(TEXT("Bed_%i"), space_dir)));
				bed->SetChildActorClass(BedBlueprint);
				bed->SetRelativeLocation(FVector(grid_size/-4, 0, 50));
				bed->SetRelativeRotation(FRotator(0,rand()%30-15,0));
				bed->SetupAttachment(shape);
 				} break;
 			case 1: {
 				int chair_rot = rand()%30-15;
 				UChildActorComponent* chair1 = NewObject<UChildActorComponent>(this, UChildActorComponent::StaticClass(), *(FString::Printf(TEXT("Chair1_%i"), space_dir)));
				chair1->SetChildActorClass(ChairBlueprint);
				chair1->SetRelativeLocation(FVector(grid_size/-4, grid_size/6, 0));
				chair1->SetRelativeRotation(FRotator(0,chair_rot,0));
				chair1->SetupAttachment(shape);

				UChildActorComponent* chair2 = NewObject<UChildActorComponent>(this, UChildActorComponent::StaticClass(), *(FString::Printf(TEXT("Chair2_%i"), space_dir)));
				chair2->SetChildActorClass(ChairBlueprint);
				chair2->SetRelativeLocation(FVector(grid_size/-4, grid_size/-6, 0));
				chair2->SetRelativeRotation(FRotator(0,-chair_rot,0));
				chair2->SetupAttachment(shape);
 				} break;
 		}*/

		shape->expand();
	}
}

void UGrammarShape::rule_walls() {
	switch(spaces) {
		case 0:
			construct_wall(0); construct_wall(1);
			construct_wall(2); construct_wall(3);
			break;
		case 1:
			for(int i = 0; i < 4; i++) {
				if(i != space_dir) {
					construct_wall(i);
				}
			}
			break;
		case 2:
			construct_wall(0); construct_wall(1);
			break;
		case 3:
			construct_wall(0);
			break;
	}
	this->rule = 2;
	this->expand();
}

void UGrammarShape::rule_roof() {
	UGrammarShape* roof = NewObject<UGrammarShape>(this, UGrammarShape::StaticClass(), *(FString(TEXT("Roof"))));
	if(spaces != 1) {
		static ConstructorHelpers::FObjectFinder<UStaticMesh> RoofObj(TEXT("/Game/EternalTemple/MeshParts/SM_Roof_3lvl_01.SM_Roof_3lvl_01"));
		roof->SetStaticMesh(RoofObj.Object);
		roof->SetRelativeLocation(FVector(0,0,grid_size/2));
		if(spaces == 0) roof->SetRelativeLocation(FVector(grid_size/-12,0,grid_size/2));
		else roof->SetRelativeScale3D(FVector(1.25,1.25,1.25));
		roof->SetupAttachment(this);
		roof->SetCollisionObjectType(ECC_WorldStatic);
		return;
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> RoofObj(TEXT("/Game/EternalTemple/MeshParts/SM_Roof_Corridor_End_01"));
 	roof->SetStaticMesh(RoofObj.Object);

 	roof->SetRelativeScale3D(FVector(0.9,0.9,0.9));
 	roof->SetCollisionObjectType(ECC_WorldStatic);
	switch(space_dir) {
		case 0:
			roof->SetRelativeLocation(FVector(grid_size/-2-grid_size/12,0,grid_size/-6));
			roof->SetRelativeRotation(FRotator(0,-90,0));
			break;
		case 1:
			roof->SetRelativeLocation(FVector(0,grid_size/-2+grid_size/24,grid_size/-6));
			break;
		case 2:
			roof->SetRelativeLocation(FVector(0,grid_size/2-grid_size/24,grid_size/-6));
			roof->SetRelativeRotation(FRotator(0,180,0));
			break;
		case 3:
			roof->SetRelativeLocation(FVector(grid_size/2-grid_size/12,0,grid_size/-6));
			roof->SetRelativeRotation(FRotator(0,90,0));
			break;
	}
	roof->SetupAttachment(this);
}

void UGrammarShape::construct_wall(int dir) {
	UGrammarShape* wall = NewObject<UGrammarShape>(this, UGrammarShape::StaticClass(), *(FString::Printf(TEXT("Wall_%i"), dir)));
	wall->SetCollisionObjectType(ECC_WorldStatic);
	//Door always spawns facing city center, random other doors always face away from center of building
	bool door = (spaces == 1) && ((space_dir == 0 && dir == 3) || (space_dir == 1 && dir == 2 && rand()%2 == 0) ||
		(space_dir == 2 && dir == 1 && rand()%2 == 0) || (space_dir == 3 && dir == 0 && rand()%2 == 0));
	door = door || (spaces == 0 && dir == 3);
	if(door) {
		//Spawn doorway instead of wall
		static ConstructorHelpers::FObjectFinder<UStaticMesh> WallObj(TEXT("/Game/EternalTemple/MeshParts/SM_Building_Entrance_02"));
	 	wall->SetStaticMesh(WallObj.Object);
	 } else {
	 	static ConstructorHelpers::FObjectFinder<UStaticMesh> WallObj(TEXT("/Game/EternalTemple/MeshParts/SM_Wall_Segment_03"));
	 	wall->SetStaticMesh(WallObj.Object);
	 }

 	UGrammarShape* column = NewObject<UGrammarShape>(this, UGrammarShape::StaticClass(), *(FString::Printf(TEXT("Column_%i"), dir)));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ColObj(TEXT("/Game/EternalTemple/MeshParts/SM_Building_Column_01"));
 	column->SetStaticMesh(ColObj.Object);
 	column->SetCollisionObjectType(ECC_WorldStatic);

	switch(dir) {
		case 0:
			wall->SetRelativeLocation(FVector(-1*(grid_size/2+grid_size/12), 0, grid_size/-6));
			column->SetRelativeLocation(FVector(-1*(grid_size/2+grid_size/12), grid_size/2, grid_size/-6));
			break;
		case 3:
			wall->SetRelativeLocation(FVector(grid_size/2-grid_size/12, 0, grid_size/-6));
			column->SetRelativeLocation(FVector(grid_size/2-grid_size/12, grid_size/-2, grid_size/-6));
			break;
		case 2:
			wall->SetRelativeLocation(FVector(grid_size/-12, grid_size/2, grid_size/-6));
			wall->SetRelativeRotation(FRotator(0,90,0));
			column->SetRelativeLocation(FVector(grid_size/2-grid_size/12, grid_size/2, grid_size/-6));
			break;
		case 1:
			wall->SetRelativeLocation(FVector(grid_size/-12, -1*(grid_size/2), grid_size/-6));
			wall->SetRelativeRotation(FRotator(0,90,0));
			column->SetRelativeLocation(FVector(grid_size/-2-grid_size/12, -1*(grid_size/2), grid_size/-6));
			break;
	}
	wall->SetupAttachment(this);
	column->SetupAttachment(this);

	//Spawn torches on walls near main door and with random chance
	
	if(!door && (space_dir == 0 || spaces == 0 || rand()%3 != 0)) {
		UChildActorComponent* child = NewObject<UChildActorComponent>(GetOwner(), UChildActorComponent::StaticClass(), *(FString::Printf(TEXT("Torch_%i"), dir)));
		child->SetChildActorClass(LightBlueprint);
		switch(dir) {
			case 0:
			case 1: child->SetRelativeLocation(FVector(50, 0, grid_size/3)); break;
			case 2:
			case 3: child->SetRelativeLocation(FVector(-50, 0, grid_size/3)); break;
		}
		child->SetupAttachment(wall);
	}
}
