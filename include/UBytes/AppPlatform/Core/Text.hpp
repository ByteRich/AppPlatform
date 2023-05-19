#pragma once

#include <string_view>
#include <string>

namespace ubytes
{
namespace app_platform
{
namespace details
{

/// Converts a UTF-8 string to a UCS-2 string.
auto to_wstring(std::u8string_view str) -> std::wstring;

/// Converts a UTF-8 string to a UCS-2 string.
auto to_wstring(std::string_view utf8str) -> std::wstring;

/// Converts a UCS-2 string to a UTF-8 string.
auto to_utf8_string(std::wstring_view wstr) -> std::string;

/// Converts u8string_view to a regular string_view.
/// @note The bytes or addresses are unaffected, only the type is changed.
auto to_regular_sv(std::u8string_view str) -> std::string_view;

} // namespace details
} // namespace app_platform
} // namespace ubytes