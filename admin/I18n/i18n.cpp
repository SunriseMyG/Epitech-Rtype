#include "i18n.hpp"

std::string i18n::get(const std::string& key) {
    auto langIt = translations.find(language);
    if (langIt != translations.end()) {
        auto keyIt = langIt->second.find(key);
        if (keyIt != langIt->second.end()) {
            return keyIt->second;
        }
    }

    return "translation[" + enumToString(language) + "][" + key + "] not found";
}
void i18n::set(const Language& language, const std::string& key, const std::string& value) {
    translations[language][key] = value;
}

void i18n::setLanguage(const Language& language) {
    this->language = language;
}

void i18n::loadFromFile(const Language& language, const std::string& filePath) {
    std::ifstream file(filePath);
    if (file.is_open()) {
        nlohmann::json jsonFile;
        file >> jsonFile;

        for (auto& [key, value] : jsonFile.items()) {
            translations[language][key] = value;
        }

        file.close();

        std::cout << "Language " << static_cast<int>(language) << " loaded from file: " << filePath << std::endl;
    } else {
        throw std::runtime_error("Could not open file: " + filePath);
    }
}

std::string i18n::enumToString(const Language& language) {
    switch (language) {
        case Language::EN:
            return "EN";
        case Language::FR:
            return "FR";
        case Language::ES:
            return "ES";
    }

    return "Unknown";
}