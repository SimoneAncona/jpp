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
}
```