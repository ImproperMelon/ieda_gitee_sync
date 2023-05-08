/**
 * @file StaClock.h
 * @author simin tao (taosm@pcl.ac.cn)
 * @brief The class of sta clock.
 * @version 0.1
 * @date 2021-02-17
 *
 * @copyright Copyright (c) 2021
 *
 */
#pragma once

#include <utility>

#include "DisallowCopyAssign.hh"
#include "Set.hh"
#include "StaVertex.hh"
#include "Vector.hh"

namespace ista {

/**
 * @brief The class of clock waveform.
 *
 */
class StaWaveForm {
 public:
  StaWaveForm() = default;
  ~StaWaveForm() = default;

  StaWaveForm(StaWaveForm&& other) = default;
  StaWaveForm& operator=(StaWaveForm&& rhs) = default;

  void addWaveEdge(int wave_point) { _wave_edges.push_back(wave_point); }
  Vector<int>& get_wave_edges() { return _wave_edges; }

  int getRisingEdge() { return _wave_edges[0]; }
  int getFallingEdge() { return _wave_edges[1]; }

 private:
  Vector<int> _wave_edges;  //!< We assume that the edges compose of rising and
                            //!< falling edge pair.

  DISALLOW_COPY_AND_ASSIGN(StaWaveForm);
};

/**
 * @brief The clock for sta, which is convert from sdc clock.
 *
 */
class StaClock {
 public:
  enum class ClockType { kIdeal, kPropagated };

  StaClock(const char* clock_name, ClockType clock_type, int period);
  ~StaClock();

  StaClock(StaClock&& other);
  StaClock& operator=(StaClock&& rhs);

  void addVertex(StaVertex* the_vertex) { _clock_vertexes.insert(the_vertex); }
  Set<StaVertex*>& get_clock_vertexes() { return _clock_vertexes; }

  const char* get_clock_name() { return _clock_name; }

  void set_wave_form(StaWaveForm&& wave_form) {
    _wave_form = std::move(wave_form);
  }

  StaWaveForm& get_wave_form() { return _wave_form; }

  auto get_clock_type() { return _clock_type; }
  void set_clock_type(ClockType clock_type) { _clock_type = clock_type; }
  void setPropagateClock() { _clock_type = ClockType::kPropagated; }

  int getRisingEdge() { return _wave_form.getRisingEdge(); }
  int getFallingEdge() { return _wave_form.getFallingEdge(); }

  [[nodiscard]] int get_period() const { return _period; }
  double getPeriodNs() const { return PS_TO_NS(_period); }

  [[nodiscard]] bool isIdealClockNetwork() const {
    return _clock_type == ClockType::kIdeal;
  }
  void set_ideal_clock_network_latency(double ideal_network_latency) {
    _ideal_network_latency = NS_TO_FS(ideal_network_latency);
  }

  auto& get_ideal_network_latency() { return _ideal_network_latency; }

  bool isSyncClockGroup(StaClock* other_clock) const { return false; }

  unsigned exec(StaFunc& func);

 private:
  const char* _clock_name;
  Set<StaVertex*> _clock_vertexes;  //!< The graph vertex which is clock point.
  ClockType _clock_type;

  std::optional<int> _ideal_network_latency;  //!< The clock network latency
                                              //!< is ideal, unit is ps.

  int _period;  // unit is ps.
  StaWaveForm _wave_form;

  DISALLOW_COPY_AND_ASSIGN(StaClock);
};

}  // namespace ista
