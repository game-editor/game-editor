//
//  system_iphone.mm
//  gameEditor
//
//  Created by Andreas Kindlbacher on 4/11/10.
//  Copyright 2010 Visionware. All rights reserved.
//

#import "system_iphone.h"


@implementation system_iphone

@end
#include "SDL.h"
#if defined(iPhone_Player)
#include "FtpServer.h"
#endif

//#import "AppDelegate.h"

#pragma mark * URLGetController

@interface URLGetController ()

// Properties that don't need to be seen by the outside world.

@property (nonatomic, readonly) BOOL              isReceiving;
@property (nonatomic, retain)   NSURLConnection * connection;
@property (nonatomic, copy)     NSString *        filePath;
@property (nonatomic, retain)   NSOutputStream *  fileStream;

@end

@implementation URLGetController
@synthesize 
#if defined(iPhone_Player)
theServer,
#endif
baseDir;

#pragma mark * Status management

// These methods are used by the core transfer code to update the UI.
-(id)init {
	if(self= [super init]) {
		
	}
	return self;
}

- (void)_receiveDidStart
{
    //[self.activityIndicator startAnimating];
}

- (void)_updateStatus:(NSString *)statusString
{
    assert(statusString != nil);
    statusLabel = statusString;
}

- (void)_receiveDidStopWithStatus:(NSString *)statusString
{
    if (statusString == nil) {
        assert(self.filePath != nil);
        statusString = @"GET succeeded";
    }
    //[self.activityIndicator stopAnimating];

    statusLabel = statusString;
	done=true;
}

#pragma mark * Core transfer code

// This is the code that actually does the networking.

@synthesize connection    = _connection;
@synthesize filePath      = _filePath;
@synthesize fileStream    = _fileStream;

- (BOOL)isReceiving
{
    return (self.connection != nil);
}

- (NSString *)_startReceive:(char *) theUrl : (char *) filPath
// Starts a connection to download the current URL.
{
    BOOL                success;
    NSURL *             url;
    NSURLRequest *      request;
    //NSString *tUrl= 
	done=false;
    assert(self.connection == nil);         // don't tap receive twice in a row!
    assert(self.fileStream == nil);         // ditto
    assert(self.filePath == nil);           // ditto
	
    // First get and check the URL.
	statusLabel = @"test";
    url = [self smartURLForString:[NSString  stringWithUTF8String: theUrl]];
	
    success = (url != nil);
	
    // If the URL is bogus, let the user know.  Otherwise kick off the connection.
    
    if ( ! success) {
        statusLabel = @"Invalid URL";
    } else {
		
        // Open a stream for the file we're going to receive into.
		self.filePath = [NSString stringWithUTF8String:filPath];

		//        self.filePath = [self pathForTemporaryFileWithPrefix:@"Get"];

        assert(self.filePath != nil);
        
        self.fileStream = [NSOutputStream outputStreamToFileAtPath:self.filePath append:NO];
        assert(self.fileStream != nil);
        
        [self.fileStream open];
		
        // Open a connection for the URL.
		
        request = [NSURLRequest requestWithURL:url];
        assert(request != nil);
        
        self.connection = [NSURLConnection connectionWithRequest:request delegate:self];
        assert(self.connection != nil);
		
        // Tell the UI we're receiving.
        
        [self _receiveDidStart];
		while(done != true)
			SDL_PumpEvents();
		    }
	return statusLabel;
}

- (void)_stopReceiveWithStatus:(NSString *)statusString
// Shuts down the connection and displays the result (statusString == nil) 
// or the error status (otherwise).
{
    if (self.connection != nil) {
        [self.connection cancel];
        self.connection = nil;
    }
    if (self.fileStream != nil) {
        [self.fileStream close];
        self.fileStream = nil;
    }
    [self _receiveDidStopWithStatus:statusString];
    self.filePath = nil;
}

- (void)connection:(NSURLConnection *)theConnection didReceiveResponse:(NSURLResponse *)response
// A delegate method called by the NSURLConnection when the request/response 
// exchange is complete.
//
// For an HTTP request you would check [response statusCode] and [response MIMEType] to 
// verify that this is acceptable data, but for an FTP request there is no status code 
// and the type value is derived from the extension so you might as well pre-flight that.
//
// You could, use this opportunity to get [response expectedContentLength] and 
// [response suggestedFilename], but I don't need either of these values for 
// this sample.
{
#pragma unused(theConnection)
#pragma unused(response)
	
    assert(theConnection == self.connection);
    assert(response != nil);
}

- (void)connection:(NSURLConnection *)theConnection didReceiveData:(NSData *)data
// A delegate method called by the NSURLConnection as data arrives.  We just 
// write the data to the file.
{
#pragma unused(theConnection)
    NSInteger       dataLength;
    const uint8_t * dataBytes;
    NSInteger       bytesWritten;
    NSInteger       bytesWrittenSoFar;
	
    assert(theConnection == self.connection);
    
    dataLength = [data length];
    dataBytes  = (const uint8_t *)[data bytes];

    bytesWrittenSoFar = 0;
    do {
        bytesWritten = [self.fileStream write:&dataBytes[bytesWrittenSoFar] maxLength:dataLength - bytesWrittenSoFar];
        assert(bytesWritten != 0);
        if (bytesWritten == -1) {
            [self _stopReceiveWithStatus:@"File write error"];
            break;
        } else {
            bytesWrittenSoFar += bytesWritten;
        }
    } while (bytesWrittenSoFar != dataLength);
}

- (void)connection:(NSURLConnection *)theConnection didFailWithError:(NSError *)error
// A delegate method called by the NSURLConnection if the connection fails. 
// We shut down the connection and display the failure.  Production quality code 
// would either display or log the actual error.
{
#pragma unused(theConnection)
#pragma unused(error)
    assert(theConnection == self.connection);
    
    [self _stopReceiveWithStatus:@"Connection failed"];
}

- (void)connectionDidFinishLoading:(NSURLConnection *)theConnection
// A delegate method called by the NSURLConnection when the connection has been 
// done successfully.  We shut down the connection with a nil status, which 
// causes the image to be displayed.
{
#pragma unused(theConnection)
    assert(theConnection == self.connection);
    
    [self _stopReceiveWithStatus:nil];
}

#pragma mark * UI Actions

- (IBAction)getOrCancelAction:(id)sender
{
#pragma unused(sender)
    if (self.isReceiving) {
        [self _stopReceiveWithStatus:@"Cancelled"];
    } else {
        //[self _startReceive];
    }
}

- (void)textFieldDidEndEditing:(UITextField *)textField
// A delegate method called by the URL text field when the editing is complete. 
// We save the current value of the field in our settings.
{
#pragma unused(textField)
    NSString *  newValue;
    NSString *  oldValue;
    
    assert(textField == self.urlText);
	
    newValue = self.urlText;
    oldValue = [[NSUserDefaults standardUserDefaults] stringForKey:@"URLGetURLText"];
	
    // Save the URL text if it's changed.
    
    assert(newValue != nil);        // what is UITextField thinking!?!
    assert(oldValue != nil);        // because we registered a default
    
    if ( ! [newValue isEqual:oldValue] ) {
        [[NSUserDefaults standardUserDefaults] setObject:newValue forKey:@"URLGetURLText"];
    }
}

- (BOOL)textFieldShouldReturn:(UITextField *)textField
// A delegate method called by the URL text field when the user taps the Return 
// key.  We just dismiss the keyboard.
{
#pragma unused(textField)
    assert(textField == self.urlText);
    [self.urlText resignFirstResponder];
    return NO;
}

#pragma mark * Utils

- (NSString *)pathForTemporaryFileWithPrefix:(NSString *)prefix
{
    NSString *  result;
    CFUUIDRef   uuid;
    CFStringRef uuidStr;
    
    uuid = CFUUIDCreate(NULL);
    assert(uuid != NULL);
    
    uuidStr = CFUUIDCreateString(NULL, uuid);
    assert(uuidStr != NULL);
    
    result = [NSTemporaryDirectory() stringByAppendingPathComponent:[NSString stringWithFormat:@"%@-%@", prefix, uuidStr]];
    assert(result != nil);
    
    CFRelease(uuidStr);
    CFRelease(uuid);
    
    return result;
}

- (NSURL *)smartURLForString:(NSString *)str
{
    NSURL *     result;
    NSString *  trimmedStr;
    NSRange     schemeMarkerRange;
    NSString *  scheme;
    
    assert(str != nil);
	
    result = nil;
    
    trimmedStr = [str stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceCharacterSet]];
    if ( (trimmedStr != nil) && (trimmedStr.length != 0) ) {
        schemeMarkerRange = [trimmedStr rangeOfString:@"://"];
        
        if (schemeMarkerRange.location == NSNotFound) {
            result = [NSURL URLWithString:[NSString stringWithFormat:@"ftp://%@", trimmedStr]];
        } else {
            scheme = [trimmedStr substringWithRange:NSMakeRange(0, schemeMarkerRange.location)];
            assert(scheme != nil);
            
            if ( ([scheme compare:@"ftp"  options:NSCaseInsensitiveSearch] == NSOrderedSame) ) {
                result = [NSURL URLWithString:trimmedStr];
            } else {
                // It looks like this is some unsupported URL scheme.
            }
        }
    }
    
    return result;
}

#if defined(iPhone_Player)
#include <ifaddrs.h>
#include <arpa/inet.h>
- (NSString *)getIPAddress
{
	NSString *address = @"error";
	struct ifaddrs *interfaces = NULL;
	struct ifaddrs *temp_addr = NULL;
	int success = 0;
	
	// retrieve the current interfaces - returns 0 on success
	success = getifaddrs(&interfaces);
	if (success == 0)
	{
		// Loop through linked list of interfaces
		temp_addr = interfaces;
		while(temp_addr != NULL)
		{
			if(temp_addr->ifa_addr->sa_family == AF_INET)
			{
				// Check if interface is en0 which is the wifi connection on the iPhone
				if([[NSString stringWithUTF8String:temp_addr->ifa_name] isEqualToString:@"en0"])
				{
					// Get NSString from C String
					address = [NSString stringWithUTF8String:inet_ntoa(((struct sockaddr_in *)temp_addr->ifa_addr)->sin_addr)];
				}
			}
			
			temp_addr = temp_addr->ifa_next;
		}
	}
	
	// Free memory
	freeifaddrs(interfaces);
	
	return address;
}	

-(void) startFtp;
{	
	
	
	NSString *alertTitle = [NSString stringWithFormat:@"%@ %@ %s %@",@"Welcome to Game-Editor", @"Connect to", [[self getIPAddress]UTF8String], @"port 20000"];	
	
	UIAlertView *alert = [[UIAlertView alloc] initWithTitle:alertTitle 
													message:@"The FTP Server has been enabled, please use FTP client software to transfer your datafile (geplayer.dat) to this device.  Press the \"Stop FTP Server\" button once all data transfers have been completed."
												   delegate:self 
										  cancelButtonTitle:@"Stop FTP Server" 
										  otherButtonTitles:nil];
	[alert show];
	//[alert release];
	[super viewDidLoad];
	NSArray *docFolders = NSSearchPathForDirectoriesInDomains (NSDocumentDirectory, NSUserDomainMask, YES ); 
	self.baseDir =  [docFolders lastObject];	
	
	FtpServer *aServer = [[ FtpServer alloc ] initWithPort:20000 withDir:baseDir notifyObject:self ];
	self.theServer = aServer;
	
#if 1	
	while ((!alert.hidden) && (alert.superview!=nil))
	{
		[[NSRunLoop currentRunLoop] limitDateForMode:NSDefaultRunLoopMode];
		SDL_PumpEvents();
	}
#endif	
	[aServer release];
	
	
}


- (void)stopFtpServer {
	NSLog(@"Stopping the FTP server");
	[theServer stopFtpServer];
	[theServer release];
	theServer = nil;
}

#endif

- (void)alertView:(UIAlertView *)alert clickedButtonAtIndex:(NSInteger)buttonIndex {
	if (buttonIndex == 0) {
		[alert dismissWithClickedButtonIndex:0 animated:YES];
		[self stopFtpServer];
	} 
}


- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    // Return YES for supported orientations
    return (interfaceOrientation == UIInterfaceOrientationPortrait);
}


- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning]; // Releases the view if it doesn't have a superview
    // Release anything that's not essential, such as cached data
}


- (void)dealloc {
    [super dealloc];
}

#pragma mark * View controller boilerplate
@end
extern "C" {
void openUrl(const char * b) 
{
	NSURL *appUrl = [NSURL URLWithString:[NSString  stringWithUTF8String:b]];
	[[UIApplication sharedApplication] openURL:appUrl]; 	
}
}
void startFtp(void)
{
	
	URLGetController * URLGetCtrl=[[URLGetController alloc] init];
	
	[URLGetCtrl startFtp];	
	[URLGetCtrl release];		
}

bool FtpGet(char *FtpUrl, char *fPath)
{
	const char *r;
	//const char *u;
	
	URLGetController * URLGetCtrl=[[URLGetController alloc] init];
		

	
	r= [[URLGetCtrl _startReceive: FtpUrl :fPath]UTF8String];
	
	[URLGetCtrl release];
	
	return (strcmp(r,"GET succeeded"));
		
	
}
