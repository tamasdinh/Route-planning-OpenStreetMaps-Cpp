#pragma once

#include <iostream>
#include <vector>
#include <string>
#include "route_model.h"


class RoutePlanner {
  public:
    RoutePlanner(RouteModel &model, float start_x, float start_y, float end_x, float end_y);

    float GetDistance() const {return distance;}

    void AStarSearch();

  private:
    RouteModel &m_Model;

    RouteModel::Node *start_node, *end_node;
    float distance;

    std::vector<RouteModel::Node> ConstructFinalPath(RouteModel::Node *);

    float CalculateHValue(const RouteModel::Node*);

    std::vector<RouteModel::Node*> open_list;
    RouteModel::Node* NextNode();

    void AddNeighbors(RouteModel::Node*);

};
