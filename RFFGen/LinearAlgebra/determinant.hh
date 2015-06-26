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

#ifndef RFFGEN_LINEAR_ALGEBRA_DETERMINANT_HH
#define RFFGEN_LINEAR_ALGEBRA_DETERMINANT_HH

#include <type_traits>
#include <utility>

#include "dimension.hh"
#include "rowsAndCols.hh"
#include "../Util/at.hh"
#include "../Util/base.hh"
#include "../Util/chainer.hh"
#include "../Util/exceptions.hh"

namespace RFFGen
{
  /**
   * \cond DOCUMENT_FORWARD_DECLARATIONS
   */
  namespace Concepts {  template <class> struct SymmetricMatrixConceptCheck; }
  /**
   * \endcond
   */

  namespace LinearAlgebra
  {
    /**
     * \cond DOCUMENT_IMPLEMENTATION_DETAILS
     */
    namespace Detail
    {
      template < class Matrix >
      inline auto composeResult(Matrix const& A, Matrix const& B)
      {
        return at(A,0,0) * at(B,1,1) + at(A,1,1) * at(B,0,0) - ( at(A,0,1) * at(B,1,0) + at(A,1,0) * at(B,0,1) );
      }

      template < class Matrix >
      inline auto composeResult(Matrix const& dA, Matrix const& dB, Matrix const& dC)
      {
        return at(dB,1,1) * ( at(dA,0,0) * at(dC,2,2) - at(dA,2,0) * at(dC,0,2) ) +
            at(dB,1,2) * ( at(dA,0,1) * at(dC,2,0) - at(dA,2,1) * at(dC,0,0) ) +
            at(dB,1,0) * ( at(dA,0,2) * at(dC,2,1) - at(dA,2,2) * at(dC,0,1) );
      }

      template < class Matrix >
      inline auto composeSemiSymmetricResult(Matrix const& dA, Matrix const& dB, Matrix const& dC)
      {
        return at(dB,1,1) * ( at(dA,0,0) * at(dC,2,2) + at(dA,2,2) * at(dC,0,0) - at(dA,2,0) * at(dC,0,2) - at(dA,0,2) * at(dC,2,0) ) +
            at(dB,1,2) * ( at(dA,0,1) * at(dC,2,0) + at(dA,2,0) * at(dC,0,1) - at(dA,2,1) * at(dC,0,0) - at(dA,0,0) * at(dC,2,1) ) +
            at(dB,1,0) * ( at(dA,0,2) * at(dC,2,1) + at(dA,2,1) * at(dC,0,2) - at(dA,2,2) * at(dC,0,1) - at(dA,0,1) * at(dC,2,2) );
      }

      template <class Matrix, int dim, class = Concepts::SymmetricMatrixConceptCheck<Matrix> >
      class DeterminantImpl;

      template<class Matrix>
      class DeterminantImpl< Matrix , 2 , Concepts::SymmetricMatrixConceptCheck<Matrix> >
          : public Base , public Chainer< DeterminantImpl<Matrix,2,Concepts::SymmetricMatrixConceptCheck<Matrix> >
      {
      public:
        DeterminantImpl() = default;

        explicit DeterminantImpl(Matrix const& A_) { update(A_); }

        void update(Matrix const& A_)
        {
          A = A_;
          resultOfD0 = at(A,0,0) * at(A,1,1) - at(A,0,1) * at(A,1,0);//A[0][0] * A[1][1] - A[0][1] * A[1][0];
        }

        auto d0() const
        {
          return resultOfD0;
        }

        template <int>
        auto d1(Matrix const& dA1) const
        {
          return composeResult(A, dA1);
        }

        template <int,int>
        auto d2(Matrix const& dA1, Matrix const& dA2) const
        {
          return composeResult(dA2,dA1);
        }

      private:
        Matrix A;
        std::decay_t< at_t<Matrix> > resultOfD0 = 0.;
      };

      template <class Matrix>
      class DeterminantImpl<Matrix,3,Concepts::SymmetricMatrixConceptCheck<Matrix> >
          : public Base , public Chainer< DeterminantImpl<Matrix,3,Concepts::SymmetricMatrixConceptCheck<Matrix> >
      {
      public:
        DeterminantImpl() = default;

        DeterminantImpl(Matrix const& A_) { update(A_); }

        void update(Matrix const& A_)
        {
          A = A_;
          resultOfD0 = composeResult(A,A,A);
        }

        auto d0() const { return resultOfD0; }

        template <int>
        auto d1(Matrix const& dA1) const
        {
          return composeResult(dA1,A,A) + composeResult(A,dA1,A) + composeResult(A,A,dA1);
        }

        template <int,int>
        auto d2(Matrix const& dA1, Matrix const& dA2) const
        {
          return composeSemiSymmetricResult(A,dA2,dA1) + composeSemiSymmetricResult(dA1,A,dA2) + composeSemiSymmetricResult(A,dA1,dA2);
        }

        template <int,int,int>
        auto d3(Matrix const& dA1, Matrix const& dA2, Matrix const& dA3) const
        {
          return composeSemiSymmetricResult(dA1,dA2,dA3) + composeSemiSymmetricResult(dA1,dA3,dA2) + composeSemiSymmetricResult(dA2,dA1,dA3);
        }

      private:
        Matrix A;
        std::decay_t< at_t<Matrix> > resultOfD0 = 0.;
      };
    }

    /**
     * \ingroup LinearAlgebraGroup
     * \brief Determinant of constant size matrix with first three derivatives.
     */
    template <class Matrix>
    using ConstantSizeDeterminant = Detail::DeterminantImpl<Matrix,dimension<Matrix>()>;

    /**
     * \ingroup LinearAlgebraGroup
     * \brief Determinant of dynamic size matrix with first three derivatives.
     */
    template <class Matrix>
    class DynamicSizeDeterminant :
        public Base , public Chainer< DynamicSizeDeterminant<Matrix> >
    {
    public:
      DynamicSizeDeterminant() = default;

      /// Constructor.
      DynamicSizeDeterminant(Matrix const& A) : dim(rows(A))
      {
        update(A);
      }

      /// Reset point of evaluation.
      void update(Matrix const& A)
      {
#ifdef RFFGEN_ENABLE_EXCEPTIONS
        if( rows(A) != cols(A) ) throw NonSymmetricMatrixException("DynamicSizeDeterminant",rows(A),cols(A),__FILE__,__LINE__);
#endif
        dim = rows(A);
        if( dim == 2 ) det2D.update(A);
        if( dim == 3 ) det3D.update(A);
      }

      /// Function value.
      auto d0() const { return ( dim==2 ) ? det2D.d0() : det3D.d0(); }

      /// First (directional) derivative.
      template <int id>
      auto d1(Matrix const& dA1) const
      {
        return ( dim==2 ) ? det2D.template d1<id>(dA1) : det3D.template d1<id>(dA1) ;
      }

      /// Second (directional) derivative.
      template < int idx , int idy >
      auto d2(Matrix const& dA1, Matrix const& dA2) const
      {
        return ( dim==2 ) ? det2D.template d2<idx,idy>(dA1,dA2) : det3D.template d2<idx,idy>(dA1,dA2);
      }

        /// Third (directional) derivative.
      template < int idx , int idy , int idz >
      auto d3(Matrix const& dA1, Matrix const& dA2, Matrix const& dA3) const
      {
        return ( dim==2 ) ? 0 : det3D.template d3<idx,idy,idz>(dA1,dA2,dA3);
      }

    private:
      int dim = 0;
      Detail::DeterminantImpl<Matrix,2> det2D;
      Detail::DeterminantImpl<Matrix,3> det3D;
    };
    /**
     * \endcond
     */

    /**
     * \ingroup LinearAlgebraGroup
     * \brief Determinant with first three derivatives.
     */
    template < class Matrix >
    using Determinant = std::conditional_t< Checks::isConstantSizeMatrix<Matrix>() , ConstantSizeDeterminant<Matrix> , DynamicSizeDeterminant<Matrix> >;

    /**
     * \ingroup LinearAlgebraGroup
     * \brief Convenient generation of Determinant<Matrix>(A).
     */
    template<class Matrix>
    auto det(Matrix const& A)
    {
      return Determinant<Matrix>(A);
    }

    /**
     * \ingroup LinearAlgebraGroup
     * \brief Convenient computation of \f$\det(A)\f$.
     */
    template<class Matrix>
    auto determinant(Matrix const& A)
    {
      return Determinant<Matrix>(A)();
    }
  }
}

#endif // RFFGEN_LINEAR_ALGEBRA_DETERMINANT_HH
