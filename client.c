/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiyoulee <jiyoulee@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/02 23:24:52 by jiyoulee          #+#    #+#             */
/*   Updated: 2022/03/16 00:36:36 by jiyoulee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

int	g_is_received;

pid_t	parse_pid(char *str)
{
	int	i;
	int	num;

	i = 0;
	num = 0;
	while (str[i])
	{
		if ('0' <= str[i] && str[i] <= '9')
			num = num * 10 + str[i] - '0';
		else
			return (-1);
		i++;
	}
	return (num);
}

void	sigusr_handler(int signum)
{
	(void)signum;
	g_is_received = TRUE;
	write(1, "Received!\n", 10);
}

int	send_str_by_signal(char *str, pid_t server_pid)
{
	int		i;
	int		bit;
	int		ch;

	i = 0;
	while (1)
	{
		bit = 7;
		while (bit >= 0)
		{
			ch = (int)str[i] >> bit & 1;
			if (ch == 0 && kill(server_pid, SIGUSR1) == ERROR)
				return (1);
			else if (ch == 1 && kill(server_pid, SIGUSR2) == ERROR)
				return (1);
			usleep(200);
			bit--;
		}
		if (str[i] == '\0')
			break ;
		i++;
	}
	return (0);
}

int	main(int argc, char **argv)
{
	pid_t	server_pid;

	if (argc != 3)
	{
		write(-1, "argc invalid\n", 13);
		return (1);
	}
	server_pid = parse_pid(argv[1]);
	if (server_pid < 101 || server_pid >= 100000)
	{
		write(-1, "pid invalid\n", 12);
		return (1);
	}
	g_is_received = 0;
	if (signal(SIGUSR1, sigusr_handler) == SIG_ERR
		|| send_str_by_signal(argv[2], server_pid) != 0)
		return (1);
	if (g_is_received == TRUE)
		usleep(500);
	if (g_is_received == FALSE)
		return (0);
	return (1);
}
