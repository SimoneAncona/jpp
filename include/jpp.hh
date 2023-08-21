/**
 * @file jpp.hh
 * @author Simone Ancona
 * @brief A JSON parser for C++
 * @version 1.4.1
 * @date 2023-07-23
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

#define l_object std::vector<std::pair<std::string, std::any>>
#define l_array std::vector<std::any>

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

        std::map<std::string, Json> parse_object(std::string_view str, size_t &index)
        {
            std::map<std::string, Jpp::Json> object;
            Jpp::Token next;
            std::string current_property;
            Jpp::Json current_value;

            ++index;
            skip_white_spaces(str, index);

            while (true)
            {
                next = match_next(str, index);

                switch (next)
                {
                case Jpp::Token::END:
                    throw std::invalid_argument("Unexpected the end of the string, expected a '}' at position: " + std::to_string(index));
                case Jpp::Token::ARRAY_START:
                    throw std::runtime_error("Unexpected the start of an array, expected a property name at position: " + std::to_string(index));
                case Jpp::Token::ARRAY_END:
                    throw std::runtime_error("Unexpected the end of an array, expected a property name at position: " + std::to_string(index));
                case Jpp::Token::ALPHA:
                    throw std::runtime_error("Unexpected a boolean value, expected a property name at position: " + std::to_string(index));
                case Jpp::Token::NUMBER:
                    throw std::runtime_error("Unexpected a number value, expected a property name at position: " + std::to_string(index));
                case Jpp::Token::OBJECT_START:
                    throw std::runtime_error("Unexpected the start of an object, expected a property name at position: " + std::to_string(index));
                case Jpp::Token::OBJECT_END:
                    return object;
                case Jpp::Token::SEPARATOR:
                    throw std::runtime_error("Unexpected separator, expected a property name at position: " + std::to_string(index));
                case Jpp::Token::STRING:
                    current_property = parse_string(str, index, str[index]);
                    break;
                }

                skip_white_spaces(str, index);

                if (str[index] != ':')
                    throw std::runtime_error("Expected ':' at position: " + std::to_string(index));

                ++index;

                skip_white_spaces(str, index);

                next = match_next(str, index);

                switch (next)
                {
                case Jpp::Token::END:
                    throw std::runtime_error("Unexpected the end of the string, a value is expected at position: " + std::to_string(index));
                case Jpp::Token::ARRAY_START:
                    current_value = get_unresolved_object(str, index, false);
                    break;
                case Jpp::Token::ARRAY_END:
                    throw std::runtime_error("Unexpected the end of an array, a value is expected at position: " + std::to_string(index));
                case Jpp::Token::OBJECT_START:
                    current_value = get_unresolved_object(str, index, true);
                    break;
                case Jpp::Token::OBJECT_END:
                    throw std::runtime_error("Unexpected the end of the object, a value is expected at position: " + std::to_string(index));
                case Jpp::Token::ALPHA:
                    if (str[index] == 'n')
                        current_value = Jpp::Json(parse_null(str, index), Jpp::JSON_NULL);
                    else
                        current_value = Jpp::Json(parse_boolean(str, index), Jpp::JSON_BOOLEAN);
                    break;
                case Jpp::Token::NUMBER:
                    current_value = Jpp::Json(parse_number(str, index), Jpp::JSON_NUMBER);
                    break;
                case Jpp::Token::STRING:
                    current_value = Jpp::Json(parse_string(str, index, str[index]), Jpp::JSON_STRING);
                    break;
                case Jpp::Token::SEPARATOR:
                    throw std::runtime_error("Unexpected separator, a value is expected at position: " + std::to_string(index));
                }

                skip_white_spaces(str, index);

                next = match_next(str, index);
                if (next != Jpp::Token::SEPARATOR && next != Jpp::Token::OBJECT_END)
                    throw std::runtime_error("Expected a ',' or the end of the object at position: " + std::to_string(index));

                ++index;

                skip_white_spaces(str, index);

                object.insert(std::pair<std::string, Jpp::Json>(current_property, current_value));

                if (next == Jpp::Token::OBJECT_END)
                    return object;
            }
        }

        std::map<std::string, Json> parse_array(std::string_view str, size_t &index)
        {
            std::map<std::string, Jpp::Json> object;
            Jpp::Token next;
            size_t current_index = 0;
            Jpp::Json current_value;

            ++index;
            skip_white_spaces(str, index);

            while (true)
            {
                next = match_next(str, index);

                switch (next)
                {
                case Jpp::Token::END:
                    throw std::runtime_error("Unexpected the end of the string, the end of the array is expected at position: " + std::to_string(index));
                case Jpp::Token::ARRAY_START:
                    current_value = Jpp::Json(parse_array(str, index), Jpp::JSON_ARRAY);
                    break;
                case Jpp::Token::ARRAY_END:
                    return object;
                case Jpp::Token::OBJECT_START:
                    current_value = Jpp::Json(parse_object(str, index), Jpp::JSON_OBJECT);
                    break;
                case Jpp::Token::OBJECT_END:
                    throw std::runtime_error("Unexpected '}' token, a value is expected at position: " + std::to_string(index));
                case Jpp::Token::ALPHA:
                    if (str[index] == 'n')
                        current_value = Jpp::Json(parse_null(str, index), Jpp::JSON_NULL);
                    else
                        current_value = Jpp::Json(parse_boolean(str, index), Jpp::JSON_BOOLEAN);
                    break;
                case Jpp::Token::NUMBER:
                    current_value = Jpp::Json(parse_number(str, index), Jpp::JSON_NUMBER);
                    break;
                case Jpp::Token::STRING:
                    current_value = Jpp::Json(parse_string(str, index, str[index]), Jpp::JSON_STRING);
                    break;
                case Jpp::Token::SEPARATOR:
                    throw std::runtime_error("Unexpected separator, a value is expected at position: " + std::to_string(index));
                }

                skip_white_spaces(str, index);

                next = match_next(str, index);
                if (next != Jpp::Token::SEPARATOR && next != Jpp::Token::ARRAY_END)
                    throw std::runtime_error("Expected a ',' or the end of the array at position: " + std::to_string(index));

                ++index;

                skip_white_spaces(str, index);

                object.insert(std::pair<std::string, Jpp::Json>(std::to_string(current_index), current_value));
                ++current_index;

                if (next == Jpp::Token::ARRAY_END)
                    return object;
            }
        }

        std::string parse_string(std::string_view str, size_t &index, char start_with)
        {
            std::string value;
            bool escape = false;

            ++index;
            while (true)
            {
                if (index >= str.length())
                    throw std::runtime_error("Expected the end of the string");
                if (str[index] == '\n')
                    throw std::runtime_error("Unexpected end of the line while parsing the string: '" + value + "' at position: " + std::to_string(index));
                if (str[index] == '\\' && !escape)
                {
                    escape = true;
                    ++index;
                    continue;
                }

                if (str[index] == start_with && !escape)
                {
                    ++index;
                    return value;
                }

                if (escape)
                {
                    escape = false;
                    switch (str[index])
                    {
                    case 'n':
                        value += '\n';
                        ++index;
                        continue;
                    case 't':
                        value += '\t';
                        ++index;
                        continue;
                    case 'r':
                        value += '\r';
                        ++index;
                        continue;
                    case 'v':
                        value += '\v';
                        ++index;
                        continue;
                    case 'b':
                        value += '\b';
                        ++index;
                        continue;
                    }
                }
                value += str[index];
                ++index;
                escape = false;
            }
        }

        std::any parse_number(std::string_view str, size_t &index)
        {
            size_t start = index;
            next_white_space_or_separator(str, index);
            size_t end = index;
            std::string_view substr = str.substr(start, end - start);

            return std::stod(substr.data());
        }

        std::any parse_boolean(std::string_view str, size_t &index)
        {
            size_t start = index;
            next_white_space_or_separator(str, index);
            size_t end = index;
            std::string_view substr = str.substr(start, end - start);

            if (substr == "true")
                return true;
            if (substr == "false")
                return false;
            throw std::runtime_error("Unrecognized token: " + std::string(substr.data()) + " at position: " + std::to_string(index));
        }

        std::any parse_null(std::string_view str, size_t &index)
        {
            size_t start = index;
            next_white_space_or_separator(str, index);
            size_t end = index;
            std::string_view substr = str.substr(start, end - start);

            if (substr == "null")
                return nullptr;

            throw std::runtime_error("Unrecognized token: " + std::string(substr.data()) + " at position: " + std::to_string(index));
        }

        bool is_resolved;
        std::string unresolved_string;

        Token match_next(std::string_view str, size_t &index)
        {
            if (index >= str.length())
                return Jpp::Token::END;
            switch (str[index])
            {
            case '{':
                return Jpp::Token::OBJECT_START;
            case '}':
                return Jpp::Token::OBJECT_END;
            case ',':
                return Jpp::Token::SEPARATOR;
            case '"':
            case '\'':
                return Jpp::Token::STRING;
            case '[':
                return Jpp::Token::ARRAY_START;
            case ']':
                return Jpp::Token::ARRAY_END;
            }
            if (str[index] >= '0' && str[index] <= '9')
                return Jpp::Token::NUMBER;
            if (isalpha(str[index]))
                return Jpp::Token::ALPHA;
            throw std::runtime_error("Unexpected " + std::string(1, str[index]) + " token at position: " + std::to_string(index));
        }

        inline bool is_space(char ch) noexcept
        {
            return ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r' || ch == '\v';
        }

        inline void next_white_space_or_separator(std::string_view str, size_t &index) noexcept
        {
            while (index < str.length() && !is_space(str[index]) && str[index] != '[' && str[index] != '{' && str[index] != ',' && str[index] != ']' && str[index] != '}')
                ++index;
        }

        inline void skip_white_spaces(std::string_view str, size_t &index) noexcept
        {
            while (index < str.length() && is_space(str[index]))
                ++index;
        }

        inline std::string json_object_to_string(Json json)
        {
            std::map<std::string, Jpp::Json> children = json.children;
            if (children.size() == 0)
                return "{}";
            if (!is_resolved)
                return unresolved_string;
            std::map<std::string, Jpp::Json>::iterator it;
            std::string str = "{";

            for (it = children.begin(); it != std::prev(children.end()); ++it)
            {
                str += "\"" + it->first + "\":";
                str += it->second.to_string();
                str += ", ";
            }

            str += "\"" + std::prev(children.end())->first + "\":";
            str += std::prev(children.end())->second.to_string();

            return str + "}";
        }

        inline std::string json_array_to_string(Json json)
        {
            std::map<std::string, Jpp::Json> children = json.children;
            if (children.size() == 0)
                return "[]";
            if (!is_resolved)
                return unresolved_string;
            std::map<std::string, Jpp::Json>::iterator it;
            std::string str = "[";

            for (it = children.begin(); it != std::prev(children.end()); ++it)
            {
                str += it->second.to_string();
                str += ",";
            }

            str += std::prev(children.end())->second.to_string();

            return str + "]";
        }

        inline std::string str_replace(std::string_view original, char old, std::string_view new_str)
        {
            std::string str = "";
            for (char ch : original)
            {
                if (ch == old)
                    str += new_str;
                else
                    str += ch;
            }
            return str;
        }

        Json get_unresolved_object(std::string_view str, size_t &index, bool is_object)
        {
            const char end = is_object ? '}' : ']';
            const char start = is_object ? '{' : '[';
            char is_string = false;
            bool escape = false;
            std::string unresolved;
            Jpp::Json unresolved_json;
            int level = 0;
            bool cycle = true;

            while (cycle)
            {
                if (!(is_space(str[index]) && !is_string))
                    unresolved += str[index];
                index++;
                if (index >= str.length())
                    throw std::runtime_error("Unexpected end of the string");

                switch (str[index])
                {
                case '"':
                    if (escape || is_string == '\'')
                    {
                        escape = false;
                        break;
                    }
                    if (is_string == '"')
                    {
                        escape = false;
                        is_string = false;
                        break;
                    }
                    escape = false;
                    is_string = '"';
                    break;
                case '\'':
                    if (escape || is_string == '"')
                    {
                        escape = false;
                        break;
                    }
                    if (is_string == '\'')
                    {
                        escape = false;
                        is_string = false;
                        break;
                    }
                    escape = false;
                    is_string = '\'';
                    break;
                case '\\':
                    if (!is_string)
                        throw std::runtime_error("Unexpected '\\' token at position: " + std::to_string(index));
                    escape = !escape;
                    break;
                case '{':
                case '[':
                    if (str[index] != start)
                        break;
                    if (!is_string)
                        level++;
                    break;
                case '}':
                case ']':
                    if (str[index] != end)
                        break;
                    if (!is_string)
                    {
                        if (level == 0)
                            cycle = false;
                        else
                            level--;
                        break;
                    }
                    break;
                default:
                    escape = false;
                    break;
                }
            }
            index++;
            unresolved += end;
            unresolved_json.type = is_object ? JSON_OBJECT : JSON_ARRAY;
            unresolved_json.is_resolved = false;
            unresolved_json.unresolved_string = unresolved;
            return unresolved_json;
        }

    public:
        /**
         * @brief Construct a new Json object
         * @since v1.0
         */
        inline Json() noexcept
        {
            this->type = JSON_OBJECT;
            this->is_resolved = true;
        }

        /**
         * @brief Construct a new Json object
         *
         * @param children
         * @param type
         * @since v1.0
         */
        inline Json(std::map<std::string, Json> children, JsonType type) noexcept
        {
            this->children = children;
            this->type = type;
            this->is_resolved = true;
        }

        /**
         * @brief Construct a new Json object
         *
         * @param value
         * @param type
         * @since v1.0
         */
        inline Json(std::any value, JsonType type) noexcept
        {
            this->value = value;
            this->type = type;
            this->is_resolved = true;
        }

        /**
         * @brief Construct a new Json object
         *
         * @param values
         * @since v1.4
         */
        inline Json(std::vector<std::any> values)
        {
            this->type = JSON_ARRAY;
            this->is_resolved = true;
            for (size_t i = 0; i < values.size(); ++i)
            {
                this->children.emplace(std::to_string(i), Json(values[i]));
            }
        }

        /**
         * @brief Construct a new Json object
         *
         * @param key_values
         * @since v1.4
         */
        inline Json(std::vector<std::pair<std::string, std::any>> key_values)
        {
            this->type = JSON_OBJECT;
            this->is_resolved = true;
            for (size_t i = 0; i < key_values.size(); ++i)
            {
                this->children.emplace(key_values[i].first, Json(key_values[i].second));
            }
        }

        /**
         * @brief Construct a new Json object
         *
         * @param value
         * @since v1.4
         */
        Json(std::any value)
        {
            this->is_resolved = true;
            size_t hash = value.type().hash_code();
            if (hash == typeid(int).hash_code())
            {
                this->value = static_cast<double>(std::any_cast<int>(value));
                this->type = JSON_NUMBER;
                return;
            }
            if (hash == typeid(const char *).hash_code())
            {
                this->value = std::string(std::any_cast<const char *>(value));
                this->type = JSON_STRING;
                return;
            }
            if (hash == typeid(std::string).hash_code())
            {
                this->value = value;
                this->type = JSON_STRING;
                return;
            }
            if (hash == typeid(bool).hash_code())
            {
                this->value = value;
                this->type = JSON_BOOLEAN;
                return;
            }
            if (hash == typeid(double).hash_code())
            {
                this->value = value;
                this->type = JSON_NUMBER;
                return;
            }
            if (hash == typeid(std::nullptr_t).hash_code())
            {
                this->value = value;
                this->type = JSON_NULL;
                return;
            }
            throw std::runtime_error("Unknown type: " + std::string(value.type().name()));
        }

        /**
         * @brief Construct a new Json object
         *
         * @param str
         * @since v1.0
         */
        inline Json(std::string str) noexcept
        {
            this->value = str;
            this->type = JSON_STRING;
            this->is_resolved = true;
        }

        /**
         * @brief Construct a new Json object
         *
         * @param num
         * @since v1.0
         */
        inline Json(double num) noexcept
        {
            this->value = num;
            this->type = JSON_NUMBER;
            this->is_resolved = true;
        }

        /**
         * @brief Construct a new Json object
         *
         * @param val
         * @since v1.0
         */
        inline Json(bool val) noexcept
        {
            this->value = val;
            this->type = JSON_BOOLEAN;
            this->is_resolved = true;
        }

        /**
         * @brief Construct a new Json object
         *
         * @param null
         * @since v1.0
         */
        inline Json(nullptr_t null) noexcept
        {
            this->value = null;
            this->type = JSON_NULL;
            this->is_resolved = true;
        }

        ~Json() = default;

        /**
         * @brief Get the type object
         *
         * @return JsonType
         * @since v1.0
         */
        inline JsonType get_type() noexcept
        {
            return this->type;
        }

        /**
         * @brief Get the value object
         *
         * @return std::any
         * @since v1.0
         */
        inline std::any get_value() noexcept
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
        inline bool is_array() noexcept
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
        inline bool is_object() noexcept
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
        inline bool is_string() noexcept
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
        inline bool is_boolean() noexcept
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
        inline bool is_number() noexcept
        {
            return this->type == JSON_NUMBER;
        }

        /**
         * @brief Parse a JSON string
         * @since v1.0
         */
        void parse(const std::string &json_string)
        {
            size_t start = 0;
            this->is_resolved = true;
            if (json_string[start] == '{')
            {
                this->children = parse_object(json_string, start);
                this->unresolved_string = "";
                this->type = Jpp::JSON_OBJECT;
                return;
            }
            if (json_string[start] == '[')
            {
                this->children = parse_array(json_string, start);
                this->unresolved_string = "";
                this->type = Jpp::JSON_ARRAY;
                return;
            }
            throw std::runtime_error("Unexpected " + std::string(1, json_string[0]) + " at the beginning of the string");
        }

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
        inline Json &operator[](size_t index)
        {
            if (this->type > Jpp::JSON_OBJECT)
                throw std::out_of_range("Cannot use the subscript operator with an atomic value, use get_value");
            if (!is_resolved)
                parse(unresolved_string);
            return this->children[std::to_string(index)];
        }

        /**
         * @brief Access to a value of the object with the given property name
         *
         * @return Json&
         * @since v1.0
         */
        inline Json &operator[](const std::string &property)
        {
            if (this->type > Jpp::JSON_OBJECT)
                throw std::out_of_range("Cannot use the subscript operator with an atomic value, use get_value");
            if (this->type == Jpp::JSON_OBJECT && this->children.find(property) == this->children.end())
                this->children.emplace(property, Json(nullptr));
            if (!is_resolved)
                parse(unresolved_string);
            return this->children.at(property);
        }

        /**
         * @return Json&
         * @since v1.0
         */
        inline Json &operator=(const std::string &str)
        {
            this->children.clear();
            this->is_resolved = true;
            this->type = Jpp::JSON_STRING;
            this->value = str;

            return *this;
        }

        /**
         * @return Json&
         * @since v1.0
         */
        inline Json &operator=(double val)
        {
            this->children.clear();
            this->is_resolved = true;
            this->type = Jpp::JSON_NUMBER;
            this->value = val;

            return *this;
        }

        /**
         * @return Json&
         * @since v1.0
         */
        inline Json &operator=(int val)
        {
            this->children.clear();
            this->is_resolved = true;
            this->type = Jpp::JSON_NUMBER;
            this->value = static_cast<double>(val);

            return *this;
        }

        /**
         * @return Json&
         * @since v1.0
         */
        inline Json &operator=(bool val)
        {
            this->children.clear();
            this->is_resolved = true;
            this->type = Jpp::JSON_BOOLEAN;
            this->value = val;

            return *this;
        }

        /**
         * @return Json&
         * @since v1.0
         */
        inline Json &operator=(const char str[])
        {
            this->children.clear();
            this->is_resolved = true;
            this->type = Jpp::JSON_STRING;
            this->value = std::string(str);

            return *this;
        }

        /**
         * @return Json&
         * @since v1.4
         */
        inline Json &operator=(std::vector<std::any> array)
        {
            this->children.clear();
            this->type = Jpp::JSON_ARRAY;
            this->is_resolved = true;
            for (size_t i = 0; i < array.size(); ++i)
            {
                this->children.emplace(std::to_string(i), Json(array[i]));
            }
            return *this;
        }

        /**
         * @return Json&
         * @since v1.4
         */
        inline Json &operator=(std::vector<std::pair<std::string, std::any>> object)
        {
            this->children.clear();
            this->type = Jpp::JSON_OBJECT;
            this->is_resolved = true;
            for (size_t i = 0; i < object.size(); ++i)
            {
                this->children.emplace(object[i].first, Json(object[i].second));
            }
            return *this;
        }

        /**
         * @brief Convert the JSON object to its JSON representation.
         *
         * @return std::string
         */
        inline std::string to_string()
        {
            switch (this->type)
            {
            case Jpp::JSON_OBJECT:
                return json_object_to_string(*this);
            case Jpp::JSON_ARRAY:
                return json_array_to_string(*this);
            case Jpp::JSON_STRING:
                return "\"" +
                       str_replace(
                           str_replace(std::any_cast<std::string>(this->value), '"', "\\\""), '\n', "\\n") +
                       "\"";
            case Jpp::JSON_BOOLEAN:
                return std::any_cast<bool>(this->value) ? "true" : "false";
            case Jpp::JSON_NUMBER:
                return std::to_string(std::any_cast<double>(this->value));
            case Jpp::JSON_NULL:
                return "null";
            }
            return "";
        }

        /**
         * @brief Begin iterator
         *
         * @return std::map<std::string, Json>::iterator
         * @since v1.1
         */
        inline std::map<std::string, Json>::iterator begin() noexcept
        {
            return children.begin();
        }

        /**
         * @brief End iterator
         *
         * @return std::map<std::string, Json>::iterator
         * @since v1.1
         */
        inline std::map<std::string, Json>::iterator end() noexcept
        {
            return children.end();
        }

        /**
         * @brief Reverse begin iterator
         *
         * @return std::map<std::string, Json>::iterator
         * @since v1.1
         */
        inline std::map<std::string, Json>::reverse_iterator rbegin() noexcept
        {
            return children.rbegin();
        }

        /**
         * @brief Reverse end iterator
         *
         * @return std::map<std::string, Json>::iterator
         * @since v1.1
         */
        inline std::map<std::string, Json>::reverse_iterator rend() noexcept
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