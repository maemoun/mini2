#include ".././parsing/main.h"

int	ft_strcmpp(char *s1, char *s2)
{
	while (*s1 && *s2 && (*s1 == *s2))
	{
		s1++;
		s2++;
	}
	return (*s1 - *s2);
}

int	ft_check_strcmp(const char *s1, const char *s2)
{
	if (!s1 || !s2)
		return (-1);
	while (*s1 && (*s1 == *s2))
	{
		s1++;
		s2++;
	}
	return ((unsigned char)*s1 - (unsigned char)*s2);
}

int	ft_strncmp_2(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	if (!s1 || !s2)
		return (-1);
	while (n > i && s1[i] && s2[i] && s1[i] == s2[i])
		i++;
	if (n == i)
		return (0);
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

int	ft_strlen_2(const char *s)
{
	int	len;

	len = 0;
	while (*s++)
		len++;
	return (len);
}

char	*ft_strjoin_2(char const *s1, char const *s2)
{
	char	*p;
	int		len_s;
	int		i;
	int		j;

	if (!s1 && !s2)
		return (NULL);
	if (!s1)
		return (ft_strdup_2(s2));
	if (!s2)
		return (ft_strdup_2(s1));
	len_s = ft_strlen_2(s1) + ft_strlen_2(s2);
	p = (char *)malloc((sizeof(char) * len_s) + 1);
	if (!p)
		return (strerror(errno), NULL);
	i = -1;
	while (s1[++i])
		p[i] = s1[i];
	j = 0;
	while (s2[j])
		p[i++] = s2[j++];
	p[i] = '\0';
	return (p);
}

char	*ft_strdup_2(const char *s1)
{
	char	*ma;
	size_t	i;

	i = 0;
	if (!s1)
		return (NULL);
	ma = (char *)malloc(sizeof(char) * (ft_strlen_2(s1) + 1));
	if (!ma)
		return (strerror(errno), NULL);
	while (s1[i])
	{
		ma[i] = s1[i];
		i++;
	}
	ma[i] = '\0';
	return (ma);
}

void	list_add_back(t_env **env_list, t_env *new)
{
	t_env	*tmp;

	if (!env_list)
		return ;
	if (!(*env_list))
	{
		(*env_list) = new;
		return ;
	}
	tmp = *env_list;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
}

t_env	*list_new_node(char *value, char *key, bool eg)
{
	t_env	*new;

	new = malloc(sizeof(t_env));
	if (!new)
		return (print_error(errno, NULL, NULL), NULL);
	new->key = ft_strdup_2(key);
	new->value = ft_strdup_2(value);
	new->eg = eg;
	new->next = NULL;
	return (new);
}

int	ft_isdigit(int c)
{
	return (c >= 48 && c <= 57);
}

int	ft_atoi(const char *str)
{
	int					i;
	int					sign;
	unsigned long long	tot;

	i = 0;
	sign = 1;
	tot = 0;
	while ((str[i] <= 13 && str[i] >= 9) || str[i] == 32)
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -sign;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		tot = (tot * 10) + (str[i] - 48);
		i++;
	}
	return ((int)(tot * sign));
}

char	*ft_substr_2(char const *s, unsigned int start, size_t len)
{
	char	*ma;
	size_t	mlen;
	size_t	b;

	if (!s)
		return (NULL);
	mlen = ft_strlen(s);
	b = 0;
	if (start >= (unsigned int)mlen)
		return (ft_strdup(""));
	if (len > mlen - start)
		len = mlen - start;
	ma = (char *)malloc(sizeof(char) * (len + 1));
	if (!ma)
		return (strerror(errno), NULL);
	while (b < len)
	{
		ma[b] = s[start + b];
		b++;
	}
	ma[b] = '\0';
	return (ma);
}

char	*ft_strchr_2(const char *s, int c)
{
	size_t	x;

	x = 0;
	while (s[x])
	{
		if (s[x] == (char)c)
			return ((char *)s + x);
		x++;
	}
	if (s[x] == (char)c)
		return ((char *)s + x);
	return (NULL);
}
