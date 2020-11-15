#pragma once

#include <fung/util/traverse.hh>

#include <limits>
#include <type_traits>
#include <tuple>

namespace FunG
{
    /// @cond
    template < class, int >
    struct Variable;
    namespace Concepts
    {
        template < class >
        struct FunctionConceptCheck;
    }

    template < class T, class Other >
    struct ContainsType : std::false_type
    {
    };

    template < class T >
    struct ContainsType< T, T > : std::true_type
    {
    };

    template < class T, class Gradient >
    struct ContainsType< T, std::tuple< T, Gradient > > : std::true_type
    {
    };

    template < class T, class Value >
    struct ContainsType< T, std::tuple< Value, T > > : std::true_type
    {
    };

    namespace VariableDetail
    {
        template < class Arg, class Extended >
        struct Assign;

        template < class Arg, class Gradient >
        struct Assign< Arg, std::tuple< Arg, Gradient > >
        {
            static auto apply( Arg& t, const std::tuple< Arg, Gradient >& x )
            {
                t = std::get< 0 >( x );
            }
        };

        template < class Arg, class Value >
        struct Assign< Arg, std::tuple< Value, Arg > >
        {
            static auto apply( Arg& t, const std::tuple< Value, Arg >& x )
            {
                t = std::get< 1 >( x );
            }
        };

        template < class Arg >
        struct Assign< Arg, Arg >
        {
            static void apply( Arg& t, const Arg& x )
            {
                t = x;
            }
        };

        template < class T, class Other >
        struct ExtractReturnValue;

        template < class T >
        struct ExtractReturnValue< T, T >
        {
            static const T& apply( const T& x )
            {
                return x;
            }
        };

        template < class T, class Gradient >
        struct ExtractReturnValue< T, std::tuple< T, Gradient > >
        {
            static const T& apply( const std::tuple< T, Gradient >& x )
            {
                return std::get< 0 >( x );
            }
        };

        template < class T, class Value >
        struct ExtractReturnValue< T, std::tuple< Value, T > >
        {
            static const T& apply( const std::tuple< Value, T >& x )
            {
                return std::get< 1 >( x );
            }
        };

        template < bool >
        struct Update
        {
            template < class T, class Arg >
            static void apply( T& t, const Arg& x )
            {
                Assign< T, Arg >::apply( t, x );
            }
        };

        template <>
        struct Update< false >
        {
            template < class T, class Arg >
            static void apply( const T&, const Arg& )
            {
            }
        };
    } // namespace VariableDetail
    /// @endcond

    /// Independent variable. Can be uniquely identified by its id.
    template < class T, int id >
    struct Variable
    {
        Variable() = default;

        constexpr explicit Variable( const T& t_ ) : t( t_ )
        {
        }

        constexpr explicit Variable( T&& t_ ) : t( std::move( t_ ) )
        {
        }

        /// Update variable if index==id.
        template < int index, class Arg >
        void update( const Arg& t_ )
        {
            VariableDetail::Update< index == id >::apply( t, t_ );
        }

        /// Value of the variable.
        constexpr const T& operator()() const noexcept
        {
            return t;
        }

        /// First directional derivative. Only available if id==index.
        template < int index, class Arg, class = std::enable_if_t< id == index > >
        const T& d1( const Arg& dt ) const noexcept
        {
            return VariableDetail::ExtractReturnValue< T, Arg >::apply( dt );
        }

    private:
        T t;
    };

    /// Generate variable from input type.
    template < int id, class T >
    Variable< T, id > variable( const T& t )
    {
        return Variable< T, id >( t );
    }

    /// @cond
    namespace VariableDetail
    {
        /// Check if Type is variable.
        template < class >
        struct IsVariable : std::false_type
        {
        };

        template < class T, int n >
        struct IsVariable< Variable< T, n > > : std::true_type
        {
        };

        template < class, int >
        struct HasVariableWithId : std::false_type
        {
        };

        template < class Type, int id0, int id >
        struct HasVariableWithId< FunG::Variable< Type, id0 >, id >
            : std::integral_constant< bool, id == id0 >
        {
        };

        namespace Has
        {
            /// Check if Type contains has variable.
            template < class F >
            using Variable = Meta::AnyOf< F, IsVariable >;

            template < class F, int id >
            struct VariableId
            {
                template < class G >
                struct HasVariable
                {
                    static constexpr bool value = HasVariableWithId< G, id >::value;
                };
                static constexpr bool value = Meta::AnyOf< F, HasVariable >::value;
            };
        } // namespace Has

        constexpr bool greater( int a, int b )
        {
            return a > b;
        }

        template < class F, class G >
        struct Max
            : std::integral_constant< int, greater( F::value, G::value ) ? F::value : G::value >
        {
        };

        template < class F, class G >
        struct Min
            : std::integral_constant< int, greater( G::value, F::value ) ? F::value : G::value >
        {
        };

        namespace Detail
        {
            template < class Type >
            struct MaxVariableId
                : std::integral_constant< int, std::numeric_limits< int >::lowest() >
            {
            };

            template < class T, int id >
            struct MaxVariableId< FunG::Variable< T, id > > : std::integral_constant< int, id >
            {
            };

            template < class T, int id >
            struct MaxVariableId< const FunG::Variable< T, id > >
                : std::integral_constant< int, id >
            {
            };

            template < class Type >
            struct MinVariableId : std::integral_constant< int, std::numeric_limits< int >::max() >
            {
            };

            template < class T, int id >
            struct MinVariableId< FunG::Variable< T, id > > : std::integral_constant< int, id >
            {
            };

            template < class T, int id >
            struct MinVariableId< const FunG::Variable< T, id > >
                : std::integral_constant< int, id >
            {
            };
        } // namespace Detail

        template < class F >
        using MaxVariableId = Meta::Traverse< F, Detail::MaxVariableId, Max >;

        template < class F >
        using MinVariableId = Meta::Traverse< F, Detail::MinVariableId, Min >;

        template < class F, int id >
        struct VariableType
        {
            using type = void;
        };

        template < class T, int id >
        struct VariableType< Variable< T, id >, id >
        {
            using type = T;
        };

        template < class T, int id >
        struct VariableType< const Variable< T, id >, id >
        {
            using type = T;
        };

        template < class T, class >
        struct ChooseTypeImpl
        {
            using type = T;
        };

        template < class T >
        struct ChooseTypeImpl< T, void >
        {
            using type = T;
        };

        template < class T >
        struct ChooseTypeImpl< void, T >
        {
            using type = T;
        };

        template <>
        struct ChooseTypeImpl< void, void >
        {
            using type = void;
        };

        template < class F, class G >
        struct ChooseType
        {
            using type = typename ChooseTypeImpl< typename F::type, typename G::type >::type;
        };

        template < class F, int id >
        struct VariableT
        {
            template < class G >
            using Extractor = VariableType< G, id >;

            using type = typename Meta::Traverse< std::decay_t< F >, Extractor, ChooseType >::type;
        };
    } // namespace VariableDetail
    /// @endcond

    /// Get underlying type of variable with index id.
    template < class F, int id >
    using Variable_t = typename VariableDetail::VariableT< F, id >::type;

    namespace Checks
    {
        /** @addtogroup ConceptCheck
         *  @{ */

        /// Check if T is of type Variable<Type,n>.
        template < class T >
        constexpr bool isVariable()
        {
            return VariableDetail::IsVariable< T >::value;
        }

        namespace Has
        {
            /// Check if T contains a type Variable<Type,n>.
            template < class T >
            constexpr bool variable()
            {
                return VariableDetail::Has::Variable< std::decay_t< T > >::value;
            }

            /// Check if T contains a type Variable<Type,id>.
            template < class T, int id >
            constexpr bool variableId()
            {
                return VariableDetail::Has::VariableId< std::decay_t< T >, id >::value;
            }

            /// Check if T contains at least two variables.
            template < class T >
            constexpr bool moreThanOneVariable()
            {
                return VariableDetail::MinVariableId< std::decay_t< T > >::value <
                       VariableDetail::MaxVariableId< std::decay_t< T > >::value;
            }
        } // namespace Has

        /// Check if variable with index id has type Type.
        template < class F, class Type, int id >
        struct CheckArgument
        {
            static constexpr bool value = ContainsType< Variable_t< F, id >, Type >::value;
        };
        /** @} */
    } // namespace Checks
} // namespace FunG
