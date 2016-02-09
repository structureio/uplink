// This file is part of Uplink, an easy-to-use cross-platform live RGBD streaming protocol.
// Copyright (c) 2016, Occipital, Inc.  All rights reserved.
// License: See LICENSE.

# pragma once

# include "./apple-image-codecs.h"
# include <CoreGraphics/CGDataProvider.h>
# include <ImageIO/CGImageDestination.h>
# include <CoreFoundation/CFDictionary.h>

namespace uplink {

//------------------------------------------------------------------------------

/*
    Image Type Identifiers

    If we were willing to be absolutely correct, this is what we would need to include here:

#include <TargetConditionals.h>
#if TARGET_OS_IPHONE
#   include <MobileCoreServices/MobileCoreServices.h>
#else
#   include <CoreServices/CoreServices.h>
#endif

    And this is what ALL (yes, virally) of our final targets would need to link against:

        CoreServices on OSX
        MobileCoreServices on iOS

    But, since the only symbols we need from these frameworks are a few strings that are highly unlikely to ever change, we simply dupe them below.

    See: https://developer.apple.com/library/mac/Documentation/Miscellaneous/Reference/UTIRef/Articles/System-DeclaredUniformTypeIdentifiers.html
    And: https://developer.apple.com/library/mac/documentation/MobileCoreServices/Reference/UTTypeRef/index.html#//apple_ref/doc/constant_group/UTI_Image_Content_Types
*/

inline CFStringRef UTI_PNG  () { return CFSTR("public.png");        } // kUTTypePNG
inline CFStringRef UTI_JPEG () { return CFSTR("public.jpeg");       } // kUTTypeJPEG
inline CFStringRef UTI_BMP  () { return CFSTR("com.microsoft.bmp"); } // kUTTypeBMP

//------------------------------------------------------------------------------

inline float
CoreGraphicsJpegQualityFromNormalizedLibJpegQuality(float normalizedLibJpegQuality)
{
    const float x = normalizedLibJpegQuality;
    const float x2 = x * x;
    const float x3 = x2 * x;
    const float x4 = x3 * x;
    const float x5 = x4 * x;

    // Function determined by doing a compression test across different JPEG APIs.
    // More info: Dropbox/CV Team/JPEG Compression Test/

    // Below ~0.17, the sqrt is negative.
    // Since CGQuality is near-zero in this range, use a very low compression quality.
    if (x < 0.17)
        return 0.01;

    const float coreGraphicsQuality = (sqrt(609200000*(2366.8*x5 - 4919.1*x4 + 3840.6*x3 - 1395*x2 + 301.27*x - 4.2418)-13229791599)-27599)/304600;

    return coreGraphicsQuality;
}

//------------------------------------------------------------------------------

inline bool
encode_image (
    graphics_ImageCodec     imageCodec,
    const uint8_t* inputBuffer,
    size_t         inputSize,
    graphics_PixelFormat    inputFormat,
    size_t         inputWidth,
    size_t         inputHeight,
    MemoryBlock& outputMemoryBlock,
    float          outputQuality
)
{
    // FIXME: Make calls with outputBuffer == 0 faster!
    // Currently, we actually need to decompress twice :(

    // 0.5 quality ~= 32KB @ 640x480 grayscale

    // FIXME: Minimize allocations and copies here.

    // FIXME: we need to add an autoreleasepool block here since it is running in a background
    // thread. See https://developer.apple.com/library/ios/documentation/Cocoa/Conceptual/MemoryMgmt/Articles/mmAutoreleasePools.html#//apple_ref/doc/uid/20000047-1041876
 
    assert(0 != inputBuffer);
    assert(0 < inputSize);
    assert(
        graphics_PixelFormat_Gray == inputFormat
     || graphics_PixelFormat_RGB  == inputFormat
     || graphics_PixelFormat_RGBA == inputFormat
    );
    assert(0 < inputWidth);
    assert(0 < inputHeight);
    assert(0. < outputQuality && outputQuality < 1.);

    outputQuality = CoreGraphicsJpegQualityFromNormalizedLibJpegQuality(outputQuality);

    CGDataProviderRef dataProvider = CGDataProviderCreateWithData(
        NULL, inputBuffer, inputSize, NULL
    );

    return_false_if_zero(dataProvider);

    CGColorSpaceRef colorSpace = 0;
    size_t bytesPerPixel = 0;

    switch (inputFormat)
    {
        case graphics_PixelFormat_Gray:
        {
            colorSpace = CGColorSpaceCreateDeviceGray();
            bytesPerPixel = 1;
        }
        break;
    
        case graphics_PixelFormat_RGB:
        {
            colorSpace = CGColorSpaceCreateDeviceRGB();
            bytesPerPixel = 3;
        }
        break;

        case graphics_PixelFormat_RGBA:
        {
            colorSpace = CGColorSpaceCreateDeviceRGB();
            bytesPerPixel = 4;
        }
        break;
    }

    return_false_if_zero(colorSpace);

    CGImageRef image = CGImageCreate(
        inputWidth,
        inputHeight,
        8,
        8 * bytesPerPixel,
        inputWidth * bytesPerPixel,
        colorSpace,
        0,
        dataProvider,
        NULL,
        false,
        kCGRenderingIntentDefault
    );

    return_false_if_zero(image);

    CFMutableDataRef data = CFDataCreateMutable(NULL, 0);

    return_false_if_zero(data);
   
    CFStringRef typeIdentifier = 0;
    CFDictionaryRef properties = 0;

    // See:

    switch (imageCodec)
    {
        case graphics_ImageCodec_JPEG:
        {
            typeIdentifier = UTI_JPEG();
            static const size_t NumProperties = 1;
            const void* keys [NumProperties] =
            {
                kCGImageDestinationLossyCompressionQuality
            };
            CFNumberRef lossyCompressionQuality = CFNumberCreate(kCFAllocatorDefault, kCFNumberFloatType, &outputQuality);
            const void* values[NumProperties] =
            {
                lossyCompressionQuality
            };
            properties = CFDictionaryCreate(
                kCFAllocatorDefault,
                keys,
                values,
                NumProperties,
                &kCFCopyStringDictionaryKeyCallBacks,
                &kCFTypeDictionaryValueCallBacks
            );
            CFRelease(lossyCompressionQuality);
        }
        break;
        
        case graphics_ImageCodec_PNG:
        {
            typeIdentifier = UTI_PNG();
            properties = 0;
        }
        break;

        case graphics_ImageCodec_BMP:
        {
            typeIdentifier = UTI_BMP();
            properties = 0;
        }
        break;
            
        default:
            
            // Codec not yet supported in this method.
            assert(false);
            break;
    }

    CGImageDestinationRef destination = CGImageDestinationCreateWithData(data, typeIdentifier, 1, NULL);

    return_false_if_zero(destination);

    CGImageDestinationAddImage(destination, image, properties);

    if (0 != properties)
        CFRelease(properties);

    return_false_unless(CGImageDestinationFinalize(destination));

    const size_t   compressedSize  = CFDataGetLength(data);
    const uint8_t* compressedBytes = CFDataGetBytePtr(data);
    
    outputMemoryBlock.ReplaceWith((uint8_t*)compressedBytes, compressedSize, false, [data]() {
        CFRelease(data); // will be called when the MemoryBlock is freed
    });

    CFRelease(destination);
    CGImageRelease(image);
    CGColorSpaceRelease(colorSpace);
    CGDataProviderRelease(dataProvider);

    return true;
}

//------------------------------------------------------------------------------

inline bool
decode_image (
    graphics_ImageCodec     imageCodec,
    const uint8_t* inputBuffer,
    size_t         inputSize,
    graphics_PixelFormat    outputFormat,
    size_t&        outputWidth,
    size_t&        outputHeight,
    MemoryBlock& outputMemoryBlock
)
{
    assert(0 != inputBuffer);
    assert(0 != inputSize);
    assert(
        graphics_PixelFormat_Gray == outputFormat
     || graphics_PixelFormat_RGB  == outputFormat
     || graphics_PixelFormat_RGBA == outputFormat
    );
    assert(0 == outputWidth); // Just in case the caller made false assumptions about the calling semantics of this function.
    assert(0 == outputHeight); // Just in case the caller made false assumptions about the calling semantics of this function.

    // TODO: This function outputs an RGB buffer, but we really shouldn't bother converting to RGB at all.
    // We could choose a faster path like direct YUV display (from JPEG's internal YUV) or at least
    // retain the premultiplied alpha that comes out of the regular iOS/OS X JPEG decode.

    // FIXME: we need to add an autoreleasepool block here since it is running in a background
    // thread. See https://developer.apple.com/library/ios/documentation/Cocoa/Conceptual/MemoryMgmt/Articles/mmAutoreleasePools.html#//apple_ref/doc/uid/20000047-1041876
    
    // TODO: We can cache the buffer to avoid malloc/free.
    
    CFDataRef data  = CFDataCreateWithBytesNoCopy(NULL, inputBuffer, inputSize, kCFAllocatorNull);
    
    CFStringRef       imageSourceCreateKeys[1];
    CFTypeRef         imageSourceCreateValues[1];
    CFDictionaryRef   imageSourceCreateOptions;
    
    imageSourceCreateKeys[0] = kCGImageSourceShouldCache;
    imageSourceCreateValues[0] = (CFTypeRef)kCFBooleanFalse;
    imageSourceCreateOptions = CFDictionaryCreate(
        kCFAllocatorDefault,
        reinterpret_cast<const void**>(imageSourceCreateKeys),
        reinterpret_cast<const void**>(imageSourceCreateValues),
        1,
        &kCFTypeDictionaryKeyCallBacks,
        &kCFTypeDictionaryValueCallBacks
    );

    CGImageSourceRef imageSource = CGImageSourceCreateWithData (data, imageSourceCreateOptions);

    //read image
    CGImageRef image = CGImageSourceCreateImageAtIndex(imageSource, 0, NULL);
    
    CFRelease(imageSourceCreateOptions);

    outputWidth = CGImageGetWidth(image);
    outputHeight = CGImageGetHeight(image);

    static const size_t maximumReasonableSize = 1024 * 32;

    assert(outputWidth < maximumReasonableSize);
    assert(outputHeight < maximumReasonableSize);

    CGColorSpaceRef colorSpace    = 0;
    size_t          bytesPerPixel = 0;
    CGBitmapInfo    bitmapInfo    = 0;

    switch (outputFormat)
    {
        case graphics_PixelFormat_Gray:
        {
            colorSpace = CGColorSpaceCreateDeviceGray();
            bytesPerPixel = 1;
            bitmapInfo = kCGImageAlphaNone;
        }
        break;

        case graphics_PixelFormat_RGB:
        case graphics_PixelFormat_RGBA:
        {
            colorSpace = CGColorSpaceCreateDeviceRGB();
            bytesPerPixel = 4;
            bitmapInfo = kCGImageAlphaNoneSkipLast;
        }
        break;
    }

    // Set the outputSize to the RGBA buffer size.
    size_t outputSize = outputWidth * outputHeight * bytesPerPixel;

    outputMemoryBlock.Resize(outputSize);
    
	CGContextRef context = CGBitmapContextCreate(
        outputMemoryBlock.Data,
        outputWidth,
        outputHeight,
        8,
        outputWidth * bytesPerPixel,
        colorSpace,
        bitmapInfo
    );

    if(0 == context)
    {
        CFRelease(data);
        CFRelease(image);
        CFRelease(imageSource);
        CFRelease(colorSpace);
    
        return false;
    }

    CGContextDrawImage(context, CGRectMake(0, 0, outputWidth, outputHeight), image);

    switch (outputFormat)
    {
        case graphics_PixelFormat_RGB:
        {
            uint32_t* rgba = reinterpret_cast<uint32_t*>(outputMemoryBlock.Data);

            // In-place convert from RGBA to RGB.
            for (int k = 0; k < outputWidth * outputHeight; ++k)
            {
                // Trick: Copies 4 bytes but increments pointer by 3.
                *reinterpret_cast<uint32_t*>(outputMemoryBlock.Data + k * 3) = rgba[k];
            }

            // Set the outputSize to the actual RGB buffer size.
            outputMemoryBlock.Resize(outputWidth * outputHeight * 3);
        }
        break;
    
        case graphics_PixelFormat_Gray:
        case graphics_PixelFormat_RGBA:
        {
            // Nothing to do.
        }
        break;
    }

    CFRelease(image);

    CGColorSpaceRelease(colorSpace);
    CGContextRelease(context);
   
    CFRelease(imageSource);
    CFRelease(data);
    
    return true;
}

//------------------------------------------------------------------------------

}
