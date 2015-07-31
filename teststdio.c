// read a file into memory
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

static long readConfigDataFromString(char *str) {
    long sum = 0;

    if (str) {
        int key = -1;
        int value = -1;
        int cnt;
        const char *sep = "\n";
        char *line, *brkt;

        for (line = strtok_r(str, sep, &brkt);
                line;
                line = strtok_r(NULL, sep, &brkt)) {
            cnt = sscanf(line, "%d:%d;\n", &key, &value);
            if (cnt == 2) {
                fprintf(stdout, "%d:%d;\n", key, value);
                sum += key;
                sum += value;
            }
        }
    }

    return sum;
}

int main() {
    int key = -1;
    int value = -1;
    long sum = 0;
    int cnt;

    FILE *is;
    // TODO char * comment;

    is = fopen("data.txt", "r");
    if (is) {
        do {
            cnt = fscanf(is, "%d:%d;\n", &key, &value);
            if (cnt == 2) {
                fprintf(stdout, "%d:%d;\n", key, value);
                sum += key;
                sum += value;
            } else {
                char buf[80];
                // TODO read over non valid contents ...
                //BSD only! size_t len;
                //BSD only: comment = fgetln(is, &len);
                (void) fgets(buf, sizeof(buf), is);
                //XXX break;  //TODO format error ...
            }
        } while (!feof(is));
        fclose(is);
        assert(sum == 143);
        fprintf(stdout, "sum=%ld\n", sum);
    } else {
        perror("fopen");
    }
    puts("-----------------------------");

    // ==================================================
    // and again as row binary;
    // FIXME not useable for large files! ck
    // ==================================================
    {
        ssize_t fileSize;
        size_t bufferLen;
        char * buffer;
        is = fopen("data.txt", "rb");
        if (is) {
            // get length of file:
            (void) fseek(is, 0L, SEEK_END);
            fileSize = ftell(is);
            if (fileSize == -1) {
                perror("ftell");
                return -1;
            }
            (void) fseek(is, 0L, SEEK_SET);    // same as: rewind(is);

            // allocate memory:
            bufferLen = (size_t) fileSize; //FIXME prevent warnings
            buffer = malloc(bufferLen);
            if (buffer) {

                // read data as a block:
                //NOTE: They return the number of objects read or written
                size_t cnt = fread(buffer, 1, bufferLen , is);
                if (cnt == 0) {
                    perror("read");
                } else {
                    long sum;
                    buffer[fileSize - 1] = 0; // termiantes as string
                    (void) fwrite(buffer, 1, bufferLen, stdout);
                    puts("-----------------------------");
                    sum = readConfigDataFromString(buffer);
                    assert(sum == 143);
                    fprintf(stdout, "sum=%ld\n", sum);
                }
                fclose(is);

                free(buffer);
            }
        }
    }

    return 0;
}

// =======
// output:
// =======
// 1:1;
// 2:3;
// 5:8;
// 13:21;
// 34:55;
// sum=143
// -----------------------------
// 0x77:088;
//
// 1:1;
// 2:3;
//  5: 8;
//  13: 21;
//  089 : 0xXX ;     #XXX non vaild line!
//
//  34: 55;
//
// error:EOF
// -----------------------------
// 1:1;
// 2:3;
// 5:8;
// 13:21;
// 34:55;
// sum=143

