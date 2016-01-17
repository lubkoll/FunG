// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#ifndef FUNG_CONCEPT_CHECK_HH
#define FUNG_CONCEPT_CHECK_HH

#include "fung/util/extract_rows_and_cols.hh"
#include "fung/util/static_checks.hh"

namespace FunG
{
  namespace Concepts
  {
    /** @addtogroup ConceptCheck
     *  @{ */
    /// Static check if the requirements of CopyConcept are satisfied.
    template < class Arg >
    struct CopyConceptCheck
    {
      static_assert(std::is_copy_constructible<Arg>(), "CopyConcept: Input types must be copy-constructible.");
      static_assert(std::is_copy_assignable<Arg>(), "CopyConcept: Input types must be copy-assignable.");
    };


    /// Static check if the requirements of MultiplyWithArithmeticFromLeftConcept are satisfied.
    template < class Arg >
    struct MultiplyWithArithmeticFromLeftConceptCheck
    {
      static_assert(Checks::hasFree_Multiplication< Arg , double > () ||
                    Checks::hasMemFn_InPlaceMultiplication< Arg , double >(),
                    "MultiplyWithArithmeticFromLeftConcept: Input types must support multiplication with double from the left (operator*(double,const Arg&))");
      static_assert(Checks::hasFree_Multiplication< Arg , int > () ||
                    Checks::hasMemFn_InPlaceMultiplication< Arg , int >(),
                    "MultiplyWithArithmeticFromLeftConcept: Input types must support multiplication with int from the left (operator*(int,const Arg&))");
    };


    /// Static check if the requirements of SummationConcept are satisfied.
    template < class Arg >
    struct SummationConceptCheck
    {
      static_assert(Checks::hasFree_Summation< Arg >() ||
                    Checks::hasMemFn_InPlaceSummation< Arg >(),
                    "SummationConcept: Input types must support summation (operator+(const Arg&, const Arg&)");
    };


    /// Static check if the requirements of MultiplicationConcept are satisfied.
    template < class Arg1 , class Arg2 >
    struct MultiplicationConceptCheck
    {
      static_assert(Checks::hasFree_Multiplication< Arg1 , Arg2 >() ||
                    Checks::hasMemFn_rightmultiplyany< Arg1 , Arg2 >() ||
                    Checks::hasMemFn_InPlaceMultiplication< Arg1 , Arg2 >(),
                    "MultiplicationConcept: Input types must support multiplication (operator*(const Arg&, const Arg&");
    };


    /// Static check if the requirements of ArithmeticConcept are satisfied.
    template <class Arg>
    struct ArithmeticConceptCheck
        : CopyConceptCheck<Arg> ,
        MultiplyWithArithmeticFromLeftConceptCheck<Arg> ,
        SummationConceptCheck<Arg>
    {};


    /// Static check if the requirements of MatrixConcept are satisfied.
    template < class Matrix >
    struct MatrixConceptCheck : ArithmeticConceptCheck<Matrix>
    {
      static_assert(Checks::HasMemFn_SquareBracketAccessForMatrix<Matrix>::value ||
                    Checks::HasMemFn_RoundBracketAccessForMatrix<Matrix>::value,
                    "MatrixConcept: Currently only matrices that allow access to their elements via A[i][j] or A(i,j) are supported.\nYou may contact the developer to ask for further access or provide your own patch.");
    };


    /// Static check if the requirements of VectorConcept are satisfied.
    template < class Vector >
    struct VectorConceptCheck : ArithmeticConceptCheck<Vector>
    {
      static_assert(Checks::HasMemFn_SquareBracketAccessForVector<Vector>::value ||
                    Checks::HasMemFn_RoundBracketAccessForVector<Vector>::value,
                    "VectorConcept: Currently only vectors that allow access to their elements via v[i] or v(i) are supported.\nYou may contact the developer to ask for further access or provide your own patch.");
    };

    /// Static check if the requirements of SquareMatrixConcept are satisfied.
    template < class Matrix >
    struct SquareMatrixConceptCheck : MatrixConceptCheck<Matrix>, MultiplicationConceptCheck<Matrix,Matrix>
    {
      /// Require symmetric matrix
      static_assert( LinearAlgebra::NumberOfRows<Matrix>::value == LinearAlgebra::NumberOfColumns<Matrix>::value,
                     "SquareMatrixConcept: Input matrix must be symmetric.");
    };


    /// Static check if the requirements of FunctionConcept are satisfied.
    template <class F>
    struct FunctionConceptCheck : CopyConceptCheck<F>
    {
      static_assert( Checks::isFunction<F>() ,
                     "FunctionConcept: Functions must provide a member function d0() to access its value." );
    };
    /** @} */
  }
}

#endif // FUNG_CONCEPT_CHECK_HH
