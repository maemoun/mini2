/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdo <abdo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 16:28:49 by abdo              #+#    #+#             */
/*   Updated: 2025/08/24 15:56:40 by abdo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../main.h"

void	ctlc_handler(int sig)
{
	(void)sig;
	write (1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	ft_signals(void)
{
	signal(SIGINT, ctlc_handler);
	signal(SIGQUIT, SIG_IGN);
}

// #include "../main.h"

// int g_var = 0; // global flag to track shell state

// void ft_change_global(int flag)
// {
//     g_var = flag;
// }
// // Ctrl-D handler
// void ctld_handler(void)
// {
//     // Just print exit and quit
//     printf("\033[1A");   // Move cursor up one line
//     printf("\033[12C");  // Move cursor 12 spaces to the right
//     printf("exit\n");
//     exit(0);
// }

// // Ctrl-C handler
// void ctlc_handler(int sig)
// {
//     (void)sig;
//     write(1, "\n", 1);
//     rl_on_new_line();
//     rl_replace_line("", 0);
//     if (g_var == 0)  // Only redisplay prompt if shell is idle
//         rl_redisplay();
// }

// // Set signal handlers
// void ft_signals(void)
// {
//     signal(SIGINT, ctlc_handler);  // Ctrl-C
//     signal(SIGQUIT, SIG_IGN);      // Ctrl-'\'
// }