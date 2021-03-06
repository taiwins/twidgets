/*
 * nk_backends.h - taiwins client nuklear backends header
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

#ifndef NK_BACKENDS_H
#define NK_BACKENDS_H

#include <stdbool.h>
#include <stdint.h>
#include <xkbcommon/xkbcommon.h>
#include <xkbcommon/xkbcommon-keysyms.h>

#include <twclient/ui.h>

struct nk_wl_backend;
struct nk_context;
struct nk_style;
struct tw_egl_env;

#define NK_ZERO_COMMAND_MEMORY
#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_FONT
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_KEYSTATE_BASED_INPUT

#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wsign-compare"
#pragma clang diagnostic ignored "-Wunused-variable"
#elif defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsign-compare"
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

#include "nuklear/nuklear.h"

#if defined(__clang__)
#pragma clang diagnostic pop
#elif defined(__GNUC__)
#pragma GCC diagnostic pop
#endif

struct xdg_toplevel;
struct xdg_surface;

typedef void (*nk_wl_drawcall_t)(struct nk_context *ctx,
                                 float width, float height,
                                 struct tw_appsurf *app);

/*******************************************************************************
 * shell implementation
 ******************************************************************************/

void
nk_wl_impl_xdg_toplevel(struct tw_appsurf *app,
                        struct xdg_toplevel *toplevel);
void
nk_wl_impl_wl_shell_surface(struct tw_appsurf *app,
                            struct wl_shell_surface *protocol);

/*******************************************************************************
 * backends
 ******************************************************************************/

/* cairo_backend */
struct nk_wl_backend *
nk_cairo_create_backend(void);

void
nk_cairo_destroy_backend(struct nk_wl_backend *bkend);

void
nk_cairo_impl_app_surface(struct tw_appsurf *surf, struct nk_wl_backend *bkend,
                          nk_wl_drawcall_t draw_cb,  const struct tw_bbox geo);


/* egl_backend */
struct nk_wl_backend*
nk_egl_create_backend(const struct wl_display *display);

void
nk_egl_destroy_backend(struct nk_wl_backend *b);

void
nk_egl_impl_app_surface(struct tw_appsurf *surf, struct nk_wl_backend *bkend,
                        nk_wl_drawcall_t draw_cb, const struct tw_bbox geo);

const struct tw_egl_env *
nk_egl_get_current_env(struct nk_wl_backend *b);

const struct nk_style *
nk_wl_get_curr_style(struct nk_wl_backend *bkend);

void
nk_wl_test_draw(struct nk_wl_backend *bkend, struct tw_appsurf *app,
		nk_wl_drawcall_t draw_call);


/*******************************************************************************
 * image loader
 ******************************************************************************/
/**
 * @brief create an image from malloced pixels (if asked)
 *
 * The pixels need to be This function takes away the pixel content you have.
 * This function does not add to nuklear backend automatically
 */
struct nk_image
nk_wl_image_from_buffer(unsigned char *pixels, struct nk_wl_backend *b,
                        unsigned int width, unsigned int height,
                        unsigned int stride, bool take);
/**
 * @brief adding a image from path to nuklear backend
 */
struct nk_image *
nk_wl_load_image(const char *path, enum wl_shm_format format,
                 struct nk_wl_backend *b);
/**
 * @brief adding a image to nuklear backend, backend will now take the ownership
 * of the image
 */
struct nk_image *
nk_wl_add_image(struct nk_image img, struct nk_wl_backend *b);

void
nk_wl_free_image(struct nk_image *img);


/*******************************************************************************
 * font loader
 ******************************************************************************/

enum nk_wl_font_slant {
	NK_WL_SLANT_ROMAN,
	NK_WL_SLANT_ITALIC,
	NK_WL_SLANT_OBLIQUE,
};

//we accept only true type font
struct nk_wl_font_config {
	const char *name;
	enum nk_wl_font_slant slant;
	int pix_size, scale;
	int nranges;
	const nk_rune **ranges;

	//fc config offers light, medium, demibold, bold, black
	//demibold, bold and black is true, otherwise false.
	bool bold; //classified as bold
	//private: TTF only
	bool TTFonly;
};


struct nk_user_font *
nk_wl_new_font(struct nk_wl_font_config config,
               struct nk_wl_backend *backend);

void
nk_wl_destroy_font(struct nk_user_font *font);

#endif
