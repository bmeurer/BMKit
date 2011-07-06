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

#import "UIImage+BMKitAdditions.h"


BMImageOrientation BMImageOrientationFromUIImageOrientation(UIImageOrientation imageOrientation)
{
    switch (imageOrientation) {
        case UIImageOrientationUp: default:     return BMImageOrientationUp;
        case UIImageOrientationUpMirrored:      return BMImageOrientationUpMirrored;
        case UIImageOrientationDown:            return BMImageOrientationDown;
        case UIImageOrientationDownMirrored:    return BMImageOrientationDownMirrored;
        case UIImageOrientationLeft:            return BMImageOrientationLeft;
        case UIImageOrientationLeftMirrored:    return BMImageOrientationLeftMirrored;
        case UIImageOrientationRight:           return BMImageOrientationRight;
        case UIImageOrientationRightMirrored:   return BMImageOrientationRightMirrored;
    }
}


UIImageOrientation UIImageOrientationFromBMImageOrientation(BMImageOrientation imageOrientation)
{
    switch (imageOrientation) {
        case BMImageOrientationUp: default:     return UIImageOrientationUp;
        case BMImageOrientationUpMirrored:      return UIImageOrientationUpMirrored;
        case BMImageOrientationDown:            return UIImageOrientationDown;
        case BMImageOrientationDownMirrored:    return UIImageOrientationDownMirrored;
        case BMImageOrientationLeft:            return UIImageOrientationLeft;
        case BMImageOrientationLeftMirrored:    return UIImageOrientationLeftMirrored;
        case BMImageOrientationRight:           return UIImageOrientationRight;
        case BMImageOrientationRightMirrored:   return UIImageOrientationRightMirrored;
    }
}


@implementation UIImage (BMKitAdditions)


- (UIImage *)imageRotatedToUpOrientation
{
    UIImage *image = nil;
    CGImageRef CGImage = BMImageCreateWithImageInOrientation(self.CGImage,
                                                             BMImageOrientationFromUIImageOrientation(self.imageOrientation));
    if (CGImage) {
        image = [[self class] imageWithCGImage:CGImage
                                         scale:self.scale
                                   orientation:UIImageOrientationUp];
        CFRelease(CGImage);
    }
    return image;
}


@end

