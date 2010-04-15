//
//  system_iphone.h
//  gameEditor
//
//  Created by Andreas Kindlbacher on 4/11/10.
//  Copyright 2010 Visionware. All rights reserved.
//

#import <UIKit/UIKit.h>


@interface system_iphone : NSObject {
// For future use
}

@end

@interface URLGetController : UIViewController <UITextFieldDelegate>
{
    //UITextField *               _urlText;
    UIImageView *               _imageView;
    NSString *                   statusLabel;
    UIActivityIndicatorView *   _activityIndicator;
    //UIBarButtonItem *           _getOrCancelButton;
    
    NSURLConnection *           _connection;
    NSString *                  _filePath;
    NSOutputStream *            _fileStream;
	
	NSString	*				urlText;
	NSString	*				getOrCancelButton;
	bool						done;
}
@property (nonatomic, retain)  NSString *					urlText;
@property (nonatomic, retain)  UIImageView *               imageView;
@property (nonatomic, retain)  NSString *                   statusLabel;
@property (nonatomic, retain)  UIActivityIndicatorView *   activityIndicator;
@property (nonatomic, retain)  NSString *           getOrCancelButton;

- (IBAction)getOrCancelAction:(id)sender;
- (id)init;
- (NSURL *)smartURLForString:(NSString *)str;
- (BOOL)isImageURL:(NSURL *)url;
- (NSString *)pathForTemporaryFileWithPrefix:(NSString *)prefix;
@end

const char *FtpGet(char *FtpUrl);