#define _CRT_SECURE_NO_WARNINGS
#include <iostream>

using namespace std;

extern "C"
{
    void WriteInt(int val)
    {
        cout << val;
    }
    void WriteLineInt(int val)
    {
        cout << val << endl;
    }
    void WriteStr(char* val)
    {
        cout << val;
    }
    void WriteLineStr(char* val)
    {
        cout << val << endl;
    }
    void WriteBool(bool val)
    {
        cout.setf(ios::boolalpha);
        cout << val;
        cout.unsetf(ios::boolalpha);
    }
    void WriteLineBool(bool val)
    {
        cout.setf(ios::boolalpha);
        cout << val << endl;
        cout.unsetf(ios::boolalpha);
    }
    const char* dateCAY()
    {
        time_t now = time(nullptr);
        tm* localTime = localtime(&now);
        if (!localTime)
            return "Invalid Date";

        char buffer[11];
        strftime(buffer, sizeof(buffer), "%d.%m.%Y", localTime);

        return buffer;
    }
    int randCAY()
    {
        static unsigned long long seed = time(nullptr);
        seed = (6364136223846793005ULL * seed + 1);
        return static_cast<int>(seed % RAND_MAX);
    }
    bool isEmptyCAY(const char* val)
    {
        int len = strlen(val);
        for (int i = 0; i < strlen(val); ++i)
        {
            if (val[i] == ' ')
                len--;
        }
        return len == 0;
    }
    bool isEvenCAY(int val)
    {
        return val % 2 == 0;
    }
    int lenCAY(const char* val)
    {
        int len = strlen(val);
        for (int i = 0; i < strlen(val); ++i)
        {
            if (val[i] == ' ')
                len--;
            else
                break;
        }
        return len;
    }
    bool compareCAY(const char* val1, const char* val2, int operation)
    {
        if (!val1 || !val2) return false;
        switch (operation)
        {
        case 1:
            return strlen(val1) > strlen(val2);
            break;
        case 2:
            return strlen(val1) < strlen(val2);
            break;
        case 3:
            return strlen(val1) >= strlen(val2);
            break;
        case 4:
            return strlen(val1) <= strlen(val2);
            break;
        case 5:
            return strlen(val1) == strlen(val2);
            break;
        case 6:
            return strlen(val1) != strlen(val2);
            break;
        default:
            break;
        }
    }
}