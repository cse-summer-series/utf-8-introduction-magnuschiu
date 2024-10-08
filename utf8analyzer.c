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
    printf("Enter a UTF-8 encoded string: ");
    char sentence[50]; //50 byte limit seems reasonable
    fgets(sentence, 50, stdin);

    //need this placeholder because capitalize directly changes the sentence
    char changing[50];
    int counter =0;
    for(int i =0; sentence[i] != '\0'; i++){
        changing[i] = sentence[i];
        counter++;
    }
    changing[counter-1] = '\0'; //i think this has to happen because it cant put in a null byte itself(-1 cause starts at 1 and want to start at 0)

    printf("Valid ASCII: %s\n", is_ascii(sentence)? "true":"false"); //found this online
    int32_t ret = capitalize_ascii(changing);
    printf("Uppercased ASCII: %s\n", changing);
    printf("Length in bytes: %d\n", codepoint_index_to_byte_index(sentence, utf8_strlen(sentence))); //almost tricked me! 
    printf("Number of Codepoints: %d\n", utf8_strlen(sentence));
    printf("Bytes per code point: ");
    char result[6]; 
    utf8_substring(sentence, 0, 6, result);
    printf("Substring of the first 6 code points: \"%s\"\n", result);
    printf("Code points as decimal numbers: ");
    printf("Animal emojis: ");

}