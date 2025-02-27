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
            "AIModule",				//AI ���
			"NavigationSystem",		//�׺���̼� �ý��� ���
			"GameplayTasks",		//�����̺�� Ʈ������ ������ �� ������ ���� ���
			"UMG"

		});

		PrivateDependencyModuleNames.AddRange(new string[] {  });
		
		PublicIncludePaths.AddRange(new string[] { "NXProject" });
	}
}
