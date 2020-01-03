/*!
 *	@file			ParseImage.m
 *
 *	@title			ParseImage
 *
 *	@author			Created by Diney Bomfim on 15/2/11.
 *	
 *	@copyright		© 2011 DB-Interactive. All rights reserved.
 *
 *	@discussion		This project is an integrant part of the serie of tutorials "All about OpenGL ES 2.x"
 *					created on http://db-in.com/blog/
 *
 *					<ul>
 *						<li>1th - http://db-in.com/blog/2011/01/all-about-opengl-es-2-x-part-13/</li>
 *						<li>2nd - http://db-in.com/blog/2011/02/all-about-opengl-es-2-x-part-23/</li>
 *						<li>3rd - http://db-in.com/blog/2011/05/all-about-opengl-es-2-x-part-33/</li>
 *					</ul>
 *
 *					This tutorial contains a platform specific part for the iPhone iOS 3.2
 *					and may need modifications before you can run it.
 
 */

#import "ParseImage.h"

@implementation ParseImage

#pragma mark -
#pragma mark Properties
//**********************************************************************************************************
//
//  Properties
//
//**********************************************************************************************************

@synthesize width = _width, height = _height, data = _data;

#pragma mark -
#pragma mark Initialization
//**********************************************************************************************************
//
//  Initialization
//
//**********************************************************************************************************

- (id) initWithImage:(UIImage *)uiImage
{
	if ((self = [self init]))
	{
		CGImageRef cgImage;
		CGContextRef context;
		CGColorSpaceRef	colorSpace;
		
		// Sets the CoreGraphic Image to work on it.
		cgImage = [uiImage CGImage];
		
		// Sets the image's size.
		_width = CGImageGetWidth(cgImage);
		_height = CGImageGetHeight(cgImage);
		
		// Extracts the pixel informations and place it into the data.
		colorSpace = CGColorSpaceCreateDeviceRGB();
		_data = malloc(_width * _height * 4);
		context = CGBitmapContextCreate(_data, _width, _height, 8, 4 * _width, colorSpace,
										kCGImageAlphaPremultipliedLast | kCGBitmapByteOrder32Big);
		CGColorSpaceRelease(colorSpace);
		
		// Adjusts position and invert the image.
		// The OpenGL uses the image data upside-down compared commom image files.
		CGContextTranslateCTM(context, 0, _height);
		CGContextScaleCTM(context, 1.0, -1.0);
		
		// Clears and ReDraw the image into the context.
		CGContextClearRect(context, CGRectMake(0, 0, _width, _height));
		CGContextDrawImage(context, CGRectMake(0, 0, _width, _height), cgImage);
		
		// Releases the context.
		CGContextRelease(context);
	}
	
	return self;
}

#pragma mark -
#pragma mark Override Public Methods
//**********************************************************************************************************
//
//  Override Public Methods
//
//**********************************************************************************************************


- (void) dealloc
{
	free(_data);
	
	[super dealloc];
}

@end