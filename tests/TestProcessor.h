/* This file is part of MAUS: http://micewww.pp.rl.ac.uk:8080/projects/maus
 *
 * MAUS is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * MAUS is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with MAUS.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef NPROCESSOR_TEST_HPP_
#define NPROCESSOR_TEST_HPP_

// CppUnit
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

// Mic11
#include "src/Processor.h"

class tproc_int : public InOutProcessor<int,int> {
 public:
  tproc_int() : InOutProcessor<int,int> ("testProcInt") {}
  ~tproc_int() {}

  void init(std::string s) {}
  bool process();
  void close() {}
};

class tproc_str : public InOutProcessor<int,std::string> {
 public:
  tproc_str() : InOutProcessor("testProcStr") {}
  ~tproc_str() {}

  void init(std::string s) {}
  bool process();
  void close() {}
};

class tout_int : public OutProcessor<int> {
 public:
  tout_int() : OutProcessor<int> ("testProcInt") {}
  ~tout_int() {}

  void init(std::string s) {}
  bool process() {return true;}
  void close() {}
};

class TestNProcessor  : public CppUnit::TestFixture {

 public:
  void setUp(void);
  void tearDown(void);

 protected:
  void TestProcessSameData(void);
  void TestProcessDiffData(void);
  void TestSetInOut(void);

 private:
  tproc_int mTestObj_1;
  tproc_str mTestObj_2;
  int *inPtr, *outPtr_int;
  std::string *outPtr_str;

  CPPUNIT_TEST_SUITE( TestNProcessor );
  CPPUNIT_TEST( TestSetInOut );
  CPPUNIT_TEST( TestProcessSameData );
  CPPUNIT_TEST( TestProcessDiffData );
  CPPUNIT_TEST_SUITE_END();
};

#endif
