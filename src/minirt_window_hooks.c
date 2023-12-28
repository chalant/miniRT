/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf_window_hooks.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 15:53:50 by ychalant          #+#    #+#             */
/*   Updated: 2023/08/30 14:42:39 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	update_view(t_minirt *minirt)
{
	(void)minirt;
	return (0);
}

int	close_program(t_minirt *fdf)
{
	(void)fdf;
	minirt_cleanup(fdf, 0);
	exit(0);
}

int	handle_expose(t_minirt *fdf)
{
	(void)fdf;
	//update_view(fdf);
	return (0);
}
