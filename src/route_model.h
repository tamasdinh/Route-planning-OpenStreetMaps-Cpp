#pragma once

#include <limits>
#include <cmath>
#include <unordered_map>
#include "model.h"
#include <iostream>

class RouteModel : public Model {

  public:
    class Node : public Model::Node {
      public:
        Node(){}
        Node(int idx, RouteModel * search_model, Model::Node node) : Model::Node(node), parent_model(search_model), index(idx) {}

        Node *parent = nullptr;
        float h_value = std::numeric_limits<float>::max();
        float g_value = 0.0f;
        bool visited = false;
        std::vector<Node*> neighbors;

        float Distance(Node other) const {
            return std::sqrt(std::pow(this->x - other.x, 2) + std::pow(this->y - other.y, 2));
        }

        void FindNeighbors();

      private:
        int index;
        RouteModel *parent_model = nullptr;

        Node* FindNeighbor(std::vector<int> node_indices);

    };

    RouteModel(const std::vector<std::byte> &xml);
    std::vector<Node> path; // This variable will eventually store the path that is found by the A* search.
    auto &SNodes() {return m_Nodes;}

    auto &getNodeToRoadMap() {return node_to_road;};

    Node &FindClosestNode(float x, float y);


  private:
    std::vector<Node> m_Nodes;

    std::unordered_map<int, std::vector<const Model::Road*>> node_to_road;
    void CreateNodeToRoadHashmap();

};
