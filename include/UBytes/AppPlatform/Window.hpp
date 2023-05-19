#pragma once

#include <memory>
#include <string_view>

#include <UBytes/AppPlatform/Export.hpp>
#include <UBytes/AppPlatform/Core/Vec2.hpp>
#include <UBytes/AppPlatform/Core/Rect.hpp>
#include <UBytes/AppPlatform/Core/Color.hpp>

namespace ubytes
{
namespace app_platform
{

/// An opaque, platform-specific handle to a window.
struct UBYTES_EXPORT WindowHandle
{
  void* handle;
};

/// A native window that can be created and managed by the application.
class UBYTES_EXPORT Window
{
public:
  enum WindowMode
  {
    Normal,
    Maximized,
    Fullscreen,
  };

  /// @brief Creates a window and returns an owned pointer to it.
  template <typename T = Window>
  static auto create() -> std::unique_ptr<T>
  {
    auto window = std::make_unique<T>();
    create(*window);
    return window;
  }

  /// Returns the native handle to the window.
  auto handle() const -> WindowHandle
  {
    return _handle;
  }

  // Appearance

  /// Makes the window borderless (e.g. no title bar, no borders, etc.)
  auto set_borderless() -> void;

  /// Sets the title of the window.
  /// @param title - utf8 encoded string
  auto set_title(std::string_view title) -> void;

  /// Returns the title of the window as an UTF-8 encoded string.
  auto get_title() const -> std::string;

  /// Sets the absolute window size.
  auto set_size(Vec2u size) -> void;

  /// Returns the absolute window size.
  auto get_size() const -> Vec2u;

  /// Sets the absolute position of the window.
  auto set_position(Vec2i position) -> void;

  /// Returns the absolute position of the window.
  auto get_position() const -> Vec2i;

  /// Sets the absolute bounds of the window.
  auto set_bounds(Rect2i bounds) -> void;

  /// Returns the absolute bounds of the window.
  auto get_bounds() const -> Rect2i;

  /// Returns true if the window is maximized.
  auto is_maximized() const -> bool;

  /// Returns true if the window is minimized.
  auto is_minimized() const -> bool;

  /// Returns true if the window is borderless.
  auto is_borderless() const -> bool
  {
    return _borderless;
  }

  /// Returns the client size of the window.
  auto get_inner_size() const -> Vec2u;

  // Requests

  /// Sends a request to close the window.
  auto request_close() -> void;

  /// Sends a request to toggle the window's maximized state.
  auto request_toggle_maximize() -> void;

  /// Sends a request to minimize the window.
  auto request_minimize() -> void;

  // Overridable events

  /// Called when the window is resized.
  /// @param new_size The new size of the window.
  virtual auto on_resize(Rect2i new_size) -> void
  {
  }

  /// The color used to clear the window.
  /// By default, it is black.
  Color4f background_color = Color4f(0, 0, 0);

private:
  /// Creates a native window using already constructed Window object.
  static auto create(Window& window) -> void;

  WindowHandle _handle;

  bool _borderless = false;
};

} // namespace app_platform
} // namespace ubytes