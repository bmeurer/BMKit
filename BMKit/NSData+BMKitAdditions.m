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

#include <CommonCrypto/CommonDigest.h>

#import "NSData+BMKitAdditions.h"


@implementation NSData (BMKitAdditions)


#pragma mark -
#pragma mark Base64 Encoding


static const uint8_t NSDataBase64DecodingTable[256] =
{
	65, 65, 65, 65, 65, 65, 65, 65, 65, 64, 64, 65, 64, 64, 65, 65,
	65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65,
	64, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 62, 65, 65, 65, 63,
	52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 65, 65, 65, 65, 65, 65,
	65,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
	15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 65, 65, 65, 65, 65,
	65, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
	41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 65, 65, 65, 65, 65,
	65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65,
	65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65,
	65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65,
	65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65,
	65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65,
	65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65,
	65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65,
	65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65
};

static const char NSDataBase64EncodingTable[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";


+ (NSData *)dataWithBase64EncodedString:(NSString *)aString
{
    return [[[self alloc] initWithBase64EncodedString:aString] autorelease];
}


- (id)initWithBase64EncodedString:(NSString *)aString
{
    const char *str = [aString cStringUsingEncoding:NSASCIIStringEncoding];
	NSUInteger strLength = strlen(str);
	unsigned c, i = 0, j = 0, k;
	uint8_t *data = (uint8_t *)malloc(strLength);
    if (data) {
        while (((c = *str++) != '\0') && strLength-- != 0) {
            if (c == '=') {
                if (*str != '=' && ((i % 4) == 1)) {
                invalid:
                    free(data), data = NULL;
                    break;
                }
            }
            else {
                c = NSDataBase64DecodingTable[c];
                if (c != 64) { // 64 identifies whitespace, just skip
                    if (c == 65) { // 65 identifies an invalid character
                        goto invalid;
                    }
                    switch (i % 4) {
                        case 0:
                            data[j] = c << 2;
                            break;
                            
                        case 1:
                            data[j++] |= c >> 4;
                            data[j] = (c & 0x0f) << 4;
                            break;
                            
                        case 2:
                            data[j++] |= c >>2;
                            data[j] = (c & 0x03) << 6;
                            break;
                            
                        case 3:
                            data[j++] |= c;
                            break;
                    }
                    i++;
                }
            }
        }
        k = j;
        if (c == '=') {
            switch (i % 4) {
                case 1:
                    goto invalid;
                    
                case 2:
                    k++;
                    // FALL-THROUGH
                case 3:
                    data[k] = 0;
                    break;
            }
        }
    }
    if (data) {
        self = [self initWithBytes:data length:j];
    }
    else {
        [self release];
        self = nil;
    }
    free(data);
    return self;
}


- (NSString *)base64EncodedString
{
    NSString *base64EncodedString = nil;
	NSUInteger dataLength = [self length];
    const uint8_t *data = [self bytes];
    if (dataLength) {
        char *buffer = (char *)malloc(((dataLength + 2) / 3) * 4);
        char *bufptr = buffer;
        if (buffer) {
            for (; dataLength > 2; data += 3, dataLength -= 3) {
                *bufptr++ = NSDataBase64EncodingTable[data[0] >> 2];
                *bufptr++ = NSDataBase64EncodingTable[((data[0] & 0x03) << 4) | (data[1] >> 4)];
                *bufptr++ = NSDataBase64EncodingTable[((data[1] & 0x0f) << 2) | (data[2] >> 6)];
                *bufptr++ = NSDataBase64EncodingTable[data[2] & 0x3f];
            }
            if (dataLength) {
                *bufptr++ = NSDataBase64EncodingTable[data[0] >> 2];
                if (dataLength > 1) {
                    *bufptr++ = NSDataBase64EncodingTable[((data[0] & 0x03) << 4) | (data[1] >> 4)];
                    *bufptr++ = NSDataBase64EncodingTable[(data[1] & 0x0f) << 2];
                }
                else {
                    *bufptr++ = NSDataBase64EncodingTable[(data[0] & 0x03) << 4];
                    *bufptr++ = '=';
                }
                *bufptr++ = '=';
            }
            base64EncodedString = [[NSString alloc] initWithBytesNoCopy:buffer
                                                                 length:(bufptr - buffer)
                                                               encoding:NSASCIIStringEncoding
                                                           freeWhenDone:YES];
            if (!base64EncodedString) {
                free(buffer);
            }
        }
    }
    else {
        base64EncodedString = [[NSString alloc] init];
    }
    return [base64EncodedString autorelease];
}


#pragma mark -
#pragma mark Message Digests


static const char NSDataDigestStringHexDigits[] = "0123456789abcdef";


static NSString *NSDataDigestString(NSData *digestData)
{
    NSString *digestString = nil;
    if (digestData) {
        unsigned digestLength = [digestData length];
        const UInt8 *digest = [digestData bytes];
        char str[digestLength * 2];
        for (unsigned n = 0; n < digestLength; ++n) {
            str[2 * n]     = NSDataDigestStringHexDigits[digest[n] >> 4];
            str[2 * n + 1] = NSDataDigestStringHexDigits[digest[n] & 0xf];
        }
        digestString = [[NSString alloc] initWithBytes:str
                                                length:(digestLength * 2)
                                              encoding:NSASCIIStringEncoding];
    }
    return [digestString autorelease];
}


- (NSData *)MD2
{
    UInt8 digest[CC_MD2_DIGEST_LENGTH];
    CC_MD2([self bytes], [self length], digest);
    return [NSData dataWithBytes:digest length:sizeof(digest)];
}


- (NSString *)MD2String
{
    return NSDataDigestString([self MD2]);
}


- (NSData *)MD4
{
    UInt8 digest[CC_MD4_DIGEST_LENGTH];
    CC_MD4([self bytes], [self length], digest);
    return [NSData dataWithBytes:digest length:sizeof(digest)];
}


- (NSString *)MD4String
{
    return NSDataDigestString([self MD4]);
}


- (NSData *)MD5
{
    UInt8 digest[CC_MD5_DIGEST_LENGTH];
    CC_MD5([self bytes], [self length], digest);
    return [NSData dataWithBytes:digest length:sizeof(digest)];
}


- (NSString *)MD5String
{
    return NSDataDigestString([self MD5]);
}


- (NSData *)SHA1
{
    UInt8 digest[CC_SHA1_DIGEST_LENGTH];
    CC_SHA1([self bytes], [self length], digest);
    return [NSData dataWithBytes:digest length:sizeof(digest)];
}


- (NSString *)SHA1String
{
    return NSDataDigestString([self SHA1]);
}


- (NSData *)SHA224
{
    UInt8 digest[CC_SHA224_DIGEST_LENGTH];
    CC_SHA224([self bytes], [self length], digest);
    return [NSData dataWithBytes:digest length:sizeof(digest)];
}


- (NSString *)SHA224String
{
    return NSDataDigestString([self SHA224]);
}


- (NSData *)SHA256
{
    UInt8 digest[CC_SHA256_DIGEST_LENGTH];
    CC_SHA256([self bytes], [self length], digest);
    return [NSData dataWithBytes:digest length:sizeof(digest)];
}


- (NSString *)SHA256String
{
    return NSDataDigestString([self SHA256]);
}


- (NSData *)SHA384
{
    UInt8 digest[CC_SHA384_DIGEST_LENGTH];
    CC_SHA384([self bytes], [self length], digest);
    return [NSData dataWithBytes:digest length:sizeof(digest)];
}


- (NSString *)SHA384String
{
    return NSDataDigestString([self SHA384]);
}


- (NSData *)SHA512
{
    UInt8 digest[CC_SHA512_DIGEST_LENGTH];
    CC_SHA512([self bytes], [self length], digest);
    return [NSData dataWithBytes:digest length:sizeof(digest)];
}


- (NSString *)SHA512String
{
    return NSDataDigestString([self SHA512]);
}


@end
