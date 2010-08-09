/*
 * libshveu: A library for controlling SH-Mobile VEU
 * Copyright (C) 2009 Renesas Technology Corp.
 * Copyright (C) 2010 Renesas Electronics Corporation
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston MA  02110-1301 USA
 */

/** \file
 * Image/Video processing: Scale, rotate, crop, color conversion
 * Note: When using V4L2_PIX_FMT_RGB32 format, the data it treated as XRGB888,
 * i.e. the most significant byte is ignored.
 */

#ifndef __VEU_COLORSPACE_H__
#define __VEU_COLORSPACE_H__

#include <linux/videodev2.h>	/* For pixel formats */


/** Rotation */
typedef enum {
	SHVEU_NO_ROT=0,	/**< No rotation */
	SHVEU_ROT_90,	/**< Rotate 90 degrees clockwise */
} shveu_rotation_t;

/** Start a (scale|rotate) & crop between YCbCr & RGB surfaces
 * \param veu VEU handle
 * \param src_py Physical address of Y or RGB plane of source image
 * \param src_pc Physical address of CbCr plane of source image (ignored for RGB)
 * \param src_width Width in pixels of source image
 * \param src_height Height in pixels of source image
 * \param src_pitch Line pitch of source image
 * \param src_fmt Format of source image. One of:
 *		V4L2_PIX_FMT_NV12
 *		V4L2_PIX_FMT_NV16
 *		V4L2_PIX_FMT_RGB565
 *		V4L2_PIX_FMT_RGB32
 * \param dst_py Physical address of Y or RGB plane of destination image
 * \param dst_pc Physical address of CbCr plane of destination image (ignored for RGB)
 * \param dst_width Width in pixels of destination image
 * \param dst_height Height in pixels of destination image
 * \param dst_pitch Line pitch of destination image
 * \param dst_fmt Format of destination image. One of:
 *		V4L2_PIX_FMT_NV12
 *		V4L2_PIX_FMT_NV16
 *		V4L2_PIX_FMT_RGB565
 *		V4L2_PIX_FMT_RGB32
 * \param rotate Rotation to apply
 * \retval 0 Success
 * \retval -1 Error: Attempt to perform simultaneous scaling and rotation
 */
int
shveu_start_locked(
	SHVEU *veu,
	unsigned long src_py,
	unsigned long src_pc,
	unsigned long src_width,
	unsigned long src_height,
	unsigned long src_pitch,
	int src_fmt,
	unsigned long dst_py,
	unsigned long dst_pc,
	unsigned long dst_width,
	unsigned long dst_height,
	unsigned long dst_pitch,
	int dst_fmt,
	shveu_rotation_t rotate);

/** Wait for a VEU operation to complete. The operation is started by a call to shveu_start.
 * \param veu VEU handle
 */
void
shveu_wait(SHVEU *veu);

/** Perform scale & crop between YCbCr & RGB surfaces
 * \param veu VEU handle
 * \param src_py Physical address of Y or RGB plane of source image
 * \param src_pc Physical address of CbCr plane of source image (ignored for RGB)
 * \param src_width Width in pixels of source image
 * \param src_height Height in pixels of source image
 * \param src_fmt Format of source image. One of:
 *		V4L2_PIX_FMT_NV12
 *		V4L2_PIX_FMT_NV16
 *		V4L2_PIX_FMT_RGB565
 *		V4L2_PIX_FMT_RGB32
 * \param dst_py Physical address of Y or RGB plane of destination image
 * \param dst_pc Physical address of CbCr plane of destination image (ignored for RGB)
 * \param dst_width Width in pixels of destination image
 * \param dst_height Height in pixels of destination image
 * \param dst_fmt Format of destination image. One of:
 *		V4L2_PIX_FMT_NV12
 *		V4L2_PIX_FMT_NV16
 *		V4L2_PIX_FMT_RGB565
 *		V4L2_PIX_FMT_RGB32
 * \retval 0 Success
 * \retval -1 Error: Unsupported parameters
 */
int
shveu_rescale(
	SHVEU *veu,
	unsigned long src_py,
	unsigned long src_pc,
	unsigned long src_width,
	unsigned long src_height,
	int src_fmt,
	unsigned long dst_py,
	unsigned long dst_pc,
	unsigned long dst_width,
	unsigned long dst_height,
	int dst_fmt);

/** Perform rotate & crop between YCbCr & RGB surfaces
 * \param veu VEU handle
 * \param src_py Physical address of Y or RGB plane of source image
 * \param src_pc Physical address of CbCr plane of source image (ignored for RGB)
 * \param src_width Width in pixels of source image
 * \param src_height Height in pixels of source image
 * \param src_fmt Format of source image. One of:
 *		V4L2_PIX_FMT_NV12
 *		V4L2_PIX_FMT_NV16
 *		V4L2_PIX_FMT_RGB565
 *		V4L2_PIX_FMT_RGB32
 * \param dst_py Physical address of Y or RGB plane of destination image
 * \param dst_pc Physical address of CbCr plane of destination image (ignored for RGB)
 * \param dst_fmt Format of destination image. One of:
 *		V4L2_PIX_FMT_NV12
 *		V4L2_PIX_FMT_NV16
 *		V4L2_PIX_FMT_RGB565
 *		V4L2_PIX_FMT_RGB32
 * \param rotate Rotation to apply
 * \retval 0 Success
 * \retval -1 Error: Unsupported parameters
 */
int
shveu_rotate(
	SHVEU *veu,
	unsigned long src_py,
	unsigned long src_pc,
	unsigned long src_width,
	unsigned long src_height,
	int src_fmt,
	unsigned long dst_py,
	unsigned long dst_pc,
	int dst_fmt,
	shveu_rotation_t rotate);


/** Set cropping attributes.
 * This is called prior to calling shveu_rescale, shveu_rotate, or shveu_start_locked.
 * \param veu VEU handle
 * \param crop_dst Set cropping attributes for source(=1) or destination(=0)
 * \param x1 Horizontal offset to left of image selection
 * \param x1 Horizontal offset to left of image selection
 * \param y1 Vertical offset to top of image selection
 * \param x2 Horizontal offset to right of image selection
 * \param y2 Vertical offset to bottom of image selection
 * \retval 0 Success
 * \retval -1 Error: Unsupported parameters
 */
void
shveu_crop (
	SHVEU *veu,
	int crop_dst,
	int x1,
	int y1,
	int x2,
	int y2);


#endif				/* __VEU_COLORSPACE_H__ */
