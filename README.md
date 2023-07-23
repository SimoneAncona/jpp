# JPP
A JSON parser for C++

# Tutorial

## Introduction

This library allows you to parse JSON strings

## Step 1

In this step we will see how to parse a simple JSON string

```c++
#include "jpp.hh"
#include <iostream>

int main(int argc, char **argv)
{
    Jpp::Json json;     // my json object
    json.parse("{ 'name': 'Simon', 'surname': 'Red', 'age': 30 }"); // parse a json string

    std::cout << json.to_string() << std::endl; // expected output: {"age":30.000000, "name":"Simon", "surname":"Red"}

    return 0;
}
```

## Step 2

Now we will see how to access to our json object

```c++
#include "jpp.hh"
#include <iostream>

int main(int argc, char **argv)
{
    Jpp::Json json;
    json.parse("{\
        'name': 'Andrew',\
        'surname': 'Smith',\
        'address': 'Bourbon Street',\
        'friends': [\
            {'name': 'Luca', 'age': 23},\
            {'name': 'Frank', 'age': 20},\
            {'name': 'Andrea', 'age': 67}\
        ]\
    }");

    std::cout << any_cast<std::string>(json["name"].get_value()) << std::endl;  // expected output: Andrew
    std::cout << any_cast<double>(json["friends"][0]["age"].get_value()) << std::endl;  // expected output: 23

    return 0;
}
```

## Step 3

In this step we will see how to create a JSON object from zero

```c++
#include "jpp.hh"
#include <iostream>

int main(int argc, char **argv)
{
    Jpp::Json car;
    Jpp::Json car_collection;
    car["brand"] = "Brand1";
    car["age"] = 10;
    car["model"] = "Model1";

    car_collection["favoriteCar"] = car;
    std::cout << car_collection.to_string() << std::endl;    // expected output: {"favoriteCar":{"age":10.000000, "brand":"Brand1", "model":"Model1"}}
    return 0;
}
```

## Step 4

In this final test we will see how to create a JSON object from literals

```c++
#include "jpp.hh"
#include <iostream>

int main(int argc, char **argv)
{
    Jpp::Json my_object = l_object{
        {"title", "Der Process"},
        {"year", 1925},
        {"author": "Franz Kafka"}
    };

    Jpp::Json my_array = l_array{0, 1, 2, 10, 123.43, 2322.111};
    return 0;
}
```
