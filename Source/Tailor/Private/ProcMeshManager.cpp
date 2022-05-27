//=======================================================================================================================================
// Copyright (C) 2022, AppleParfait, All Rights Reserved.
//=======================================================================================================================================


#include "ProcMeshManager.h"
#include "Components/StaticMeshComponent.h"
#include "Rendering/PositionVertexBuffer.h"
#include "Engine/StaticMesh.h"
#include "StaticMeshResources.h"
#include "UObject/ConstructorHelpers.h"
#include "FileHelper.h"
#include <fstream>

using namespace std;

void AProcMeshManager::readFileToBuffer(FString filename, int index, bool isWeight) {
	TArray<uint8> result;
	FFileHelper().LoadFileToArray(result, *filename);
	UE_LOG(LogTemp, Error, TEXT("allocatedSize: %d"), result.GetAllocatedSize());
	int number = 0;
	if (isWeight) {
		number = dimensions[index] * dimensions[index + 1];
	}
	else {
		number = dimensions[index + 1];
	}
	
	for (int i = 0; i < number ; i++) {
		uint8 temp[8];
		temp[0] = result[8 * i];
		temp[1] = result[8 * i + 1];
		temp[2] = result[8 * i + 2];
		temp[3] = result[8 * i + 3];
		temp[4] = result[8 * i + 4];
		temp[5] = result[8 * i + 5];
		temp[6] = result[8 * i + 6];
		temp[7] = result[8 * i + 7];
		double* doubleTemp = (double*)temp;
		if (isWeight) {
			layerWeights[index].Add(doubleTemp[0]);
		}
		else {
			layerBias[index].Add(doubleTemp[0]);
		}
	}
	//ifstream file(filename, ios::in | ios::binary | ios::ate);
	//int size = file.tellg();
	//UE_LOG(LogTemp, Error, TEXT("%d"), size);
	//char* memblock = new char[size];
	//file.seekg(0, ios::beg);
	//file.read(memblock, size);
	//file.close();

	//buffer[index] = (double*)memblock;
}
void forward(TArray<double> weight, double* input, double* output, int numRow, int numCol) {
	for (int row = 0; row < numRow; row++) {
		double sum = 0.0;
		for (int k = 0; k < numCol; k++) {
			sum += weight[row * numCol + k] * input[k];
		}
		output[row] = sum;
	}
}

void addBias(double* matrix, TArray<double> bias, int count) {
	for (int i = 0; i < count; i++) {
		matrix[i] += bias[i];
	}
}

void relu(double* matrix, int count) {
	for (int i = 0; i < count; i++) {
		if (matrix[i] < 0) {
			matrix[i] = 0;
		}
	}
}

// Sets default values
AProcMeshManager::AProcMeshManager()
{
	for (int layer = 0; layer < NUM_LAYER; layer++) {
		FString weightFilepath= FPaths::ProjectContentDir().Append(TEXT("model/"));
		FString biasFilepath = FPaths::ProjectContentDir().Append(TEXT("model/"));
		weightFilepath.Append(TEXT("layer"));
		biasFilepath.Append(TEXT("layer"));
		weightFilepath.AppendInt(layer);
		biasFilepath.AppendInt(layer);
		weightFilepath.Append(TEXT("weights"));
		biasFilepath.Append(TEXT("bias"));

		readFileToBuffer(weightFilepath, layer, true);
		readFileToBuffer(biasFilepath, layer, false);
	}
	UE_LOG(LogTemp, Error, TEXT("%f"), layerWeights[0][0]);
	UE_LOG(LogTemp, Error, TEXT("%f"), layerWeights[0][25000]);

	// 
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/// <summary>
	/// 20220408 AppleParfait:
	/// </summary>
	m_proceduralMeshComp = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("GeneratedMesh"));
	RootComponent = m_proceduralMeshComp;

	/// <summary>
	/// 20220408 AppleParfait: New in UE 4.17, multi-threaded PhysX cooking.
	/// </summary>
	m_proceduralMeshComp->bUseAsyncCooking = true;
}

// Called when the game starts or when spawned
void AProcMeshManager::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AProcMeshManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/// <summary>
	/// 20220408 AppleParfait: New in UE 4.17, multi-threaded PhysX cooking.
	/// </summary>
	if (bOriginalMeshLoad)
	{
		ChangeProcMeshData(DeltaTime);
	}
}

/// <summary>
/// 20220408 AppleParfait: This is called when actor is spawned (at runtime or when you drop it into the world in editor)
/// </summary>
void AProcMeshManager::PostActorCreated()
{
	Super::PostActorCreated();

	//CreateTriangle();
}

/// <summary>
/// 20220408 AppleParfait : This is called when actor is already in level and map is opened
/// </summary>
void AProcMeshManager::PostLoad()
{
	Super::PostLoad();

	//CreateTriangle();
}

/// <summary>
/// 20220408 AppleParfait: Initialize dic_originalVertices
/// </summary>
void AProcMeshManager::InitDicVertices()
{
	int maxHandlingSectionCount = m_proceduralMeshComp->GetNumSections();
	for (int i = 0; i < maxHandlingSectionCount; i++)
	{
		dic_originalVertices.Add(i);
	}

	bOriginalMeshLoad = true;
}

/// <summary>
/// 20220408 AppleParfait: an example of Creating a triangle by using UProceduralMeshComponent (not a UStaticMeshComp) 
/// </summary>
void AProcMeshManager::CreateTriangle()
{
	TArray<FVector> vertices;
	vertices.Add(FVector(0, 0, 0));
	vertices.Add(FVector(0, 100, 0));
	vertices.Add(FVector(0, 0, 100));

	TArray<int32> Triangles;
	Triangles.Add(0);
	Triangles.Add(1);
	Triangles.Add(2);

	TArray<FVector> normals;
	normals.Add(FVector(1, 0, 0));
	normals.Add(FVector(1, 0, 0));
	normals.Add(FVector(1, 0, 0));

	TArray<FVector2D> UV0;
	UV0.Add(FVector2D(0, 0));
	UV0.Add(FVector2D(10, 0));
	UV0.Add(FVector2D(0, 10));


	TArray<FProcMeshTangent> tangents;
	tangents.Add(FProcMeshTangent(0, 1, 0));
	tangents.Add(FProcMeshTangent(0, 1, 0));
	tangents.Add(FProcMeshTangent(0, 1, 0));

	TArray<FLinearColor> vertexColors;
	vertexColors.Add(FLinearColor(0.75, 0.75, 0.75, 1.0));
	vertexColors.Add(FLinearColor(0.75, 0.75, 0.75, 1.0));
	vertexColors.Add(FLinearColor(0.75, 0.75, 0.75, 1.0));

	//
	m_proceduralMeshComp->CreateMeshSection_LinearColor(0, vertices, Triangles, normals, UV0, vertexColors, tangents, true);

	// Enable collision data
	m_proceduralMeshComp->ContainsPhysicsTriMeshData(true);
}

/// <summary>
/// 20220408 AppleParfait: Change MeshData(only hadling vertices in the example function) for for a simple animation.
/// </summary>
void AProcMeshManager::ChangeProcMeshData(float _deltaTime)
{
	counter++;
	if (counter > 1000) counter = 0;
	double input[20752];
	double output[20752];
	fill_n(input, 20752, 0);
	fill_n(output, 20752, 0);
	for (int i = 0; i < 72;i++) {
		input[i] = poseStart[i] + (poseEnd[i] - poseStart[i]) * ((float)counter / 1000);
	}

	for (int layer = 0; layer < NUM_LAYER; layer++) {
		forward(layerWeights[layer], input, output, dimensions[layer + 1], dimensions[layer]);
		addBias(output, layerBias[layer], dimensions[layer + 1]);
		if (layer != NUM_LAYER - 1) {
			relu(output, dimensions[layer + 1]);
			for (int i = 0; i < dimensions[layer + 1]; i++) { input[i] = output[i]; }
		}
	}

	timeSpent += _deltaTime;

	TArray<FVector> vertices;
	TArray<FVector> normals;
	TArray<FVector2D> UV0;
	TArray<FProcMeshTangent> tangents;
	TArray<FColor> vertexColors;

	if (!bOriginalVerticesLoad)
	{
		for (auto& Elem : dic_originalVertices)
		{
			FProcMeshSection* aa = m_proceduralMeshComp->GetProcMeshSection(Elem.Key);

			int vertexCount = aa->ProcVertexBuffer.Num();

			for (int i = 0; i < vertexCount; i++)
			{
				Elem.Value.Add(aa->ProcVertexBuffer[i].Position);
			}
		}

		bOriginalVerticesLoad = true;
	}


	

	for (int j = 0; j < m_proceduralMeshComp->GetNumSections(); j++)
	{
		vertices.Empty();

		FProcMeshSection* aa = m_proceduralMeshComp->GetProcMeshSection(j);

		int vertexCount = aa->ProcVertexBuffer.Num();

		for (int i = 0; i < vertexCount; i++) {
			int fidx = faceIdx[i]-1;
			vertices.Add(300 * FVector(output[3 * fidx], output[3 * fidx + 1], output[3 * fidx + 2]));
		}

		m_proceduralMeshComp->UpdateMeshSection(j, vertices, normals, UV0, vertexColors, tangents);
	}

}

