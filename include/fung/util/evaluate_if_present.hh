#pragma once

#include "voider.hh"

#include <type_traits>
#include <utility>

namespace FunG
{
    /// @cond
    namespace Detail
    {
        template < class F, class Arg >
        using TryCallOfUpdate = decltype( std::declval< F >().update( std::declval< Arg >() ) );

        template < class F, class Arg, int id >
        using TryCallOfUpdateWithIndex =
            decltype( std::declval< F >().template update< id >( std::declval< Arg >() ) );

        template < class F, class Arg, class = void >
        struct HasUpdateWithoutIndex : std::false_type
        {
        };

        template < class F, class Arg >
        struct HasUpdateWithoutIndex< F, Arg, void_t< TryCallOfUpdate< F, Arg > > > : std::true_type
        {
        };

        template < class F, class Arg, int id, class = void >
        struct HasUpdateWithIndex : std::false_type
        {
        };

        template < class F, class Arg, int id >
        struct HasUpdateWithIndex< F, Arg, id, void_t< TryCallOfUpdateWithIndex< F, Arg, id > > >
            : std::true_type
        {
        };

        template < class F, class... IndexedArgs >
        struct BulkUpdate
        {
            static constexpr auto present = false;
        };

        template <
            class F, class IndexedArg,
            bool = HasUpdateWithIndex< F, typename IndexedArg::type, IndexedArg::index >::value >
        struct BulkUpdateImpl
        {
            static constexpr auto present = false;

            static void apply( const F&, const IndexedArg& )
            {
            }
        };

        template < class F, class IndexedArg >
        struct BulkUpdateImpl< F, IndexedArg, true >
        {
            static constexpr auto present = true;

            static void apply( const F& f, const IndexedArg& arg )
            {
                f.template update< IndexedArg::index >( arg.value );
            }
        };

        template < class F, class IndexedArg >
        struct BulkUpdate< F, IndexedArg > : BulkUpdateImpl< F, std::decay_t< IndexedArg > >
        {
        };

        template < class F, class IndexedArg, class... IndexedArgs >
        struct BulkUpdate< F, IndexedArg, IndexedArgs... >
        {
            static constexpr auto present =
                BulkUpdate< F, IndexedArg >::present && BulkUpdate< F, IndexedArgs... >::present;

            static void apply( const F& f, const IndexedArg& arg, const IndexedArgs&... args )
            {
                BulkUpdate< F, IndexedArg >::apply( f, arg );
                BulkUpdate< F, IndexedArgs... >::apply( f, args... );
            }
        };
    } // namespace Detail
    /// @endcond

    template < class F, class Arg,
               std::enable_if_t< !Detail::HasUpdateWithoutIndex< F, Arg >::value >* = nullptr >
    void update_if_present( F&&, Arg&& )
    {
    }

    template < class F, class Arg,
               std::enable_if_t< Detail::HasUpdateWithoutIndex< F, Arg >::value >* = nullptr >
    void update_if_present( F&& f, Arg&& x )
    {
        f.update( std::forward< Arg >( x ) );
    }

    template < int id, class F, class Arg,
               std::enable_if_t< !Detail::HasUpdateWithIndex< F, Arg, id >::value >* = nullptr >
    void update_if_present( F&&, Arg&& )
    {
    }

    template < int id, class F, class Arg,
               std::enable_if_t< Detail::HasUpdateWithIndex< F, Arg, id >::value >* = nullptr >
    void update_if_present( F&& f, Arg&& x )
    {
        f.template update< id >( std::forward< Arg >( x ) );
    }

    template < class F, class... IndexArgs,
               std::enable_if_t< Detail::BulkUpdate< F, IndexArgs... >::present >* = nullptr >
    void bulk_update_if_present( F&& f, IndexArgs&&... x )
    {
        Detail::BulkUpdate< F, IndexArgs... >::apply( std::forward< F >( f ),
                                                      std::forward< IndexArgs >( x )... );
    }
} // namespace FunG
