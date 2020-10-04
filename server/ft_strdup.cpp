#include "../includes/daemon.hpp"

char	*ft_strdup(char *s1)
{
	char	*str;

	if (!s1)
		return (NULL);
	if (!(str = (char *)malloc(sizeof(char) * (ft_strlen(s1) + 1))))
		return (NULL);
	ft_strcpy(str, s1);
	return (str);
}
