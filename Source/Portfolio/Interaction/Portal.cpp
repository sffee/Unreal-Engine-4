#include "Portal.h"

APortal::APortal()
{
	m_ParticleComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle"));
	m_ParticleComponent->SetupAttachment(m_Box);

	ConstructorHelpers::FObjectFinder<UParticleSystem> Portal(TEXT("ParticleSystem'/Game/RPGEffects/Particles/P_Status_Teleport_Portal.P_Status_Teleport_Portal'"));
	if (Portal.Succeeded())
	{
		m_ParticleComponent->SetTemplate(Portal.Object);
	}
}

void APortal::Interact()
{
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("Boss"));
}