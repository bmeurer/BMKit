/*-
 * Copyright (c) 2004-2011, Benedikt Meurer <benedikt.meurer@googlemail.com>
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


/** BMKit related additions to the NSData class. */
@interface NSData (BMKitAdditions)

///-----------------------
/// @name Base64 Encoding
///-----------------------

/** Returns the decoded data of a Base64-encoded string.
 
 @param aString A Base64-encoded string.
 @return The decoded data of the Base64-encoded string `aString`, or `nil` in case of an error.
 @see initWithBase64EncodedString:
 @see base64EncodedString
 */
+ (NSData *)dataWithBase64EncodedString:(NSString *)aString;

/** Initializes the receiver by decoding the data from a Base64-encoded string.
 
 @param aString A Base64-encoded string.
 @return The receiver, or `nil` in case of an error.
 @see dataWithBase64EncodedString:
 @see base64EncodedString
 */
- (id)initWithBase64EncodedString:(NSString *)aString;

/** Returns the Base64-encoded content of the receiver.
 
 @return An NSString with the Base64 encoding of the receivers bytes.
 @see dataWithBase64EncodedString:
 @see initWithBase64EncodedString:
 */
- (NSString *)base64EncodedString;

///-----------------------
/// @name Message Digests
///-----------------------

/** Returns the MD5 message digest of the receivers bytes.
 
 @return The MD5 message digest of the receivers bytes.
 @see MD5String
 */
- (NSData *)MD5;

/** Returns the string representation of the MD5 message digest of the receivers bytes.
 
 @return The string representation of the MD5 message digest of the receivers bytes.
 @see MD5
 */
- (NSString *)MD5String;

@end
