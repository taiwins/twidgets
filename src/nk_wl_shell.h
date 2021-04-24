/*
 * nk_wl_shell.h - taiwins client nuklear shell functions
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

#ifndef NK_WL_SHELL_IMPL_H
#define NK_WL_SHELL_IMPL_H

#ifdef __cplusplus
extern "C" {
#endif

#include <wayland-xdg-shell-client-protocol.h>
#include "nk_wl_internal.h"

static void
nk_wl_shell_handle_ping(void *data,
		     struct wl_shell_surface *wl_shell_surface,
		     uint32_t serial)
{
	wl_shell_surface_pong(wl_shell_surface, serial);
}


static void
nk_wl_shell_handle_popup_done(void *data, struct wl_shell_surface *shell_surface)
{
}

static void
nk_wl_shell_handle_configure(void *data, struct wl_shell_surface *shell_surface,
		 uint32_t edges, int32_t width, int32_t height)
{
	struct tw_appsurf *app = data;
	tw_appsurf_resize(app, width, height, app->allocation.s);
}

static struct wl_shell_surface_listener nk_wl_shell_impl = {
	.ping = nk_wl_shell_handle_ping,
	.configure = nk_wl_shell_handle_configure,
	.popup_done = nk_wl_shell_handle_popup_done
};


WL_EXPORT void
nk_wl_impl_wl_shell_surface(struct tw_appsurf *app,
			    struct wl_shell_surface *protocol)
{
	wl_shell_surface_add_listener(protocol,
				      &nk_wl_shell_impl, app);
}

static void
nk_wl_impl_xdg_configure(void *data,
			 struct xdg_toplevel *xdg_toplevel,
			 int32_t width,
			 int32_t height,
			 struct wl_array *states)
{
	struct tw_appsurf *app = data;

	if (!width || !height)
		return;
	tw_appsurf_resize(app, width, height, app->allocation.s);
}

static void
nk_wl_xdg_toplevel_close(void *data,
			 struct xdg_toplevel *xdg_toplevel)
{
	struct xdg_surface *xdg_surface = data;
	struct tw_appsurf *app =
		xdg_surface_get_user_data(xdg_surface);

	xdg_toplevel_destroy(xdg_toplevel);
	xdg_surface_destroy(xdg_surface);
	tw_appsurf_release(app);
}

static struct xdg_toplevel_listener nk_wl_xdgtoplevel_impl =  {
	.configure = nk_wl_impl_xdg_configure,
	.close = nk_wl_xdg_toplevel_close,
};


WL_EXPORT void
nk_wl_impl_xdg_toplevel(struct tw_appsurf *app,
                        struct xdg_toplevel *toplevel)
{
	xdg_toplevel_add_listener(toplevel, &nk_wl_xdgtoplevel_impl,
	                          app);
}

#ifdef __cplusplus
}
#endif

#endif /* EOF */
