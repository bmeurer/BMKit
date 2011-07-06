/*-
 * Copyright (c) 2010-2011, Benedikt Meurer <benedikt.meurer@googlemail.com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#ifndef __BMIMAGEUTILITIES__
#define __BMIMAGEUTILITIES__

#include <CoreGraphics/CoreGraphics.h>

#ifdef __cplusplus
extern "C" {
#endif
    
/** EXIF image orientations. */
typedef enum _BMImageOrientation {
    BMImageOrientationUp            = 1,
    BMImageOrientationUpMirrored    = 2,
    BMImageOrientationDown          = 3,
    BMImageOrientationDownMirrored  = 4,
    BMImageOrientationLeftMirrored  = 5,
    BMImageOrientationLeft          = 6,
    BMImageOrientationRightMirrored = 7,
    BMImageOrientationRight         = 8
} BMImageOrientation;
    
/** Returns the JPEG data of the specified image with the given compression quality and EXIF orientation. */
extern CFDataRef BMImageCopyJPEGData(CGImageRef         image,
                                     BMImageOrientation imageOrientation,
                                     float              imageQuality);
    
/** Creates an image by rotating another image to "Up" orientation. */
extern CGImageRef BMImageCreateWithImageInOrientation(CGImageRef         image,
                                                      BMImageOrientation imageOrientation);

/** Creates an image by scaling another image. */
extern CGImageRef BMImageCreateWithImageScaled(CGImageRef             image,
                                               CGFloat                scaleWidth,
                                               CGFloat                scaleHeight,
                                               CGInterpolationQuality interpolationQuality);

/** Creates an image by scaling down another image using "aspect fill" strategy. */
extern CGImageRef BMImageCreateWithImageScaledDownToAspectFill(CGImageRef             image,
                                                               CGSize                 size,
                                                               CGInterpolationQuality interpolationQuality);

#ifdef __cplusplus
}
#endif

#endif /* __BMIMAGEUTILITIES__ */
