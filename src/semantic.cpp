#include <iostream>
#include "semantic.h"

void print_semantic_id_list(struct semantic *s){
    
    for(size_t i = 0; i < s->identifier_list.size(); i++){
        std::cout << i << ": name: " << s->identifier_list[i].name << ", type: " << s->identifier_list[i].i_type << " , scope: " << s->identifier_list[i].scope << std::endl;
    }
}

int check_id(struct semantic *s, struct identifier id){

    if(id.name.find("&") != std::string::npos){
        return 0;
    }

    for(size_t i = 0; i < s->identifier_list.size(); i++){
        
        if(s->identifier_list[i].name.find(id.name) != std::string::npos && s->identifier_list[i].scope == id.scope){
            return 1;
        }

    }
    return 0;
}

void push_identifier(struct semantic *s, std::string id_name, std::string type, int scope){

    s->current_identifier.name = id_name;
    s->current_identifier.i_type = type;
    s->current_identifier.scope = scope;

    if(check_id(s, s->current_identifier)){
        std::cerr << "ERRO: identifier " << s->current_identifier.name << " already declared" << std::endl;
        return;
    }

    s->identifier_list.push_back(s->current_identifier);
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