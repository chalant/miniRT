/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt_bindings.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexphil <alexphil@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 15:54:53 by ychalant          #+#    #+#             */
/*   Updated: 2024/01/31 16:01:46 by alexphil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_BINDINGS_H
# define MINIRT_BINDINGS_H

# ifdef __APPLE__

#  define TU 0x0D
#  define TD 0x01
#  define TL 0x00
#  define TR 0x02

#  define RU 126
#  define RD 125
#  define RL 124
#  define RR 123

#  define ZI 24
#  define ZO 27

#  define SHIFT 257
#  define CTRL 259
#  define RELOAD 114
#  define MODE 46
#  define SPACE 49
#  define ESCAPE 53
#  define COMMAND 261
#  define TAB 48

#endif

# ifdef __linux__

#  define TU 122
#  define TD 115
#  define TL 113
#  define TR 100

#  define RU 65362
#  define RD 65364
#  define RL 65363
#  define RR 65361

#  define ZI 61
#  define ZO 45

#  define SHIFT 65505
#  define CTRL 65507
#  define RELOAD 114
#  define MODE 109
#  define SPACE 32
#  define ESCAPE 65307
#  define TAB 65289

#endif

#endif
