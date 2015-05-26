#error Documentation only.


/**
 * \defgroup Examples Examples
 *
 * \brief Examples mainly from hyperelasticity.
 *
 * Contains models of the neoHookean-material model (Rubber/neoHooke.hh), the Mooney-Rivlin model (Rubber/mooneyRivlin.hh),
 * an extended Mooney-Rivlin model for the description of skin tissue (Biomechanics/skinTissue_Hendriks.hh) and Fung-elastic
 * models for the description of adipose (Biomechanics/adiposeTissue_SommerHolzapfel.hh) and muscle tissue (Biomechanics/muscleTissue_Martins.hh).
 */

    /**
     * \defgroup Rubber Rubber
     * \ingroup Examples
     * \brief Isotropic models for the description of rubber materials (neo-Hookean and Mooney-Rivlin models).
     */

    /**
     * \defgroup Biomechanics Biomechanics
     * \ingroup Examples
     * \brief Models for the description of different biologial soft tissues.
     */



/**
 * \defgroup MathematicalOperationsGroup Mathematical Operations
 * \brief Mathematical operations and corresponding differentation rules.
 */



/**
 * \defgroup CMathGroup CMath
 * \brief Wrappers for functions from \<cmath\>.
 *
 * Provides Cos, Sin, LN, Exp, Pow<int>, Sqrt.
 * You may also use the objects cosine, sine, ln, expo, sqr and the template function power\<int\>().
 */

/**
 * \defgroup LinearAlgebraGroup Linear Algebra
 * \brief Functionality from linear algebra such as (modified) principal and mixed matrix invariants.
 */



/**
 * \defgroup Concepts
 * \brief Requirements on the input types.
 *
 * For purpose of documentation free functions are declared as friends in concepts.hh.
 * Thus to determine exactly the requirements on input types you may have inspect the module page of the elementary concepts.
 * Or just compile your code and wait for the static assertions to tell you if some functionality is missing.
 */

    /**
     * \defgroup CopyConcept
     * \ingroup Concepts
     * \brief Require copy-constructability and copy-assignability.
     */


    /**
     * \defgroup MultiplyWithArithmeticFromLeftConcept
     * \ingroup Concepts
     * \brief Requires that multiplication with double and int can be performed either by in-place multiplication or by multiplication from the left.
     */

    /**
     * \defgroup SummationConcept
     * \ingroup Concepts
     * \brief Requires that summation can be performed.
     */

    /**
     * \defgroup MultiplicationConcept
     * \ingroup Concepts
     * \brief Requires that multiplication can be performed.
     */

    /**
     * \defgroup ConceptCheck
     * \ingroup Concepts
     * \brief Static checks for requirements specified in Concepts.
     */

  // Namespaces

  /**
   * \namespace RFFGen
   * \brief Main namespace of the RFFGen library.
   * \see MathematicalOperations
   * \see CMath
   */

  /**
   * \ingroup MathematicalOperationsGroup
   * \namespace MathematicalOperations
   * \brief Mathematical operations and corresponding differentation rules.
   * \see MathematicalOperationsGroup
   */

  /**
   * \ingroup CMathGroup
   * \namespace CMath
   * \brief Wrappers for functions from \<cmath\>.
   * \see CMathGroup
   */

  /**
   * \ingroup LinearAlgebraGroup
   * \namespace LinearAlgebra
   * \brief Functionality from linear algebra such as (modified) principal and mixed matrix invariants.
   * \see LinearAlgebraGroup
   */

  /**
   * \namespace Checks
   * \brief Static checks for the presence of different operators and functions.
   */