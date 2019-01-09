#pragma once

#include <type_traits>
#include <utility>

#include <texy/util/chainer.hh>
#include <fung/concept_check.hh>
#include <fung/util/compute_product.hh>
#include <fung/util/compute_sum.hh>
#include <fung/util/derivative_wrappers.hh>
#include <fung/util/evaluate_if_present.hh>
#include <fung/util/indexed_type.hh>
#include <fung/util/mathop_traits.hh>
#include <fung/util/type_traits.hh>

namespace texy
{
    namespace MathematicalOperations
    {
        /**
         * @ingroup TexifyMathematicalOperationsGroup
         * @brief %Squared function \f$f^2\f$.
         */
        template < class F, class = FunG::Concepts::FunctionConceptCheck< F > >
        struct Squared : Chainer< Squared< F, FunG::Concepts::FunctionConceptCheck< F > > >
        {
        private:
            template < class IndexedArgX, class IndexedArgY >
            using D2Sum = FunG::ComputeSum<
                FunG::ComputeProduct< FunG::D0< F >, FunG::D2< F, IndexedArgX, IndexedArgY > >,
                FunG::ComputeProduct< FunG::D1< F, IndexedArgY >, FunG::D1< F, IndexedArgX > > >;

            template < class IndexedArgX, class IndexedArgY, class IndexedArgZ >
            using D3Sum = FunG::ComputeSum<
                FunG::ComputeProduct< FunG::D0< F >,
                                      FunG::D3< F, IndexedArgX, IndexedArgY, IndexedArgZ > >,
                FunG::ComputeProduct< FunG::D1< F, IndexedArgZ >,
                                      FunG::D2< F, IndexedArgX, IndexedArgY > >,
                FunG::ComputeProduct< FunG::D1< F, IndexedArgY >,
                                      FunG::D2< F, IndexedArgX, IndexedArgZ > >,
                FunG::ComputeProduct< FunG::D2< F, IndexedArgY, IndexedArgZ >,
                                      FunG::D1< F, IndexedArgX > > >;

        public:
            /**
             * @brief Constructor
             * @param f_ initializer for F
             */
            template < class InitF,
                       std::enable_if_t< !std::is_same< std::decay_t< InitF >, Squared >::value >* =
                           nullptr >
            constexpr Squared( InitF&& f_ )
                : f( std::forward< InitF >( f_ ) ), value( FunG::multiply_via_traits( f(), f() ) )
            {
            }

            /// Update point of evaluation.
            template < class Arg >
            void update( Arg const& x )
            {
                FunG::update_if_present( f, x );
                value = FunG::multiply_via_traits( f(), f() );
            }

            /// Update variable corresponding to index.
            template < int index, class Arg >
            void update( const Arg& x )
            {
                FunG::update_if_present< index >( f, x );
                value = FunG::multiply_via_traits( f(), f() );
            }

            /// Function value.
            constexpr decltype( auto ) d0() const noexcept
            {
                return value;
            }

            /**
             * @brief First directional derivative.
             * @param dx direction for which the derivative is computed
             */
            template < int id, class Arg, class IndexedArg = FunG::IndexedType< Arg, id >,
                       class = std::enable_if_t< FunG::ComputeProduct<
                           FunG::D0< F >, FunG::D1< F, IndexedArg > >::present > >
            auto d1( Arg const& dx ) const
                -> FunG::decay_t< decltype( FunG::multiply_via_traits( std::declval< F >()(),
                                                                       std::declval< F >()() ) ) >
            {
                return FunG::multiply_via_traits(
                    2,
                    FunG::multiply_via_traits( f(), FunG::D1_< F, IndexedArg >::apply( f, dx ) ) );
            }

            /**
             * @brief Second directional derivative.
             * @param dx direction for which the derivative is computed
             * @param dy direction for which the derivative is computed
             */
            template < int idx, int idy, class ArgX, class ArgY,
                       class IndexedArgX = FunG::IndexedType< ArgX, idx >,
                       class IndexedArgY = FunG::IndexedType< ArgY, idy >,
                       class = std::enable_if_t< D2Sum< IndexedArgX, IndexedArgY >::present > >
            auto d2( ArgX const& dx, ArgY const& dy ) const
                -> FunG::decay_t< decltype( FunG::multiply_via_traits( std::declval< F >()(),
                                                                       std::declval< F >()() ) ) >
            {
                return multiply_via_traits(
                    2, sum( product( FunG::D0< F >( f ),
                                     FunG::D2< F, IndexedArgX, IndexedArgY >( f, dx, dy ) ),
                            product( FunG::D1< F, IndexedArgY >( f, dy ),
                                     FunG::D1< F, IndexedArgX >( f, dx ) ) )() );
            }

            /**
             * @brief Third directional derivative.
             * @param dx direction for which the derivative is computed
             * @param dy direction for which the derivative is computed
             * @param dz direction for which the derivative is computed
             */
            template < int idx, int idy, int idz, class ArgX, class ArgY, class ArgZ,
                       class IndexedArgX = FunG::IndexedType< ArgX, idx >,
                       class IndexedArgY = FunG::IndexedType< ArgY, idy >,
                       class IndexedArgZ = FunG::IndexedType< ArgZ, idz >,
                       class = std::enable_if_t<
                           D3Sum< IndexedArgX, IndexedArgY, IndexedArgZ >::present > >
            auto d3( ArgX const& dx, ArgY const& dy, ArgZ const& dz ) const
                -> FunG::decay_t< decltype( FunG::multiply_via_traits( std::declval< F >()(),
                                                                       std::declval< F >()() ) ) >
            {
                return multiply_via_traits(
                    2, sum( product( FunG::D0< F >( f ),
                                     FunG::D3< F, IndexedArgX, IndexedArgY, IndexedArgZ >(
                                         f, dx, dy, dz ) ),
                            product( FunG::D1< F, IndexedArgZ >( f, dz ),
                                     FunG::D2< F, IndexedArgX, IndexedArgY >( f, dx, dy ) ),
                            product( FunG::D1< F, IndexedArgY >( f, dy ),
                                     FunG::D2< F, IndexedArgX, IndexedArgZ >( f, dx, dz ) ),
                            product( FunG::D2< F, IndexedArgY, IndexedArgZ >( f, dy, dz ),
                                     FunG::D1< F, IndexedArgX >( f, dx ) ) )() );
            }

        private:
            F f;
            FunG::decay_t< decltype(
                FunG::multiply_via_traits( std::declval< F >()(), std::declval< F >()() ) ) >
                value;
        };
    }
}
