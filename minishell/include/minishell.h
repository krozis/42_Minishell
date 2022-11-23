#ifndef MINISHELL_H
# define MINISHELL_H

# define RED "\1\033[1;31m\2"
# define NORMAL "\1\x1b[0m\2"
# define NAME "minishell> "

# define NB_CHAR_VALID 13
# define VALID_CHAR "=/|<>.'\" $?-_"
# define CMD_CHAR " ><|"
# define C_CHECK_CHAR "\'\"$ "
# define SPECIAL_CHAR "|<>"

# define SIMPLE 0
# define DOUBLE 1

# define DEFAULT 1
# define CMD 2
# define ARG_FILE_IN 3
# define ARG_FILE_OUT_OVER 4
# define ARG_FILE_OUT_APP 5
# define LIMITOR 6
# define PIPE 7 /* | */
# define FILE_IN 8 /* < */
# define FILE_OUT_OVER 9 /* > */
# define FILE_OUT_APP 10 /* >> */
# define HERE_DOC 11 /* << */

# define CMD_BEGIN 0
# define CMD_END 1
# define CMD_MIDDLE 2
# define CMD_FILE_IN 3
# define CMD_FILE_IN_END 4
# define CMD_FILE_OUT 5
# define CMD_FILE_OUT_END 6

# define CD 0
# define ECHO 1

# define ERR_ENV_MALLOC "Error Malloc env\n"
# define ERR_MAIN_MALLOC "Error Malloc Main struct\n"
# define ERR_LEXER "Error Lexer\n"
# define ERR_NB_CHAR "ERROR NB_CHAR\n"

# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <stdio.h>
# include <stdbool.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <sys/wait.h>
# include "../libft/includes/libft.h"

/*
 * @brief Environnement variable structure
 * @param var: (char *) name of the environnement variable
 * @param value: (char *) what the var contains
 * @param next: (t_env *) address to the next t_env variable
*/
typedef struct s_env
{
	char			*var;
	char			*value;
	struct s_env	*next;
}				t_env;

//Value for AST/LST_CMD
typedef union s_value
{
	char	**cmd;
	char	*oper;
}				t_value;

/*
 * @brief t_lst_parser - double way list of Commands
 * @param next: s_lst_cmd *
 * @param prev: s_lst_cmd *
 * @param value: t_value
 * @param type: int
*/
typedef struct s_lst_cmd
{
	struct s_lst_cmd	*next;
	struct s_lst_cmd	*prev;
	t_value				value;
	int					type;

}				t_lst_parser;

//LEXER TOKEN
typedef struct s_list
{
	char			*content;
	struct s_list	*next;
	int				type;
}				t_list;

typedef struct s_char_check
{
	int		last_simple_q;
	int		last_double_q;
	char	*char_valid;
	int		error;
}				t_char_check;

typedef struct s_struct
{
	int				i;
	int				is_arg;
	t_list			*lst;
	t_char_check	char_check;
}		t_struct;

/*
 * @file main/signal.c
*/

void	global_signals_handler(int argc, char **argv);

/**
**		lexer/lexer.c
**/

int		ms_main_lexer(char *str, t_env *st);

/**
**		main/env_create.c
**/

int		ms_create_env(char **envp, t_env **st);
t_env	*ms_lst_new_env(char *var, char *value);
int		ms_lst_add_back_env(t_env **st, t_env *new);

/**
**		main/env_handler.c
**/

void	set_env(t_env *env);
t_env	*get_env(void);
void	free_env(t_env *env);


/*
**		lexer/char_check_special.c
*/

int		ms_exclude_special(char c, t_struct *main);

/*
**		lexer/remove_quotes.c
*/

int		ms_main_remove_quotes(t_list **lst);

/*
**		lexer/expand_var.c
*/

int		ms_main_replace_env(t_list **lst, t_env *st);

/**
**		lexer/count.c
**/

int		ms_start_read_var(char *str, int index);
int		ms_lenght_read_var(char *str, int index);
int		ms_count_char(char *str);
int		ms_count_nb_quotes(char *str);
int		ms_count_remove_quotes(char *str);

/*
**		lexer/utils.c
*/

int		ms_strjoin_3(char *dest, char *str, int start);
char	*ms_strjoin_2(char *str1, char *str2);
char	*ms_strjoin_c(char *str, char c);
int		ms_have_space(char *str);

/*
**		lexer/utils_2.c
*/

char	*ms_strcpy_2(char *str);
int		ms_strlen(const char *str);

/*
**		lexer/read_char.c
*/

void	ms_read_special(char *str, t_struct *main);
void	ms_read_cmd(char *str, t_struct *main);
int		ms_count_read_quotes(char *str, int start);

/*
**		lexer/unwanted.c
*/

char	*ms_check_quotes(char *str, t_struct *main);
char	*ms_remove_special(char *str, t_struct *main, int i);

/*
**		lexer/lst.c
*/

int		ms_lst_size(t_list *lst);
char	*ms_get_lst_str_index(t_list *lst, int index);
int		ms_lstadd(t_list **lst, t_list *new);
char	*ms_find_var(char *str, t_env *st);

/*
**		lexer/lst_2.c
*/

t_list	*ms_lst_new_join(char *content, int type);
t_list	*ms_lst_new(char *content, int type);
t_list	*ms_lst_last(t_list *lst);
int		ms_lstadd_back(t_list **lst, t_list *new);

/*
**		lexer/lst_print.c
*/



/*
**		lexer/free.c
*/

void	ms_free_lst(t_list *lst);
void	ms_free_all(t_struct *main, t_env *st);
void	ms_free_parse(t_lst_parser **lst_parser);

/*
**		lexer/remove_spaces.c
*/

int		ms_remove_spaces(t_list **lst);

/*
**		lexer/set_type_cmd.c
*/

int		ms_set_type_cmd(t_list **lst);

/*
**		parse/parse.c
*/

int		ms_parse(t_list *lst, t_env *st);

/*
**		parse/exec.c
*/

void	ms_main_exec(char **complete_cmd, t_env *st, int read, int write, int read2, int write2, int type);
void	ms_is_builtin(char **complete_cmd, t_env *st, int read, int write, int read2, int write2, int type);
void	ms_is_builtin_dumb(char **complete_cmd, t_env *st, int read, int write, int read2, int write2, int type);
//void	ms_main_exec(char **complete_cmd, t_env *st, int pip[2], int pip2[2], int type);

/*
**		parse/lst.c
*/

t_lst_parser	*ms_lst_parse_new(char **cmd, char *oper, int type);
int		ms_lst_parse_add_back(t_lst_parser **lst, t_lst_parser *new);
int		ms_lst_parse_add_front(t_lst_parser **lst, t_lst_parser *new);
void	ms_print_lst_parse(t_lst_parser *lst);
void	ms_print_lst_parse_reverse(t_lst_parser *lst);
char	*ms_find_var_path(char *str, t_env *st);

/*
**		parse/tools.c
*/

char	**ms_strcpy_cmd(char **arr);

/*
 *  ------- DEBUG FUNCTIONS ----------
*/

void	_ms_print_env(t_env *ms_env);
void	_ms_print_lst(t_list *lst);
int		_basic_checks(void);
void	ms_clear_env(t_env **st);

#endif
