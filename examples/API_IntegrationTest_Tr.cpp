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

// C++
#include <sstream>
#include <thread>
#include <chrono>
#include <mutex>
extern std::mutex mtx;

// Mic11
#include "Worker.h"
#include "Macros.h"

using namespace std;
using namespace std::chrono;

/** Define 3 dummy processors.
 *  The following interface must be implemented:
 *
 *   void init(string s) override;
 *   bool process() override;
 *   void close() override;
 */


/** This processor class is a producer.
 *  The output data type is int.
 */
class t_input : public InProcessor<int> {
 public:
  t_input(): InProcessor("t_input") {}

  void init(string s) override {}
  bool process() override;
  void close() override {}
};

int init_v(0);
bool t_input::process() {

  /// Simulate some work here.
  **output_ = ++init_v + 220;
  std::this_thread::sleep_for( milliseconds(5) );

  return true;
}


/** This processor class is a consumer and producer.
 *  The input data type is int and the output type is string.
 */
class t_proc : public InOutProcessor<int, std::string> {
 public:
  t_proc() : InOutProcessor("t_proc") {}

  void init(string s) override {}
  bool process() override;
  void close() override {}
};

bool t_proc::process() {

  /// Simulate some work here.
  std::stringstream ss;
  ss << **input_ - 120;
  **output_ = ss.str();
  std::this_thread::sleep_for( milliseconds(3) );

  return true;
}


/** This processor is a consumer.
 *  The input data type is string.
 */
class t_out : public OutProcessor<std::string> {
 public:
  t_out() : OutProcessor("t_out") {}

  void init(string s) override {}
  bool process() override;
  void close() override {}
};

bool t_out::process() {

  /// Simulate some work here.
  std::unique_lock<std::mutex> lck (mtx, std::defer_lock);
  lck.lock();
  std::cout << "output: " << **input_ << std::endl;
  lck.unlock();

  std::this_thread::sleep_for( milliseconds(6) );

  return true;
}

/// For each processor, define a worker.
IMPLEMENT_INPUT(t_input_worker, int, t_input)           // worker name, data type, proce name

IMPLEMENT_TWORKER(t_proc_worker, int, std::string, t_proc)  // worker name, input data type, output data type,
                                                                // processor name

IMPLEMENT_OUTPUT(t_output_worker, std::string, t_out)            // worker name, data type, processor name

int main(int argc, char* argv[]) {

  /// Hire 4 workers.
  t_input_worker  w0(0);
  t_proc_worker   w1(1), w2(2);
  t_output_worker w3(3);

  /**Position the 4 workers.
  *
  *       |--> w1 -->|
  *       |          |
  *  w0-->|          |--> w3
  *       |          |
  *       |--> w2 -->|
  */

  /** w1 and w2 are consumers of the output of w0.
   *  In this case the data buffer is owned by w0.
   */
  w0.getOutput() >> w1 >> w2;

//   /** w3 is a consumer of the outputs of w1 and w2.
//    *  In this case the data buffer is owned by w3.
//    */
  w1 << w3.getInput();
  w2 << w3.getInput();
//   w1 << ( w2 << w3.getInput() );

  vector<thread> threads;
  threads.push_back( thread(std::ref(w0), 100) ); // Start the work. 100 iterations.
//   threads.push_back( thread(std::ref(w0)) ); // Start the work. Run forever.
  threads.push_back( thread(std::ref(w1)) );
  threads.push_back( thread(std::ref(w2)) );
  threads.push_back( thread(std::ref(w3)) );

  for (auto &t:threads)
    t.join();

  return 0;
}

