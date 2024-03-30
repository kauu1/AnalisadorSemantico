#include <cstdio>
#include <iostream>
#include "semantic.h"
#include <string>

void print_semantic_id_list(struct semantic *s){
    
    for(size_t i = 0; i < s->identifier_list.size(); i++){
        std::cout << i << ": name: " << s->identifier_list[i].name << ", type: " << s->identifier_list[i].i_type << " , scope: " << s->identifier_list[i].scope << std::endl;
    }
}

void print_semantic_ex_list(struct semantic *s){
    for(size_t i = 0; i < s->id_expression.size(); i++){
        std::cout << i << ": name: " << s->id_expression[i] << std::endl;
    }
}

int check_id(struct semantic *s, struct identifier id){

    if(id.name.find("&") != std::string::npos){
        return 0;
    }

    for(size_t i = 0; i < s->identifier_list.size(); i++){
        
        if(s->identifier_list[i].name.find(id.name) != std::string::npos && s->identifier_list[i].scope == id.scope){
            if(i == 0){
                std::cerr << "ERRO: program name can not be used" << std::endl;
            }
            return 1;
        }

    }
    return 0;
}

int check_id_procedure(struct semantic *s, struct identifier id){

    if(id.name.find("&") != std::string::npos){
        return 0;
    }

    for(size_t i = 0; i < s->identifier_list.size(); i++){
        
        if(s->identifier_list[i].name.find(id.name) != std::string::npos && s->identifier_list[i].scope == id.scope && s->identifier_list[i].i_type == "procedure"){
            return 1;
        }

    }
    return 0;
}

void push_identifier_list(struct semantic *s, std::string id_name, std::string type, int scope){

    s->current_identifier.name = id_name;
    s->current_identifier.i_type = type;
    s->current_identifier.scope = scope;

    if(check_id(s, s->current_identifier)){
        std::cerr << "ERRO: identifier " << s->current_identifier.name << " already declared" << std::endl;
        return;
    }

    s->identifier_list.push_back(s->current_identifier);
}

void push_expression_list(struct semantic *s, std::string id_name){

    s->id_expression.push_back(get_id_type(s, id_name));
}

void set_types(struct semantic *s, std::string type){
    int i_position = s->identifier_list.size() - 2;

    while(s->identifier_list[i_position].name.find("&") == std::string::npos && s->identifier_list[i_position].i_type.find("procedure") == std::string::npos){

        s->identifier_list[i_position].i_type = type;
        i_position--;
    }
}

void erase_marks(struct semantic *s){
     for(size_t i = 0; i < s->identifier_list.size(); i++){
        
        if(s->identifier_list[i].name.find("&") != std::string::npos){
            s->identifier_list.erase(s->identifier_list.begin()+i);
        }
    }
}

std::string get_id_type(struct semantic *s, std::string id_name){
    for(size_t i = 0; i < s->identifier_list.size(); i++){
        
        if(s->identifier_list[i].name.find(id_name) != std::string::npos){
            std::cout << "name: " << s->identifier_list[i].name << std::endl;
            return s->identifier_list[i].i_type;
        }

    }
    return id_name;
}

void erase_scope(struct semantic *s){
    int i_position = s->identifier_list.size() - 1;

    while(s->identifier_list[i_position].scope == s->scope){
        i_position--;
        s->identifier_list.pop_back();
    }
    s->scope--;
}

void update_ex_list(struct semantic *s){

}