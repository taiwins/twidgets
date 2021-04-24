/*
 * nk_wl_image.h - taiwins client nuklear image handling functions
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

#ifndef NK_WL_IMAGE_H
#define NK_WL_IMAGE_H

#include <stdbool.h>
#include <stdlib.h>
#include <wayland-client.h>
#include <cairo/cairo.h>
#include <wayland-util.h>
#include "nk_wl_internal.h"
#include <twclient/ui.h>
#include <twclient/image_cache.h>
//hack for now
/* #include <stb/stb_image.h> */

#ifdef __cplusplus
extern "C" {
#endif

struct nk_wl_image {
	struct wl_list link;
	struct nk_image image;
};

#if defined (NK_EGL_BACKEND)

static struct nk_image
nk_wl_to_gpu_image(const struct nk_image *, struct nk_wl_backend *);

static void
nk_wl_free_gpu_image(const struct nk_image *);

#endif


static inline struct nk_rect
nk_rect_from_bbox(const struct tw_bbox *box)
{
	return nk_rect(box->x , box->y,
	               box->w * box->s,
	               box->h * box->s);
}

/******************************************************************************
 * image loaders
 *****************************************************************************/

WL_EXPORT struct nk_image *
nk_wl_add_image(struct nk_image img, struct nk_wl_backend *b)
{
	struct nk_wl_image *image =
		calloc(1, sizeof(struct nk_wl_image));

	if (!image)
		return NULL;

	wl_list_init(&image->link);
	image->image = img;

	wl_list_insert(&b->images, &image->link);
	return &image->image;
}

struct nk_image
nk_wl_image_from_buffer(unsigned char *pixels, struct nk_wl_backend *b,
                        unsigned int width, unsigned int height,
                        unsigned int stride, bool take);


WL_EXPORT struct nk_image*
nk_wl_load_image(const char *path, enum wl_shm_format format,
                 struct nk_wl_backend *b)
{
	cairo_format_t cairo_format = tw_translate_wl_shm_format(format);
	if (cairo_format != CAIRO_FORMAT_ARGB32)
		return NULL;
	int width, height, nchannels;
	image_info(path, &width, &height, &nchannels);
	if (!width || !height || !nchannels)
		return NULL;

	unsigned char *mem = image_load(path, &width, &height, &nchannels);

	return nk_wl_add_image(nk_wl_image_from_buffer(mem, b,
	                                               width, height,
	                                               width * 4, true),
	                       b);
}

WL_EXPORT void
nk_wl_free_image(struct nk_image *im)
{
	struct nk_wl_image *wl_img =
		wl_container_of(im, wl_img, image);
	wl_list_remove(&wl_img->link);

#if defined (NK_EGL_BACKEND)
	nk_wl_free_gpu_image(im);
#else
	free(im->handle.ptr);
#endif
	free(wl_img);
}


#ifdef __cplusplus
}
#endif

#endif /* EOF */
