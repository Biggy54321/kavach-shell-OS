#ifndef _STR_UTIL_H_
#define _STR_UTIL_H_


#define IS_WHITESPACE(ch)                       \
    ({                                          \
        ((ch) == ' ') || ((ch) == '\t');        \
    })

#define IS_INPUT_REDIREC_OP(ch)                 \
    ({                                          \
        ((ch) == '<');                          \
    })

#define IS_OUTPUT_REDIREC_OP(ch)                \
    ({                                          \
        ((ch) == '>');                          \
    })

#define IS_PIPE_OP(ch)                          \
    ({                                          \
        ((ch) == '|');                          \
    })

#define IS_NULL(ch)                             \
    ({                                          \
        ((ch) == '\0');                         \
    })

#define IS_SINGLE_QUOTE(ch)                     \
    ({                                          \
        ((ch) == '\'');                         \
    })

#define IS_DOUBLE_QUOTE(ch)                     \
    ({                                          \
        ((ch) == '"');                          \
    })

#define IS_BACKGROUND_OP(ch)                    \
    ({                                          \
        ((ch) == '&');                          \
    })

#define IS_VALID_IDENTIFIER(ch)                     \
    ({                                              \
        (((ch) >= 'a') && ((ch) <= 'z')) ||         \
        (((ch) >= 'A') && ((ch) <= 'Z')) ||         \
        (((ch) >= '0') && ((ch) <= '9')) ||         \
        ((ch) == '_')                    ||         \
        ((ch) == '-')                    ||         \
        ((ch) == '.')                    ||         \
        ((ch) == '~')                    ||         \
        ((ch) == ',')                    ||         \
        ((ch) == '/');                              \
    })

#endif
