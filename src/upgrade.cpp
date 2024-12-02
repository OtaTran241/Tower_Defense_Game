#include "Upgrade.h"

Upgrade::Upgrade(UpgradeType type, float value, unsigned int cost)
    : type(type), value(value), cost(cost) {}

bool Upgrade::apply(Tower* tower) {
    bool upgradeApplied = false;

    switch (type) {
    case ATTACK_SPEED: {
        int UpgradeCount = tower->getUpgradeCount();
        if (UpgradeCount <= 0) {
            return false; 
        }

        tower->setShootCooldown(tower->getShootCooldown() - value);
        tower->setUpgradeCount(--UpgradeCount);
        upgradeApplied = true; 
        break;
    }
    case DAMAGE: {
        int UpgradeCount = tower->getUpgradeCount();
        if (UpgradeCount <= 0) {
            return false;
        }

        tower->setDamage(tower->getDamage() + value);
        tower->setUpgradeCount(--UpgradeCount);
        upgradeApplied = true; 
        break;
    }
    case RANGE: {
        int UpgradeCount = tower->getUpgradeCount();
        if (UpgradeCount <= 0) {
            return false;
        }

        tower->setRange(tower->getRange() + value);
        tower->setUpgradeCount(--UpgradeCount);
        upgradeApplied = true; 
        break;
    }
    default:
        break;
    }

    return upgradeApplied; 
}

unsigned int Upgrade::getCost() const {
    return cost;
}

Upgrade::UpgradeType Upgrade::getType() const {
    return type;
}

std::string Upgrade::getTypeName() const {
    switch (type) {
    case ATTACK_SPEED: return "Attack Speed";
    case DAMAGE: return "Damage";
    case RANGE: return "Range";
    default: return "Unknown";
    }
}
