/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiyoulee <jiyoulee@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/02 23:25:05 by jiyoulee          #+#    #+#             */
/*   Updated: 2022/03/15 22:58:55 by jiyoulee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

void	restore_data_from_bit(int bit, pid_t client_pid)
{
	static int	curr_ch;
	static int	curr_bit;
	static char	buff[1000];
	static int	len;

	curr_ch = (curr_ch << 1) | bit;
	curr_bit++;
	if (curr_bit == 8)
	{
		buff[len++] = curr_ch;
		if (curr_ch == '\0' || len >= 1000)
		{
			write(1, buff, len - (curr_ch == '\0'));
			write(1, "\n", 1);
			len = 0;
			if (curr_ch == '\0')
				kill(client_pid, SIGUSR1);
		}
		curr_ch = 0;
		curr_bit = 0;
	}
}

void	sigusr_handler(int signum, siginfo_t *info, void *context)
{
	(void)signum;
	(void)context;
	restore_data_from_bit(signum == SIGUSR2, info->si_pid);
}

void	print_pid(pid_t	pid)
{
	char	c;

	if (!pid)
		return ;
	print_pid(pid / 10);
	c = (pid % 10) + '0';
	write(1, &c, 1);
}

int	set_up_handlers(void)
{
	struct sigaction	sa;

	sa.sa_flags = SA_SIGINFO;
	sa.sa_sigaction = sigusr_handler;
	if (sigemptyset(&sa.sa_mask) < 0
		|| sigaction(SIGUSR1, &sa, NULL) < 0
		|| sigaction(SIGUSR2, &sa, NULL) < 0)
		return (-1);
	return (0);
}

int	main(void)
{
	pid_t				pid;

	if (set_up_handlers() == -1)
		return (1);
	pid = getpid();
	write(1, "server_pid: ", 13);
	print_pid(pid);
	write(1, "\n", 1);
	while (1)
		pause();
	return (0);
}
