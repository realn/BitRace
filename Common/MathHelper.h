#pragma once

namespace math {
  template<typename _Type>
  inline const bool inrange(const _Type& x, const _Type& min, const _Type& max) {
    return x >= min && x <= max;
  }
}