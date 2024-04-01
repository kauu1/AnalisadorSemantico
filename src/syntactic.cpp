#include "syntactic.h"
#include "lexical.h"
#include <iostream>
#include <string>
#include <vector>
#include "semantic.h"

std::vector<std::string> types = {"real", "integer", "boolean"};

bool in_array(std::string &value, std::vector<std::string> &array)
{
    return std::find(array.begin(), array.end(), value) != array.end();
}
void syntactic_init(struct syntactic *synt, std::string file_path, int option){
    std::vector<lexical> aux;

    if (option == 1) {
        aux = lexical_analyser_AFD(file_path);

    }else if (option == 2){
        aux = lexical_analyser_ReGex(file_path);
    }
    synt->lexical_analyser_results = aux;
    synt->position = 0;
    synt->errors = 0;

}

void next(struct syntactic* synt){
    if(synt->position < synt->lexical_analyser_results.size() - 1){
        synt->position++;
    }
}

void factor(struct syntactic* synt){
    
    if(synt->lexical_analyser_results[synt->position].token == "true" || synt->lexical_analyser_results[synt->position].token == "false"){
        push_expression_list(&(synt->s_analyser), "boolean");
        next(synt);
    }else if(synt->lexical_analyser_results[synt->position].type == Identifier){

        synt->s_analyser.current_identifier.name = synt->lexical_analyser_results[synt->position].token;
        synt->s_analyser.current_identifier.scope = synt->s_analyser.scope;

        if(!(check_id(&(synt->s_analyser), synt->s_analyser.current_identifier))){
            std::cerr << "Line "<< synt->lexical_analyser_results[synt->position].line <<" ERRO: undefined identifier " << synt->s_analyser.current_identifier.name << std::endl;
        }
        
        push_expression_list(&(synt->s_analyser), synt->s_analyser.current_identifier.name);
        next(synt);

        if(synt->lexical_analyser_results[synt->position].token.find("(") != std::string::npos){
            expression_list(synt);

            if(synt->lexical_analyser_results[synt->position].token.find(")") != std::string::npos){
                next(synt);
            }
            else{
                synt->errors++;
                std::cerr << "Line " << synt->lexical_analyser_results[synt->position].line << " ERRO: Expected \")\"\n";
            }
        }
    }

    if(synt->lexical_analyser_results[synt->position].type == Integer){
        push_expression_list(&(synt->s_analyser), "integer");
        next(synt);
    }
    else if(synt->lexical_analyser_results[synt->position].type == Float){
        push_expression_list(&(synt->s_analyser), "real");
        next(synt);
    }
    else if(synt->lexical_analyser_results[synt->position].type == Boolean){
        push_expression_list(&(synt->s_analyser), "boolean");
        next(synt);
    }

    if(synt->lexical_analyser_results[synt->position].token.find("(") != std::string::npos){
        next(synt);

        expression(synt);

        if(synt->lexical_analyser_results[synt->position].token.find(")") != std::string::npos){
            next(synt);
        }
        else{
            synt->errors++;
            std::cerr << "Line " << synt->lexical_analyser_results[synt->position].line << " ERRO: Expected \")\"\n";
        }
    }

    if(synt->lexical_analyser_results[synt->position].token.find("not") != std::string::npos){
        next(synt);
        factor(synt);
    }
      

}

void term(struct syntactic* synt){
    factor(synt);

    if(synt->lexical_analyser_results[synt->position].type == Multiplicative_operator){
        next(synt);
        term(synt);
    }
}

void simple_expression2(struct syntactic* synt){
    if(synt->lexical_analyser_results[synt->position].token.find("(") == std::string::npos && synt->lexical_analyser_results[synt->position].type == Identifier){
        synt->s_analyser.current_identifier.name = synt->lexical_analyser_results[synt->position].token;
        push_expression_list(&(synt->s_analyser), synt->s_analyser.current_identifier.name);
    }

    if(synt->lexical_analyser_results[synt->position].type == Aditive_operator){
        next(synt);
        term(synt);
        simple_expression2(synt);
    }

}

void simple_expression(struct syntactic* synt){


    if(synt->lexical_analyser_results[synt->position].token.find("+") != std::string::npos
    || synt->lexical_analyser_results[synt->position].token.find("-") != std::string::npos){

        next(synt);
        term(synt);
        simple_expression2(synt);
    }else {
        term(synt);
        simple_expression2(synt);
    }

    update_ex_list(&(synt->s_analyser));
    

}

void expression(struct syntactic* synt){

    simple_expression(synt);

    update_ex_list(&(synt->s_analyser));

    if(synt->lexical_analyser_results[synt->position].type == Relational_operator){
        next(synt);
        simple_expression(synt);
        update_ex_list(&(synt->s_analyser));
        synt->s_analyser.id_expression.pop_back();
        synt->s_analyser.id_expression.pop_back();
        synt->s_analyser.id_expression.push_back("boolean");
        
    }else if(!(check_and_clean_types_remaining(&(synt->s_analyser)))){
        std::cerr << "Line " << synt->lexical_analyser_results[synt->position].line <<" ERRO: incompatible types " << synt->s_analyser.id_expression[0] << " := " << synt->s_analyser.id_expression[1] << std::endl;
    }
    
}

void expression_list2(struct syntactic* synt){

    if(synt->lexical_analyser_results[synt->position].token.find(",") != std::string::npos){
        next(synt);
        expression(synt);
        expression_list2(synt);
    }else if(synt->lexical_analyser_results[synt->position].token.find(")") != std::string::npos){
        next(synt);
        return;
    }else{
        std::cerr << "Line " << synt->lexical_analyser_results[synt->position-1].line << " ERRO: Expected \")\"\n";
    }

}

void expression_list(struct syntactic* synt){

    if(synt->lexical_analyser_results[synt->position].token.find(")") != std::string::npos){
        next(synt);
        return;
    }else {
        expression(synt);
        expression_list2(synt);
    }
}

void command(struct syntactic* synt){
    
    if(synt->lexical_analyser_results[synt->position].type == Identifier){

        
        synt->s_analyser.current_identifier.name = synt->lexical_analyser_results[synt->position].token;
        synt->s_analyser.current_identifier.scope = synt->s_analyser.scope;

        if(!(check_id(&(synt->s_analyser), synt->s_analyser.current_identifier))){
            std::cerr << "Line "<< synt->lexical_analyser_results[synt->position].line <<" ERRO: undefined identifier " << synt->s_analyser.current_identifier.name << std::endl;
        }
        

        next(synt);
        if(synt->lexical_analyser_results[synt->position].token.find(":=") != std::string::npos){
            push_expression_list(&(synt->s_analyser), synt->s_analyser.current_identifier.name);
            next(synt);
            expression(synt);

        }
        else if (synt->lexical_analyser_results[synt->position].token.find("=") != std::string::npos){
            push_expression_list(&(synt->s_analyser), synt->s_analyser.current_identifier.name);
            next(synt);
            synt->errors++;
            std::cerr << "Line " << synt->lexical_analyser_results[synt->position].line << " ERRO: Expected \":\" before \"=\"\n";
            expression(synt);



        }else if(synt->lexical_analyser_results[synt->position].token.find("(") != std::string::npos){

            if(!(check_id_procedure(&(synt->s_analyser), synt->s_analyser.current_identifier))){
            std::cerr << "Line "<< synt->lexical_analyser_results[synt->position].line <<" ERRO: undefined procedure " << synt->s_analyser.current_identifier.name << std::endl;
        }
            next(synt);
            expression_list(synt);
                
        }
        
        if(synt->lexical_analyser_results[synt->position].token.find(";") != std::string::npos){
            
            next(synt);
        }
        else {
            synt->errors++;
            std::cerr << "Line " << synt->lexical_analyser_results[synt->position-1].line << " ERRO: Expected \";\"\n";
        }
    }

    if(synt->lexical_analyser_results[synt->position].token.find("begin") != std::string::npos){
        compost_command(synt);
    }

    if(synt->lexical_analyser_results[synt->position].token.find("if") != std::string::npos){
        next(synt);
        expression(synt);
        if(synt->s_analyser.id_expression[0] != "boolean"){
            std::cerr << "Line " << synt->lexical_analyser_results[synt->position].line <<" ERRO: incompatible type" << std::endl;
        }
        if(!(check_and_clean_types_remaining(&(synt->s_analyser)))){
            std::cerr << "Line " << synt->lexical_analyser_results[synt->position].line <<" ERRO: incompatible types " << synt->s_analyser.id_expression[0] << " := " << synt->s_analyser.id_expression[1] << std::endl;
        }

        if(synt->lexical_analyser_results[synt->position].token.find("then") != std::string::npos){
            next(synt);
        }else {
            synt->errors++;
            std::cerr << "Line " << synt->lexical_analyser_results[synt->position].line << " ERRO: Expected \"then\"\n";
        }

        command(synt);

        if(synt->lexical_analyser_results[synt->position].token.find("else") != std::string::npos){

            next(synt);
            command(synt);
        }
    }

    if(synt->lexical_analyser_results[synt->position].token.find("while") != std::string::npos){
        next(synt);
        expression(synt);

        if(synt->s_analyser.id_expression[0] != "boolean"){
            std::cerr << "Line " << synt->lexical_analyser_results[synt->position].line <<" ERRO: incompatible type" << std::endl;
        }
        if(!(check_and_clean_types_remaining(&(synt->s_analyser)))){
            std::cerr << "Line " << synt->lexical_analyser_results[synt->position].line <<" ERRO: incompatible types " << synt->s_analyser.id_expression[0] << " := " << synt->s_analyser.id_expression[1] << std::endl;
        }

        if(synt->lexical_analyser_results[synt->position].token.find("do") != std::string::npos){}
        else{
            synt->errors++;
            std::cerr << "Line " << synt->lexical_analyser_results[synt->position].line << " ERRO: Expected \"do\"\n";
        }

        next(synt);
        command(synt);
    }
}

void command_list2(struct syntactic* synt){

    if(synt->lexical_analyser_results[synt->position].token.find("end") != std::string::npos){
        return;
    }

    command(synt);

    command_list2(synt);
}

void command_list(struct syntactic* synt){

    command(synt);

    if(synt->lexical_analyser_results[synt->position].token.find("end") != std::string::npos){
        return;
    }

    command_list2(synt);
}

void compost_command(struct syntactic* synt){

    if(synt->lexical_analyser_results[synt->position].token.find("begin") != std::string::npos){
        next(synt);
    }
    else {
        synt->errors++;
        std::cerr << "Line " << synt->lexical_analyser_results[synt->position].line << " ERRO: Expected \"begin\"\n";
    }

    if(synt->lexical_analyser_results[synt->position].token.find("end") != std::string::npos){
        next(synt);
        return;
    }
    
    command_list(synt);

    if(synt->lexical_analyser_results[synt->position].token.find("end") != std::string::npos){
        next(synt);
    }else{
        synt->errors++;
        std::cerr << "Line " << synt->lexical_analyser_results[synt->position].line << " ERRO: Expected \"end\"\n";
    }
}

void parameter_list2(struct syntactic* synt){

    if(synt->lexical_analyser_results[synt->position].token.find(",") != std::string::npos){

        next(synt);
        identifier_list(synt);

        if(synt->lexical_analyser_results[synt->position].token.find(":") == std::string::npos){
            synt->errors++;
            std::cerr << "Line " << synt->lexical_analyser_results[synt->position].line << " ERRO: Expected \":\"\n";
        }

        next(synt);

        if(in_array(synt->lexical_analyser_results[synt->position].token, types)){

            push_identifier_list(&(synt->s_analyser), "&", "\0", synt->s_analyser.scope);

            set_types(&(synt->s_analyser),synt->lexical_analyser_results[synt->position].token );

        }else {
            synt->errors++;
            std::cerr << "Line " << synt->lexical_analyser_results[synt->position].line << " ERRO: Expected type\n";
        }

        next(synt);

        parameter_list2(synt); 

    }
}

void parameter_list(struct syntactic* synt){

    identifier_list(synt);

    if(synt->lexical_analyser_results[synt->position].token.find(":") != std::string::npos){

    }else{
        synt->errors++;
        std::cerr << "Line " << synt->lexical_analyser_results[synt->position].line << " ERRO: Expected \":\"\n";
    }

    next(synt);
    if(in_array(synt->lexical_analyser_results[synt->position].token, types)){

        push_identifier_list(&(synt->s_analyser), "&", "\0", synt->s_analyser.scope);

        set_types(&(synt->s_analyser),synt->lexical_analyser_results[synt->position].token );

    }else {
        synt->errors++;
        std::cerr << "Line " << synt->lexical_analyser_results[synt->position].line << " ERRO: Expected type\n";
    }

    next(synt);

    parameter_list2(synt);
}

void arguments(struct syntactic* synt){

    if(synt->lexical_analyser_results[synt->position].token.find("(") != std::string::npos){
        next(synt);
        parameter_list(synt);

        if(synt->lexical_analyser_results[synt->position].token.find(")") == std::string::npos){
            synt->errors++;
            std::cerr << "Line " << synt->lexical_analyser_results[synt->position].line << " ERRO: Expected \")\"\n";
        }

        erase_marks(&(synt->s_analyser));
        
        next(synt);
    }   
}
void subprogram_declaration(struct syntactic *synt){
    if(synt->lexical_analyser_results[synt->position].token.find("procedure") != std::string::npos){
        synt->s_analyser.scope++;

        next(synt);

        if(synt->lexical_analyser_results[synt->position].type == Identifier){

            push_identifier_list(&(synt->s_analyser), synt->lexical_analyser_results[synt->position].token, "procedure", synt->s_analyser.scope-1);

            next(synt);
        }else {
            synt->errors++;
            std::cerr << "Line " << synt->lexical_analyser_results[synt->position].line << " ERRO: Expected IDENTIFIER\n";
        }

        arguments(synt);

        if(synt->lexical_analyser_results[synt->position].token.find(";") != std::string::npos){
            next(synt);
        }
        else {
            synt->errors++;
            std::cerr << "Line " << synt->lexical_analyser_results[synt->position].line << " ERRO: Expected \";\"\n";
        }

        variable_declaration(synt);

        subprograms_declaration(synt);

        compost_command(synt);

        if(synt->lexical_analyser_results[synt->position].token.find(";") != std::string::npos){
            next(synt);
        }
        else {
            synt->errors++;
            std::cerr << "Line " << synt->lexical_analyser_results[synt->position-1].line << " ERRO: Expected \";\"\n";
        }
    }
}

void subprograms_declaration(struct syntactic *synt){
    if(synt->lexical_analyser_results[synt->position].token.find("procedure") != std::string::npos){
        
        subprogram_declaration(synt);

        erase_scope(&(synt->s_analyser));

        subprograms_declaration(synt);
    }
}

void identifier_list2(struct syntactic* synt){
    if (synt->lexical_analyser_results[synt->position].token.find(",") != std::string::npos) {

        next(synt);

        if(synt->lexical_analyser_results[synt->position].type == Identifier){
            
            push_identifier_list(&(synt->s_analyser), synt->lexical_analyser_results[synt->position].token, "\0", synt->s_analyser.scope);

            next(synt);

        }else {
            synt->errors++;
            std::cerr << "Line " << synt->lexical_analyser_results[synt->position].line << " ERRO: Expected IDENTIFIER\n";
        }
        
        identifier_list2(synt);
    }

}

void identifier_list(struct syntactic* synt){
    if(synt->lexical_analyser_results[synt->position].type == Identifier){

        push_identifier_list(&(synt->s_analyser), synt->lexical_analyser_results[synt->position].token, "\0", synt->s_analyser.scope);

        next(synt);

    }else {
        synt->errors++;
        std::cerr << "Line " << synt->lexical_analyser_results[synt->position].line << " ERRO: Expected IDENTIFIER\n";
    }

    identifier_list2(synt);
}



void variable_declaration_list(struct syntactic *synt){
    
    identifier_list(synt);

    if(synt->lexical_analyser_results[synt->position].token.find(":") != std::string::npos){
        next(synt);
    }else{
        synt->errors++;
        std::cerr << "Line " << synt->lexical_analyser_results[synt->position].line << " ERRO: Expected \":\"\n";
    }

    if(in_array(synt->lexical_analyser_results[synt->position].token, types)){
        

        push_identifier_list(&(synt->s_analyser), "&", "\0", synt->s_analyser.scope);
        

        set_types(&(synt->s_analyser), synt->lexical_analyser_results[synt->position].token);

        next(synt);
        
    }else {
        synt->errors++;
        std::cerr << "Line " << synt->lexical_analyser_results[synt->position].line << " ERRO: Expected type\n";
    }

    if(synt->lexical_analyser_results[synt->position].token.find(";") != std::string::npos){
        next(synt);
    }else {
        synt->errors++;
        std::cerr << "Line " << synt->lexical_analyser_results[synt->position].line << " ERRO: Expected \";\"\n";
    }

    if(synt->lexical_analyser_results[synt->position].type == Identifier){
        variable_declaration_list(synt);
    }
    
}

void variable_declaration(struct syntactic *synt){ 
    if(synt->lexical_analyser_results[synt->position].token.find("var") != std::string::npos){
        
        push_identifier_list(&(synt->s_analyser), "&", "\0", synt->s_analyser.scope);

        next(synt);
        variable_declaration_list(synt);
        erase_marks(&(synt->s_analyser));
    }
}

void program(struct syntactic *synt){
    if(synt->lexical_analyser_results[synt->position].token.find("program") != std::string::npos){
        next(synt);
    }else {
        synt->errors++;
        std::cerr << "Line " << synt->lexical_analyser_results[synt->position].line << " ERRO: Expected \"program\"\n";
    }

    if(synt->lexical_analyser_results[synt->position].type == Identifier){
        push_identifier_list(&(synt->s_analyser), synt->lexical_analyser_results[synt->position].token, "program", 0);
        next(synt);
    }else {
        synt->errors++;
        std::cerr << "Line " << synt->lexical_analyser_results[synt->position].line << " ERRO: Expected \"IDENTIFIER\"\n";
    }

    if(synt->lexical_analyser_results[synt->position].token.find(";") != std::string::npos){
        next(synt);
    }else {
        synt->errors++;
        std::cerr << "Line " << synt->lexical_analyser_results[synt->position].line << " ERRO: Expected \";\"\n";
        next(synt);
    }

    variable_declaration(synt);
    subprograms_declaration(synt);
    compost_command(synt);

    if(synt->lexical_analyser_results[synt->position].token.find(".") != std::string::npos){}
    else{
        synt->errors++;
        std::cerr << "Line " << synt->lexical_analyser_results[synt->position].line << " ERRO: Expected \".\"\n";
    }
}