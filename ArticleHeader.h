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

#pragma once

#include "CompositeMessagePart.h"
#include <QString>
#include <vector>

namespace core {

    typedef std::vector<CompositeMessagePart> CompositeMessageParts;

    struct Header;

    typedef std::vector<Header> Headers;

    typedef std::vector<Header*> HeaderPtrs;

    struct HeadersData
    {
        Headers headers;
        int status;
    };

    /**
     * @brief The Header struct stores data related to an article's header
     */
    struct Header
    {
        int messageId;
        std::string subject;
        std::string date;
        std::string author;

        std::string getSubject()
        {
            return std::string(subject.begin() + 9, subject.end());
        }

        std::string getDate()
        {
            return std::string(date.begin() + 6, date.end());
        }

        std::string getAuthor()
        {
            return std::string(author.begin() + 8, author.end());
        }

        //
        // For consecutive ordering of headers
        //
        int index;

        //
        // For display in list, what row does header reside in? (Populated in
        // ManagedGroupTab)
        //
        int row;

        //
        // If the message is composite (composed of multiple parts)
        // also store its composite messages
        //
        bool isComposite;
        CompositeMessageParts compositeMessageParts;

        bool hasBeenSelected;

        //
        // For storing where item is downloaded
        //
        QString downloadPath;
    };

}
