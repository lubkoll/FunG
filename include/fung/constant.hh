#pragma once

#include <fung/util/chainer.hh>

namespace FunG
{
  /// @cond
  template <class> struct ArithmeticConceptCheck;
  /// @endcond

  /// Wrap a constant.
  template <class Type, class = ArithmeticConceptCheck<Type> >
  struct Constant : Chainer< Constant<Type , ArithmeticConceptCheck<Type> > >
  {
    Constant() = default;

   /// Construct constant from copy.
   Constant(Type t_) : t(std::move(t_))
   {}

   /// Function value.
   const Type& d0() const noexcept
   {
     return t;
   }

  private:
    Type t;
  };

//  /**
//   * \brief Generate a constant function that stores its argument as constant reference.
//   *
//   * This admits to use variable constant arguments, i.e. parameters that we want to study.
//   */
//  template <class Arg>
//  auto constRef(const Arg& x)
//  {
//    return Constant<const Arg&>(x);
//  }

  /**
   * \brief Wrap a constant.
   * \return Constant<Arg>(x)
   */
  template <class Arg>
  auto constant(const Arg& x)
  {
    return Constant<Arg>(x);
  }
}
