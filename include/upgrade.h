#pragma once
#ifndef UPGRADE_H
#define UPGRADE_H

#include "Tower.h"

class Upgrade {
public:
    enum UpgradeType { ATTACK_SPEED, DAMAGE, RANGE };

private:
    UpgradeType type;
    float value;
    unsigned int cost; 

public:
    Upgrade(UpgradeType type, float value, unsigned int cost);

    bool apply(Tower* tower);
    unsigned int getCost() const;
    UpgradeType getType() const;
    std::string getTypeName() const;
};

#endif
