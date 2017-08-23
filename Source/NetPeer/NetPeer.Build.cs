// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.IO;

public class NetPeer : ModuleRules
{
	 private string ModulePath
    {
        get { return ModuleDirectory; }
    }
	
	private string ThirdPartyPath
    {
        get { return Path.GetFullPath(Path.Combine(ModulePath, "../../ThirdParty/")); }
    }
	
	public NetPeer(ReadOnlyTargetRules Target):base(Target)
	{
		string mpackIncPath = ThirdPartyPath + "mpackLib";

        string MessagePackIncPath = ThirdPartyPath + "MessagePack/";
		
		PublicIncludePaths.AddRange(
			new string[] {				
				"NetPeer/Public/Module",
				"NetPeer/Public/Net",
				"Runtime/Networking/Public",
				mpackIncPath,
				MessagePackIncPath			
				
				// ... add public include paths required here ...
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				"NetPeer/Private/Module",
				"NetPeer/Private/Net",
				"NetPeer/Private/MessagePack"
				// ... add other private include paths required here ...
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Sockets",
				"Networking"
				// ... add private dependencies that you statically link with here ...	
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
	}
}
