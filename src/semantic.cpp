#include <iostream>
#include "semantic.h"

void print_semantic_id_list(struct semantic *s){
    
    for(int i = 0; i < s->identifier_list.size(); i++){
        std::cout << i << ": name: " << s->identifier_list[i].name << ", type: " << s->identifier_list[i].i_type << " , scope: " << s->identifier_list[i].scope << std::endl;
    }
}