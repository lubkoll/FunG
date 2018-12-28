#pragma once

#include <fung/util/chainer.hh>
#include <texy/util/string.hh>

#include <string>

namespace texy
{
    /** @addtogroup TexifyLinearAlgebraGroup
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
        class RightCauchyGreenStrainTensor : public FunG::Chainer< RightCauchyGreenStrainTensor >
        {
        public:
            RightCauchyGreenStrainTensor() = default;
            /**
             * @brief Constructor.
             * @param F point of evaluation.
             */
            explicit RightCauchyGreenStrainTensor( const std::string& F )
            {
                update( F );
            }

            /// Reset point of evaluation.
            void update( const std::string& F )
            {
                this->F = F;
            }

            /// Function value \f$ F^T * F \f$.
            std::string d0() const noexcept
            {
                return addScope( F ).append( "^T*" ).append( addScope( F ) );
            }

            /// First directional derivative \f$ F^T dF_1 + dF_1^T F \f$.
            std::string d1( const std::string& dF ) const
            {
                return addScope( F )
                    .append( "^T*" )
                    .append( addScope( dF ) )
                    .append( "+" )
                    .append( addScope( dF ) )
                    .append( "^T*" )
                    .append( addScope( F ) );
            }

            /// Second directional derivative \f$ dF_2^T dF_1 + dF_1^T dF_2 \f$.
            std::string d2( const std::string& dF1, const std::string& dF2 ) const
            {
                return addScope( dF2 )
                    .append( "^T*" )
                    .append( addScope( dF1 ) )
                    .append( "+" )
                    .append( addScope( dF1 ) )
                    .append( "^T*" )
                    .append( addScope( dF2 ) );
            }

        private:
            std::string F;
        };

        /**
         * @brief Left Cauchy-Green strain tensor \f$ F^T F \f$ for a symmetric matrix \f$ F \f$.
         *
         * Used in nonlinear material models based on the deformation gradient \f$\nabla\varphi\f$,
         * which takes the role of \f$F\f$.
         */
        class LeftCauchyGreenStrainTensor : public FunG::Chainer< LeftCauchyGreenStrainTensor >
        {
        public:
            LeftCauchyGreenStrainTensor() = default;
            /**
             * @brief Constructor.
             * @param F point of evaluation.
             */
            explicit LeftCauchyGreenStrainTensor( const std::string& F )
            {
                update( F );
            }

            /// Reset point of evaluation.
            void update( const std::string& F )
            {
                this->F = F;
            }

            /// Function value \f$ F^T * F \f$.
            std::string d0() const noexcept
            {
                return addScope( F ).append( "*" ).append( addScope( F ) ).append( "^T" );
            }

            /// First directional derivative \f$ F^T dF_1 + dF_1^T F \f$.
            std::string d1( const std::string& dF ) const
            {
                return addScope( F )
                    .append( "*" )
                    .append( addScope( dF ) )
                    .append( "^T+" )
                    .append( addScope( dF ) )
                    .append( "*" )
                    .append( addScope( F ) )
                    .append( "^T" );
            }

            /// Second directional derivative \f$ dF_2^T dF_1 + dF_1^T dF_2 \f$.
            std::string d2( const std::string& dF1, const std::string& dF2 ) const
            {
                return addScope( dF2 )
                    .append( "*" )
                    .append( addScope( dF1 ) )
                    .append( "^T+" )
                    .append( addScope( dF1 ) )
                    .append( "*" )
                    .append( addScope( dF2 ) )
                    .append( "^T" );
            }

        private:
            std::string F;
        };

        /**
         * \brief Generate the right Cauchy-Green strain tensor \f$A*A^T\f$.
         * \param A matrix
         * \return RightCauchyGreenStrainTensor<Matrix>(A)
         */
        auto strainTensor( const std::string& A )
        {
            return RightCauchyGreenStrainTensor{A};
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
            return RightCauchyGreenStrainTensor{f()}( f );
        }

        /**
         * \brief Generate the left Cauchy-Green strain tensor \f$A^T*A\f$.
         * \param A matrix
         * \return LeftCauchyGreenStrainTensor<Matrix>(A)
         */
        auto leftStrainTensor( const std::string& A )
        {
            return LeftCauchyGreenStrainTensor{A};
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
            return LeftCauchyGreenStrainTensor{f()}( f );
        }
    }
    /** @} */
}
