#include <stdio.h>
#include <string.h> 
#include <stdlib.h>

typedef struct {
    char **key;
    char **val;
    int length;
} parameters;

int main (int argc, char * const argv[], char * envp[] ) {

    printf("Content-type: text/plain\n\n");

    char *QueryString;
    char *Method;
    int length = 0;

    if ( getenv( "REQUEST_METHOD" ) == 0 ) {

        printf("%s\n", "Error: HTTP method not supplied");
        return 1;
    }
    else {

        // Allocate an initial size for the query string
        QueryString = (char *) malloc(4096);
        Method = (char *) malloc(5);

        // POST Method
        if ( strcmp( getenv("REQUEST_METHOD"), "POST") == 0 ) {

            // Get the length
            if ( getenv("CONTENT_LENGTH") == 0 ) {
                printf("%s\n", "Error: Content_Length not supplied.");
                return 1;
            }
            length = atoi(getenv("CONTENT_LENGTH"));
            QueryString = (char *) realloc(QueryString, (length+1));
            Method = "POST";

            fread(QueryString , length, 1, stdin);
        }
        // GET Method
        else if ( strcmp( getenv("REQUEST_METHOD"), "GET") == 0 ) {

            length = strlen(getenv("QUERY_STRING"));
            QueryString = (char *) realloc(QueryString, (length+1));
            Method = "GET";

            strncpy(QueryString, getenv("QUERY_STRING"), length+1);
        }

    }

    printf("The %s query string is: %d chars long.\n", Method, length);
    printf("Query string: %s\n", QueryString);

    // Parse QUERY STRING
    char *token_ptr;
    char *token;
    int x = 0;

    // Instantiate the query data structure
    parameters query;
    query.length = 0;
    query.key = 0;
    query.val = 0;

    // Tokenize the query string
    token = strtok_r(QueryString, "&", &token_ptr);
   
    /* walk through other tokens */
    while ( token != NULL ) {

        // Parse fieldset pairs
        char *pair_ptr;
        char *pair = malloc(4096);

        // Resize and reallocate the query structure
        x++;
        int FieldSetSize =  (x * sizeof(char*));
        query.key = realloc(query.key, FieldSetSize);
        query.val = realloc(query.val, FieldSetSize);
        query.key[x] = (char *) malloc(1);
        query.val[x] = (char *) malloc(1);

        // Reallocate and copy the token
        pair = (char*) realloc(pair, strlen(token));
        strncpy(pair, token, strlen(token));
        query.length = x;

        // Tokenize the first token string
        pair = strtok_r(token, "=", &pair_ptr);

        // Reallocate and store the key
        query.key[x] = (char*) realloc(query.key[x], strlen(pair));
        strncpy(query.key[x], pair, strlen(pair));

        while ( pair != NULL ) {

            // Retrieve the value
            pair = strtok_r(NULL, "=", &pair_ptr);

            if ( pair != NULL ) {
                // Reallocate and store the value
                query.val[x] = (char*) realloc(query.val[x], strlen(pair));
                strncpy(query.val[x], pair, strlen(pair));
            }
        }
        printf("query object params (key: %s, val: \"%s\") for position %d\n", query.key[x], query.val[x], x);

        // Get the next token pair
        free(pair);
        token = strtok_r(NULL, "&", &token_ptr);
    }

    free(Method);
    free(QueryString);

    return 0;
}
