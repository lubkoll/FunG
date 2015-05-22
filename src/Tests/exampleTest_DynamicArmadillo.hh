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

#ifndef RFFGEN_PERFORMANCE_TEST_EXAMPLE_TEST_DYNAMIC_ARMADILLO_HH
#define RFFGEN_PERFORMANCE_TEST_EXAMPLE_TEST_DYNAMIC_ARMADILLO_HH

#include <chrono>
#include <iostream>

#include "Examples/Rubber/neoHooke.hh"
#include "Examples/Rubber/mooneyRivlin.hh"
#include "Examples/Biomechanics/skinTissue_Hendriks.hh"
#include "Examples/Biomechanics/adiposeTissue_SommerHolzapfel.hh"
#include "Examples/Biomechanics/muscleTissue_Martins.hh"
#include "math.hh"
#include "../Util/armadillo.hh"

namespace DynamicArmadilloDetail
{
  template <class M, class Function>
  void runTest(Function& f)
  {
    using std::cout;
    using std::endl;
    using namespace std::chrono;
    auto nUpdates = 10'000'000u;
    auto nTimes = 1;
    auto nEval = nUpdates * nTimes;

    M a(3,3);
    a.eye();
    M da0 = 2*a, da1 = 3*a, da2 = 4*a;
    auto val = 0.;

    cout << "number of updates: " << nUpdates << endl;
    auto startTime = high_resolution_clock::now();
    for(auto i=0u; i<nUpdates; ++i){ if(i%2==0) a*=1.01; else a*=1./1.01; f.update(a); }
    cout << "updates: " << duration_cast<milliseconds>(high_resolution_clock::now() - startTime).count() << "ms\n";
    cout << "number of evaluations: " << nEval << endl;
    startTime = high_resolution_clock::now();
    for(auto i=0u; i<nEval; ++i) val = f();
    cout << "d0: " << duration_cast<nanoseconds>(high_resolution_clock::now() - startTime).count() << "ns\n";
    cout << "value: " << val << endl;
    startTime = high_resolution_clock::now();
    for(auto i=0u; i<nEval; ++i) val = f.template d1<0>(da0);
    cout << "d1: " << duration_cast<milliseconds>(high_resolution_clock::now() - startTime).count() << "ms\n";
    cout << "value: " << val << endl;
    startTime = high_resolution_clock::now();
    for(auto i=0u; i<nEval; ++i) val = f.template d2<0,0>(da0,da1);
    cout << "d2: " << duration_cast<milliseconds>(high_resolution_clock::now() - startTime).count() << "ms\n";
    cout << "value: " << val << endl;
    startTime = high_resolution_clock::now();
    for(auto i=0u; i<nEval; ++i) val = f.template d3<0,0,0>(da0,da1,da2);
    cout << "d3: " << duration_cast<milliseconds>(high_resolution_clock::now() - startTime).count() << "ms\n";
    cout << "value: " << val << endl;
    cout << endl;
  }
}

void testExamples_DynamicArmadillo()
{
  using std::cout;
  using std::endl;
  using RFFGen::CMath::LN;
  using RFFGen::CMath::Pow;
  using namespace DynamicArmadilloDetail;
  constexpr int dim = 3;
  using M = arma::mat;//::fixed<dim,dim>;

  cout << "Starting performance test with dynamic armadillo matrices.\n" << endl;
  cout << RFFGen::Checks::AccessTo_n_rows<M>() << endl;
  cout << RFFGen::Checks::isDynamicMatrix<M>() << endl;
  auto c0 = 1.;
  auto c1 = 1.;
  auto d0 = 1.;
  auto d1 = 1.;
  M fiberTensor(3,3), I(3,3);
  I.eye();
  fiberTensor.zeros();
  fiberTensor(0,0) = 1.;
  cout << RFFGen::LinearAlgebra::rows(fiberTensor) << endl;
  cout << RFFGen::LinearAlgebra::cols(fiberTensor) << endl;
  cout << RFFGen::LinearAlgebra::rows(I) << endl;
  cout << RFFGen::LinearAlgebra::cols(I) << endl;

  auto incompressibleNeoHooke = RFFGen::incompressibleNeoHooke<M,dim>(c0,I);
  auto incompressibleModifiedNeoHooke = RFFGen::modifiedIncompressibleNeoHooke<M,dim>(c0,I);
  auto compressibleNeoHooke = RFFGen::compressibleNeoHooke<Pow<2>,LN,M,dim>(c0,d0,d1,I);
  auto compressibleModifiedNeoHooke = RFFGen::modifiedCompressibleNeoHooke<Pow<2>,LN,M,dim>(c0,d0,d1,I);

  auto incompressibleMooneyRivlin = RFFGen::incompressibleMooneyRivlin<M,dim>(c0,c1,I);
  auto compressibleMooneyRivlin = RFFGen::compressibleMooneyRivlin<Pow<2>,LN,M,dim>(c0,c1,d0,d1,I);

  auto incompressibleSkin = RFFGen::incompressibleSkin_Hendriks<M,dim>(I);
  auto compressibleSkin = RFFGen::compressibleSkin_Hendriks<Pow<2>,LN,M,dim>(d0,d1,I);

  auto incompressibleAdipose = RFFGen::incompressibleAdiposeTissue_SommerHolzapfel<M,dim>(fiberTensor,I);
  auto compressibleAdipose = RFFGen::compressibleAdiposeTissue_SommerHolzapfel<Pow<2>,LN,M,dim>(d0,d1,fiberTensor,I);

  auto incompressibleMuscle = RFFGen::incompressibleMuscleTissue_Martins<M,dim>(fiberTensor,I);
  auto compressibleMuscle = RFFGen::compressibleMuscleTissue_Martins<Pow<2>,LN,M,dim>(d0,d1,fiberTensor,I);

  using namespace std::chrono;
  auto startTime = high_resolution_clock::now();
  cout << "incompressible neo-Hooke" << endl;
  runTest<M>(incompressibleNeoHooke);

  cout << "modified incompressible neo-Hooke" << endl;
  runTest<M>(incompressibleModifiedNeoHooke);

  cout << "compressible neo-Hooke" << endl;
  runTest<M>(compressibleNeoHooke);

  cout << "modified compressible neo-Hooke" << endl;
  runTest<M>(compressibleModifiedNeoHooke);

  cout << "incompressible Mooney-Rivlin" << endl;
  runTest<M>(incompressibleMooneyRivlin);

  cout << "compressible Mooney-Rivlin" << endl;
  runTest<M>(compressibleMooneyRivlin);

  cout << "incompressible skin (Hendriks)" << endl;
  runTest<M>(incompressibleSkin);

  cout << "compressible skin (Hendriks)" << endl;
  runTest<M>(compressibleSkin);

  cout << "incompressible adipose (Sommer,Holzapfel et al)" << endl;
  runTest<M>(incompressibleAdipose);

  cout << "compressible adipose (Sommer,Holzapfel et al)" << endl;
  runTest<M>(compressibleAdipose);

  cout << "incompressible muscle (Martins et al)" << endl;
  runTest<M>(incompressibleMuscle);

  cout << "compressible muscle (Martins et al)" << endl;
  runTest<M>(compressibleMuscle);

  cout << "Terminating performance test with dynamic armadillo matrices.\n" << endl;
  cout << "Overall computation time: " << duration_cast<seconds>(high_resolution_clock::now() - startTime).count() << "s\n";
}

#endif /* RFFGEN_PERFORMANCE_TEST_EXAMPLE_TEST_DYNAMIC_ARMADILLO_HH */
