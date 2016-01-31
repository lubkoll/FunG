// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#ifndef FUNG_GENERATE_HH
#define FUNG_GENERATE_HH

#include <iostream>
#include <type_traits>

#include "fung/constant.hh"
#include "fung/operations.hh"
#include "fung/util/add_missing_operators.hh"
#include "fung/util/static_checks.hh"
#include "fung/util/type_traits.hh"
#include "fung/variable.hh"

namespace FunG
{
  /// @cond
  namespace GenerateDetail
  {
    template < class F, class G,
               bool = Checks::isFunction< std::decay_t<F> >(),
               bool = Checks::isFunction< std::decay_t<G> >() >
    struct SumGenerator;

    template < class F, class G >
    struct SumGenerator< F, G, true, true >
    {
      static auto apply( F&& f, G&& g )
      {
        return MathematicalOperations::Sum< std::decay_t<F>, std::decay_t<G> >( std::forward<F>(f), std::forward<G>(g) );
      }
    };

    template < class F, class G >
    struct SumGenerator< F, G, true, false >
    {
      static auto apply( F&& f, G&& g )
      {
        return MathematicalOperations::Sum< std::decay_t<F> , Constant< std::decay_t<G> > >( std::forward<F>(f) , constant( std::forward<G>(g) ) );
      }
    };

    template < class F, class G >
    struct SumGenerator< F, G, false, true >
    {
      static auto apply( F&& f, G&& g )
      {
        return MathematicalOperations::Sum< Constant< std::decay_t<F> > , std::decay_t<G> >( constant( std::forward<F>(f) ) , std::forward<G>(g) );
      }
    };

    template < class F, class G,
               bool = Checks::isFunction< std::decay_t<F> >(),
               bool = Checks::isFunction< std::decay_t<G> >(),
               bool = is_arithmetic< std::decay_t<F> >::value,
               bool = is_arithmetic< std::decay_t<G> >::value >
    struct ProductGenerator;

    template < class F, class G >
    struct ProductGenerator< F, G, true, true, false, false >
    {
      static auto apply( F&& f, G&& g )
      {
        return MathematicalOperations::Product< std::decay_t<F>, std::decay_t<G> >( std::forward<F>(f), std::forward<G>(g) );
      }
    };

    template < class F, class G >
    struct ProductGenerator< F, G, false, true, true, false >
    {
      static auto apply( F f, G&& g )
      {
        return MathematicalOperations::Scale< std::decay_t<G> >( f , std::forward<G>(g) );
      }
    };

    template < class F, class G >
    struct ProductGenerator< F, G, true, false, false, true >
    {
      static auto apply( F&& f, G g )
      {
        return ProductGenerator< G, F, false, true, true, false >( g , std::forward<F>(f) );
      }
    };

    template < class F, class G >
    struct ProductGenerator< F, G, false, true, false, false >
    {
      static auto apply( F&& f, G&& g)
      {
        using Const = Constant< std::decay_t<F> >;
        return MathematicalOperations::Product< Const, std::decay_t<G> >( Const( std::forward<F>(f) ), std::forward<G>(g) );
      }
    };

    template <class F, class G>
    struct ProductGenerator<F,G,true,false,false,false>
    {
      static auto apply( F&& f, G&& g)
      {
        using Const = Constant< std::decay_t<G> >;
        return MathematicalOperations::Product< std::decay_t<F>, Const >( std::forward<F>(f) , Const( std::forward<G>(g) ) );
      }
    };
  }
  /// @endcond

  /**
   * \brief overload of "+"-operator for the generation of functions.
   *
   * This is not to be confused with delayed computations with expression templates. This operator is only used to admit intuitive definition of functions.
   * If the resulting type represents a polynomial of order smaller than two, than you need to wrap it into Finalize to generate missing derivatives.
   */
  template < class F, class G,
             std::enable_if_t< Checks::isFunction< std::decay_t<F> >() ||
                               Checks::isFunction< std::decay_t<G> >() >* = nullptr >
  auto operator+ ( F&& f, G&& g )
  {
    return GenerateDetail::SumGenerator< F, G >::apply( std::forward<F>(f), std::forward<G>(g) );
  }

  /**
   * \brief overload of "*"-operator for the generation of functions.
   *
   * This is not to be confused with delayed computations with expression templates. This operator is only used to admit intuitive definition of functions.
   * If the resulting type represents a polynomial of order smaller than two, than you need to wrap it into Finalize to generate missing derivatives.
   */
  template < class F, class G,
             std::enable_if_t< Checks::isFunction< std::decay_t<F> >() ||
                               Checks::isFunction< std::decay_t<G> >() >* = nullptr >
  auto operator*( F&& f, G&& g )
  {
    return GenerateDetail::ProductGenerator< F, G >::apply( std::forward<F>(f), std::forward<G>(g) );
  }


  /**
   * \brief overload of "^"-operator for the generation of functions.
   *
   * This is not to be confused with delayed computations with expression templates. This operator is only used to admit intuitive definition of functions.
   * If the resulting type represents a polynomial of order smaller than two, than you need to wrap it into Finalize to generate missing derivatives.
   */
  template < class F,
             std::enable_if_t< Checks::isFunction< std::decay_t<F> >() >* = nullptr >
  auto operator^( F&& f, int k )
  {
    if(k!=2)
    {
      std::cerr << "operator^ only defined for k=2. Terminating." << std::endl;
      exit(1);
    }
    return MathematicalOperations::Squared< std::decay_t<F> >( std::forward<F>(f) );
  }

  /**
   * \brief overload of "<<"-operator for chaining functions \f$f\f$ and \f$g\f$ to \f$ f \circ g \f$.
   *
   * This is not to be confused with delayed computations with expression templates. This operator is only used to admit intuitive definition of functions.
   * If the resulting type represents a polynomial of order smaller than two, than you need to wrap it into Finalize to generate missing derivatives.
   */

  template < class F, class G,
             std::enable_if_t< Checks::isFunction< std::decay_t<F> >() &&
                               Checks::isFunction< std::decay_t<G> >() >* = nullptr >
  auto operator<<( F&& f, G&& g )
  {
    static_assert( !Checks::hasVariable< std::decay_t<F> >(), "Independent variables can not be on the left side of the chain operator." );
    return MathematicalOperations::Chain< std::decay_t<F>, std::decay_t<G> >( std::forward<F>(f), std::forward<G>(g) );
  }

  /**
   * \brief overload of "-"-operator for the generation of functions. Here the second argument is a constant that is wrapped in to an object of type Constant.
   *
   * This is not to be confused with delayed computations with expression templates. This operator is only used to admit intuitive definition of functions.
   * If the resulting type represents a polynomial of order smaller than two, than you need to wrap it into Finalize to generate missing derivatives.
   */
  template < class F, class T,
             std::enable_if_t< Checks::isFunction< std::decay_t<F> >() &&
                               !Checks::isFunction< std::decay_t<T> >()>* = nullptr >
  auto operator-( F&& f, T&& t )
  {
    return std::forward<F>(f) + ( -1 * std::forward<T>(t) );
  }

  /**
   * \brief overload of "-"-operator for the generation of functions. Here the first argument is a constant that is wrapped in to an object of type Constant.
   *
   * This is not to be confused with delayed computations with expression templates. This operator is only used to admit intuitive definition of functions.
   * If the resulting type represents a polynomial of order smaller than two, than you need to wrap it into Finalize to generate missing derivatives.
   */
  template < class F, class T,
             std::enable_if_t< Checks::isFunction< std::decay_t<F> >() &&
                               !Checks::isFunction< std::decay_t<T> >()>* = nullptr >
  auto operator-( T&& t, F&& f )
  {
    return std::forward<T>(t) + ( -1 * std::forward<F>(f) );
  }
}

#endif // FUNG_GENERATE_HH
