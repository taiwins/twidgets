/*
 * nk_wl_font.h - taiwins client nuklear font handling functions
 *
 * Copyright (c) 2019-2020 Xichen Zhou
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice (including the
 * next paragraph) shall be included in all copies or substantial
 * portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT.  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

#ifndef NK_WL_FONT_H
#define NK_WL_FONT_H

#include <string.h>
#include "nk_wl_internal.h"

#include <fontconfig/fontconfig.h>

#include <cairo/cairo.h>
#include <cairo/cairo-ft.h>
#include <freetype2/ft2build.h>
#include FT_FREETYPE_H

#ifdef __cplusplus
extern "C" {
#endif

struct nk_wl_user_font {
	struct wl_list link;
	struct nk_user_font user_font;
};

static struct nk_wl_font_config default_config = {
	.name = "sans",
	.slant = NK_WL_SLANT_ROMAN,
	.pix_size = 16,
	.scale = 1,
	.nranges = 0,
	.ranges = NULL,
	.TTFonly = false,
};

static char *
nk_wl_find_font(const struct nk_wl_font_config *user_config)
{
	FcInit();
	FcConfig *config = FcInitLoadConfigAndFonts();
	FcPattern *font = NULL;
	FcPattern *pat = FcNameParse((const FcChar8 *)user_config->name);
	char *path = NULL;

	FcConfigSubstitute(config, pat, FcMatchPattern);
	FcDefaultSubstitute(pat);
	//boldness
	if (user_config->bold)
		FcPatternAddInteger(pat, FC_WEIGHT, FC_WEIGHT_DEMIBOLD);
	else
		FcPatternAddInteger(pat, FC_WEIGHT, FC_WEIGHT_REGULAR);
	//slant
	FcPatternAddInteger(
		pat, FC_SLANT,
		(user_config->slant == NK_WL_SLANT_ROMAN) ? FC_SLANT_ROMAN :
		(user_config->slant == NK_WL_SLANT_OBLIQUE) ? FC_SLANT_OBLIQUE :
		FC_SLANT_ITALIC);

	FcResult result;
	FcFontSet *set = FcFontSort(config, pat, false, NULL, &result);
	if (!set)
		goto fini;
	//deal with TTF
	if (user_config->TTFonly) {
		const char *format = "TrueType";
		//select only the ttf
		FcPattern *ttf_pattern = FcPatternCreate();
		FcPatternAddString(ttf_pattern, FC_FONTFORMAT, (const FcChar8*)format);
		font = FcFontSetMatch(config, &set, 1, ttf_pattern, &result);
		FcPatternDestroy(ttf_pattern);
	} else
		font = FcFontSetMatch(config, &set, 1, pat, &result);
	FcFontSetDestroy(set);
	//return string
	FcChar8* file = NULL;
	if (FcPatternGetString(font, FC_FILE, 0, &file) != FcResultMatch)
		goto fini;
	/* printf("%s\n", file); */
	path = strdup((const char *)file);
fini:
	if (font)
		FcPatternDestroy(font);
	FcPatternDestroy(pat);
	FcConfigDestroy(config);
	FcFini();
	return path;
}

#if defined (NK_INCLUDE_FONT_BAKING)

//merge unicodes
static inline void
union_unicode_range(const nk_rune left[2], const nk_rune right[2], nk_rune out[2])
{
	nk_rune tmp[2];
	tmp[0] = left[0] < right[0] ? left[0] : right[0];
	tmp[1] = left[1] > right[1] ? left[1] : right[1];
	out[0] = tmp[0];
	out[1] = tmp[1];
}

//return true if left and right are insersected, else false
static inline bool
intersect_unicode_range(const nk_rune left[2], const nk_rune right[2])
{
	return (left[0] <= right[1] && left[1] >= right[1]) ||
		(left[0] <= right[0] && left[1] >= right[0]);
}

static inline int
unicode_range_compare(const void *l, const void *r)
{
	const nk_rune *range_left = (const nk_rune *)l;
	const nk_rune *range_right = (const nk_rune *)r;
	return ((int)range_left[0] - (int)range_right[0]);
}

static int
merge_unicode_range(const nk_rune *left, const nk_rune *right, nk_rune *out)
{
	//get the range
	int left_size = 0;
	while(*(left+left_size)) left_size++;
	int right_size = 0;
	while(*(right+right_size)) right_size++;
	//sort the range,
	nk_rune sorted_ranges[left_size+right_size];
	memcpy(sorted_ranges, left, sizeof(nk_rune) * left_size);
	memcpy(sorted_ranges+left_size, right, sizeof(nk_rune) * right_size);
	qsort(sorted_ranges, (left_size+right_size)/2, sizeof(nk_rune) * 2,
	      unicode_range_compare);
	//merge algorithm
	nk_rune merged[left_size+right_size+1];
	merged[0] = sorted_ranges[0];
	merged[1] = sorted_ranges[1];
	int m = 0;
	for (int i = 0; i < (left_size+right_size) / 2; i++) {
		if (intersect_unicode_range(&sorted_ranges[i*2],
					    &merged[2*m]))
			union_unicode_range(&sorted_ranges[i*2], &merged[2*m],
					    &merged[2*m]);
		else {
			m++;
			merged[2*m] = sorted_ranges[2*i];
			merged[2*m+1] = sorted_ranges[2*i+1];
		}
	}
	m++;
	merged[2*m] = 0;

	if (!out)
		return 2*m;
	memcpy(out, merged, (2*m+1) * sizeof(nk_rune));
	return 2*m;
}

#endif /* NK_INCLUDE_FONT_BAKING */



#ifdef __cplusplus
}
#endif

#endif /* EOF */
