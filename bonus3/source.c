int main(int argc, char *argv[]) {
  FILE *password_file;
  unsigned int buffer1[16];
  char buffer2[66];
  char temp_byte;
  int comparison_result;
  unsigned int *buffer_ptr;

  password_file = fopen("/home/user/end/.pass", "r");

  // ファイルが開けなかった場合または引数の数が2でない場合はエラーを返す
  if (password_file == NULL || argc != 2) {
      return -1;
  } else {
      // パスワードファイルからデータを読み込む
      fread(buffer1, 1, 66, password_file);

      // buffer1 の指定された位置にあるバイトをゼロに設定
      ((char *)buffer1)[atoi(argv[1])] = 0;

      // 追加のデータを buffer2 に読み込む
      fread(buffer2, 1, 65, password_file);

      // ファイルを閉じる
      fclose(password_file);

      if (strcmp((char *)buffer1, argv[1]) == 0) {
          // パスワードが一致した場合、シェルを起動
          execl("/bin/sh", "sh", NULL);
      } else {
          // パスワードが一致しない場合、buffer2 の内容を表示
          puts(buffer2);
      }

      return 0;
  }
}
