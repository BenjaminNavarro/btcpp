module;

/*** This part has been extracted from boost/core/demangle.hpp ***/

// __has_include is currently supported by GCC and Clang. However GCC 4.9 may
// have issues and returns 1 for 'defined( __has_include )', while
// '__has_include' is actually not supported:
// https://gcc.gnu.org/bugzilla/show_bug.cgi?id=63662
#if defined(__has_include) && (!defined(BOOST_GCC) || (__GNUC__ + 0) >= 5)
#if __has_include(<cxxabi.h>)
#define HAS_CXXABI_H
#endif
#elif defined(__GLIBCXX__) || defined(__GLIBCPP__)
#define HAS_CXXABI_H
#endif

#if defined(HAS_CXXABI_H)
#include <cxxabi.h>
// For some archtectures (mips, mips64, x86, x86_64) cxxabi.h in Android NDK is
// implemented by gabi++ library
// (https://android.googlesource.com/platform/ndk/+/master/sources/cxx-stl/gabi++/),
// which does not implement abi::__cxa_demangle(). We detect this implementation
// by checking the include guard here.
#if defined(__GABIXX_CXXABI_H__)
#undef HAS_CXXABI_H
#else
#include <cstdlib>
#include <cstddef>
#endif
#endif

/*** original code ***/

export module btcpp:demangle;

import std;

export namespace btcpp {
std::string demangle(const char* name) {
#if defined(HAS_CXXABI_H)
    int status{};

    std::unique_ptr<char, decltype(&std::free)> res{
        abi::__cxa_demangle(name, nullptr, nullptr, &status), std::free};

    return status == 0 ? res.get() : name;
#else
    return std::string{name};
#endif
}

} // namespace btcpp
