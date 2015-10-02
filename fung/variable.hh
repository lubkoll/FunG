/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                             */
/*   This file is part of the C++-library FunG.                              */
/*   Copyright 2015 Lars Lubkoll                                               */
/*                                                                             */
/*   FunG is free software: you can redistribute it and/or modify            */
/*   it under the terms of the GNU General Public License as published by      */
/*   the Free Software Foundation, either version 3 of the License, or         */
/*   (at your option) any later version.                                       */
/*                                                                             */
/*   FunG is distributed in the hope that it will be useful,                 */
/*   but WITHOUT ANY WARRANTY; without even the implied warranty of            */
/*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             */
/*   GNU General Public License for more details.                              */
/*                                                                             */
/*   You should have received a copy of the GNU General Public License         */
/*   along with FunG.  If not, see <http://www.gnu.org/licenses/>.           */
/*                                                                             */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef FUNG_VARIABLE_HH
#define FUNG_VARIABLE_HH

#include <limits>
#include <type_traits>

#include "fung/util/base.hh"
#include "fung/util/chainer.hh"

namespace FunG
{
  /**
   * \cond DOCUMENT_IMPLEMENTATIONS_DETAILS
   */
  template <class,int> struct Variable;
  namespace Concepts { template <class> class FunctionConceptCheck; }

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

    template <template <class,class> class G, class F>
    struct HasVariable< G<F,Concepts::FunctionConceptCheck<F> > > : std::integral_constant< bool, HasVariable<F>::value > {};

    template <template <class,class,class,class> class H, class F, class G>
    struct HasVariable< H<G,F,Concepts::FunctionConceptCheck<F>,Concepts::FunctionConceptCheck<G> > > : std::integral_constant< bool , HasVariable<F>::value || HasVariable<G>::value > {};


    template <class, int id> struct HasVariableId : std::false_type {};

    template <class Type, int id0, int id>
    struct HasVariableId< Variable<Type,id0> , id > : std::integral_constant<bool,id==id0> {};

    template <template <class,class> class G, class F, int id>
    struct HasVariableId< G<F,Concepts::FunctionConceptCheck<F> > , id > : std::integral_constant< bool, HasVariableId<F,id>::value > {};

    template <template <class,class,class,class> class H, class F, class G, int id>
    struct HasVariableId< H<F,G,Concepts::FunctionConceptCheck<F>,Concepts::FunctionConceptCheck<G> > , id > : std::integral_constant< bool , HasVariableId<F,id>::value || HasVariableId<G,id>::value > {};


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

    template <template <class,class> class G, class F>
    struct MaxVariableId< G<F,Concepts::FunctionConceptCheck<F> > >
        : std::integral_constant< int , MaxVariableId<F>::value >
    {};

    template <template <class,class,class,class> class H, class F, class G>
    struct MaxVariableId< H<F,G,Concepts::FunctionConceptCheck<F> ,Concepts::FunctionConceptCheck<F> > >
        : std::integral_constant< int , ComputeMax< MaxVariableId<F> , MaxVariableId<G> >::value >
    {};

    template <class T, int id>
    struct MaxVariableId< Variable<T,id> > : std::integral_constant< int , id >
    {};


    template <class Type>
    struct MinVariableId
        : std::integral_constant< int , std::numeric_limits<int>::max()>
    {};

    template <template <class,class> class G, class F>
    struct MinVariableId< G<F,Concepts::FunctionConceptCheck<F> > >
        : std::integral_constant< int , MinVariableId<F>::value >
    {};

    template <template <class,class,class,class> class H, class F, class G>
    struct MinVariableId< H<F,G,Concepts::FunctionConceptCheck<F>,Concepts::FunctionConceptCheck<G> > >
        : std::integral_constant< int , ComputeMax< MinVariableId<F> , MinVariableId<G> >::value >
    {};

    template <class T, int id>
    struct MinVariableId< Variable<T,id> > : std::integral_constant< int , id >
    {};



    template <class F, int id> struct VariableType { using type = void; };

    template <template <class,class> class G, class F, int id>
    struct VariableType< G<F,Concepts::FunctionConceptCheck<F> > , id >
    {
      using type = typename VariableType<F,id>::type;
    };

    template <template <class,class,class,class> class H, class F, class G, int id>
    struct VariableType< H<F,G,Concepts::FunctionConceptCheck<F>,Concepts::FunctionConceptCheck<G> > , id >
    {
      using type = std::conditional_t< std::is_same<void,typename VariableType<F,id>::type>::value ,
                                       typename VariableType<G,id>::type, typename VariableType<F,id>::type>;
    };

    template <class T, int id>
    struct VariableType< Variable<T,id>, id > { using type = T; };
  }
  /**
   * \endcond
   */

  /**
   * \brief Independent variable. Can be uniquely identified by its id.
   */
  template <class T, int id>
  struct Variable : Base , Chainer< Variable<T,id> >
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

  /**
   * \brief Get underlying type of variable with index id.
   */
  template <class F, int id>
  using Variable_t = typename VariableDetail::VariableType<F,id>::type;

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
      return VariableDetail::MinVariableId<Type>::value < VariableDetail::MaxVariableId<Type>::value;
    }

    /**
     * \ingroup Checks
     * \brief Check if variable with index id has type Type.
     */
    template <class F, class Type, int id>
    constexpr bool checkArgument()
    {
      return std::is_same<typename VariableDetail::VariableType<F,id>::type, Type>::value;
    }
  }
}

#endif // FUNG_VARIABLE_HH