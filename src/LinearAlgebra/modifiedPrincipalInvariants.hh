/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                             */
/*   This file is part of the C++-library RFFGen.                              */
/*   Copyright 2015 Lars Lubkoll                                               */
/*                                                                             */
/*   RFFGen is free software: you can redistribute it and/or modify            */
/*   it under the terms of the GNU General Public License as published by      */
/*   the Free Software Foundation, either version 3 of the License, or         */
/*   (at your option) any later version.                                       */
/*                                                                             */
/*   RFFGen is distributed in the hope that it will be useful,                 */
/*   but WITHOUT ANY WARRANTY; without even the implied warranty of            */
/*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             */
/*   GNU General Public License for more details.                              */
/*                                                                             */
/*   You should have received a copy of the GNU General Public License         */
/*   along with RFFGen.  If not, see <http://www.gnu.org/licenses/>.           */
/*                                                                             */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef RFFGEN_MODIFIED_PRINCIPAL_INVARIANTS_HH
#define RFFGEN_MODIFIED_PRINCIPAL_INVARIANTS_HH

#include "principalInvariants.hh"
#include "shiftedInvariant.hh"
#include "../Util/modifyInvariants.hh"

namespace RFFGen
{
  /**
   * \cond DOCUMENT_FORWARD_DECLARATIONS
   */
  namespace Concepts { template <class> struct SymmetricMatrixConceptCheck; }
  /**
   * \endcond
   */

  namespace LinearAlgebra
  {
    /**
     * \ingroup LinearAlgebraGroup
     * \brief Isochoric (volume-preserving), first modified principal invariant \f$ \bar\iota_1(A)=\iota_1\iota_3^{-1/3} \f$, where \f$\iota_1\f$ is the first
     * and \f$\iota_3\f$ is the third principal invariant.
     */
    template <class Matrix, class = Concepts::SymmetricMatrixConceptCheck<Matrix> >
    struct FirstModifiedPrincipalInvariant
        : ModifyFirstPrincipalInvariant<Matrix,FirstPrincipalInvariant<Matrix> >
    {
      /// Default constructor.
      FirstModifiedPrincipalInvariant() = default;

      /**
       * @brief Constructor.
       * @param A matrix to compute first modified principal invariant from.
       */
      FirstModifiedPrincipalInvariant(const Matrix& A)
        : ModifyFirstPrincipalInvariant<Matrix,FirstPrincipalInvariant<Matrix> > ( A , FirstPrincipalInvariant<Matrix>(A) )
      {}
    };

    /**
     * \ingroup LinearAlgebraGroup
     * \brief Isochoric (volume-preserving), second modified principal invariant \f$ \bar\iota_2(A)=\iota_2\iota_3^{-1/3} \f$, where \f$\iota_2\f$ is the second
     * and \f$\iota_3\f$ is the third principal invariant.
     */
    template <class Matrix, class = Concepts::SymmetricMatrixConceptCheck<Matrix> >
    struct SecondModifiedPrincipalInvariant
        : ModifySecondPrincipalInvariant<Matrix,SecondPrincipalInvariant<Matrix> >
    {
      /// Default constructor.
      SecondModifiedPrincipalInvariant() = default;

      /**
       * @brief Constructor.
       * @param A matrix to compute second modified principal invariant from.
       */
      SecondModifiedPrincipalInvariant(const Matrix& A)
        : ModifySecondPrincipalInvariant<Matrix,SecondPrincipalInvariant<Matrix> > ( A , SecondPrincipalInvariant<Matrix>(A) )
      {}
    };

    /**
     * \ingroup LinearAlgebraGroup
     * \brief Third modified principal invariant is the same as the third principal invariant. This invariant describes volumetric changes.
     */
    template <class Matrix>
    using ThirdModifiedPrincipalInvariant = ThirdPrincipalInvariant<Matrix>;


    /**
     * \ingroup LinearAlgebraGroup
     * \brief Shifted first modified principal invariant \f$ \bar\iota_1(A) - n \f$ for \f$ A\in\mathbb{R}^{n,n} \f$.
     */
    template <class Matrix>
    using ShiftedFirstModifiedPrincipalInvariant  = ShiftedInvariant< FirstModifiedPrincipalInvariant<Matrix>  , dimension<Matrix>() >;

    /**
     * \ingroup LinearAlgebraGroup
     * \brief Shifted second modified principal invariant \f$ \bar\iota_2(A) - n \f$ for \f$ A\in\mathbb{R}^{n,n} \f$.
     */
    template <class Matrix>
    using ShiftedSecondModifiedPrincipalInvariant = ShiftedInvariant< SecondModifiedPrincipalInvariant<Matrix> , dimension<Matrix>() >;

    /**
     * \ingroup LinearAlgebraGroup
     * \brief Shifted third modified principal invariant \f$ \bar\iota_3(A) - 1 \f$ for \f$ A\in\mathbb{R}^{n,n} \f$.
     */
    template <class Matrix>
    using ShiftedThirdModifiedPrincipalInvariant  = ShiftedInvariant< ThirdModifiedPrincipalInvariant<Matrix>  , 1 >;
  }
}

#endif // RFFGEN_MODIFIED_PRINCIPAL_INVARIANTS_HH
