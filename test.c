#include <stdio.h>
#include <string.h> 
#include <stdlib.h>

//int main (int argc, const char * argv[]) {
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

    char * pair;
    char * key;
    double value;

    /*
    if ( QueryString && strlen(QueryString) > 0 ) {

        pair = strtok(QueryString, "&");
        free(pair);
        //while (pair) {

        //    printf("pair: %s\n",pair);
        //}

        /*
        while (pair) {
            key = (char *) malloc(strlen(pair)+1);
            sscanf(pair, "%[^=]=%lf", key, &value);
            if ( !strcmp(key, "lat") ) {
                lat = value;
            } else if (!strcmp(key, "lng") ) {
                lng = value;
            }
            free(key);
            pair = strtok((char *)0, "&");
        }
        */
    /*
    }
    */

    free(Method);
    free(QueryString);

    return 0;
/*
    len++;
    //int *postdata;
    //postdata = malloc(len + 1);

    //if (!postdata) { 
    //    printf("Error: no data\n");
    //    return(1);
    //}

    //fgets(postdata, len + 1, stdin);
    /* work with postdata */
/*
    printf("<table frame='box' rules='all' cellpadding='5' >\n<tr><td>data... bytes = %d</td>\n<td>", len );

    int i = 0;

    for ( i = 0 ; i < len ; ++i ) { 
        char c = fgetc(stdin) ; 
        if (c=='\n') printf("<br />") ; 
        else printf( "%c", c ) ; 
    }

    printf("</td>\n</tr>\n") ; 
    printf("</table>\n</body>\n");
    printf("</html>\n");
    fflush(stdout) ; 

    //free(postdata);
*/
    return 0;
}
