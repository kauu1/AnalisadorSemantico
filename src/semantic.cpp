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

void update_ex_list_last(struct semantic *s, std::string resultType){
    if(s->id_expression.size() > 2){
        s->id_expression.pop_back();
        s->id_expression.pop_back();
        s->id_expression.push_back(resultType);
    }
}

void update_ex_list(struct semantic *s){
    while(s->id_expression.size() > 2){

        if(s->id_expression[s->id_expression.size()-1].find("integer") != std::string::npos &&
        s->id_expression[s->id_expression.size()-2].find("integer") != std::string::npos){
            update_ex_list_last(s, "integer");

        }else if(s->id_expression[s->id_expression.size()-1].find("real") != std::string::npos &&
        s->id_expression[s->id_expression.size()-2].find("real") != std::string::npos){
            update_ex_list_last(s, "real");

        }else if(s->id_expression[s->id_expression.size()-1].find("integer") != std::string::npos &&
        s->id_expression[s->id_expression.size()-2].find("real") != std::string::npos){
            update_ex_list_last(s, "real");

        }else if(s->id_expression[s->id_expression.size()-1].find("real") != std::string::npos &&
        s->id_expression[s->id_expression.size()-2].find("integer") != std::string::npos){
            update_ex_list_last(s, "real");

        }else if(s->id_expression[s->id_expression.size()-1] == s->id_expression[s->id_expression.size()-2]){
            update_ex_list_last(s, s->id_expression[s->id_expression.size()-1]);

        }else{
            std::cerr << "ERRO: incompatible types" << std::endl;
        }
    }
}

int check_and_clean_types_remaining(struct semantic *s){
    if(s->id_expression.size() < 2){
        s->id_expression.clear();
        return 1;
    }

    if(s->id_expression[0] == s->id_expression[1]){
        s->id_expression.clear();
        return 1;
    }

    if(s->id_expression[0] == "real" && s->id_expression[1] == "integer"){
        s->id_expression.clear();
        return 1;
    }

    if(s->id_expression[0] == "integer" && s->id_expression[1] == "real"){
        s->id_expression.clear();
        return 0;
    }

    s->id_expression.clear();
    return 0;
}

