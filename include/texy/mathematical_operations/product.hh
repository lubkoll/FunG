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

namespace texy
{
    namespace MathematicalOperations
    {
        /**
         * @ingroup TexifyMathematicalOperationsGroup
         * @brief %Product \f$fg\f$ of functions of type F and G (F and G must satisfy the
         * requirements of Concepts::FunctionConcept).
         */
        template < class F, class G, class = FunG::Concepts::FunctionConceptCheck< F >,
                   class = FunG::Concepts::FunctionConceptCheck< G > >
        struct Product : Chainer< Product< F, G, FunG::Concepts::FunctionConceptCheck< F >,
                                           FunG::Concepts::FunctionConceptCheck< G > > >
        {
        private:
            template < class IndexedArg >
            using D1Type = FunG::ComputeSum<
                FunG::ComputeProduct< FunG::D1< F, IndexedArg >, FunG::D0< G > >,
                FunG::ComputeProduct< FunG::D0< F >, FunG::D1< G, IndexedArg > > >;

            template < class IndexedArgX, class IndexedArgY >
            using D2Type = FunG::ComputeSum<
                FunG::ComputeProduct< FunG::D2< F, IndexedArgX, IndexedArgY >, FunG::D0< G > >,
                FunG::ComputeProduct< FunG::D1< F, IndexedArgX >, FunG::D1< G, IndexedArgY > >,
                FunG::ComputeProduct< FunG::D1< F, IndexedArgY >, FunG::D1< G, IndexedArgX > >,
                FunG::ComputeProduct< FunG::D0< F >, FunG::D2< G, IndexedArgX, IndexedArgY > > >;

            template < class IndexedArgX, class IndexedArgY, class IndexedArgZ >
            using D3Type = FunG::ComputeSum<
                FunG::ComputeProduct< FunG::D3< F, IndexedArgX, IndexedArgY, IndexedArgZ >,
                                      FunG::D0< G > >,
                FunG::ComputeProduct< FunG::D2< F, IndexedArgX, IndexedArgY >,
                                      FunG::D1< G, IndexedArgZ > >,
                FunG::ComputeProduct< FunG::D2< F, IndexedArgX, IndexedArgZ >,
                                      FunG::D1< G, IndexedArgY > >,
                FunG::ComputeProduct< FunG::D1< F, IndexedArgX >,
                                      FunG::D2< G, IndexedArgY, IndexedArgZ > >,
                FunG::ComputeProduct< FunG::D2< F, IndexedArgY, IndexedArgZ >,
                                      FunG::D1< G, IndexedArgX > >,
                FunG::ComputeProduct< FunG::D1< F, IndexedArgY >,
                                      FunG::D2< G, IndexedArgX, IndexedArgZ > >,
                FunG::ComputeProduct< FunG::D1< F, IndexedArgZ >,
                                      FunG::D2< G, IndexedArgX, IndexedArgY > >,
                FunG::ComputeProduct< FunG::D0< F >,
                                      FunG::D3< G, IndexedArgX, IndexedArgY, IndexedArgZ > > >;

        public:
            /**
             * @brief Constructor passing arguments to function constructors.
             * @param f_ input for constructor of left side of product
             * @param g_ input for constructor of right side of product
             */
            template < class InitF, class InitG >
            constexpr Product( InitF&& f_, InitG&& g_ )
                : f( std::forward< InitF >( f_ ) ), g( std::forward< InitG >( g_ ) ),
                  value( FunG::multiply_via_traits( f(), g() ) )
            {
            }

            /// Update point of evaluation.
            template < class Arg >
            void update( Arg const& x )
            {
                FunG::update_if_present( f, x );
                FunG::update_if_present( g, x );
                value = FunG::multiply_via_traits( f(), g() );
            }

            /// Update variable corresponding to index.
            template < int index, class Arg >
            void update( const Arg& x )
            {
                FunG::update_if_present< index >( f, x );
                FunG::update_if_present< index >( g, x );
                value = FunG::multiply_via_traits( f(), g() );
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
                       class = std::enable_if_t< D1Type< IndexedArg >::present > >
            auto d1( Arg const& dx ) const
            {
                return sum(
                    FunG::product( FunG::D1< F, IndexedArg >( f, dx ), FunG::D0< G >( g ) ),
                    FunG::product( FunG::D0< F >( f ), FunG::D1< G, IndexedArg >( g, dx ) ) )();
            }

            /**
             * @brief Second directional derivative.
             * @param dx direction for which the derivative is computed
             * @param dy direction for which the derivative is computed
             */
            template < int idx, int idy, class ArgX, class ArgY,
                       class IndexedArgX = FunG::IndexedType< ArgX, idx >,
                       class IndexedArgY = FunG::IndexedType< ArgY, idy >,
                       class = std::enable_if_t< D2Type< IndexedArgX, IndexedArgY >::present > >
            auto d2( ArgX const& dx, ArgY const& dy ) const
            {
                return FunG::sum(
                    FunG::product( FunG::D2< F, IndexedArgX, IndexedArgY >( f, dx, dy ),
                                   FunG::D0< G >( g ) ),
                    FunG::product( FunG::D1< F, IndexedArgX >( f, dx ),
                                   FunG::D1< G, IndexedArgY >( g, dy ) ),
                    FunG::product( FunG::D1< F, IndexedArgY >( f, dy ),
                                   FunG::D1< G, IndexedArgX >( g, dx ) ),
                    FunG::product( FunG::D0< F >( f ),
                                   FunG::D2< G, IndexedArgX, IndexedArgY >( g, dx, dy ) ) )();
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
                           D3Type< IndexedArgX, IndexedArgY, IndexedArgZ >::present > >
            auto d3( ArgX const& dx, ArgY const& dy, ArgZ const& dz ) const
            {
                return FunG::sum(
                    FunG::product(
                        FunG::D3< F, IndexedArgX, IndexedArgY, IndexedArgZ >( f, dx, dy, dz ),
                        FunG::D0< G >( g ) ),
                    FunG::product( FunG::D2< F, IndexedArgX, IndexedArgY >( f, dx, dy ),
                                   FunG::D1< G, IndexedArgZ >( g, dz ) ),
                    FunG::product( FunG::D2< F, IndexedArgX, IndexedArgZ >( f, dx, dz ),
                                   FunG::D1< G, IndexedArgY >( g, dy ) ),
                    FunG::product( FunG::D1< F, IndexedArgX >( f, dx ),
                                   FunG::D2< G, IndexedArgY, IndexedArgZ >( g, dy, dz ) ),
                    FunG::product( FunG::D2< F, IndexedArgY, IndexedArgZ >( f, dy, dz ),
                                   FunG::D1< G, IndexedArgX >( g, dx ) ),
                    FunG::product( FunG::D1< F, IndexedArgY >( f, dy ),
                                   FunG::D2< G, IndexedArgX, IndexedArgZ >( g, dx, dz ) ),
                    FunG::product( FunG::D1< F, IndexedArgZ >( f, dz ),
                                   FunG::D2< G, IndexedArgX, IndexedArgY >( g, dx, dy ) ),
                    FunG::product(
                        FunG::D0< F >( f ),
                        FunG::D3< G, IndexedArgX, IndexedArgY, IndexedArgZ >( g, dx, dy, dz ) ) )();
            }

        private:
            F f;
            G g;
            FunG::decay_t< decltype(
                FunG::multiply_via_traits( std::declval< F >()(), std::declval< G >()() ) ) >
                value;
        };
    }
}
