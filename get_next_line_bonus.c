/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpicchio <tpicchio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/11 13:07:12 by tpicchio          #+#    #+#             */
/*   Updated: 2024/01/23 14:43:15 by tpicchio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

static void	*ft_calloc(size_t nmemb, size_t size)
{
	size_t	tot;
	void	*ptr;
	size_t	i;

	tot = nmemb * size;
	if (nmemb != 0 && tot / nmemb != size)
		return (NULL);
	ptr = malloc(tot);
	if (ptr == NULL)
		return (NULL);
	i = 0;
	while (i < tot)
		((unsigned char *)ptr)[i++] = 0;
	return (ptr);
}

static char	*ft_remainder_trim(char *remainder)
{
	size_t	remainder_index;
	size_t	line_index;
	char	*line;

	remainder_index = 0;
	line_index = 0;
	while (remainder[remainder_index] != '\n' && remainder[remainder_index])
		remainder_index++;
	if (!remainder[remainder_index])
	{
		free(remainder);
		return (NULL);
	}
	line = ft_calloc(ft_strlen(remainder) - remainder_index + 1, sizeof(char));
	remainder_index++;
	while (remainder[remainder_index])
		line[line_index++] = remainder[remainder_index++];
	free(remainder);
	return (line);
}

static char	*ft_get_line(char *remainder)
{
	char	*line;
	size_t	index;

	index = 0;
	if (!remainder[index])
		return (NULL);
	while (remainder[index] != '\n' && remainder[index])
		index++;
	line = ft_calloc(index + 2, sizeof(char));
	index = 0;
	while (*remainder != '\n' && *remainder)
		line[index++] = *remainder++;
	if (*remainder == '\n')
		line[index] = '\n';
	return (line);
}

static char	*ft_read_line(int fd, char *remainder)
{
	char	*temp_remainder;
	char	*read_remainder;
	ssize_t	bytes_read;

	if (!remainder)
		remainder = ft_calloc(1, sizeof(char));
	temp_remainder = remainder;
	read_remainder = ft_calloc(BUFFER_SIZE + 1, sizeof(char));
	bytes_read = 1;
	while (!ft_strchr(remainder, '\n') && bytes_read > 0)
	{
		bytes_read = read(fd, read_remainder, BUFFER_SIZE);
		if (bytes_read == -1)
		{
			free(read_remainder);
			free(temp_remainder);
			return (NULL);
		}
		read_remainder[bytes_read] = '\0';
		temp_remainder = remainder;
		remainder = ft_strjoin(temp_remainder, read_remainder, 1);
	}
	free(read_remainder);
	return (remainder);
}

char	*get_next_line(int fd)
{
	char		*line;
	static char	*remainder[FD_MAX];

	if (fd == -1 || BUFFER_SIZE <= 0 || fd >= FD_MAX)
		return (NULL);
	remainder[fd] = ft_read_line(fd, remainder[fd]);
	if (!remainder[fd])
		return (NULL);
	line = ft_get_line(remainder[fd]);
	remainder[fd] = ft_remainder_trim(remainder[fd]);
	return (line);
}
