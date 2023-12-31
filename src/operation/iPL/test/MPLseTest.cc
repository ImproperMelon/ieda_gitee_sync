#include <omp.h>

#include <Eigen/Dense>
#include <Eigen/Sparse>
#include <algorithm>
#include <chrono>
#include <iostream>
#include <random>
#include <set>

#include "analytical_method/LSEWirelength.hh"
#include "analytical_method/MProblem.hh"
#include "analytical_method/Solver.hh"
#include "utility/Geometry.hh"
#include "utility/Image.hh"
int main()
{
  int num_v = 10000;
  int num_e = 10000;
  int netdegree = 3;
  int core_w = 10000;
  int core_h = 80000;
  ipl::MProblem mp;
  mp.setRandom(num_v, num_e, netdegree, core_w, core_h, 0.5);
  Vec x = (Vec::Random(num_v) + Vec::Ones(num_v)) * (core_w / 2);
  Vec y = (Vec::Random(num_v) + Vec::Ones(num_v)) * (core_h / 2);
  Vec r = (Vec::Random(num_v) + Vec::Ones(num_v)) * M_PI;
  // Vec r = (Vec::Random(num_v) + Vec::Ones(num_v)) * 0;
  x.setConstant(core_w / 2);
  y.setConstant(core_h / 2);
  // for (size_t i = 0; i < r.rows(); i++) {
  //   r(i) = r(i) + (i % 4) * M_PI_2;
  // }

  Mat var(num_v, 3);
  var.col(0) = x;
  var.col(1) = y;
  var.col(2) = r;
  ipl::Option opt;
  opt.max_iter = 200;
  opt.hasBoundConstraint = true;
  opt.threads = 32;
  ipl::Solver::solve(mp, var, opt);
  // std::cout << var;
}