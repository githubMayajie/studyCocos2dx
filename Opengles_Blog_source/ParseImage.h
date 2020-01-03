/*!
 *	@file			ParseImage.h
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

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

/*!
 *	@class			ParseImage
 *	
 *	@abstract		This class is responsible by parse image files.
 *
 *	@discussion		The outputs will be the image's size (width and height) and the
 *					binarized array of pixels.
 */
@interface ParseImage : NSObject
{
@private
	int					_width;
	int					_height;
	void				*_data;
}

/*!
 *	@discussion		The width in pixels.
 */
@property (nonatomic, readonly) int width;

/*!
 *	@discussion		The height in pixels.
 */
@property (nonatomic, readonly) int height;

/*!
 *	@discussion		The array of pixels.
 */
@property (nonatomic, readonly) void *data;


/*!
 *	@method			initWithImage:
 *
 *	@abstract		Initiates the parsing based on an UIImage.
 *
 *	@discussion		This method parses an image using 4 bytes per pixel. The output format will be RGBA
 *
 *	@param			uiImage
 *					The UIImage instance to be parsed.
 */
- (id) initWithImage:(UIImage *)uiImage;

@end