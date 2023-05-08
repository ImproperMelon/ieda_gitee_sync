#pragma once

#include "Net.hpp"
#include "VRPin.hpp"

namespace irt {

class VRNet
{
 public:
  VRNet() = default;
  ~VRNet() = default;
  // getter
  Net* get_origin_net() { return _origin_net; }
  irt_int get_net_idx() const { return _net_idx; }
  std::vector<VRPin>& get_vr_pin_list() { return _vr_pin_list; }
  VRPin& get_vr_driving_pin() { return _vr_driving_pin; }
  BoundingBox& get_bounding_box() { return _bounding_box; }
  MTree<RTNode>& get_dr_result_tree() { return _dr_result_tree; }
  std::map<LayerCoord, std::set<irt_int>, CmpLayerCoordByXASC>& get_key_coord_pin_map() { return _key_coord_pin_map; }
  MTree<LayerCoord>& get_coord_tree() { return _coord_tree; }
  MTree<PHYNode>& get_vr_result_tree() { return _vr_result_tree; }
  // setter
  void set_origin_net(Net* origin_net) { _origin_net = origin_net; }
  void set_net_idx(const irt_int net_idx) { _net_idx = net_idx; }
  void set_vr_pin_list(const std::vector<VRPin>& vr_pin_list) { _vr_pin_list = vr_pin_list; }
  void set_vr_driving_pin(const VRPin& vr_driving_pin) { _vr_driving_pin = vr_driving_pin; };
  void set_bounding_box(const BoundingBox& bounding_box) { _bounding_box = bounding_box; }
  void set_dr_result_tree(const MTree<RTNode>& dr_result_tree) { _dr_result_tree = dr_result_tree; }
  void set_key_coord_pin_map(const std::map<LayerCoord, std::set<irt_int>, CmpLayerCoordByXASC>& key_coord_pin_map)
  {
    _key_coord_pin_map = key_coord_pin_map;
  }
  void set_coord_tree(const MTree<LayerCoord>& coord_tree) { _coord_tree = coord_tree; }
  void set_vr_result_tree(const MTree<PHYNode>& vr_result_tree) { _vr_result_tree = vr_result_tree; }

 private:
  Net* _origin_net = nullptr;
  irt_int _net_idx = -1;
  std::vector<VRPin> _vr_pin_list;
  VRPin _vr_driving_pin;
  BoundingBox _bounding_box;
  MTree<RTNode> _dr_result_tree;
  std::map<LayerCoord, std::set<irt_int>, CmpLayerCoordByXASC> _key_coord_pin_map;
  MTree<LayerCoord> _coord_tree;
  MTree<PHYNode> _vr_result_tree;
};

}  // namespace irt
