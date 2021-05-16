#include <iostream>
#include <cstring>
#include <string>

using namespace std;

struct String {
    char* cstr;
    int length;
};

String new_string(char const *cstr) {
    String str;
    str.length = strlen(cstr);
    str.cstr = new char[2 * str.length + 1];
    strcpy(str.cstr, cstr);
    return str;
}

String new_string() {
    String x;
    x.length = 0;
    *(x.cstr) = 0;
    return x;
}

String new_string(String const &src) {
    String str;
    str.length = src.length;
    str.cstr = new char[2 * str.length + 1];
    strcpy(str.cstr, src.cstr);
    return str;
}

void delete_string(String &str) {
    delete[] (str.cstr);
    str.length = 0;
}

unsigned length(String const &str) {
    return str.length;
}

String& copy(String &dst, String const &src) {
    dst.length = src.length;
    delete[] dst.cstr;
    dst.cstr = new char[2 * dst.length + 1];
    strcpy(dst.cstr, src.cstr);
    return dst;
}

char get_char_at(String const &str, unsigned pos) {
    return str.cstr[pos];
}

String& set_char_at(String &str, unsigned pos, char c) {
    str.cstr[pos] = c;
    return str;
}

String& append(String &dst, char const *cstr) {
    cstr = &(dst.cstr[strlen(cstr)]);

    String str;
    str.length = dst.length + strlen(cstr);
    str.cstr = new char[4 * str.length + 1];
    for (int i = 0; i < str.length; i++) {
        if (i < dst.length) {
            str.cstr[i] = dst.cstr[i];
        } else {
            str.cstr[i] = cstr[i];
        }
    }
    strcpy(dst.cstr, str.cstr);
    dst.length = str.length;
    return dst;
}

String& append(String &dst, String const &src){
    return append(dst, src.cstr);
}

void print(String const &str){
    cout << str.cstr;
}

int main() {

    return 0;
}