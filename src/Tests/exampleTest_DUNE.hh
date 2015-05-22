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

#ifndef RFFGEN_PERFORMANCE_TEST_EXAMPLE_TEST_DUNE_HH
#define RFFGEN_PERFORMANCE_TEST_EXAMPLE_TEST_DUNE_HH

#include <chrono>
#include <iostream>

#include <dune/common/fmatrix.hh>

#include "Examples/Rubber/neoHooke.hh"
#include "Examples/Rubber/mooneyRivlin.hh"
#include "Examples/Biomechanics/skinTissue_Hendriks.hh"
#include "Examples/Biomechanics/adiposeTissue_SommerHolzapfel.hh"
#include "Examples/Biomechanics/muscleTissue_Martins.hh"

template <class M, class Function>
void runTest_DUNE(Function& f)
{
  using std::cout;
  using std::endl;
  using namespace std::chrono;
  auto nUpdates = 10'000'000u;
  auto nTimes = 1;
  auto nEval = nUpdates * nTimes;

  M a(0.);
  a[0][0] = a[1][1] = a[2][2] = 1.;
  M da0(a), da1(a), da2(a);
  da0 *= 2;
  da1 *= 3;
  da2 *= 4;
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
  for(auto i=0u; i<nEval; ++i){ if(i%2==0) da0 *= 1.01; else da0 *= 1./1.01; val = f.template d1<0>(da0); }
  cout << "d1: " << duration_cast<milliseconds>(high_resolution_clock::now() - startTime).count() << "ms\n";
  cout << "value: " << val << endl;
  startTime = high_resolution_clock::now();
  for(auto i=0u; i<nEval; ++i){ if(i%2==0) da0 *= 1.01; else da0 *= 1./1.01; da1 = da0; val = f.template d2<0,0>(da0,da1); }
  cout << "d2: " << duration_cast<milliseconds>(high_resolution_clock::now() - startTime).count() << "ms\n";
  cout << "value: " << val << endl;
  startTime = high_resolution_clock::now();
  for(auto i=0u; i<nEval; ++i){ if(i%2==0) da0 *= 1.01; else da0 *= 1./1.01; da1 = da2 = da0; val = f.template d3<0,0,0>(da0,da1,da2); }
  cout << "d3: " << duration_cast<milliseconds>(high_resolution_clock::now() - startTime).count() << "ms\n";
  cout << "value: " << val << endl;
  cout << endl;
}

void testExamples_DUNE()
{
  using std::cout;
  using std::endl;
  using RFFGen::CMath::LN;
  using RFFGen::CMath::Pow;
  using RFFGen::LinearAlgebra::Invariant;
  constexpr int dim = 3;
  using M = Dune::FieldMatrix<double,dim,dim>;

  cout << "Starting performance test with DUNE matrices.\n" << endl;

  auto c0 = 1.;
  auto c1 = 1.;
  auto d0 = 1.;
  auto d1 = 1.;
  M fiberTensor(0.), I(0.);
  I[0][0] = I[1][1] = I[2][2] = fiberTensor[0][0] = 1.;

  auto incompressibleNeoHooke = RFFGen::incompressibleNeoHooke(c0,I);
  auto incompressibleModifiedNeoHooke = RFFGen::modifiedIncompressibleNeoHooke(c0,I);
  auto compressibleNeoHooke = RFFGen::compressibleNeoHooke<Pow<2>,LN>(c0,d0,d1,I);
  auto compressibleModifiedNeoHooke = RFFGen::modifiedCompressibleNeoHooke<Pow<2>,LN>(c0,d0,d1,I);

  auto incompressibleMooneyRivlin = RFFGen::incompressibleMooneyRivlin(c0,c1,I);
  auto compressibleMooneyRivlin = RFFGen::compressibleMooneyRivlin<Pow<2>,LN>(c0,c1,d0,d1,I);

  auto incompressibleSkin = RFFGen::incompressibleSkin_Hendriks(I);
  auto compressibleSkin = RFFGen::compressibleSkin_Hendriks<Pow<2>,LN>(d0,d1,I);

  auto incompressibleAdipose = RFFGen::incompressibleAdiposeTissue_SommerHolzapfel(fiberTensor,I);
  auto compressibleAdipose = RFFGen::compressibleAdiposeTissue_SommerHolzapfel<Pow<2>,LN>(d0,d1,fiberTensor,I);

  auto incompressibleMuscle = RFFGen::incompressibleMuscleTissue_Martins(fiberTensor,I);
  auto compressibleMuscle = RFFGen::compressibleMuscleTissue_Martins<Pow<2>,LN>(d0,d1,fiberTensor,I);

  using namespace std::chrono;
  auto startTime = high_resolution_clock::now();
  cout << "incompressible neo-Hooke" << endl;
  runTest_DUNE<M>(incompressibleNeoHooke);

  cout << "modified incompressible neo-Hooke" << endl;
  runTest_DUNE<M>(incompressibleModifiedNeoHooke);

  cout << "compressible neo-Hooke" << endl;
  runTest_DUNE<M>(compressibleNeoHooke);

  cout << "modified compressible neo-Hooke" << endl;
  runTest_DUNE<M>(compressibleModifiedNeoHooke);

  cout << "incompressible Mooney-Rivlin" << endl;
  runTest_DUNE<M>(incompressibleMooneyRivlin);

  cout << "compressible Mooney-Rivlin" << endl;
  runTest_DUNE<M>(compressibleMooneyRivlin);

  cout << "incompressible skin (Hendriks)" << endl;
  runTest_DUNE<M>(incompressibleSkin);

  cout << "compressible skin (Hendriks)" << endl;
  runTest_DUNE<M>(compressibleSkin);

  cout << "incompressible adipose (Sommer,Holzapfel et al)" << endl;
  runTest_DUNE<M>(incompressibleAdipose);

  cout << "compressible adipose (Sommer,Holzapfel et al)" << endl;
  runTest_DUNE<M>(compressibleAdipose);

  cout << "incompressible muscle (Martins et al)" << endl;
  runTest_DUNE<M>(incompressibleMuscle);

  cout << "compressible muscle (Martins et al)" << endl;
  runTest_DUNE<M>(compressibleMuscle);

  cout << "Terminating performance test with DUNE matrices.\n" << endl;
  cout << "Overall computation time: " << duration_cast<seconds>(high_resolution_clock::now() - startTime).count() << "s\n";}

#endif /* RFFGEN_PERFORMANCE_TEST_EXAMPLE_TEST_DUNE_HH */
