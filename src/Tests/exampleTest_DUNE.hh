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

#include <iostream>

#include <boost/timer/timer.hpp>

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
  auto nUpdates = 10u * 1000u * 1000u;
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
  boost::timer::cpu_timer timer;
  for(auto i=0u; i<nUpdates; ++i) f.update(a);
  cout << "updates: " << boost::timer::format(timer.elapsed());
  timer.stop();
  cout << "number of evaluations: " << nEval << endl;
  timer.start();
  for(auto i=0u; i<nEval; ++i) val = f();
  cout << "d0: " << boost::timer::format(timer.elapsed());
  cout << "value: " << val << endl;
  timer.stop();
  timer.start();
  for(auto i=0u; i<nEval; ++i) val = f.template d1<0>(da0);
  cout << "d1: " << boost::timer::format(timer.elapsed());
  cout << "value: " << val << endl;
  timer.stop();
  timer.start();
  for(auto i=0u; i<nEval; ++i) val = f.template d2<0,0>(da0,da1);
  cout << "d2: " << boost::timer::format(timer.elapsed());
  cout << "value: " << val << endl;
  timer.stop();
  timer.start();
  for(auto i=0u; i<nEval; ++i) val = f.template d3<0,0,0>(da0,da1,da2);
  cout << "d3: " << boost::timer::format(timer.elapsed());
  cout << "value: " << val << endl;
  cout << endl;
}

void testExamples_DUNE()
{
  using std::cout;
  using std::endl;
  using RFFGen::LN;
  using RFFGen::Pow;
  using RFFGen::Invariant;
  constexpr int dim = 3;
  using M = Dune::FieldMatrix<double,dim,dim>;

  cout << "Starting performance test with DUNE matrices.\n" << endl;

  auto c0 = 1.;
  auto c1 = 1.;
  auto d0 = 1.;
  auto d1 = 1.;
  M fiberTensor = RFFGen::zero<M>(), I = RFFGen::unitMatrix<M>();
  fiberTensor[0][0] = 1.;

  auto incompressibleNeoHooke = RFFGen::incompressibleNeoHooke<M>(c0);
  auto incompressibleModifiedNeoHooke = RFFGen::modifiedIncompressibleNeoHooke<M>(c0);
  auto compressibleNeoHooke = RFFGen::compressibleNeoHooke<Pow<2>,LN,M>(c0,d0,d1);
  auto compressibleModifiedNeoHooke = RFFGen::modifiedCompressibleNeoHooke<Pow<2>,LN,M>(c0,d0,d1);

  auto incompressibleMooneyRivlin = RFFGen::incompressibleMooneyRivlin<M>(c0,c1);
  auto compressibleMooneyRivlin = RFFGen::compressibleMooneyRivlin<M,Pow<2>,LN>(c0,c1,d0,d1);

  auto incompressibleSkin = RFFGen::incompressibleSkin_Hendriks<M>();
  auto compressibleSkin = RFFGen::compressibleSkin_Hendriks<Pow<2>,LN,M>(d0,d1);

  auto incompressibleAdipose = RFFGen::incompressibleAdiposeTissue_SommerHolzapfel(fiberTensor);
  auto compressibleAdipose = RFFGen::compressibleAdiposeTissue_SommerHolzapfel<Pow<2>,LN>(d0,d1,fiberTensor);

  auto incompressibleMuscle = RFFGen::incompressibleMuscleTissue_Martins(fiberTensor);
  auto compressibleMuscle = RFFGen::compressibleMuscleTissue_Martins<Pow<2>,LN>(d0,d1,fiberTensor);

  boost::timer::cpu_timer timer;
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
  cout << "Overall computation time: " << boost::timer::format(timer.elapsed());
}

#endif /* RFFGEN_PERFORMANCE_TEST_EXAMPLE_TEST_DUNE_HH */
