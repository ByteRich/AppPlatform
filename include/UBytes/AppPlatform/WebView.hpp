#pragma once

#include <UBytes/AppPlatform/Export.hpp>

#include <UBytes/AppPlatform/Window.hpp>
#include <UBytes/AppPlatform/Core/Rect.hpp>
#include <UBytes/AppPlatform/Core/Color.hpp>
#include <UBytes/AppPlatform/Core/Keyboard.hpp>

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

struct WebViewPermissionOpaque
{
  std::array<char, sizeof(void*) * 2> _data;
};
static_assert(sizeof(WebViewPermissionOpaque) == 2 * sizeof(void*), "WebViewPermissionOpaque size is invalid");

} // namespace details

struct UBYTES_EXPORT WebViewSettings
{
  struct Features
  {
    /// Controls [the elastic overscroll](https://tinyurl.com/yck9kdyt) feature.
    bool elastic_overscroll = false;

    /// Controls the following CSS feature:
    ///
    /// ```css
    /// app-region: drag;
    /// ```
    /// This will enable your app window to be draggable using the title bar or
    /// any other element with the `app-region: drag` CSS property.
    bool draggable_regions = true;
  } features;
};

/// A webview that can be used to display web content.
class UBYTES_EXPORT WebView
{
public:
  struct Permission
  {
    enum Kind
    {
      Unknown = 0,
      Microphone,
      Camera,
      Geolocation,
      Notifications,
      OtherSensors,
      ClipboardRead,
      MultipleAutomaticDownloads,
      FileReadWrite,
      Autoplay,
      LocalFonts,
      MidiSystemExclusiveMessages,
      WindowManagement,
    };

    enum Response
    {
      Default = 0,
      Allow,
      Deny,
    };

    class Request final
    {
    public:
      Request();
      ~Request();

      auto get_kind() const -> Kind;
      auto get_response() const -> Response;
      auto get_saves_in_profile() const -> bool;
      auto is_user_initiated() const -> bool;
      auto get_url() const -> std::string;

      auto set_response(Response state) const -> void;
      auto set_saves_in_profile(bool save) const -> void;

      auto mark_completed() const -> void;

      details::WebViewPermissionOpaque _opaque;
    };
  };

  enum class FocusReason
  {
    Programmatic,
    TabNext,
    TabPrev,
  };

  struct AcceleratorKey
  {
    Keyboard::Key key;
    bool          ctrl         = false;
    bool          shift        = false;
    bool          alt          = false;
    int           repeat_count = 1;
  };

  // Event handlers

  /// Called when the WebView is ready to be set up and used.
  /// Do not call methods that aren't marked as "pre-ready" before this event.
  std::function<void()> on_ready;

  /// Called when a message is received from the WebView.
  /// The message is a view over UTF-8 encoded string.
  std::function<void(std::string)> on_message;

  /// Called when user presses a key combination that is
  /// registered as an "accelerator" sequence.
  std::function<void(AcceleratorKey)> on_accelerator_key;

  /// Called when the WebView requests a permission (like microphone or local file access).
  std::function<void(Permission::Request)> on_permission_request;

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
  auto begin_setup(Window const& window, WebViewSettings settings = {}) -> void;

  /// Starts the setup of the WebView using a native window handle.
  /// @note [pre-ready]
  /// The WebView cannot be moved before the setup is finished.
  auto begin_setup(WindowHandle window_handle, WebViewSettings settings = {}) -> void;

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

  // Navigating to URLs

  /// Navigates to the specified UTF-8 encoded URL.
  auto navigate(std::string_view url) -> void;

  /// Navigates to the specified UTF-8 encoded URL.
  auto navigate(std::u8string_view url) -> void;

  /// Navigates to the specified UCS-2 encoded URL.
  /// @note This function is a faster alternative to the UTF-8 variant on Windows
  /// because it doesn't need to convert the string to UTF-8.
  auto navigate(std::wstring_view url) -> void;

  // Sending JSON messages

  /// Sends a JSON message to the WebView JS window.
  /// @param message The message to send - must be UTF-8, null-terminated string slice containing a valid JSON.
  auto send_message(std::string_view message) -> void;

  /// Sends a JSON message to the WebView JS window.
  /// @param message The message to send - must be UTF-8, null-terminated string slice containing a valid JSON.
  auto send_message(std::u8string_view message) -> void;

  /// Sends a JSON message to the WebView JS window.
  /// @param message The message to send - must be UCS-2, null-terminated string slice containing a valid JSON.
  /// @note This function is a faster alternative to the UTF-8 variant on Windows
  /// because it doesn't need to convert the string to UTF-8.
  auto send_message(std::wstring_view message) -> void;

  // Sending raw messages

  /// Sends a string message to the WebView JS window.
  /// @param message The message to send - must be UTF-8, null-terminated string slice.
  auto send_message_str(std::string_view message) -> void;

  /// Sends a string message to the WebView JS window.
  /// @param message The message to send - must be UTF-8, null-terminated string slice.
  auto send_message_str(std::u8string_view message) -> void;

  /// Sends a string message to the WebView JS window.
  /// @param message The message to send - must be UCS-2, null-terminated string slice.
  /// @note This function is a faster alternative to the UTF-8 variant on Windows
  /// because it doesn't need to convert the string to UTF-8.
  auto send_message_str(std::wstring_view message) -> void;

  details::WebViewOpaque _opaque;

private:
  bool _setup_finished = false;
};


} // namespace app_platform
} // namespace ubytes