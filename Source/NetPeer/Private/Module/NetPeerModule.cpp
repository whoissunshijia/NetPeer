// Copyright 2014-2017 Barry, Inc. All Rights Reserved.

#include "NetPeerPrivatePCH.h"

#define LOCTEXT_NAMESPACE "FNetPeerModule"

DEFINE_LOG_CATEGORY(LogNetPeer);

void FNetPeerModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FNetPeerModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FNetPeerModule, NetPeer)