//------------------------------------------------------------------------------
// CLING - the C++ LLVM-based InterpreterG :)
//
// This file is dual-licensed: you can choose to license it under the University
// of Illinois Open Source License or the GNU Lesser General Public License. See
// LICENSE.TXT for details.
//------------------------------------------------------------------------------

#ifndef LLVM_CLING_RUNTIME_H
#define LLVM_CLING_RUNTIME_H

namespace cling {

/// \brief When CLING_CLANG_RUNTIME_PATCH is enabled, the code changes cling
/// requires to the clang library are handled at runtime, not compile time.
/// ABI compatibility will still be broken (member's and arguments have been
/// added), but should allow for other clients to be re-compile and linked with
/// the shared libraries from cling and get compliant parsing & code-generation.

#ifndef CLING_CLANG_RUNTIME_PATCH
  constexpr bool isClient() { return true; }
 #ifdef ROOT_BINARY_DIR
  constexpr bool isROOT() { return true; }
 #else
  constexpr bool isROOT() { return false; }
 #endif

  // Compile time error...unless someone's playing games
  template <class T> void setClientFlags(T val) {
    val.CLING_CLANG_RUNTIME_PATCH_IS_NOT_DEFINED();
  }
#else
  enum ClientFlags {
    kClingIsNotHost,
    kClingIsHost,
    kRootIsHost,
    kClingIsInteractive,
  };

  /// May go away. So maybe don't use this directly?
  namespace dont { namespace use { namespace directly {
    extern ClientFlags gClingRuntimeFlags;
  } } }

  /// \brief Returns whether cling is the client using the clang libraries.
  /// When CLING_CLANG_RUNTIME_PATCH is not defined isClient should be a nop
  inline ClientFlags isClient() { return dont::use::directly::gClingRuntimeFlags; }
  inline bool isROOT() { return dont::use::directly::gClingRuntimeFlags & kRootIsHost; }

  /// \brief Call to set cling as the hosting process for the libraries.
  /// It's a very short function, but we want to force it to link.
  ///
  /// \returns The previous flags.
  ClientFlags setClientFlags(ClientFlags NewFlags);
#endif


} // namespace cling

#endif
