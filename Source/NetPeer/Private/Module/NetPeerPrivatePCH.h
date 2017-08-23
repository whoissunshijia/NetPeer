// Copyright 2014-2017 Barry, Inc. All Rights Reserved.

#include "NetPeerModule.h"

// You should place include statements to your module's private header files here.  You only need to
// add includes for headers that are used in most of your module's source files though.
#include "Engine.h"
#include "Core.h"
#include "CoreUObject.h"

#define WITH_MSGPACK_CPLUSPLUS 0

#define MSGPACK_USE_CPP03
#define MSGPACK_DISABLE_LEGACY_NIL