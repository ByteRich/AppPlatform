#pragma once

#include <UBytes/AppPlatform/Export.hpp>

#include <cinttypes>

namespace ubytes
{
namespace app_platform
{

namespace details
{

template <typename T>
struct ColorTypeProperties
{
};

template <>
struct ColorTypeProperties<float>
{
  static auto constexpr FULL_INTENSITY = float(1.0f);

  static auto from_byte(std::uint8_t byte) noexcept -> float
  {
    return float(byte) / 255.0f;
  }

  static auto to_byte(float value) noexcept -> std::uint8_t
  {
    return std::uint8_t(value * 255.0f);
  }
};

template <>
struct ColorTypeProperties<std::uint8_t>
{
  static auto constexpr FULL_INTENSITY = std::uint8_t(255);

  static auto from_byte(std::uint8_t byte) noexcept -> std::uint8_t
  {
    return byte;
  }

  static auto to_byte(std::uint8_t value) noexcept -> std::uint8_t
  {
    return value;
  }
};

template <typename T>
struct ColorBaseRGBA;

template <typename T>
struct ColorBaseRGB
{
  using Props = ColorTypeProperties<T>;

  T r, g, b;

  constexpr ColorBaseRGB() noexcept
    : r(0)
    , g(0)
    , b(0)
  {
  }

  constexpr ColorBaseRGB(T r, T g, T b) noexcept
    : r(r)
    , g(g)
    , b(b)
  {
  }

  static auto constexpr from_hex(std::uint32_t hex) noexcept -> ColorBaseRGB
  {
    auto color = ColorBaseRGB();
    color.set_hex(hex);
    return color;
  }

  static auto constexpr from_rgb(std::uint8_t r, std::uint8_t g, std::uint8_t b) noexcept -> ColorBaseRGB
  {
    auto color = ColorBaseRGBA();
    color.set_rgb(r, g, b);
    return color;
  }

  auto constexpr set(T r, T g, T b) noexcept -> void
  {
    this->r = r;
    this->g = g;
    this->b = b;
  }

  auto constexpr set_rgb(std::uint8_t r, std::uint8_t g, std::uint8_t b) noexcept -> void
  {
    this->set(Props::from_byte(r), Props::from_byte(g), Props::from_byte(b));
  }

  auto constexpr set_hex(std::uint32_t hex) noexcept -> void
  {
    this->set_rgb((hex >> 24) & 0xFF, (hex >> 16) & 0xFF, (hex >> 8) & 0xFF);
  }

  auto constexpr to_hex() const noexcept -> std::uint32_t
  {
    return (std::uint32_t(Props::to_byte(r)) << 24) | (std::uint32_t(Props::to_byte(g)) << 16) |
           (std::uint32_t(Props::to_byte(b)) << 8) | (uint8_t(0xFF) << 0);
  }

  auto constexpr to_rgba() const noexcept -> ColorBaseRGBA<T>
  {
    return ColorBaseRGBA<T>(r, g, b, Props::FULL_INTENSITY);
  }
};

template <typename T>
struct ColorBaseRGBA
{
  using Props = ColorTypeProperties<T>;

  T r, g, b, a;

  /// Constructs a black, opaque color.
  constexpr ColorBaseRGBA() noexcept
    : r(0)
    , g(0)
    , b(0)
    , a(Props::FULL_INTENSITY)
  {
  }

  /// Constructs a color with the given components, by default opaque.
  constexpr ColorBaseRGBA(T r, T g, T b, T a = Props::FULL_INTENSITY) noexcept
    : r(r)
    , g(g)
    , b(b)
    , a(a)
  {
  }

  /// Constructs a color from a hex value (integer, e.g. 0xFF00FFFF).
  /// @note Do not forget about the alpha channel.
  static auto constexpr from_hex(std::uint32_t hex) noexcept -> ColorBaseRGBA
  {
    auto color = ColorBaseRGBA();
    color.set_hex(hex);
    return color;
  }

  /// Constructs an opaque color from RGB byte (uint8_t) components.
  static auto constexpr from_rgb(std::uint8_t r, std::uint8_t g, std::uint8_t b) noexcept -> ColorBaseRGBA
  {
    auto color = ColorBaseRGBA();
    color.set_rgb(r, g, b);
    return color;
  }

  /// Constructs a color from RGBA byte (uint8_t) components.
  static auto constexpr from_rgba(std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a) noexcept -> ColorBaseRGBA
  {
    auto color = ColorBaseRGBA();
    color.set_rgba(r, g, b, a);
    return color;
  }

  /// Sets the color to the given components, by default opaque.
  auto constexpr set(T r, T g, T b, T a = Props::FULL_INTENSITY) noexcept -> void
  {
    this->r = r;
    this->g = g;
    this->b = b;
    this->a = a;
  }

  /// Sets the color to the given RGB byte (uint8_t) components.
  auto constexpr set_rgb(std::uint8_t r, std::uint8_t g, std::uint8_t b) noexcept -> void
  {
    this->set(Props::from_byte(r), Props::from_byte(g), Props::from_byte(b));
  }

  /// Sets the color to the given RGBA byte (uint8_t) components.
  auto constexpr set_rgba(std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a) noexcept -> void
  {
    this->set_rgb(r, g, b);
    this->a = Props::from_byte(a);
  }

  /// Sets the color to the given hex value (integer, e.g. 0xFF00FFFF).
  /// @note Do not forget about the alpha channel.
  auto constexpr set_hex(std::uint32_t hex) noexcept -> void
  {
    this->set_rgba((hex >> 24) & 0xFF, (hex >> 16) & 0xFF, (hex >> 8) & 0xFF, (hex >> 0) & 0xFF);
  }

  /// Returns the color as a hex value (integer, e.g. 0xFF00FFFF).
  auto constexpr to_hex() const noexcept -> std::uint32_t
  {
    return (std::uint32_t(Props::to_byte(r)) << 24) | (std::uint32_t(Props::to_byte(g)) << 16) |
           (std::uint32_t(Props::to_byte(b)) << 8) | (std::uint32_t(Props::to_byte(a)) << 0);
  }

  /// Returns the color as a RGB color (alpha channel is discarded).
  auto constexpr to_rgb() const noexcept -> ColorBaseRGB<T>
  {
    return ColorBaseRGB<T>(r, g, b);
  }
};

template <typename T, bool AlphaChannel>
struct ColorBaseInternal;

template <typename T>
struct ColorBaseInternal<T, true> : public ColorBaseRGBA<T>
{
  using Super = ColorBaseRGBA<T>;
  using Super::Super;
};

template <typename T>
struct ColorBaseInternal<T, false> : public ColorBaseRGB<T>
{
  using Super = ColorBaseRGB<T>;
  using Super::Super;
};

} // namespace details

/// @brief Defines a RGB or RGBA color
/// @tparam T - The type of each component
/// @tparam AlphaChannel - Whether the color has an alpha channel.
/// @note Use the provided aliases for common color types (Color4f, Color3f, Color4u, Color3u, etc.)
template <typename T, bool AlphaChannel>
struct UBYTES_EXPORT ColorBase : public details::ColorBaseInternal<T, AlphaChannel>
{
  using Super = details::ColorBaseInternal<T, AlphaChannel>;
  using Super::Super;
};

/// RGBA color with each component as a float (0 - 1 value range)
using Color4f = ColorBase<float, true>;
/// RGB color with each component as a float (0 - 1 value range)
using Color3f = ColorBase<float, false>;

/// RGBA color with each component as a byte (uint8_t, 0 - 255 value range)
using Color4u = ColorBase<std::uint8_t, true>;

/// RGB color with each component as a byte (uint8_t, 0 - 255 value range)
using Color3u = ColorBase<std::uint8_t, false>;

} // namespace app_platform
} // namespace ubytes