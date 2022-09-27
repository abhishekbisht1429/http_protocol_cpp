//
// Created by abhishek on 25/9/22.
//

#include <string>
#include<vector>
#include<thread>
#include<regex>
#include<cstring>
#include "util.h"

namespace tb_util {
    std::string ltrim(const std::string &s) {
        size_t start = s.find_first_not_of(" \n\r\t\f\v");
        return (start == std::string::npos) ? "" : s.substr(start);
    }

    std::string rtrim(const std::string &s) {
        size_t end = s.find_last_not_of(" \n\r\t\f\v");
        return (end == std::string::npos) ? "" : s.substr(0, end + 1);
    }

    std::string trim(const std::string &s) {
        return rtrim(ltrim(s));
    }

    std::vector<std::string> tokenize(std::string str, std::string delim) {
        std::string temp;
        std::vector<std::string> vec;
        int i=0;
        while(i<str.size()) {
            if(str.substr(i, delim.size()) == delim) {
                vec.push_back(temp);
                temp.clear();
                i += delim.size();
            } else {
                temp += str[i];
                ++i;
            }
        }
        if(temp.size() > 0)
            vec.push_back(temp);

        return vec;
    }

    std::string join(std::string path1, std::string path2) {
        if(path2.size()>0 && path2[0] == '/')
            return path2;
        else if(path1.size()>0 && path1[path1.size()-1] == '/')
            return path1 + path2;
        else
            return path1 + "/" + path2;
    }

    std::string home = join("/home", getenv("USERNAME"));
    std::string expand_tilde(std::string path) {
        if(path.size() > 0 && path[0] == '~') {
            if(path.size()>1 && path[1] == '/')
                return join(home, path.substr(2));
            else if(path.size() == 1)
                return home;
        }
        return path;
    }

    std::vector<std::string> tokenize_command(std::string inp) {
        std::vector<std::string> res;
        std::regex reg("([^\"]\\S*|\".+?\")\\s*");
        std::smatch match;
        while(regex_search(inp, match, reg)) {
            std::string str = trim(match.str());
            if(str[0] == '"')
                str = str.substr(1, str.size()-2);

            res.push_back(str);
            inp = match.suffix().str();
        }
        for(int i=1; i<res.size(); ++i)
            res[i] = expand_tilde(res[i]);

        return res;
    }

    bytes s2b(std::string str) {
        bytes bs;
        for(int i=0; i<str.size(); ++i)
            bs += (unsigned char)str[i];
        return bs;
    }

    std::string b2s(bytes bs) {
        std::string str;
        for(int i=0; i<bs.size(); ++i)
            str += (char)bs[i];
        return str;
    }

    bytes serialize_bytes_vec(std::vector<bytes> vec) {
        bytes out;
        for(auto val : vec) {
            // store the length of val
            uint32_t len = val.length();
            for(int i=0; i<4; ++i) {
                out.push_back((unsigned char)(len & 255));
                len >>= 8;
            }
            out += val;
        }
        return out;
    }

    std::vector<bytes> deserialize_bytes_vec(bytes inp) {
        std::vector<bytes> out;
        int i=0;
        while(i < inp.length()) {
            uint32_t len = 0;
            for(int j=0; j<4; ++j) {
                uint32_t temp = inp[i+j];
                len |= temp << (j*8);
            }
            i += 4;

            out.push_back(inp.substr(i, len));
            i += len;
        }
        return out;
    }

    void output_bytes(std::ostream &os, bytes &b) {
        for(auto val : b) {
            os << (int)val<<" ";
        }
        os << std::endl;
    }
};

//int main() {
//    std::vector<tb_util::bytes> vec;
//    tb_util::bytes b1, b2, b3;
//    for(int i=0; i<10; ++i) {
//        b1 += (unsigned char)i;
//        b2 += (unsigned char)(2 * i);
//        b3 += (unsigned char)(3 * i);
//    }
//    vec.push_back(b1);
//    vec.push_back(b2);
//    vec.push_back(b3);
//
//    auto sz = tb_util::serialize_bytes_vec(vec);
//    std::cout<<sz.length()<<"\n";
//
//    auto dsz = tb_util::deserialize_bytes_vec(sz);
//    for(auto b : vec) {
//        for(unsigned char val : b)
//            std::cout<<(int)val<<" ";
//        std::cout<<"\n";
//    }
//}