#include ".././parsing/main.h"

int	ft_exit_status(int status)
{
	static int	exit_status;

	if (status == -1)
		return (exit_status);
	exit_status = status;
	return (exit_status);
}

void	close_fd(int *fd)
{
	if (*fd != -1)
	{
		close(*fd);
		*fd = -1;
	}
}
void	clear_all_pipes(t_command *cmd)
{
	t_command	*cur;

	if (!cmd)
		return ;
	cur = cmd;
	while (cur)
	{
		if (cur->fd_pipe[0] > 0)
			close(cur->fd_pipe[0]);
		if (cur->fd_pipe[1] > 0)
			close(cur->fd_pipe[1]);
		if (cur->fd_herdoc[0] > 0)
			close(cur->fd_herdoc[0]);
		if (cur->fd_cmd[0] > 0)
			close(cur->fd_cmd[0]);
		if (cur->fd_cmd[1] > 0)
			close(cur->fd_cmd[1]);
		cur = cur->next_command;
	}
}

void	free_list_node(t_env *node)
{
	if (node)
	{
		if (node->key)
			free(node->key);
		if (node->value)
			free(node->value);
		free(node);
	}
	node = NULL;
}

void	free_list(t_env **env_list)
{
	t_env	*cur;
	t_env	*next;

	if (!env_list || !*env_list)
		return ;
	cur = *env_list;
	while (cur)
	{
		next = cur->next;
		free_list_node(cur);
		cur = next;
	}
	*env_list = NULL;
}

void	free_redirec_list(t_redirection *red)
{
	t_redirection	*temp;

	while (red)
	{
		temp = red;
		red = red->next_red;
		free(temp->name);
		free(temp);
	}
}

void	free_cmd_node(t_command *cmd)
{
	int	i;

	if (!cmd)
		return ;
	i = 0;
	while (cmd->command && cmd->command[i])
		free(cmd->command[i++]);
	if (cmd->command)
		free(cmd->command);
	if (cmd->red)
		free_redirec_list(cmd->red);
	free(cmd);
}

void	free_cmd_list(t_command *cmd)
{
	t_command	*next;

	while (cmd)
	{
		next = cmd->next_command;
		free_cmd_node(cmd);
		cmd = next;
	}
}

void	cleanup_shell(t_command *cmd, t_env *env_list)
{
	clear_all_pipes(cmd);
	free_list(&env_list);
	free_cmd_list(cmd);
	free(cmd);
	cmd = NULL;
}

int	status_scan(int *status)
{
	if (WIFSIGNALED(*status))
	{
		if (WTERMSIG(*status) == SIGINT)
		{
			write(1, "\n", 1);
			return (130);
		}
		else if (WTERMSIG(*status) == SIGQUIT)
		{
			write(1, "Quit: 3\n", 8);
			return (131);
		}
	}
	return (0);
}

bool	wait_and_exit(int last_pid)
{
	int	status;
	int	pid;

	pid = 1;
	while (pid > 0)
	{
		pid = wait(&status);
		if (pid == -1)
			continue ;
		if (pid == last_pid)
		{
			if (WIFEXITED(status))
			{
				ft_exit_status(WEXITSTATUS(status));
				if (ft_exit_status(-1) == 1)
					return (true);
			}
			else if (WIFSIGNALED(status))
			{
				ft_exit_status(status_scan(&status));
			}
		}
	}
	return (false);
}