#pragma once

#if defined _WIN32 || defined __CYGWIN__ || defined __MINGW32__
#ifdef UBYTES_APP_PLATFORM_BUILDING_DLL
#ifdef __GNUC__
#ifndef UBYTES_EXPORT
#define UBYTES_EXPORT __attribute__((dllexport))
#endif
#else
#ifndef UBYTES_EXPORT
#define UBYTES_EXPORT __declspec(dllexport)
#endif
#endif
#else
#ifdef __GNUC__
#ifndef UBYTES_EXPORT
#define UBYTES_EXPORT __attribute__((dllimport))
#endif
#else
#ifndef UBYTES_EXPORT
#define UBYTES_EXPORT __declspec(dllimport)
#endif
#endif
#endif
#ifndef UBYTES_NOEXPORT
#define UBYTES_NOEXPORT
#endif
#else
#if __GNUC__ >= 4
#ifndef UBYTES_EXPORT
#define UBYTES_EXPORT __attribute__((visibility("default")))
#endif
#ifndef UBYTES_NOEXPORT
#define UBYTES_NOEXPORT __attribute__((visibility("hidden")))
#endif
#else
#ifndef UBYTES_EXPORT
#define UBYTES_EXPORT
#endif
#ifndef UBYTES_NOEXPORT
#define UBYTES_NOEXPORT
#endif
#endif
#endif