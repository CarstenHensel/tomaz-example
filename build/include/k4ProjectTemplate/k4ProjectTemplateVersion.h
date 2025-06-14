#ifndef k4ProjectTemplate_VERSION_H
#define k4ProjectTemplate_VERSION_H

#include <cstdint>
#include <tuple>
#include <ostream>
#if __cplusplus >= 202002L
#include <compare>
#endif

// Some preprocessor constants and macros for the use cases where they might be
// necessary

/// Define a version to be used in k4ProjectTemplate.
#define k4ProjectTemplate_GET_VERSION(major, minor, patch) (((unsigned long)(major) << 32) | ((unsigned long)(minor) << 16) | ((unsigned long)(patch)))
/// Get the major version from a preprocessor defined version
#define k4ProjectTemplate_GET_MAJOR_VERSION(v) (((v) & (-1UL >> 16)) >> 32)
/// Get the minor version from a preprocessor defined version
#define k4ProjectTemplate_GET_MINOR_VERSION(v) (((v) & (-1UL >> 32)) >> 16)
/// Get the patch version from a preprocessor defined version
#define k4ProjectTemplate_GET_PATCH_VERSION(v) ((v) & (-1UL >> 48))

// Some helper constants that are populated by the cmake configure step
#define k4ProjectTemplate_VERSION_MAJOR 1
#define k4ProjectTemplate_VERSION_MINOR 2
#define k4ProjectTemplate_VERSION_PATCH 3
#define k4ProjectTemplate_VERSION 1.2.3

/// The encoded version with which k4ProjectTemplate has been built
#define k4ProjectTemplate_BUILD_VERSION k4ProjectTemplate_VERSION(k4ProjectTemplate_VERSION_MAJOR, k4ProjectTemplate_VERSION_MINOR, k4ProjectTemplate_VERSION_PATCH)

namespace k4ProjectTemplate::version {

  /**
   * Version class consisting of 3 16 bit unsigned integers to hold the major,
   * minor and patch version. Provides constexpr comparison operators that allow
   * to use this class in constexpr-if clauses.
   */
  struct Version {
    uint16_t major{0};
    uint16_t minor{0};
    uint16_t patch{0};

#if __cplusplus >= 202002L
    auto operator<=>(const Version&) const = default;
#else
// No spaceship yet in c++17
#define DEFINE_COMP_OPERATOR(OP)                                                   \
    constexpr bool operator OP(const Version& o) const noexcept {                  \
      return std::tie(major, minor, patch) OP std::tie(o.major, o.minor, o.patch); \
    }

    DEFINE_COMP_OPERATOR(<)
    DEFINE_COMP_OPERATOR(<=)
    DEFINE_COMP_OPERATOR(>)
    DEFINE_COMP_OPERATOR(>=)
    DEFINE_COMP_OPERATOR(==)
    DEFINE_COMP_OPERATOR(!=)

#undef DEFINE_COMP_OPERATOR
#endif

    friend std::ostream& operator<<(std::ostream&, const Version& v);
  };

  inline std::ostream& operator<<(std::ostream& os, const Version& v) {
    return os << v.major << "." << v.minor << "." << v.patch;
  }

  /**
   * The current build version
   */
  static constexpr Version build_version{k4ProjectTemplate_VERSION_MAJOR, k4ProjectTemplate_VERSION_MINOR, k4ProjectTemplate_VERSION_PATCH};

  /**
   * Decode a version from a 64 bit unsigned
   */
  static constexpr Version decode_version(unsigned long version) noexcept {
    return Version{
      (uint16_t) k4ProjectTemplate_GET_MAJOR_VERSION(version),
      (uint16_t) k4ProjectTemplate_GET_MINOR_VERSION(version),
      (uint16_t) k4ProjectTemplate_GET_PATCH_VERSION(version)
    };
  }
}


#endif
