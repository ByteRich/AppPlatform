#pragma once

#include <UBytes/AppPlatform/Export.hpp>

#include <UBytes/AppPlatform/Window.hpp>
#include <UBytes/AppPlatform/Core/Rect.hpp>
#include <UBytes/AppPlatform/Core/Color.hpp>

#include <functional>
#include <array>
#include <string>
#include <string_view>

namespace ubytes
{
namespace app_platform
{

class Window;

namespace details
{

/// Internal, OS-based information about the webview.
/// At the header level the information is opaque not to pollute other headers.
struct WebViewOpaque
{
  std::array<char, sizeof(void*) * 2> _data;
};
static_assert(sizeof(WebViewOpaque) == 2 * sizeof(void*), "WebViewOpaque size is invalid");

} // namespace details


/// A webview that can be used to display web content.
class UBYTES_EXPORT WebView
{
public:
  enum class FocusReason
  {
    Programmatic,
    TabNext,
    TabPrev,
  };

  // Event handlers

  /// Called when the WebView is ready to be set up and used.
  /// Do not call methods that aren't marked as "pre-ready" before this event.
  std::function<void()> on_ready;

  /// Called when a message is received from the WebView.
  /// The message is a view over UTF-8 encoded string.
  std::function<void(std::string)> on_message;

  // Methods

  /// Constructs a new, valid webview handler without constructing the internal
  /// webview object. Use `begin_setup()` to start the construction and wait for
  /// the `on_ready` to be called.
  /// @note Make sure to properly bind the `on_ready` event handler before.
  WebView();

  /// Destroys the webview.
  ~WebView();

  /// Move constructor
  WebView(WebView&& other) noexcept;

  /// Move assignment
  auto operator=(WebView&& other) noexcept -> WebView&;

  /// Copy constructor (not allowed)
  WebView(WebView const& other) noexcept = delete;

  /// Copy assignment (not allowed)
  auto operator=(WebView const& other) noexcept -> WebView& = delete;

  /// Starts the setup of the WebView.
  /// @note [pre-ready]
  /// The WebView cannot be moved before the setup is finished.
  auto begin_setup(Window const& window) -> void;

  /// Determines whether the WebView is ready to be used.
  /// @note [pre-ready]
  auto setup_finished() const -> bool
  {
    return _setup_finished;
  }

  /// Sets the bounds of the WebView relative to the parent window work area.
  auto set_bounds(Rect2i bounds) -> void;

  /// Sets the background color of the WebView.
  /// @param color The target color.
  /// @note This may not be fully supported on all platforms.
  /// Status:
  /// - WebView2 (Windows): transparency works only if alpha is 0.
  ///
  /// As a workaround you may use setTransparentBackground(true) and
  /// set the HTML body background color to the desired value - this way
  /// you can achieve any transparency you want.
  auto set_background_color(Color4f color) -> void;

  /// Sets whether the WebView should have a fully transparent background.
  auto set_transparent_background(bool enabled) -> void;

  /// Moves the focus to the WebView.
  /// @param reason The reason for the focus change.
  auto focus(FocusReason reason = FocusReason::Programmatic) -> void;

  /// Changes the parent of the WebView to another window.
  auto set_parent_window(WindowHandle window) -> void;

  /// Navigates to the specified UTF-8 encoded URL.
  auto navigate(std::string_view url) -> void;

  /// Navigates to the specified UTF-8 encoded URL.
  auto navigate(std::u8string_view url) -> void;

  /// Navigates to the specified UCS-2 encoded URL.
  /// @note This function is a faster alternative to the UTF-8 version on Windows
  /// because it doesn't need to convert the string to UTF-8.
  auto navigate(std::wstring const& url) -> void;

  details::WebViewOpaque _opaque;
private:

  bool _setup_finished = false;
};


} // namespace app_platform
} // namespace ubytes