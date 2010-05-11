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


// 11/02/08 changes to made to allow stopping of the server.  Also added a bit of code to show users IP address.
// Cleaned up the code to make the retains and releases a bit more obvious... mostly programming style preferences.  
// Added a stop Ftp server method to be called from where ever it was start.  See iphoneLibTestViewController.m to see how I was calling it.
// Most users wouldn't want the server to continue to run once the transfers are done.


#import "FtpServer.h"

@implementation FtpServer

@synthesize listenSocket, connectedSockets, server, notificationObject, portNumber, delegate, commands, baseDir, connections;

// ----------------------------------------------------------------------------------------------------------
- (id)initWithPort:(unsigned)serverPort withDir:(NSString*)aDirectory notifyObject:(id)sender
// ----------------------------------------------------------------------------------------------------------
{
    if( self = [super init] ) {
		
		NSError *error = nil;
		
		self.notificationObject = sender;
		
		// Load up commands
		NSString *plistPath = [[NSBundle mainBundle] pathForResource:@"ftp_commands" ofType:@"plist"];
		if ( ! [ [ NSFileManager defaultManager ] fileExistsAtPath:plistPath ] )
		{
			NSLog(@"ftp_commands.plist missing");
		}
		
		
		NSDictionary *myCommands = [[NSDictionary alloc] initWithContentsOfFile:plistPath];
		self.commands = myCommands;
		[myCommands release];

		// Clear out connections list
		NSMutableArray *myConnections = [[NSMutableArray alloc] init];
		self.connections = myConnections;
		[myConnections release];

		
		// Create a socket
        self.portNumber = serverPort;
		
		AsyncSocket *myListenSocket = [[AsyncSocket alloc] initWithDelegate:self];
		self.listenSocket = myListenSocket;
		[myListenSocket release];
		
		NSLog(@"Listening on %u", portNumber);
		[listenSocket acceptOnPort:serverPort error:&error];					// start lisetning on this port.
		
		NSMutableArray *myConnectedSockets = [[NSMutableArray alloc] initWithCapacity:1];
		self.connectedSockets = myConnectedSockets;
		[myConnectedSockets release];
		
		self.baseDir = aDirectory;													// FIXME dealloc

    }
    return self;
}

#pragma mark ASYNCSOCKET DELEGATES
// ----------------------------------------------------------------------------------------------------------
- (void)onSocket:(AsyncSocket *)sock didAcceptNewSocket:(AsyncSocket *)newSocket
// ----------------------------------------------------------------------------------------------------------
{
	// Create an ftp connection
	FtpConnection *newConnection = [[ FtpConnection alloc ] initWithAsyncSocket:newSocket forServer:self];	
	
	// Make a copy of the basedir
	newConnection.currentDir = baseDir;																		
	
	[connections addObject:newConnection];			// Add this to our list of connections
	
	NSLog(@"FS:didAcceptNewSocket  port:%i", [sock localPort]);
	
	if ([sock localPort] == portNumber ) {
		NSLog(@"Connection on Server Port");
	} else {
		// must be a data comms port
		// spawn a data comms port
		// look for the connection with the same port
		// and attach it
		NSLog(@"--ERROR %i, %i", [sock localPort],portNumber);
	}
}

// ----------------------------------------------------------------------------------------------------------
- (void)onSocket:(AsyncSocket *)sock didConnectToHost:(NSString *)host port:(UInt16)port
// ----------------------------------------------------------------------------------------------------------
{
	NSLog(@"FtpServer:didConnectToHost  port:%i", [sock localPort]);
}

#pragma mark NOTIFICATIONS
// ----------------------------------------------------------------------------------------------------------
-(void)didReceiveFileListChanged
// ----------------------------------------------------------------------------------------------------------
{
//	[notificationObject didReceiveFileListChanged];
}
#pragma mark CONNECTIONS
// ----------------------------------------------------------------------------------------------------------
- (void)closeConnection:(id)theConnection
// ----------------------------------------------------------------------------------------------------------
{
	// Search through connections for this one - and delete
	// this should release it - and delloc

	[connections removeObject:theConnection ];


}

// ----------------------------------------------------------------------------------------------------------
-(NSString*)createList:(NSString*)directoryPath
// ----------------------------------------------------------------------------------------------------------
{ 
	return createList(directoryPath);
}

// ----------------------------------------------------------------------------------------------------------
- (void)dealloc
// ----------------------------------------------------------------------------------------------------------
{   
	NSLog(@"Dealloc got called");
	[listenSocket release];
	[connectedSockets release];
	[server release];
	[notificationObject release];
	[delegate release];	
	[connections release];
	[commands release];
	[baseDir release];

	[super dealloc];
}

- (BOOL)stopFtpServer {
	
	//make a copy as the original will change as the connections are shutdown.
	NSMutableArray *myConnections = [[self connections] copy];
	int i;
	for (i = 0; i < myConnections.count; i++) {
		// tell each connecton to stop by faking a quit command.s
		NSLog(@"stopping %d connections", myConnections.count);
		[[myConnections objectAtIndex:i] doQuit:self arguments:[NSArray arrayWithObjects:@"byebye", nil]];
	}
	[myConnections release];
	return YES;
}

@end
