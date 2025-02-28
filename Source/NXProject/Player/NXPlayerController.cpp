#include "Player/NXPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"
#include "Player/NXPlayerCharacter.h"

ANXPlayerController::ANXPlayerController()
	:InputMappingContext(nullptr),
    MoveAction(nullptr),
    JumpAction(nullptr),
    LookAction(nullptr),
    SprintAction(nullptr),
    InteractAction(nullptr),
    CrouchAction(nullptr),
    UnCrouchAction(nullptr)
    
{

}

void ANXPlayerController::BeginPlay()
{
    Super::BeginPlay();



    // 현재 PlayerController에 연결된 Local Player 객체를 가져옴    
    if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
    {
        // Local Player에서 EnhancedInputLocalPlayerSubsystem을 획득
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
        {
            if (InputMappingContext)
            {
                // Subsystem을 통해 우리가 할당한 IMC를 활성화
                // 우선순위(Priority)는 0이 가장 높은 우선순위
                Subsystem->AddMappingContext(InputMappingContext, 0);
            }
        }
    }

    if (HUDWidgetClass)
    {
        UUserWidget* HUDWidget = CreateWidget<UUserWidget>(this, HUDWidgetClass);
        if (HUDWidget)
        {
            HUDWidget->AddToViewport();

          
            ANXPlayerCharacter* PlayerCharacter = Cast<ANXPlayerCharacter>(GetPawn());
            if (PlayerCharacter)
            {
                PlayerCharacter->SetHUDWidget(HUDWidget);
            }
        }
    }
}
