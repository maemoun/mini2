#include ".././parsing/main.h"

void	init_herdoc_fds(t_command *cmd)
{
	t_command *cur;

	cur = cmd;
	while (cur)
	{
		cur->fd_herdoc[0] = -1;
		cur->fd_herdoc[1] = -1;
		cur->fd_cmd[0] = -1;
		cur->fd_cmd[1] = -1;
		cur->fd_pipe[0] = -1;
		cur->fd_pipe[1] = -1;
		cur = cur->next_command;
	}
}

void	herdoc_read(t_command *cmd, char *name, t_env *env_list, t_red_type type)
{
	char	*line;

	if (!name)
		return ;
	close(cmd->fd_herdoc[0]);
	while (1)
	{

		line = readline("> ");
		if (!line)
			break ;
		if (!ft_check_strcmp(line, name))
		{
			// free(line);
			// line = NULL;
			break ;
		}
		// if (type == D_HERDOC_Q)
			// ft_putstr_fd2(line, cmd->fd_herdoc[1]);
		// else
		// 	expand_cmd_heredoc(&line, cmd, cmd->fd_herdoc[1]);
		free(line);
	}
	if (line)
		free(line);
	// free(name);
	close(cmd->fd_herdoc[1]);
}


bool	ft_create_herdoc(t_env *env_list, t_command *cmd, char *name, t_red_type type)
{
	int		fork_pid;
	char	*name_copy;

	name_copy = ft_strdup_2(name);
	if (!name_copy)
		return (false);
	fork_pid = fork();
	if (fork_pid < 0)
	{
		free(name_copy);
		close_fd(&cmd->fd_herdoc[0]);
		close_fd(&cmd->fd_herdoc[1]);
		return (print_error(errno, NULL, NULL), false);
	}
	if (fork_pid == 0)
	{
		signal(SIGINT, ctlc_handler);
		herdoc_read(cmd, name_copy, env_list, type);
		free_cmd_list(cmd);
		free_list(&env_list);
		exit(0);
	}
	if (fork_pid > 0)
	{
		free(name_copy);
		close_fd(&cmd->fd_herdoc[1]);
		if (wait_and_exit(fork_pid))
			return (false);
	}
	return (true);
}

bool	ft_process_heredocs(t_command *cmd, t_env *env_list)
{
	t_command		*tmp;
	t_redirection	*redi;

	tmp = cmd;
	init_herdoc_fds(cmd);
	while (tmp)
	{
		redi = tmp->red;
		while (redi)
		{
			if (redi->type == RED_HEREDOC)
			{
				if (tmp->fd_herdoc[0] != -1)
					close_fd(&tmp->fd_herdoc[0]);
				if (tmp->fd_herdoc[1] != -1)
					close_fd(&tmp->fd_herdoc[1]);
				if (!ft_create_herdoc(env_list, tmp, redi->name, redi->type))
					return (close_fd(&tmp->fd_herdoc[0]), close_fd(&tmp->fd_herdoc[1]), false);
			}
			redi = redi->next_red;
		}
		tmp = tmp->next_command;
	}
	return (true);
}
