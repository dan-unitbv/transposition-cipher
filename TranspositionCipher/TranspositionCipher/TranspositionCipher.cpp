#include <iostream>
#include <algorithm>
#include <sstream>
#include <numeric>
#include <vector>
#include <string>

void PrintMatrix(const std::vector<std::vector<std::string>>& matrix)
{
    for (int i = 0; i < matrix.size(); i++)
    {
        for (int j = 0; j < matrix[i].size(); j++)
        {
            std::cout << matrix[i][j] << " ";
        }

        std::cout << std::endl;
    }
}

void CreateMatrix(const std::string& key, const std::string& messageToEncrypt, std::vector<std::vector<std::string>>& matrix)
{
    std::vector<std::string> keyRow;

    for (int i = 0; i < key.size(); i++)
    {
        std::string letter;

        letter.push_back(key[i]);

        keyRow.push_back(letter);
    }

    matrix.push_back(keyRow);

    std::vector<std::string> sortedKeyRow(keyRow.begin(), keyRow.end());
    std::sort(sortedKeyRow.begin(), sortedKeyRow.end());

    std::vector<int> keyIndices;

    for (int i = 0; i < key.size(); i++)
    {
        auto position = std::distance(sortedKeyRow.begin(), std::find(sortedKeyRow.begin(), sortedKeyRow.end(), std::string(1, key[i])));

        keyIndices.push_back(position + 1);
    }

    std::vector<std::string> indicesRow;

    for (int i = 0; i < keyIndices.size(); i++)
    {
        indicesRow.push_back(std::to_string(keyIndices[i]));
    }

    matrix.push_back(indicesRow);

    int nrOfCharacters = 0;

    std::vector<std::string> newRow;

    for (int i = 0; i < messageToEncrypt.length(); i++)
    {
        if (messageToEncrypt[i] != ' ')
        {
            std::string character;

            character.push_back(messageToEncrypt[i]);

            newRow.push_back(character);
        }
        else
        {
            newRow.push_back("_");
        }

        nrOfCharacters++;

        if (nrOfCharacters == matrix[0].size())
        {
            matrix.push_back(newRow);

            newRow.clear();

            nrOfCharacters = 0;
        }
    }

    std::vector<char> alphabet(26);
    std::iota(alphabet.begin(), alphabet.end(), 'a');

    int alphabetIndex = 0;

    if (nrOfCharacters != 0)
    {
        while (nrOfCharacters < matrix[0].size())
        {
            std::string character;

            character.push_back(alphabet[alphabetIndex]);

            newRow.push_back(character);

            alphabetIndex++;

            nrOfCharacters++;
        }
        matrix.push_back(newRow);
    }

    PrintMatrix(matrix);
}

void EncryptMessage(const std::vector<std::vector<std::string>>& matrix, const std::vector<int>& keyIndices, std::string& encryptedMessage)
{
    std::vector<int> sortedIndices = keyIndices;
    std::sort(sortedIndices.begin(), sortedIndices.end());

    for (int i = 0; i < sortedIndices.size(); i++)
    {
        auto position = std::distance(matrix[1].begin(), std::find(matrix[1].begin(), matrix[1].end(), std::to_string(sortedIndices[i])));

        for (int j = 2; j < matrix.size(); j++)
        {
            if (position < matrix[j].size())
            {
                encryptedMessage += matrix[j][position];
            }
        }
    }
}

int main()
{
    std::string key;

    std::cout << "Key: ";

    std::cin >> key;

    std::cout << std::endl;

    for (int i = 0; i < key.length(); i++)
    {
        if (std::count(key.begin(), key.end(), key[i]) > 1)
        {
            std::cout << "All letters of the key must be distinct! Please try again.";
            return 0;
        }
    }

    std::string messageToEncrypt;

    std::cout << "Message to encrypt: ";

    std::cin.ignore();

    std::getline(std::cin, messageToEncrypt);

    std::cout << std::endl;

    std::vector<std::vector<std::string>> matrix;

    CreateMatrix(key, messageToEncrypt, matrix);

    std::cout << std::endl;

    std::vector<int> keyIndices;

    for (int j = 0; j < matrix[1].size(); j++)
    {
        keyIndices.push_back(std::stoi(matrix[1][j]));
    }

    std::string encryptedMessage;

    EncryptMessage(matrix, keyIndices, encryptedMessage);

    std::cout << "Encrypted message: " << encryptedMessage << std::endl;

    return 0;
}