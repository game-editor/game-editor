/*
 DiddyFtpServer
 Copyright (C) 2008  Richard Dearlove ( Diddy )
 
 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.
 
 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.
 
 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */
#import "FtpConnection.h"
#import "FtpServer.h"


@implementation FtpConnection

@synthesize transferMode;
@synthesize currentDir;

// ----------------------------------------------------------------------------------------------------------
-(id)initWithAsyncSocket:(AsyncSocket*)newSocket forServer:(id)myServer 
// ----------------------------------------------------------------------------------------------------------
{
	self = [super init ];
	if (self)
	{
		connectionSocket = [newSocket retain ];
		server = myServer;
		[ connectionSocket setDelegate:self ];
		[ connectionSocket writeData:DATASTR(@"220 geplayer FTP server ready.\r\n") withTimeout:-1 tag:0 ];		
		[ connectionSocket readDataToData:[AsyncSocket CRLFData] withTimeout:READ_TIMEOUT tag:FTP_CLIENT_REQUEST ];
		dataListeningSocket = nil;
		dataPort=2001;
		transferMode = pasvftp;
		queuedData = [[ NSMutableArray alloc ] init ];
		
		currentDir = [[[ NSFileManager defaultManager ] currentDirectoryPath ] copy ];
		currentFile = nil;	
		currentFileHandle = nil;				// not saving to a file yet
		
		NSLog(currentDir );
	}
	return self;
}

#pragma mark STATE 
// ----------------------------------------------------------------------------------------------------------
//-(void)setTransferMode:(int)mode
// ----------------------------------------------------------------------------------------------------------
//{
//	transferMode = mode;
//}

// ----------------------------------------------------------------------------------------------------------
-(NSString*)connectionAddress
// ----------------------------------------------------------------------------------------------------------
{
	return [connectionSocket connectedHost];
	
}


// ==========================================================================================================
#pragma mark CHOOSE DATA SOCKET 
// ==========================================================================================================

// ----------------------------------------------------------------------------------------------------------
-(void)setupDataListeningSocket
// ----------------------------------------------------------------------------------------------------------
{
	// change this to using a choice between types of socket - portftp, pasvftp etc
	NSLog(@"FC:Setting up DataComms Listening Socket - on %i",dataPort);
	
	dataListeningSocket = [[ AsyncSocket alloc ] initWithDelegate:self ];
	[ dataListeningSocket acceptOnPort:dataPort error:nil ];		
	
	// this should trigger onSocketWillConnect and then didAcceptNewSocket
}


// ----------------------------------------------------------------------------------------------------------
-(BOOL)openDataSocket:(int)portNumber
// ----------------------------------------------------------------------------------------------------------
{
	NSString *responseString;
	NSError *error = nil;		
		
	dataSocket = [ [ AsyncSocket alloc ] initWithDelegate:self ];				// create our socket for Listening or Direct Connection
			
	switch (transferMode) {
		case portftp:
			dataPort = portNumber;
			responseString = [ NSString stringWithFormat:@"200: PORT command successful."];
			[ dataSocket connectToHost:[self connectionAddress] onPort:portNumber error:&error ];
//			sleep(1);
			dataConnection = [[ FtpDataConnection alloc ] initWithAsyncSocket:dataSocket forConnection:self withQueuedData:queuedData ];	
			
			break;
			
		case lprtftp:					// FIXME wrong return message
			dataPort = portNumber;
			responseString = [ NSString stringWithFormat:@"228 Entering Long Passive Mode 	(af, hal, h1, h2, h3,..., pal, p1, p2...)", dataPort >>8, dataPort & 0xff];
			[ dataSocket connectToHost:[self connectionAddress] onPort:portNumber error:&error ];
			dataConnection = [[ FtpDataConnection alloc ] initWithAsyncSocket:dataSocket forConnection:self withQueuedData:queuedData ];	
			break;
			
		case eprtftp:
			dataPort = portNumber;
			responseString = @"200 EPRT command successful.";
			[ dataSocket connectToHost:[self connectionAddress] onPort:portNumber error:&error ];
			dataConnection = [[ FtpDataConnection alloc ] initWithAsyncSocket:dataSocket forConnection:self withQueuedData:queuedData ];	
			break;
			
		case pasvftp:
			dataPort = [ self choosePasvDataPort ];
			NSString *address = [ [connectionSocket localHost ] stringByReplacingOccurrencesOfString:@"." withString:@"," ]; // autoreleased
			responseString = [ NSString stringWithFormat:@"227 Entering Passive Mode (%@,%d,%d)",address, dataPort >>8, dataPort & 0xff];				
			[ dataSocket acceptOnPort: dataPort error:&error  ];
			dataConnection = nil;  // will pickup from the listening socket
			break;
			
		case epsvftp:
			dataPort = [ self choosePasvDataPort ];
			responseString = [ NSString stringWithFormat:@"229 Entering Extended Passive Mode (|||%d|)", dataPort ];				
			[ dataSocket acceptOnPort: dataPort error:&error  ];
			
			dataConnection = nil;  // will pickup from the listening socket
			break;
			
			
		default:
			break;
	}
	NSLog( @"-- %@", [ error localizedDescription ] );
	
	[ self sendMessage:responseString ];
	
	return YES;
}


// ----------------------------------------------------------------------------------------------------------
-(int)choosePasvDataPort
// ----------------------------------------------------------------------------------------------------------
{
	struct timeval tv;
    unsigned short int seed[3];
	
    gettimeofday(&tv, NULL);
    seed[0] = (tv.tv_sec >> 16) & 0xFFFF;
    seed[1] = tv.tv_sec & 0xFFFF;
    seed[2] = tv.tv_usec & 0xFFFF;
    seed48(seed);
	
	int portNumber;
	portNumber = (lrand48() % 64512) + 1024;
	//	NSLog(@"New Port number is %i", portNumber );
	
		return portNumber;																// FIXME - presetting to 2001 for the moment
	
	//return 2001;
	
}



// ==========================================================================================================
#pragma mark ASYNCSOCKET DATACONNECTION 
// ==========================================================================================================


// ----------------------------------------------------------------------------------------------------------
-(BOOL)onSocketWillConnect:(AsyncSocket *)sock 
// ----------------------------------------------------------------------------------------------------------
{
	NSLog(@"FC:onSocketWillConnect");
	[ sock readDataWithTimeout:READ_TIMEOUT tag:0 ];
	return YES;
}

// ----------------------------------------------------------------------------------------------------------
-(void)onSocket:(AsyncSocket *)sock didAcceptNewSocket:(AsyncSocket *)newSocket		// should be for a data connection on 2001
// ----------------------------------------------------------------------------------------------------------
{
	// opened socket for passive data  - new socket connected to this which is the passive connection
	
	NSLog(@"FC:New Connection -- should be for the data port");

	dataConnection = [[ FtpDataConnection alloc ] initWithAsyncSocket:newSocket forConnection:self  withQueuedData:queuedData];	
}



// ==========================================================================================================
#pragma mark ASYNCSOCKET FTPCLIENT CONNECTION 
// ==========================================================================================================

// ----------------------------------------------------------------------------------------------------------
-(void)onSocket:(AsyncSocket*)sock didReadData:(NSData*)data withTag:(long)tag			// DATA READ
// ----------------------------------------------------------------------------------------------------------
{
	
	NSLog(@"FC:didReadData");
	[ connectionSocket readDataToData:[AsyncSocket CRLFData] withTimeout:READ_TIMEOUT tag:FTP_CLIENT_REQUEST ];		// start reading again

	[ self processDataRead:data ];
//	if ( ! dataListeningSocket )			// NEXT - this would be moved to its own method - testing version from a command "new"
//	{
//		[ self setupDataListeningSocket ];
//	}		
}


// ----------------------------------------------------------------------------------------------------------
-(void)onSocket:(AsyncSocket*)sock didWriteDataWithTag:(long)tag						// DATA WRITTEN
// ----------------------------------------------------------------------------------------------------------
{
	[ connectionSocket readDataToData:[AsyncSocket CRLFData] withTimeout:READ_TIMEOUT tag:FTP_CLIENT_REQUEST ];	// start reading again	
//	NSLog(@"FC:didWriteData");

}

// ----------------------------------------------------------------------------------------------------------
-(void)sendMessage:(NSString*)ftpMessage				// REDUNDANT really - FIXME
// ----------------------------------------------------------------------------------------------------------
{
	NSLog(@">%@",ftpMessage );
	NSMutableData *dataString = [[ ftpMessage dataUsingEncoding:NSUTF8StringEncoding ] mutableCopy];				// Autoreleased
	[ dataString appendData:[AsyncSocket CRLFData] ];												

	[ connectionSocket writeData:dataString withTimeout:READ_TIMEOUT tag:FTP_CLIENT_REQUEST ];
	
	[ connectionSocket readDataToData:[AsyncSocket CRLFData] withTimeout:READ_TIMEOUT tag:FTP_CLIENT_REQUEST ];	// start reading again		
//	[ connectionSocket readDataWithTimeout:READ_TIMEOUT tag:FTP_CLIENT_REQUEST ];
}
// ----------------------------------------------------------------------------------------------------------
-(void)sendDataString:(NSString*)dataString
// ----------------------------------------------------------------------------------------------------------
{
	NSMutableData *data = [[ dataString dataUsingEncoding:NSUTF8StringEncoding ] mutableCopy];				// Autoreleased
	
	if (dataConnection )
	{	NSLog(@"FC:sendData");
		[ dataConnection writeData:data ];
		
	}
	else
	{
		[ queuedData addObject:data ];
	}

}
// ----------------------------------------------------------------------------------------------------------
-(void)sendData:(NSMutableData*)data
// ----------------------------------------------------------------------------------------------------------
{
	if (dataConnection )
	{	NSLog(@"FC:sendData");
		[ dataConnection writeData:data ];
	}
	else
	{
		[ queuedData addObject:data ];
	}
	
}
// ----------------------------------------------------------------------------------------------------------
-(void)didReceiveDataWritten			// notification from FtpDataConnection that the dataWasWritten
// ----------------------------------------------------------------------------------------------------------
{
	
	
	
	NSLog(@"SENDING COMPLETED");

	[ self sendMessage:@"226 Transfer complete." ]; // send completed message to client
	[ dataConnection closeConnection ];		
}

// ----------------------------------------------------------------------------------------------------------
-(void)didReceiveDataRead			// notification from FtpDataConnection that the dataWasWritten
// ----------------------------------------------------------------------------------------------------------
{
	
	NSLog(@"FC:didReceiveDataRead");
	
	// must have sent a file
	
	// Should start writing file out if not written yet : FIXNOW
	
	if ( currentFileHandle != nil )
	{
		// Append data on
		NSLog(@"FC:Writing File to %@", currentFile );
		[ currentFileHandle writeData:dataConnection.receivedData ];
		
	}
	else
	{
		NSLog(@"Couldnt write data");
	}
	
	
	
}

// ----------------------------------------------------------------------------------------------------------
-(void)didFinishReading							// Called at the end of a data connection from the client we presume
// ----------------------------------------------------------------------------------------------------------
{
	if (currentFile)
	{
		
		// write file to currentdir + pathname    // FIXME - look for full pathname
		
		//		[ dataConnection.receivedData writeToFile:currentFile  ];	// 
		//[ dataConnection.receivedData writeToFile:currentFile atomically:NO ];
		
		// should check or error - FIXME
		NSLog(@"Closing File Handle");
		
		currentFile = nil;
	}
	else
	{
		NSLog(@"FC:Data Sent but not sure where its for ");
	}
	
	[ self sendMessage:@"226 Transfer complete." ]; // send completed message to client
	
//	[ dataConnection closeConnection ];									// It must be closed, it dropped us

	if ( currentFileHandle != nil )
	{
		NSLog(@"Closing File Handle");
		
		[ currentFileHandle closeFile ];								// Close the file handle
		[ currentFileHandle release ];
		currentFileHandle = nil;
		[ server didReceiveFileListChanged];
	}
	
	dataConnection.connectionState = clientQuiet;

}


// ==========================================================================================================
#pragma mark PROCESS 
// =========================================================================================================

// ----------------------------------------------------------------------------------------------------------
-(void)processDataRead:(NSData*)data			// convert to commands as Client Connection
// ----------------------------------------------------------------------------------------------------------
{	
	NSData *strData = [data subdataWithRange:NSMakeRange(0, [data length] - 2)];				// remove last 2 chars
	NSString *crlfmessage = [[[NSString alloc] initWithData:strData encoding:NSUTF8StringEncoding] autorelease];
	NSString *message;
	
//	message = [ crlfmessage stringByReplacingOccurrencesOfString:@"\r\n" withString:@""];				// gets autoreleased
	
	message = [ crlfmessage stringByTrimmingCharactersInSet:[NSCharacterSet newlineCharacterSet ]];
	NSLog(@"<%@",message );
    msgComponents = [message componentsSeparatedByString:@" "];				// change this to use spaces - for the FTP protocol
	
	//	NSLog(@"<%@  args %i", message, [ msgComponents count]);
	
	[ self processCommand ];
	
	[connectionSocket readDataToData:[AsyncSocket CRLFData] withTimeout:-1 tag:0 ];					// force to readdata CHECK
}

// ----------------------------------------------------------------------------------------------------------
-(void)processCommand // assumes data has been place in Array msgComponents
// ----------------------------------------------------------------------------------------------------------
{
	NSString *commandString =  [ msgComponents objectAtIndex:0];
			
	if ([ commandString length ] > 0)																// If there is a command here
	{
		// Search through dictionary for correct matching command and method that it calls
		
		NSString *commandSelector = [ [[server commands] objectForKey:[commandString lowercaseString] ] stringByAppendingString:@"arguments:"];
		
		if ( commandSelector )																		// If we have a matching command
		{
			
			SEL action = NSSelectorFromString(commandSelector);										// Turn into a method
			
			if ( [ self respondsToSelector:action ])												// If we respond to this method
			{	
						// DO COMMAND
				[self performSelector:action withObject:self withObject:msgComponents ];			// Preform method with arguments
			}
			else
			{			// UNKNOWN COMMAND
				NSString *outputString =[ NSString stringWithFormat:@"500 '%@': command not understood.", commandString ];
				[ self sendMessage:outputString ];
				
				NSLog(@"DONT UNDERSTAND");
			}
		}
		else			// UNKNOWN COMMAND
		{
			NSString *outputString =[ NSString stringWithFormat:@"500 '%@': command not understood.", commandString ];
			[ self sendMessage:outputString ];
		}
	}
	else
	{
		// Write out an error msg
	}
	
}

// ==========================================================================================================
#pragma mark COMMANDS 
// ==========================================================================================================

// ----------------------------------------------------------------------------------------------------------
-(void)doQuit:(id)sender arguments:(NSArray*)arguments
// ----------------------------------------------------------------------------------------------------------
{
	NSLog(@"Quit : %@",arguments);

	[ self sendMessage:@"221- Data traffic for this session was 0 bytes in 0 files"];
	
	[self sendMessage:@"221 Thank you for using the game-editor FTP service" ];
			
	[connectionSocket disconnectAfterWriting ];				// Will this close the socket ?
	
	[ server closeConnection:self ];			// Tell the server to close us down, remove us from the list of connections
	
	
	// FIXME - delete the dataconnection if its open
	
	
}
// ----------------------------------------------------------------------------------------------------------
-(void)doUser:(id)sender arguments:(NSArray*)arguments
// ----------------------------------------------------------------------------------------------------------
{
	// send out confirmation message -- 331 password required for
	NSString *user =  [ arguments objectAtIndex:1 ];
	NSString *outputString = [ NSString stringWithFormat:@"331 Password required for %@", user ];
	[ sender sendMessage:outputString];
}
// ----------------------------------------------------------------------------------------------------------
-(void)doPass:(id)sender arguments:(NSArray*)arguments
// ----------------------------------------------------------------------------------------------------------
{
	NSString *pass = [ arguments objectAtIndex:1 ]; 
	NSString *outputString = [ NSString stringWithFormat:@"230 User %@ logged in.", pass ];
	[ sender sendMessage:outputString];
}
// ----------------------------------------------------------------------------------------------------------
-(void)doStat:(id)sender arguments:(NSArray*)arguments
// ----------------------------------------------------------------------------------------------------------
{
	// Send out stat message
	[ sender sendMessage:@"211-localhost FTP server status:"];
	// FIXME - add in the stats
	[ sender sendMessage:@"211 End of Status"];
}
// ----------------------------------------------------------------------------------------------------------
-(void)doFeat:(id)sender arguments:(NSArray*)arguments
// ----------------------------------------------------------------------------------------------------------
{
	[ sender sendMessage:@"211-Features supported"];
	[ sender sendMessage:@"211 End"];
}
// ----------------------------------------------------------------------------------------------------------
-(void)doList:(id)sender arguments:(NSArray*)arguments
// ----------------------------------------------------------------------------------------------------------
{
	NSLog(currentDir);
	NSString *listText = [ [ server createList:currentDir] retain ];	

	[ sender sendMessage:@"150: Opening ASCII mode data connection for '/bin/ls'."];	
	[ sender sendDataString:listText  ];	
	
	[listText release ];
}
// ----------------------------------------------------------------------------------------------------------
-(void)doPwd:(id)sender arguments:(NSArray*)arguments
// ----------------------------------------------------------------------------------------------------------
{
	NSString *cmdString = [ NSString stringWithFormat:@"257 \"%@\" is the current directory.", currentDir ]; // autoreleased
	[ sender sendMessage:cmdString ];			// FIXME - seems to be buggy on ftp command line client
	
}
// ----------------------------------------------------------------------------------------------------------
-(void)doNoop:(id)sender arguments:(NSArray*)arguments
// ----------------------------------------------------------------------------------------------------------
{
	[sender sendMessage:@"200 NOOP command successful." ];
	
}
// ----------------------------------------------------------------------------------------------------------
-(void)doSyst:(id)sender arguments:(NSArray*)arguments
// ----------------------------------------------------------------------------------------------------------
{
	[ sender sendMessage:@"215 UNIX Type: L8 Version: game-editor 20080912" ];
}
// ----------------------------------------------------------------------------------------------------------
-(void)doLprt:(id)sender arguments:(NSArray*)arguments
// ----------------------------------------------------------------------------------------------------------
{	
	// LPRT,"6,16,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,2,231,92"	
	NSString *socketDesc = [ arguments objectAtIndex:1 ] ;
	NSArray *socketAddr = [ socketDesc componentsSeparatedByString:@"," ];

	int hb = [[socketAddr objectAtIndex:19] intValue ];
	int lb = [[socketAddr objectAtIndex:20] intValue ];

	NSLog(@"%d %d %d",hb <<8, hb,lb );
	int clientPort = (hb <<8 ) + lb;
		
	[sender setTransferMode:lprtftp];
	
	[ sender openDataSocket:clientPort ];
}
// ----------------------------------------------------------------------------------------------------------
-(void)doEprt:(id)sender arguments:(NSArray*)arguments
// ----------------------------------------------------------------------------------------------------------
{
	// EPRT |2|1080::8:800:200C:417A|5282|
	
	NSString *socketDesc = [ arguments objectAtIndex:1 ] ;
	NSArray *socketAddr = [ socketDesc componentsSeparatedByString:@"|" ];
	
	NSString *item;
	for (item in socketAddr) {
		NSLog(item);
	}
	int clientPort = [[ socketAddr objectAtIndex:3 ] intValue ];
	
	NSLog(@"Got Send Port %d", clientPort );
	
	[sender setTransferMode:eprtftp];
	//	[ sender initDataSocket:clientPort ];
	[ sender openDataSocket:clientPort ];
}


// ----------------------------------------------------------------------------------------------------------
-(void)doPasv:(id)sender arguments:(NSArray*)arguments
// ----------------------------------------------------------------------------------------------------------
{
	
	[sender setTransferMode:pasvftp];
	//	[ sender initDataSocket:0 ];
	[ sender openDataSocket:0 ];
	
}
// ----------------------------------------------------------------------------------------------------------
-(void)doEpsv:(id)sender arguments:(NSArray*)arguments
// ----------------------------------------------------------------------------------------------------------
{
	// FIXME - open a port random high address
	[sender setTransferMode:epsvftp];
	//	[ sender initDataSocket:0 ];
	[ sender openDataSocket:0 ];
}
// ----------------------------------------------------------------------------------------------------------
-(void)doPort:(id)sender arguments:(NSArray*)arguments
// ----------------------------------------------------------------------------------------------------------
{
	int hb, lb;

	//  PORT 127,0,0,1,197,251  looks like this
	// get 2nd argument and split up by , and then take the last 2 bits

	NSString *socketDesc = [ arguments objectAtIndex:1 ] ;
	NSArray *socketAddr = [ socketDesc componentsSeparatedByString:@"," ];

	hb = [[socketAddr objectAtIndex:4] intValue ];
	lb = [[socketAddr objectAtIndex:5] intValue ];
	
	
	int clientPort = (hb <<8 ) + lb;
	
	
	[sender setTransferMode:portftp];
	
	[ sender openDataSocket:clientPort ];
	
}

// ----------------------------------------------------------------------------------------------------------
-(void)doOpts:(id)sender arguments:(NSArray*)arguments
// ----------------------------------------------------------------------------------------------------------
{
	NSString *cmd = [ arguments objectAtIndex:1 ];
	NSString *cmdstr = [ NSString stringWithFormat:@"502 Unknown command '%@'",cmd ];
	[ sender sendMessage:cmdstr ];
	//	502 Unknown command 'sj'		
}
// ----------------------------------------------------------------------------------------------------------
-(void)doType:(id)sender arguments:(NSArray*)arguments
// ----------------------------------------------------------------------------------------------------------
{
	//		200 Type set to A.
	
	// FIXME - change the data output to the matching type -- we dont do anything with this yet
	
	NSString *cmd =  [ arguments objectAtIndex:1 ];
	
	//if ( [ [cmd lowercaseString] isEqualToString:@"i" ])
//	{
		NSString *cmdstr = [ NSString stringWithFormat:@"200 Type set to  %@.",cmd ];
		[ sender sendMessage:cmdstr ];
//	}
//	else
//	{
//		NSString *cmdstr = [ NSString stringWithFormat:@"500 'type %@': command not understood.",cmd ];
//		[ sender sendMessage:cmdstr ];		
//	}
}
// ----------------------------------------------------------------------------------------------------------
-(void)doCwd:(id)sender arguments:(NSArray*)arguments
// ----------------------------------------------------------------------------------------------------------
{
//	250 
//	500, 501, 502, 421, 530, 550
//	250 CWD command successful.
	
	NSString *cwdDir = [ self fileNameFromArgs:arguments] ;				// FIXME - this might need to be retain	
	if ([cwdDir isEqualToString:@".."])
	{
		// remove one part of the path.. and display
		NSLog(@"Asked to go up a directory");
	}
	else
	{
		// if its not a full path - then add on the currentDir // FIXME eg Documents   add currentdir/documents
		
		currentDir = [[ self fileNameFromArgs:arguments] copy ];				// FIXME - this might need to be retain
		NSLog(@"currentDir is now %@",currentDir );
	}
	
	[ sender sendMessage:@"250 CWD command successful." ];

}
// ----------------------------------------------------------------------------------------------------------
-(void)doNlst:(id)sender arguments:(NSArray*)arguments
// ----------------------------------------------------------------------------------------------------------
{
	[self doList:sender arguments:arguments ];
}
// ----------------------------------------------------------------------------------------------------------
-(void)doStor:(id)sender arguments:(NSArray*)arguments
// ----------------------------------------------------------------------------------------------------------
{
	// eg STOR my filename here.mp3        STOR=0 my=1 filename=2 etc
	
	NSString *filename =[self fileNameFromArgs:arguments];		// autoreleased
	NSString *cmdstr = [ NSString stringWithFormat:@"150: Opening BINARY mode data connection for '%@'.",filename ];	// autoreleased
	
	currentFile = [ [currentDir stringByAppendingPathComponent:filename] retain ];					// Hang onto the filename, then we know where to write any data that is about to come
	
	
	// CREATE and OPEN NEW FILE FOR WRITING
	if ([[NSFileManager defaultManager] createFileAtPath:currentFile contents:nil attributes:nil]==YES)
	{
		currentFileHandle = [[ NSFileHandle fileHandleForWritingAtPath:currentFile] retain];				// Open the file handle to write to
		
	}
	else
	{
		// couldnt do it
		NSLog(@"Unable to create file %@ to write to.", currentFile);
	}
	

	NSLog(@"FC:doStor  %@", currentFile );
	// connection will be set to receive alredy by prev port of passv command - i hope...
	
	[sender sendMessage:cmdstr];

	if (dataConnection )																			// if not we're in trouble. mind u currentFile is doing similar job. 
	{
		NSLog(@"FC:setting connection state to clientSending");
		dataConnection.connectionState = clientSending;
		
	}
	else
	{
		NSLog(@"FC:Erorr  Cant set connection state to Client Sending : no Connection yet ");
	}
	
	
}

// ----------------------------------------------------------------------------------------------------------
-(void)doRetr:(id)sender arguments:(NSArray*)arguments			// DOWNLOAD to CLIENT
// ----------------------------------------------------------------------------------------------------------
{
	NSString *filename =[self fileNameFromArgs:arguments];		// autoreleased
	NSString *cmdstr; 
	
	// get file that is listed from the directory or current directory
	
	NSString *filePath = [ currentDir stringByAppendingPathComponent:filename];

	NSLog(@"doRetr: %@", filePath );
	
	if ( [ [ NSFileManager defaultManager] fileExistsAtPath:filePath ])
	{
		NSMutableData   *fileData = [[ NSMutableData dataWithContentsOfFile:filePath ] retain];										// FIXME - open in bits ? seems risky opening file in one piece
		
		cmdstr = [ NSString stringWithFormat:@"150: Opening BINARY mode data connection for '%@'.",filename ];
		[sender sendMessage:cmdstr];

		[ sender sendData:fileData  ];	
		
		[fileData release ];
	}
	else
	{
		cmdstr = [ NSString stringWithFormat:@"50 %@: No such file or directory.",filename ];		
		NSLog(@"FC:doRetr: file %@ doesnt' exist ", filePath);		
	}
}

// ----------------------------------------------------------------------------------------------------------
-(void)doDele:(id)sender arguments:(NSArray*)arguments
// ----------------------------------------------------------------------------------------------------------
{
	NSError *error;
	NSString *filename =[self fileNameFromArgs:arguments];		// autoreleased
	NSString *filePath = [ currentDir stringByAppendingPathComponent:filename ];
	NSString *cmdStr;
	
	// attempt to delete the file
	
	if ( [[ NSFileManager defaultManager ] fileExistsAtPath:filePath ])
	{				
		if ([[ NSFileManager defaultManager ] removeItemAtPath:filePath error:&error ])
		{
			cmdStr = [ NSString stringWithFormat:@"250: DELE command successful.",filename ];
		}
		else
		{
			cmdStr = [ NSString stringWithFormat:@"550: DELE command unsuccessful.",filename ];					// FIXME put correct error code in
		}
	}
	else
	{
		cmdStr = [ NSString stringWithFormat:@"550 %@: No such file or directory.", filename];
	}

	[ sender sendMessage:cmdStr	];
	
	// 250: DELE command successful.
}
// ----------------------------------------------------------------------------------------------------------
-(void)doMlst:(id)sender arguments:(NSArray*)arguments
// ----------------------------------------------------------------------------------------------------------
{
	NSString *filename = [self fileNameFromArgs: arguments];
	NSString *cmdstr = [ NSString stringWithFormat:@"150: Opening BINARY mode data connection for '%@'.",filename ];
	// tell connection to expect a file	
	
	[sender sendMessage:cmdstr];
/*
 250: MLST 2012.pdf
 Type=file;Size=2420017;Modify=20080808074805;Perm=adfrw;Unique=AgAADpIHZwA; /Users/diddy/Documents/2012.pdf
 End
 */
}
// ----------------------------------------------------------------------------------------------------------
-(void)doSize:(id)sender arguments:(NSArray*)arguments
// ----------------------------------------------------------------------------------------------------------
{
	NSString *filename = [self fileNameFromArgs: arguments];
	NSString *cmdStr;
	// if filename exists
	NSString *filePath = [ currentDir stringByAppendingPathComponent:filename ];
	
	BOOL fileExists = [[ NSFileManager defaultManager ] fileExistsAtPath:filePath ];
	
	if (fileExists)			// FIXME - change to check for filename existence
	{
		if ([self fileSize:filePath] <10240)		// FIXME - check size of file
		{
			cmdStr = [ NSString stringWithFormat:@"213 %qu",[self fileSize:filePath] ];			// FIXME - add in the actual size of the file
		}
		else
		{
			cmdStr = [ NSString stringWithFormat:@"550 %@ file too large for SIZE.",filename ];
		}

	}
	else
	{
		cmdStr = [ NSString stringWithFormat:@"550 %@: No such file or directory.",filename ];

	}
	
	
	// tell connection to expect a file	
	
	[sender sendMessage:cmdStr];
	/*
	 250: MLST 2012.pdf
	 Type=file;Size=2420017;Modify=20080808074805;Perm=adfrw;Unique=AgAADpIHZwA; /Users/diddy/Documents/2012.pdf
	 End
	 */
}

//213 0

#pragma mark UTILITIES
// ----------------------------------------------------------------------------------------------------------
-(unsigned long long)fileSize:(NSString*)filePath
// ----------------------------------------------------------------------------------------------------------
{

	
	NSError *error;
	NSDictionary *fileAttribs = [ [ NSFileManager defaultManager ] attributesOfItemAtPath:filePath error:&error ];

	NSNumber *fileSize = [ fileAttribs valueForKey:NSFileSize ];
	NSLog(@"File size is %qu ", [fileSize unsignedLongLongValue]);
	return [ fileSize unsignedLongLongValue];

}
// ----------------------------------------------------------------------------------------------------------
-(NSString*)fileNameFromArgs:(NSArray*)arguments
// ----------------------------------------------------------------------------------------------------------
{
	NSLog([arguments description ] );
	NSString *filename =[arguments objectAtIndex:1];												// First part of filename , may be followed by other bits which were space delimited.
	
	for ( int n=2; n<[arguments count]; n++) {														// Start at arg2, 
		filename = [ NSString stringWithFormat:@"%@ %@", filename, [arguments objectAtIndex:n ] ];						// autoreleased
	}

	return  filename  ;			// autoreleased
}
@end
