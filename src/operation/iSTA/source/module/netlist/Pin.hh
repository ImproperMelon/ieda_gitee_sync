/**
 * @file Pin.h
 * @author simin tao (taosm@pcl.ac.cn)
 * @brief The class for pin in the netlist.
 * @version 0.1
 * @date 2021-02-03
 */

#pragma once
#include <map>
#include <memory>
#include <string>
#include <utility>

#include "DesignObject.hh"
#include "DisallowCopyAssign.hh"
#include "Type.hh"
#include "log/Log.hh"

namespace ista {
class Net;
class Instance;
class LibertyPort;
class PinBus;

/**
 * @brief The class for pin of design.
 *
 */
class Pin : public DesignObject {
 public:
  using mode_trans = std::pair<AnalysisMode, TransType>;

  explicit Pin(const char* name, LibertyPort* cell_port);
  Pin(Pin&& other) noexcept;
  Pin& operator=(Pin&& rhs) noexcept;
  ~Pin() override = default;

  unsigned isPin() override { return 1; }
  unsigned isPort() override { return 0; }
  unsigned isInput() override;
  unsigned isOutput() override;
  unsigned isInout() override;
  unsigned isConst() override { return _is_VDD || _is_GND; }

  double cap() override;
  double cap(AnalysisMode mode, TransType trans_type) override;

  Net* get_net() override { return _net; }
  void set_net(Net* net) override { _net = net; }

  void set_own_instance(Instance* own_instance) {
    _own_instance = own_instance;
  }

  Instance* get_own_instance() override { return _own_instance; }

  void set_cell_port(LibertyPort* cell_port) { _cell_port = cell_port; }
  LibertyPort* get_cell_port() { return _cell_port; }

  void set_pin_bus(PinBus* pin_bus) { _pin_bus = pin_bus; }
  auto* get_pin_bus() { return _pin_bus; }

  std::string getFullName() override;

 private:
  Net* _net = nullptr;                //!< The pin connected net.
  LibertyPort* _cell_port = nullptr;  //!< The pin corresponding to cell port.
  Instance* _own_instance = nullptr;  //!< The pin owned by the instance.
  PinBus* _pin_bus = nullptr;         //!< The pin owned by the pin bus.

  unsigned _is_VDD : 1;  //!< The pin is at a constant logic value 1.
  unsigned _is_GND : 1;  //!< The pin is at a constant logic value 0.
  unsigned _reserverd : 30;

  DISALLOW_COPY_AND_ASSIGN(Pin);
};

/**
 * @brief The class of pin bus
 *
 */
class PinBus : public DesignObject {
 public:
  PinBus(const char* name, unsigned left, unsigned right, unsigned size);
  PinBus(PinBus&& other) noexcept = default;
  PinBus& operator=(PinBus&& rhs) noexcept = default;
  ~PinBus() override = default;

  unsigned isPinBus() override { return 1; }

  [[nodiscard]] unsigned get_left() const { return _left; }
  [[nodiscard]] unsigned get_right() const { return _right; }
  [[nodiscard]] unsigned get_size() const { return _size; }

  void addPin(unsigned index, Pin* pin) {
    LOG_FATAL_IF(index >= _size) << "beyond bus size";
    _pins[index] = pin;
    pin->set_pin_bus(this);
  }
  Pin* getPin(unsigned index) { return _pins[index]; }
  auto& getPins() { return _pins; }

 private:
  unsigned _left;   //!< The left range.
  unsigned _right;  //!< The right range.

  std::unique_ptr<Pin*[]> _pins;  //!< The pins.
  unsigned _size;                 //!< The pin bus size.
  DISALLOW_COPY_AND_ASSIGN(PinBus);
};

}  // namespace ista
