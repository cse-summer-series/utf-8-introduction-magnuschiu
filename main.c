#include <stdint.h>
#include <stdio.h>
#include <string.h>

int32_t is_ascii(char str[]){
    for(int i = 0; str[i] != '\0'; i++){
        return str[i] <= 127 && str[i]>0; //ascii must have more than 0 and less than 127 bytes
    } 
    
}
int count =0;
int32_t capitalize_ascii(char str[]){
    for(int i = 0; str[i] != '\0'; i++){
        if(str[i] >= 97 && str[i]<=122){//lowercase are ascii 97 to 122
            str[i] = str[i] -32; //lower to uppercase 
            count++; //this is ret, we already update the values of str in the previous line
        }
    }
    return count;
}
int32_t width_from_start_byte(char start_byte){
    
    if((start_byte & 0b11000000) == 0b10000000){
        return -1; //not ascii/continutation starts with 0b10
    }
    else if((start_byte & 0b10000000) != 0b10000000){
        return 1; //ascii never starts with 0b1
    }
    else if((start_byte & 0b11100000) == 0b11000000){
        return 2;//two bytes 0b110
    }
    else if((start_byte & 0b11110000) == 0b11100000){
        return 3;//three bytes 0b1110
    }
    else if((start_byte & 0b11111000) == 0b11110000){
        return 4; // four bytes 0b11110
    }
    
}

int32_t utf8_strlen(char str[]){
    int count = 0;
    for(int i = 0; str[i] != '\0'; i++){
        count = count + width_from_start_byte(str[i]); //adds the -1 for two bit chars and etc
    }
    return count;
}

int32_t codepoint_index_to_byte_index(char str[], int32_t cpi){
    int codepointindx = 0;
    for(int byteindx = 0; str[byteindx] != cpi;){ //using byteindex to count instead of i *edit* i forgot that its better to use cpi cause i know the value
        int charlen = width_from_start_byte(str[byteindx]);
        if (codepointindx == cpi){ //check to see if index has been reached if so then return the index of the byte
            return byteindx;
        }
        byteindx += charlen; //since there are special characters must add them
        codepointindx++; 
        
        
    }
    return -1;
}

void utf8_substring(char str[], int32_t cpi_start, int32_t cpi_end, char result[]){
    if(cpi_start > cpi_end || cpi_end < 0|| cpi_start < 0){
        return;
    }
    int bytestart = codepoint_index_to_byte_index(str, cpi_start);//calling the function to save time
    int byteend = codepoint_index_to_byte_index(str, cpi_end);
    int resindx =0;
    int i = bytestart; //have to declare this i think to ensure str starts at bytestart
    for(i; i<byteend; i++){ //looping though bytestart to byteend and putting in the characters
        result[resindx] = str[i];
        resindx++;
    }
    result[resindx] = '\0';
}

int32_t codepoint_at(char str[], int32_t cpi){
    return str[codepoint_index_to_byte_index(str, cpi)]; //can just call the codepoint_index_to_byte_index and get the value from str
}


int main(){
    printf("Is 🔥 ASCII? %d\n", is_ascii("🔥"));

    printf("Is abcd ASCII? %d\n", is_ascii("abcd"));

    int32_t ret = 0;
    char str[] = "abcd";
    ret = capitalize_ascii(str);
    printf("Capitalized String: %s\nCharacters updated: %d\n", str, ret);

    char s[] = "Héy"; // same as { 'H', 0xC3, 0xA9, 'y', 0 },   é is start byte + 1 cont. byte
    printf("Width: %d bytes\n", width_from_start_byte(s[1])); // start byte 0xC3 indicates 2-byte sequence

    printf("Width: %d bytes\n", width_from_start_byte(s[2])); // start byte 0xA9 is a continuation byte, not a start byte

    char str1[] = "Joséph";
    printf("Length of string %s is %d\n", str1, utf8_strlen(str1));  // 6 codepoints, (even though 7 bytes)


    int32_t idx = 4;
    printf("Codepoint index %d is byte index %d\n", idx, codepoint_index_to_byte_index("Joséph", idx));

    char result[17];
    char str2[] = "🦀🦮🦮🦀🦀🦮🦮";
    utf8_substring(str2, 3, 7, result);
    printf("String: %s\nSubstring: %s\n", str2, result); // these emoji are 4 bytes long

    
    int32_t idx2 = 4;
    printf("Codepoint at %d in %s is %d\n", idx2, str1, codepoint_at(str1, idx2)); // 'p' is the 4th codepoint  
}