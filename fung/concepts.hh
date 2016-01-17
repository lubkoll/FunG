// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#ifndef FUNG_CONCEPTS_HH
#define FUNG_CONCEPTS_HH

namespace FunG
{
  namespace Concepts
  {
    /// @cond
    struct unspecified;
    /// @endcond

    /** @addtogroup ConceptGroup
     *  @{ */

    /// Requires copy-constructibility and copy-assignability.
    struct CopyConcept
    {
      /// Copy-constructible.
      CopyConcept(const CopyConcept&);
      /// Copy-assignable.
      CopyConcept& operator=(const CopyConcept&);
    };

    /// Requires that multiplication with double and int can be performed either by in-place multiplication or by multiplication from the left.
    struct MultiplyWithArithmeticFromLeftConcept
    {
      /// In-place multiplication. Return type is not checked to support lazy evaluation.
      unspecified operator*=(double);
      /// In-place multiplication. Return type is not checked to support lazy evaluation.
      unspecified operator*=(int);

    };

    /// Multiplication from the left. Return type is not checked to support lazy evaluation.
    unspecified operator*(double,MultiplyWithArithmeticFromLeftConcept);

    /// Multiplication from the left. Return type is not checked to support lazy evaluation.
    unspecified operator*(int,MultiplyWithArithmeticFromLeftConcept);


    /// Requires that summation can be performed either by in-place summation or free summation.
    struct SummationConcept
    {
      /// In-place summation. Return type is not checked to support lazy evaluation.
      unspecified operator+=(SummationConcept);
    };

    /// Summation. Return type is not checked to support lazy evaluation
    unspecified operator+(SummationConcept,SummationConcept);


    /**
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


    /// Multiplication. Return type is not checked to support lazy evaluation.
    unspecified operator*(MultiplicationConcept::Arg1,MultiplicationConcept::Arg2);


    /**
     * @brief Requirements on input types.
     *
     * Multiplication between different matrices is not checked here, since this would require to provide all possible matrices to multiply
     * a matrix of type Arg with.
     */
    struct ArithmeticConcept : CopyConcept , MultiplyWithArithmeticFromLeftConcept , SummationConcept
    {};


    /**
     * @brief Requirements for matrices.
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
     * @brief Requirements for vectors.
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
     * @brief Requirements for symmetric matrices.
     *
     * The requirements of MatrixConcept must be satisfied and the number of rows and columns must be equal.
     */
    struct SquareMatrixConcept
        : ArithmeticConcept
    {};


    /// Minimal requirements for functions
    struct FunctionConcept : CopyConcept
    {
      /// Access to function value.
      unspecified operator()() const;
    };

    /** @} */
  }
}

#endif // FUNG_CONCEPTS_HH
