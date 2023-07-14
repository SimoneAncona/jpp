# JPP
A JSON parser for C++

# Tutorial

## Introduction

This library allows you to parse JSON strings

## Step 1

In this step we will see how to parse a simple JSON string

```c++
#include "json.hh"
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
#include "json.hh"
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