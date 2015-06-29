/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                             */
/*   This file is part of the C++-library RFFGen.                              */
/*   Copyright 2015 Lars Lubkoll                                               */
/*                                                                             */
/*   RFFGen is free software: you can redistribute it and/or modify            */
/*   it under the terms of the GNU General Public License as published by      */
/*   the Free Software Foundation, either version 3 of the License, or         */
/*   (at your option) any later version.                                       */
/*                                                                             */
/*   RFFGen is distributed in the hope that it will be useful,                 */
/*   but WITHOUT ANY WARRANTY; without even the implied warranty of            */
/*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             */
/*   GNU General Public License for more details.                              */
/*                                                                             */
/*   You should have received a copy of the GNU General Public License         */
/*   along with RFFGen.  If not, see <http://www.gnu.org/licenses/>.           */
/*                                                                             */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef RFFGEN_GENERATE_HH
#define RFFGEN_GENERATE_HH

#include <cassert>
#include <iostream>
#include <type_traits>

#include "constant.hh"
#include "operations.hh"
#include "Util/addMissingOperators.hh"
#include "Util/base.hh"
#include "variable.hh"

namespace RFFGen
{
  /**
   * \cond DOCUMENT_IMPLEMENATION_DETAILS
   */
  namespace GenerateDetail
  {
    template <class F, class G,
              bool = std::is_base_of<Base,F>::value,
              bool = std::is_base_of<Base,G>::value>
    struct SumGenerator;

    template <class F, class G>
    struct SumGenerator<F,G,true,true>
    {
      static auto apply(const F& f, const G& g)
      {
        return MathematicalOperations::Sum<F,G>(f,g);
      }
    };

    template <class F, class G>
    struct SumGenerator<F,G,true,false>
    {
      static auto apply(const F& f, const G& g)
      {
        return MathematicalOperations::Sum< F , Constant<G> >( f , constant(g) );
      }
    };

    template <class F, class G>
    struct SumGenerator<F,G,false,true>
    {
      static auto apply(const F& f, const G& g)
      {
        return MathematicalOperations::Sum< Constant<F> , G >( constant(f) , g );
      }
    };

    template <class F, class G,
              bool = std::is_base_of<Base,F>::value,
              bool = std::is_base_of<Base,G>::value,
              bool = std::is_arithmetic<F>::value,
              bool = std::is_arithmetic<G>::value>
    struct ProductGenerator;

    template <class F, class G>
    struct ProductGenerator<F,G,true,true,false,false>
    {
      static auto apply(const F& f, const G& g)
      {
        return MathematicalOperations::Product<F,G>(f,g);
      }
    };

    template <class F, class G>
    struct ProductGenerator<F,G,false,true,true,false>
    {
      static auto apply(F f, const G& g)
      {
        return MathematicalOperations::Scale<G>(f,g);
      }
    };

    template <class F, class G>
    struct ProductGenerator<F,G,true,false,false,true>
    {
      static auto apply(const F& f, G g)
      {
        return ProductGenerator<G,F,false,true,true,false>(g,f);
      }
    };

    template <class F, class G>
    struct ProductGenerator<F,G,false,true,false,false>
    {
      static auto apply(const F& f, const G& g)
      {
        return MathematicalOperations::Product<Constant<F>,G>(Constant<F>(f),g);
      }
    };

    template <class F, class G>
    struct ProductGenerator<F,G,true,false,false,false>
    {
      static auto apply(const F& f, const G& g)
      {
        return MathematicalOperations::Product< F,Constant<G> >( f , Constant<G>(g) );
      }
    };
  }
  /**
   * \endcond
   */

  /**
   * \brief overload of "+"-operator for the generation of functions.
   *
   * This is not to be confused with delayed computations with expression templates. This operator is only used to admit intuitive definition of functions.
   * If the resulting type represents a polynomial of order smaller than two, than you need to wrap it into Finalize to generate missing derivatives.
   */
  template <class F, class G,
            class = std::enable_if_t< std::is_base_of<Base,F>::value ||
                                      std::is_base_of<Base,G>::value > >
  auto operator+ (const F& f, const G& g)
  {
    return GenerateDetail::SumGenerator<F,G>::apply(f,g);
  }

  /**
   * \brief overload of "*"-operator for the generation of functions.
   *
   * This is not to be confused with delayed computations with expression templates. This operator is only used to admit intuitive definition of functions.
   * If the resulting type represents a polynomial of order smaller than two, than you need to wrap it into Finalize to generate missing derivatives.
   */
  template <class F, class G,
            class = std::enable_if_t< std::is_base_of<Base,F>::value || std::is_base_of<Base,G>::value > >
  auto operator*(const F& f, const G& g)
  {
    return GenerateDetail::ProductGenerator<F,G>::apply(f,g);
  }


  /**
   * \brief overload of "^"-operator for the generation of functions.
   *
   * This is not to be confused with delayed computations with expression templates. This operator is only used to admit intuitive definition of functions.
   * If the resulting type represents a polynomial of order smaller than two, than you need to wrap it into Finalize to generate missing derivatives.
   */
  template <class F,
            class = std::enable_if_t< std::is_base_of<Base,F>::value > >
  auto operator^ (const F& f, int k)
  {
    assert(k==2);
    if(k!=2)
    {
      std::cerr << "operator^ only defined for k=2. Terminating." << std::endl;
      exit(1);
    }
    return MathematicalOperations::Squared<F>(f);
  }

  /**
   * \brief overload of "<<"-operator for chaining functions \f$f\f$ and \f$g\f$ to \f$ f \circ g \f$.
   *
   * This is not to be confused with delayed computations with expression templates. This operator is only used to admit intuitive definition of functions.
   * If the resulting type represents a polynomial of order smaller than two, than you need to wrap it into Finalize to generate missing derivatives.
   */

  template <class F, class G,
            class = std::enable_if_t<std::is_base_of<Base,F>::value &&
                                     std::is_base_of<Base,G>::value> >
  auto operator<< (const F& f, const G& g)
  {
    static_assert(!Checks::hasVariable<F>(),"Independent variables can not be on the left side of the chain operator.");
    return MathematicalOperations::Chain<F,G>(f,g);
  }

  /**
   * \brief overload of "-"-operator for the generation of functions. Here the second argument is a constant that is wrapped in to an object of type Constant.
   *
   * This is not to be confused with delayed computations with expression templates. This operator is only used to admit intuitive definition of functions.
   * If the resulting type represents a polynomial of order smaller than two, than you need to wrap it into Finalize to generate missing derivatives.
   */
  template <class F, class T,
            class = std::enable_if_t<std::is_convertible<T,decltype(std::declval<F>().d0())>::value &&
                                     std::is_base_of<Base,F>::value> >
  auto operator-(const F& f, const T& t)
  {
    return f + ( -1 * t );
  }

  /**
   * \brief overload of "-"-operator for the generation of functions. Here the first argument is a constant that is wrapped in to an object of type Constant.
   *
   * This is not to be confused with delayed computations with expression templates. This operator is only used to admit intuitive definition of functions.
   * If the resulting type represents a polynomial of order smaller than two, than you need to wrap it into Finalize to generate missing derivatives.
   */
  template <class F, class T,
            class = std::enable_if_t<std::is_convertible<T,decltype(std::declval<F>().d0())>::value &&
                                     std::is_base_of<Base,F>::value> >
  auto operator-(const T& t, const F& f)
  {
    return t + ( -1 * f );
  }
}

#endif // RFFGEN_GENERATE_HH
