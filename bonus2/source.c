
//リファクタしたコード
int language = 0;

void greetuser(char *username) {
    char greeting[72];

    if (language == 1) {  // Finnish
        strcpy(greeting, "Hyvää päivää ");
    } else if (language == 2) {  // Dutch
        strcpy(greeting, "Goedemiddag! ");
    } else {  // Default to English
        strcpy(greeting, "Hello ");
    }

    strcat(greeting, username);
    puts(greeting);
}

int main(int argc, char *argv[]) {
    char bufferA[40];   // Buffer to store username
    char bufferB[32];  // Additional message (not used further)
    char *lang_env;

    if (argc == 3) {
        memset(bufferA, 0, sizeof(bufferA));

        strncpy(bufferA, argv[1], 40);   // Copy up to 40 bytes of username
        strncpy(bufferB, argv[2], 32);  // Copy up to 32 bytes of message

        lang_env = getenv("LANG");
        if (lang_env != NULL) {
            if (memcmp(lang_env, "fi", 2) == 0) {
                language = 1;  // Finnish
            } else if (memcmp(lang_env, "nl", 2) == 0) {
                language = 2;  // Dutch
            }
        } else {
            language = 0;
        }

        greetuser(bufferA);
    } else {
        return 1;
    }
    return 0;
}
