// Copyright (C) 2018 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#pragma once

#include <type_traits>

#include "determinant.hh"
#include "trace.hh"
#include <texy/cmath/pow.hh>
#include <texy/generate.hh>
#include <fung/util/chainer.hh>
#include <fung/util/type_traits.hh>

namespace texy
{
    namespace LinearAlgebra
    {
        /** @addtogroup TexifyInvariantGroup, TexifyLinearAlgebraGroup
         * @{ */
        /// Second principal invariant \f$ \iota_2(A)=\mathrm{tr}(\mathrm{cof}(A)) \f$ for
        /// \f$A\in\mathbb{R}^{n,n}\f$.
        class SecondPrincipalInvariant : public FunG::Chainer< SecondPrincipalInvariant >
        {
        public:
            SecondPrincipalInvariant() = default;

            /**
             * @brief Constructor.
             * @param A matrix to compute second principal invariant from
             */
            SecondPrincipalInvariant( const std::string& A )
            {
                update( A );
            }

            /// Reset matrix to compute second principal invariant from.
            void update( const std::string& A )
            {
                A_ = A;
            }

            /// Value of the second principal invariant
            auto d0() const
            {
                return std::string( "\\mathrm{tr}(\\mathrm{cof}(" ).append( A_ ).append( "))" );
            }

            /**
             * @brief First directional derivative
             * @param dA1 direction for which the derivative is computed
             */
            auto d1( const std::string& dA ) const
            {
                return std::string( "\\mathrm{tr}(\\mathrm{cof'}(" )
                    .append( A_ )
                    .append( ")(" )
                    .append( dA )
                    .append( "))" );
            }

            /**
             * @brief Second directional derivative
             * @param dA1 direction for which the derivative is computed
             * @param dA2 direction for which the derivative is computed
             */
            auto d2( const std::string& dA1, const std::string& dA2 ) const
            {
                return std::string( "\\mathrm{tr}(\\mathrm{cof''}(" )
                    .append( A_ )
                    .append( ")(" )
                    .append( dA1 )
                    .append( "," )
                    .append( dA2 )
                    .append( "))" );
            }

        private:
            std::string A_;
        };

        /**
         * @brief Generate first principal invariant.
         *
         * Depending on the argument either generates \f$\mathrm{tr}(A)\f$ or \f$\mathrm{tr}\circ
         * f\f$.
         *
         * @return if x is a matrix then the this functions returns Trace<Arg>(x), if x is a
         * function, then it returns
         * Trace< std::decay_t<decltype(x())> >( x() )( x );
         */
        template < class Arg >
        auto i1( const Arg& x )
        {
            return trace( x );
        }

        /**
         * @brief Convenient generation of second principal invariant \f$
         * \iota_2(A)=\mathrm{tr}(\mathrm{cof}(A)) \f$ for \f$A\in\mathbb{R}^{n,n}\f$.
         * @return SecondPrincipalInvariant<Matrix>(A)
         */
        auto i2( const std::string& A )
        {
            return SecondPrincipalInvariant( A );
        }

        /**
         * @brief Convenient generation of second principal invariant \f$ \iota_2\circ f \f$ for
         * \f$f:\cdot\mapsto\mathbb{R}^{n,n}\f$.
         * @return SecondPrincipalInvariant( f() )( f )
         */
        template < class F, std::enable_if_t< FunG::Checks::isFunction< F >() >* = nullptr >
        auto i2( const F& f )
        {
            return SecondPrincipalInvariant( f() )( f );
        }

        /**
         * @brief Generate third principal invariant.
         *
         * Depending on the argument either generates \f$\det(A)\f$ or \f$\det\circ f\f$.
         *
         * @return if x is a matrix then the this functions returns Determinant<Arg>(x), if x is a
         * function, then it returns
         * Determinant( x() )( x );
         */
        template < class Arg >
        auto i3( const Arg& x )
        {
            return det( x );
        }

        /**
         * @brief Isochoric (volume-preserving), first modified principal invariant \f$
         * \bar\iota_1(A)=\iota_1\iota_3^{-1/3} \f$, where \f$\iota_1\f$ is the first
         * and \f$\iota_3\f$ is the third principal invariant.
         * @param x either a square matrix or a function returning a square matrix
         */
        template < int n, class Arg >
        auto mi1( const Arg& x )
        {
            return i1( x ) * pow< -1, n >( det( x ) );
        }

        /**
         * @brief Isochoric (volume-preserving), second modified principal invariant \f$
         * \bar\iota_2(A)=\iota_2\iota_3^{-2/3} \f$, where \f$\iota_2\f$ is the second
         * and \f$\iota_3\f$ is the third principal invariant.
         * @param x either a square matrix or a function returning a square matrix
         */
        template < int n, class Arg >
        auto mi2( const Arg& x )
        {
            return i2( x ) * pow< -2, n >( det( x ) );
        }
        /** @} */
    }
}
