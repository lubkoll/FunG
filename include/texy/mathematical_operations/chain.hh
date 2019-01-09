#pragma once

#include <type_traits>
#include <utility>

#include <fung/concept_check.hh>
#include <fung/util/compute_chain.hh>
#include <fung/util/compute_sum.hh>
#include <fung/util/derivative_wrappers.hh>
#include <fung/util/evaluate_if_present.hh>
#include <fung/util/indexed_type.hh>

namespace texy
{
    /// @cond
    template < class >
    struct Chainer;
    /// @endcond

    namespace MathematicalOperations
    {
        /**
         * @ingroup TexifyMathematicalOperationsGroup
         *
         * @brief %Chain \f$ f\circ g \f$ of functions \f$f\f$ and \f$g\f$ of type F resp. G (F and
         * G must satisfy the requirements of Concepts::FunctionConcept).
         */
        template < class F, class G, class = FunG::Concepts::FunctionConceptCheck< F >,
                   class = FunG::Concepts::FunctionConceptCheck< G > >
        struct Chain : Chainer< Chain< F, G, FunG::Concepts::FunctionConceptCheck< F >,
                                       FunG::Concepts::FunctionConceptCheck< G > > >
        {
        private:
            using FArg = decltype( std::declval< G >()() );

            template < class IndexedArgX, class IndexedArgY, class IndexedFArgX,
                       class IndexedFArgY >
            using D2LazyType = FunG::ComputeSum<
                FunG::ComputeChainD2< F, FunG::D1< G, IndexedArgX >, FunG::D1< G, IndexedArgY >,
                                      IndexedFArgX, IndexedFArgY >,
                FunG::ComputeChainD1< F, FunG::D2< G, IndexedArgX, IndexedArgY >, IndexedFArgX > >;

            template < class IndexedArgX, class IndexedArgY, class IndexedArgZ, class IndexedFArgX,
                       class IndexedFArgY, class IndexedFArgZ >
            using D3LazyType = FunG::ComputeSum<
                FunG::ComputeChainD3< F, FunG::D1< G, IndexedArgX >, FunG::D1< G, IndexedArgY >,
                                      FunG::D1< G, IndexedArgZ >, IndexedFArgX, IndexedFArgY,
                                      IndexedFArgZ >,
                FunG::ComputeChainD2< F, FunG::D2< G, IndexedArgX, IndexedArgZ >,
                                      FunG::D1< G, IndexedArgY >, IndexedFArgX, IndexedFArgY >,
                FunG::ComputeChainD2< F, FunG::D1< G, IndexedArgX >,
                                      FunG::D2< G, IndexedArgY, IndexedArgZ >, IndexedFArgX,
                                      IndexedFArgY >,
                FunG::ComputeChainD2< F, FunG::D2< G, IndexedArgX, IndexedArgY >,
                                      FunG::D1< G, IndexedArgZ >, IndexedFArgX, IndexedFArgZ >,
                FunG::ComputeChainD1< F, FunG::D3< G, IndexedArgX, IndexedArgY, IndexedArgZ >,
                                      IndexedFArgX > >;

        public:
            /**
             * @brief Constructor taking copies of the functions to be chained.
             * @param f_ outer function
             * @param g_ inner function
             */
            constexpr Chain( const F& f_, const G& g_ ) : g( g_ ), f( f_ )
            {
                FunG::update_if_present( f, g() );
            }

            /**
             * @brief Constructor taking moving the functions to be chained.
             * @param f_ outer function
             * @param g_ inner function
             */
            constexpr Chain( F&& f_, G&& g_ ) : g( std::move( g_ ) ), f( std::move( f_ ) )
            {
                FunG::update_if_present( f, g() );
            }

            /// Update point of evaluation.
            template < class Arg >
            void update( const Arg& x )
            {
                FunG::update_if_present( g, x );
                FunG::update_if_present( f, g() );
            }

            /// Update variable corresponding to index.
            template < int index, class Arg >
            void update( const Arg& x )
            {
                FunG::update_if_present< index >( g, x );
                FunG::update_if_present( f, g() );
            }

            /// Function value.
            constexpr decltype( auto ) d0() const noexcept
            {
                return f();
            }

            /**
             * @brief First directional derivative.
             * @param dx direction for which the derivative is computed
             */
            template < int id, class Arg, class IndexedArg = FunG::IndexedType< Arg, id >,
                       class IndexedFArg = FunG::IndexedType< FArg, id >,
                       class = std::enable_if_t< FunG::ComputeChainD1< F, FunG::D1< G, IndexedArg >,
                                                                       IndexedFArg >::present > >
            auto d1( Arg const& dx ) const
            {
                return FunG::chain< IndexedFArg >( f, FunG::D1< G, IndexedArg >( g, dx ) )();
            }

            /**
             * @brief Second directional derivative.
             * @param dx direction for which the derivative is computed
             * @param dy direction for which the derivative is computed
             */
            template < int idx, int idy, class ArgX, class ArgY,
                       class IndexedArgX = FunG::IndexedType< ArgX, idx >,
                       class IndexedArgY = FunG::IndexedType< ArgY, idy >,
                       class IndexedFArgX = FunG::IndexedType< FArg, idx >,
                       class IndexedFArgY = FunG::IndexedType< FArg, idy >,
                       class = std::enable_if_t< D2LazyType< IndexedArgX, IndexedArgY, IndexedFArgX,
                                                             IndexedFArgY >::present > >
            auto d2( ArgX const& dx, ArgY const& dy ) const
            {
                return FunG::sum( FunG::chain< IndexedFArgX, IndexedFArgY >(
                                      f, FunG::D1< G, IndexedArgX >( g, dx ),
                                      FunG::D1< G, IndexedArgY >( g, dy ) ),
                                  FunG::chain< IndexedFArgX >(
                                      f, FunG::D2< G, IndexedArgX, IndexedArgY >( g, dx, dy ) ) )();
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
                       class IndexedFArgX = FunG::IndexedType< FArg, idx >,
                       class IndexedFArgY = FunG::IndexedType< FArg, idy >,
                       class IndexedFArgZ = FunG::IndexedType< FArg, idz >,
                       class = std::enable_if_t<
                           D3LazyType< IndexedArgX, IndexedArgY, IndexedArgZ, IndexedFArgX,
                                       IndexedFArgY, IndexedFArgZ >::present > >
            auto d3( ArgX const& dx, ArgY const& dy, ArgZ const& dz ) const
            {
                FunG::D1< G, IndexedArgX > dGdx( g, dx );
                FunG::D1< G, IndexedArgY > dGdy( g, dy );
                FunG::D1< G, IndexedArgZ > dGdz( g, dz );
                return FunG::sum(
                    FunG::chain< IndexedFArgX, IndexedFArgY, IndexedFArgZ >( f, dGdx, dGdy, dGdz ),
                    FunG::chain< IndexedFArgX, IndexedFArgY >(
                        f, FunG::D2< G, IndexedArgX, IndexedArgZ >( g, dx, dz ), dGdy ),
                    FunG::chain< IndexedFArgX, IndexedFArgY >(
                        f, dGdx, FunG::D2< G, IndexedArgY, IndexedArgZ >( g, dy, dz ) ),
                    FunG::chain< IndexedFArgX, IndexedFArgZ >(
                        f, FunG::D2< G, IndexedArgX, IndexedArgY >( g, dx, dy ), dGdz ),
                    FunG::chain< IndexedFArgX >(
                        f,
                        FunG::D3< G, IndexedArgX, IndexedArgY, IndexedArgZ >( g, dx, dy, dz ) ) )();
            }

        private:
            G g;
            F f;
        };
    }
}
