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

#include <objc/runtime.h>

#import "NSObject+BMKitAdditions.h"


@implementation NSObject (BMKitAdditions)


#pragma mark -
#pragma mark Associative References


- (id)associatedObjectForKey:(BMAssociationKey)aKey
{
    return objc_getAssociatedObject(self, aKey);
}


- (void)setAssociatedObject:(id)anObject forKey:(BMAssociationKey)aKey
{
    objc_setAssociatedObject(self, aKey, anObject, OBJC_ASSOCIATION_RETAIN_NONATOMIC);
}


- (void)setAssociatedObject:(id)anObject forKey:(BMAssociationKey)aKey policy:(BMAssociationPolicy)policy
{
    switch (policy) {
        case BMAssociationAssignPolicy:
            policy = (BMAssociationPolicy)OBJC_ASSOCIATION_ASSIGN;
            break;
            
        case BMAssociationAtomicCopyPolicy:
            policy = (BMAssociationPolicy)OBJC_ASSOCIATION_COPY;
            break;
            
        case BMAssociationAtomicRetainPolicy:
            policy = (BMAssociationPolicy)OBJC_ASSOCIATION_RETAIN;
            break;
            
        case BMAssociationNonatomicCopyPolicy:
            policy = (BMAssociationPolicy)OBJC_ASSOCIATION_COPY_NONATOMIC;
            break;
            
        case BMAssociationNonatomicRetainPolicy:
            policy = (BMAssociationPolicy)OBJC_ASSOCIATION_RETAIN_NONATOMIC;
            break;
    }
    objc_setAssociatedObject(self, aKey, anObject, policy);
}


- (void)removeAssociatedObjects
{
    objc_removeAssociatedObjects(self);
}


#pragma mark -
#pragma mark Performing Blocks


- (void)BM_invokeTargetBlock:(BMTargetBlock)aBlock
{
    if (aBlock) {
        aBlock(self);
    }
}


- (void)BM_invokeTargetBlockWithAutoreleasePool:(BMTargetBlock)aBlock
{
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    [self BM_invokeTargetBlock:aBlock];
    [pool drain];
}


- (BMTargetBlock)performBlock:(BMTargetBlock)aBlock afterDelay:(NSTimeInterval)delay
{
    aBlock = [aBlock copy];
    [self performSelector:@selector(BM_invokeTargetBlock:) withObject:aBlock afterDelay:delay];
    return [aBlock autorelease];
}


- (BMTargetBlock)performBlock:(BMTargetBlock)aBlock afterDelay:(NSTimeInterval)delay inModes:(NSArray *)modes
{
    aBlock = [aBlock copy];
    [self performSelector:@selector(BM_invokeTargetBlock:) withObject:aBlock afterDelay:delay inModes:modes];
    return [aBlock autorelease];
}


- (void)performBlockOnMainThread:(BMTargetBlock)aBlock waitUntilDone:(BOOL)wait
{
    if (!wait) {
        aBlock = [aBlock copy];
    }
    [self performSelectorOnMainThread:@selector(BM_invokeTargetBlock:) withObject:aBlock waitUntilDone:wait];
    if (!wait) {
        [aBlock release];
    }
}


- (void)performBlockOnMainThread:(BMTargetBlock)aBlock waitUntilDone:(BOOL)wait modes:(NSArray *)modes
{
    if (!wait) {
        aBlock = [aBlock copy];
    }
    [self performSelectorOnMainThread:@selector(BM_invokeTargetBlock:) withObject:aBlock waitUntilDone:wait modes:modes];
    if (!wait) {
        [aBlock release];
    }
}


- (void)performBlock:(BMTargetBlock)aBlock onThread:(NSThread *)aThread waitUntilDone:(BOOL)wait
{
    if (!wait) {
        aBlock = [aBlock copy];
    }
    [self performSelector:@selector(BM_invokeTargetBlock:) onThread:aThread withObject:aBlock waitUntilDone:wait];
    if (!wait) {
        [aBlock release];
    }
}


- (void)performBlock:(BMTargetBlock)aBlock onThread:(NSThread *)aThread waitUntilDone:(BOOL)wait modes:(NSArray *)modes
{
    if (!wait) {
        aBlock = [aBlock copy];
    }
    [self performSelector:@selector(BM_invokeTargetBlock:) onThread:aThread withObject:aBlock waitUntilDone:wait modes:modes];
    if (!wait) {
        [aBlock release];
    }
}


- (void)performBlockInBackground:(BMTargetBlock)aBlock
{
    aBlock = [aBlock copy];
    [self performSelectorInBackground:@selector(BM_invokeTargetBlockWithAutoreleasePool:) withObject:aBlock];
    [aBlock release];
}


- (void)performBlock:(BMTargetBlock)aBlock onQueue:(dispatch_queue_t)aQueue waitUntilDone:(BOOL)wait
{
    if (!aQueue) {
        [NSException raise:NSInvalidArgumentException
                    format:@"aQueue is NULL (in '%@')", NSStringFromSelector(_cmd)];
    }
    if (wait) {
        dispatch_sync(aQueue, ^{
            [self BM_invokeTargetBlock:aBlock];
        });
    }
    else {
        aBlock = [aBlock copy];
        dispatch_async(aQueue, ^{
            [self BM_invokeTargetBlockWithAutoreleasePool:aBlock];
        });
        [aBlock release];
    }
}


+ (void)cancelPreviousPerformRequestsWithTarget:(id)aTarget block:(BMTargetBlock)aBlock
{
    if (aTarget && aBlock) {
        [self cancelPreviousPerformRequestsWithTarget:aTarget selector:@selector(BM_invokeTargetBlock:) object:aBlock];
    }
}


#pragma mark -
#pragma mark Sending Messages


- (void)performSelector:(SEL)aSelector onQueue:(dispatch_queue_t)aQueue withObject:(id)anObject waitUntilDone:(BOOL)wait
{
    if (!aSelector) {
        [NSException raise:NSInvalidArgumentException
                    format:@"aSelector is NULL (in '%@')", NSStringFromSelector(_cmd)];
    }
    [self performBlock:^(id self) {
        [self performSelector:aSelector withObject:anObject];
    } onQueue:aQueue waitUntilDone:wait];
}


@end
