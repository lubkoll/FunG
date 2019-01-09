#pragma once

#include <type_traits>
#include <utility>

#include <texy/util/chainer.hh>
#include <fung/concept_check.hh>
#include <fung/mathematical_operations/dot.hh>
#include <fung/util/compute_dot.hh>
#include <fung/util/compute_sum.hh>
#include <fung/util/derivative_wrappers.hh>
#include <fung/util/evaluate_if_present.hh>
#include <fung/util/indexed_type.hh>

namespace texy
{
    namespace MathematicalOperations
    {
        using FunG::MathematicalOperations::Dot;
        //        /**
        //         * @ingroup MathematicalOperationsGroup
        //         * @brief %Dot \f$f \cdot g\f$ of functions of type F and G (F and G must satisfy
        //         the
        //         * requirements of Concepts::FunctionConcept).
        //         */
        //        template < class F, class G, class = Concepts::FunctionConceptCheck< F >,
        //                   class = Concepts::FunctionConceptCheck< G > >
        //        struct Dot : Chainer< Dot< F, G, Concepts::FunctionConceptCheck< F >,
        //                                   Concepts::FunctionConceptCheck< G > > >
        //        {
        //        private:
        //            template < class IndexedArg >
        //            using D1Type = ComputeSum< ComputeDot< D1< F, IndexedArg >, D0< G > >,
        //                                       ComputeDot< D0< F >, D1< G, IndexedArg > > >;

        //            template < class IndexedArgX, class IndexedArgY >
        //            using D2Type = ComputeSum< ComputeDot< D2< F, IndexedArgX, IndexedArgY >, D0<
        //            G > >,
        //                                       ComputeDot< D1< F, IndexedArgX >, D1< G,
        //                                       IndexedArgY > >,
        //                                       ComputeDot< D1< F, IndexedArgY >, D1< G,
        //                                       IndexedArgX > >,
        //                                       ComputeDot< D0< F >, D2< G, IndexedArgX,
        //                                       IndexedArgY > > >;

        //            template < class IndexedArgX, class IndexedArgY, class IndexedArgZ >
        //            using D3Type =
        //                ComputeSum< ComputeDot< D3< F, IndexedArgX, IndexedArgY, IndexedArgZ >,
        //                D0< G > >,
        //                            ComputeDot< D2< F, IndexedArgX, IndexedArgY >, D1< G,
        //                            IndexedArgZ > >,
        //                            ComputeDot< D2< F, IndexedArgX, IndexedArgZ >, D1< G,
        //                            IndexedArgY > >,
        //                            ComputeDot< D1< F, IndexedArgX >, D2< G, IndexedArgY,
        //                            IndexedArgZ > >,
        //                            ComputeDot< D2< F, IndexedArgY, IndexedArgZ >, D1< G,
        //                            IndexedArgX > >,
        //                            ComputeDot< D1< F, IndexedArgY >, D2< G, IndexedArgX,
        //                            IndexedArgZ > >,
        //                            ComputeDot< D1< F, IndexedArgZ >, D2< G, IndexedArgX,
        //                            IndexedArgY > >,
        //                            ComputeDot< D0< F >, D3< G, IndexedArgX, IndexedArgY,
        //                            IndexedArgZ > > >;

        //            using ReturnType =
        //                decay_t< decltype( std::declval< F >()().dot( std::declval< G >()() ) ) >;

        //        public:
        //            /**
        //             * @brief Constructor passing arguments to function constructors.
        //             * @param f_ input for constructor of left side of product
        //             * @param g_ input for constructor of right side of product
        //             */
        //            template < class InitF, class InitG >
        //            constexpr Dot( InitF&& f_, InitG&& g_ )
        //                : f( std::forward< InitF >( f_ ) ), g( std::forward< InitG >( g_ ) ),
        //                  value( f().dot( g() ) )
        //            {
        //            }

        //            /// Update point of evaluation.
        //            template < class Arg >
        //            void update( Arg const& x )
        //            {
        //                update_if_present( f, x );
        //                update_if_present( g, x );
        //                value = f().dot( g() );
        //            }

        //            /// Update variable corresponding to index.
        //            template < int index, class Arg >
        //            void update( const Arg& x )
        //            {
        //                update_if_present< index >( f, x );
        //                update_if_present< index >( g, x );
        //                value = f().dot( g() );
        //            }

        //            /// Function value.
        //            constexpr decltype( auto ) d0() const noexcept
        //            {
        //                return value;
        //            }

        //            /**
        //             * @brief First directional derivative.
        //             * @param dx direction for which the derivative is computed
        //             */
        //            template < int id, class Arg, class IndexedArg = IndexedType< Arg, id >,
        //                       class = std::enable_if_t< D1Type< IndexedArg >::present > >
        //            ReturnType d1( Arg const& dx ) const
        //            {
        //                return sum( dot_impl( D1< F, IndexedArg >( f, dx ), D0< G >( g ) ),
        //                            dot_impl( D0< F >( f ), D1< G, IndexedArg >( g, dx ) ) )();
        //            }

        //            /**
        //             * @brief Second directional derivative.
        //             * @param dx direction for which the derivative is computed
        //             * @param dy direction for which the derivative is computed
        //             */
        //            template < int idx, int idy, class ArgX, class ArgY,
        //                       class IndexedArgX = IndexedType< ArgX, idx >,
        //                       class IndexedArgY = IndexedType< ArgY, idy >,
        //                       class = std::enable_if_t< D2Type< IndexedArgX, IndexedArgY
        //                       >::present > >
        //            ReturnType d2( ArgX const& dx, ArgY const& dy ) const
        //            {
        //                return sum(
        //                    dot_impl( D2< F, IndexedArgX, IndexedArgY >( f, dx, dy ), D0< G >( g )
        //                    ),
        //                    dot_impl( D1< F, IndexedArgX >( f, dx ), D1< G, IndexedArgY >( g, dy )
        //                    ),
        //                    dot_impl( D1< F, IndexedArgY >( f, dy ), D1< G, IndexedArgX >( g, dx )
        //                    ),
        //                    dot_impl( D0< F >( f ), D2< G, IndexedArgX, IndexedArgY >( g, dx, dy )
        //                    ) )();
        //            }

        //            /**
        //             * @brief Third directional derivative.
        //             * @param dx direction for which the derivative is computed
        //             * @param dy direction for which the derivative is computed
        //             * @param dz direction for which the derivative is computed
        //             */
        //            template < int idx, int idy, int idz, class ArgX, class ArgY, class ArgZ,
        //                       class IndexedArgX = IndexedType< ArgX, idx >,
        //                       class IndexedArgY = IndexedType< ArgY, idy >,
        //                       class IndexedArgZ = IndexedType< ArgZ, idz >,
        //                       class = std::enable_if_t<
        //                           D3Type< IndexedArgX, IndexedArgY, IndexedArgZ >::present > >
        //            ReturnType d3( ArgX const& dx, ArgY const& dy, ArgZ const& dz ) const
        //            {
        //                return sum(
        //                    dot_impl( D3< F, IndexedArgX, IndexedArgY, IndexedArgZ >( f, dx, dy,
        //                    dz ),
        //                              D0< G >( g ) ),
        //                    dot_impl( D2< F, IndexedArgX, IndexedArgY >( f, dx, dy ),
        //                              D1< G, IndexedArgZ >( g, dz ) ),
        //                    dot_impl( D2< F, IndexedArgX, IndexedArgZ >( f, dx, dz ),
        //                              D1< G, IndexedArgY >( g, dy ) ),
        //                    dot_impl( D1< F, IndexedArgX >( f, dx ),
        //                              D2< G, IndexedArgY, IndexedArgZ >( g, dy, dz ) ),
        //                    dot_impl( D2< F, IndexedArgY, IndexedArgZ >( f, dy, dz ),
        //                              D1< G, IndexedArgX >( g, dx ) ),
        //                    dot_impl( D1< F, IndexedArgY >( f, dy ),
        //                              D2< G, IndexedArgX, IndexedArgZ >( g, dx, dz ) ),
        //                    dot_impl( D1< F, IndexedArgZ >( f, dz ),
        //                              D2< G, IndexedArgX, IndexedArgY >( g, dx, dy ) ),
        //                    dot_impl( D0< F >( f ),
        //                              D3< G, IndexedArgX, IndexedArgY, IndexedArgZ >( g, dx, dy,
        //                              dz ) ) )();
        //            }

        //        private:
        //            F f;
        //            G g;
        //            ReturnType value;
        //        };
    }
}
