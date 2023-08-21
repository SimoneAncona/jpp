#include "jpp.hh"
#include <fstream>
#include <sstream>
#include <iostream>
#include <ctime>

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

        auto children = json.get_children();
        for (auto child : children)
        {
            std::cout << child.second.to_string() << std::endl;
        }
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

        Jpp::Json e2;
        std::string large_json = read_string_from_file("json/large.json");
        std::string e2_json = read_string_from_file("json/e2.json");
        time_t t1, t2;

        std::cout << "started parse loop test" << std::endl;
        t1 = time(0);
        for (int i = 0; i < 1'000; i++)
        {
            e2.parse(e2_json);
        }
        t2 = time(0);
        std::cout << t2 - t1 << "s" << std::endl;

        std::cout << "started large json test" << std::endl;
        t1 = time(0);
        e2.parse(large_json);
        t2 = time(0);
        std::cout << t2 - t1 << "s" << std::endl;

        std::cout << "started large json serialization test" << std::endl;
        t1 = time(0);
        e2.to_string();
        t2 = time(0);
        std::cout << t2 - t1 << "s" << std::endl;

        std::cout << "started large array access loop test" << std::endl;
        t1 = time(0);
        for (int i = 0; i < 1'000; i++)
        {
            auto x = e2[i];
        }
        t2 = time(0);
        std::cout << t2 - t1 << "s" << std::endl;

        std::cout << "started for each loop and serialization test" << std::endl;
        t1 = time(0);
        for (auto c : e2)
        {
            c.second.to_string();
        }
        t2 = time(0);
        std::cout << t2 - t1 << "s" << std::endl;

        Jpp::Json literal_array = l_array{1, "wow", 5, 10.234, "hello"};
        std::cout << literal_array.to_string() << std::endl;

        Jpp::Json literal_object = l_object{{"name", "Franz"}, {"surname", "Kafka"}, {"birth", 1883}};
        std::cout << literal_object.to_string() << std::endl;
    }
    catch (const std::exception e)
    {
        std::cout << e.what() << std::endl;
    }
    system("PAUSE");
    return 0;
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