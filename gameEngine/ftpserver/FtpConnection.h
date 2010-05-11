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



#import <UIKit/UIKit.h>


#import "AsyncSocket.h"

#import "FtpDataConnection.h"
#import "FtpDefines.h"
#include <sys/time.h>
#include <time.h>


@class FtpServer;

@interface FtpConnection : NSObject {
	AsyncSocket			*connectionSocket;					// Socket for this particular connection
	FtpServer			*server;							// pointer to Server object
	
	AsyncSocket			*dataListeningSocket;				// Socket to listen for a data connection to spawn on
	AsyncSocket         *dataSocket;						// duplicates the listening socket - remove listening socket from code when workin
	
	FtpDataConnection	*dataConnection;					// instance handling spawned data connection socket
	
	NSArray				*msgComponents;						// The Message rcvd broken into an array
	UInt16				dataPort;
	int					transferMode;
	NSMutableArray		*queuedData;	
	
	NSString			*currentDir;
	NSString			*currentFile;						// File that is about to be uploaded
	NSFileHandle		*currentFileHandle;					// File handle of what to save

}

-(id)initWithAsyncSocket:(AsyncSocket*)newSocket forServer:(id)myServer ;
#pragma mark STATE
@property(readwrite,copy) NSString *currentDir;
@property(readwrite)int transferMode;


-(NSString*)connectionAddress;


#pragma mark ASYNCSOCKET DATACONN 
-(void)setupDataListeningSocket;
-(BOOL)openDataSocket:(int)portNumber;
-(int)choosePasvDataPort;


-(BOOL)onSocketWillConnect:(AsyncSocket *)sock;
-(void)onSocket:(AsyncSocket *)sock didAcceptNewSocket:(AsyncSocket *)newSocket;

#pragma mark ASYNCSOCKET FTPCLIENT CONNECTION 
-(void)onSocket:(AsyncSocket*)sock didReadData:(NSData*)data withTag:(long)tag;
-(void)onSocket:(AsyncSocket*)sock didWriteDataWithTag:(long)tag;
-(void)sendMessage:(NSString*)ftpMessage;							// calls FC  writedata
-(void)sendDataString:(NSString*)dataString;								// calls FDC writedata
-(void)sendData:(NSMutableData*)data;
-(void)didReceiveDataWritten;										// notification that FDC wrote data
-(void)didReceiveDataRead;											// notification that FDC read data ie a transfer
-(void)didFinishReading;											// Called at the closing ==  end of a data connection from the client we presume

#pragma mark PROCESS 

-(void)processDataRead:(NSData*)data;
-(void)processCommand;



#pragma mark COMMANDS 
-(void)doQuit:(id)sender arguments:(NSArray*)arguments;
-(void)doUser:(id)sender arguments:(NSArray*)arguments;
-(void)doPass:(id)sender arguments:(NSArray*)arguments;
-(void)doStat:(id)sender arguments:(NSArray*)arguments;
-(void)doFeat:(id)sender arguments:(NSArray*)arguments;
-(void)doList:(id)sender arguments:(NSArray*)arguments;
-(void)doPwd:(id)sender arguments:(NSArray*)arguments;
-(void)doNoop:(id)sender arguments:(NSArray*)arguments;
-(void)doSyst:(id)sender arguments:(NSArray*)arguments;
-(void)doLprt:(id)sender arguments:(NSArray*)arguments;
-(void)doPasv:(id)sender arguments:(NSArray*)arguments;
-(void)doEpsv:(id)sender arguments:(NSArray*)arguments;
-(void)doPort:(id)sender arguments:(NSArray*)arguments;
-(void)doNlst:(id)sender arguments:(NSArray*)arguments;
-(void)doStor:(id)sender arguments:(NSArray*)arguments;
-(void)doRetr:(id)sender arguments:(NSArray*)arguments;
-(void)doDele:(id)sender arguments:(NSArray*)arguments;
-(void)doMlst:(id)sender arguments:(NSArray*)arguments;
-(void)doSize:(id)sender arguments:(NSArray*)arguments;

#pragma mark UTITILITES
-(unsigned long long)fileSize:(NSString*)filePath;
-(NSString*)fileNameFromArgs:(NSArray*)arguments;
@end
