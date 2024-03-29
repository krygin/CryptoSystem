#include <iostream>
#include <fstream>
#include <string>
#include <vector>


int main(int argc, const char *argv[]) {

    std::string message = "qwertyo";

    const char *data = message.data();
    int size = message.size();

    std::vector<char> bytes(size);
    for (int i = 0; i < size; i++) {
        bytes[i] = data[i];
    }
    unsigned int key = 4;
    unsigned int leftShift = 3;
    srand(key);

    std::ofstream outputFile;
    outputFile.open("test", std::ios::binary);
    for (int i = 0; i < bytes.size(); i += 2) {
        unsigned int gamma = rand();
        unsigned char b1 = bytes[i];
        unsigned char b2 = i + 1 < bytes.size() ? bytes[i + 1] : 0u;

        unsigned int message_xored_with_gamma =
                ((static_cast<unsigned int>(b1) << 8u) | static_cast<unsigned int>(b2)) ^gamma;
        unsigned int encrypted_block =
                (message_xored_with_gamma << leftShift) | (message_xored_with_gamma & 0xFFFF) >> (16 - leftShift);

        char r1 = encrypted_block >> 8;
        char r2 = encrypted_block;
        outputFile.write(&r1, sizeof(r1));
        outputFile.write(&r2, sizeof(r2));
    }
    outputFile.close();



    srand(key);
    std::ifstream readFile;
    readFile.open("test", std::ios::binary);

    std::vector<char> encryptedData((std::istreambuf_iterator<char>(readFile)),
                                    std::istreambuf_iterator<char>());
    readFile.close();

    std::vector<char> decryptedData(encryptedData.size());

    for (int i = 0; i < encryptedData.size(); i+=2) {
        unsigned int gamma = rand();
        unsigned char b1 = encryptedData[i];
        unsigned char b2 = i + 1 < encryptedData.size() ? encryptedData[i + 1] : 0u;

        unsigned int encrypted_block =
                ((static_cast<unsigned int>(b1) << 8u) | static_cast<unsigned int>(b2));

        unsigned int shifted_encrypted_block =
                ((encrypted_block & 0xFFFF) >> leftShift) | encrypted_block << (16 - leftShift);

        unsigned int result =
                shifted_encrypted_block ^gamma;

        unsigned char r1 = result >> 8;
        unsigned char r2 = result;
        decryptedData[i] = r1;
        decryptedData[i+1] = r2;
    }

    for (unsigned int i = 0; i < decryptedData.size(); i++) {
        std::cout << decryptedData[i];
    }

    return 0;
}