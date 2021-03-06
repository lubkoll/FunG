#pragma once

#include <type_traits>
#include <utility>

#include <fung/concept_check.hh>
#include <fung/util/chainer.hh>
#include <fung/util/compute_product.hh>
#include <fung/util/compute_sum.hh>
#include <fung/util/derivative_wrappers.hh>
#include <fung/util/evaluate_if_present.hh>
#include <fung/util/indexed_type.hh>
#include <fung/util/mathop_traits.hh>
#include <fung/util/type_traits.hh>

namespace FunG
{
    namespace MathematicalOperations
    {
        /**
         * @ingroup MathematicalOperationsGroup
         * @brief %Squared function \f$f^2\f$.
         */
        template < class F, class = Concepts::FunctionConceptCheck< F > >
        struct Squared : Chainer< Squared< F, Concepts::FunctionConceptCheck< F > > >
        {
        private:
            template < class IndexedArgX, class IndexedArgY >
            using D2Sum =
                ComputeSum< ComputeProduct< D0< F >, D2< F, IndexedArgX, IndexedArgY > >,
                            ComputeProduct< D1< F, IndexedArgY >, D1< F, IndexedArgX > > >;

            template < class IndexedArgX, class IndexedArgY, class IndexedArgZ >
            using D3Sum = ComputeSum<
                ComputeProduct< D0< F >, D3< F, IndexedArgX, IndexedArgY, IndexedArgZ > >,
                ComputeProduct< D1< F, IndexedArgZ >, D2< F, IndexedArgX, IndexedArgY > >,
                ComputeProduct< D1< F, IndexedArgY >, D2< F, IndexedArgX, IndexedArgZ > >,
                ComputeProduct< D2< F, IndexedArgY, IndexedArgZ >, D1< F, IndexedArgX > > >;

        public:
            /**
             * @brief Constructor
             * @param f_ initializer for F
             */
            template < class InitF,
                       std::enable_if_t< !std::is_same< std::decay_t< InitF >, Squared >::value >* =
                           nullptr >
            constexpr Squared( InitF&& f_ )
                : f( std::forward< InitF >( f_ ) ), value( multiply_via_traits( f(), f() ) )
            {
            }

            /// Update point of evaluation.
            template < class Arg >
            void update( Arg const& x )
            {
                update_if_present( f, x );
                value = multiply_via_traits( f(), f() );
            }

            /// Update variable corresponding to index.
            template < int index, class Arg >
            void update( const Arg& x )
            {
                update_if_present< index >( f, x );
                value = multiply_via_traits( f(), f() );
            }

            template < class... IndexedArgs >
            void bulk_update( IndexedArgs&&... args )
            {
                bulk_update_if_present( f, std::forward< IndexedArgs >( args )... );
                value = multiply_via_traits( f(), f() );
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
            template < int id, class Arg, class IndexedArg = IndexedType< Arg, id >,
                       class = std::enable_if_t<
                           ComputeProduct< D0< F >, D1< F, IndexedArg > >::present > >
            auto d1( Arg const& dx ) const
                -> decay_t< decltype( multiply_via_traits( std::declval< F >()(),
                                                           std::declval< F >()() ) ) >
            {
                return multiply_via_traits(
                    2, multiply_via_traits( f(), D1_< F, IndexedArg >::apply( f, dx ) ) );
            }

            /**
             * @brief Second directional derivative.
             * @param dx direction for which the derivative is computed
             * @param dy direction for which the derivative is computed
             */
            template < int idx, int idy, class ArgX, class ArgY,
                       class IndexedArgX = IndexedType< ArgX, idx >,
                       class IndexedArgY = IndexedType< ArgY, idy >,
                       class = std::enable_if_t< D2Sum< IndexedArgX, IndexedArgY >::present > >
            auto d2( ArgX const& dx, ArgY const& dy ) const
                -> decay_t< decltype( multiply_via_traits( std::declval< F >()(),
                                                           std::declval< F >()() ) ) >
            {
                return multiply_via_traits(
                    2, sum( product( D0< F >( f ), D2< F, IndexedArgX, IndexedArgY >( f, dx, dy ) ),
                            product( D1< F, IndexedArgY >( f, dy ),
                                     D1< F, IndexedArgX >( f, dx ) ) )() );
            }

            /**
             * @brief Third directional derivative.
             * @param dx direction for which the derivative is computed
             * @param dy direction for which the derivative is computed
             * @param dz direction for which the derivative is computed
             */
            template < int idx, int idy, int idz, class ArgX, class ArgY, class ArgZ,
                       class IndexedArgX = IndexedType< ArgX, idx >,
                       class IndexedArgY = IndexedType< ArgY, idy >,
                       class IndexedArgZ = IndexedType< ArgZ, idz >,
                       class = std::enable_if_t<
                           D3Sum< IndexedArgX, IndexedArgY, IndexedArgZ >::present > >
            auto d3( ArgX const& dx, ArgY const& dy, ArgZ const& dz ) const
                -> decay_t< decltype( multiply_via_traits( std::declval< F >()(),
                                                           std::declval< F >()() ) ) >
            {
                return multiply_via_traits(
                    2, sum( product( D0< F >( f ), D3< F, IndexedArgX, IndexedArgY, IndexedArgZ >(
                                                       f, dx, dy, dz ) ),
                            product( D1< F, IndexedArgZ >( f, dz ),
                                     D2< F, IndexedArgX, IndexedArgY >( f, dx, dy ) ),
                            product( D1< F, IndexedArgY >( f, dy ),
                                     D2< F, IndexedArgX, IndexedArgZ >( f, dx, dz ) ),
                            product( D2< F, IndexedArgY, IndexedArgZ >( f, dy, dz ),
                                     D1< F, IndexedArgX >( f, dx ) ) )() );
            }

        private:
            F f;
            decay_t< decltype(
                multiply_via_traits( std::declval< F >()(), std::declval< F >()() ) ) >
                value;
        };
    } // namespace MathematicalOperations
} // namespace FunG
