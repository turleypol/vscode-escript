#include "ExtensionConfig.h"
#include "napi.h"

namespace VSCodeEscript
{
ExtensionConfiguration gExtensionConfiguration;

ExtensionConfiguration::ExtensionConfiguration()
    : polCommitId( "" ), showModuleFunctionComments( false ), continueAnalysisOnError( true )
{
}

Napi::Value ExtensionConfiguration::Get( const Napi::CallbackInfo& info )
{
  Napi::Env env = info.Env();

  if ( info.Length() < 1 || !info[0].IsString() )
  {
    Napi::TypeError::New( env, Napi::String::New( env, "Invalid arguments" ) )
        .ThrowAsJavaScriptException();
    return Napi::Value();
  }

  std::string property = info[0].As<Napi::String>().Utf8Value();

  if ( property == "polCommitId" )
  {
    return Napi::String::New( env, gExtensionConfiguration.polCommitId );
  }
  else if ( property == "showModuleFunctionComments" )
  {
    return Napi::Boolean::New( env, gExtensionConfiguration.showModuleFunctionComments );
  }
  else if ( property == "continueAnalysisOnError" )
  {
    return Napi::Boolean::New( env, gExtensionConfiguration.continueAnalysisOnError );
  }
  Napi::TypeError::New( env, Napi::String::New( env, "Invalid arguments" ) )
      .ThrowAsJavaScriptException();
  return Napi::Value();
}

Napi::Value ExtensionConfiguration::SetFromObject( const Napi::CallbackInfo& info )
{
  Napi::Env env = info.Env();

  if ( info.Length() < 1 || !info[0].IsObject() )
  {
    Napi::TypeError::New( env, Napi::String::New( env, "Invalid arguments" ) )
        .ThrowAsJavaScriptException();
    return Napi::Value();
  }

  auto config = info[0].As<Napi::Object>();

  if ( config.Has( "polCommitId" ) )
  {
    auto value = config.Get( "polCommitId" );
    if ( value.IsString() )
    {
      gExtensionConfiguration.polCommitId = value.As<Napi::String>().Utf8Value();
    }
    else
    {
      gExtensionConfiguration.polCommitId = "";
    }
  }

  if ( config.Has( "showModuleFunctionComments" ) )
  {
    auto value = config.Get( "showModuleFunctionComments" );
    if ( value.IsBoolean() )
    {
      gExtensionConfiguration.showModuleFunctionComments = value.As<Napi::Boolean>().Value();
    }
    else
    {
      gExtensionConfiguration.showModuleFunctionComments = false;
    }
  }

  if ( config.Has( "continueAnalysisOnError" ) )
  {
    auto value = config.Get( "continueAnalysisOnError" );
    if ( value.IsBoolean() )
    {
      gExtensionConfiguration.continueAnalysisOnError = value.As<Napi::Boolean>().Value();
    }
    else
    {
      gExtensionConfiguration.continueAnalysisOnError = false;
    }
  }

  return env.Undefined();
}
}  // namespace VSCodeEscript
