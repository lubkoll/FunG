// Copyright (C) 2018 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#pragma once

#include "principal_invariants.hh"
#include <texy/constant.hh>
#include <texy/generate.hh>
#include <texy/identity.hh>
#include <fung/util/static_checks.hh>

namespace texy
{

    namespace LinearAlgebra
    {
        /** @addtogroup TexifyInvariantGroup, TexifyLinearAlgebraGroup
         *  @{ */

        /**
         * \brief Generate first mixed invariant \f$ \iota_4=\iota_1(AM) \f$ of a matrix
         * \f$A\in\mathbb{R}^{n,n}\f$ with respect to the structural tensor
         * \f$M\in\mathbb{R}^{n,n}\f$.
         * \param A square matrix
         * \param M structural tensor describing principal (fiber) direction
         */
        auto i4( const std::string& A, const std::string& M )
        {
            return i1( identity( A ) * constant( M ) );
        }

        /**
         * \brief Generate first mixed invariant \f$ \iota_4\circ f \f$ with
         * \f$f\mapsto\mathbb{R}^{n,n}\f$ and structural tensor \f$M\in\mathbb{R}^{n,n}\f$.
         * \param f function returning a square matrix
         * \param M structural tensor describing principal (fiber) direction
         */
        template < class F, std::enable_if_t< FunG::Checks::isFunction< F >() >* = nullptr >
        auto i4( const F& f, const std::string& M )
        {
            return i1( f * constant( M ) );
        }

        /**
         * \brief Generate second mixed invariant \f$ \iota_5=\iota_1(A^2M) \f$ of a matrix
         * \f$A\in\mathbb{R}^{n,n}\f$ with respect to the structural tensor
         * \f$M\in\mathbb{R}^{n,n}\f$.
         * \param A square matrix
         * \param M structural tensor describing principal (fiber) direction
         */
        auto i5( const std::string& A, const std::string& M )
        {
            return i1( ( identity( A ) ^ 2 ) * constant( M ) );
        }

        /**
         * \brief Generate second mixed invariant \f$ \iota_5\circ f \f$ with
         * \f$f\mapsto\mathbb{R}^{n,n}\f$ and structural tensor \f$M\in\mathbb{R}^{n,n}\f$.
         * \param f function returning a square matrix
         * \param M structural tensor describing principal (fiber) direction
         */
        template < class F, std::enable_if_t< FunG::Checks::isFunction< F >() >* = nullptr >
        auto i5( const F& f, const std::string& M )
        {
            return i1( ( f ^ 2 ) * constant( M ) );
        }

        /**
         * \brief Generate third mixed invariant \f$ \iota_6=\iota_1(AM^2) \f$ of a matrix
         * \f$A\in\mathbb{R}^{n,n}\f$ with respect to the structural tensor
         * \f$M\in\mathbb{R}^{n,n}\f$.
         * \param A square matrix
         * \param M structural tensor describing principal (fiber) direction
         */
        auto i6( const std::string& A, const std::string& M )
        {
            return i1( ( constant( M ) ^ 2 ) * identity( A ) );
        }

        /**
         * \brief Generate third mixed invariant \f$ \iota_6\circ f \f$ with
         * \f$f\mapsto\mathbb{R}^{n,n}\f$ and structural tensor \f$M\in\mathbb{R}^{n,n}\f$.
         * \param f function returning a square matrix
         * \param M structural tensor describing principal (fiber) direction
         */
        template < class F, std::enable_if_t< FunG::Checks::isFunction< F >() >* = nullptr >
        auto i6( const F& f, const std::string& M )
        {
            return i1( ( constant( M ) ^ 2 ) * f );
        }

        /**
         * \brief Isochoric (volume-preserving), first modified mixed invariant \f$
         * \bar\iota_4(A)=\iota_4\iota_3^{-1/3} \f$, where \f$\iota_4\f$ is the first mixed
         * and \f$\iota_3\f$ is the third principal invariant.
         * \param x either a square matrix or a function returning a square matrix
         * \param M structural tensor describing principal (fiber) direction
         * \return \f$\bar\iota_4(x)\f$ if x is a matrix, else \f$\bar\iota_4 \circ x\f$
         */
        template < int n, class Arg >
        auto mi4( const Arg& x, const std::string& M )
        {
            return i4( x, M ) * Pow< -1, n >()( det( x ) );
        }

        /**
         * \brief Isochoric (volume-preserving), second modified principal invariant \f$
         * \bar\iota_5(A)=\iota_5\iota_3^{-2/3} \f$, where \f$\iota_5\f$ is the
         * second mixed and \f$\iota_3\f$ is the third principal invariant.
         *
         * \param x either a square matrix or a function returning a square matrix.
         * \param M structural tensor describing principal (fiber) direction
         * \return \f$\bar\iota_5(x)\f$ if x is a matrix, else \f$\bar\iota_5 \circ x\f$
         */
        template < int n, class Arg >
        auto mi5( const Arg& x, const std::string& M )
        {
            return i5( x, M ) * Pow< -2, n >()( det( x ) );
        }

        /**
         * \brief Isochoric (volume-preserving), second modified principal invariant \f$
         * \bar\iota_6(A)=\iota_6\iota_3^{-1/3} \f$, where \f$\iota_6\f$ is the
         * third mixed and \f$\iota_3\f$ is the third principal invariant.
         * \param x either a square matrix or a function returning a square matrix.
         * \param M structural tensor describing principal (fiber) direction
         * \return \f$\bar\iota_6(x)\f$ if x is a matrix, else \f$\bar\iota_6 \circ x\f$
         */
        template < int n, class Arg >
        auto mi6( const Arg& x, const std::string& M )
        {
            return i6( x, M ) * ( Pow< -1, n >()( det( x ) ) );
        }
        /** @} */
    }
}
