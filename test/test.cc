#include "jpp.hh"
#include <fstream>
#include <sstream>
#include <iostream>

std::string read_string_from_file(const std::string &);

int main(int argc, char **argv)
{
    try
    {
        Jpp::Json json;
        Jpp::Json json1;
        json.parse(read_string_from_file("json/e1.json"));
        std::cout << json.to_string() << "\n"
                  << std::endl;
        std::cout << json["quiz"]["maths"].to_string() << "\n"
                  << std::endl;
        json["hello"] = "world";
        std::cout << json.to_string() << "\n"
                  << std::endl;

        std::cout << json1.to_string() << "\n"
                  << std::endl;
        json1["name"] = "simon";
        json1["surname"] = Jpp::Json(nullptr);
        std::cout << json1.to_string() << "\n"
                  << std::endl;

        Jpp::Json car;
        Jpp::Json car_collection;
        car["brand"] = "Brand1";
        car["age"] = 10;
        car["model"] = "Model1";

        car_collection["favoriteCar"] = car;

        std::cout << car_collection.to_string() << "\n"
                  << std::endl;

        Jpp::Json array;
        array.parse("[1, 2, 3, \"Hello World\"]");
        std::cout << array.to_string() << std::endl;
        for (auto a : array)
        {
            std::cout << a.second.to_string() << std::endl;
        }

        std::vector<Jpp::Json> to_vector = array.get_vector();
        for (auto a : to_vector)
        {
            std::cout << a.to_string() << " ";
        }
        std::cout << std::endl;
    }
    catch (const std::exception e)
    {
        std::cout << e.what() << std::endl;
    }
}

std::string read_string_from_file(const std::string &file_path)
{
    const std::ifstream input_stream(file_path, std::ios_base::binary);

    if (input_stream.fail())
    {
        throw std::runtime_error("Failed to open file");
    }

    std::stringstream buffer;
    buffer << input_stream.rdbuf();

    return buffer.str();
}