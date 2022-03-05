#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CIPHER_KEY 3

void main()
{
    char fileName[20], ch;
    FILE *fpTxtOriginal, *fpTxtEncrypt, *fpBinDecrypt;
    FILE *fpBin, *fpBinData;
    char strTemp[100] = "", strOriginal[100] = "";
    char strEncrypted[100] = "", strDecrypted[100] = "";
    char charPlain, charCipher;
    char strBin[100] = "";
    float floatBin = 0.0;
    unsigned char dataBin[300] = {0x00};
    int i, length, ret;

    printf("\nEnter the name of file to encrypt: ");
    scanf("%s", fileName);

    printf("Opening original file %s in text mode\n", fileName);
    fpTxtOriginal = fopen(fileName, "r"); // text mode
    if (fpTxtOriginal == NULL)
    {
        printf("File does not exist or error in opening the file %s\n", fileName);
        exit(1);
    }

    printf("Opening file %s in text mode to write encrypted content\n",
           "Lab5_EncryptedText.txt");
    fpTxtEncrypt = fopen("Lab5_EncryptedText.txt", "w+"); // text mode
    if (fpTxtEncrypt == NULL)
    {
        printf("Error in creating %s\n", "Lab5_EncryptedText.txt");
        fclose(fpTxtOriginal);
        exit(2);
    }
    while (1)
    {
        ch = fgetc(fpTxtOriginal);
        if (ch == EOF)
        {
            break;
        }
        else
        {
            // Perform Caesar Cipher substitution encryption
            if (ch != ' ')
            {
                if (ch >= 'A' && ch <= 'Z')
                {
                    ch += CIPHER_KEY;
                    if (ch > 'Z')
                        ch = ch - 'Z' + 'A' - 1;
                }
            }
            fputc(ch, fpTxtEncrypt);
        }
    }

    ret = fseek(fpTxtOriginal, 0, SEEK_SET); // Set position to the beginning of the file
    while (fgets(strTemp, sizeof(strTemp), fpTxtOriginal))
    {
        strcat(strOriginal, strTemp);
    }
    printf("\nOriginal content: %s\n", strOriginal);
    printf("Encrypted content written into text mode file %s\n", "Lab5_EncryptedText.txt");

    printf("Closing original text mode file %s\n", fileName);
    printf("Closing encrypted text mode file %s\n", "Lab5_EncryptedText.txt");

    fclose(fpTxtOriginal);
    fclose(fpTxtEncrypt);

    printf("Opening encrypted text mode file %s\n", "Lab5_EncryptedText.txt");
    fpTxtEncrypt = fopen("Lab5_EncryptedText.txt", "r"); // text mode
    if (fpTxtEncrypt == NULL)
    {
        printf(" File does not exist or error in opening %s\n", "Lab5_EncryptedText.txt");
        fclose(fpTxtOriginal);
        exit(4);
    }

    printf("\nEncrypted content: ");
    if (fgets(strEncrypted, 100, fpTxtEncrypt) != NULL)
    {
        puts(strEncrypted); // write content to stdout
    }

    printf("Closing encrypted text mode file %s\n", "Lab5_EncryptedText.txt");
    fclose(fpTxtEncrypt);

    // Decrypt
    length = strlen(strEncrypted);
    for (i = 0; i < length; i++)
    {
        ch = strEncrypted[i];

        // Perform Caesar Cipher substitution decryption
        if (ch != ' ')
        {
            if (ch >= 'A' && ch <= 'Z')
            {
                ch -= CIPHER_KEY;
                if (ch < 'A')
                    ch = ch - 'A' + 'Z' + 1;
            }
        }
        strncat(strDecrypted, &ch, 1);
    }
    printf("\nDecrypted content: %s\n", strDecrypted);

    printf("Opening decrypted bin mode file %s to write decrypted content\n",
           "Lab5_DecryptedBin.txt");
    fpBinDecrypt = fopen("Lab5_DecryptedBin.txt", "wb+"); // binary mode
    if (fpBinDecrypt == NULL)
    {
        printf("Error in creating file %s\n", "Lab5_DecryptedBin.txt");
        fclose(fpBinDecrypt);
        exit(5);
    }

    printf("Write different data types into bin mode data file, %s\n", "Lab5_BinData.bin");
    fpBin = fopen("Lab5_BinData.bin", "wb");
    if (fpBin == NULL)
    {
        printf("Error in creating file %s\n", "Lab5_BinData.txt");
        fclose(fpBin);
        exit(6);
    }
    // add ascii data: "THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG"
    ret = fputs(strDecrypted, fpBin);

    // add float data: 1.234
    float f = 1.234;
    fwrite(&f, 1, sizeof(f), fpBin);

    // add binary data: 0x00 to 0xFF
    for (i = 0x00; i <= 0xFF; i++)
    {
        ch = i;
        ret = fputc(ch, fpBin);
    }
    printf("Closing bin mode data file, %s\n", "Lab5_BinData.bin");
    fclose(fpBin);

    printf("\nOpening bin mode data file, %s\n", "Lab5_BinData.bin");
    fpBinData = fopen("Lab5_BinData.bin", "rb"); // binary mode
    if (fpBinData == NULL)
    {
        printf("Error in creating file, %s\n", "Lab5_BinData.bin");
        fclose(fpBinData);
        exit(6);
    }

    printf("\nRead string from bin mode file, %s: ", "Lab5_BinData.bin");
    if (fgets(strBin, 44, fpBinData) != NULL)
    {
        printf("<%s>", strBin);
    }
    printf("\n"); // newline

    printf("\nRead float from bin mode file, %s: ", "Lab5_BinData.bin");
    fread(&floatBin, 1, sizeof(floatBin), fpBinData);
    printf("<%f>", floatBin);

    printf("\n"); // newline
    printf("\nRead binary data from bin mode file, %s: ", "Lab5_BinData.bin");
    if (fread(dataBin, sizeof(dataBin), 1, fpBinData) < 300)
    {
        for (i = 0; i < 256; i++)
        {
            if ((i % 16) == 0)
                printf("\n"); // newline
            printf("[%02X]", dataBin[i]);
        }
    }

    printf("\nClosing bin mode data file, %s\n\n", "Lab5_BinData.bin");
    fclose(fpBinData);
}