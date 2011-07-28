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

#import <Foundation/Foundation.h>


/** BMKit related additions to the `NSEnumerator` class. */
@interface NSEnumerator (BMKitAdditions)

///----------------------------------------------
/// @name Sending Messages to Enumerated Objects
///----------------------------------------------

/** Sends to each object the receiver has yet to enumerate the message identified by a given selector.
 
 Put another way, the message is not send to objects that have already been enumerated with previous `nextObject` messages.

 Invoking this method exhausts the enumerator’s collection so that subsequent invocations of `nextObject` return `nil`.
 
 If the _aSelector_ parameter is `NULL` this method will raise an exception.
 
 @param aSelector A selector that identifies the message to send to the objects the receiver has yet to enumerate. The method must not take any arguments, and must not have the side effect of modifying the enumerator's collection.
 @see makeObjectsPerformSelector:withObject:
 @see enumerateObjectsUsingBlock:
 */
- (void)makeObjectsPerformSelector:(SEL)aSelector;

/** Sends to each object the receiver has yet to enumerate the message identified by a given selector.
 
 Put another way, the message is not send to objects that have already been enumerated with previous `nextObject` messages.
 
 Invoking this method exhausts the enumerator’s collection so that subsequent invocations of `nextObject` return `nil`.
 
 If the _aSelector_ parameter is `NULL` this method will raise an exception.

 @param aSelector A selector that identifies the message to send to the objects the receiver has yet to enumerate. The method must take a single argument of type id, and must not have the side effect of modifying the enumerator's collection.
 @param anObject The object to send as the argument to each invocation of the *aSelector* method.
 @see makeObjectsPerformSelector:
 @see enumerateObjectsUsingBlock:
 */
- (void)makeObjectsPerformSelector:(SEL)aSelector withObject:(id)anObject;

/** Executes a given block using each object the receiver has yet to enumerate.
 
 Put another way, the block is not executed with objects that have already been enumerated with previous `nextObject` messages.
 
 Invoking this method exhausts the enumerator’s collection so that subsequent invocations of `nextObject` return `nil`.
 
 If the _aBlock_ parameter is `nil` this method will raise an exception.
 
 @param aBlock The block to apply to the objects the receiver has yet to enumerate.
 @see makeObjectsPerformSelector:
 @see makeObjectsPerformSelector:withObject:
 */
- (void)enumerateObjectsUsingBlock:(void (^)(id obj, BOOL *stop))aBlock;

@end
