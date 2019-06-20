#include "route_model.h"
#include <iostream>

RouteModel::RouteModel(const std::vector<std::byte> &xml) : Model(xml) {
    int counter = 1;
    for (Model::Node node : this->Nodes()) {
        m_Nodes.push_back(Node(counter, this, node));
        counter++;
    }
    CreateNodeToRoadHashmap();
}

void RouteModel::CreateNodeToRoadHashmap() {
    for (const Model::Road &road : Roads()) {
        if (road.type != Model::Road::Footway) {
            for (int node_idx : Ways()[road.way].nodes) {
                if (node_to_road.find(node_idx) == node_to_road.end())
                    node_to_road[node_idx] = std::vector<const Model::Road*> {};
                node_to_road[node_idx].push_back(&road);
            }
        }
    }
}

RouteModel::Node* RouteModel::Node::FindNeighbor(std::vector<int> node_indices) {
    Node *closest_node = nullptr;
    Node node;

    for (int index : node_indices) {
        node = parent_model->SNodes()[index];
        if (!node.visited && this->Distance(node) > 0) {
            if (closest_node == nullptr || this->Distance(node) < this->Distance(*closest_node))
                closest_node = &parent_model->SNodes()[index];
        }
    }
    return closest_node;
}

void RouteModel::Node::FindNeighbors() {
    for (auto &road : parent_model->node_to_road[this->index]) {
        Node* neighbor = this->FindNeighbor(parent_model->Ways()[road->way].nodes);
        if (neighbor != nullptr)
            this->neighbors.emplace_back(*neighbor);
    }

}

RouteModel::Node &RouteModel::FindClosestNode(float x, float y) {
    Node temp;
    temp.x = x;
    temp.y = y;

    float min_dist = std::numeric_limits<float>::max();
    float dist;
    int closest_idx;

    for (const Model::Road &road : Roads()) {
        if (road.type != Model::Road::Type::Footway) {
            for (int index : Ways()[road.way].nodes) {
                dist = temp.Distance(SNodes()[index]);
                if (dist < min_dist) {
                    closest_idx = index;
                    min_dist = dist;
                }
            }
        }
    }
    return SNodes()[closest_idx];
}

