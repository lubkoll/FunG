#pragma once

#include <fung/util/chainer.hh>
#include <fung/util/static_checks.hh>

#include <cmath>

namespace FunG
{
    /** @addtogroup CMathGroup
     *  @{ */

    /*!
      @brief Error function including first three derivatives.

      For scalar functions directional derivatives are less interesting. Incorporating this function
      as building block for more complex functions requires directional derivatives. These occur
      during applications of the chain rule.
     */
    struct Erf : Chainer< Erf >
    {
        //! @copydoc Cos::Cos()
        explicit Erf( double x = 0. )
        {
            update( x );
        }

        //! @copydoc Cos::update()
        void update( double x )
        {
            x_ = x;
            value = std::erf( x_ );
            firstDerivative = scale * std::exp(-x_*x_);
        }

        //! @copydoc Cos::d0()
        double d0() const noexcept
        {
            return value;
        }

        //! @copydoc Cos::d1()
        double d1( double dx = 1. ) const
        {
            return firstDerivative * dx;
        }

        //! @copydoc Cos::d2()
        double d2( double dx = 1., double dy = 1. ) const
        {
            return -2*x_*d1(dx)*dy;
        }

        //! @copydoc Cos::d3()
        double d3( double dx = 1., double dy = 1., double dz = 1. ) const
        {
            return (4*x_*x_ - 2)*d1(dx)*dy*dz;
        }

    private:
        double scale = 2/std::sqrt(M_PI);
        double value = 0.;
        double firstDerivative = 1.;
        double x_ = 0;
    };

    /*!
      @brief Generate \f$ \erf\circ f \f$.
      @param f function mapping into a scalar space
      @return object of type MathematicalOperations::Chain<Erf,Function>
     */
    template < class Function, class = std::enable_if_t< Checks::isFunction< Function >() > >
    auto erf( const Function& f )
    {
        return Erf()( f );
    }
    /** @} */
}
