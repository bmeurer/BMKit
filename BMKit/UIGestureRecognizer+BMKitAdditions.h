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

#import <UIKit/UIKit.h>


typedef void (^BMGestureRecognizerBlock)(UIGestureRecognizer *gestureRecognizer);

/** BMKit related additions to the `UIGestureRecognizer` class. */
@interface UIGestureRecognizer (BMKitAdditions)

///-----------------------------------------
/// @name Initializing a Gesture Recognizer
///-----------------------------------------

/** Initializes an allocated gesture-recognizer object with a block.
 
 @param gestureRecognizerBlock A block that is the recipient of action messages sent by the receiver when it recognizes a gesture.
 @return An initialized instance of a concrete `UIGestureRecognizer` subclass or `nil` if an error occurred in the attempt to initialize the object.
 @see addBlock:
 @see removeBlock:
 */
- (id)initWithBlock:(BMGestureRecognizerBlock)gestureRecognizerBlock;

///----------------------------------
/// @name Adding and Removing Blocks
///----------------------------------

/** Adds a block to a gesture-recognizer object.
 
 @param gestureRecognizerBlock A block that is the recipient of action messages sent by the receiver when it recognizes a gesture.
 @see initWithBlock:
 @see removeBlock:
 */
- (void)addBlock:(BMGestureRecognizerBlock)gestureRecognizerBlock;

/** Removes a block from a gesture-recognizer object.
 
 @param gestureRecognizerBlock An object that currently is a recipient of action messages sent by the receiver when the represented gesture occurs. Specify `nil` if you want to remove all blocks and targets from the receiver.
 @see addBlock:
 @see removeBlock:
 */
- (void)removeBlock:(BMGestureRecognizerBlock)gestureRecognizerBlock;

@end
