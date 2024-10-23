#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define MAX_VARS 100
#define MAX_VAR_NAME 50

typedef struct
{
    char name[MAX_VAR_NAME];
    int value;
} Variable;

Variable vars[MAX_VARS];
int var_count = 0;

// permet de trouver la valeur d'une variable via son nom
int get_variable_value(const char *name)
{
    for (int i = 0; i < var_count; i++)
    {
        if (strcmp(vars[i].name, name) == 0)
        {
            return vars[i].value;
        }
    }
    return 0;
}

//enleve les espace 
void remove_spaces(char *str)
{
    char *i = str;
    char *j = str;
    while (*j != 0)
    {
        *i = *j++;
        if (*i != ' ')
        {
            i++;
        }
    }
    *i = 0;
}

// permet de créer une variable ou de la modifier si elle est déjà déclarer 
void set_variable(const char *name, int value)
{
    char cleaned_name[MAX_VAR_NAME];
    strncpy(cleaned_name, name, MAX_VAR_NAME - 1);
    cleaned_name[MAX_VAR_NAME - 1] = '\0';
    remove_spaces(cleaned_name);
    for (int i = 0; i < var_count; i++)
    {
        if (strcmp(vars[i].name, cleaned_name) == 0)
        {
            vars[i].value = value;
            return;
        }
    }
    strncpy(vars[var_count].name, cleaned_name, MAX_VAR_NAME - 1);
    vars[var_count].name[MAX_VAR_NAME - 1] = '\0';
    vars[var_count].value = value;
    var_count++;
}

// fonction ou les calcules se passe systeme de pile pour gérer les paranthèse en gros on
// faire tous les calcules dans les paranthèse les mettres dans un tableau 
// ensuite on va lire notre tableau (désenpiler) en regardant les opérateur entre les ()
// et les effectué, puis on additionne tous les résultat 
int evaluate_expression(const char *expression)
{
    int values[50];
    char ops[50];
    int value_top = -1;
    int op_top = -1;

    //je lis jusqu au bout 
    for (size_t i = 0; i <= strlen(expression); i++)
    {
        char c = expression[i];

        // on recup nos nombres et les ajoutes a notre tableau values
        if (isdigit(c))
        {
            int value = 0;
            while (isdigit(c))
            {
                value = value * 10 + (c - '0');
                c = expression[++i];
            }
            values[++value_top] = value;
            --i;
        }
        else if (isalpha(c))
        {
            // gestion des variables
            char buffer[MAX_VAR_NAME];
            int j = 0;
            // on boucle jusqu'a ca qu'on recupere le nom complet de la variable 
            while (isalnum(c) && j < MAX_VAR_NAME - 1)
            {
                buffer[j++] = c;
                c = expression[++i];
            }
            buffer[j] = '\0';                                 
            values[++value_top] = get_variable_value(buffer); // on ajoute notre variable a values (on empile)
            --i;
        }
        else if (c == '+' || c == '-' || c == '*' || c == '/')
        {
        while (op_top != -1 && ((ops[op_top] == '*' || ops[op_top] == '/') || ((ops[op_top] == '+' || ops[op_top] == '-') && (c == '+' || c == '-'))))
            {
                //1er groupe
                int b = values[value_top--]; // on recup nos valeurs en haut de la pile
                int a = values[value_top--]; // pareil value_top-- pour depiler a chaque tour
                char op = ops[op_top--]; // on recup nos opérateur en haut de la pile 
                if (op == '+')
                    values[++value_top] = a + b;
                if (op == '-')
                    values[++value_top] = a - b;
                if (op == '*')
                    values[++value_top] = a * b;
                if (op == '/')
                {
                    if (b != 0)
                        values[++value_top] = a / b;
                    else
                    {
                        printf("Erreur : Division par zéro.\n");
                        return 0;
                    }
                }
            }
            ops[++op_top] = c;
        }
        else if (c == '(') 
        {
            ops[++op_top] = c; // on ajoute les parenthèse a notre pile permattant de gérer des priorité
        }
        else if (c == ')') // jusqu'a ce quelle soit fermé
        {
            while (op_top != -1 && ops[op_top] != '(') // on fait nous calcule pour les valeurs dans les paranthèse
            {
                //2eme groupe
                int b = values[value_top--];
                int a = values[value_top--];
                char op = ops[op_top--];
                if (op == '+')
                    values[++value_top] = a + b;
                if (op == '-')
                    values[++value_top] = a - b;
                if (op == '*')
                    values[++value_top] = a * b;
                if (op == '/')
                {
                    if (b != 0)
                        values[++value_top] = a / b;
                    else
                    {
                        printf("Erreur : Division par zéro.\n");
                        return 0;
                    }
                }
            }
            op_top--;
        }
    }
    while (op_top != -1) // on fait nos dernière opération 
    {
        //3eme groupe
        int b = values[value_top--];
        int a = values[value_top--];
        char op = ops[op_top--];
        if (op == '+')
            values[++value_top] = a + b;
        if (op == '-')
            values[++value_top] = a - b;
        if (op == '*')
            values[++value_top] = a * b;
        if (op == '/')
        {
            if (b != 0)
                values[++value_top] = a / b;
            else
            {
                printf("Erreur : Division par zéro.\n");
                return 0;
            }
        }
    }
    // en gros le premier groupes va gérer l'expression (total) le deuxième les priorithès avec les parenthèses ensuite 
    // dans le 3eme on effctue les dernière opèration 
    return values[value_top]; // resultat final
}

void interpret(char *input)
{
    char *delim = "=";
    char input_copy[256];
    strncpy(input_copy, input, sizeof(input_copy) - 1);
    input_copy[sizeof(input_copy) - 1] = '\0';

    char *affectornot = strchr(input, '=');
    char *token = strtok(input_copy, delim);
    if (affectornot != NULL)
    {
        char var_name[MAX_VAR_NAME];
        strncpy(var_name, token, MAX_VAR_NAME - 1);
        var_name[MAX_VAR_NAME - 1] = '\0';
        token = strtok(NULL, delim);
        if (token != NULL) // dans le cas ou c'est une assigantion par exemple x = 1 ou x = 1 + 1
        { 
            // on essaye d'enlever le max d'espace entre les valeurs
            char *value_token = token;
            while (isspace(*value_token))
            {
                value_token++;
            }
            char *end = value_token + strlen(value_token) - 1;
            while (end > value_token && isspace(*end))
            {
                end--;
            }
            end[1] = '\0';
            //  printf("%s", value_token);
            int evaluated_value = evaluate_expression(value_token); 
            set_variable(var_name, evaluated_value);
            printf("%s = %d\n", var_name, evaluated_value);
        }
    }
    else
    {
        // parti pour le print
        char *str = malloc(sizeof(char) * strlen(input));
        char *var = malloc(sizeof(char) * strlen(input));
        int i = 0;
        int compt_var = 0;
        while (input[i] != '(')
        {
            str[i] = input[i];
            i++;
        }
        str[i] = '\0';
        i++;
        while (input[i] != ')')
        {
            var[compt_var] = input[i];
            compt_var++;
            i++;
        }
        var[compt_var] = '\0';

        if (strcmp(str, "print") == 0)
        {
            int value = get_variable_value(var);
            if (value != 0)
            {
                printf("|print(%s) = %i|\n", var, value);
            }
            else
            {
                printf("|print(%s) = %i|\n", var, evaluate_expression(var));
            }
        }
        free(str);
        free(var);
    }
}

int main(int ac, char **av)
{
    // printf("%i", ac);
    // compiler avec un fichier excecuter ./mon_language fichier
    if (ac == 2)
    {
        FILE *file = fopen(av[1], "r");
        if (file == NULL)
        {
            perror("Erreur lors de l'ouverture du fichier");
            return EXIT_FAILURE;
        }

        char input[100];
        while (fgets(input, sizeof(input), file) != NULL)
        {
            input[strcspn(input, "\n")] = '\0'; // modifier la \n par \0
            interpret(input);
        }
        fclose(file);
    }
    else if (ac == 3)
        // utiliser en ligne de commande il faut 2 argument n'importe lesquelle:par exemple = ./mon_language x a  
    {
        char input[300];

        printf("Entrez vos commandes:\n");
        while (strcmp(input, "exit") != 0)
        {
            printf("> ");
            if (fgets(input, sizeof(input), stdin) == NULL)
            {
                break;
            }
            input[strcspn(input, "\n")] = '\0'; // modifier la \n par \0
            interpret(input);
        }
    }
    else
    {
        // juste lancer l'executable sans argument ./mon_language
        interpret("coucou = 2");
        interpret("n = 2");
        interpret("m = 10");
        interpret("p = 5");
        interpret("v = 2");
        interpret("w = 10");
        interpret("i = 5");
        interpret("c = 10");
        interpret("z = 6 / 2 * (v + w)"); // 36
        interpret("x = 10");              // x = 10
        interpret("y = x + 5");           // y = 15
        interpret("print(y)");            // 15
        interpret("a = (3 + 4 * 2 * 2 + 4 + x + y) * coucou");
        interpret("b = (1 + 2) * (2 * 3)");
        interpret("q = (10 / 2) * (2 * 5)");
        interpret("print(a)"); // 48 * coucou 
        interpret("print(b)"); // 18
        interpret("print(z)"); // 36
        interpret("print(q)"); // 36
        // Dans votre fonction main()
        interpret("x = 10");
        interpret("y = x + 5");
        interpret("print(y)");              // Doit afficher 15
        interpret("a = 3 + 4 * 2 / 2");     // Doit donner a = 11
        interpret("b = (1 + 2) * (3 - 1)"); // Doit donner b = 6
        interpret("print(a)");              // Doit afficher 11
        interpret("print(b)");              // Doit afficher 6
        interpret("print(1 + 1)");          // Doit afficher 6
    }
    return 0;
}
