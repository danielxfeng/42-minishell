/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   terminal_caps.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Xifeng <xifeng@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 01:28:01 by Xifeng            #+#    #+#             */
/*   Updated: 2025/02/08 18:11:41 by Xifeng           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

/**
* @brief Initiate termcaps settings to use terminal capabilites
* @param termcaps struct with terminal capabilities capabilities
*
* - Get the current terminal settings for `STDIN` and store them in
* `old_term` to retore terminal later to original state
*
* - Makes sure `TERM` is set
*
* - Indicate to the termcap lib with type of terminal we are using.
* It will save that info internally to use its capabilities later
*
* - Checks if the terminal has all the capabilities required
*/
void	init_termcaps(t_termcaps *termcaps)
{
	char	*term_type;

	if (tcgetattr(STDIN_FILENO, &termcaps -> old_term) == -1)
		quit(EXIT_FAILURE);
	term_type = ft_getenv("TERM");
	if (!term_type)
		quit(EXIT_FAILURE);
	if (tgetent(termcaps -> buffer, term_type) <= 0)
		quit(EXIT_FAILURE);
	else if (!has_capabilities(termcaps))
		quit(EXIT_FAILURE);
	free(term_type);
}

/**
 * @brief Checks if the terminal supports required capabilities.
 *
 * Retrieves and stores essential terminal capabilities using `tgetstr()`.  
 * If any required capability is missing, the function returns `0`.
 *
 * @param termcaps Pointer to `t_termcaps` struct where capabilities are stored.
 *
 * @note Handles platform-specific backspace (`kb` for Linux, `\x7f` for macOS).
 */
int	has_capabilities(t_termcaps *termcaps)
{
	int	check;

	termcaps -> keys_on = tgetstr("ks", &termcaps -> buffer);
	if (termcaps -> keys_on)
		tputs(termcaps -> keys_on, 1, ft_put);
	termcaps -> keys_off = tgetstr("ke", &termcaps -> buffer);
	termcaps -> up_arrow = tgetstr("ku", &termcaps -> buffer);
	termcaps -> down_arrow = tgetstr("kd", &termcaps -> buffer);
	if (IS_LINUX)
		termcaps -> backspace = tgetstr("kb", &termcaps->buffer);
	else
		termcaps -> backspace = ft_strdup("\x7f");
	termcaps -> del_line = tgetstr("dl", &termcaps -> buffer);
	termcaps -> set_cursor_begin = tgetstr("cr", &termcaps -> buffer);
	if (!termcaps -> keys_on || !termcaps -> keys_off
		|| !termcaps -> up_arrow || !termcaps -> down_arrow
		|| !termcaps -> backspace || !termcaps -> del_line
		|| !termcaps -> set_cursor_begin)
		check = 0;
	else
		check = 1;
	return (check);
}
/**
* @brief Disables canonical mode for real-time input processing.

* - Copies the original terminal settings for modification.

* - Turns off ICANON (process input immediately without Enter).

* - Disables ECHO (hides typed characters).

* - Blocks signals (Ctrl-C, Ctrl-Z) and special processing (Ctrl-V, Ctrl-S, Ctrl-Q).

* - Sets VMIN=1 (read returns per byte) and VTIME=0 (no input delay).

* - Applies changes immediately using tcsetattr.
*/
void	disable_canonical_mode(t_termcaps *termcaps)
{
	termcaps->new_term = termcaps->old_term;
	termcaps->new_term.c_lflag &= ~ICANON;
	termcaps->new_term.c_lflag &= ~ECHO;
	termcaps->new_term.c_lflag &= ~ISIG;
	termcaps->new_term.c_lflag &= ~IEXTEN;
	termcaps->new_term.c_iflag &= ~IXON;
	termcaps->new_term.c_cc[VMIN] = 1;
	termcaps->new_term.c_cc[VTIME] = 0;
	if (tcsetattr(STDIN_FILENO, TCSANOW, &termcaps->new_term) == -1)
		quit(EXIT_FAILURE);
}

/**
* @brief Restores canonical mode by reapplying the original terminal settings.
*/
void	enable_canonical_mode(t_termcaps *termcaps)
{
	if (tcsetattr(STDIN_FILENO, TCSANOW, &termcaps->old_term) == -1)
		quit(EXIT_FAILURE);
}

int	ft_put(int c)
{
	return (write(1, &c, 1));
}
