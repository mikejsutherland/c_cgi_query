// c_cgi_query
//   - simplied c based cgi application for dynamically storing and processing http query data
// 
// Copyright 2013 Michael Sutherland
// mike@codesmak.com
//

#include <stdio.h>
#include <string.h> 
#include <stdlib.h>
#include <c_cgi_query.h>

int main (int argc, char * const argv[], char * envp[] ) {

    printf("Content-type: text/plain\n\n");

    char *QueryString;
    int length = 0;

    if ( getenv( "REQUEST_METHOD" ) == 0 ) {

        printf("%s\n", "Error: HTTP method not supplied");
        return 1;
    }
    else {

        // Allocate an initial size for the query string
        QueryString = (char *) calloc(4096, 1);
        if ( QueryString == NULL ) {
            fprintf(stderr, "%s\n", "Unable to allocate memory");
        }

        // POST Method
        if ( strcmp( getenv("REQUEST_METHOD"), "POST") == 0 ) {

            // Get the length
            if ( getenv("CONTENT_LENGTH") == 0 ) {
                printf("%s\n", "Error: Content_Length not supplied.");
                return 1;
            }
            length = atoi(getenv("CONTENT_LENGTH"));
            QueryString = (char *) realloc(QueryString, (length+1));

            fread(QueryString , length, 1, stdin);
        }
        // GET Method
        else if ( strcmp( getenv("REQUEST_METHOD"), "GET") == 0 ) {

            length = strlen(getenv("QUERY_STRING"));
            QueryString = (char *) realloc(QueryString, (length+1));

            strncpy(QueryString, getenv("QUERY_STRING"), length+1);
        }

    }

    printf("The query string is: %d chars long.\n", length);
    printf("Query string: %s\n", QueryString);
    printf("--------------------------------------------------------\n");

    // Instantiate the query data object
    parameters qs;
    init_parameters(&qs);

    // Tokenize the query string and store it
    parameterize_querystring(QueryString, &qs);

    // Clean up
    free(QueryString);

    // ***************************************
    // Testing
    // *************************************** 
    testing(&qs);

    // Clean up
    free_parameters(&qs);
    fflush(stdout);

    return 0;
}

void parameterize_querystring(char *querystring, parameters *qs) {

    // Tokenize the query string
    char *token, *token_ptr;
    token = strtok_r(querystring, "&", &token_ptr);
   
    // Walk through each token pair
    while ( token != NULL ) {

        //printf("token: %s\n", token);

        // Tokenize the first token string
        char *key, *val, *pair_ptr;
        key = strtok_r(token, "=", &pair_ptr);
        val = strtok_r(NULL, "=", &pair_ptr);

        if ( val == NULL ) {
            //printf("key: %s, val: %s\n", key, "");
            add_parameter(qs, key, "");
        }
        else {
            //printf("key: %s, val: %s\n", key, val);
            add_parameter(qs, key, val);
        }

        // Get the next token pair
        token = strtok_r(NULL, "&", &token_ptr);
    }
}

void init_parameters(parameters *qs) {

    // Initialize the parameter object if not already
    qs->length = 0;

    qs->key = (char **) calloc(1, 1);
    qs->val = (char **) calloc(1, 1);

    qs->key[ qs->length ] = (char *) calloc(1, 1);
    qs->val[ qs->length ] = (char *) calloc(1, 1);
}

void add_parameter(parameters *qs, char *key, char *val) {

    // Determine the size required to store the key/val
    int key_s = strlen(key) +1;
    int val_s = strlen(val) +1;

    // Resize the parameter object
    qs->length++;
    qs->key = (char **) realloc(qs->key, qs->length);
    qs->val = (char **) realloc(qs->val, qs->length);

    // Allocate space required to store the key/val
    qs->key[ qs->length ] = (char *) calloc(key_s, 1);
    qs->val[ qs->length ] = (char *) calloc(val_s, 1);

    // Store the key/val
    strncpy(qs->key[ qs->length ], key, key_s);
    strncpy(qs->val[ qs->length ], val, val_s);
}

void get_parameter_value_by_key(parameters *qs, char *key, char *val) {

    // Loop through each key until a match is found
    int i;

    for (i=1; i<=qs->length; i++) {
        if ( !strcmp(qs->key[i], key) ) {
            val = (char *) realloc(val, strlen(qs->val[i])+1);
            strncpy(val, qs->val[i], strlen(qs->val[i]));
            break;
        }
    }
}

void free_parameters(parameters *qs) {

    int i;

    for (i=1; i<=qs->length; i++) {
        free(qs->key[i]);
        free(qs->val[i]);
    }

    free(qs->key);
    free(qs->val);
}

void testing(parameters *qs) {

    // Print out the key/value pairs
    printf("parameters detected: %d\n", qs->length);

    int i;

    for (i=1; i<=qs->length; i++) {
        printf("%d) query object params (key: %s, val: \"%s\")\n", i, qs->key[i], qs->val[i]);
    }

    // Retrieve a single keys value
    char *test = calloc(255, 1);

    get_parameter_value_by_key(qs, "test2", test);

    printf("key 'test2' has value: '%s'", test);
    free(test);
}
