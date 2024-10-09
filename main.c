#include <stdint.h>
#include <stdio.h>
#include <string.h>

int32_t is_ascii(char str[]){
    for(int i = 0; str[i] != '\0'; i++){
        if (str[i] < 0 || str[i] > 127){
            return 0;
        }
        i++;
    } 
    return 1;
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

void utf8_substring(char str[], int32_t cpi_start, int32_t cpi_end, char res[]){
    if(cpi_start > cpi_end || cpi_end < 0|| cpi_start < 0){
        return;
    }
    int bytestart = codepoint_index_to_byte_index(str, cpi_start);//calling the function to save time
    int byteend = codepoint_index_to_byte_index(str, cpi_end);
    int resindx =0;
    for(int i=bytestart; i<byteend; i++){ //looping though bytestart to byteend and putting in the characters
        res[resindx] = str[i];
        resindx++;
    }
    res[resindx] = '\0';
}

int32_t codepoint_at(char str[], int32_t cpi){
    int codepointindx = 0;
    for(int byteindx = 0; str[byteindx] != cpi;){ //using byteindex to count instead of i *edit* i forgot that its better to use cpi cause i know the value
        int charlen = width_from_start_byte(str[byteindx]);
        if (codepointindx == cpi){ //check to see if index has been reached if so then return the index of the byte
            if(width_from_start_byte(str[byteindx]) == 1) {
                
                return str[byteindx];
            }
            else if(charlen == 2){
                
                return (str[byteindx] & 0b00011111) * 64 + (str[byteindx+1] & 0b00111111);
            }
            else if(charlen == 3){
                
                return (str[byteindx] & 0b00001111) * 4096 + (str[byteindx+1] & 0b00111111) * 64 + (str[byteindx+2] & 0b00111111);
            }
            else if(charlen == 4){
                
                return (str[byteindx] & 0b00000111) * 262144 +(str[byteindx+1] & 0b00111111) * 4096 +(str[byteindx+2] & 0b00111111) * 64 +(str[byteindx+3] & 0b00111111);   
            }
            else if(charlen == -1){
                return -1;
            }
    }

        byteindx += charlen; //since there are special characters must add them
        codepointindx++; 
        
        
    }
    return -1;

    
}
char is_animal_emoji_at(char str[], int32_t cpi){
    int32_t codep = codepoint_at(str, cpi);
    int32_t rat = codepoint_at("🐀", 0); //theres probably an easier way of doing this but i prefer doing it this way
    int32_t squirrel = codepoint_at("🐿️", 0);
    int32_t crab = codepoint_at("🦀", 0);
    int32_t dog = codepoint_at("🦮", 0);

    if((codep >= rat && codep<= squirrel)||(codep >= crab && codep <= dog)){
        return 1;
    }
    return 0;
}


int main(){
    printf("Is 🔥 ASCII? %d\n", is_ascii("🔥"));

    printf("Is abcd ASCII? %d\n", is_ascii("abcd"));

    int32_t ret = 0;
    char str[] = "abcd";
    ret = capitalize_ascii(str);
    printf("Capitalized String: %s\nCharacters updated: %d\n", str, ret);

    char s[] = "Héy"; // same as { 'H', 0xstr[cpi+2], 0xA9, 'y', 0 },   é is start byte + 1 cont. byte
    printf("Width: %d bytes\n", width_from_start_byte(s[1])); // start byte 0xstr[cpi+2] indicates 2-byte sequence

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
    printf("Codepoint at %d in %s is %d\n", idx2, str1, codepoint_at(str1, idx2));
    // 'p' is the 4th codepoint  
    printf("animal?  %d %d", idx2, is_animal_emoji_at(str2, idx2)); // this is purely testing
    
}