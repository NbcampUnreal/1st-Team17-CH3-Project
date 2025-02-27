// NXProject.Build.cs

using UnrealBuildTool;

public class NXProject : ModuleRules
{
	public NXProject(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[]
		{
			// Initial Dependencies
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
            "AIModule",				//AI 모듈
			"NavigationSystem",		//네비게이션 시스템 모듈
			"GameplayTasks",		//비헤이비어 트리에서 블랙보드 값 갱신을 위한 모듈
			"UMG"

		});

		PrivateDependencyModuleNames.AddRange(new string[] {  });
		
		PublicIncludePaths.AddRange(new string[] { "NXProject" });
	}
}
