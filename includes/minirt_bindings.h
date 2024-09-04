/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt_bindings.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 15:54:53 by ychalant          #+#    #+#             */
/*   Updated: 2024/02/20 16:05:35 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_BINDINGS_H
# define MINIRT_BINDINGS_H

# ifdef __APPLE__

#  define TU 0x0D
#  define TD 0x01
#  define TL 0x00
#  define TR 0x02

#  define ROT_UP 126
#  define ROT_DOWN 125
#  define ROT_LEFT 124
#  define ROT_RIGHT 123
#  define TILT_LEFT 12 
#  define TILT_RIGHT 14

#  define SHIFT 257
#  define CTRL 259
#  define RELOAD 114
#  define MODE 46
#  define SPACE 49
#  define ESCAPE 53
#  define COMMAND 261
#  define TAB 48
#  define ALT 261

# endif

# ifdef __linux__

#  define TU 119
#  define TD 115
#  define TL 97
#  define TR 100

#  define ROT_UP 65362
#  define ROT_DOWN 65364
#  define ROT_LEFT 65363
#  define ROT_RIGHT 65361
#  define TILT_LEFT 113 
#  define TILT_RIGHT 101

#  define SHIFT 65505
#  define CTRL 65507
#  define RELOAD 114
#  define MODE 109
#  define SPACE 32
#  define ESCAPE 65307
#  define TAB 65289
#  define ALT 65513

# endif

#endif
