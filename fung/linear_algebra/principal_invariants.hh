// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#ifndef FUNG_LINEAR_ALGEBRA_PRINCIPAL_INVARIANTS_HH
#define FUNG_LINEAR_ALGEBRA_PRINCIPAL_INVARIANTS_HH

#include <type_traits>

#include "fung/cmath/pow.hh"
#include "dimension.hh"
#include "trace.hh"
#include "cofactor.hh"
#include "determinant.hh"
#include "fung/util/chainer.hh"
#include "fung/util/type_traits.hh"

namespace FunG
{
  /// @cond
  namespace Concepts { template <class> struct MatrixConceptCheck; }
  /// @endcond

  namespace LinearAlgebra
  {
    /// @cond
    namespace Detail
    {

      template <int row, class Matrix>
      auto symmetricCofactorDerivative(const Matrix& A, const Matrix& dA)
      {
        return computeCofactorDirectionalDerivative<row,row>(A,dA) + computeCofactorDirectionalDerivative<row,row>(dA,A);
      }


      template <int> struct Compute;

      template <>
      struct Compute<2>
      {
        template <class Matrix>
        static auto sumOfDiagonalCofactors(const Matrix& A)
        {
          return computeCofactor<0,0>(A) + computeCofactor<1,1>(A);
        }

        template <class Matrix>
        static auto sumOfSymmetricCofactorDerivatives(const Matrix& A, const Matrix& B)
        {
          return symmetricCofactorDerivative<0>(A,B) + symmetricCofactorDerivative<1>(A,B);
        }
      };

      template <>
      struct Compute<3>
      {
        template <class Matrix>
        static auto sumOfDiagonalCofactors(const Matrix& A)
        {
          return computeCofactor<0,0>(A) + computeCofactor<1,1>(A) + computeCofactor<2,2>(A);
        }

        template <class Matrix>
        static auto sumOfSymmetricCofactorDerivatives(const Matrix& A, const Matrix& B)
        {
          return symmetricCofactorDerivative<0>(A,B) + symmetricCofactorDerivative<1>(A,B) + symmetricCofactorDerivative<2>(A,B);
        }
      };

      /// Fallback implementation if matrix size can not be determined at compile time.
      template<>
      struct Compute<-1>
      {
        template <class Matrix>
        static auto sumOfDiagonalCofactors(const Matrix& A)
        {
          if(rows(A) == 2) return Compute<2>::sumOfDiagonalCofactors(A);
          /*if(rows(A) == 3)*/ return Compute<3>::sumOfDiagonalCofactors(A);
        }

        template < class Matrix >
        static auto sumOfSymmetricCofactorDerivatives(const Matrix& A, const Matrix& B)
        {
          if(rows(A) == 2) return Compute<2>::sumOfSymmetricCofactorDerivatives(A,B);
          /*if(rows(A) == 3)*/ return Compute<3>::sumOfSymmetricCofactorDerivatives(A,B);

        }
      };
    }
    /// @endcond

    /** @addtogroup InvariantGroup, LinearAlgebraGroup
     * @{ */
    /// Second principal invariant \f$ \iota_2(A)=\mathrm{tr}(\mathrm{cof}(A)) \f$ for \f$A\in\mathbb{R}^{n,n}\f$.
    template <class Matrix, class = Concepts::MatrixConceptCheck<Matrix> >
    class SecondPrincipalInvariant :
        public Chainer<SecondPrincipalInvariant< Matrix , Concepts::MatrixConceptCheck<Matrix> > >
    {
    public:
      SecondPrincipalInvariant() = default;

      /**
       * @brief Constructor.
       * @param A matrix to compute second principal invariant from
       */
      SecondPrincipalInvariant(const Matrix& A) { update(A); }

      /// Reset matrix to compute second principal invariant from.
      void update(const Matrix& A)
      {
        if( !initialized )
        {
          new(&A_) Matrix{A};
          initialized = true;
        }
        else A_ = A;
        value = Detail::Compute<dim<Matrix>()>::sumOfDiagonalCofactors(A);
      }

      /// Value of the second principal invariant
      auto d0() const
      {
        return value;
      }

      /**
       * @brief First directional derivative
       * @param dA1 direction for which the derivative is computed
       */
      auto d1(const Matrix& dA1) const
      {
        return Detail::Compute<dim<Matrix>()>::sumOfSymmetricCofactorDerivatives(A_,dA1);
      }

      /**
       * @brief Second directional derivative
       * @param dA1 direction for which the derivative is computed
       * @param dA2 direction for which the derivative is computed
       */
      auto d2(const Matrix& dA1, const Matrix& dA2) const
      {
        return Detail::Compute<dim<Matrix>()>::sumOfSymmetricCofactorDerivatives(dA1,dA2);
      }

    private:
      Matrix A_;
      std::decay_t< decltype(at(std::declval<Matrix>(),0,0)) > value = 0;
      bool initialized = false;
    };

    /**
     * @brief Generate first principal invariant.
     *
     * Depending on the argument either generates \f$\mathrm{tr}(A)\f$ or \f$\mathrm{tr}\circ f\f$.
     *
     * @return if x is a matrix then the this functions returns Trace<Arg>(x), if x is a function, then it returns
     * Trace< std::decay_t<decltype(x())> >( x() )( x );
     */
    template <class Arg>
    auto i1(const Arg& x)
    {
      return trace(x);
    }

    /**
     * @brief Convenient generation of second principal invariant \f$ \iota_2(A)=\mathrm{tr}(\mathrm{cof}(A)) \f$ for \f$A\in\mathbb{R}^{n,n}\f$.
     * @return SecondPrincipalInvariant<Matrix>(A)
     */
    template <class Matrix,
              std::enable_if_t< !Checks::isFunction<Matrix>() >* = nullptr >
    auto i2(const Matrix& A)
    {
      return SecondPrincipalInvariant<Matrix>(A);
    }

    /**
     * @brief Convenient generation of second principal invariant \f$ \iota_2\circ f \f$ for \f$f:\cdot\mapsto\mathbb{R}^{n,n}\f$.
     * @return SecondPrincipalInvariant<Matrix>(A)
     */
    template <class F,
              std::enable_if_t<Checks::isFunction<F>() >* = nullptr>
    auto i2(const F& f)
    {
      return SecondPrincipalInvariant< decay_t<decltype(f())> >( f() )( f );
    }

    /**
     * @brief Generate third principal invariant.
     *
     * Depending on the argument either generates \f$\det(A)\f$ or \f$\det\circ f\f$.
     *
     * @return if x is a matrix then the this functions returns Determinant<Arg>(x), if x is a function, then it returns
     * Determinant< std::decay_t<decltype(x())> >( x() )( x );
     */
    template <class Arg>
    auto i3(const Arg& x)
    {
      return det(x);
    }

    /**
     * @brief Isochoric (volume-preserving), first modified principal invariant \f$ \bar\iota_1(A)=\iota_1\iota_3^{-1/3} \f$, where \f$\iota_1\f$ is the first
     * and \f$\iota_3\f$ is the third principal invariant.
     * @param x either a square matrix or a function returning a square matrix
     */
    template <class Arg, int n = dim<Arg>()>
    auto mi1(const Arg& x)
    {
      return i1(x) * pow<-1,n>( det(x) );
    }

    /**
     * @brief Isochoric (volume-preserving), second modified principal invariant \f$ \bar\iota_2(A)=\iota_2\iota_3^{-2/3} \f$, where \f$\iota_2\f$ is the second
     * and \f$\iota_3\f$ is the third principal invariant.
     * @param x either a square matrix or a function returning a square matrix
     */
    template <class Arg, int n = dim<Arg>()>
    auto mi2(const Arg& x)
    {
      return i2(x) * pow<-2,n>( det(x) );
    }
    /** @} */
  }
}

#endif // FUNG_LINEAR_ALGEBRA_PRINCIPAL_INVARIANTS_HH
