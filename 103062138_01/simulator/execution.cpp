#include"execution.h"

template<std::size_t B>
long bitset_to_long(const std::bitset<B>& b)
{
    struct
    {
long x:
        B;
    } s;
    return s.x = b.to_ulong();
}



execution::execution()
{

    //test.open( "//test1.out", ios::out | ios::binary );
}

void execution::fetch(int PC)
{
    instruction = declaration::iMemory[ PC >> 2 ];
    /*if( declaration::cycle == 150 )
        cout << instruction << endl;*/
    ////test << instruction << endl;
}


void execution::decode_execute()
{
    int idx = 0;
    for( int i = 26; i <= 31; i++ )
    {
        opcode[idx] = instruction[i];
        idx++;
    }

    ////test << opcode << endl;

    //R-type
    if( opcode == 0x00 )
    {
        idx = 0;
        for( int i = 21; i <= 25; i++ )
        {
            rs[idx] = instruction[i];
            idx++;
        }
        idx = 0;
        for( int i = 16; i <= 20; i++ )
        {
            rt[idx] = instruction[i];
            idx++;
        }
        idx = 0;
        for( int i = 11; i <= 15; i++ )
        {
            rd[idx] = instruction[i];
            idx++;
        }
        idx = 0;
        for( int i = 6; i <= 10 ; i++ )
        {
            shamt[idx] = instruction[i];
            idx++;
        }
        idx = 0;
        for( int i = 0; i <= 5; i++ )
        {
            funct[idx] = instruction[i];
            idx++;
        }
        R_type();
        ////test << opcode << rs << rt << rd << shamt << funct << endl;

    }

    //J-type
    else if ( opcode == 0x02 || opcode == 0x03 )
    {
        idx = 0;
        for( int i = 0; i <= 25; i++ )
        {
            address[idx] = instruction[i];
            idx++;
        }
        J_type ( );
        ////test << opcode << address << endl;
    }

    //Terminal-type
    else if ( opcode == 0x3F )
    {
        idx = 0;
        for( int i = 0; i <= 25; i++ )
        {
            address[idx] = instruction[i];
            idx++;
        }
        terminal_type();
        ////test << opcode << address << endl;
    }

    //I-type
    else
    {
        idx = 0;
        for( int i = 21; i <= 25; i++ )
        {
            rs[idx] = instruction[i];
            idx++;
        }
        idx = 0;
        for( int i = 16; i <= 20; i++ )
        {
            rt[idx] = instruction[i];
            idx++;
        }
        idx = 0;
        for( int i = 0; i <= 15; i++ )
        {
            immediate[idx] = instruction[i];
            idx++;
        }
        I_type();
        ////test << opcode << rs << rt << immediate << endl;
    }
}



void execution::R_type()
{

    unsigned rsNow = ( unsigned )( rs.to_ulong() );
    unsigned rtNow = ( unsigned )( rt.to_ulong() );
    unsigned rdNow = ( unsigned )( rd.to_ulong() );
    unsigned shamtNow = ( unsigned )( shamt.to_ulong() );
    unsigned functNow = ( unsigned )( funct.to_ulong() );

    switch( functNow )
    {

    case ADD:

        overflow = declaration::reg[rsNow] + declaration::reg[rtNow];
        if( declaration::reg[rsNow] > 0 && declaration::reg[rtNow] > 0 && overflow <= 0 )
            declaration::numberOverflow = 1;
        else if( declaration::reg[rsNow] < 0 && declaration::reg[rtNow] < 0 && overflow >= 0 )
            declaration::numberOverflow = 1;
        if( rdNow == 0 ){
            declaration::writeToZero = 1;
            break;
        }

        declaration::reg[rdNow] = declaration::reg[rsNow] + declaration::reg[rtNow];
        //test << "add" << endl;
        break;
    case ADDU:

        if( rdNow == 0 ){
            declaration::writeToZero = 1;
            break;
        }

        declaration::reg[rdNow] = declaration::reg[rsNow] + declaration::reg[rtNow];
        //test << "addu" << endl;
        break;
    case SUB:
        {

        int complement = - ( declaration::reg[rtNow] );
        overflow = declaration::reg[rsNow] + complement;
        if( declaration::reg[rsNow] > 0 && complement > 0 && overflow <= 0 )
            declaration::numberOverflow = 1;
        else if( declaration::reg[rsNow] < 0 && complement < 0 && overflow >= 0 )
            declaration::numberOverflow = 1;
        if( rdNow == 0 ){
            declaration::writeToZero = 1;
            break;
        }

        declaration::reg[rdNow] = declaration::reg[rsNow] + complement;
        //test << "sub" << endl;
        break;

        }
    case AND:

        if( rdNow == 0 ){
            declaration::writeToZero = 1;
            break;
        }

        declaration::reg[rdNow] = declaration::reg[rsNow] & declaration::reg[rtNow];
        //test << "and" << endl;
        break;
    case OR:

        if( rdNow == 0 ){
            declaration::writeToZero = 1;
            break;
        }

        declaration::reg[rdNow] = declaration::reg[rsNow] | declaration::reg[rtNow];
        //test << "or" << endl;
        break;
    case XOR:

        if( rdNow == 0 ){
            declaration::writeToZero = 1;
            break;
        }

        declaration::reg[rdNow] = declaration::reg[rsNow] ^ declaration::reg[rtNow];
        //test << "xor" << endl;
        break;
    case NOR:

        if( rdNow == 0 ){
            declaration::writeToZero = 1;
            break;
        }

        declaration::reg[rdNow] = ~( declaration::reg[rsNow] | declaration::reg[rtNow] ) ;
        //test << "nor" << endl;
        break;
    case NAND:

        if( rdNow == 0 ){
            declaration::writeToZero = 1;
            break;
        }

        declaration::reg[rdNow] = ~( declaration::reg[rsNow] & declaration::reg[rtNow] );
        //test << "nand" << endl;
        break;
    case SLT:

        if( rdNow == 0 ){
            declaration::writeToZero = 1;
            break;
        }

        declaration::reg[rdNow] = ( declaration::reg[rsNow] < declaration::reg[rtNow] );
        //test << "slt" << endl;
        break;
    case SLL:

        if( rdNow == 0 )
        {
            if( rtNow != 0 || shamt != 0 ){
                declaration::writeToZero = 1;
                break;
            }
        }

        declaration::reg[rdNow] = (unsigned)declaration::reg[rtNow] << shamtNow;
        //test << "sll" << endl;
        break;
    case SRL:

        if( rdNow == 0 ){
            declaration::writeToZero = 1;
            break;
        }

        declaration::reg[rdNow] = (unsigned)declaration::reg[rtNow] >> shamtNow;
        //test << "srl" << endl;
        break;
    case SRA:

        if( rdNow == 0 ){
            declaration::writeToZero = 1;
            break;
        }

        declaration::reg[rdNow] = declaration::reg[rtNow] >> shamtNow;
        //test << "sra" << endl;
        break;
    case JR:

        declaration::PC = declaration::reg[rsNow];
        //test << "jr" << endl;
        declaration::cycle++;
        return;

    }

    declaration::PC += 4;
    declaration::cycle++;

}
void execution::I_type()
{

    unsigned opcodeNow = ( unsigned )( opcode.to_ulong() );
    unsigned rsNow = ( unsigned )( rs.to_ulong() );
    unsigned rtNow = ( unsigned )( rt.to_ulong() );
    unsigned immediateNow = ( unsigned )( immediate.to_ulong() );
    int immediateSigned = bitset_to_long(immediate);

    switch( opcodeNow )
    {
    case ADDI:

        overflow = declaration::reg[rsNow] + immediateSigned;
        if( declaration::reg[rsNow] > 0 && immediateSigned > 0 && overflow <= 0 )
            declaration::numberOverflow = 1;
        else if( declaration::reg[rsNow] < 0 && immediateSigned < 0 && overflow >= 0 )
            declaration::numberOverflow = 1;
        if( rtNow == 0 ){
            declaration::writeToZero = 1;
            break;
        }


        declaration::reg[rtNow] = declaration::reg[rsNow] + immediateSigned;
        //test << "addi" << endl;
        break;

    case ADDIU:

        if( rtNow == 0 ){
            declaration::writeToZero = 1;
            break;
        }


        declaration::reg[rtNow] = declaration::reg[rsNow] + immediateSigned;
        //test << "addiu" << endl;
        break;

    case LW:
        {


        overflow = declaration::reg[rsNow] + immediateSigned;
        if( declaration::reg[rsNow] > 0 && immediateSigned > 0 && overflow <= 0 )
            declaration::numberOverflow = 1;
        else if ( declaration::reg[rsNow] < 0 && immediateSigned < 0 && overflow >= 0 )
            declaration::numberOverflow = 1;

        if( overflow % 4 != 0 ){
            declaration::dataMisalign = 1;
            declaration::halt = 1;
        }


        overflow = declaration::reg[rsNow] + immediateSigned;
        if( overflow >= 1024 || overflow < 0 ){
            declaration::memoryOverflow = 1;
            declaration::halt = 1;
        }
        overflow = declaration::reg[rsNow] + immediateSigned + 3;
        if( overflow >= 1024 || overflow < 0 ){
            declaration::memoryOverflow = 1;
            declaration::halt = 1;
        }


        if( rtNow == 0 ){
            declaration::writeToZero = 1;
        }

        if( declaration::writeToZero == 1 || declaration::memoryOverflow == 1 || declaration::dataMisalign == 1 )
            break;

        int idx = 0;
        bitset<32> tmp;
        int idy = declaration::reg[rsNow] + immediateSigned + 3;

        for( int i = 0; i < 32; i++ )
        {
            tmp[i] = declaration::dMemory[idy][idx];
            idx++;
            if( idx == 8 )
            {
                idx = 0;
                idy--;
            }
        }
        declaration::reg[rtNow] = (unsigned) ( tmp.to_ulong() );
        //test << "lw" << endl;
        break;
    }

    case LH:
    {



        overflow = declaration::reg[rsNow] + immediateSigned;
        if( declaration::reg[rsNow] > 0 && immediateSigned > 0 && overflow <= 0 )
            declaration::numberOverflow = 1;
        else if( declaration::reg[rsNow] < 0 && immediateSigned < 0 && overflow >= 0 )
            declaration::numberOverflow = 1;

        if( overflow % 2 != 0 ){
            declaration::dataMisalign = 1;
            declaration::halt = 1;
        }

        overflow = declaration::reg[rsNow] + immediateSigned;
        if( overflow >= 1024 || overflow < 0 ){
            declaration::memoryOverflow = 1;
            declaration::halt = 1;
        }
        overflow = declaration::reg[rsNow] + immediateSigned + 1;
        if( overflow >= 1024 || overflow < 0 ){
            declaration::memoryOverflow = 1;
            declaration::halt = 1;
        }

        if( rtNow == 0 ){
            declaration::writeToZero = 1;
        }


        if( declaration::writeToZero == 1 || declaration::memoryOverflow == 1 || declaration::dataMisalign == 1 )
            break;




        int idx = 0;
        bitset<16> tmp;
        int idy = declaration::reg[rsNow] + immediateSigned + 1;
        for( int i = 0; i < 16; i++ )
        {
            tmp[i] = declaration::dMemory[idy][idx];
            idx++;
            if( idx == 8 )
            {
                idx = 0;
                idy--;
            }
        }

        declaration::reg[rtNow] = bitset_to_long(tmp);
        //test << "lh" << endl;
        break;
    }

    case LHU:
    {

        overflow = declaration::reg[rsNow] + immediateSigned;
        if( declaration::reg[rsNow] > 0 && immediateSigned > 0 && overflow <= 0 )
            declaration::numberOverflow = 1;
        else if( declaration::reg[rsNow] < 0 && immediateSigned < 0 && overflow >= 0 )
            declaration::numberOverflow = 1;

        if( overflow % 2 != 0 ){
            declaration::dataMisalign = 1;
            declaration::halt = 1;
        }


        overflow = declaration::reg[rsNow] + immediateSigned;
        if( overflow >= 1024 || overflow < 0 ){
            declaration::memoryOverflow = 1;
            declaration::halt = 1;
        }
        overflow = declaration::reg[rsNow] + immediateSigned + 1;
        if( overflow >= 1024 || overflow < 0 ){
            declaration::memoryOverflow = 1;
            declaration::halt = 1;
        }

        if( rtNow == 0 ){
            declaration::writeToZero = 1;
        }


        if( declaration::writeToZero == 1 || declaration::memoryOverflow == 1 || declaration::dataMisalign == 1 )
            break;


        int idx = 0;
        bitset<16> tmp;
        int idy = declaration::reg[rsNow] + immediateSigned + 1;
        for( int i = 0; i < 16; i++ )
        {
            tmp[i] = declaration::dMemory[idy][idx];
            idx++;
            if( idx == 8 )
            {
                idx = 0;
                idy--;
            }
        }
        declaration::reg[rtNow] = (unsigned)( tmp.to_ulong() );
        //test << "lhu" << endl;
        break;
    }

    case LB:
    {



        overflow = declaration::reg[rsNow] + immediateSigned;
        if( declaration::reg[rsNow] > 0 && immediateSigned > 0 && overflow <= 0 )
            declaration::numberOverflow = 1;
        else if( declaration::reg[rsNow] < 0 && immediateSigned < 0 && overflow >= 0 )
            declaration::numberOverflow = 1;

        if( overflow >= 1024 || overflow < 0 ){
            declaration::memoryOverflow = 1;
            declaration::halt = 1;
        }

        if( rtNow == 0 ){
            declaration::writeToZero = 1;
        }

        if( declaration::writeToZero == 1 || declaration::memoryOverflow == 1 )
            break;



        int idx = 0;
        bitset<8> tmp;
        int idy = declaration::reg[rsNow] + immediateSigned;
        for( int i = 0; i < 8; i++ )
        {
            tmp[i] = declaration::dMemory[idy][idx];
            idx++;
            if( idx == 8 )
            {
                idx = 0;
                idy--;
            }
        }
        // signed checked !!!!
        declaration::reg[rtNow] = bitset_to_long(tmp);
        //test << "lb" << endl;
        break;
    }

    case LBU:
    {


        overflow = declaration::reg[rsNow] + immediateSigned;
        if( declaration::reg[rsNow] > 0 && immediateSigned > 0 && overflow <= 0 )
            declaration::numberOverflow = 1;
        else if( declaration::reg[rsNow] < 0 && immediateSigned < 0 && overflow >= 0 )
            declaration::numberOverflow = 1;

        if( overflow >= 1024 || overflow < 0){
            declaration::memoryOverflow = 1;
            declaration::halt = 1;
        }

        if( rtNow == 0 ){
            declaration::writeToZero = 1;
        }

        if( declaration::writeToZero == 1 || declaration::memoryOverflow == 1 )
            break;



        int idx = 0;
        bitset<8> tmp;
        int idy = declaration::reg[rsNow] + immediateSigned;
        for( int i = 0; i < 8; i++ )
        {
            tmp[i] = declaration::dMemory[idy][idx];
            idx++;
            if( idx == 8 )
            {
                idx = 0;
                idy--;
            }
        }
        declaration::reg[rtNow] = (unsigned)( tmp.to_ulong() );
        //test << "lbu" << endl;
        break;
    }

    case SW:
    {

        overflow = declaration::reg[rsNow] + immediateSigned;
        if( overflow >= 1024 || overflow < 0 ){
            declaration::memoryOverflow = 1;
            declaration::halt = 1;
        }
        overflow = declaration::reg[rsNow] + immediateSigned + 3;
        if( overflow >= 1024 || overflow < 0){
            declaration::memoryOverflow = 1;
            declaration::halt = 1;
        }
        overflow = declaration::reg[rsNow] + immediateSigned;
        if( overflow % 4 != 0 ){
            declaration::dataMisalign = 1;
            declaration::halt = 1;
        }
        if( declaration::reg[rsNow] > 0 && immediateSigned > 0 && overflow <= 0 )
            declaration::numberOverflow = 1;
        else if( declaration::reg[rsNow] < 0 && immediateSigned < 0 && overflow >= 0 )
            declaration::numberOverflow = 1;


        if( declaration::memoryOverflow == 1 || declaration::dataMisalign == 1 )
            break;

        int idx = 0;
        bitset<32>tmp( declaration::reg[rtNow] );
        int idy = declaration::reg[rsNow] + immediateSigned + 3;
        for( int i = 0; i < 32; i++ )
        {
            declaration::dMemory[idy][idx] = tmp[i];
            idx++;
            if( idx == 8 )
            {
                idx = 0;
                idy--;
            }
        }
        //test << "sw" << endl;
        break;
    }

    case SH:
    {

        overflow = declaration::reg[rsNow] + immediateSigned;
        if( overflow >= 1024 || overflow < 0 ){
            declaration::memoryOverflow = 1;
            declaration::halt = 1;
        }
        overflow = declaration::reg[rsNow] + immediateSigned + 1;
        if( overflow >= 1024 || overflow < 0 ){
            declaration::memoryOverflow = 1;
            declaration::halt = 1;
        }
        overflow = declaration::reg[rsNow] + immediateSigned;
        if( overflow % 2 != 0 ){
            declaration::dataMisalign = 1;
            declaration::halt = 1;
        }
        if( declaration::reg[rsNow] > 0 && immediateSigned > 0 && overflow <= 0 )
            declaration::numberOverflow = 1;
        else if( declaration::reg[rsNow] < 0 && immediateSigned < 0 && overflow >= 0 )
            declaration::numberOverflow = 1;


        if( declaration::memoryOverflow == 1 || declaration::dataMisalign == 1 )
            break;


        int idx = 0;
        bitset<16>tmp( declaration::reg[rtNow] & 0x0000FFFF );
        int idy = declaration::reg[rsNow] + immediateSigned + 1;
        for( int i = 0; i < 16; i++ )
        {
            declaration::dMemory[idy][idx] = tmp[i];
            idx++;
            if( idx == 8 )
            {
                idx = 0;
                idy--;
            }
        }
        //test << "sh" << endl;
        break;
    }

    case SB:
    {

        overflow = declaration::reg[rsNow] + immediateSigned;
        if( overflow >= 1024 || overflow < 0 ){
            declaration::memoryOverflow = 1;
            declaration::halt = 1;
        }
        if( declaration::reg[rsNow] > 0 && immediateSigned > 0 && overflow <= 0 )
            declaration::numberOverflow = 1;
        else if( declaration::reg[rsNow] < 0 && immediateSigned < 0 && overflow >= 0 )
            declaration::numberOverflow = 1;


        if( declaration::memoryOverflow == 1 )
            break;


        int idx = 0;
        bitset<8>tmp( declaration::reg[rtNow] & 0x000000FF );
        int idy = declaration::reg[rsNow] + immediateSigned;
        for( int i = 0; i < 8; i++ )
        {
            declaration::dMemory[idy][idx] = tmp[i];
            idx++;
            if( idx == 8 )
            {
                idx = 0;
                idy--;
            }
        }
        //test << "sb" << endl;
        break;
    }

    case LUI:

        if( rtNow == 0 ){
            declaration::writeToZero = 1;
            break;
        }

        declaration::reg[rtNow] = immediateNow << 16;
        //test << "lui" << endl;
        break;

    case ANDI:

        if( rtNow == 0 ){
            declaration::writeToZero = 1;
            break;
        }

        declaration::reg[rtNow] = declaration::reg[rsNow]  & immediateNow;
        //test << "andi" << endl;
        break;

    case ORI:

        if( rtNow == 0 ){
            declaration::writeToZero = 1;
            break;
        }

        declaration::reg[rtNow]  =  declaration::reg[rsNow]  | immediateNow;
        //test << "ori" << endl;
        break;

    case NORI:

        if( rtNow == 0 ){
            declaration::writeToZero = 1;
            break;
        }

        declaration::reg[rtNow]  = ~(  declaration::reg[rsNow] | immediateNow );
        //test << "nori" << endl;
        break;

    case SLTI:
        if( rtNow == 0 ){
            declaration::writeToZero = 1;
            break;
        }

        declaration::reg[rtNow]  = declaration::reg[rsNow]  < immediateSigned;
        //test << "slti" << endl;
        break;

    case BEQ:
        if( declaration::reg[rtNow] == declaration::reg[rsNow] )
        {
            declaration::PC = declaration::PC + 4 + 4 * immediateSigned;
            declaration::cycle++;
            //test << "beq" << endl;
            return;
        }
        //test << "beq" << endl;
        break;

    case BNE:
        if( declaration::reg[rtNow] != declaration::reg[rsNow] )
        {
            declaration::PC = declaration::PC + 4 + 4 * immediateSigned;
            declaration::cycle++;
            //test << "bne" << endl;
            return;
        }
        //test << "bne" << endl;
        break;

    case BGTZ:

        if( declaration::reg[rsNow] > 0 )
        {
            declaration::PC = declaration::PC + 4 + 4 * immediateSigned;
            declaration::cycle++;
            //test << "bgtz" << endl;
            return;
        }
        //test << "bgtz" << endl;
        break;

    }

    declaration::PC += 4;
    declaration::cycle++;

}
void execution::J_type()
{
    unsigned opNow = ( unsigned ) ( opcode.to_ulong() ) ;
    unsigned addressNow = ( unsigned)  ( address.to_ulong() ) ;
    switch( opNow )
    {
    case J :
        declaration::PC = ( ( declaration::PC + 4 ) >> 28 << 28 ) | ( 4 * addressNow );
        //test << "j" << endl;
        break;
    case JAL :
        declaration::reg[31] = declaration::PC + 4;
        declaration::PC = ( ( declaration::PC + 4 ) >> 28 << 28 ) | ( 4 * addressNow );
        //test << "jal" << endl;
        break;
    }

    declaration::cycle++;

}
void execution::terminal_type()
{
    //test << "halt" << endl;
    declaration::halt = 1;
}


