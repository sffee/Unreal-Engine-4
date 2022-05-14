#pragma once

#include <Components/Image.h>

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LockOnWidget.generated.h"

UCLASS()
class PORTFOLIO_API ULockOnWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true")) 
	UImage* m_LockOnImage;

public:
	void SetLockOnVisible(bool _Set)
	{
		if (_Set)
			m_LockOnImage->SetVisibility(ESlateVisibility::Visible);
		else
			m_LockOnImage->SetVisibility(ESlateVisibility::Hidden);
	}

public:
	~ULockOnWidget();
	
public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& Geometry, float DT) override;
};