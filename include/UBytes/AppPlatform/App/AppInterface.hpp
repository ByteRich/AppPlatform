#pragma once

#include <UBytes/AppPlatform/Export.hpp>

namespace ubytes
{
namespace app_platform
{

class UBYTES_EXPORT AppInterface
{
public:
  virtual auto on_start() -> void = 0;
};


/// Runs the application in a default way.
UBYTES_EXPORT auto run_default(AppInterface& app) -> int;

} // namespace app_platform
} // namespace ubytes