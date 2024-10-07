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
}