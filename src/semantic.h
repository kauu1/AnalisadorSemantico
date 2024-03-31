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
    std::vector<std::string> id_expression;
    struct identifier current_identifier;
    int scope = 0;
};

void print_semantic_id_list(struct semantic *s);

void print_semantic_ex_list(struct semantic *s);

void push_identifier_list(struct semantic *s, std::string id_name, std::string type, int scope);

void push_expression_list(struct semantic *s, std::string id_name);

std::string get_id_type(struct semantic *s, std::string id_name);

int check_id(struct semantic *s, struct identifier id);

int check_id_procedure(struct semantic *s, struct identifier id);

void update_ex_list(struct semantic *s);

void update_ex_list_last(struct semantic *s, std::string resultType);

int check_and_clean_types_remaining(struct semantic *s);

void erase_marks(struct semantic *s);

void erase_scope(struct semantic *s);

void set_types(struct semantic *s, std::string type);