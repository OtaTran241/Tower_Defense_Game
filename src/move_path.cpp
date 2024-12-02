#include "move_path.h"

MovePath::MovePath(float width, sf::Color color) : width(width), color(color) {}

void MovePath::addNode(const MoveNode& node) {
    movePath.push_back(node);
    createSegments();
}

const std::vector<MoveNode>& MovePath::getMovePath() const {
    return movePath;
}

const std::vector<sf::Vector2f>& MovePath::getSegmentBounds() const {
    return segmentBounds;
}

void MovePath::clearMovePath() {
    for (auto node = movePath.begin(); node != movePath.end();) {
        node = movePath.erase(node);
    }
}

sf::Vector2f MovePath::getFirstNodePosition() const {
    if (!movePath.empty()) {
        return movePath.front().getPosition(); // Lấy vị trí của node đầu tiên
    }
    return sf::Vector2f(0.0f, 0.0f); // Trả về vị trí mặc định nếu không có node nào
}

void MovePath::createSegments() {
    segments.clear();
    segmentBounds.clear();

    if (movePath.size() < 2) return; 

    for (size_t i = 0; i < movePath.size() - 1; ++i) {
        sf::Vector2f start = movePath[i].getPosition();
        sf::Vector2f end = movePath[i + 1].getPosition();

        // Tính toán hướng và độ dài đoạn thẳng
        sf::Vector2f direction = end - start;
        float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);

        if (length > 0.0f) {
            direction /= length; // Chuẩn hóa hướng

            // Mở rộng điểm đầu và điểm cuối
            sf::Vector2f extendedStart = start - direction * width; // Lùi điểm đầu
            sf::Vector2f extendedEnd = end + direction * width;     // Kéo dài điểm cuối

            // Tính độ dài mới
            float extendedLength = length + width;

            // Tính vị trí trung tâm mới
            sf::Vector2f center = (extendedStart + extendedEnd) / 2.0f;

            // Tạo đoạn đường
            sf::RectangleShape segment(sf::Vector2f(extendedLength, width));
            segment.setFillColor(color); // Màu của đường
            segment.setOrigin(sf::Vector2f(extendedLength / 2.0f, width / 2.0f)); // Đặt gốc tại tâm
            segment.setPosition(center); // Đặt vị trí trung tâm
            segment.setRotation(std::atan2(direction.y, direction.x) * 180.0f / 3.14159f); // Góc xoay theo hướng
            segments.push_back(segment);
            segmentBounds.push_back(sf::Vector2f(extendedLength, width));
        }
    }
}

//void drawIcon(sf::Vector2f direction, sf::RenderWindow& window) {
//    sf::Texture arrowTexture;
//    if (!arrowTexture.loadFromFile("images/icons/RightArrowIcon.png")) {
//        return;
//    }
//
//    sf::Sprite arrowSprite(arrowTexture);
//    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
//
//    if (length > 0.0f) {
//        direction /= length;
//    }
//
//    float angle = std::atan2(direction.y, direction.x) * 180.f / 3.14159f;
//    sf::Vector2f offset = direction * 30.0f;
//
//    arrowSprite.setPosition(nearEndPoint + offset);
//    arrowSprite.setOrigin(arrowTexture.getSize().x / 2.f, arrowTexture.getSize().y / 2.f);
//    arrowSprite.setRotation(angle);
//
//    window.draw(arrowSprite);
//}

void MovePath::render(sf::RenderWindow& window) {
    for (const auto& segment : segments) {
        window.draw(segment);
    }

    sf::Vector2f endPoint = movePath[1].getPosition();
    sf::Vector2f nearEndPoint = movePath[0].getPosition();
    sf::Vector2f direction = endPoint - nearEndPoint;

    sf::Texture arrowTexture;
    if (!arrowTexture.loadFromFile("images/icons/RightArrowIcon.png")) {
        return;
    }

    sf::Sprite startArrowSprite(arrowTexture);
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    direction /= length;
    

    float angle = std::atan2(direction.y, direction.x) * 180.f / 3.14159f; 
    sf::Vector2f offset = direction * 30.0f; 

    startArrowSprite.setPosition(nearEndPoint + offset);
    startArrowSprite.setOrigin(arrowTexture.getSize().x / 2.f, arrowTexture.getSize().y / 2.f);
    startArrowSprite.setRotation(angle);

    window.draw(startArrowSprite);

    /*for (size_t i = 0; i < segments.size(); ++i) {
        sf::RectangleShape boundShape(segmentBounds[i]); 
        boundShape.setOrigin(segmentBounds[segments.size()] / 2.0f);  
        boundShape.setPosition(segments[i].getPosition()); 
        boundShape.setRotation(segments[i].getRotation()); 
        boundShape.setFillColor(sf::Color::Transparent);  
        boundShape.setOutlineColor(sf::Color::Red);    
        boundShape.setOutlineThickness(1.0f);       

        window.draw(boundShape);
    }*/
}


