#ifndef _MOSH_DEBUG_H
#define _MOSH_DEBUG_H

#include <string>
#include <vector>
#include <utility>
#include <iostream>
#include "mosh.h"
#include "mosh_parser.h"
#include "mosh_ast_node.h"
#include "mosh_command.h"
#include "mosh_pipe.h"

std::string token_label_str(token_label label);

void debug_tokens(std::vector<std::string> tokens);
void debug_labeled_tokens(std::vector<std::pair<std::string, token_label>> labeled_tokens);
void debug_ast_list(std::vector<mosh_ast_node *> ast_list);

#endif