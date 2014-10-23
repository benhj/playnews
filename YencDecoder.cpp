/*
Copyright (c) 2013-2014, BenHJ

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the <organization> nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "YencDecoder.h"
#include <QDebug>
#include <string>
#include <iostream>
#include <algorithm>
#include <functional>
#include <cctype>
#include <locale>


typedef unsigned int uInt;
typedef unsigned long uLong;

namespace {

    //
    // Following trimming functions are from
    // http://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
    //
    // trim from start
    static inline std::string &ltrim(std::string &s) {
            s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
            return s;
    }

    // trim from end
    static inline std::string &rtrim(std::string &s) {
            s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
            return s;
    }

    // trim from both ends
    static inline std::string &trim(std::string &s) {
            return ltrim(rtrim(s));
    }

    /**
     * @brief getAttribute pinched from code at https://code.google.com/p/libyenc/source/browse/trunk/src/ydecoder.cpp
     * @param read_buffer the string containing the data
     * @param attr the attribute to return data of
     * @return the attribute data
     */
    const char* getAttribute( std::string const &read_buffer, const char *attr )
    {
        int pos = read_buffer.find( attr );

        if( pos ){
            const char *text = read_buffer.c_str();
            int skip = pos + strlen( attr ) + 1;

            while( skip ){
                *text++;
                skip--;
            }

            return text;
        }

        return NULL;
    }

    /**
     * @brief getName also from https://code.google.com/p/libyenc/source/browse/trunk/src/ydecoder.cpp
     * @param read_buffer the string containing the data
     * @return the file name
     */
    const char* getName(std::string const &read_buffer)
    {
        int pos = read_buffer.find( "name" );

        if( pos ){
            const char *text = read_buffer.c_str();
            int skip = pos + 5;

            while( skip ){
                *text++;
                skip--;
            }

            std::stringstream stream;

            while( !( *text == '\r' || *text == '\n' ) ){
                stream << *text;
                *text++;
            }

            std::string name = stream.str();
            trim( name );
            int len = name.length() + 1;
            char *name_ = new char[len];
            strcpy( name_, name.c_str() );
            return name_;
        }

        return NULL;
    }


}

//
// Code originally adapted from https://raw.github.com/madgeekfiend/yenc/master/lib/y_enc.rb
//

YencDecoder::YencDecoder():m_fileSize(0)
{
}

bool
YencDecoder::setHead(std::istream &inputStream)
{
    std::string line;
    bool found = false;
    while ( std::getline(inputStream, line) ) {

        //
        // Process header part
        //
        if(line.find("=ybegin") != std::string::npos) {
            parseHeadLine(line);
            found = true;
            break;
        }
    }
    return found;
}

void
YencDecoder::decode(std::istream &inputStream, std::ostream &outputStream)
{
    std::string line;
    bool beginRead = false;
    while ( std::getline(inputStream, line) ) {
        //
        // Process header part
        //
        if(line.find("=ybegin") != std::string::npos) {
            beginRead = true;
            continue;
        }

        //
        // Begin reading proper
        //
        if(beginRead){
            while ( std::getline(inputStream, line) ) {

                //
                // Process ending part
                //
                if(line.find("=yend") != std::string::npos) {
                    parseTailLine(line);
                    beginRead = false;
                    break;
                }

                std::string::iterator it;
                std::vector<char> buf;
                for(it = line.begin(); it < line.end(); it++) {

                    if( *it == '.' && (it - line.begin() == 0)) {

                        if (*(it + 1) == '.') {
                            continue;
                        }
                    }

                    //
                    // code fragment adapted from
                    // https://code.google.com/p/libyenc/source/browse/trunk/src/ydecoder.cpp
                    //
                    if( *it == '\r' || *it == '\n') {
                        continue;
                    }

                    if( *it == '=' ) {
                        it++;
                        *it -= 64;
                    }

                    *it -= 42;
                    buf.push_back(*it);
                }
                outputStream.write((char*)&buf.front(), buf.size());

            }
            emit partDecoded();
        }
    }
}

void
YencDecoder::parseHeadLine(std::string const &headline)
{
    //
    // lines
    //
    m_lineCount = atoi(getAttribute(headline, "line"));

    //
    // size
    //
    m_fileSize += atoi(getAttribute(headline, "size"));

    //
    // filename
    //
    m_filename = std::string(getName(headline));
}

void
YencDecoder::parseTailLine(std::string const &tailLine)
{

    //
    // filesize
    //
    m_fileSizeTail += atoi(getAttribute(tailLine, "size"));

    //
    // crc value for checking that file was decoded correctly
    //
    m_crc = atoi(getAttribute(tailLine, "crc32"));
}

std::string
YencDecoder::getFileName()
{
    return m_filename;
}
