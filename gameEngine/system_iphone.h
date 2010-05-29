//
//  system_iphone.h
//  gameEditor
//
//  Created by Andreas Kindlbacher on 4/11/10.
//  Copyright 2010 Visionware. All rights reserved.
//

#import <UIKit/UIKit.h>

#if defined(iPhone_Player)
#import "FtpServer.h"
#endif


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
#if defined(iPhone_Player)
	FtpServer *theServer;
#endif
	NSString *baseDir;
	
}
@property (nonatomic, retain)  NSString *					urlText;
@property (nonatomic, retain)  UIImageView *               imageView;
@property (nonatomic, retain)  NSString *                   statusLabel;
@property (nonatomic, retain)  UIActivityIndicatorView *   activityIndicator;
@property (nonatomic, retain)  NSString *           getOrCancelButton;

#if defined(iPhone_Player)
@property (nonatomic, retain) FtpServer *theServer;
#endif

@property (nonatomic, retain) NSString *baseDir;

-(void)didReceiveFileListChanged;
- (void)stopFtpServer;

- (IBAction)getOrCancelAction:(id)sender;
- (id)init;
- (NSURL *)smartURLForString:(NSString *)str;
- (NSString *)pathForTemporaryFileWithPrefix:(NSString *)prefix;
@end

const char *FtpGet(char *FtpUrl);