#pragma once

#include <memory>

#include <UBytes/AppPlatform/Export.hpp>
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

  /// Returns true if the window is borderless.
  auto is_borderless() const -> bool
  {
    return _borderless;
  }

  /// Returns the client size of the window.
  auto get_inner_size() const -> Rect2u;

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
  virtual auto on_resize(Rect2u new_size) -> void
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