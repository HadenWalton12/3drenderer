#pragma once



//Pointers? - Object that holds memory address to data at given location.

//Malloc - Dynamically allocate number of bytes in heap
//Calloc - Dyanmically allocates number of bytes to heap, also sets allocated memory to zero
//sizeOF - C Language operator


//ColourBuffer - Type of pixel buffer that stores pixel color data. Must create a data structure to represent and 
//store this data. Data structure will be an array of elements
//NOTE - COLOR AND FRAME BUFFER REFER TO SAME THING
//uint32_t -Unsigned int data type, provides exactly 4 bytes

//idef & ifndef - Preprocessor directives ised to check existince of blocks of code, skipping or adding code to final compliation
//Can be used when defining platform specfic code, debugging ect...  
/*

typedef struct

typedef - Keyword that lets you create alias for existing type, makes declarations easier to read and maintain.

I.E typedef int number;
Create alias for int called number, making both declarations the same type.

int num1;
number num2;

For vec2_t vec1 = {1.0, 1.0};

is same as:
float vec1_point1 = 1.0;
float vec1_point2 = 2.0;

Structs - group related variables in same place
Structs can contain other structs
Struct initalisers list are great to set initial values

*/