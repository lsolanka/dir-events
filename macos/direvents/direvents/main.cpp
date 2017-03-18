//
//  main.cpp
//  direvents
//
//  Created by Lukas Solanka on 18/03/2017.
//  Copyright © 2017 Lukas Solanka. All rights reserved.
//

#include <iostream>
#include <iomanip>

#include <CoreServices/CoreServices.h>

void event_callback(
        ConstFSEventStreamRef streamRef,
        void *clientCallBackInfo,
        size_t numEvents,
        void *eventPaths,
        const FSEventStreamEventFlags eventFlags[],
        const FSEventStreamEventId eventIds[])
{
    //std::cout << "called callback" << std::endl;
    for (int i = 0; i < numEvents; ++i)
    {
        std::cout << "Changed " << std::hex << std::showbase << eventIds[i] <<
                     " in " << std::quoted(((const char**)eventPaths)[i]) <<
                     ", flags: " << std::hex << eventFlags[i] << std::endl;
    }
}

int main(int argc, const char * argv[]) {
    if (argc != 2)
    {
        std::cout << "Usage: " << argv[0] << " <directory>" << std::endl;
        return EXIT_FAILURE;
    }

    const char* path_to_scan = argv[1];

    CFStringRef mypath = CFStringCreateWithCString(NULL, path_to_scan, kCFStringEncodingASCII);
    CFArrayRef pathsToWatch = CFArrayCreate(NULL, (const void **)&mypath, 1, NULL);
    FSEventStreamRef stream;
    CFAbsoluteTime latency = 3.0; /* Latency in seconds */
 
    stream = FSEventStreamCreate(NULL,
        &event_callback,
        NULL,
        pathsToWatch,
        kFSEventStreamEventIdSinceNow, /* Or a previous event ID */
        latency,
        kFSEventStreamCreateFlagNone /* Flags explained in reference */
    );

    /* Create the stream before calling this. */
    FSEventStreamScheduleWithRunLoop(stream, CFRunLoopGetCurrent(), kCFRunLoopDefaultMode);
    FSEventStreamStart(stream);

    std::cout << "Started the stream." << std::endl;
    std::cout << "Starting the loop." << std::endl;

    CFRunLoopRun();

    return EXIT_SUCCESS;
}
