//
//  system_macosx.h
//  gameEditor
//
//  Created by Andreas on 3/31/10.
//  Copyright 2010 Visionware. All rights reserved.
//

//#import <SenTestingKit/SenTestingKit.h>

#import <Cocoa/Cocoa.h>
#import "gedString.h"
@interface system_macosx:NSObject {
	
}
- (id)init;
- (NSString *)openDoc :(ListString&) Filter CanOpen:(bool) bOpen SaveName:(gedString) sName;
- (void) openUrl:(const char*) url;
- (const char*) getResourcePath;
@end

