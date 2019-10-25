/* This file is part of node-sword-interface.

   Copyright (C) 2019 Tobias Klein <contact@ezra-project.net>

   node-sword-interface is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   node-sword-interface is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of 
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
   See the GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with node-sword-interface. See the file COPYING.
   If not, see <http://www.gnu.org/licenses/>. */

#include <sstream>
#include <string>

#include <swmodule.h>
#include "strongs_entry.hpp"
#include "string_helper.hpp"

using namespace std;
using namespace sword;

StrongsEntry::StrongsEntry(string key, string rawEntry)
{
  this->key = key;
  this->parseFromRawEntry(rawEntry);
}

StrongsEntry* StrongsEntry::getStrongsEntry(SWModule* module, string key)
{
    if (module == 0) {
        return 0;
    }

    // Cut off the first character (H or G), since the Sword engine uses the actual number strings as the key for Strong's
    string strongsNumberString = key.substr(1);
    module->setKey(strongsNumberString.c_str());
    StrongsEntry* strongsEntry = new StrongsEntry(key, module->getRawEntry());

    return strongsEntry;
}

void StrongsEntry::parseFromRawEntry(string rawEntry)
{
    this->rawEntry = rawEntry;

    vector<string> allLines = StringHelper::split(this->rawEntry, "\n");
    string firstLine = allLines[0];

    vector<string> firstLineEntries = StringHelper::split(firstLine, "  ");
    this->transcription = firstLineEntries[1];
    this->phoneticTranscription = firstLineEntries[2];

    // Erase the first three lines, this leaves the definition
    allLines.erase(allLines.begin(), allLines.begin() + 3);
    
    vector<string> references;
    stringstream definition;

    for (unsigned int i = 0; i < allLines.size(); i++) {
      string currentLine = allLines[i];
      if (currentLine.substr(0,4) == " see") {
        StringHelper::trim(currentLine);
        references.push_back(currentLine);
      } else {
        definition << currentLine;
      }
    }

    this->references = references;
    this->definition = definition.str();
    StringHelper::trim(this->definition);
}