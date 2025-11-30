/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moelamma <moelamma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/30 13:48:58 by moelamma          #+#    #+#             */
/*   Updated: 2025/11/30 15:02:28 by moelamma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

static char	*extract_line(char *leftover)
{	
	char	*newline_pos;
	int		i;

	newline_pos = ft_strchr(leftover, '\n');
	if (newline_pos)
	{
		i = newline_pos - leftover;
		return (ft_substr(leftover, 0, i + 1));
	}
	return (ft_substr(leftover, 0, ft_strlen(leftover)));
}

static char	*update_leftover(char *leftover)
{
	char	*newline_pos;
	char	*new_copy;
	int		i;

	newline_pos = ft_strchr(leftover, '\n');
	if (!newline_pos)
	{
		free(leftover);
		return (NULL);
	}
	i = newline_pos - leftover + 1;
	new_copy = ft_substr(leftover, i, ft_strlen(leftover) - i);
	free(leftover);
	return (new_copy);
}

static char	*read_and_join(int fd, char *leftover)
{
	char	*buffer;
	int		bytes_read;
	char	*temp;

	buffer = malloc(sizeof(char) * ((size_t)BUFFER_SIZE + 1));
	if (!buffer)
		return (NULL);
	bytes_read = 1;
	while (!ft_strchr(leftover, '\n') && bytes_read > 0)
	{
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		if (bytes_read == -1)
			return (free(leftover), free(buffer), NULL);
		buffer[bytes_read] = '\0';
		temp = ft_strjoin(leftover, buffer);
		if (!temp)
			return (free(leftover), free(buffer), NULL);
		free(leftover);
		leftover = temp;
	}
	free(buffer);
	return (leftover);
}

char	*get_next_line(int fd)
{
	static char	*leftover[MAX_FD];
	char		*line;
	char		*temp;

	if (fd < 0 || fd >= MAX_FD || BUFFER_SIZE <= 0)
		return (NULL);
	if (!leftover[fd])
	{
		leftover[fd] = malloc(1);
		if (!leftover[fd])
			return (NULL);
		leftover[fd][0] = '\0';
	}
	leftover[fd] = read_and_join(fd, leftover[fd]);
	if (!leftover[fd] || *leftover[fd] == '\0')
		return (free(leftover[fd]), leftover[fd] = NULL, NULL);
	line = extract_line(leftover[fd]);
	if (!line)
		return (free(leftover[fd]), leftover[fd] = NULL, NULL);
	temp = update_leftover(leftover[fd]);
	if (!temp)
		return (leftover[fd] = NULL, line);
	leftover[fd] = temp;
	return (line);
}
