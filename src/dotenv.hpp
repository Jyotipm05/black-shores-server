// Copyright (c) 2018 Heikki Johannes Hildén <hildenjohannes@gmail.com>
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//
//     * Redistributions in binary form must reproduce the above
//       copyright notice, this list of conditions and the following
//       disclaimer in the documentation and/or other materials provided
//       with the distribution.
//
//     * Neither the name of copyright holder nor the names of other
//       contributors may be used to endorse or promote products derived
//       from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#pragma once
#include <string>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <functional>
#include <cctype>

class dotenv
{
public:
    dotenv() = delete;
    ~dotenv() = delete;
    static const unsigned char Preserve = 1 << 0;
    static const int OptionsNone = 0;
    static void init(const char* filename = ".env");
    static void init(int flags, const char* filename = ".env");
    static std::string getenv(const char* name, const std::string& def = "");
private:
    static void do_init(int flags, const char* filename);
    static std::string strip_quotes(const std::string& str);
    static std::pair<std::string,bool> resolve_vars(size_t iline, const std::string& str);
    static void  ltrim(std::string& s);
    static void  rtrim(std::string& s);
    static void  trim(std::string& s);
    static std::string trim_copy(std::string s);
    static size_t find_var_start(const std::string& str, size_t pos, std::string& start_tag);
    static size_t find_var_end(const std::string& str, size_t pos, const std::string& start_tag);
};

inline void dotenv::init(const char* filename)
{
    dotenv::do_init(OptionsNone, filename);
}
inline void dotenv::init(int flags, const char* filename)
{
    dotenv::do_init(flags, filename);
}
inline std::string dotenv::getenv(const char* name, const std::string& def)
{
    const char* str = std::getenv(name);
    return str ? std::string(str) : def;
}
#if defined(_MSC_VER) || defined(__MINGW32__)
inline int setenv(const char *name, const char *value, int overwrite)
{
    int errcode = 0;
    if (!overwrite)
    {
        size_t envsize = 0;
        errcode = getenv_s(&envsize, NULL, 0, name);
        if (errcode || envsize) return errcode;
    }
    return _putenv_s(name, value);
}
#endif // _MSC_VER
inline size_t dotenv::find_var_start(const std::string& str, size_t pos, std::string& start_tag)
{
   size_t p1      = str.find('$',pos);
   size_t p2      = str.find("${",pos);
   size_t pos_var = (std::min)(p1,p2);
   if(pos_var != std::string::npos) start_tag = (pos_var == p2)? "${":"$";
   return pos_var;
}
inline size_t dotenv::find_var_end(const std::string& str, size_t pos, const std::string& start_tag)
{
   char end_tag    = (start_tag == "${")? '}':' ';
   size_t pos_end  = str.find(end_tag,pos);
   if(pos_end == std::string::npos && end_tag==' ') pos_end = str.length();
   return pos_end;
}
inline void dotenv::ltrim(std::string& s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int c) {return !std::isspace(c); }));
}
inline void dotenv::rtrim(std::string& s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](int c) {return !std::isspace(c); }).base(), s.end());
}
inline void dotenv::trim(std::string& s) {
    ltrim(s);
    rtrim(s);
}
inline std::string dotenv::trim_copy(std::string s) {
    trim(s);
    return s;
}
inline std::pair<std::string, bool> dotenv::resolve_vars(size_t iline, const std::string& str)
{
   std::string resolved;
   size_t pos = 0;
   size_t pre_pos = pos;
   size_t nvar = 0;
   bool finished=false;
   while(!finished)
   {
      std::string start_tag;
      pos = find_var_start(str,pos,start_tag);
      if(pos != std::string::npos)
      {
         nvar++;
         size_t pos_start = pos;
         size_t lstart = start_tag.length();
         size_t lend   = (lstart>1)? 1 : 0;
         resolved += str.substr(pre_pos,pos-pre_pos);
         pos = find_var_end(str,pos,start_tag);
         if(pos != std::string::npos)
         {
            std::string var = str.substr(pos_start,pos-pos_start+1);
            std::string env_var = var.substr(lstart,var.length()-lstart-lend);
            rtrim(env_var);
            if(const char* env_str = std::getenv(env_var.c_str()))
            {
               resolved += env_str;
               nvar--;
            }
            else
            {
               std::cout << "dotenv: Variable " << var << " is not defined on line " << iline << std::endl;
            }
            pre_pos = pos+lend;
         }
      }
      else {
         finished = true;
      }
   }
   if(pre_pos < str.length())
   {
      resolved += str.substr(pre_pos);
   }
   return std::make_pair(resolved,(nvar==0));
}
inline void dotenv::do_init(int flags, const char* filename)
{
    std::ifstream file;
    std::string line;
    file.open(filename);
    if (file)
    {
        unsigned int i = 1;
        while (getline(file, line))
        {
            const auto len = line.length();
            if (len == 0 || line[0] == '#') {
                continue;
            }
            const auto pos = line.find("=");
            if (pos == std::string::npos) {
                std::cout << "dotenv: Ignoring ill-formed assignment on line "
                          << i << ": '" << line << "'" << std::endl;
            } else {
                auto name = trim_copy(line.substr(0, pos));
                auto line_stripped = strip_quotes(trim_copy(line.substr(pos + 1)));
                auto p = resolve_vars(i,line_stripped);
                bool ok = p.second;
                if(!ok) {
                   std::cout << "dotenv: Ignoring ill-formed assignment on line "
                   << i << ": '" << line << "'" << std::endl;
                }
                else {
                   const auto& val = p.first;
                   setenv(name.c_str(), val.c_str(), ~flags & dotenv::Preserve);
                }
            }
            ++i;
        }
    }
}
inline std::string dotenv::strip_quotes(const std::string& str)
{
    const std::size_t len = str.length();
    if (len < 2)
        return str;
    const char first = str[0];
    const char last = str[len - 1];
    if (first == last && ('"' == first || '\'' == first))
        return str.substr(1, len - 2);

    return str;
}