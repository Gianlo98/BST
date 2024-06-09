#include "BST.hpp"

bst_duplicated_value_exception::bst_duplicated_value_exception(const std::string &message)
: std::runtime_error(message) { }
