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

#include <objc/runtime.h>

#import "NSTimer+BMKitAdditions.h"


@implementation NSTimer (BMKitAdditions)


#pragma mark -
#pragma mark Creating a Timer


static void BMTimerBlockInvokeWithTimer(BMTimerBlock self, SEL _cmd, NSTimer *aTimer)
{
    self(aTimer);
}


static BMTimerBlock BMTimerBlockPrepare(BMTimerBlock aBlock)
{
    aBlock = [[aBlock copy] autorelease];
    if (aBlock) {
        // Instead of wrapping the block within a new class, we simply add a
        // method -BM_invokeWithTimer: to the block class (which is most
        // probably the one and only class for heap-allocated blocks).
        // If this fails, we check to see if we did already register the method
        // earlier; if someone else did (which should not happen), we bail out!
        Class class = object_getClass(aBlock);
        if (!class_addMethod(class,
                             @selector(BM_invokeWithTimer:),
                             (IMP)BMTimerBlockInvokeWithTimer,
                             "v@:@")) {
            IMP timerBlockFireWithTimer = class_getMethodImplementation(class, @selector(BM_invokeWithTimer:));
            if (!timerBlockFireWithTimer) {
                // Most probably out-of-memory, handle this gracefully...
                aBlock = nil;
            }
            else if (timerBlockFireWithTimer != (IMP)BMTimerBlockInvokeWithTimer) {
                // Someone messed with our internals, should not happen...
                [NSException raise:NSInternalInconsistencyException format:@"Invalid method implementation of -BM_invokeWithTimer: for class %s", class_getName(class)];
            }
        }
    }
    return aBlock;
}


+ (NSTimer *)scheduledTimerWithTimeInterval:(NSTimeInterval)seconds
                                      block:(BMTimerBlock)aBlock
                                   userInfo:(id)userInfo
                                    repeats:(BOOL)repeats
{
    NSTimer *timer = nil;
    aBlock = BMTimerBlockPrepare(aBlock);
    if (aBlock) {
        timer = [self scheduledTimerWithTimeInterval:seconds
                                              target:aBlock
                                            selector:@selector(BM_invokeWithTimer:)
                                            userInfo:userInfo
                                             repeats:repeats];
    }
    return timer;
}


+ (NSTimer *)timerWithTimeInterval:(NSTimeInterval)seconds
                             block:(BMTimerBlock)aBlock
                          userInfo:(id)userInfo
                           repeats:(BOOL)repeats
{
    NSTimer *timer = nil;
    aBlock = BMTimerBlockPrepare(aBlock);
    if (aBlock) {
        timer = [self timerWithTimeInterval:seconds
                                     target:aBlock
                                   selector:@selector(BM_invokeWithTimer:)
                                   userInfo:userInfo
                                    repeats:repeats];
    }
    return timer;
}


- (id)initWithFireDate:(NSDate *)date
              interval:(NSTimeInterval)seconds
                 block:(BMTimerBlock)aBlock
              userInfo:(id)userInfo
               repeats:(BOOL)repeats
{
    aBlock = BMTimerBlockPrepare(aBlock);
    if (aBlock) {
        self = [self initWithFireDate:date
                             interval:seconds
                               target:aBlock
                             selector:@selector(BM_invokeWithTimer:)
                             userInfo:userInfo
                              repeats:repeats];
    }
    else {
        [self release], self = nil;
    }
    return self;
}


@end

