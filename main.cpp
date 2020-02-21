#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>


using namespace std;

#define ENCRYPT_TYPE_NAME "encrypt"
#define DECRYPT_TYPE_NAME "decrypt"
#define SIMPLE_KIND_NAME "simple"
#define COMPLICATED_KIND_NAME "complicated"
#define CIPHER_BLOCK_DELIM '\n'
#define MAX_RANDOM 11


enum methodType {Encryption, Decrytpion};
enum methodKind {Simple, Complicated};
struct Method {
    methodType type;
    methodKind kind;
};

struct IO {
    string inputFilePath;
    string outputFilePath;
};

typedef string output_t;
typedef string input_t;
typedef unsigned int seed_t;
typedef string plain_t;
typedef vector<int> cipher_t;


Method getMethod();
string getKey();
IO getIO();
output_t processOutput(const Method&, const string& key, const IO&);
string readFileContent(const string& filePath);
output_t processMethod(const input_t& input, const Method&, const string& key);
string getCipherText(const cipher_t&);
cipher_t getCipher(string cipherText);
cipher_t encrypt(const plain_t& input, const string& key, methodKind);
plain_t decrypt(const cipher_t& input, const string& key, methodKind);
cipher_t encryptSimple(const plain_t& input, const string& key);
cipher_t encryptComplicated(const plain_t& input, const string& key);
plain_t decryptSimple(const cipher_t& input, const string& key);
plain_t decryptComplicated(const cipher_t& input, const string& key);
seed_t getRandomSeed(const string& key);
vector<int> getRandomKey(const unsigned int length);
void writeOutput(const output_t&, const string& filePath);

int main() {
    Method method = getMethod();
    string key = getKey(); 
    IO io = getIO();
    output_t output = processOutput(method, key, io);
    writeOutput(output, io.outputFilePath);

    return 0;
}

Method getMethod() {
    string methodTypeName, methodKindName;
    cin >> methodTypeName >> methodKindName;
    Method method;
    method.type = methodTypeName == ENCRYPT_TYPE_NAME ? Encryption : Decrytpion;
    method.kind = methodKindName == SIMPLE_KIND_NAME ? Simple : Complicated;
    return method;
}

string getKey() {
    string key;
    cin >> key;
    return key;
}

IO getIO() {
    IO io;
    cin >> io.inputFilePath >> io.outputFilePath;
    return io;
}

output_t processOutput(const Method& method, const string& key, const IO& io) {
    input_t input = readFileContent(io.inputFilePath);
    output_t output = processMethod(input, method, key);
    return output;
}  

output_t processMethod(const input_t& input, const Method& method, const string& key) {
    cipher_t cipher;
    switch (method.type) {
    case Encryption:
        cipher = encrypt(input, key, method.kind);
        return (output_t) getCipherText(cipher);
    case Decrytpion:
        cipher = getCipher(input);
        return (output_t) decrypt(cipher, key, method.kind);
    }
}

string getCipherText(const cipher_t& cipher) {
    string cipherText = "";
    for (int cipherBlock : cipher) 
        cipherText += to_string(cipherBlock) + CIPHER_BLOCK_DELIM;
    return cipherText;
}

cipher_t getCipher(string cipherText) {
    string line;
    stringstream cipherTextStream(cipherText);
    cipher_t cipher;
    while (getline(cipherTextStream, line, CIPHER_BLOCK_DELIM))
        cipher.push_back(stoi(line));
    return cipher;
}

cipher_t encrypt(const plain_t& input, const string& key, methodKind kind) {
    switch (kind)
    {
    case Simple:
        return encryptSimple(input, key);
    case Complicated:
        return encryptComplicated(input, key);
    }
}

cipher_t encryptSimple(const plain_t& input, const string& key) {
    cipher_t cipher;
    for (size_t i = 0; i < input.length(); i++) {
        int keyIndex = i % key.length();
        int cipherBlock = int(input[i]) + int(key[keyIndex]);
        cipher.push_back(cipherBlock);
    }
    return cipher;
}

cipher_t encryptComplicated(const plain_t& input, const string& key) {
    srand(getRandomSeed(key));
    vector<int> randomKey = getRandomKey(input.length());
    cipher_t cipher;
    for (size_t i = 0; i < input.length(); i++) {
        int cipherBlock = randomKey[i] + int(input[i]);
        cipher.push_back(cipherBlock);
    }
    return cipher;
}

vector<int> getRandomKey(const unsigned int length) {
    vector<int> randomKey;
    for (size_t i = 0; i < length; i++)
        randomKey.push_back(rand() % MAX_RANDOM);
    return randomKey;
}

seed_t getRandomSeed(const string& key) {
    seed_t seed = 0;
    for (char c : key) 
        seed += uint(c);
    return seed;
}

plain_t decrypt(const cipher_t& input, const string& key, methodKind kind) {
    switch (kind) {
    case Simple:
        return decryptSimple(input, key);
    case Complicated:
        return decryptComplicated(input, key);
    }
}

plain_t decryptSimple(const cipher_t& input, const string& key) {
    plain_t plainText = "";
    for (size_t i = 0; i < input.size(); i++) {
        int keyIndex = i % key.length();
        char plainChar = char(input[i] - int(key[keyIndex]));
        plainText += plainChar;
    }
    return plainText;
}

plain_t decryptComplicated(const cipher_t& input, const string& key) {
    srand(getRandomSeed(key));
    vector<int> randomKey = getRandomKey(input.size());
    plain_t plainText = "";
    for (size_t i = 0; i < input.size(); i++) {
        char plainChar = char(input[i] - randomKey[i]);
        plainText += plainChar;
    }
    return plainText;
}

string readFileContent(const string& filePath) {
    ifstream file(filePath);
    stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

void writeOutput(const output_t& output, const string& filePath) {
    ofstream file(filePath);
    stringstream buffer(output);
    buffer >> file.rdbuf();
}