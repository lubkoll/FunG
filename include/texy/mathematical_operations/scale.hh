#pragma once

#include <type_traits>
#include <utility>

#include <texy/util/chainer.hh>
#include <fung/concept_check.hh>
#include <fung/util/derivative_wrappers.hh>
#include <fung/util/evaluate_if_present.hh>
#include <fung/util/indexed_type.hh>
#include <fung/util/mathop_traits.hh>

namespace texy
{
    namespace MathematicalOperations
    {
        /**
         * @ingroup TexifyMathematicalOperationsGroup
         * @brief Scaling \f$ af \f$ of some function \f$ f \f$ with a double \f$ a \f$ (F must
         * satisfy the requirements of Concepts::FunctionConcept).
         */
        template < class Scalar, class F, class = FunG::Concepts::FunctionConceptCheck< F > >
        struct Scale : Chainer< Scale< Scalar, F, FunG::Concepts::FunctionConceptCheck< F > > >
        {
            /**
             * @brief Constructor passing arguments to function constructor.
             * @param a_ scaling
             * @param f_ input for constructor of outer function
             */
            template < class... InitF >
            constexpr Scale( Scalar a_, InitF&&... f_ )
                : a( a_ ), f( std::forward< InitF >( f_ )... ),
                  value( FunG::multiply_via_traits( a, f() ) )
            {
            }

            /// Update point of evaluation.
            template < class Arg >
            void update( const Arg& x )
            {
                FunG::update_if_present( f, x );
                value = FunG::multiply_via_traits( a, f() );
            }

            /// Update variable corresponding to index.
            template < int index, class Arg >
            void update( const Arg& x )
            {
                FunG::update_if_present< index >( f, x );
                value = FunG::multiply_via_traits( a, f() );
            }

            /// Function value.
            constexpr decltype( auto ) d0() const noexcept
            {
                return value;
            }

            /// First directional derivative.
            template < int idx, class Arg, class IndexedArg = FunG::IndexedType< Arg, idx >,
                       class = std::enable_if_t< FunG::D1< F, IndexedArg >::present > >
            auto d1( const Arg& dx ) const
            {
                return FunG::multiply_via_traits( a, FunG::D1_< F, IndexedArg >::apply( f, dx ) );
            }

            /// Second directional derivative.
            template <
                int idx, int idy, class ArgX, class ArgY,
                class IndexedArgX = FunG::IndexedType< ArgX, idx >,
                class IndexedArgY = FunG::IndexedType< ArgY, idy >,
                class = std::enable_if_t< FunG::D2< F, IndexedArgX, IndexedArgY >::present > >
            auto d2( const ArgX& dx, const ArgY& dy ) const
            {
                return FunG::multiply_via_traits(
                    a, FunG::D2_< F, IndexedArgX, IndexedArgY >::apply( f, dx, dy ) );
            }

            /// Third directional derivative.
            template < int idx, int idy, int idz, class ArgX, class ArgY, class ArgZ,
                       class IndexedArgX = FunG::IndexedType< ArgX, idx >,
                       class IndexedArgY = FunG::IndexedType< ArgY, idy >,
                       class IndexedArgZ = FunG::IndexedType< ArgZ, idz >,
                       class = std::enable_if_t<
                           FunG::D3< F, IndexedArgX, IndexedArgY, IndexedArgZ >::present > >
            auto d3( const ArgX& dx, const ArgY& dy, const ArgZ& dz ) const
            {
                return FunG::multiply_via_traits(
                    a,
                    FunG::D3_< F, IndexedArgX, IndexedArgY, IndexedArgZ >::apply( f, dx, dy, dz ) );
            }

        private:
            Scalar a = 1.;
            F f;
            std::decay_t< decltype( std::declval< F >()() ) > value;
        };
    }
}
