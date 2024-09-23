#ifndef RSA_H
#define RSA_H

#include <string>

using std::string;

class rsa{

    static string encode(string message);

    static string decode(string code);

};

#endif // RSA_H
