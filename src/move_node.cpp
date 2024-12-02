#include "Move_Node.h"

MoveNode::MoveNode(float x, float y) : position(x, y) {}

sf::Vector2f MoveNode::getPosition() const {
    return position;
}
