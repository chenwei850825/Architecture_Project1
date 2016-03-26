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
    ;
}

void execution::fetch(int PC)
{
    instruction = declaration::iMemory[ PC >> 2 ];
    //cout << instruction << endl;
}


void execution::decode_execute()
{
    int idx = 0;
    for( int i = 26; i <= 31; i++ )
    {
        opcode[idx] = instruction[i];
        idx++;
    }

    cout << opcode << endl;

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
        //cout << opcode << rs << rt << rd << shamt << funct << endl;

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
        //cout << opcode << address << endl;
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
        //cout << opcode << address << endl;
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
        //cout << opcode << rs << rt << immediate << endl;
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
        if( declaration::reg[rsNow] > 0 && declaration::reg[rtNow] > 0 && overflow < 0 )
            declaration::numberOverflow = 1;
        else if( declaration::reg[rsNow] < 0 && declaration::reg[rtNow] < 0 && overflow > 0 )
            declaration::numberOverflow = 1;
        if( rdNow == 0 ){
            declaration::writeToZero = 1;
            break;
        }

        declaration::reg[rdNow] = declaration::reg[rsNow] + declaration::reg[rtNow];
        cout << "add" << endl;
        break;
    case ADDU:

        if( rdNow == 0 ){
            declaration::writeToZero = 1;
            break;
        }

        declaration::reg[rdNow] = declaration::reg[rsNow] + declaration::reg[rtNow];
        cout << "addu" << endl;
        break;
    case SUB:

        overflow = declaration::reg[rsNow] - declaration::reg[rtNow];
        if( declaration::reg[rsNow] > 0 && declaration::reg[rtNow] < 0 && overflow < 0 )
            declaration::numberOverflow = 1;
        else if( declaration::reg[rsNow] < 0 && declaration::reg[rtNow] > 0 && overflow > 0 )
            declaration::numberOverflow = 1;
        if( rdNow == 0 ){
            declaration::writeToZero = 1;
            break;
        }

        declaration::reg[rdNow] = declaration::reg[rsNow] - declaration::reg[rtNow];
        cout << "sub" << endl;
        break;
    case AND:

        if( rdNow == 0 ){
            declaration::writeToZero = 1;
            break;
        }

        declaration::reg[rdNow] = declaration::reg[rsNow] & declaration::reg[rtNow];
        cout << "and" << endl;
        break;
    case OR:

        if( rdNow == 0 ){
            declaration::writeToZero = 1;
            break;
        }

        declaration::reg[rdNow] = declaration::reg[rsNow] | declaration::reg[rtNow];
        cout << "or" << endl;
        break;
    case XOR:

        if( rdNow == 0 ){
            declaration::writeToZero = 1;
            break;
        }

        declaration::reg[rdNow] = declaration::reg[rsNow] ^ declaration::reg[rtNow];
        cout << "xor" << endl;
        break;
    case NOR:

        if( rdNow == 0 ){
            declaration::writeToZero = 1;
            break;
        }

        declaration::reg[rdNow] = ~( declaration::reg[rsNow] | declaration::reg[rtNow] ) ;
        cout << "nor" << endl;
        break;
    case NAND:

        if( rdNow == 0 ){
            declaration::writeToZero = 1;
            break;
        }

        declaration::reg[rdNow] = ~( declaration::reg[rsNow] & declaration::reg[rtNow] );
        cout << "nand" << endl;
        break;
    case SLT:

        if( rdNow == 0 ){
            declaration::writeToZero = 1;
            break;
        }

        declaration::reg[rdNow] = ( declaration::reg[rsNow] < declaration::reg[rtNow] );
        cout << "slt" << endl;
        break;
    case SLL:

        if( rdNow == 0 )
        {
            if( rtNow != 0 || shamt != 0 ){
                declaration::writeToZero = 1;
                break;
            }
        }

        declaration::reg[rdNow] = declaration::reg[rtNow] << shamtNow;
        cout << "sll" << endl;
        break;
    case SRL:

        if( rdNow == 0 ){
            declaration::writeToZero = 1;
            break;
        }

        declaration::reg[rdNow] = declaration::reg[rtNow] >> shamtNow;
        cout << "srl" << endl;
        break;
    case SRA:

        if( rdNow == 0 ){
            declaration::writeToZero = 1;
            break;
        }

        declaration::reg[rdNow] = declaration::reg[rtNow] >> shamtNow;
        cout << "sra" << endl;
        break;
    case JR:

        declaration::PC = declaration::reg[rsNow];
        cout << "jr" << endl;
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
        if( declaration::reg[rsNow] > 0 && immediateSigned > 0 && overflow < 0 )
            declaration::numberOverflow = 1;
        else if( declaration::reg[rsNow] < 0 && immediateSigned < 0 && overflow > 0 )
            declaration::numberOverflow = 1;
        if( rtNow == 0 ){
            declaration::writeToZero = 1;
            break;
        }

        declaration::reg[rtNow] = declaration::reg[rsNow] + immediateSigned;
        cout << "addi" << endl;
        break;

    case ADDIU:

        if( rtNow == 0 ){
            declaration::writeToZero = 1;
            break;
        }


        declaration::reg[rtNow] = declaration::reg[rsNow] + immediateNow;
        cout << "addiu" << endl;
        break;

    case LW:
        {


        overflow = declaration::reg[rsNow] + immediateSigned + 3;
        if( overflow >= 1024 ){
            declaration::memoryOverflow = 1;
            declaration::halt = 1;
        }
        overflow = declaration::reg[rsNow] + immediateSigned;
        if( overflow % 4 != 0 ){
            declaration::dataMisalign = 1;
            declaration::halt = 1;
        }
        if( declaration::reg[rsNow] > 0 && immediateSigned > 0 && overflow < 0 )
            declaration::numberOverflow = 1;
        else if( declaration::reg[rsNow] < 0 && immediateSigned < 0 && overflow > 0 )
            declaration::numberOverflow = 1;
        if( rtNow == 0 ){
            declaration::writeToZero = 1;
            break;
        }


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
        cout << "lw" << endl;
        break;
    }

    case LH:
    {


        overflow = declaration::reg[rsNow] + immediateSigned + 1;
        if( overflow >= 1024 ){
            declaration::memoryOverflow = 1;
            declaration::halt = 1;
        }
        overflow = declaration::reg[rsNow] + immediateSigned;
        if( overflow % 2 != 0 ){
            declaration::dataMisalign = 1;
            declaration::halt = 1;
        }
        if( declaration::reg[rsNow] > 0 && immediateSigned > 0 && overflow < 0 )
            declaration::numberOverflow = 1;
        else if( declaration::reg[rsNow] < 0 && immediateSigned < 0 && overflow > 0 )
            declaration::numberOverflow = 1;
        if( rtNow == 0 ){
            declaration::writeToZero = 1;
            break;
        }


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
        cout << "lh" << endl;
        break;
    }

    case LHU:
    {

        overflow = declaration::reg[rsNow] + immediateSigned + 1;
        if( overflow >= 1024 ){
            declaration::memoryOverflow = 1;
            declaration::halt = 1;
        }
        overflow = declaration::reg[rsNow] + immediateSigned;
        if( overflow % 2 != 0 ){
            declaration::dataMisalign = 1;
            declaration::halt = 1;
        }
        if( declaration::reg[rsNow] > 0 && immediateSigned > 0 && overflow < 0 )
            declaration::numberOverflow = 1;
        else if( declaration::reg[rsNow] < 0 && immediateSigned < 0 && overflow > 0 )
            declaration::numberOverflow = 1;
        if( rtNow == 0 ){
            declaration::writeToZero = 1;
            break;
        }


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
        cout << "lhu" << endl;
        break;
    }

    case LB:
    {



        overflow = declaration::reg[rsNow] + immediateSigned;
        if( overflow >= 1024 ){
            declaration::memoryOverflow = 1;
            declaration::halt = 1;
        }
        if( declaration::reg[rsNow] > 0 && immediateSigned > 0 && overflow < 0 )
            declaration::numberOverflow = 1;
        else if( declaration::reg[rsNow] < 0 && immediateSigned < 0 && overflow > 0 )
            declaration::numberOverflow = 1;
        if( rtNow == 0 ){
            declaration::writeToZero = 1;
            break;
        }



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
        cout << "lb" << endl;
        break;
    }

    case LBU:
    {


        overflow = declaration::reg[rsNow] + immediateSigned;
        if( overflow >= 1024 ){
            declaration::memoryOverflow = 1;
            declaration::halt = 1;
        }
        if( declaration::reg[rsNow] > 0 && immediateSigned > 0 && overflow < 0 )
            declaration::numberOverflow = 1;
        else if( declaration::reg[rsNow] < 0 && immediateSigned < 0 && overflow > 0 )
            declaration::numberOverflow = 1;
        if( rtNow == 0 ){
            declaration::writeToZero = 1;
            break;
        }



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
        cout << "lbu" << endl;
        break;
    }

    case SW:
    {

        overflow = declaration::reg[rsNow] + immediateSigned + 3;
        if( overflow >= 1024 ){
            declaration::memoryOverflow = 1;
            declaration::halt = 1;
        }
        overflow = declaration::reg[rsNow] + immediateSigned;
        if( overflow % 4 != 0 ){
            declaration::dataMisalign = 1;
            declaration::halt = 1;
        }
        if( declaration::reg[rsNow] > 0 && immediateSigned > 0 && overflow < 0 )
            declaration::numberOverflow = 1;
        else if( declaration::reg[rsNow] < 0 && immediateSigned < 0 && overflow > 0 )
            declaration::numberOverflow = 1;



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
        cout << "sw" << endl;
        break;
    }

    case SH:
    {

        overflow = declaration::reg[rsNow] + immediateSigned + 1;
        if( overflow >= 1024 ){
            declaration::memoryOverflow = 1;
            declaration::halt = 1;
        }
        overflow = declaration::reg[rsNow] + immediateSigned;
        if( overflow % 2 != 0 ){
            declaration::dataMisalign = 1;
            declaration::halt = 1;
        }
        if( declaration::reg[rsNow] > 0 && immediateSigned > 0 && overflow < 0 )
            declaration::numberOverflow = 1;
        else if( declaration::reg[rsNow] < 0 && immediateSigned < 0 && overflow > 0 )
            declaration::numberOverflow = 1;


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
        cout << "sh" << endl;
        break;
    }

    case SB:
    {

        overflow = declaration::reg[rsNow] + immediateSigned;
        if( overflow >= 1024 ){
            declaration::memoryOverflow = 1;
            declaration::halt = 1;
        }
        if( declaration::reg[rsNow] > 0 && immediateSigned > 0 && overflow < 0 )
            declaration::numberOverflow = 1;
        else if( declaration::reg[rsNow] < 0 && immediateSigned < 0 && overflow > 0 )
            declaration::numberOverflow = 1;


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
        cout << "sb" << endl;
        break;
    }

    case LUI:

        if( rtNow == 0 ){
            declaration::writeToZero = 1;
            break;
        }

        declaration::reg[rtNow] = immediateNow << 16;
        cout << "lui" << endl;
        break;

    case ANDI:

        if( rtNow == 0 ){
            declaration::writeToZero = 1;
            break;
        }

        declaration::reg[rtNow] = declaration::reg[rsNow]  & immediateNow;
        cout << "andi" << endl;
        break;

    case ORI:

        if( rtNow == 0 ){
            declaration::writeToZero = 1;
            break;
        }

        declaration::reg[rtNow]  =  declaration::reg[rsNow]  | immediateNow;
        cout << "ori" << endl;
        break;

    case NORI:

        if( rtNow == 0 ){
            declaration::writeToZero = 1;
            break;
        }

        declaration::reg[rtNow]  = ~(  declaration::reg[rsNow] | immediateNow );
        cout << "nori" << endl;
        break;

    case SLTI:
        if( rtNow == 0 ){
            declaration::writeToZero = 1;
            break;
        }

        declaration::reg[rtNow]  = declaration::reg[rsNow]  < immediateSigned;
        cout << "slti" << endl;
        break;

    case BEQ:
        if( declaration::reg[rtNow] == declaration::reg[rsNow] )
        {
            declaration::PC = declaration::PC + 4 + 4 * immediateSigned;
            declaration::cycle++;
            cout << "beq" << endl;
            return;
        }
        cout << "beq" << endl;
        break;

    case BNE:
        if( declaration::reg[rtNow] != declaration::reg[rsNow] )
        {
            declaration::PC = declaration::PC + 4 + 4 * immediateSigned;
            declaration::cycle++;
            cout << "bne" << endl;
            return;
        }
        cout << "bne" << endl;
        break;

    case BGTZ:
        if( (int)declaration::reg[rtNow] > 0 )
        {
            declaration::PC = declaration::PC + 4 + 4 * immediateSigned;
            declaration::cycle++;
            cout << "bgtz" << endl;
            return;
        }
        cout << "bgtz" << endl;
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
        cout << "j" << endl;
        break;
    case JAL :
        declaration::reg[31] = declaration::PC + 4;
        declaration::PC = ( ( declaration::PC + 4 ) >> 28 << 28 ) | ( 4 * addressNow );
        cout << "jal" << endl;
        break;
    }

    declaration::cycle++;

}
void execution::terminal_type()
{
    cout << "halt" << endl;
    declaration::halt = 1;
}


