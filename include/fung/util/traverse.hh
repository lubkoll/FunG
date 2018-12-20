#pragma once

#include <fung/concept_check.hh>

#include <type_traits>

namespace FunG
{
    namespace Meta
    {
        template < class F, template < class > class Operation,
                   template < class, class > class Combine >
        struct Traverse : Operation< F >
        {
        };

        // for Finalize
        template < template < class, bool > class G, class F, bool hasVariable,
                   template < class > class Operation, template < class, class > class Combine >
        struct Traverse< G< F, hasVariable >, Operation, Combine >
            : Traverse< F, Operation, Combine >
        {
        };

        // for Scale
        template < template < class, class, class > class G, class Scalar, class F,
                   template < class > class Operation, template < class, class > class Combine >
        struct Traverse< G< Scalar, F, Concepts::FunctionConceptCheck< F > >, Operation, Combine >
            : Traverse< F, Operation, Combine >
        {
        };

        // for Squared
        template < template < class, class > class G, class F, template < class > class Operation,
                   template < class, class > class Combine >
        struct Traverse< G< F, Concepts::FunctionConceptCheck< F > >, Operation, Combine >
            : Traverse< F, Operation, Combine >
        {
        };

        // For Sum, Product, Chain
        template < template < class, class, class, class > class H, class F, class G,
                   template < class > class Operation, template < class, class > class Combine >
        struct Traverse<
            H< F, G, Concepts::FunctionConceptCheck< F >, Concepts::FunctionConceptCheck< G > >,
            Operation, Combine >
            : std::conditional< Combine< Operation< F >, Operation< G > >::value,
                                Combine< Operation< F >, Operation< G > >,
                                Combine< Traverse< F, Operation, Combine >,
                                         Traverse< G, Operation, Combine > > >::type
        {
        };

        template < class F, class G >
        using And = std::integral_constant< bool, F::value && G::value >;

        template < class F, class G >
        using Or = std::integral_constant< bool, F::value || G::value >;

        template < class F, template < class > class Operation >
        using AllOf = Traverse< F, Operation, And >;

        template < class F, template < class > class Operation >
        using AnyOf = Traverse< F, Operation, Or >;
    }
}
