#ifndef I18N_HPP
#define I18N_HPP

#include <map>
#include <string>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

class i18n {
    public:
        enum class Language {
            EN,
            FR,
            ES
        };

        i18n() = default;
        ~i18n() = default;

        std::string get(const std::string& key);
        void set(const Language& language, const std::string& key, const std::string& value);
        void setLanguage(const Language& language);
        Language getLanguage() const { return language; }
        void loadFromFile(const Language& language, const std::string& filePath);
        std::string enumToString(const Language& language);
        int getNumberOfLanguages() const { return translations.size() + 1; }

    private:
        Language language;
        std::map<Language, std::map<std::string, std::string>> translations;
};

#endif // I18N_HPP
