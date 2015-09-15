/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                             */
/*   This file is part of the C++-library FunG.                              */
/*   Copyright 2015 Lars Lubkoll                                               */
/*                                                                             */
/*   FunG is free software: you can redistribute it and/or modify            */
/*   it under the terms of the GNU General Public License as published by      */
/*   the Free Software Foundation, either version 3 of the License, or         */
/*   (at your option) any later version.                                       */
/*                                                                             */
/*   FunG is distributed in the hope that it will be useful,                 */
/*   but WITHOUT ANY WARRANTY; without even the implied warranty of            */
/*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             */
/*   GNU General Public License for more details.                              */
/*                                                                             */
/*   You should have received a copy of the GNU General Public License         */
/*   along with FunG.  If not, see <http://www.gnu.org/licenses/>.           */
/*                                                                             */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef FUNG_LINEAR_ALGEBRA_INVARIANTS_HH
#define FUNG_LINEAR_ALGEBRA_INVARIANTS_HH

#include "mixed_invariants.hh"
#include "modified_mixed_invariants.hh"
#include "modified_principal_invariants.hh"
#include "principal_invariants.hh"

namespace FunG
{
  namespace LinearAlgebra
  {
    /**
     * \ingroup InvariantGroup
     * \brief Enum to statically choose invariant traits.
     */
    enum class Invariant { PRINCIPAL, MIXED, MODIFIED, MODIFIED_MIXED };

    /**
     * \cond DOCUMENT_IMPLEMENTATION_DETAILS
     */
    template <Invariant> struct InvariantTraits;
    /**
     * \endcond
     */

    /**
     * \ingroup InvariantGroup
     * \brief Traits class for access of (shifted) principal invariants.
     */
    template <>
    struct InvariantTraits<Invariant::PRINCIPAL>
    {
      /// First principal invariant.
      template <class Matrix>
      using FirstInvariant          = FirstPrincipalInvariant<Matrix>;

      /// Second principal invariant.
      template <class Matrix>
      using SecondInvariant         = SecondPrincipalInvariant<Matrix>;

      /// Third principal invariant.
      template <class Matrix>
      using ThirdInvariant          = ThirdPrincipalInvariant<Matrix>;


      /// Shifted first principal invariant.
      template < class Matrix , int offset = LinearAlgebra::dimension<Matrix>() >
      using ShiftedFirstInvariant   = ShiftedFirstPrincipalInvariant<Matrix,offset>;

      /// Shifted second principal invariant.
      template < class Matrix , int offset = LinearAlgebra::dimension<Matrix>() >
      using ShiftedSecondInvariant  = ShiftedSecondPrincipalInvariant<Matrix,offset>;

      /// Shifted third principal invariant.
      template <class Matrix>
      using ShiftedThirdInvariant   = ShiftedThirdPrincipalInvariant<Matrix>;
    };

    /**
     * \ingroup InvariantGroup
     * \brief  Traits class for access of (shifted) modified principal invariants.
     */
    template <>
    struct InvariantTraits<Invariant::MODIFIED>
    {
      /// First modified principal invariant.
      template <class Matrix>
      using FirstInvariant          = FirstModifiedPrincipalInvariant<Matrix>;

      /// Second modified principal invariant.
      template <class Matrix>
      using SecondInvariant         = SecondModifiedPrincipalInvariant<Matrix>;

      /// Third modified principal invariant.
      template <class Matrix>
      using ThirdInvariant          = ThirdModifiedPrincipalInvariant<Matrix>;


      /// Shifted first modified principal invariant.
      template < class Matrix , int offset = LinearAlgebra::dimension<Matrix>() >
      using ShiftedFirstInvariant   = ShiftedFirstModifiedPrincipalInvariant<Matrix,offset>;

      /// Shifted second modified principal invariant.
      template < class Matrix , int offset = LinearAlgebra::dimension<Matrix>() >
      using ShiftedSecondInvariant  = ShiftedSecondModifiedPrincipalInvariant<Matrix,offset>;

      /// Shifted third modified principal invariant.
      template <class Matrix>
      using ShiftedThirdInvariant   = ShiftedThirdModifiedPrincipalInvariant<Matrix>;
    };

    /**
     * \ingroup InvariantGroup
     * \brief Traits class for access of (shifted) mixed invariants.
     */
    template <>
    struct InvariantTraits<Invariant::MIXED>
    {
      /// First mixed invariant.
      template <class Matrix>
      using FirstInvariant          = FirstMixedInvariant<Matrix>;

      /// Second mixed invariant.
      template <class Matrix>
      using SecondInvariant         = SecondMixedInvariant<Matrix>;

      /// Third mixed invariant.
      template <class Matrix>
      using ThirdInvariant          = ThirdMixedInvariant<Matrix>;

      /// Shifted first mixed invariant.
      template <class Matrix>
      using ShiftedFirstInvariant   = ShiftedFirstMixedInvariant<Matrix>;

      /// Shifted second mixed invariant.
      template <class Matrix>
      using ShiftedSecondInvariant  = ShiftedSecondMixedInvariant<Matrix>;

      /// Shifted third second invariant.
      template <class Matrix>
      using ShiftedThirdInvariant   = ShiftedThirdMixedInvariant<Matrix>;
    };

    /**
     * \ingroup InvariantGroup
     * \brief Traits class for access of (shifted) modified mixed invariants.
     */
    template <>
    struct InvariantTraits<Invariant::MODIFIED_MIXED>
    {
      /// First modified mixed invariant.
      template <class Matrix>
      using FirstInvariant          = FirstModifiedMixedInvariant<Matrix>;

      /// Second modified mixed invariant.
      template <class Matrix>
      using SecondInvariant         = SecondModifiedMixedInvariant<Matrix>;

      /// Third modified mixed invariant.
      template <class Matrix>
      using ThirdInvariant          = ThirdModifiedMixedInvariant<Matrix>;


      /// Shifted first modified mixed invariant.
      template <class Matrix>
      using ShiftedFirstInvariant   = ShiftedFirstModifiedMixedInvariant<Matrix>;

      /// Shifted second modified mixed invariant.
      template <class Matrix>
      using ShiftedSecondInvariant  = ShiftedSecondModifiedMixedInvariant<Matrix>;

      /// Shifted third modified mixed invariant.
      template <class Matrix>
      using ShiftedThirdInvariant   = ShiftedThirdModifiedMixedInvariant<Matrix>;
    };
  }
}

#endif // FUNG_LINEAR_ALGEBRA_INVARIANTS_HH
