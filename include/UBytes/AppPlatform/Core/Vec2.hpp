#pragma once

#include <cinttypes>
#include <algorithm>

namespace ubytes::app_platform
{

template <typename T>
struct Vec2
{
  T x = T();
  T y = T();

  constexpr Vec2() noexcept = default;

  constexpr Vec2(T x, T y) noexcept
    : x(x)
    , y(y)
  {
  }

  template <typename U>
  static auto max(Vec2<T> a, Vec2<U> b) noexcept -> Vec2<T>
  {
    return Vec2<T>(std::max(a.x, T(b.x)), std::max(a.y, T(b.y)));
  }

  template <typename U>
  static auto min(Vec2<T> a, Vec2<U> b) noexcept -> Vec2<T>
  {
    return Vec2<T>(std::min(a.x, T(b.x)), std::min(a.y, T(b.y)));
  }
};

using Vec2i = Vec2<std::int32_t>;
using Vec2u = Vec2<std::uint32_t>;
using Vec2f = Vec2<float>;
using Vec2d = Vec2<double>;

template <typename T>
inline auto operator+(Vec2<T> a, Vec2<T> b) noexcept -> Vec2<T>
{
  return Vec2<T>(a.x + b.x, a.y + b.y);
}

template <typename T>
inline auto operator-(Vec2<T> a, Vec2<T> b) noexcept -> Vec2<T>
{
  return Vec2<T>(a.x - b.x, a.y - b.y);
}

template <typename T>
inline auto operator*(Vec2<T> a, Vec2<T> b) noexcept -> Vec2<T>
{
  return Vec2<T>(a.x * b.x, a.y * b.y);
}

template <typename T>
inline auto operator/(Vec2<T> a, Vec2<T> b) noexcept -> Vec2<T>
{
  return Vec2<T>(a.x / b.x, a.y / b.y);
}

template <typename T>
inline auto operator+(Vec2<T> a, T b) noexcept -> Vec2<T>
{
  return Vec2<T>(a.x + b, a.y + b);
}

template <typename T>
inline auto operator-(Vec2<T> a, T b) noexcept -> Vec2<T>
{
  return Vec2<T>(a.x - b, a.y - b);
}

template <typename T>
inline auto operator*(Vec2<T> a, T b) noexcept -> Vec2<T>
{
  return Vec2<T>(a.x * b, a.y * b);
}

template <typename T>
inline auto operator/(Vec2<T> a, T b) noexcept -> Vec2<T>
{
  return Vec2<T>(a.x / b, a.y / b);
}

} // namespace ubytes::app_platform