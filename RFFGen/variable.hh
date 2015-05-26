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

#ifndef RFFGEN_VARIABLE_HH
#define RFFGEN_VARIABLE_HH

#include <limits>
#include <type_traits>

#include "Util/base.hh"

namespace RFFGen
{
  /**
   * \cond DOCUMENT_IMPLEMENTATIONS_DETAILS
   */
  template <class,int> struct Variable;

  namespace VariableDetail
  {
    template <bool>
    struct Update
    {
      template <class T, class Arg>
      void operator()(T& t, const Arg& x)
      {
        static_assert(std::is_same<T,Arg>::value,"Updating variable with incompatible argument. Please check your input for the update-function.");
        t = x;
      }
    };

    template <>
    struct Update<false>
    {
      template <class T, class Arg>
      void operator()(const T&, const Arg&) {}
    };


    /// Check if Type is variable.
    template <class> struct IsVariable : std::false_type {};

    template <class T, int n>
    struct IsVariable< Variable<T,n> > : std::true_type {};


    /// Check if Type contains has variable.
    template <class Type> struct HasVariable : IsVariable<Type> {};

    template <template <class,class> class G, class F, class CheckF>
    struct HasVariable< G<F,CheckF> > : std::integral_constant< bool, HasVariable<F>::value > {};

    template <template <class,class,class,class> class H, class F, class G, class CheckF, class CheckG>
    struct HasVariable< H<G,F,CheckF,CheckG> > : std::integral_constant< bool , HasVariable<F>::value || HasVariable<G>::value > {};


    template <class, int id> struct HasVariableId : std::false_type {};

    template <class Type, int id0, int id>
    struct HasVariableId< Variable<Type,id0> , id > : std::integral_constant<bool,id==id0> {};

    template <template <class,class> class G, class F, int id, class CheckF>
    struct HasVariableId< G<F,CheckF> , id > : std::integral_constant< bool, HasVariableId<F,id>::value > {};

    template <template <class,class,class,class> class H, class F, class G, int id, class CheckF, class CheckG>
    struct HasVariableId< H<F,G,CheckF,CheckG> , id > : std::integral_constant< bool , HasVariableId<F,id>::value || HasVariableId<G,id>::value > {};


    template <class F, class G>
    struct ComputeMax
    {
      static constexpr int value = F::value > G::value ? F::value : G::value;
    };

    template <class F, class G>
    struct ComputeMin
    {
      static constexpr int value = F::value < G::value ? F::value : G::value;
    };

    template <class Type>
    struct MaxVariableId
        : std::integral_constant< int , std::numeric_limits<int>::lowest()>
    {};

    template <template <class,class> class G, class F, class CheckF>
    struct MaxVariableId< G<F,CheckF> >
        : std::integral_constant< int , MaxVariableId<F> >
    {};

    template <template <class,class,class,class> class H, class F, class G, class CheckF, class CheckG>
    struct MaxVariableId< H<F,G,CheckF,CheckG> >
        : std::integral_constant< int , ComputeMax< MaxVariableId<F> , MaxVariableId<G> >::value >
    {};

    template <class T, int id>
    struct MaxVariableId< Variable<T,id> > : std::integral_constant< int , id >
    {};


    template <class Type>
    struct MinVariableId
        : std::integral_constant< int , std::numeric_limits<int>::max()>
    {};

    template <template <class,class> class G, class F, class CheckF>
    struct MinVariableId< G<F,CheckF> >
        : std::integral_constant< int , MinVariableId<F> >
    {};

    template <template <class,class,class,class> class H, class F, class G, class CheckF, class CheckG>
    struct MinVariableId< H<F,G,CheckF,CheckG> >
        : std::integral_constant< int , ComputeMax< MinVariableId<F> , MinVariableId<G> >::value >
    {};

    template <class T, int id>
    struct MinVariableId< Variable<T,id> > : std::integral_constant< int , id >
    {};
  }
  /**
   * \endcond
   */

  /**
   * \brief Independent variable. Can be uniquely identified by its id.
   */
  template <class T, int id>
  struct Variable : Base/*, std::integral_constant<int,id>*/
  {
    Variable() = default;

    /// Construct variable with meaningful default value.
    explicit Variable(const T& t_) : t(t_) {}

    /// Update variable.
    template <int index, class Arg>
    void updateVariable(const Arg& t_)
    {
      VariableDetail::Update<index==id>()(t,t_);
    }

    /// Value of the variable. Convenient access to d0().
    const T& operator()() const noexcept
    {
      return d0();
    }

    /// Value of the variable.
    const T& d0() const noexcept
    {
      return t;
    }

    /**
     * \brief First directional derivative.
     *
     * Only available if id==index.
     */
    template < int index , class = std::enable_if_t< id == index > >
    const T& d1(const T& dt) const noexcept
    {
      return dt;
    }

  private:
    T t;
  };

  /// Generate variable from input type.
  template <int id, class T>
  Variable<T,id> variable(const T& t)
  {
    return Variable<T,id>(t);
  }


  namespace Checks
  {
    /**
     * \ingroup Checks
     * \brief Check if T is of type Variable<Type,n>.
     */
    template <class T>
    constexpr bool isVariable() { return VariableDetail::IsVariable<T>::value; }

    /**
     * \ingroup Checks
     * \brief Check if T contains a type Variable<Type,n>.
     */
    template <class T>
    constexpr bool hasVariable() { return VariableDetail::HasVariable<T>::value; }

    /**
     * \ingroup Checks
     * \brief Check if T contains a type Variable<Type,id>.
     */
    template <class T, int id>
    constexpr bool hasVariableId() { return VariableDetail::HasVariableId<T,id>::value; }

    /**
     * \ingroup Checks
     * \brief Check if T contains at least two variables.
     */
    template <class Type>
    constexpr bool hasMoreThanOneVariable()
    {
      return MinVariableId<Type>::value < MaxVariableId<Type>::value;
    }
  }
}

#endif // RFFGEN_VARIABLE_HH
