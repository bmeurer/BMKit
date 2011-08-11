/*-
 * Copyright (c) 2011, Benedikt Meurer <benedikt.meurer@googlemail.com>
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

#import "NSObject+BMKitAdditions.h"
#import "UIGestureRecognizer+BMKitAdditions.h"


@implementation UIGestureRecognizer (BMKitAdditions)

static const char *const BMGestureRecognizerBlockArrayKey = "BMGestureRecognizerBlockArrayKey";


#pragma mark -
#pragma mark Initializing a Gesture Recognizer


- (id)initWithBlock:(BMGestureRecognizerBlock)gestureRecognizerBlock
{
    self = [self init];
    [self addBlock:gestureRecognizerBlock];
    return self;
}


#pragma mark -
#pragma mark Adding and Removing Blocks


static void BMGestureRecognizerBlockInvokeWithGestureRecognizer(BMGestureRecognizerBlock self, SEL _cmd, UIGestureRecognizer *gestureRecognizer)
{
    self(gestureRecognizer);
}


static BMGestureRecognizerBlock BMGestureRecognizerBlockPrepare(BMGestureRecognizerBlock aBlock)
{
    aBlock = [[aBlock copy] autorelease];
    if (aBlock) {
        // Instead of wrapping the block within a new class, we simply add a
        // method -BM_invokeWithGestureRecognizer: to the block class (which is
        // most probably the one and only class for heap-allocated blocks).
        // If this fails, we check to see if we did already register the method
        // earlier; if someone else did (which should not happen), we bail out!
        Class class = object_getClass(aBlock);
        if (!class_addMethod(class,
                             @selector(BM_invokeWithGestureRecognizer:),
                             (IMP)BMGestureRecognizerBlockInvokeWithGestureRecognizer,
                             "v@:@")) {
            IMP gestureRecognizerBlockInvokeWithGestureRecognizer = class_getMethodImplementation(class, @selector(BM_invokeWithGestureRecognizer:));
            if (!gestureRecognizerBlockInvokeWithGestureRecognizer) {
                // Most probably out-of-memory, handle this gracefully...
                aBlock = nil;
            }
            else if (gestureRecognizerBlockInvokeWithGestureRecognizer != (IMP)BMGestureRecognizerBlockInvokeWithGestureRecognizer) {
                // Someone messed with our internals, should not happen...
                [NSException raise:NSInternalInconsistencyException format:@"Invalid method implementation of -BM_invokeWithGestureRecognizer: for class %s", class_getName(class)];
            }
        }
    }
    return aBlock;
}


- (void)addBlock:(BMGestureRecognizerBlock)gestureRecognizerBlock
{
    gestureRecognizerBlock = BMGestureRecognizerBlockPrepare(gestureRecognizerBlock);
    if (gestureRecognizerBlock) {
        NSMutableArray *blocks = [self associatedObjectForKey:BMGestureRecognizerBlockArrayKey];
        if (!blocks) {
            blocks = [NSMutableArray array];
            [blocks addObject:gestureRecognizerBlock];
            [self setAssociatedObject:blocks forKey:BMGestureRecognizerBlockArrayKey];
        }
        else if ([blocks indexOfObjectIdenticalTo:gestureRecognizerBlock] == NSNotFound) {
            [blocks addObject:gestureRecognizerBlock];
        }
        [self addTarget:gestureRecognizerBlock action:@selector(BM_invokeWithGestureRecognizer:)];
    }
}


- (void)removeBlock:(BMGestureRecognizerBlock)gestureRecognizerBlock
{
    [self removeTarget:gestureRecognizerBlock action:NULL];
    if (gestureRecognizerBlock) {
        NSMutableArray *blocks = [self associatedObjectForKey:BMGestureRecognizerBlockArrayKey];
        [blocks removeObjectIdenticalTo:gestureRecognizerBlock];
    }
}


@end
