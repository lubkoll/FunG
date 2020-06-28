#pragma once

#include <fung/constant.hh>
#include <fung/util/compute_conditional.hh>
#include <fung/util/derivative_wrappers.hh>
#include <fung/util/evaluate_if_present.hh>
#include <fung/util/static_checks.hh>
#include <fung/util/type_traits.hh>

#include <type_traits>

namespace FunG
{
    /** @addtogroup CMathGroup
     *  @{ */

    /*!
      @brief min function \f$ \min(x,y) \f$ including first three derivatives.

      For scalar functions directional derivatives are less interesting. Incorporating this function
      as building block for more complex functions requires directional derivatives. These occur
      during applications of the chain rule.
     */
    template < class F, class G >
    struct Min
    {
        //! @copydoc Cos::Cos()
        explicit Min( const F& f, const G& g ) : f_( f ), g_( g )
        {
            update_value();
        }

        /// Update point of evaluation.
        template < class Arg >
        void update( Arg&& x )
        {
            update_if_present( f_, x );
            update_if_present( g_, x );
            update_value();
        }

        /// Update variable corresponding to index.
        template < int index, class Arg >
        void update( Arg&& x )
        {
            update_if_present< index >( f_, x );
            update_if_present< index >( g_, x );
            update_value();
        }

        template < class... IndexedArgs >
        void bulk_update( IndexedArgs&&... args )
        {
            bulk_update_if_present( f_, args... );
            bulk_update_if_present( g_, std::forward< IndexedArgs >( args )... );
            update_value();
        }

        //! @copydoc Cos::d0()
        double operator()() const noexcept
        {
            return value_;
        }

        /// First directional derivative.
        template < int id, class Arg, class IndexedArg = IndexedType< std::decay_t< Arg >, id >,
                   class = std::enable_if_t<
                       ComputeConditional< D1< F, IndexedArg >, D1< G, IndexedArg > >::present > >
        auto d1( Arg&& dx ) const
        {
            using D1F = D1< F, IndexedArg >;
            using D1G = D1< G, IndexedArg >;
            return ComputeConditional< D1F, D1G >( D1F( f_, dx ), D1G( g_, dx ),
                                                   f_smaller_than_g_ )();
        }

        /// Second directional derivative.
        template <
            int idx, int idy, class ArgX, class ArgY,
            class IndexedArgX = IndexedType< std::decay_t< ArgX >, idx >,
            class IndexedArgY = IndexedType< std::decay_t< ArgY >, idy >,
            class = std::enable_if_t< ComputeSum< D2< F, IndexedArgX, IndexedArgY >,
                                                  D2< G, IndexedArgX, IndexedArgY > >::present > >
        auto d2( ArgX&& dx, ArgY&& dy ) const
        {
            using D2F = D2< F, IndexedArgX, IndexedArgY >;
            using D2G = D2< G, IndexedArgX, IndexedArgY >;
            return ComputeConditional< D2F, D2G >( D2F( f_, dx, dy ), D2G( g_, dx, dy ),
                                                   f_smaller_than_g_ )();
        }

        /// Third directional derivative.
        template < int idx, int idy, int idz, class ArgX, class ArgY, class ArgZ,
                   class IndexedArgX = IndexedType< std::decay_t< ArgX >, idx >,
                   class IndexedArgY = IndexedType< std::decay_t< ArgY >, idy >,
                   class IndexedArgZ = IndexedType< std::decay_t< ArgZ >, idz >,
                   class = std::enable_if_t<
                       ComputeSum< D3< F, IndexedArgX, IndexedArgY, IndexedArgZ >,
                                   D3< G, IndexedArgX, IndexedArgY, IndexedArgZ > >::present > >
        auto d3( ArgX&& dx, ArgY&& dy, ArgZ&& dz ) const
        {
            using D3F = D3< F, IndexedArgX, IndexedArgY, IndexedArgZ >;
            using D3G = D3< G, IndexedArgX, IndexedArgY, IndexedArgZ >;
            return ComputeConditional< D3F, D3G >( D3F( f_, dx, dy, dz ), D3G( g_, dx, dy, dz ),
                                                   f_smaller_than_g_ )();
        }

    private:
        void update_value()
        {
            const auto x = f_();
            const auto y = g_();
            f_smaller_than_g_ = x < y;
            value_ = f_smaller_than_g_ ? x : y;
        }
        F f_;
        G g_;
        decay_t< decltype( std::declval< F >()() ) > value_;
        bool f_smaller_than_g_;
    };

    template < class F, class G,
               std::enable_if_t< Checks::isFunction< std::decay_t< F > >() &&
                                 Checks::isFunction< std::decay_t< G > >() >* = nullptr >
    decltype( auto ) min( F&& f, G&& g )
    {
        return Min< std::decay_t< F >, std::decay_t< G > >( std::forward< F >( f ),
                                                            std::forward< G >( g ) );
    }

    template < class F, class G,
               std::enable_if_t< Checks::isFunction< std::decay_t< F > >() &&
                                 !Checks::isFunction< std::decay_t< G > >() >* = nullptr >
    decltype( auto ) min( F&& f, G&& g )
    {
        return Min< std::decay_t< F >, Constant< std::decay_t< G > > >(
            std::forward< F >( f ), constant( std::forward< G >( g ) ) );
    }

    template < class F, class G,
               std::enable_if_t< !Checks::isFunction< std::decay_t< F > >() &&
                                 Checks::isFunction< std::decay_t< G > >() >* = nullptr >
    decltype( auto ) min( F&& f, G&& g )
    {
        return Min< Constant< std::decay_t< F > >, std::decay_t< G > >(
            constant( std::forward< F >( f ) ), std::forward< G >( g ) );
    }
    /** @} */
} // namespace FunG
