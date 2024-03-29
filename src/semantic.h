#pragma once

#include <vector>
#include <string>

struct identifier{
    std::string name;
    std::string i_type = "\0";
    int scope = 0;
};

struct semantic{
    std::vector<struct identifier> identifier_list;
    struct identifier current_identifier;
    int scope = 0;
};

void print_semantic_id_list(struct semantic *s);