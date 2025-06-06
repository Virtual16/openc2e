/*
 *  mmapifstream.h
 *  openc2e
 *
 *  Created by Alyssa Milburn on Sat Jul 24 2004.
 *  Copyright (c) 2004 Alyssa Milburn. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 */

#ifndef _OPENC2E_MMAPIFSTREAM_H
#define _OPENC2E_MMAPIFSTREAM_H

#include <string>
#include <fstream>
#ifdef _WIN32
#include <windows.h>
#endif

// Destructor properly closes any open file and mapping
class mmapifstream : public std::ifstream {
public:
        bool live;
        unsigned int filesize;
        char *map;
#ifdef _WIN32
        HANDLE hFile;
        HANDLE hMap;
#endif
        mmapifstream() { live = false; }
        mmapifstream(std::string filename);
        ~mmapifstream(); // cleans up resources
        void mmapopen(std::string filename);
};

#endif
/* vim: set noet: */
