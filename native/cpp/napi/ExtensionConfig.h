#pragma once

#include <map>
#include <napi.h>
#include <vector>

namespace VSCodeEscript
{
class ExtensionConfiguration
{
public:
  ExtensionConfiguration();

  static Napi::Value SetFromObject( const Napi::CallbackInfo& info );
  static Napi::Value Get( const Napi::CallbackInfo& info );

  std::string polCommitId;
  bool showModuleFunctionComments;
  bool continueAnalysisOnError;
};

extern ExtensionConfiguration gExtensionConfiguration;
}  // namespace VSCodeEscript
