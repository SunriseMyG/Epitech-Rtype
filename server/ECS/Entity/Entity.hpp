#ifndef ENTITY_HPP
    #define ENTITY_HPP

    #include <string>
    #include <vector>
    #include <iostream>
    #include <memory>
    #include "../Component/Component.hpp"

    class Entity {
        public:
            Entity(int id, std::string group, std::string type) : id_(id), group_(group), type_(type) {}
            ~Entity() {}

            void addComponent(std::shared_ptr<Component> component) {
                components_.push_back(component);
            }

            template <typename T>
            std::shared_ptr<T> getComponent() {
                for (auto component : components_) {
                    if (std::shared_ptr<T> t = std::dynamic_pointer_cast<T>(component)) {
                        return t;
                    }
                }
                return nullptr;
            }

            int getId() const {
                return id_;
            }

            void setId(int id) {
                id_ = id;
            }

            std::string getGroup() const {
                return group_;
            }

            void setGroup(std::string group) {
                group_ = group;
            }

            std::string getType() const {
                return type_;
            }

            void setType(std::string type) {
                type_ = type;
            }

            bool isMarkedForDeletion() const {
                return markedForDeletion_;
            }

            void markForDeletion() {
                markedForDeletion_ = true;

                if (this->getComponent<StateComponent>()) {
                    this->getComponent<StateComponent>()->state = "DELETE";
                }
            }

        private:
            int id_;
            std::string group_;
            std::string type_;
            std::vector<std::shared_ptr<Component>> components_;
            bool markedForDeletion_ = false;
    };

#endif // ENTITY_HPP
