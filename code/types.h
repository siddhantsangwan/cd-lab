#define FOREACH_TOKEN(TOKEN) \
        TOKEN(T_IDENTIFIER) \
        TOKEN(T_KEYWORD) \
        TOKEN(T_SPECIAL) \
        TOKEN(T_ARITHMETIC_OPERATOR) \
        TOKEN(T_LOGICAL_OPERATOR) \
        TOKEN(T_ASSIGNMENT) \
        TOKEN(T_CONST) \
        TOKEN(T_EOF)

#define GENERATE_ENUM(ENUM) ENUM, 
#define GENERATE_STRING(STRING) #STRING, 

enum tokenType{
    FOREACH_TOKEN(GENERATE_ENUM)
};

static const char* token_names[] = {
    FOREACH_TOKEN(GENERATE_STRING)
};