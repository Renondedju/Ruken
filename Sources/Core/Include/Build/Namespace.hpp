#pragma once

#define RUKEN_NAMESPACE ruken
#define USING_RUKEN_NAMESPACE using namespace RUKEN_NAMESPACE;
#define BEGIN_RUKEN_NAMESPACE namespace RUKEN_NAMESPACE {
#define END_RUKEN_NAMESPACE }

#define RUKEN_INTERNAL_NAMESPACE internal

// This avoids msvc error C2871 (or equivalent) to be triggered
namespace RUKEN_NAMESPACE::RUKEN_INTERNAL_NAMESPACE {}