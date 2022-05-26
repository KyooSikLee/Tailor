//=======================================================================================================================================
// Copyright (C) 2022, AppleParfait, All Rights Reserved.
//=======================================================================================================================================
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "ProcMeshManager.generated.h"

#define NUM_LAYER 4
UCLASS()
class AProcMeshManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AProcMeshManager();
	int dimensions[5] = { 20752, 128, 64, 128, 5283 };
	TArray<double> layerWeights[NUM_LAYER];
	TArray<double> layerBias[NUM_LAYER];

	void readFileToBuffer(FString filename,  int index, bool isWeight);
	double poseStart[72] = { -0.09681905061006546, -0.03425717353820801, 0.030272137373685837, 0.18732109665870667, 0.15736013650894165, 0.0922677293419838, 0.24782311916351318, -0.3062630891799927, -0.14813724160194397, 0.08571051806211472, 0.004716965835541487, 0.14138393104076385, -0.1410536766052246, -0.04115522652864456, -0.061735574156045914, -0.12601728737354279, -0.20469997823238373, 0.05190874636173248, -0.3144761025905609, 0.05176307633519173, -0.08825551718473434, 0.13344736397266388, 0.2245791107416153, -0.21885357797145844, 0.18842996656894684, -0.13706348836421967, 0.1572127342224121, 0.3086540102958679, 0.009847837500274181, -0.10058707743883133, -0.08821946382522583, 0.017798731103539467, 0.2307359278202057, -0.24255089461803436, -0.30764061212539673, -0.19544371962547302, 0.3383592665195465, 0.15434494614601135, 0.1321900635957718, -0.4681617319583893, 0.22779157757759094, 0.025413736701011658, -0.1858500838279724, 0.06513457745313644, 0.04978000000119209, 0.07719670236110687, -0.1145344004034996, -0.47248613834381104, 0.24021629989147186, -0.736968994140625, -1.239113211631775, 0.45409613847732544, 0.10593412816524506, 1.233400583267212, 0.447902649641037, -1.9297162294387817, 0.7999812364578247, 0.6600711941719055, 1.0941388607025146, 0.062452852725982666, 0.027025967836380005, -0.22080282866954803, 1.129733681678772, 0.2533450126647949, 0.20894865691661835, -0.724321186542511, -1.2145293951034546, 0.0555441677570343, -0.956276535987854, -0.8460237979888916, 0.21015393733978271, 0.919246256351471 };
	double poseEnd[72] = { 0.30112454295158386, 0.1769694983959198, -0.012414965778589249, 0.0320163331925869, -0.06407471001148224, -0.03512653708457947, -0.49614736437797546, 0.060162078589200974, 0.049079008400440216, -0.19473381340503693, -0.11346104741096497, -0.10543348640203476, 0.06557971239089966, -0.31604647636413574, 0.04056539013981819, 0.14555171132087708, 0.06212051957845688, -0.035065967589616776, -0.17153708636760712, -0.19897328317165375, 0.07511334866285324, 0.4524807929992676, 0.3352327346801758, -0.40288984775543213, 0.13555306196212769, -0.28204402327537537, 0.3330462574958801, -0.1794840395450592, 0.04358832910656929, 0.07896304130554199, -0.30717870593070984, -0.24200063943862915, 0.44992417097091675, -0.13716357946395874, 0.27962905168533325, -0.7264453768730164, 0.4957231283187866, -0.14589932560920715, -0.20724773406982422, 0.4499511420726776, 0.10128620266914368, -0.3182484209537506, 0.36704862117767334, -0.023976324126124382, 0.46231862902641296, 0.13211464881896973, 0.3247305452823639, 0.11494815349578857, 0.17725375294685364, -0.3299679160118103, -1.1604299545288086, 0.19247755408287048, 0.2514416575431824, 0.8298072218894958, 0.2173585444688797, -0.3332197368144989, 0.10697995871305466, 0.32078731060028076, 0.3695584535598755, 0.026443619281053543, 0.3804725706577301, 0.16453686356544495, -0.007566618267446756, 0.5607278347015381, -0.22531093657016754, 0.011505158618092537, -1.047356128692627, -0.44851383566856384, -0.8637425303459167, -1.0021032094955444, 0.39083489775657654, 0.8328424096107483 };
	int counter = 0;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	/// <summary>
	/// 20220408 AppleParfait: ue4 procedural mesh component
	/// </summary>
	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UProceduralMeshComponent* m_proceduralMeshComp;

	/// <summary>
	/// 20220408 AppleParfait: check true when the original static mesh is loaded.
	/// </summary>
	UPROPERTY()
		bool bOriginalMeshLoad = false;

	/// <summary>
	/// 20220408 AppleParfait: check true when vertices of the original static meshes are loaded.
	/// </summary>
	UPROPERTY()
		bool bOriginalVerticesLoad = false;

	/// <summary>
	/// 20220408 AppleParfait: timeSpent for a simple animation.
	/// </summary>
	UPROPERTY()
		float timeSpent = 0.0f;

	/// <summary>
	/// 20220408 AppleParfait: a dictionary for vertices in MeshSection of procedural mesh
	/// </summary>
	TMap<int32, TArray<FVector>> dic_originalVertices;

public:

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/// <summary>
	/// 20220408 AppleParfait: This is called when actor is spawned (at runtime or when you drop it into the world in editor)
	/// </summary>
	virtual void PostActorCreated() override;

	/// <summary>
	/// 20220408 AppleParfait: This is called when actor is already in level and map is opened
	/// </summary>
	virtual void PostLoad() override;

	/// <summary>
	/// 20220408 AppleParfait: Initialize dic_originalVertices
	/// 20220408 AppleParfait: Note that the function named 'CopyProceduralMeshfromStaticMeshComponent' is only available on blueprint.
	/// 20220408 AppleParfait: So, we have to assign 'BlueprintCallable' on the function.
	/// </summary>
	UFUNCTION(BlueprintCallable)
		void InitDicVertices();

private:
	/// <summary>
	/// 20220408 AppleParfait: an example of Creating a triangle by using UProceduralMeshComponent (not a UStaticMeshComp) 
	/// </summary>
	UFUNCTION()
		void CreateTriangle();

	/// <summary>
	/// 20220408 AppleParfait: Change MeshData(only hadling vertices in the example function) for for a simple animation.
	/// </summary>
	UFUNCTION()
		void ChangeProcMeshData(float _deltaTime);
};
