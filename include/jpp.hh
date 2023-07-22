/**
 * @file json.hh
 * @author Simone Ancona
 * @brief A JSON parser for C++
 * @version 1.3.1
 * @date 2023-07-22
 *
 * @copyright Copyright (c) 2023
 *
 */

#pragma once

#include <string>
#include <map>
#include <stdexcept>
#include <any>
#include <cctype>
#include <vector>

namespace Jpp
{
    enum JsonType
    {
        JSON_ARRAY,
        JSON_OBJECT,
        JSON_STRING,
        JSON_BOOLEAN,
        JSON_NUMBER,
        JSON_NULL
    };

    enum Token
    {
        OBJECT_START,
        ARRAY_START,
        OBJECT_END,
        ARRAY_END,
        NUMBER,
        STRING,
        ALPHA,
        SEPARATOR,
        END,
    };

    /**
     * @brief The Json class allows to parse a json string
     *
     */
    class Json
    {
    private:
        JsonType type;
        std::map<std::string, Json> children;
        std::any value;

        std::map<std::string, Json> parse_object(std::string_view, size_t &);
        std::map<std::string, Json> parse_array(std::string_view, size_t &);
        std::string parse_string(std::string_view, size_t &, char);
        std::any parse_number(std::string_view, size_t &);
        std::any parse_boolean(std::string_view, size_t &);
        std::any parse_null(std::string_view, size_t &);
        bool is_resolved;
        std::string unresolved_string;

        Token match_next(std::string_view, size_t &);

        inline void next_white_space_or_separator(std::string_view str, size_t &index)
        {
            while (index < str.length() && !isspace(str[index]) && str[index] != '[' && str[index] != '{' && str[index] != ',' && str[index] != ']' && str[index] != '}')
                ++index;
        }

        inline void skip_white_spaces(std::string_view str, size_t &index)
        {
            while (index < str.length() && isspace(str[index]))
                ++index;
        }

        std::string json_object_to_string(Json);
        std::string json_array_to_string(Json);
        std::string str_replace(std::string_view, char, std::string_view);

        Json get_unresolved_object(std::string_view, size_t&, bool);

    public:
        inline Json() noexcept
        {
            this->type = JSON_OBJECT;
            this->is_resolved = true;
        }

        inline Json(std::map<std::string, Json> children, JsonType type) noexcept
        {
            this->children = children;
            this->type = type;
            this->is_resolved = true;
        }

        inline Json(std::any value, JsonType type) noexcept
        {
            this->value = value;
            this->type = type;
            this->is_resolved = true;
        }

        inline Json(std::string str) noexcept
        {
            this->value = str;
            this->type = JSON_STRING;
            this->is_resolved = true;
        }

        inline Json(double num) noexcept
        {
            this->value = num;
            this->type = JSON_NUMBER;
            this->is_resolved = true;
        }

        Json(bool val)
        {
            this->value = val;
            this->type = JSON_BOOLEAN;
            this->is_resolved = true;
        }

        Json(nullptr_t null)
        {
            this->value = null;
            this->type = JSON_NULL;
            this->is_resolved = true;
        }

        ~Json() = default;

        /**
         * @brief Get the type object
         *
         * @return json_type_t
         * @since v1.0
         */
        inline JsonType get_type()
        {
            return this->type;
        }

        /**
         * @brief Get the value object
         *
         * @return std::any
         * @since v1.0
         */
        inline std::any get_value()
        {
            return this->value;
        }

        /**
         * @brief Check if the JSON value is an array
         *
         * @return true
         * @return false
         * @since v1.0
         */
        inline bool is_array()
        {
            return this->type == JSON_ARRAY;
        }

        /**
         * @brief Check if the JSON value is an object
         *
         * @return true
         * @return false
         * @since v1.0
         */
        inline bool is_object()
        {
            return this->type == JSON_OBJECT;
        }

        /**
         * @brief Check if the JSON is an atomic string value
         *
         * @return true
         * @return false
         * @since v1.0
         */
        inline bool is_string()
        {
            return this->type == JSON_STRING;
        }

        /**
         * @brief Check if the JSON is an atomic boolean value
         *
         * @return true
         * @return false
         * @since v1.0
         */
        inline bool is_boolean()
        {
            return this->type == JSON_BOOLEAN;
        }

        /**
         * @brief Check if the JSON is an atomic double value
         *
         * @return true
         * @return false
         * @since v1.0
         */
        inline bool is_number()
        {
            return this->type == JSON_NUMBER;
        }

        /**
         * @brief Parse a JSON string
         * @since v1.0
         */
        void parse(std::string);

        /**
         * @brief Get the children object
         *
         * @return std::map<std::string, Json>
         * @since v1.0
         */
        inline std::map<std::string, Json> get_children()
        {
            if (!is_resolved)
                parse(unresolved_string);
            return this->children;
        }

        /**
         * @brief Access to a position of the array
         * @example
         *  Jpp::Json json;
         *  json.parse("[0, 1, 2, 3]");
         *  json[0] // returns an any value
         * @return Json&
         * @since v1.0
         */
        Json &operator[](size_t);

        /**
         * @brief Access to a value of the object with the given property name
         *
         * @return Json&
         * @since v1.0
         */
        Json &operator[](const std::string&);

        /**
         * @return Json&
         * @since v1.0
         */
        Json &operator=(const std::string&);

        /**
         * @return Json&
         * @since v1.0
         */
        Json &operator=(double);

        /**
         * @return Json&
         * @since v1.0
         */
        Json &operator=(int);

        /**
         * @return Json&
         * @since v1.0
         */
        Json &operator=(bool);

        /**
         * @return Json&
         * @since v1.0
         */
        Json &operator=(const char[]);

        /**
         * @brief Convert the JSON object to its JSON representation.
         *
         * @return std::string
         */
        std::string to_string();

        /**
         * @brief Begin iterator
         *
         * @return std::map<std::string, Json>::iterator
         * @since v1.1
         */
        inline std::map<std::string, Json>::iterator begin()
        {
            return children.begin();
        }

        /**
         * @brief End iterator
         *
         * @return std::map<std::string, Json>::iterator
         * @since v1.1
         */
        inline std::map<std::string, Json>::iterator end()
        {
            return children.end();
        }

        /**
         * @brief Reverse begin iterator
         *
         * @return std::map<std::string, Json>::iterator
         * @since v1.1
         */
        inline std::map<std::string, Json>::reverse_iterator rbegin()
        {
            return children.rbegin();
        }

        /**
         * @brief Reverse end iterator
         *
         * @return std::map<std::string, Json>::iterator
         * @since v1.1
         */
        inline std::map<std::string, Json>::reverse_iterator rend()
        {
            return children.rend();
        }

        /**
         * @brief Get the vector if the JSON object is an array
         *
         * @return std::vector<Json>
         * @since v1.2
         */
        inline std::vector<Json> get_vector()
        {
            if (type != JSON_ARRAY)
                throw std::runtime_error("Cannot convert a non-array JSON to a vector");
            std::vector<Jpp::Json> vct;
            for (auto json : children)
            {
                vct.push_back(json.second);
            }
            return vct;
        }
    };
};