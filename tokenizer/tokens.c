
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "../vector/vect.h"
#include "tokens.h"

/** Is the character whitespace? */
static int is_whitespace(char ch)
{
    return ch == ' ' || ch == '\t' || ch == '\n';
}

/** Is the character one of the single-character special tokens? */
static int is_special(char ch)
{
    return ch == '(' || ch == ')' || ch == '<' || ch == '>' ||
           ch == ';' || ch == '|';
}

/** Add the substring input[start, start + len) to the vector as a token. */
static void add_substring(vect_t *tokens, const char *start, size_t len)
{
    char *tok = malloc(len + 1);
    assert(tok != NULL);
    memcpy(tok, start, len);
    tok[len] = '\0';

    vect_add(tokens, tok); /* the vector stores its own copy */
    free(tok);
}

/** Split the input string into a vector of tokens. */
vect_t *tokenize(const char *input)
{
    assert(input != NULL);

    vect_t *tokens = vect_new();
    assert(tokens != NULL);

    size_t i = 0;
    while (input[i] != '\0')
    {
        char ch = input[i];

        if (is_whitespace(ch))
        {
            /* Whitespace only separates tokens; skip it. */
            i++;
        }
        else if (is_special(ch))
        {
            /* Each special character is a token on its own. */
            add_substring(tokens, &input[i], 1);
            i++;
        }
        else if (ch == '"')
        {
            /* Quoted string: take everything up to the closing quote (or the end of input if the quote is never closed). */
            i++; /* skip opening quote */
            size_t start = i;
            while (input[i] != '\0' && input[i] != '"')
            {
                i++;
            }
            add_substring(tokens, &input[start], i - start);
            if (input[i] == '"')
            {
                i++; /* skip closing quote */
            }
        }
        else
        {
            size_t start = i;
            while (input[i] != '\0' && !is_whitespace(input[i]) &&
                   !is_special(input[i]) && input[i] != '"')
            {
                i++;
            }
            add_substring(tokens, &input[start], i - start);
        }
    }

    return tokens;
}