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

#import "NSArray+BMKitAdditions.h"


@implementation NSArray (BMKitAdditions)


#pragma mark -
#pragma mark Performing Blocks on Elements


- (void)makeObjectsPerformBlock:(BMTargetBlock)aBlock
{
    if (!aBlock) {
        [NSException raise:NSInvalidArgumentException
                    format:@"aBlock is nil (in '%@')", NSStringFromSelector(_cmd)];
    }
    for (id object in self) {
        aBlock(object);
    }
}


#pragma mark -
#pragma mark Deriving New Arrays


- (NSArray *)filteredArrayUsingPredicateBlock:(BMPredicateBlock)aBlock
{
    NSArray *filteredArray = nil;
    if (aBlock) {
        NSUInteger i, j, numberOfObjects = [self count];
        id *objects = (id *)malloc(numberOfObjects * sizeof(id));
        if (objects) {
            [self getObjects:objects range:NSMakeRange(0, numberOfObjects)];
            for (i = j = 0; i < numberOfObjects; ++i) {
                id object = objects[i];
                if (aBlock(object)) {
                    objects[j++] = object;
                }
            }
            filteredArray = [NSArray arrayWithObjects:objects count:j];
            free(objects);
        }
    }
    return filteredArray;
}


- (NSArray *)transformedArrayUsingTransformator:(BMTransformator)aTransformator
{
    NSArray *transformedArray = nil;
    if (aTransformator) {
        NSUInteger i, numberOfObjects = [self count];
        id *objects = (id *)malloc(numberOfObjects * sizeof(id));
        if (objects) {
            [self getObjects:objects range:NSMakeRange(0, numberOfObjects)];
            for (i = 0; i < numberOfObjects; ++i) {
                objects[i] = aTransformator(objects[i]) ?: [NSNull null];
            }
            transformedArray = [NSArray arrayWithObjects:objects count:numberOfObjects];
            free(objects);
        }
    }
    return transformedArray;
}


@end
