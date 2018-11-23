// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#ifndef FUNG_UTIL_INDEXEDTYPE_HH
#define FUNG_UTIL_INDEXEDTYPE_HH

namespace FunG
{
  /// @cond
  template <class Type, int id>
  struct IndexedType
  {
    using type = Type;
    static constexpr int index = id;
  };
  /// @endcond
}

#endif // FUNG_UTIL_INDEXEDTYPE_HH
