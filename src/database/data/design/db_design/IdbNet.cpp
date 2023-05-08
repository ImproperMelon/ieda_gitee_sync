/**
 * iEDA
 * Copyright (C) 2021  PCL
 *
 * This program is free software;
 *
 */

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * @project		iDB
 * @file		IdbNet.h
 * @copyright	(c) 2021 All Rights Reserved.
 * @date		25/05/2021
 * @version		0.1
 * @description


        Defines netlist connectivity and regular-routes for nets containing regular pins
 *
 */

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "IdbNet.h"

#include <algorithm>

#include "IdbInstance.h"

namespace idb {

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

IdbNet::IdbNet()
{
  _net_name = "";
  _xtalk = 0;
  _connect_type = IdbConnectType::kNone;
  _source_type = IdbInstanceType::kNone;
  _weight = 0;

  _fix_bump = false;
  _frequency = -1;

  _io_pin = nullptr;
  // _io_pin_list = new IdbPins();
  _instance_pin_list = new IdbPins();
  _instance_list = new IdbInstanceList();

  _wire_list = new IdbRegularWireList();
}

IdbNet::~IdbNet()
{
  if (_instance_pin_list != nullptr) {
    delete _instance_pin_list;
    _instance_pin_list = nullptr;
  }

  if (_instance_list != nullptr) {
    delete _instance_list;
    _instance_list = nullptr;
  }

  if (_wire_list != nullptr) {
    delete _wire_list;
    _wire_list = nullptr;
  }
}

void IdbNet::clear_wire_list()
{
  _wire_list->clear();
}

void IdbNet::set_connect_type(string type)
{
  set_connect_type(IdbEnum::GetInstance()->get_connect_property()->get_type(type));
}

void IdbNet::set_source_type(string type)
{
  _source_type = IdbEnum::GetInstance()->get_instance_property()->get_type(type);
}

// If pin's direction is OUTPUT, the pin is a driven pin
// If pin's direction is INOUT(Generally IO Pin), the driven pin depends on the first connected pin's direction
// case 1 : pin1[OUTPUT], Pin2[INPUT], pin3[INPUT]... : driven pin = pin1

// to be discuss
// case 2 : pin1[INOUT], pin2[OUT]                    : driven pin = pin2 ?? Not sure if the situation is exist???
// case 3 : pin1[INOUT], {pin2[INOUT]<--Instance-->pin3[OUTPUT]}, pin4[INPUT]...  : driven pin = pin1
//          ------------>---------------------------------------->--------------
// case 4 : pin1[INOUT], {pin2[INOUT]<--Instance-->pin3[INPUT]}, pin4[OUTPUT]...  : driven pin = pin2
//          ------------<----------------------------------------<--------------
//          both pin1 & pin2 are INOUT direction, the direction depends on direction of pin3 which is in the same
//          instance
// IdbPin* IdbNet::findDrivingPin()
IdbPin* IdbNet::get_driving_pin()
{
  if (_io_pin == nullptr) {
    // case 1
    for (IdbPin* pin : _instance_pin_list->get_pin_list()) {
      if (pin->get_term()->get_direction() == IdbConnectDirection::kOutput) {
        return pin;
      }
    }
    std::cout << "No driving pin in net : " << _net_name << std::endl;
  } else  // If IO PIN exist， there are only a pair pins in a net (including 1 IO Pin and 1 instance pin)
  {
    // case 2
    if (_io_pin->get_term()->get_direction() == IdbConnectDirection::kInput) {
      return _io_pin;
    }
    if (_instance_pin_list->get_pin_list().size() > 0) {
      IdbPin* instance_pin = _instance_pin_list->get_pin_list().at(0);
      if (instance_pin->get_term()->get_direction() == IdbConnectDirection::kOutput) {
        return instance_pin;
      }
    }

    //     // case 3 or case 4
    // IdbInstance* instance      = instance_pin->get_instance();
    // IdbPins* instance_pin_list = instance->get_pin_list();
    // for (IdbPin* pin : instance_pin_list->get_pin_list()) {
    //   if (pin->get_pin_name() != instance_pin->get_pin_name()) {
    //     return pin->get_term()->get_direction() == IdbConnectDirection::kOutput ? _io_pin : instance_pin;
    //   }
    //   }
  }

  // std::cout << "Error : No driven pin exist..." << std::endl;
  return nullptr;
}

vector<IdbPin*> IdbNet::get_load_pins()
{
  vector<IdbPin*> pin_list;
  if (_io_pin == nullptr) {
    // case 1
    for (IdbPin* pin : _instance_pin_list->get_pin_list()) {
      if (pin->get_term()->get_direction() != IdbConnectDirection::kOutput) {
        pin_list.emplace_back(pin);
      }
    }
  } else  // If IO PIN exist， there are only a pair pins in a net (including 1 IO Pin and 1 instance pin)
  {
    // case 2
    if (_io_pin->get_term()->get_direction() != IdbConnectDirection::kInput) {
      pin_list.emplace_back(_io_pin);
    }

    if (_instance_pin_list->get_pin_list().size() > 0) {
      // IdbPin* instance_pin = _instance_pin_list->get_pin_list().at(0);
      // if (instance_pin->get_term()->get_direction() != IdbConnectDirection::kOutput) {
      //   pin_list.emplace_back(instance_pin);
      // }

      for (IdbPin* pin : _instance_pin_list->get_pin_list()) {
        if (pin->get_term()->get_direction() != IdbConnectDirection::kOutput) {
          pin_list.emplace_back(pin);
        }
      }
    }
  }
  return pin_list;
}

bool IdbNet::set_bounding_box()
{
  // IdbRect* rect = get_bounding_box();

  // int32_t ll_x = _average_coordinate->get_x() - _io_term->get_bounding_box()->get_width()/2;
  // int32_t ll_y = _average_coordinate->get_y() - _io_term->get_bounding_box()->get_height()/2;
  // int32_t ur_x = _average_coordinate->get_x() + _io_term->get_bounding_box()->get_width()/2;
  // int32_t ur_y = _average_coordinate->get_y() + _io_term->get_bounding_box()->get_height()/2;
  //  rect->set_rect(ll_x, ll_y, ur_x, ur_y);
  return false;
}

void IdbNet::remove_pin(IdbPin* pin)
{
  if (_io_pin == pin) {
    _io_pin->remove_net();
    _io_pin = nullptr;
  } else {
    _instance_pin_list->remove_pin(pin);
  }
}

bool IdbNet::checkConnection()
{
  bool b_result = false;

  // /// build pin
  // for (IdbPin* pin : _instance_pin_list->get_pin_list()) {
  // }

  // /// build wire
  // for (IdbRegularWire* wire : _wire_list->get_wire_list()) {
  //   for (IdbRegularWireSegment* segment : wire->get_segment_list()) {
  //   }
  // }

  if (!b_result) {
    std::cout << "[IdbNetList Error] Net connected failed. Net name = " << get_net_name() << std::endl;
  }

  return b_result;
}

int32_t IdbNet::wireLength()
{
  return _wire_list->wireLength();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
IdbNetList::IdbNetList()
{
  _num = 0;
}

IdbNetList::~IdbNetList()
{
  for (auto& net : _net_list) {
    if (net != nullptr) {
      delete net;
      net = nullptr;
    }
  }

  _net_list.clear();
  _num = 0;
}

IdbNet* IdbNetList::find_net(string name)
{
  //   for (IdbNet* net : _net_list) {
  //     if (net->get_net_name() == name) {
  //       return net;
  //     }
  //   }

  //   return nullptr;
  auto net_pair = _net_map.find(name);
  if (net_pair != _net_map.end()) {
    return net_pair->second;
  }

  return nullptr;
}

IdbNet* IdbNetList::find_net(size_t index)
{
  if (_num > index) {
    return _net_list.at(index);
  }

  return nullptr;
}

IdbNet* IdbNetList::add_net(IdbNet* net)
{
  IdbNet* pNet = net;
  if (pNet == nullptr) {
    pNet = new IdbNet();
  }
  _net_list.emplace_back(pNet);
  _net_map.insert(make_pair(pNet->get_net_name(), pNet));
  _num++;

  return pNet;
}

IdbNet* IdbNetList::add_net(string name, IdbConnectType type)
{
  IdbNet* pNet = new IdbNet();
  pNet->set_net_name(name);
  pNet->set_connect_type(type);
  _net_map.insert(make_pair(name, pNet));
  _net_list.emplace_back(pNet);
  _num++;

  return pNet;
}

/**
 * @Brief : remove net in net list
 * @param  name
 * @return true
 * @return false
 */
bool IdbNetList::remove_net(string name)
{
  /// remove net from net map
  auto it_map = _net_map.find(name);
  if (it_map != _net_map.end()) {
    it_map = _net_map.erase(it_map);
  }

  /// remove net from netlist
  auto it = std::find_if(_net_list.begin(), _net_list.end(), [name](auto net) { return name == net->get_net_name(); });
  if (it == _net_list.end()) {
    return false;
  }

  /// delete net & release resource
  delete *it;
  *it = nullptr;
  _net_list.erase(it);
  _num--;

  return true;
}

/**
 * @Brief : remove all the wire in the net
 */
void IdbNetList::clear_wire_list()
{
  for (IdbNet* net : _net_list) {
    net->clear_wire_list();
  }
}

/**
 * @Brief : check the connection for all the nets
 * @return true
 * @return false
 */
bool IdbNetList::checkConnection()
{
  bool b_result = false;
  for (IdbNet* net : _net_list) {
    if (!net->checkConnection()) {
    }
  }

  return b_result;
}

uint64_t IdbNetList::maxFanout()
{
  uint64_t net_len = 0;
  for (auto net : _net_list) {
    net_len += net->wireLength();
  }
  return net_len;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool IdbNetChecker::checkNetConnection(IdbNet* net)
{
  bool b_result = false;

  // /// build pin
  // for (IdbPin* pin : net->get_instance_pin_list()->get_pin_list()) {
  // }

  // /// build wire
  // for (IdbRegularWire* wire : net->get_wire_list()->get_wire_list()) {
  //   for (IdbRegularWireSegment* segment : wire->get_segment_list()) {
  //   }
  // }

  if (!b_result) {
    std::cout << "[IdbNetList Error] Net connected failed. Net name = " << net->get_net_name() << std::endl;
  }

  return b_result;
}

}  // namespace idb
