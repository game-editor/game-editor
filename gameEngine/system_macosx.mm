//
//  system_macosx.m
//  gameEditor
//
//  Created by Andreas on 3/31/10.
//  Copyright 2010 Visionware. All rights reserved.
//

#import "system_macosx.h"
#import <Cocoa/Cocoa.h>

#include "gedString.h"


@implementation system_macosx
-(id)init {
	if(self= [super init]) {
		
	}
	return self;
}

- (NSString *)openDoc :(ListString&) multiFilter CanOpen:(bool) bOpen SaveName:(gedString) saveName
{
	NSFileManager *filemgr=[NSFileManager defaultManager];
	NSString *path, *sName;
	static NSString *aFile=@"";
	gedString test,filter;
	NSMutableArray *fileTypes=[[NSMutableArray alloc]init];
	

	
	if(saveName.length() > 0)	
		sName=[NSString stringWithUTF8String:saveName.getCharBuf()];
	else {
		sName=@"";
	}

	if(bOpen)
	{
//		filter=[NSString stringWithUTF8String:multiFilter.getCharBuf()];
		
		//NSArray *fileTypes = [NSArray arrayWithObject:multiFilter];

		if(multiFilter.Count())
		{
			//Make filter
			int i0;
			
			//Description		
			for(i0 = 0; i0 < multiFilter.Count(); i0++)
			{	
				char *c=multiFilter[i0].getCharBuf();
				NSString *t=[[NSString alloc]initWithUTF8String:c];
				
				
				//Type
				[fileTypes addObject:t];
				
				[t release];
			}
		}
		
		
		
		NSOpenPanel *oPanel = [NSOpenPanel openPanel];
	
	
		[oPanel setAllowsMultipleSelection:NO];
		[oPanel setCanChooseFiles:YES];
	
		int result = [oPanel runModalForDirectory:nil
			  
									 file:sName types:fileTypes];
	
		if (result == NSOKButton) {
		
        NSArray *filesToOpen = [oPanel filenames];
		
		aFile = [filesToOpen objectAtIndex:0];
			
			path=[aFile stringByDeletingLastPathComponent];
			if ([filemgr changeCurrentDirectoryPath:path] == NO)
				NSLog (@"Cannot change directory.");
			
			
        
		}
	} else {

		NSSavePanel *sPanel = [NSSavePanel savePanel];
		//NSString *aFile=@"";
		
				
		int result = [sPanel runModalForDirectory:nil
					  
											 file:sName ];
		if (result == NSOKButton) {
			
			aFile = [sPanel filename];
			
			path=[aFile stringByDeletingLastPathComponent];
			if ([filemgr changeCurrentDirectoryPath:path] == NO)
				NSLog (@"Cannot change directory.");
			
		}

		
	}
	
	
    
	return aFile;
}
- (void) openUrl:(const char*) url;
{
	[[NSWorkspace sharedWorkspace] openURL:[NSURL URLWithString:[NSString stringWithUTF8String:url]]];	
}
-(const char*) getResourcePath;
{
	return [[[NSBundle mainBundle] resourcePath]UTF8String]; 	
}
-(const char*) getBundlePath;
{
	return [[[NSBundle mainBundle] bundlePath]UTF8String]; 	
}

@end
gedString FileDialog(ListString& multiFilter, bool bOpen, gedString saveName = "")
{	gedString s=multiFilter[0];
	gedString filter;
	 system_macosx *system_mosx=[[system_macosx alloc]init];

	
	gedString file= [ [system_mosx openDoc :multiFilter CanOpen:bOpen SaveName:saveName]UTF8String]	;

	if (file.length() >0)
		chdir(file.GetFilePath().getCharBuf());
	[system_mosx release];
	return file.GetFileName(); 

}
const char* getResourcePath(void)
{
	const char* r;
	
	system_macosx *system_mosx=[[system_macosx alloc]init];
	r= [system_mosx getResourcePath];
	
	[system_mosx release];
	return r;
}
const char* getBundlePath(void)
{
	const char* r;
	
	system_macosx *system_mosx=[[system_macosx alloc]init];
	r= [system_mosx getBundlePath];
	
	[system_mosx release];
	return r;
}

extern "C"
{
void openUrl(const char *url)
{
	system_macosx *system_mosx=[[system_macosx alloc]init];
	[system_mosx openUrl:url];

	[system_mosx release];

}
	

}