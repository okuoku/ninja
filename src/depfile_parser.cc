/* Generated by re2c 0.13.5 on Wed Aug 22 08:19:03 2012 */
#line 1 "/Users/clausklein/Downloads/ninja/src/depfile_parser.in.cc"
// Copyright 2011 Google Inc. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "depfile_parser.h"

// A note on backslashes in Makefiles, from reading the docs:
// Backslash-newline is the line continuation character.
// Backslash-# escapes a # (otherwise meaningful as a comment start).
// Backslash-% escapes a % (otherwise meaningful as a special).
// Finally, quoting the GNU manual, "Backslashes that are not in danger
// of quoting '%' characters go unmolested."
// How do you end a line with a backslash?  The netbsd Make docs suggest
// reading the result of a shell command echoing a backslash!
// see http://www.gnu.org/software/make/manual/html_node/Static-Usage.html
// Rather than implement all of above, we do a simpler thing here:
// Backslashes escape a set of characters (see "escapes" defined below),
// otherwise they are passed through verbatim.
// If anyone actually has depfiles that rely on the more complicated
// behavior we can adjust this.
bool DepfileParser::Parse(string* content, string* err) {
  // in: current parser input point.
  // end: end of input.
  // parsing_targets: whether we are parsing targets or dependencies.
  char* in = &(*content)[0];
  char* end = in + content->size();
  bool parsing_targets = true;
  while (in < end) {
    // out: current output point (typically same as in, but can fall behind
    // as we de-escape backslashes).
    char* out = in;
    // filename: start of the current parsed filename.
    char* filename = out;
    for (;;) {
      // start: beginning of the current parsed span.
      const char* start = in;
      
#line 51 "/Users/clausklein/Downloads/ninja/src/depfile_parser.cc"
    {
      char yych;

      yych = *in;
      switch (yych) {
      case 0x00:  goto yy6;
      case '(':
      case ')':
      case '+':
      case ',':
      case '-':
      case '.':
      case '/':
      case '0':
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':
      case ':':
      case '=':
      case '@':
      case 'A':
      case 'B':
      case 'C':
      case 'D':
      case 'E':
      case 'F':
      case 'G':
      case 'H':
      case 'I':
      case 'J':
      case 'K':
      case 'L':
      case 'M':
      case 'N':
      case 'O':
      case 'P':
      case 'Q':
      case 'R':
      case 'S':
      case 'T':
      case 'U':
      case 'V':
      case 'W':
      case 'X':
      case 'Y':
      case 'Z':
      case '_':
      case 'a':
      case 'b':
      case 'c':
      case 'd':
      case 'e':
      case 'f':
      case 'g':
      case 'h':
      case 'i':
      case 'j':
      case 'k':
      case 'l':
      case 'm':
      case 'n':
      case 'o':
      case 'p':
      case 'q':
      case 'r':
      case 's':
      case 't':
      case 'u':
      case 'v':
      case 'w':
      case 'x':
      case 'y':
      case 'z':
      case '~':  goto yy4;
      case '\\':  goto yy2;
      default:  goto yy8;
      }
yy2:
      ++in;
      switch ((yych = *in)) {
      case 0x00:
      case '\n':  goto yy3;
      case ' ':
      case '#':
      case '$':
      case '*':
      case '[':
      case '\\':
      case '|':  goto yy13;
      default:  goto yy11;
      }
yy3:
#line 83 "/Users/clausklein/Downloads/ninja/src/depfile_parser.in.cc"
      {
        // For any other character (e.g. whitespace), swallow it here,
        // allowing the outer logic to loop around again.
        break;
      }
#line 156 "/Users/clausklein/Downloads/ninja/src/depfile_parser.cc"
yy4:
      ++in;
      yych = *in;
      goto yy10;
yy5:
#line 71 "/Users/clausklein/Downloads/ninja/src/depfile_parser.in.cc"
      {
        // Got a span of plain text.
        int len = (int)(in - start);
        // Need to shift it over if we're overwriting backslashes.
        if (out < start)
          memmove(out, start, len);
        out += len;
        continue;
      }
#line 172 "/Users/clausklein/Downloads/ninja/src/depfile_parser.cc"
yy6:
      ++in;
#line 80 "/Users/clausklein/Downloads/ninja/src/depfile_parser.in.cc"
      {
        break;
      }
#line 179 "/Users/clausklein/Downloads/ninja/src/depfile_parser.cc"
yy8:
      yych = *++in;
      goto yy3;
yy9:
      ++in;
      yych = *in;
yy10:
      switch (yych) {
      case '(':
      case ')':
      case '+':
      case ',':
      case '-':
      case '.':
      case '/':
      case '0':
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':
      case ':':
      case '=':
      case '@':
      case 'A':
      case 'B':
      case 'C':
      case 'D':
      case 'E':
      case 'F':
      case 'G':
      case 'H':
      case 'I':
      case 'J':
      case 'K':
      case 'L':
      case 'M':
      case 'N':
      case 'O':
      case 'P':
      case 'Q':
      case 'R':
      case 'S':
      case 'T':
      case 'U':
      case 'V':
      case 'W':
      case 'X':
      case 'Y':
      case 'Z':
      case '_':
      case 'a':
      case 'b':
      case 'c':
      case 'd':
      case 'e':
      case 'f':
      case 'g':
      case 'h':
      case 'i':
      case 'j':
      case 'k':
      case 'l':
      case 'm':
      case 'n':
      case 'o':
      case 'p':
      case 'q':
      case 'r':
      case 's':
      case 't':
      case 'u':
      case 'v':
      case 'w':
      case 'x':
      case 'y':
      case 'z':
      case '~':  goto yy9;
      default:  goto yy5;
      }
yy11:
      ++in;
#line 65 "/Users/clausklein/Downloads/ninja/src/depfile_parser.in.cc"
      {
        // Let backslash before other characters through verbatim.
        *out++ = '\\';
        *out++ = yych;
        continue;
      }
#line 273 "/Users/clausklein/Downloads/ninja/src/depfile_parser.cc"
yy13:
      ++in;
#line 60 "/Users/clausklein/Downloads/ninja/src/depfile_parser.in.cc"
      {
        // De-escape backslashed character.
        *out++ = yych;
        continue;
      }
#line 282 "/Users/clausklein/Downloads/ninja/src/depfile_parser.cc"
    }
#line 88 "/Users/clausklein/Downloads/ninja/src/depfile_parser.in.cc"

    }

    int len = (int)(out - filename);
    const bool is_target = parsing_targets;
    if (len > 0 && filename[len - 1] == ':') {
      len--;  // Strip off trailing colon, if any.
      parsing_targets = false;
    }

    if (len == 0)
      continue;

    if (!is_target) {
      ins_.push_back(StringPiece(filename, len));
    } else if (!out_.str_) {
      out_ = StringPiece(filename, len);
    } else if (out_ != StringPiece(filename, len)) {
      *err = "depfile has multiple output paths.";
      return false;
    }
  }
  return true;
}
