/**
 * @file NetList.h
 * @author simin tao (taosm@pcl.ac.cn)
 * @brief The netlist class.
 * @version 0.1
 * @date 2021-02-04
 */
#pragma once

#include <list>
#include <utility>
#include <vector>

#include "Config.hh"
#include "DisallowCopyAssign.hh"
#include "HashMap.hh"
#include "Instance.hh"
#include "Net.hh"
#include "Pin.hh"
#include "Port.hh"
#include "Vector.hh"
#include "string/StrMap.hh"

namespace ista {

class PortIterator;
class PortBusIterator;
class InstanceIterator;
class NetIterator;

/**
 * @brief The netlist class for design.
 *
 */
class Netlist : public DesignObject {
 public:
  Netlist() : DesignObject("top"){};
  ~Netlist() override = default;

  friend PortIterator;
  friend PortBusIterator;
  friend InstanceIterator;
  friend NetIterator;

  unsigned isNetlist() override { return 1; }

  Port& addPort(Port&& port) {
    _ports.emplace_back(std::move(port));
    Port* the_port = &(_ports.back());
    _str2port.insert(the_port->get_name(), the_port);
    return *the_port;
  }

  Port* findPort(const char* port_name) const {
    auto found_port = _str2port.find(port_name);

    if (found_port != _str2port.end()) {
      return found_port->second;
    }
    return nullptr;
  }

  std::vector<DesignObject*> findPort(const char* pattern, bool regexp,
                                      bool nocase);

  std::vector<DesignObject*> findPin(const char* pattern, bool regexp,
                                     bool nocase);

  std::vector<DesignObject*> findObj(const char* pattern, bool regexp,
                                     bool nocase);

  PortBus& addPortBus(PortBus&& port_bus) {
    _port_buses.emplace_back(std::move(port_bus));
    auto* the_port_bus = &(_port_buses.back());
    _str2portbus.insert(the_port_bus->get_name(), the_port_bus);
    return *the_port_bus;
  }
  auto& get_port_buses() { return _port_buses; }

  PortBus* findPortBus(const char* port_bus_name) const {
    auto found_port_bus = _str2portbus.find(port_bus_name);

    if (found_port_bus != _str2portbus.end()) {
      return found_port_bus->second;
    }
    return nullptr;
  }

  Net& addNet(Net&& net) {
    _nets.emplace_back(std::move(net));
    Net* the_net = &(_nets.back());
    const char* net_name = the_net->get_name();
    _str2net.insert(net_name, the_net);
    return *the_net;
  }

  void removeNet(Net* net) {
    const char* net_name = net->get_name();
    _str2net.erase(net_name);

    auto it = std::find_if(_nets.begin(), _nets.end(),
                           [net](auto& the_net) { return net == &the_net; });
    LOG_FATAL_IF(it == _nets.end());
    _nets.erase(it);
  }

  Net* findNet(const char* net_name) const {
    auto found_net = _str2net.find(net_name);

    if (found_net != _str2net.end()) {
      return found_net->second;
    }
    return nullptr;
  }

  Instance& addInstance(Instance&& instance) {
    _instances.emplace_back(std::move(instance));

    Instance* the_instance = &(_instances.back());
    const char* instance_name = the_instance->get_name();
    _str2instance.insert(instance_name, the_instance);

    return *the_instance;
  }

  void removeInstance(const char* instance_name) {
    auto found_instance = _str2instance.find(instance_name);
    LOG_FATAL_IF(found_instance == _str2instance.end());
    auto* the_instance = found_instance->second;

    auto it = std::find_if(
        _instances.begin(), _instances.end(),
        [the_instance](auto& instance) { return the_instance == &instance; });
    _str2instance.erase(found_instance);
    _instances.erase(it);
  }

  Instance* findInstance(const char* instance_name) const {
    auto found_instance = _str2instance.find(instance_name);

    if (found_instance != _str2instance.end()) {
      return found_instance->second;
    }
    return nullptr;
  }

  std::size_t getInstanceNum() { return _instances.size(); }
  std::size_t getNetNum() { return _nets.size(); }

  void reset();

  void writeVerilog(const char* verilog_file_name,
                    std::set<std::string> exclude_cell_names);

 private:
  std::list<Port> _ports;
  StrMap<Port*> _str2port;  //!< The port name to port for search.
  std::list<PortBus> _port_buses;
  StrMap<PortBus*> _str2portbus;

  std::list<Net> _nets;
  StrMap<Net*> _str2net;  //!< The net name to net for search.
  std::list<Instance> _instances;
  StrMap<Instance*> _str2instance;

  DISALLOW_COPY_AND_ASSIGN(Netlist);
};

/**
 * @brief Port Iterator of netlist, that provide Java style access.
 *
 */
class PortIterator {
 public:
  explicit PortIterator(Netlist* nl);
  ~PortIterator() = default;

  bool hasNext();
  Port& next();

 private:
  Netlist* _nl;
  std::list<Port>::iterator _iter;

  DISALLOW_COPY_AND_ASSIGN(PortIterator);
};

/**
 * @brief usage:
 * Netlist* nl;
 * Port* port;
 * FOREACH_PORT(nl, port)
 * {
 *    do_something_for_port();
 * }
 *
 */
#define FOREACH_PORT(nl, port) \
  for (PortIterator iter(nl);  \
       iter.hasNext() ? port = &(iter.next()), true : false;)

/**
 * @brief Port bus Iterator of netlist, that provide Java style access.
 *
 */
class PortBusIterator {
 public:
  explicit PortBusIterator(Netlist* nl);
  ~PortBusIterator() = default;

  bool hasNext();
  PortBus& next();

 private:
  Netlist* _nl;
  std::list<PortBus>::iterator _iter;

  DISALLOW_COPY_AND_ASSIGN(PortBusIterator);
};

/**
 * @brief usage:
 * Netlist* nl;
 * PortBUS* port_bus;
 * FOREACH_PORT_BUS(nl, port_bus)
 * {
 *    do_something_for_port_bus();
 * }
 *
 */
#define FOREACH_PORT_BUS(nl, port_bus) \
  for (PortBusIterator iter(nl);       \
       iter.hasNext() ? port_bus = &(iter.next()), true : false;)

/**
 * @brief Instance Iterator of netlist, that provide Java style access.
 *
 */
class InstanceIterator {
 public:
  explicit InstanceIterator(Netlist* nl);
  ~InstanceIterator() = default;

  bool hasNext();
  Instance& next();

 private:
  Netlist* _nl;
  std::list<Instance>::iterator _iter;

  DISALLOW_COPY_AND_ASSIGN(InstanceIterator);
};

/**
 * @brief usage:
 * Netlist* nl;
 * Instance* inst;
 * FOREACH_PORT(nl, inst)
 * {
 *    do_something_for_inst();
 * }
 *
 */
#define FOREACH_INSTANCE(nl, inst) \
  for (InstanceIterator iter(nl);  \
       iter.hasNext() ? inst = &(iter.next()), true : false;)

/**
 * @brief
 *
 */
class NetIterator {
 public:
  explicit NetIterator(Netlist* nl);
  ~NetIterator() = default;

  bool hasNext();
  Net& next();

 private:
  Netlist* _nl;
  std::list<Net>::iterator _iter;

  DISALLOW_COPY_AND_ASSIGN(NetIterator);
};

/**
 * @brief usage:
 * Netlist* nl;
 * Net* net;
 * FOREACH_PORT(nl, net)
 * {
 *    do_something_for_net();
 * }
 *
 */
#define FOREACH_NET(nl, net)       \
  for (ista::NetIterator iter(nl); \
       iter.hasNext() ? net = &(iter.next()), true : false;)

}  // namespace ista
