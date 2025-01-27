/*
** EPITECH PROJECT, 2025
** B-CPP-500-NCE-5-2-rtype-steven.deffontaine
** File description:
** ObstacleFactory
*/

class ObstacleFactory {
public:
    static std::shared_ptr<Entity> createObstacle(float x, float y) {
        auto obstacle = std::make_shared<Entity>();
        obstacle->addComponent<PositionComponent>(x, y);
        obstacle->addComponent<ObstacleComponent>(0.01f);

        auto render = RenderComponent(RenderComponent::Type::OBSTACLE);
        
        // Charger la sprite sheet des obstacles
        render.texture = TextureManager::loadTexture("client/sprites/r-typesheet39.png");
        render.sprite.setTexture(*render.texture);
        
        // Configuration de l'animation
        render.frameRect = sf::IntRect(0, 0, 32, 100);  // Ajustez ces valeurs selon votre sprite sheet
        render.sprite.setTextureRect(render.frameRect);
        render.sprite.setOrigin(16.0f, 90.0f);
        render.sprite.setScale(4.0f, 4.0f);
        
        // Configuration de l'animation si nécessaire
        render.frameTime = 0.08f;
        render.frameCount = 3;  // Ajustez selon le nombre de frames dans votre sprite sheet
        render.currentFrame = 0;
        render.animationTimer = 0.0f;
        render.frameWidth = 32;  // Ajustez selon la largeur de chaque frame

        obstacle->addComponent<RenderComponent>(render);
        return obstacle;
    }
};
