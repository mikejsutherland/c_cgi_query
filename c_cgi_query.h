typedef struct {
    char **key;
    char **val;
    int length;
} parameters;

void parameterize_querystring(char *, parameters *);
void init_parameters(parameters *);
void add_parameter(parameters *, char *, char *);
void get_parameter_value_by_key(parameters *, char *, char *);
void free_parameters(parameters *);
void testing(parameters *);