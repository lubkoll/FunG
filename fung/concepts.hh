// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#ifndef FUNG_CONCEPTS_HH
#define FUNG_CONCEPTS_HH

#include "fung/util/base.hh"

/**
 * \file concepts.hh
 */

namespace FunG
{
  namespace Concepts
  {
    /**
     * \cond DOCUMENT_IMPLEMETATION_DETAILS
     */
    struct unspecified;
    /**
     * \endcond
     */

    /**
     * \ingroup CopyConcept
     * \brief Requires copy-constructibility and copy-assignability.
     */
    struct CopyConcept
    {
      /// Copy-constructible.
      CopyConcept(const CopyConcept&);
      /// Copy-assignable.
      CopyConcept& operator=(const CopyConcept&);
    };

    /**
     * \ingroup MultiplyWithArithmeticFromLeftConcept
     * \brief Requires that multiplication with double and int can be performed either by in-place multiplication or by multiplication from the left.
     * \tparam Arg type to check
     */
    struct MultiplyWithArithmeticFromLeftConcept
    {
      /// In-place multiplication. Return type is not checked to support lazy evaluation.
      unspecified operator*=(double);
      /// In-place multiplication. Return type is not checked to support lazy evaluation.
      unspecified operator*=(int);

    };
    /**
     * \ingroup MultiplyWithArithmeticFromLeftConcept
     * \brief Multiplication from the left. Return type is not checked to support lazy evaluation.
     */
    unspecified operator*(double,MultiplyWithArithmeticFromLeftConcept);
    /**
     * \ingroup MultiplyWithArithmeticFromLeftConcept
     * \brief Multiplication from the left. Return type is not checked to support lazy evaluation.
     */
    unspecified operator*(int,MultiplyWithArithmeticFromLeftConcept);


    /**
     * \ingroup SummationConcept
     * \brief Requires that summation can be performed either by in-place summation or free summation.
     */
    struct SummationConcept
    {
      /// In-place summation. Return type is not checked to support lazy evaluation.
      unspecified operator+=(SummationConcept);
    };
    /**
     * \ingroup SummationConcept
     * \brief Summation. Return type is not checked to support lazy evaluation
     */
    unspecified operator+(SummationConcept,SummationConcept);


    /**
     * \ingroup MultiplicationConcept
     * \brief Requires that multiplication can be performed.
     *
     * Requires that either a free operator*(Arg1,Arg2) exists for multiplication or Arg1 provides either the in-place multiplication operator*=(Arg2) or the member function rightmultiplyany(Arg2).
     */
    struct MultiplicationConcept
    {
      struct Arg1; struct Arg2;
      /// In-place multiplication. Return type is not checked to support lazy evaluation.
      unspecified operator*=(Arg2);
      /// Multiplication via rightmultiplyany(Arg2). Return type is not checked to support lazy evaluation.
      unspecified rightmultiplyany(Arg2);
    };


    /**
     * \ingroup MultiplicationConcept
     * \brief Multiplication. Return type is not checked to support lazy evaluation.
     */
    unspecified operator*(MultiplicationConcept::Arg1,MultiplicationConcept::Arg2);


    /**
     * \ingroup Concepts
     * \brief Requirements on input types.
     *
     * Multiplication between different matrices is not checked here, since this would require to provide all possible matrices to multiply
     * a matrix of type Arg with.
     */
    struct ArithmeticConcept : CopyConcept , MultiplyWithArithmeticFromLeftConcept , SummationConcept
    {};


    /**
     * \ingroup Concepts
     * \brief Requirements for matrices.
     *
     * Access to matrix elements must be possible either via A[i][j] or A(i,j). Moreover the requirements of ArithmeticConcept must be satisfied.
     */
    struct MatrixConcept : ArithmeticConcept
    {
      /// Access to row, providing itself the same operator[](int).
      unspecified operator[](int);
      /// Access to entry.
      unspecified operator()(int,int);
    };


    /**
     * \ingroup Concepts
     * \brief Requirements for vectors.
     *
     * Access to vector elements must be possible either via A[i] or A(i). Moreover the requirements of ArithmeticConcept must be satisfied.
     */
    struct VectorConcept : ArithmeticConcept
    {
      /// Access to entry.
      unspecified operator[](int);
      /// Access to entry.
      unspecified operator()(int);
    };


    /**
     * \ingroup Concepts
     * \brief Requirements for symmetric matrices.
     *
     * The requirements of MatrixConcept must be satisfied and the number of rows and columns must be equal.
     */
    struct SquareMatrixConcept
        : ArithmeticConcept
    {};


    /**
     * \ingroup Concepts
     * \brief Minimal requirements for functions
     */
    struct FunctionConcept : Base , CopyConcept
    {
      /// Access to function value.
      unspecified d0() const;
    };
  }
}

#endif // FUNG_CONCEPTS_HH
