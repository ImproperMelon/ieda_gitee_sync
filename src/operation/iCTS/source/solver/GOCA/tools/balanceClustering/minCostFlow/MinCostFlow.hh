// ***************************************************************************************
// Copyright (c) 2023-2025 Peng Cheng Laboratory
// Copyright (c) 2023-2025 Institute of Computing Technology, Chinese Academy of Sciences
// Copyright (c) 2023-2025 Beijing Institute of Open Source Chip
//
// iEDA is licensed under Mulan PSL v2.
// You can use this software according to the terms and conditions of the Mulan PSL v2.
// You may obtain a copy of Mulan PSL v2 at:
// http://license.coscl.org.cn/MulanPSL2
//
// THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
// EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
// MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
//
// See the Mulan PSL v2 for more details.
// ***************************************************************************************
/**
 * @file MinCostFlow.hh
 * @author Dawn Li (dawnli619215645@gmail.com)
 */
#pragma once
// #include <lemon/list_graph.h>
// #include <lemon/maps.h>
// #include <lemon/network_simplex.h>

#include <vector>

// using namespace lemon;
namespace icts {
/**
 * @brief MinCostFlow template class for solving clustring by min cost flow
 *       input: nodes, centers
 *       constraint: max_dist, max_fanout
 *       output: cluster
 *
 * @tparam Value
 */
template <typename Value>
class MinCostFlow
{
 public:
  MinCostFlow() = default;
  ~MinCostFlow() = default;

  void add_node(const double& x, const double& y, Value& value) { _nodes.push_back({FlowPoint{x, y}, value}); }

  void add_center(const double& x, const double& y) { _centers.push_back({x, y}); }
  std::vector<std::vector<Value>> run(const double& max_dist, const size_t& max_fanout) { return {}; }
  // std::vector<std::vector<Value>> run(const double& max_dist, const size_t& max_fanout)
  // {
  //   // Define the graph
  //   // Builds src -> [sink nodes] -> [buffer nodes] - > target
  //   ListDigraph graph;

  //   // Define the node and arc types
  //   typedef ListDigraph::Node Node;
  //   typedef ListDigraph::NodeMap<std::pair<int, int>> NodeMap;
  //   typedef ListDigraph::Arc Arc;
  //   typedef ListDigraph::ArcMap<int> ArcMap;
  //   typedef ListDigraph::ArcIt ArcIt;

  //   // Add nodes to the graph
  //   Node source = graph.addNode();
  //   Node target = graph.addNode();
  //   std::vector<Node> sink_nodes, buffer_nodes;
  //   for (size_t i = 0; i < _nodes.size(); ++i) {
  //     sink_nodes.emplace_back(graph.addNode());
  //   }
  //   for (size_t i = 0; i < _centers.size(); ++i) {
  //     buffer_nodes.emplace_back(graph.addNode());
  //   }
  //   std::vector<Arc> source_sink_arcs, sink_buffer_arcs, buffer_target_arcs;
  //   for (size_t i = 0; i < _nodes.size(); ++i) {
  //     source_sink_arcs.emplace_back(graph.addArc(source, sink_nodes[i]));
  //   }
  //   std::vector<float> dist_costs;
  //   for (size_t i = 0; i < sink_nodes.size(); ++i) {
  //     for (size_t j = 0; j < buffer_nodes.size(); ++j) {
  //       auto dist = calcManhDist(_nodes[i].point, _centers[j]);
  //       if (dist <= max_dist * 2) {
  //         sink_buffer_arcs.emplace_back(graph.addArc(sink_nodes[i], buffer_nodes[j]));
  //         dist_costs.emplace_back(dist);
  //       }
  //     }
  //   }
  //   for (auto& buffer_node : buffer_nodes) {
  //     buffer_target_arcs.emplace_back(graph.addArc(buffer_node, target));
  //   }
  //   ArcMap arc_cost(graph), arc_capacity(graph);
  //   for (auto& arc : source_sink_arcs) {
  //     arc_capacity[arc] = 1;
  //   }
  //   for (size_t i = 0; i < sink_buffer_arcs.size(); ++i) {
  //     arc_capacity[sink_buffer_arcs[i]] = 1;
  //     arc_cost[sink_buffer_arcs[i]] = dist_costs[i];
  //   }
  //   for (auto& arc : buffer_target_arcs) {
  //     arc_capacity[arc] = max_fanout;
  //   }
  //   NetworkSimplex<ListDigraph, int, int> mcf(graph);
  //   mcf.costMap(arc_cost);
  //   mcf.upperMap(arc_capacity);
  //   mcf.stSupply(source, target, _nodes.size());
  //   mcf.run();
  //   ListDigraph::ArcMap<int> solution(graph);
  //   mcf.flowMap(solution);
  //   std::vector<std::vector<Value>> clusters(_centers.size());
  //   NodeMap node_map(graph);
  //   for (size_t i = 0; i < sink_nodes.size(); ++i) {
  //     node_map[sink_nodes[i]] = {i, -1};
  //   }

  //   for (size_t i = 0; i < buffer_nodes.size(); ++i) {
  //     node_map[buffer_nodes[i]] = {-1, i};
  //   }

  //   node_map[source] = {-2, -2};
  //   node_map[target] = {-2, -2};

  //   for (ArcIt it(graph); it != INVALID; ++it) {
  //     if (solution[it] == 0) {
  //       continue;
  //     }
  //     if (node_map[graph.source(it)].second == -1 && node_map[graph.target(it)].first == -1) {
  //       auto cluster_id = node_map[graph.target(it)].second;
  //       clusters[cluster_id].emplace_back(_nodes[node_map[graph.source(it)].first].value);
  //     }
  //   }
  //   return clusters;
  // }

 private:
  struct FlowPoint
  {
    double x;
    double y;
  };
  struct FlowNode
  {
    FlowPoint point;
    Value value;
  };
  static double calcManhDist(const FlowPoint& p1, const FlowPoint& p2) { return std::fabs(p1.x - p2.x) + std::fabs(p1.y - p2.y); }
  std::vector<FlowPoint> _centers;
  std::vector<FlowNode> _nodes;
};

}  // namespace icts