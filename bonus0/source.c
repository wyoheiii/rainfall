undefined4 main(void)

{
  char local_3a [46];

  pp(local_3a);
  puts(local_3a);
  return 0;
}

void p(char *param_1,char *param_2)

{
  char *pcVar1;
  char local_100c [4104];

  puts(param_2);
  read(0,local_100c,0x1000); //4096
  pcVar1 = strchr(local_100c,10); //10= LF 改行探す
  *pcVar1 = '\0';
  strncpy(param_1,local_100c,0x14); //20バイトコピー
  return;
}

void pp(char *param_1)

{
  char local_34[20];
    char local_20[20];

    p(local_34, " - ");
    p(local_20, " - ");


		strcpy(param_1, local_34);

    size_t len = strlen(param_1);
    param_1[len] = ' ';
    param_1[len + 1] = '\0';

    strcat(param_1, local_20);

    return;
}