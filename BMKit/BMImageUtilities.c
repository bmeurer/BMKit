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

#include <ImageIO/ImageIO.h>
#include <MobileCoreServices/MobileCoreServices.h>

#include "BMImageUtilities.h"


CFDataRef BMImageCopyJPEGData(CGImageRef         image,
                              BMImageOrientation imageOrientation,
                              float              imageQuality)
{
    CFDataRef JPEGData = NULL;
    if (image) {
        CFAllocatorRef allocator = CFGetAllocator(image);
        CFMutableDataRef data = CFDataCreateMutable(allocator, 0);
        if (data) {
            CGImageDestinationRef destination = CGImageDestinationCreateWithData(data, kUTTypeJPEG, 1, NULL);
            if (destination) {
                CFStringRef keys[2];
                CFTypeRef values[2];
                keys[0] = kCGImagePropertyOrientation;
                values[0] = CFNumberCreate(allocator, kCFNumberIntType, &imageOrientation);
                keys[1] = kCGImageDestinationLossyCompressionQuality;
                values[1] = CFNumberCreate(allocator, kCFNumberFloatType, &imageQuality);
                if (values[0] && values[1]) {
                    CFDictionaryRef properties = CFDictionaryCreate(allocator,
                                                                    (const void **)keys,
                                                                    (const void **)values,
                                                                    2,
                                                                    &kCFTypeDictionaryKeyCallBacks,
                                                                    &kCFTypeDictionaryValueCallBacks);
                    if (properties) {
                        CGImageDestinationAddImage(destination, image, properties);
                        if (CGImageDestinationFinalize(destination)) {
                            JPEGData = CFDataCreateCopy(allocator, data);
                        }
                        CFRelease(properties);
                    }
                }
                if (values[1]) CFRelease(values[1]);
                if (values[0]) CFRelease(values[0]);
                CFRelease(destination);
            }
            CFRelease(data);
        }
    }
    return JPEGData;
}


CGImageRef BMImageCreateWithImageInOrientation(CGImageRef         image,
                                               BMImageOrientation imageOrientation)
{
    CGImageRef transformedImage = NULL;
    if (image) {
        CGRect imageRect = CGRectMake((CGFloat)0.0f,
                                      (CGFloat)0.0f,
                                      CGImageGetWidth(image),
                                      CGImageGetHeight(image));
        
        // Figure out the image transformation to apply
        CGAffineTransform transform = CGAffineTransformIdentity;
        switch (imageOrientation) {
            case BMImageOrientationDown: // 0th row is at the bottom, and 0th column is on the right - Rotate 180 degrees
                transform = CGAffineTransformMake(-1.0f, 0.0f, 0.0f, -1.0f, imageRect.size.width, imageRect.size.height);
                break;
                
            case BMImageOrientationLeft: // 0th row is on the left, and 0th column is the bottom - Rotate -90 degrees
                transform = CGAffineTransformMake(0.0f, 1.0f, -1.0f, 0.0f, imageRect.size.height, 0.0f);
                break;
                
            case BMImageOrientationRight: // 0th row is on the right, and 0th column is the top - Rotate 90 degrees
                transform = CGAffineTransformMake(0.0f, -1.0f, 1.0f, 0.0f, 0.0f, imageRect.size.width);
                break;
                
            case BMImageOrientationUpMirrored: // 0th row is at the top, and 0th column is on the right - Flip Horizontal
                transform = CGAffineTransformMake(-1.0f, 0.0f, 0.0f, 1.0f, imageRect.size.width, 0.0f);
                break;
                
            case BMImageOrientationDownMirrored: // 0th row is at the bottom, and 0th column is on the left - Flip Vertical
                transform = CGAffineTransformMake(1.0f, 0.0f, 0.0f, -1.0f, 0.0f, imageRect.size.height);
                break;
                
            case BMImageOrientationLeftMirrored: // 0th row is on the left, and 0th column is the top - Rotate -90 degrees and Flip Vertical
                transform = CGAffineTransformMake(0.0f, -1.0f, -1.0f, 0.0f, imageRect.size.height, imageRect.size.width);
                break;
                
            case BMImageOrientationRightMirrored: // 0th row is on the right, and 0th column is the bottom - Rotate 90 degrees and Flip Vertical
                transform = CGAffineTransformMake(0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f);
                break;
                
            default:
                break;
        }
        
        if (!CGAffineTransformIsIdentity(transform)) {
            // Figure out the transformed image size
            CGSize transformedSize = imageRect.size;
            switch (imageOrientation) {
                case BMImageOrientationLeft:
                case BMImageOrientationLeftMirrored:
                case BMImageOrientationRight:
                case BMImageOrientationRightMirrored:
                    // Image will be transposed, swap width and height
                    transformedSize = CGSizeMake(transformedSize.height,
                                                 transformedSize.width);
                    break;
                    
                default:
                    break;
            }
            
            // Render the transformed image
            CGContextRef context = CGBitmapContextCreate(NULL,
                                                         transformedSize.width,
                                                         transformedSize.height,
                                                         CGImageGetBitsPerComponent(image),
                                                         0,
                                                         CGImageGetColorSpace(image),
                                                         CGImageGetBitmapInfo(image));
            if (context) {
                CGContextConcatCTM(context, transform);
                CGContextDrawImage(context, imageRect, image);
                transformedImage = CGBitmapContextCreateImage(context);
                CFRelease(context);
            }
        }
        else {
            // No need to transform the image, already in "Up" orientation
            transformedImage = CGImageRetain(image);
        }
    }
    return transformedImage;
}


CGImageRef BMImageCreateWithImageScaled(CGImageRef             image,
                                        CGFloat                scaleWidth,
                                        CGFloat                scaleHeight,
                                        CGInterpolationQuality interpolationQuality)
{
    CGImageRef scaledImage = NULL;
    if (image) {
        CGSize imageSize = CGSizeMake(CGImageGetWidth(image),
                                      CGImageGetHeight(image));
        CGRect scaledRect = CGRectIntegral(CGRectMake((CGFloat)0.0f,
                                                      (CGFloat)0.0f,
                                                      imageSize.width * scaleWidth,
                                                      imageSize.height * scaleHeight));
        if (CGSizeEqualToSize(scaledRect.size, imageSize)) {
            // No need to actually scale the image
            scaledImage = CGImageRetain(image);
        }
        else if (!CGRectIsEmpty(scaledRect) && !CGRectIsInfinite(scaledRect)) {
            CGContextRef context = CGBitmapContextCreate(NULL,
                                                         scaledRect.size.width,
                                                         scaledRect.size.height,
                                                         CGImageGetBitsPerComponent(image),
                                                         0,
                                                         CGImageGetColorSpace(image),
                                                         CGImageGetBitmapInfo(image));
            if (context) {
                CGContextSetInterpolationQuality(context, interpolationQuality);
                CGContextDrawImage(context, scaledRect, image);
                scaledImage = CGBitmapContextCreateImage(context);
                CFRelease(context);
            }
        }
    }
    return scaledImage;
}


CGImageRef BMImageCreateWithImageScaledDownToAspectFill(CGImageRef             image,
                                                        CGSize                 size,
                                                        CGInterpolationQuality interpolationQuality)
{
    CGImageRef scaledImage = NULL;
    if (image && size.width > (CGFloat)0.0f && size.height > (CGFloat)0.0f) {
        CGRect imageRect = CGRectMake((CGFloat)0.0f,
                                      (CGFloat)0.0f,
                                      CGImageGetWidth(image),
                                      CGImageGetHeight(image));
        if (!CGRectIsEmpty(imageRect)) {
            // Scale down the image to "aspect fill" the requested size
            CGFloat scaleWidth = size.width / imageRect.size.width;
            CGFloat scaleHeight = size.height / imageRect.size.height;
            CGFloat scale = (scaleWidth > scaleHeight) ? scaleWidth : scaleHeight;
            if (scale > (CGFloat)1.0f) {
                scale = (CGFloat)1.0f;
            }
            scaledImage = BMImageCreateWithImageScaled(image, scale, scale, interpolationQuality);
            if (scaledImage) {
                CGSize scaledSize = CGSizeMake(CGImageGetWidth(scaledImage),
                                               CGImageGetHeight(scaledImage));
                if (scaledSize.width > size.width || scaledSize.height > size.height) {
                    // Crop the requested part of the scaled image
                    CGRect croppedRect = CGRectIntegral(CGRectMake((CGFloat)0.0f,
                                                                   (CGFloat)0.0f,
                                                                   size.width,
                                                                   size.height));
                    if (croppedRect.size.width < scaledSize.width) {
                        croppedRect.origin.x += (scaledSize.width - croppedRect.size.width) / (CGFloat)2.0f;
                    }
                    else {
                        croppedRect.size.width = scaledSize.width;
                    }
                    if (croppedRect.size.height < scaledSize.height) {
                        croppedRect.origin.y += (scaledSize.height - croppedRect.size.height) / (CGFloat)2.0f;
                    }
                    else {
                        croppedRect.size.height = scaledSize.height;
                    }
                    
                    CGImageRef croppedImage = CGImageCreateWithImageInRect(scaledImage, croppedRect);
                    CGImageRelease(scaledImage);
                    scaledImage = croppedImage;
                }
            }
        }
    }
    return scaledImage;
}


