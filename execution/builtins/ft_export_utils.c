#include "../.././parsing/main.h"

int	ft_isalpha_2(int c)
{
	return ((c >= 65 && c <= 90) || (c >= 97 && c <= 122));
}

int	ft_isalnum_2(int c)
{
	return (ft_isalpha_2(c) || ft_isdigit(c));
}

char	*get_valide_key(char *command)
{
	int		i;
	char	*key;

	if (!command || !command[0])
		return (NULL);
	if (command[0] != '_' && !ft_isalpha_2(command[0]))
		return (NULL);
	i = 1;
	while (command[i] && (ft_isalnum_2(command[i]) || command[i] == '_'))
		i++;
	if (command[i] == '+' && command[i + 1] == '=')
		key = ft_substr_2(command, 0, i);
	else if (command[i] == '=' || command[i] == '\0')
		key = ft_substr_2(command, 0, i);
	else
		return (NULL);
	if (!key || key[0] == '\0')
		return (NULL);
	return (key);
}

void	add_export_list(t_env *env_list, char *value, char *key, bool eg)
{
	t_env	*tmp;
	int		exist;

	exist = 0;
	tmp = env_list;
	while (tmp)
	{
		if (!ft_check_strcmp(tmp->key, key))
		{
			exist = 1;
			if (eg)
			{
				if (tmp->value)
					free(tmp->value);
				tmp->value = ft_strdup_2(value);
				tmp->eg = eg;
				return ;
			}
		}
		tmp = tmp->next;
	}
	if (!exist)
		list_add_back(&env_list, list_new_node(value, key, eg));
}

void	desplay_list_export(t_env *env_list)
{
	t_env	*tmp;

	tmp = env_list;
	while (tmp)
	{
		if (tmp->eg == true)
			printf("declare -x %s=\"%s\"\n", tmp->key, tmp->value);
		else
			printf("%s\n", tmp->key);
		tmp = tmp->next;
	}
}

t_env	*ft_copy_env(t_env *env_list)
{
	t_env	*list;
	t_env	*new;

	new = NULL;
	list = env_list;
	while (list)
	{
		list_add_back(&new, list_new_node(list->value, list->key, list->eg));
		list = list->next;
	}
	return (new);
}

void	swap_nodes(t_env *node1, t_env *node2)
{
	char	*temp_key;
	char	*temp_value;
	bool	temp_eg;

	if (!node1 || !node2)
		return ;
	temp_key = node1->key;
	node1->key = node2->key;
	node2->key = temp_key;
	temp_value = node1->value;
	node1->value = node2->value;
	node2->value = temp_value;
	temp_eg = node1->eg;
	node1->eg = node2->eg;
	node2->eg = temp_eg;
}
