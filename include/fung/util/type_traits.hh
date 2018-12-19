#pragma once

#include <type_traits>

#include "voider.hh"

namespace FunG
{
    /// @cond
    namespace Checks
    {
        namespace Try
        {
            namespace NestedType
            {
                // access underlying type of the expression templates of the Eigen library
                template < class EigenArg >
                using PlainObject = typename EigenArg::PlainObject;
            }
        }
    }
    /// @endcond

    /// %Identity, i.e. Decay<F>::type == F
    template < class F, class = void >
    struct Decay
    {
        using type = F;
    };

    /// Underlying type for expression templates of the Eigen library.
    template < class F >
    struct Decay< F, void_t< Checks::Try::NestedType::PlainObject< F > > >
    {
        using type = typename F::PlainObject;
    };

    /// Specialize this template class to register arithmetic types that are not built-in.
    template < class F >
    struct is_arithmetic : std::is_arithmetic< F >
    {
    };

    /// Access underlying type (if it is hidden by expression templates).
    template < class F >
    using decay_t = typename Decay< std::decay_t< F > >::type;

    /// Access underlying type (if it is hidden by expression templates).
    template < class F >
    using remove_reference_t = typename Decay< std::remove_reference_t< F > >::type;
}
