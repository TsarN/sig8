#include <sig8.h>
#include <stdio.h>

void update(void)
{
}

int main(int argc, char **argv)
{
    Initialize(argc, argv);
    char *s = (char *)ReadFileContents("res://main.c", NULL);
    puts(s);
    Run(update);

    return 0;
}
