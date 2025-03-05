#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "MyUserWidget.generated.h"

UCLASS()
class NXPROJECT_API UMyUserWidget : public UUserWidget
{
    GENERATED_BODY() 

public:
    UFUNCTION(BlueprintCallable, Category = "UI")
    void StartTypingEffect(const FString& FullText, float Delay);

private:
    void UpdateTypingText();

    FString OriginalText;
    FString CurrentText;
    int32 CurrentIndex;

    FTimerHandle TypingTimerHandle;

protected:
    virtual void NativeConstruct() override;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* TextBlock;
};
