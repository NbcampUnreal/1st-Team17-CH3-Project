#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "Blueprint/UserWidget.h" // UUserWidget 포함
#include "NXGameMode.generated.h"

UCLASS()
class NXPROJECT_API ANXGameMode : public AGameMode
{
    GENERATED_BODY()

public:
    // 게임오버 UI 블루프린트 클래스 설정 (에디터에서 할당 가능)
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
    TSubclassOf<UUserWidget> GameOverWidgetClass;

    // 게임오버 UI 인스턴스를 저장할 변수
    UPROPERTY()
    UUserWidget* CurrentGameOverWidget;

    // 게임오버 UI를 생성 및 표시하는 함수
    UFUNCTION(BlueprintCallable, Category = "UI")
    void ShowGameOverUI();
};
