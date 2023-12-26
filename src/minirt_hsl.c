#include "minirt_colors.h"

static unsigned int	make_rgb(float rr, float gg, float bb, float m)
{
	unsigned int	rgb;

	rgb = 0;
	rgb |= (unsigned int)((bb + m) * 255);
	rgb |= (unsigned int)((gg + m) * 255) << 8;
	rgb |= (unsigned int)((rr + m) * 255) << 16;
	return (rgb);
}

unsigned int	hsl_to_rgb(t_hsl *hsl)
{
	float	c;
	float	x;
	float	m;
	float	hue;

	c = (1 - fabs(2 * hsl->lightness - 1)) * hsl->saturation;
	hue = hsl->hue / 60;
	x = c * (1 - fabs(fmod(hue, 2) - 1));
	m = hsl->lightness - c / 2;
	if (hue >= 0 && hue < 1)
		return (make_rgb(c, x, 0, m));
	else if (hue >= 1 && hue < 2)
		return (make_rgb(x, c, 0, m));
	else if (hue >= 2 && hue < 3)
		return (make_rgb(0, c, x, m));
	else if (hue >= 3 && hue < 4)
		return (make_rgb(0, x, c, m));
	else if (hue >= 4 && hue < 5)
		return (make_rgb(x, 0, c, m));
	else if (hue >= 5 && hue < 6)
		return (make_rgb(c, 0, x, m));
	else
		return (make_rgb(0, 0, 0, m));
}

t_hsl	rgb_to_hsl(float rr, float gg, float bb)
{
	t_hsl	hsl;
	float	cmax;
	float	cmin;
	float	delta;

	cmax = fmax(fmax(rr, gg), bb);
	cmin = fmin(fmin(rr, gg), bb);
	delta = cmax - cmin;
	if (delta == 0)
		hsl.hue = 0;
	else if (cmax == rr)
		hsl.hue = 60 * fmod(((gg - bb) / delta), 6);
	else if (cmax == gg)
		hsl.hue = 60 * (((bb - rr) / delta) + 2);
	else if (cmax == bb)
		hsl.hue = 60 * (((rr - gg) / delta) + 4);
	hsl.lightness = (cmax + cmin) / 2;
	if (delta == 0)
		hsl.saturation = 0;
	else
		hsl.saturation = delta / (1 - fabs(2 * hsl.lightness - 1));
	return (hsl);
}
