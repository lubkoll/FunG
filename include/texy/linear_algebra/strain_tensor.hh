#pragma once

#include "transpose.hh"
#include <fung/mathematical_operations/sum.hh>
#include <fung/util/add_transposed_matrix.hh>
#include <fung/util/chainer.hh>

namespace FunG
{
    /// @cond
    namespace Concepts
    {
        template < class >
        struct SquareMatrixConceptCheck;
    }
    ///@endcond

    /** @addtogroup LinearAlgebraGroup
     * @{
     */
    namespace LinearAlgebra
    {
        /**
         * @brief Right Cauchy-Green strain tensor \f$ F^T F \f$ for a symmetric matrix \f$ F \f$.
         *
         * Used in nonlinear material models based on the deformation gradient \f$\nabla\varphi\f$,
         * which takes the role of \f$F\f$.
         */
        template < class Matrix, class = Concepts::SquareMatrixConceptCheck< Matrix > >
        class RightCauchyGreenStrainTensor
            : public Chainer< RightCauchyGreenStrainTensor<
                  Matrix, Concepts::SquareMatrixConceptCheck< Matrix > > >
        {
        public:
            RightCauchyGreenStrainTensor() = default;
            /**
             * @brief Constructor.
             * @param F point of evaluation.
             */
            explicit RightCauchyGreenStrainTensor( const Matrix& F )
            {
                update( F );
            }

            /// Reset point of evaluation.
            void update( const Matrix& F )
            {
                FT = Detail::transpose( F );
                FTF = FT * F;
            }

            /// Function value \f$ F^T * F \f$.
            const Matrix& d0() const noexcept
            {
                return FTF;
            }

            /// First directional derivative \f$ F^T dF_1 + dF_1^T F \f$.
            Matrix d1( const Matrix& dF1 ) const
            {
                Matrix FTdF1 = FT * dF1;
                return addTransposed( FTdF1 );
            }

            /// Second directional derivative \f$ dF_2^T dF_1 + dF_1^T dF_2 \f$.
            Matrix d2( const Matrix& dF1, const Matrix& dF2 ) const
            {
                Matrix dF2TdF1 = Detail::transpose( dF2 ) * dF1;
                return addTransposed( dF2TdF1 );
            }

        private:
            Matrix FT, FTF;
        };

        /**
         * @brief Left Cauchy-Green strain tensor \f$ F^T F \f$ for a symmetric matrix \f$ F \f$.
         *
         * Used in nonlinear material models based on the deformation gradient \f$\nabla\varphi\f$,
         * which takes the role of \f$F\f$.
         */
        template < class Matrix, class = Concepts::SquareMatrixConceptCheck< Matrix > >
        class LeftCauchyGreenStrainTensor
            : public Chainer< LeftCauchyGreenStrainTensor<
                  Matrix, Concepts::SquareMatrixConceptCheck< Matrix > > >
        {
        public:
            LeftCauchyGreenStrainTensor() = default;
            /**
             * @brief Constructor.
             * @param F point of evaluation.
             */
            explicit LeftCauchyGreenStrainTensor( const Matrix& F )
            {
                update( F );
            }

            /// Reset point of evaluation.
            void update( const Matrix& F )
            {
                FT = Detail::transpose( F );
                FFT = F * FT;
            }

            /// Function value \f$ F^T * F \f$.
            const Matrix& d0() const noexcept
            {
                return FFT;
            }

            /// First directional derivative \f$ F^T dF_1 + dF_1^T F \f$.
            Matrix d1( const Matrix& dF1 ) const
            {
                Matrix FTdF1 = dF1 * FT;
                return addTransposed( FTdF1 );
            }

            /// Second directional derivative \f$ dF_2^T dF_1 + dF_1^T dF_2 \f$.
            Matrix d2( const Matrix& dF1, const Matrix& dF2 ) const
            {
                Matrix dF1dF2T = dF1 * Detail::transpose( dF2 );
                return addTransposed( dF1dF2T );
            }

        private:
            Matrix FT, FFT;
        };

        /**
         * \brief Generate the right Cauchy-Green strain tensor \f$A*A^T\f$.
         * \param A matrix
         * \return RightCauchyGreenStrainTensor<Matrix>(A)
         */
        template < class Matrix, std::enable_if_t< !Checks::isFunction< Matrix >() >* = nullptr >
        auto strainTensor( const Matrix& A )
        {
            return RightCauchyGreenStrainTensor< Matrix >{A};
        }

        /**
         * \brief Generate the right Cauchy-Green strain tensor \f$f*f^T\f$, where
         * \f$f:\cdot\mapsto\mathbb{R}^{n,n} \f$.
         * \param f function object mapping into a space of square matrices
         * \return RightCauchyGreenStrainTensor< decay_t<decltype(f())> >(f())( f )
         */
        template < class F, std::enable_if_t< Checks::isFunction< F >() >* = nullptr >
        auto strainTensor( const F& f )
        {
            return RightCauchyGreenStrainTensor< decay_t< decltype( f() ) > >{f()}( f );
        }

        /**
         * \brief Generate the left Cauchy-Green strain tensor \f$A^T*A\f$.
         * \param A matrix
         * \return LeftCauchyGreenStrainTensor<Matrix>(A)
         */
        template < class Matrix, std::enable_if_t< !Checks::isFunction< Matrix >() >* = nullptr >
        auto leftStrainTensor( const Matrix& A )
        {
            return LeftCauchyGreenStrainTensor< Matrix >{A};
        }

        /**
         * \brief Generate the left Cauchy-Green strain tensor \f$f^T*f\f$, where
         * \f$f:\cdot\mapsto\mathbb{R}^{n,n} \f$.
         * \param f function object mapping into a space of square matrices
         * \return LeftCauchyGreenStrainTensor< decay_t<decltype(f())> >(f())( f )
         */
        template < class F, std::enable_if_t< Checks::isFunction< F >() >* = nullptr >
        auto leftStrainTensor( const F& f )
        {
            return LeftCauchyGreenStrainTensor< decay_t< decltype( f() ) > >{f()}( f );
        }
    }
    /** @} */
}
