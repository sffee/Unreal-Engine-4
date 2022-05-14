#include "InteractionBase.h"

#include "../UI/Interaction/InteractionWidget.h"
#include "../Player/MyPlayer.h"

AInteractionBase::AInteractionBase()
	: m_IsOn(false)
{
	PrimaryActorTick.bCanEverTick = true;

	m_Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	SetRootComponent(m_Box);

	m_Box->SetBoxExtent(FVector(200.f, 200.f, 200.f));
	m_Box->SetCollisionProfileName(TEXT("Interaction"));

	m_WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
	m_WidgetComponent->SetupAttachment(m_Box);

	ConstructorHelpers::FClassFinder<UUserWidget> InteractionUI(TEXT("WidgetBlueprint'/Game/BlueprintClass/UI/Interaction/WBP_Interaction.WBP_Interaction_C'"));
	if (InteractionUI.Succeeded())
	{
		m_WidgetComponent->SetWidgetClass(InteractionUI.Class);
	}

	m_WidgetComponent->SetDrawSize(FVector2D(100.f, 100.f));
}

void AInteractionBase::BeginPlay()
{
	Super::BeginPlay();
	
	m_Box->OnComponentBeginOverlap.AddDynamic(this, &AInteractionBase::OnBeginOverlap);
	m_Box->OnComponentEndOverlap.AddDynamic(this, &AInteractionBase::OnEndOverlap);
}

void AInteractionBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (m_IsOn)
	{
		AMyPlayer* Player = Cast<AMyPlayer>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn());
		FRotator PlayerRot = Player->GetControlRotation();
		PlayerRot.Pitch = 0.f;
		PlayerRot.Yaw -= 180.f;
		PlayerRot.Roll = 0.f;

		m_Box->SetWorldRotation(PlayerRot);
	}
}

void AInteractionBase::OnBeginOverlap(UPrimitiveComponent* _PrimitiveComponent, AActor* _OtherActor, UPrimitiveComponent* _OtherComp, int32 _OtherBodyIndex, bool _bFromSweep, const FHitResult& _SweepResult)
{
	if (_OtherComp->GetCollisionObjectType() == ECC_GameTraceChannel2)
	{
		Cast<UInteractionWidget>(m_WidgetComponent->GetWidget())->SetVisible(true);
		m_IsOn = true;
	}
}

void AInteractionBase::OnEndOverlap(UPrimitiveComponent* _PrimitiveComponent, AActor* _OtherActor, UPrimitiveComponent* _OtherComp, int32 _OtherBodyIndex)
{
	if (_OtherComp->GetCollisionObjectType() == ECC_GameTraceChannel2)
	{
		m_IsOn = false;
		UInteractionWidget* Widget = Cast<UInteractionWidget>(m_WidgetComponent->GetWidget());

		if (Widget != nullptr)
		{
			Widget->SetVisible(false);
		}
	}
}