#pragma once

#include <cinttypes>

namespace ubytes
{
namespace app_platform
{

template <typename T>
struct Rect2Base
{
  T x, y, w, h;
};

using Rect2f = Rect2Base<float>;
using Rect2i = Rect2Base<std::int32_t>;
using Rect2u = Rect2Base<std::uint32_t>;

} // namespace app_platform
} // namespace ubytes