#include"loading.h"

loading::loading()
{
    ;
}

void loading::load()
{
    /*****    deal with file I/O    *****/
    char input1_name[] = "iimage.bin", input2_name[] = "dimage.bin";
    fstream input1, input2;

    /*fstream test;
    test.open( "test1.out", ios::out | ios::binary);*/

    input1.open( input1_name, ios::in | ios::binary );
        /****  when encounter file error  *****/
    if( !input1 ){
        cerr << "Can't open iimage!\n";
        exit(EXIT_FAILURE);
    }
    input2.open(input2_name, ios::in | ios::binary );
        /*******  when encounter file error  *****/
    if( !input2 ){
        cerr << "Can't open dimage!\n";
        exit(EXIT_FAILURE);
    }





    /*****      read in as binary code and initialize the Memory!!!  *******/
    bitset<32> iimage, dimage;
    bitset<32> buff;
    int idx = 0;
    readCount = 0;
    while( input1.read( (char *)&buff , sizeof(int) ) != NULL ){
            /*****   convert little-endian to big-endian  *****/
            int big = 0, little = 24;
            for( int i = 1; i <= 32; i++){
                iimage[big] = buff[little];
                big++;
                little++;
                little = ( i % 8 == 0 ) ? little - 16 : little;
            }

            /****  initialization of iimage   ****/
            if ( readCount == 0 ){
                declaration::PC = ( unsigned )( iimage.to_ulong() );
                idx = declaration::PC >> 2;
            }
            else if ( readCount == 1 ){
                numberToRead = ( int )( iimage.to_ulong() );
            }
            else{
                declaration::iMemory[idx] = iimage;
                idx++;
            }
            readCount++;
            //test << iimage << endl;

    }

    //test << PC << endl;
    //test << numberToRead << endl;
    /*for( int i = 0; i < numberToRead; i++ )
        cout << declaration::iMemory[i + ( declaration::PC >> 2 ) ] << endl;*/

    int idy = 0;
    idx = 0;
    readCount = 0;
    while( input2.read( (char *)&buff , sizeof(int) ) != NULL ){
            /******   convert little-endian to big-endian *****/
            int big = 0, little = 24;
            for( int i = 1; i <= 32; i++){
                dimage[big] = buff[little];
                big++;
                little++;
                little = ( i % 8 == 0 ) ? little - 16 : little;
            }

             /****  initialization of  diimage   ****/

            if ( readCount == 0 ){
                declaration::reg[$SP] = ( unsigned )( dimage.to_ulong() );
            }
            else if ( readCount == 1 ){
                numberToRead = ( int )( dimage.to_ulong() );
            }
            else{
                idx = 0;
                for( int i = 0; i < 32; i++ ){
                    declaration::dMemory[idy][idx] = buff[i];
                    idx++;
                    if( idx == 8 ){
                        idx = 0;
                        idy++;
                    }
                }

            }
            readCount++;
            //cout << dimage << endl;
    }

    //test << reg[$SP] << endl;
    //test << numberToRead << endl;
    /*for( int i = 0; i < numberToRead * 4; i++ )
        cout << declaration::dMemory[i] << endl;*/


    input1.close();
    input2.close();
    return;

}
