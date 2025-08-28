#include ".././parsing/main.h"

#include <stdio.h>

void	print_command_debug(t_command *cmd)
{
    if (!cmd)
    {
        printf("Command: (null)\n");
        return;
    }
    printf("Command: ");
    if (cmd->command && cmd->command[0])
        printf("%s\n", cmd->command[0]);
    else
        printf("(null)\n");

    printf("fd_herdoc: [%d, %d]\n", cmd->fd_herdoc[0], cmd->fd_herdoc[1]);
    printf("cmd_backup: [%d, %d]\n", cmd->cmd_backup[0], cmd->cmd_backup[1]);
    printf("fd_cmd: [%d, %d]\n", cmd->fd_cmd[0], cmd->fd_cmd[1]);
    printf("fd_pipe: [%d, %d]\n", cmd->fd_pipe[0], cmd->fd_pipe[1]);

    printf("Next command: ");
    if (cmd->next_command && cmd->next_command->command && cmd->next_command->command[0])
        printf("%s\n", cmd->next_command->command[0]);
    else
        printf("(null)\n");

    printf("Prev command: ");
    if (cmd->prev && cmd->prev->command && cmd->prev->command[0])
        printf("%s\n", cmd->prev->command[0]);
    else
        printf("(null)\n");
}

void	print_command_list(t_command *head)
{
    int i = 0;
    t_command *current = head;
    while (current)
    {
        printf("---- Command #%d ----\n", i);
        print_command_debug(current);
        current = current->next_command;
        i++;
    }
}

bool	check_if_is_a_directory(char *path)
{
	struct stat	path_stat;

	if (stat(path, &path_stat) != 0)
		return (false);
	if (S_ISDIR(path_stat.st_mode))
		return (true);
	return (false);
}

char	**get_system_paths(t_env *env_list)
{
	char	*value;

	value = get_value(env_list, "PATH");
	if (!value)
		return (NULL);
	return (ft_split(value, ':'));
}

char	*get_executable_paths(char **path, char *command)
{
	int		i;
	char	*join;
	char	*tmp;

	i = 0;
	join = NULL; // initialize to NULL
	if (path && *command)
	{
		while (path[i])
		{
			join = ft_strjoin_2("/", command);
			tmp = ft_strdup_2(join);
			free(join);
			join = ft_strjoin_2(path[i], tmp);
			free(tmp);
			if (access(join, X_OK) == 0)
				return (join);
			i++;
		}
		if(join)
		{
			free(join);
			join = NULL;
		}
	}
	ft_putstr_fd2("minishell: ", 2);
	ft_putstr_fd2(command, 2);
	ft_putstr_fd2(": command not found\n", 2);
	return (NULL);
}

void	p2char(char ***ptr)
{
	int	r;

	if (!ptr || !*ptr)
		return ;
	r = 0;
	while ((*ptr)[r])
		free((*ptr)[r++]);
	free(*ptr);
	*ptr = NULL;
}

char	*check_if_exist_path(t_env *env_list, t_command *cmd)
{
	char	**full_path;

	full_path = NULL;
	if (ft_strchr_2(cmd->command[0], '/'))
	{
		if (access(cmd->command[0], F_OK) == 0 && access(cmd->command[0], X_OK) == -1)
			return (print_error(errno = EACCES, cmd->command[0], NULL), NULL);
		if (check_if_is_a_directory(cmd->command[0]))
			return (print_error(errno = EISDIR, cmd->command[0], NULL), NULL);
		if (!access(cmd->command[0], X_OK))
			return (ft_strdup_2(cmd->command[0]));
		if (access(cmd->command[0], F_OK))
			print_error(errno, cmd->command[0], NULL);
		return (NULL);
	}
	full_path = get_system_paths(env_list);
	if (!full_path)
	{
		if (!access(cmd->command[0], X_OK))
			return (ft_strdup(cmd->command[0]));
		else
			print_error(errno, cmd->command[0], NULL);
	}
	return (get_executable_paths(full_path, cmd->command[0]));
}

int	list_size(t_env *env_list)
{
	int		i;
	t_env	*tmp;

	i = 0;
	tmp = env_list;
	while (tmp)
	{
		i++;
		tmp = tmp->next;
	}
	return (i);
}

char	**build_environment_array(t_env *env_list)
{
	int		len;
	t_env	*tmp;
	int		i;
	char	*join;
	char	**env;

	i = 0;
	len = list_size(env_list);
	tmp = env_list;
	env = malloc(sizeof (char *) * (len + 1));
	if (!env)
	{
		print_error(errno, NULL, NULL);
		return (NULL);
	}
	while (tmp)
	{
		if (tmp->eg)
			join = ft_strjoin_2(tmp->key, "=");
		else
			join = ft_strdup_2(tmp->key);
		env[i++] = ft_strjoin_2(join, tmp->value);
		if (join)
			free(join);
		tmp = tmp->next;
	}
	env[i] = NULL;
	return (env);
}

void	child_process(t_env *env_list, t_command *cmd)
{
	char	*path;
	char	**env;

	path = NULL;
	env = NULL;
	if (!ft_handle_redirection(cmd))
		exit(1);
	if (cmd->next_command || cmd->prev) // Only if part of a pipeline
	{
		if (!setup_pipes(cmd))
			exit(1);
	}
	if (!duplicate_std_fds(cmd))
		exit(1);
	if (is_builtin(cmd->command[0]))
		return (execute_builtin(cmd->command, cmd, env_list), exit(0));
	else
	{
		path = check_if_exist_path(env_list, cmd);
		if (!path)
			exit(127);
		env = build_environment_array(env_list);
		if (execve(path, cmd->command, env) == -1)
			return (print_error(errno, cmd->command[0], NULL), free(path), p2char(&env), exit(1));
	}
}

void	parent_process(t_command *cmd)
{
	if (cmd->fd_pipe[1] > 0)
	{
		close(cmd->fd_pipe[1]);
		cmd->fd_pipe[1] = -1;
	}
	if (cmd->fd_herdoc[0] > 0)
	{
		close(cmd->fd_herdoc[0]);
		cmd->fd_herdoc[0] = -1;
	}
	if (cmd->prev)
	{
		if (cmd->prev->fd_pipe[0] > 0)
		{
			close(cmd->prev->fd_pipe[0]);
			cmd->prev->fd_pipe[0] = -1;
		}
		if (cmd->prev->prev)
		{
			if (cmd->prev->prev->fd_pipe[0] > 0)
			{
				close(cmd->prev->prev->fd_pipe[0]);
				cmd->prev->prev->fd_pipe[0] = -1;
			}
		}
	}
}

void	execute_cmd(t_command *cmd, t_env *env_list)
{
	t_command	*list;
	int		fork_pid;

	list = cmd;
	// print_command_list(cmd);
	// exit(0);
	while (list)
	{
		if (list->next_command)
		{
			if (pipe(list->fd_pipe) == -1)
				return ;
		}
		fork_pid = fork();
		// if (fork_pid < 0)
		// 	perror(FORK_FAILD);
		if (fork_pid == 0)
		{
			ft_signals_child();
			child_process(env_list, list);
		}
	    if (fork_pid != 0)
        {
            if (list->next_command || list->prev)
                parent_process(list);
        }
		list = list->next_command;
	}
	while(wait_and_exit(fork_pid));
}
