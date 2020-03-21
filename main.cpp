# include <iostream>
# include <string>
# include <vector>
# include <fstream>
# include <math.h>
# include <iomanip>

using namespace std ;

# define SPACE ' '
# define TAB '  '
# define ENTER '\n'
# define COMMENT -1

// used to locate the table
# define INSTR 1
# define PSEUDO 2
# define REG 3
# define DELIMITER 4
# define SYMBOL 5
# define INT 6
# define STR 7

# define tableStartNo 1
# define tableEndNo 4

# define SIC_FORMAT 3
# define SICXE_FORMAT1 1
# define SICXE_FORMAT2 2
# define SICXE_FORMAT3 3
# define SICXE_FORMAT4 4

//vector<string> instrSet, pseudoSet, regSet, delimiterSet ;

struct Token {
    string str ; // the origin string of the token
    int table ; // the table which this token belong with
    int no ; // the no. of this token in this table
} ;

struct originInstr {
    int lineNo ;
    string str ;
    int numOfToken ;
    vector<Token> tokens ; // store the desperate token
    bool isComment ;
    
    originInstr() {
        lineNo = 0 ;
        str = "" ;
        numOfToken = 0 ;
        isComment = false ;
    }
} ;

struct tableInfo {
    int no ; // no. of the table
    string str ;
} ;

struct Instr_Detail { // use to know whether this instruction is which type and the opcode
    string instrName ; // the name of this instruction
    int operandNum ; // how many oprand require in this instruction
    string op1 ;
    string op2 ;
    vector<int> format ; // which format is this instruction belongs
    string opcode ;
    
    Instr_Detail() {
        
        instrName = "" ;
        operandNum = 0 ;
        op1 = "" ;
        op2 = "" ;
        opcode = "" ;
        
    } // constructor
    
} ; // Instr_set

struct regInfo {
     
     string name ;
     string content ;
     
     regInfo( string str, string con ) {
         name = str ;
         content = con ;
     } // reg()
     
     
 } ; // reg
 
 struct opInfo {
     
     string opName ;
     string opcode ;
     int length ;
     vector<int> type ;
     
     opInfo() {
         
         opName = "" ;
         opcode = "" ;
         length = -1 ;
         
     } // constructor()
     
 } ; // opInfo
 
 struct symbolInfo {
     
     string name ;
     int addr ;
     int type ;
     
     symbolInfo() {
         
         name = "" ;
         addr = -1 ;
         type = -1 ;
         
     } // constructor
     
 } ; // symbolInfo
 
 struct literalInfo {
     
     string name ;
     vector<vector<Token>> tokenGroup ;
     int addr ;
     bool hasAddr ;
     
     literalInfo() {
         
         name = "" ;
         addr = -1 ;
         hasAddr = false ;
         
     } // constructor()
     
 } ; // LiteralInfo

string upperToLower( string str ) {
    
    string tmpStr = "" ;
    
    for ( int i = 0 ; i < str.length() ; i ++ ) {
        
        if ( str[ i ] >= 'A' && str[ i ] <= 'Z' ) {
            
            tmpStr += str[ i ] + 32 ;
            
        } // if()
        else {
            
            tmpStr += str[ i ] ;
            
        } // else()
        
    } // for()
    
    return tmpStr ;
    
} // upperToLower()

string lowerToUp( string str ) {
    
    string tmpStr = "" ;
    
    for ( int i = 0 ; i < str.length() ; i ++ ) {
        
        if ( str[ i ] >= 'a' && str[ i ] <= 'z' ) {
            
            tmpStr += str[ i ] - 32 ;
            
        } // if()
        else {
            
            tmpStr += str[ i ] ;
            
        } // else()
        
    } // for()
    
    return tmpStr ;
    
} // upperToLower()

int transferToAscii( string str ) {
    
    int ans = 0 ;
    
    for ( int i = 0 ; i < str.length() ; i ++ ) {
        
        ans += str[ i ] ;
        
    } // for()
    
    return ans ;
    
} // transferToAscii()

class Instruction_Set_Detail {
    
    private :
    
    vector<Instr_Detail> set ;
    
    void addInstr( string str ) {

        // ex: ADD m 3/4 18
        
        Instr_Detail tmp ;
            
        int countWhite = 0 ;
        for ( int i = 0 ; i < str.length() ; i ++ ) {
                
            if ( str[ i ] == ' ' ) {
                    
                countWhite ++ ;
                    
            } // if()
            else if ( countWhite == 0 ) { // find the name of the instruction
                
                tmp.instrName += str[ i ] ;
                    
            } // else if()
            else if ( countWhite == 1 ) {
                    
                if ( str[ i ] >= '0' && str[ i ] <= '9' ) { // this instruction has no operand
                        
                    tmp.format.push_back( ( str[ i ] - '0' ) ) ;
                    countWhite ++ ;
                        
                } // if()
                else { // this instruction has operand
                        
                    tmp.operandNum ++ ; // at least has one operand
            
                    bool isCommon = false ;
                    int tmpI ;
                    for ( tmpI = i ; str[ tmpI ] != ' ' && tmpI < str.length() ; tmpI ++ ) {
                            
                        if ( str[ tmpI ] == ',' ) {
                                
                            isCommon = true ;
                            tmp.operandNum ++ ;
                                
                        } // if()
                        else if ( !isCommon ) {
                                
                            tmp.op1 += str[ tmpI ] ;
                                
                        } // if()
                        else if ( isCommon ) {
                                
                            tmp.op2 += str[ tmpI ] ;
                                
                        } // else()
                            
                    } // for()
                        
                    i = tmpI - 1 ;
                        
                } // else()
                    
            } // else if()
            else if ( countWhite == 2 ) {
                    
                if ( str[ i ] != '/' ) {
                        
                    tmp.format.push_back( ( str[ i ] - '0' ) ) ;
                        
                } // if()
                    
            } // else if()
            else if ( countWhite == 3 ) {
                    
                tmp.opcode += str[ i ] ;
                    
            } // else if()
                
        } // for()
        
        set.push_back( tmp ) ;
            
    } // addInstr()
    
    public :
    
    vector<Instr_Detail> getSet() {
        
        return set ;
        
    } // getSet()
    
    Instruction_Set_Detail() {
        
        addInstr( "add m 3/4 18" ) ;
        addInstr( "addf m 3/4 58" ) ;
        addInstr( "addr r1,r2 2 90" ) ;
        addInstr( "and m 3/4 40" ) ;
        addInstr( "clear r1 2 B4" ) ;
        addInstr( "comp m 3/4 28" ) ;
        addInstr( "compf m 3/4 88" ) ;
        addInstr( "compr r1,r2 2 A0" ) ;
        addInstr( "div m 3/4 24" ) ;
        addInstr( "divf m 3/4 64" ) ;
        addInstr( "divr r1,r2 2 9C" ) ;
        addInstr( "fix 1 C4" ) ;
        addInstr( "float 1 C0" ) ;
        addInstr( "hio 1 F4" ) ;
        addInstr( "j m 3/4 3C" ) ;
        addInstr( "jeq m 3/4 30" ) ;
        addInstr( "jgt m 3/4 34" ) ;
        addInstr( "jlt m 3/4 38" ) ;
        addInstr( "jsub m 3/4 48" ) ;
        addInstr( "lda m 3/4 00" ) ;
        addInstr( "ldb m 3/4 68" ) ;
        addInstr( "ldch m 3/4 50" ) ;
        addInstr( "ldf m 3/4 70" ) ;
        addInstr( "ldl m 3/4 08" ) ;
        addInstr( "lds m 3/4 6C" ) ;
        addInstr( "ldt m 3/4 74" ) ;
        addInstr( "ldx m 3/4 04" ) ;
        addInstr( "lps m 3/4 D0" ) ;
        addInstr( "mul m 3/4 20" ) ;
        addInstr( "mulf m 3/4 60" ) ;
        addInstr( "mulr r1,r2 2 98" ) ;
        addInstr( "norm 1 C8" ) ;
        addInstr( "or m 3/4 44" ) ;
        addInstr( "rd m 3/4 D8" ) ;
        addInstr( "rmo r1,r2 2 AC" ) ;
        addInstr( "rsub 3/4 4C" ) ;
        addInstr( "shiftl r1,n 2 A4" ) ;
        addInstr( "shiftr r1,n 2 A8" ) ;
        addInstr( "sio 1 F0" ) ;
        addInstr( "ssk m 3/4 EC" ) ;
        addInstr( "sta m 3/4 0C" ) ;
        addInstr( "stb m 3/4 78" ) ;
        addInstr( "stch m 3/4 54" ) ;
        addInstr( "stf m 3/4 80" ) ;
        addInstr( "sti 3/4 D4" ) ;
        addInstr( "stl m 3/4 14" ) ;
        addInstr( "sts m 3/4 7C" ) ;
        addInstr( "stsw m 3/4 E8" ) ;
        addInstr( "stt m 3/4 84" ) ;
        addInstr( "stx m 3/4 10" ) ;
        addInstr( "sub m 3/4 1C" ) ;
        addInstr( "subf m 3/4 5C" ) ;
        addInstr( "subr r1,r2 2 94" ) ;
        addInstr( "svc n 2 B0" ) ;
        addInstr( "td m 3/4 E0" ) ;
        addInstr( "tio 1 F8" ) ;
        addInstr( "tix m 3/4 2C" ) ;
        addInstr( "tixr r1 2 B8" ) ;
        addInstr( "wd m 3/4 DC" ) ;
        
    } // constructor()
    
    vector<int> findFormat( string instrName ) {
        
        instrName = upperToLower( instrName ) ;
        
        for ( int i = 0 ; i < set.size() ; i ++ ) {
            
            if ( instrName == set[ i ].instrName ) {
                
                return set[ i ].format ;
                
            } // if()
            
        } // for()
        
        vector<int> err ;
        err.push_back( -1 ) ;
        
        return err ;
        
    } // findFormat()
    
    bool checkOperandNum( string instrName, int tokenGroupSize ) {
        
        instrName = upperToLower( instrName ) ;
        
        for ( int i = 0 ; i < set.size() ; i ++ ) {
            
            if ( instrName == set[ i ].instrName ) {
                
                if ( tokenGroupSize == set[ i ].operandNum ) {
                    
                    return true ;
                    
                } // if()
                
            } // if()
            
        } // for()
        
        return false ;
        
    } // checkOperandNum()
    
    string getOpcode( string instrName ) {
        
        instrName = upperToLower( instrName ) ;
        
        for ( int i = 0 ; i < set.size() ; i ++ ) {
            
            if ( instrName == set[ i ].instrName ) {
                
                return set[ i ].opcode ;
                
            } // if()
            
        } // for()
        
        return  "" ;
        
    } // getOpcode()
    
    void printSet() {
        
        for ( int count = 0 ; count < set.size() ; count ++ ) {
            
            cout << "InstrName: " << set[ count ].instrName << endl ;
            
            if ( set[ count ].op1 == "" && set[ count ].op2 == "" ) {
                
                cout << "Operand: No operand!" << endl ;
                
            } // if()
            else {
                
                cout << "Operand: " << set[ count ].op1 << " " << set[ count ].op2 << endl ;
                
            } // else()
            
            cout << "Format: " ;
            for ( int i = 0 ; i < set[ count ].format.size() ; i ++ ) {
                
                cout << set[ count ].format[ i ] << " " ;
                
            } // for()
            cout << endl ;
            
            cout << "Opecode: " << set[ count ].opcode << endl << endl ;
            
        } // for()
        
    } // printDetail()
    
} ;

class Global {
    
private:
    
# define ABSOLUTE 1
# define RELATIVE 2
    
public:
    
    char hex[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' ,'A', 'B', 'C', 'D', 'E', 'F' } ;
    
    vector<regInfo> regTable ;
    
    string version ;
    
    int locctr ;
    
    Instruction_Set_Detail isd ;
    
    vector<originInstr> originInstrSet ;
    
    vector<string> instrSet, pseudoSet, regSet, delimiterSet ;
    
    vector<tableInfo> buffer_table_1to4[ 4 ] ;
    tableInfo buffer_table_5to7[ 100 ][ 3 ] ;
    
    symbolInfo symbolTable[100] ;
    
    literalInfo literalTable[100] ;
    
    Global() {
        
        version = "" ;
        locctr = 0 ;
        
        symbolInfo tmpSym ;
        literalInfo tmpLit ;
        for ( int i = 0 ; i < 100 ; i ++ ) { // initialize
            
            symbolTable[ i ] = tmpSym ;
            literalTable[ i ] = tmpLit ;
            
        } // for()
        
        for ( int i = 0 ; i < regSet.size() ; i ++ ) {
            
            regInfo tmp( regSet[ i ], "0" ) ;
            regTable.push_back( tmp ) ;
            
            
        } // for()
        
    } // constructor()
    
    void addToSymbolTable( string symName, int address, int type ) {
        
        int ascii = transferToAscii( symName ) ;
        
        int index = ascii % 100 ;
        
        while ( symbolTable[ index ].name != "" && symName != symbolTable[ index ].name ) { // the column is not empty
            
            index ++ ;
            
            if ( index == 100 ) {
                
                index = 0 ; // start with the begin
                
            } // if()
            
        } // if()
        
        symbolInfo tmp ;
        tmp.name = symName ;
        tmp.addr = address ;
        
        symbolTable[ index ] = tmp ;
        
    } // addToSymbolTable()
    
    int findInSymTable( string str ) {
        
        int index = -1 ;
        bool hasFind = false ;
        
        for ( int i = 0 ; i < 100 && !hasFind ; i ++ ) {
            
            if ( str == symbolTable[ i ].name ) {
                
                index = i ;
                
            } // if()
            
        } // for()
        
        return index ;
        
    } // findInSymTable()
    
    int findInLitTable( string str ) {
        
        int index = -1 ;
        bool isFind = false ;
        for ( int i = 0 ; i < 100 && !isFind ; i ++ ) {
            
            if ( str == literalTable[ i ].name ) {
                
                index = i ;
                isFind = true ;
                
            } // if()
            
        } // for()
        
        return index ;
        
    } // findInLitTable()
    
    void addToLiteralTable( vector<vector<Token>> tGroup ) {
        
        // = x'b1'  只用b1 hash
        // = c'EOF' 只用EOF hash
        // = 3277   只用3277 hash
        string litName = "" ;
        
        if ( tGroup[ 0 ][ 0 ].str == "=" ) {
             
            if ( tGroup.size() == 2 ) { // = 3277
                
                litName = tGroup[ 0 ][ 1 ].str ;
                
            } // if()
            else {
                
                litName = tGroup[ 0 ][ 2 ].str ;
                
            } // else()
            
        } // if()
        else {
            
            cout << "### Error: something wrong when dealing literal! ###" << endl << endl ;
            
        } // else()
        
        int ascii = transferToAscii( litName ) ;
        
        int index = ascii % 100 ;
        
        while ( literalTable[ index ].name != "" && litName != literalTable[ index ].name ) { // the column is not empty
            
            index ++ ;
            
            if ( index == 100 ) {
                
                index = 0 ; // start with the begin
                
            } // if()
            
        } // if()
        
        // litName = lowerToUp( litName ) ;
        
        literalInfo tmp ;
        tmp.name = litName ;
        tmp.addr = 0 ;
        tGroup[ 0 ][ 2 ].str = lowerToUp( tGroup[ 0 ][ 2 ].str ) ;
        tmp.tokenGroup = tGroup ;
        
        literalTable[ index ] = tmp ;
        
    } // addToLiteralTable()
    
    vector<literalInfo> addressLiteral() {
        
        vector<literalInfo> literalSet ;
        
        for ( int i = 0 ; i < 100 ; i ++ ) {
            
            if ( literalTable[ i ].name != "" && !literalTable[ i ].hasAddr ) {
                
                literalInfo tmp ;
                tmp.name = literalTable[ i ].name ;
                tmp.tokenGroup = literalTable[ i ].tokenGroup ;
                tmp.hasAddr = true ;
                
                literalTable[ i ].addr = locctr ;
                literalTable[ i ].hasAddr = true ;
                
                literalSet.push_back( tmp ) ;
                
            } // if()
            
        } // for()
        
        return literalSet ;
        
    } // addressLiteral
    
} ; // Globle data

Global g ;

// -------------------------------- data structure for GET TOKEN --------------------------------

class Tool {
    
    public :
    
    void setTableContent( int tableNo, vector<tableInfo> tableInFile ) {
        
        if ( tableNo == 1 ) {
            
            for ( int i = 0 ; i < tableInFile.size() ; i ++ ) {
                
                g.instrSet.push_back( upperToLower( tableInFile[ i ].str ) ) ;
                
            } // for()
            
        } // if()
        else if ( tableNo == 2 ) {
            
            for ( int i = 0 ; i < tableInFile.size() ; i ++ ) {
                
                g.pseudoSet.push_back( upperToLower( tableInFile[ i ].str ) ) ;
                
            } // for()
            
        } // else if()
        else if ( tableNo == 3 ) {
            
            for ( int i = 0 ; i < tableInFile.size() ; i ++ ) {
                
                regInfo rTmp( tableInFile[ i ].str, "0" ) ;
                g.regTable.push_back( rTmp ) ;
                g.regSet.push_back( upperToLower( tableInFile[ i ].str ) ) ;
                
            } // for()
            
        } // else if()
        else if ( tableNo == 4 ) {
            
            for ( int i = 0 ; i < tableInFile.size() ; i ++ ) {
                
                g.delimiterSet.push_back( tableInFile[ i ].str ) ;
                
            } // for()
            
        } // else if()
        
    } //setTableContent()
    
    bool isSpace( char ch ) {
        
        if ( ch == ' ' || ch == '\t' ) {
            
            return true ;
            
        } // if()
        
        return false ;
        
    } // isSpace()
    
    bool isINSTR( string str ) {
        
        vector<string> :: iterator result = find( g.instrSet.begin(), g.instrSet.end(), upperToLower( str ) ) ;
        
        if ( result != g.instrSet.end() ) {
            
            return true ;
            
        } // if()
        
        return false ;
        
    } // isINSTR()
    
    bool isPSEUDO( string str ) {
        
        vector<string> :: iterator result = find( g.pseudoSet.begin(), g.pseudoSet.end(), upperToLower( str ) ) ;
        
        if ( result != g.pseudoSet.end() ) {
            
            return true ;
            
        } // if()
        
        return false ;
        
    } // isPSEUDO()
    
    bool isREG( string str ) {
        
        vector<string> :: iterator result = find( g.regSet.begin(), g.regSet.end(), upperToLower( str ) ) ;
        
        if ( result != g.regSet.end() ) {
            
            return true ;
            
        } // if()
        
        return false ;
        
    } // isREG()
    
    bool isDELIMITER( string str ) {
        
        vector<string> :: iterator result = find( g.delimiterSet.begin(), g.delimiterSet.end(), upperToLower( str ) ) ;
        
        if ( result != g.delimiterSet.end() ) {
            
            return true ;
            
        } // if()
        
        return false ;
        
    } // isDELIMITER()
    
    bool isINT( string str ) {
        
        // check that each character is between '0' and '9'
        
        if ( ( str[ 0 ] == 'x' || str[ 0 ] == 'X' ) && str[ 1 ] == '\'' && str[ str.length() - 1] == '\'' ) {
            
            return true ;
            
        } // if()
        else {

            for ( int i = 0 ; i < str.length() ; i ++ ) {
                
                if ( str[ i ] > '9' || str[ i ] < '0' ) {
                    
                    return false ;
                    
                } // if()
                
            } // for()
            
        } // else()
        
        return true ;
        
    } // isINT()
    
    bool isSTR( string str ) {
        
        if ( ( str[ 0 ] == 'c' || str[ 0 ] == 'C' ) && str[ 1 ] == '\'' && str[ str.length() - 1] == '\'' ) {
            
            return true ;
            
        } // if()
        
        return false ;
        
    } // isSTR()
    
    int classifyToken( string str ) {
        
        int category = -1 ;
        
        // table1 : instruction
        if ( isINSTR( str ) ) {
            
            category = INSTR ;
            
        } // if()
        // table2 : pseudo instruction
        else if ( isPSEUDO( str ) ) {
            
            category = PSEUDO ;
            
        } // else if()
        // table3 : register
        else if ( isREG( str ) ) {
            
            category = REG ;
            
        } // else if()
        // table4 : delimiter
        else if ( isDELIMITER( str ) ) {
            
            category = DELIMITER ;
            
        } // else if()
        // table7 : string
        else if ( isSTR( str ) ) {
            
            category = STR ;
            
        } // else if()
        // table6 : integer
        else if ( isINT( str ) ) {
            
            category = INT ;
            
        } // else if()
        // table5 : symbol
        else {
            
            category = SYMBOL ;
            
        } // else()
        
        return category ;
        
    } // classifyToken
    
    string itoa( int num ) {
        
        char ch = '\0' ;
        string str = "" ;
        
        ch = '0' + num ;
        
        str += ch ;
        
        return str ;
        
    } // itoa()
    
    int atoi( string str ) {
        
        int ans = 0 ;
        
        for ( int i = 0 ; i < str.length() ; i ++ ) {
            
            if ( str[ i ] >= '0' && str[ i ] <= '9' ) {
                
                ans *= 10 ;
                ans += str[ i ] - '0' ;
                
            } // if()
            else {
                
                cout << "### Error: wrong format of the number string! ###" << endl << endl ;
                
                return 0 ;
                
            } // else()
            
        } // for()
        
        return ans ;
        
    } // atoi()
    
    void skipSpace( int &index, string str ) {
        
        while ( isSpace( str[ index ] ) && index < str.length() ) {
            
            index ++ ;
            
        } // while()
        
    } // skipSpace()
    
    vector<Token> splitInstr( string str ) {
        
        vector<Token> tokenList ;
        
        int startIndex = 0 ;
        
        skipSpace( startIndex, str ) ;
        
        // assert : we have avoid the space in front of the first token
        
        while ( startIndex < str.length() ) {
            
            string tmpStr = "" ;
            
            string charToStr = "" ;
            bool stop = false ;
            for ( ; startIndex < str.length() && !isSpace( str[ startIndex ] ) && !stop ; startIndex ++ ) {
                
                charToStr = "" ;
                charToStr = str[ startIndex ] ;
                
                if ( !isDELIMITER( charToStr ) || charToStr == "'" ) { // test
                    
                    tmpStr += str[ startIndex ] ;
                    
                } // if()
                else {
                    
                    stop = true ;
                    
                } // else()
                
            } // for()
            
            // special case
            if ( ( isSTR( tmpStr ) || isINT( tmpStr ) ) && tmpStr != "" ) {
                
                vector<Token> tmpList = parseString( tmpStr ) ;
                
                for ( int i = 0 ; i < tmpList.size() ; i ++ ) {
                    
                    tokenList.push_back( tmpList[ i ] ) ;
                    
                } // for()
                
                tmpStr = "" ;
                charToStr = "" ;
                
                skipSpace( startIndex, str ) ;
                
            } // if()
            else if ( tmpStr != "" ) {
                
                // assert : we get one token
                
                Token token ;
                token.str = tmpStr ;
                token.table = -1 ;
                token.no = -1 ;
                tokenList.push_back( token ) ;
                
                skipSpace( startIndex, str ) ;
                
            } // else()
            
            if ( charToStr == "." ) {
                
                return tokenList ;
                
            } // if()
            else if ( isDELIMITER( charToStr ) && charToStr != "" ) {
                
                Token token ;
                token.str = charToStr ;
                token.table = DELIMITER ;
                token.no = -1 ;
                tokenList.push_back( token ) ;
                
            } // else if()
            
        } // while()
        
        return tokenList ;
        
    } // splitInstr()
    
    string decNumToHexStr( int num ) {
        
        string transStr = "" ;
        string tmp = "" ;
        
        if ( num == 0 ) return  "0" ;
        
        while ( num > 0 ) {
            
            tmp = g.hex[ num % 16 ] ;
            transStr = tmp + transStr ;
            num /= 16 ;
            
        } // while()
        
        return transStr ;
            
    } // decNumToHexStr()

    string BinaryStrToHexStr( string str ) {
        
        // four bits create one byte
        string hexStr = "" ;
        /*
        if ( ( str.length() % 4 ) != 0 ) {
            
            cout << "### Error: Wrong format of the binary string! ###" << endl << endl ;
            
            return "" ;
            
        } // if()
        */
        for ( int i = 0 ; i < str.length(); i ++ ) { // start with the last bit
            
            int ans = 0 ;
            int index = 0 ;
            for ( int count = 3 ; count >= 0 ; count -- ) {
                
                if ( str[ i + index ] == '0' ) ;
                else if ( str[ i + index ] == '1' ) {
                    
                    ans += 1 * pow( 2, count ) ;
                    
                } // else if()
                else {
                    
                    cout << "### Error: Wrong with the binary string! ###" << endl << endl ;
                    
                    return "" ;
                    
                } // else()
                
                index ++ ;
                
            } // for()
            
            hexStr += g.hex[ ans ] ;
            
            i += 3 ;
            
        } // for()
        
        return hexStr ;
        
    } // BinaryStrToHexStr()
    
    string hexToBin( char numChar ) {
        
        string transStr = "" ;
        
        if ( numChar == '0' ) transStr = "0000" ;
        else if ( numChar == '1' ) transStr = "0001" ;
        else if ( numChar == '2' ) transStr = "0010" ;
        else if ( numChar == '3' ) transStr = "0011" ;
        else if ( numChar == '4' ) transStr = "0100" ;
        else if ( numChar == '5' ) transStr = "0101" ;
        else if ( numChar == '6' ) transStr = "0110" ;
        else if ( numChar == '7' ) transStr = "0111" ;
        else if ( numChar == '8' ) transStr = "1000" ;
        else if ( numChar == '9' ) transStr = "1001" ;
        else if ( numChar == 'A' || numChar == 'a' ) transStr = "1010" ;
        else if ( numChar == 'B' || numChar == 'b' ) transStr = "1011" ;
        else if ( numChar == 'C' || numChar == 'c' ) transStr = "1100" ;
        else if ( numChar == 'D' || numChar == 'd' ) transStr = "1101" ;
        else if ( numChar == 'E' || numChar == 'e' ) transStr = "1110" ;
        else if ( numChar == 'F' || numChar == 'f' ) transStr = "1111" ;
        else cout << "### Error: Fail to transfer hex to dec! ###" << endl << endl ;
        
        return transStr ;
            
    } // hexToBin()

    string hexStrToBinaryStr( string str ) {
        
        string bStr = "" ;
        
        for ( int i = 0 ; i < str.length() ; i ++ ) {
            
            bStr += hexToBin( str[ i ] ) ;
            
        } // for()
        
        return bStr ;
        
    } // hexStrToBinaryStr()
    
    int hexStrToDecNum( string str ) {
        
        int ans = 0 ;
        int power = 0 ;
        
        for ( int i = ( int ) str.length() - 1 ; i >= 0 ; i -- ) {
            
            if ( str[ i ] >= '0' && str[ i ] <= '9' ) {
                ans += ( str[ i ] - '0' ) * pow( 16, power ) ;
            } // if()
            else if ( str[ i ] == 'a' || str[ i ] == 'A' ){
                ans += 10 * pow( 16, power ) ;
            } // else()
            else if ( str[ i ] == 'b' || str[ i ] == 'B' ){
                ans += 11 * pow( 16, power ) ;
            } // else()
            else if ( str[ i ] == 'c' || str[ i ] == 'C' ){
                ans += 12 * pow( 16, power ) ;
            } // else()
            else if ( str[ i ] == 'd' || str[ i ] == 'D' ){
                ans += 13 * pow( 16, power ) ;
            } // else()
            else if ( str[ i ] == 'e' || str[ i ] == 'E' ){
                ans += 14 * pow( 16, power ) ;
            } // else()
            else if ( str[ i ] == 'f' || str[ i ] == 'F' ){
                ans += 15 * pow( 16, power ) ;
            } // else()
            
            power ++ ;
            
        } // for()
        
        return ans ;
        
    } // hexToDec()
    
    vector<Token> parseString( string str ) {
        
        // three types : 1. c'EOF' 2. x'ABC'
        
        vector<Token> tmpList ;
        
        string tmpStr = "" ;
        
        if ( isSTR( str ) ) {
            
            for ( int i = 1 ; i < str.length() ; i ++ ) {
                
                Token tmpToken ;
                
                string charToStr = "" ;
                
                charToStr += str[ i ] ;
                
                if ( isDELIMITER( charToStr ) && charToStr != "'" ) {
                    
                    tmpToken.str = charToStr ;
                    tmpToken.table = DELIMITER ;
                    tmpToken.no = -1 ;
                    
                    tmpList.push_back( tmpToken ) ;
                    
                } // if()
                else if ( str[ i ] == '\'' ) {
                    
                    if ( i != 1 ) {
                        
                        tmpToken.str = tmpStr ;
                        tmpToken.table = STR ;
                        tmpToken.no = -1 ;
                        
                        tmpList.push_back( tmpToken ) ;
                        
                    } // if()
                    
                    tmpToken.str = charToStr ;
                    tmpToken.table = DELIMITER ;
                    tmpToken.no = -1 ;
                    
                    tmpList.push_back( tmpToken ) ;
                    
                } // else if()
                else {
                    
                    tmpStr += str[ i ] ;
                    
                } // else()
                
            } // for()
            
        } // if()
        else if ( isINT( str ) ) {
            
            string tmpStr = "" ;
            
            for ( int i = 0 ; i < str.length() ; i ++ ) {
                
                Token tmpToken ;
                
                if ( ( str[ i ] == 'x' || str[ i ] == 'X' ) && i == 0 ) {
                    
                    i ++ ;
                    
                } // if()
                
                if ( str[ i ] == '\'' ) {
                    
                    if ( i != 1 ) {
                        
                        tmpToken.str = lowerToUp( tmpStr ) ;
                        tmpToken.table = INT ;
                        tmpToken.no = -1 ;
                        
                        tmpList.push_back( tmpToken ) ;
                        
                    } // if()
                    
                    tmpToken.str = str[ i ] ;
                    tmpToken.table = -1 ;
                    tmpToken.no = -1 ;
                    
                    tmpList.push_back( tmpToken ) ;
                    
                } // if()
                else {
                    
                    tmpStr += str[ i ] ;
                    
                    if ( i == str.length() - 1 ) {
                        
                        tmpToken.str = tmpStr ;
                        tmpToken.table = INT ;
                        tmpToken.no = -1 ;
                        
                        tmpList.push_back( tmpToken ) ;
                        
                    } // if()
                    
                } // else()
                
            } // for()
            
        } // else if()
        
        return tmpList ;
        
    } // parseString()
    
    string binarySub2Bin( string num1, string num2 ) {
        
        // hexnum
        string bNum1 = "" ;
        string bNum2 = "" ;
        string bAns = "" ;
        
        bNum1 = hexStrToBinaryStr( num1 ) ;
        bNum2 = hexStrToBinaryStr( num2 ) ;
        
        if ( hexStrToDecNum( num1 ) >= hexStrToDecNum( num2 ) ) {
            
            bAns = hexStrToBinaryStr( decNumToHexStr( hexStrToDecNum( num1 ) - hexStrToDecNum( num2 ) ) ) ;
            
        } // if()
        else {
        
            // assert : num1 is smaller
        
            // transfer num2 to 2'complement
            int decAns = -1 ;
            string tmpNum2 = "" ;
            for ( int i = 0 ; i < bNum2.length() ; i ++ ) {
                
                if ( bNum2[ i ] == '0' ) {
                    tmpNum2 += '1' ;
                } // if()
                else {
                    tmpNum2 += '0' ;
                } // else()
                
            } // for()
            
            decAns = hexStrToDecNum( num1 ) + ( hexStrToDecNum( BinaryStrToHexStr( tmpNum2 ) ) + 1 ) ;
            
            bAns = hexStrToBinaryStr( decNumToHexStr( decAns ) ) ;
            
        } // else()
        
        return bAns ;
        
    } // binarySub()
    
    /*
    void print_packed_instr( Packed_instr pInstr ) {
        
        cout << "LABEL: " << pInstr.label << endl ;
        cout << "INSTR: " << pInstr.instr << endl ;
        for ( int i = 0 ; i < pInstr.amount ; i ++ ) {
            
            cout << "GROUP" << i + 1 << ": " ;
            for ( int j = 0 ; j < pInstr.token_groups[ i ].size() ; j ++ ) {
                
                cout << pInstr.token_groups[ i ][ j ].str ;
                
            } // for()
            cout << endl ;
            
        } // for()
        
    } // print_packed_instr()
    */
} ; // Tool

Tool tool ;

class ProcessTable {
    
    private :
    
    fstream file ;
    
    public :
    
    void findTokenInTable( Token token, int &tableNo, int &column ) {
         
        tableNo = -1 ;
        column = -1 ;
        
        string lowerStr = upperToLower( token.str ) ; // transfer the token string into lower case for comparison
        
        if ( token.table != -1 ) {
            
            tableNo = token.table ;
            
        } // if()
        else {
            
            tableNo = tool.classifyToken( lowerStr ) ;
            
        } // else()
         
         if ( tableNo >= 1 && tableNo <= 4 ) {
             
             for ( int i = 0 ; i < g.buffer_table_1to4[ tableNo - 1 ].size() && column == -1 ; i ++ ) {
                 
                 if ( lowerStr == g.buffer_table_1to4[ tableNo - 1 ][ i ].str ) { // find the same token
                     
                     column = g.buffer_table_1to4[ tableNo - 1 ][ i ].no ;
                     
                 } // if()
                 
             } // for()
             
         } // if()
         else {
             
             bool isFind = false ;
             for ( int i = 0 ; i < 100 && !isFind ; i ++ ) {
                 
                 if ( token.str == g.buffer_table_5to7[ i ][ tableNo - 5 ].str ) {
                     
                     column = g.buffer_table_5to7[ i ][ tableNo - 5 ].no ;
                     
                     isFind = true ;
                     
                 } // if()
                 
             } // for()
             
             if ( !isFind ) { // if we doesn't find the token in the table
                 
                 addToTable( tableNo, token.str, column ) ;
                 
             } // if()
             
         } // else()
         
     } // findTokenInTable()
    
    bool readFile() {
        
        string startNo = "" ;
        string endNo = "" ;
        
        bool success = false ;
        do {
            
            cout << endl << "[Please enter START with table no. (Ex.1) ]: " ;
            
            cin >> startNo ;
            
            cout << endl << "[Please enter END with table no. (Ex.4) ]: " ;
            
            cin >> endNo ;
            
            if (  startNo != tool.itoa( tableStartNo ) || endNo != tool.itoa( tableEndNo )  )  {
                
                cout << endl << "### Error: illegal table start or end No.! ###" << endl << endl ;
                
                cout << "Please try again!" << endl ;
                
            } // if()
            else {
                
                success = true ;
                
            } // else()
            
        } while ( !success );
        
        for ( int i = 0 ; i <= atoi( endNo.c_str() ) - atoi( startNo.c_str() ) ; i ++ ) {
            
            file.open( "Table" + tool.itoa( i + 1 ) + ".table", ios :: in ) ;
            
            if ( !file ) {
                
                cout << "### Error: build the table failed! ###" << endl << endl ;
                
                return false ;
                
            } // if()
            
            int countNo = 0 ;
            vector<tableInfo> tmp ;
            while ( !file.eof() ) {
                
                tableInfo tmpInfo ;
                string tokenStr ;
                
                getline( file, tokenStr ) ;
                
                string tmpStr = "" ;
                for ( int i = 0 ; i < tokenStr.length() && tokenStr[ i ] != '\r' && tokenStr[ i ] != ' ' ; i ++ ) {
                    
                    tmpStr += tokenStr[ i ] ;
                    
                } // for()
                tokenStr = tmpStr ;
                
                // table1~table3 we need to transfer the string into lower case
                if ( i < atoi( endNo.c_str() ) - atoi( startNo.c_str() - 1 ) ) {
                    
                    tokenStr = upperToLower( tokenStr ) ;
                    
                } // if()
                
                tmpInfo.no = countNo + 1 ;
                tmpInfo.str = tokenStr ;
                
                countNo ++ ;
                
                tmp.push_back( tmpInfo ) ;
                
            } // while()
            
            // assert : one of the table is finish reading
            
            g.buffer_table_1to4[ i ] = tmp ; // tables are in order
            
            tool.setTableContent( i + 1, tmp ) ; // we set the information from the data in the tool, then the tool we konw how to determine the token
            
            tmp.clear() ;
            
            file.close() ;
            
        } // for()
        
        // initialize table 5 - 7
        for ( int i = 0 ; i < 3 ; i ++ ) {
            
            for ( int j = 0 ; j < 100 ; j ++ ) {
                
                tableInfo initialTable ;
                initialTable.str = "" ;
                initialTable.no = j + 1 ;
                
                g.buffer_table_5to7[ j ][ i ] = initialTable ;
                
            } // for()
            
        } // for()
        
        return true ;
        
    } // readFile()
    
    bool saveFile() {
        
        for ( int i = 0 ; i < 4 ; i ++ ) {
            
            int no = tool.classifyToken( g.buffer_table_1to4[ i ][ 0 ].str ) ; // the first token of this table
            
            file.open( "myTable" + tool.itoa( no ) + ".table", ios :: out ) ;
            
            if ( !file ) {
                
                cout << "### Error: fail to save the table into files! ###" << endl << endl ;
                
                return false ;
                
            } // if()
            
            for ( int j = 0 ; j < g.buffer_table_1to4[ i ].size() ; j ++ ) {
                
                file << g.buffer_table_1to4[ i ][ j ].str << endl ;
                
            } // for()
            
            cout << "*** Successful! Create [myTable" + tool.itoa( no ) + ".table]" << "! ***" << endl << endl ;
            
            file.close() ;
            
            g.buffer_table_1to4[ i ].clear() ;
            
        } // for()
        
        for ( int i = 0 ; i < 3 ; i ++ ) {
            
            // int no = tool.classifyToken( buffer_table_5to7[ 0 ][ i ].str ) ; // the first token of this table
            
            file.open( "myTable" + tool.itoa( i + 5 ) + ".table", ios :: out ) ;
            
            if ( !file ) {
                
                cout << "### Error: fail to save the table into files! ###" << endl << endl ;
                
                return false ;
                
            } // if()
            
            for ( int j = 0 ; j < 100 ; j ++ ) {
                
                file << g.buffer_table_5to7[ j ][ i ].str << endl ;
                
            } // for()
            
            cout << "*** Successful! Create [myTable" + tool.itoa( i + 5 ) + ".table]" << "! ***" << endl << endl ;
            
            file.close() ;
            
        } // for()
        
        return true ;
         
    } // saveFile()
    
    void addToTable( int tableNo, string tokenStr, int &columnWePutIn ) {
        
        int ascii = transferToAscii( tokenStr ) ;
        
        int index = ascii % 100 ;
        
        while ( g.buffer_table_5to7[ index ][ tableNo - 5 ].str != "" ) { // the column is not empty
            
            index ++ ;
            
            if ( index == 100 ) {
                
                index = 0 ; // start with the begin
                
            } // if()
            
        } // if()
        
        g.buffer_table_5to7[ index ][ tableNo - 5 ].str = tokenStr ;
        
        // special for the symbol table
        
        if ( tableNo == 5 ) { // symbol table with only name information
            
            // we need to add the address feild for the address
            
            g.symbolTable[ index ].name = tokenStr ;
            g.symbolTable[ index ].addr = g.locctr ;
            
            columnWePutIn = index ;
            
        } // if()
        
    } // addToTable()
    
} ; // ProcessTable

ProcessTable processTable ; // most important is to store the tables

struct Packed_instr { // put the information of the each instruction in SIC
    
    string label ; // the label of this instruction
    string instr ; // the instruction token of this statement
    int amount ; // the amount of the token groups
    vector<vector<Token>> token_groups ;
    int loc ; // where is the address of this instruction
    int format ; // which format is this instruciton belongs
    int length ;
    string machineCode ;
    
    Packed_instr() {
        label = "" ;
        instr = "" ;
        amount = 0 ;
        loc = 0 ;
        format = -1 ;
        length = -1 ;
        machineCode = "" ;
    } // Packed_instr()
    
    void packed( originInstr origin_instr ) {
        
        bool hasPlus = false ;
        label = "" ;
        instr = "" ;
        amount = 0 ;
        int tokenGroupStartIndex = -1 ;
        
        if ( origin_instr.isComment ) {
            
            amount = -1 ;
            label = origin_instr.str ;
            
            return ;
            
        } // if()
        
        if ( tool.isINSTR( origin_instr.tokens[ 0 ].str ) || tool.isPSEUDO( origin_instr.tokens[ 0 ].str ) || origin_instr.tokens[ 0 ].str == "+" ) {
            
            if ( origin_instr.tokens[ 0 ].str == "+" ) {
                
                instr = origin_instr.tokens[ 1 ].str ;
                hasPlus = true ;
                
                tokenGroupStartIndex = 2 ;
                
            } // if()
            else {
                
                instr = origin_instr.tokens[ 0 ].str ;
                
                tokenGroupStartIndex = 1 ;
                
            } // else()
            
        } // if()
        else {
            
            label = origin_instr.tokens[ 0 ].str ;
            if ( origin_instr.tokens[ 1 ].str == "+" ) {
                
                instr = origin_instr.tokens[ 2 ].str ;
                hasPlus = true ;
                
                tokenGroupStartIndex = 3 ;
                
            } // if()
            else {
                
                instr = origin_instr.tokens[ 1 ].str ;
                
                tokenGroupStartIndex = 2 ;
                
            } // else()
            
        } // else()
        
        // assert : find which format is this instruction
        
        if ( g.version == "SIC" ) {
            
            format = SIC_FORMAT ;
            length = SIC_FORMAT ;
            
        } // if()
        else {
            
            if ( g.isd.findFormat( instr ).size() == 1 ) { // format 1 or 2
                
                if ( g.isd.findFormat( instr )[ 0 ] == 1 ) {
                    
                    format = SICXE_FORMAT1 ;
                    
                } // if()
                else if ( g.isd.findFormat( instr )[ 0 ] == 2 ) {
                    
                    format = SICXE_FORMAT2 ;
                    
                } // else if()
                
                length = format ;
                
            } // if()
            else if ( g.isd.findFormat( instr ).size() == 2 ) { // format 3 or 4
                
                if ( !hasPlus ) {
                    
                    format = SICXE_FORMAT3 ;
                    
                } // if()
                else {
                    
                    format = SICXE_FORMAT4 ;
                    
                } // else()
                
                length = format ;
                
            } // else if()
            
        } // else()
        
        // assert : after determine the format of this instruction, we now can check the operand
        
        vector<Token> tokenGroup_unit ; // a single token group will put in here
        for ( int i = tokenGroupStartIndex ; i < origin_instr.tokens.size() ; i ++ ) {
            
            if ( origin_instr.tokens[ i ].str == "," ) {
                
                token_groups.push_back( tokenGroup_unit ) ;
                amount ++ ;
                tokenGroup_unit.clear() ;
                
            } // if()
            else {
                
                tokenGroup_unit.push_back( origin_instr.tokens[ i ] ) ;
                
                if ( i == origin_instr.tokens.size() - 1 ) {
                    
                    token_groups.push_back( tokenGroup_unit ) ;
                    amount ++ ;
                    tokenGroup_unit.clear() ;
                    
                } // if()
                
            } // else()
            
        } // for()
        
        // assert : saperate the operands, now check whether the number of operands is correct
        
        if ( !g.isd.checkOperandNum( instr, ( int ) token_groups.size() ) && !tool.isPSEUDO( instr ) && token_groups.size() > 1
            && !tool.isREG( token_groups[ 1 ][ 0 ].str ) ) {
            
            cout << "### Error: operand number wrong exist with the instruction [" << instr << "] ! ###" << endl << endl ;
            
            return ;
            
        } // if()
        
        if ( tool.isPSEUDO( instr ) || instr == "" ) {
            
            if ( instr == "START" || instr == "start" ) {
                
                g.locctr = tool.hexStrToDecNum( token_groups[ 0 ][ 0 ].str ) ;
                
                length = 0 ;
                
            } // if()
            else if ( instr == "BYTE" || instr == "byte" ) {
                
                int tableNo = -1 ;
                int column = -1 ;
                
                processTable.findTokenInTable( token_groups[ 0 ][ 1 ], tableNo, column ) ;
                
                if ( tableNo == 6 ) { // real number
                    
                    length = ( int ) token_groups[ 0 ][ 1 ].str.length() / 2 ;
                    
                } // if()
                else if ( tableNo == 7 ) { // string
                    
                    length = ( int ) token_groups[ 0 ][ 1 ].str.length() ;
                    
                } // else if()
                
            } // else if()
            else if ( instr == "WORD" || instr == "word" ) {
                
                length = 3 ;
                
            } // else if()
            else if ( instr == "RESB" || instr == "resb" ) {
                
                length = tool.atoi( token_groups[ 0 ][ 0 ].str ) ;
                
            } // else if()
            else if ( instr == "RESW" || instr == "resw" ) {
                
                length = 3 * tool.hexStrToDecNum( token_groups[ 0 ][ 0 ].str ) ;
                
            } // else if()
            else if ( instr == "EQU" || instr == "equ" ) {
                
                int tmpAddr = 0 ;
                string del = "" ;
                vector<string> delSet ;
                vector<int> operandSet ;
                
                delSet.push_back( "+" ) ;
                
                for ( int i = 0 ; i < token_groups.size() ; i ++ ) {
                    
                    if ( tool.isDELIMITER( token_groups[ 0 ][ i ].str ) ) {
                        
                        delSet.push_back( token_groups[ 0 ][ i ].str ) ;
                        
                    } // if()
                    else {
                        
                        int indexInSymTable = g.findInSymTable( token_groups[ 0 ][ i ].str ) ;
                    
                        if ( indexInSymTable == -1 ) {
                            
                            operandSet.push_back( tool.atoi( token_groups[ 0 ][ i ].str ) ) ;
                            
                        } // if()
                        else {
                            
                            operandSet.push_back( g.symbolTable[ indexInSymTable ].addr ) ;
                            
                        } // else()
                        
                    } //else()
                    
                } // for()
                
                for ( int i = 0 ; i < operandSet.size() && i < delSet.size() ; i ++ ) {
                    
                    if ( delSet[ i ] == "+" ) {
                        
                        tmpAddr += operandSet[ i ] ;
                        
                    } // if()
                    else if ( delSet[ i ] == "-" ) {
                        
                        tmpAddr -= operandSet[ i ] ;
                        
                    } // else if()
                    else {
                        
                        cout << "### Error: wrong with EQU! ###" << endl << endl ;
                        
                    } // else()
                    
                } // for()
                
                delSet.clear() ;
                operandSet.clear() ;
                
                loc = tmpAddr ;
                
                int indexInSymTable = g.findInSymTable( label ) ;
                
                g.symbolTable[ indexInSymTable ].addr = tmpAddr ;
                
                return ;
                
            } // else if()
            
        } // if()
        
        loc = g.locctr ;
        
        if ( length != -1 ) { // if length == -1 means that this is a pseudo code
            
            g.locctr += length ;
            
        } // if()
        
        // assert : if is a symbol then add to the table
        
        if ( !tool.isINSTR( origin_instr.tokens[ 0 ].str ) && !tool.isPSEUDO( origin_instr.tokens[ 0 ].str ) ) {
            
            g.addToSymbolTable( label, loc, -1 ) ;
            
        } // if()
        
    } // Packed_instr()
    
    void print() {
        
        cout << tool.decNumToHexStr( loc ) << '\t' << label << '\t' << instr << '\t' ;
        for ( int i = 0 ; i < amount ; i ++ ) {
            
            for ( int j = 0 ; j < token_groups[ i ].size() ; j ++ ) {
                
                cout << token_groups[ i ][ j ].str ;
                
            } // for()
            
            if ( i + 1 < amount ) {
                
                cout << "," << '\t' ;
                
            } // if()
            
        } // for()
        
        // cout << '\t' << "[LENGTH:" << length << "]" << "[Format:" << format << "]" << endl ;
        
        cout << "\t" << machineCode << endl ;
        
    } // print()
    
    void print_packed_instr() {
        
        cout << "LABEL: " << label << endl ;
        cout << "INSTR: " << instr << endl ;
        for ( int i = 0 ; i < amount ; i ++ ) {
            
            cout << "GROUP" << i + 1 << ": " ;
            for ( int j = 0 ; j < token_groups[ i ].size() ; j ++ ) {
                
                cout << token_groups[ i ][ j ].str ;
                
            } // for()
            cout << endl ;
            
        } // for()
        
        cout << endl ;
        
    } // print_packed_instr()
    
} ;

// -------------------------------- data structure for lexical analysis --------------------------------

class Lexical_Analysis {
    
    private :
    
    void printTokens( vector<originInstr> buffer ) {
        
        for ( int i = 0 ; i < buffer.size() ; i ++ ) {
            
            cout << buffer[ i ].str << endl ;
            
            for ( int j = 0 ; j < buffer[ i ].numOfToken ; j ++ ) {
                
                cout << "(" << buffer[ i ].tokens[ j ].table << "," << buffer[ i ].tokens[ j ].no << ")" ;
                
            } // for()
            
            cout << endl ;
            
        } // for()
        
    } // printTokens()
    
    void getToken( vector<originInstr> &buffer_tmp ) {
        
        // assert : all the instruction are put in the buffer
        
        for ( int i = 0 ; i < buffer_tmp.size() ; i ++ ) {
            
            buffer_tmp[ i ].lineNo = i + 1 ;
            buffer_tmp[ i ].tokens = tool.splitInstr( buffer_tmp[ i ].str ) ;
            buffer_tmp[ i ].numOfToken = ( int ) buffer_tmp[ i ].tokens.size() ;
            
            if ( buffer_tmp[ i ].numOfToken == 0 ) { // this is a comment
                
                buffer_tmp[ i ].numOfToken = COMMENT ;
                buffer_tmp[ i ].isComment = true ;
                
            } // if()
            else {
                
                for ( int j = 0 ; j < buffer_tmp[ i ].numOfToken ; j ++ ) {
                    processTable.findTokenInTable( buffer_tmp[ i ].tokens[ j ], buffer_tmp[ i ].tokens[ j ].table, buffer_tmp[ i ].tokens[ j ].no ) ;
                } // for()
                
            } // else()
            
        } // for()
        
        // printTokens() ;
        
    } // getToken()
    
    public :
    
    Lexical_Analysis( vector<originInstr> &originBuffer ) {
        
        getToken( originBuffer ) ;
        
    } // constructor
    
    
} ; // Lexical)Analysis

class ProcessData {
    
    private :
    
    fstream file ;
    
    string fileName ;
    
    public :
    
    bool readFile() {
        
        string fileStr = "" ;
        
        cout << "[Please Enter the file name (Quit:q) ]: " ;
        
        cin >> fileStr ;
        
        file.open( fileStr + ".txt", ios :: in ) ;
        
        while ( !file ) {
            
            if ( fileStr == "q" ) {
                
                return false ;
                
            } // if()
            else {
                
                cout << endl << "### Error: There is no such file "<< fileStr + ".txt" << "! ###" << endl << endl ;
                
                cout << "Please try again!" << endl << endl ;
                
                cout << "[Please Enter the file name (Quit:q) ]: " ;
                
                cin >> fileStr ;
                
                file.open( fileStr + ".txt", ios :: in ) ;
                
            } // else()
            
        } // while()
        
        // assert : we have successfully open the file that we now can start read the information
        
        fileName = fileStr ;
        
        int lineNumber = 0 ; // count which line in the file right now
        string line = "" ; // the whole instruction we saw in the file
        
        while ( getline( file, line ) ) {
            
            // first we have to deal with the whole string, clean the ENTER and find the comment
            string tmpLine = "" ;
            for ( int i = 0 ; i < line.length() && line[ i ] != '\r' ; i ++ ) {
                
                tmpLine += line[ i ] ;
                
            } // for()
            line = tmpLine ;
            
            originInstr tmp ;
            lineNumber ++ ;
            
            tmp.lineNo = lineNumber ;
            tmp.str = line ;
            
            g.originInstrSet.push_back( tmp ) ;
            
        } // while()
        
        file.close() ; // close the file
        
        readTable() ;
        
        return true ;
        
    } // readFile()
    
    void readTable() {
        
        processTable.readFile() ;
        
    } // readTable()
    
    bool saveFile( vector<originInstr> buffer ) {
        
        file.open( "my_" + fileName + "_output.txt", ios :: out ) ;
        
        if ( !file ) {
            
            cout << "### Error: Can't open such file "<< fileName + "_output.txt" << "! ###" << endl << endl ;
            
            return false ;
            
        } // if()
        
        // assert : we successfully open the output file that we now can write data into the file
        
        for ( int i = 0 ; i < buffer.size() ; i ++ ) {
            
            file << buffer[ i ].str << "\n" ; // write the whole instruction first
            
            for ( int j = 0 ; j < buffer[ i ].tokens.size() ; j ++ ) {
                
                if ( buffer[ i ].numOfToken == COMMENT ) {
                    file << buffer[ i ].str ;
                } // if()
                else {
                    file << "(" << buffer[ i ].tokens[ j ].table << "," << buffer[ i ].tokens[ j ].no << ")" ;
                } // else()
                
            } // for()
            
            file << "\n" ;
            
        } // for()
        
        cout << endl << "*** Successful! Create ["<< fileName + "_output.txt] " << "! ***" << endl << endl ;
        
        file.close() ;
        
        processTable.saveFile() ;
        
        return true ;
        
    } // saveFile()
    
    void printBuffer() {
        
        for ( int i = 0 ; i < g.originInstrSet.size() ; i ++ ) {
            
            cout << g.originInstrSet[ i ].str << endl << endl ;
            
        } // for()
        
    } // printBuffer()
    
    string getFileName() {
        
        return fileName ;
        
    } // getFileName()
    
} ; // ProcessData

ProcessData process_data ;

// ------------------------------------------- Syntax analyze -------------------------------------------

# define reg_A 0 // SIC
# define reg_X 1 // SIC
# define reg_L 2 // SIC
# define reg_PC 8 // SIC
# define reg_SW 9 // SIC
# define reg_B 3 // SICXE
# define reg_S 4 // SICXE
# define reg_T 5 // SICXE
# define reg_F 6 // SICXE

class Syntax_Analyze {
    
private:
    
    vector<Packed_instr> pInstrSet ;
    
    int isReg( string str ) {
        
        str = lowerToUp( str ) ;
        
        if ( str == "A" ) {
            
            return reg_A ;
            
        } // if()
        else if ( str == "X" ) {
            
            return reg_X ;
            
        } // else if()
        else if ( str == "L" ) {
            
            return reg_L ;
            
        } // else if()
        else if ( str == "PC" ) {
            
            return reg_PC ;
            
        } // else if()
        else if ( str == "SW" ) {
            
            return reg_SW ;
            
        } // else if()
        else if ( str == "B" ) {
            
            return reg_B ;
            
        } // else if()
        else if ( str == "S" ) {
            
            return reg_S ;
            
        } // else if()
        else if ( str == "T" ) {
            
            return reg_T ;
            
        } // else if()
        else if ( str == "F" ) {
            
            return reg_F ;
            
        } // else if()
        
        return -1 ;
        
    } // isReg()
    
    string getNIXBPE( int format, char firstSign, vector<vector<Token>> tGroup, bool usePC, bool useB ) {
        
        // when calling this function, we can get a string of binary representation
        string str = "" ;
        
        if ( format == SICXE_FORMAT3 || format == SICXE_FORMAT4 ) {
            
            // deal with n
            
            if ( firstSign == '#' ) { // immediate
                
                str += "0" ;
                
            } // if()
            else {
                
                str += "1" ;
                
            } // else
            
            // deal with i
            
            if ( firstSign == '@' ) {
                
                str += "0" ;
                
            } // if()
            else {
                
                str += "1" ;
                
            } // else()
            
            // deal with x
            
            if ( tGroup.size() == 2 && ( tGroup[ 1 ][ 0 ].str == "X" || tGroup[ 1 ][ 0 ].str == "x" ) ) {
                
                str += "1" ;
                
            } // if()
            else {
                
                str += "0" ;
                
            } // else()
            
            // deal with b
            
            if ( useB ) {
                
                str += "1" ;
                
            } // if()
            else {
                
                str += "0" ;
                
            } // else()
            
            // deal with PC
            
            if ( usePC ) {
                
                str += "1" ;
                
            } // if()
            else {
                
                str += "0" ;
                
            } // else()
            
            // deal with e
            
            if ( format == SICXE_FORMAT3 ) {
                
                str += "0" ;
                
            } // if()
            else if ( format == SICXE_FORMAT4 ) {
                
                str += "1" ;
                
            } // else()
            
        } // if()
        else {
            
            cout << "### Error: format wrong when finding n i x b p e! ###" << endl << endl ;
            
        } // else()
        
        return str ;
        
    } // getNIXBPE()
    
    string generateMachineCode( Packed_instr pInstr ) {
        
        // SIC format : opcode  x   address
        // ( 3 byte )   8       1   15
        // SICXE format1 : op
        // ( 1 byte )      8
        // SICXE format2 : op   r1  r2
        // ( 2 byte )       8   4   4
        // SICXE format3 : op   n   i   x   b   p   e   disp
        // ( 3 byte )      6    1   1   1   1   1   1   12
        // SICXE format4 : op   n   i   x   b   p   e   address
        // ( 4 bute )      6    1   1   1   1   1   1   20
        
        string machineCode = "" ;
        string opcode = "" ;
        
        // pseudo code is same with two version
        
        if ( tool.isPSEUDO( pInstr.instr ) ) {
            
            if ( pInstr.instr == "BYTE" || pInstr.instr == "byte" ) {
                
                int tableNo = -1 ;
                int column = -1 ;
                
                processTable.findTokenInTable( pInstr.token_groups[ 0 ][ 1 ], tableNo, column ) ;
                
                if ( tableNo == INT ) {
                    
                    machineCode = pInstr.token_groups[ 0 ][ 1 ].str ;
                    
                } // if()
                else if ( tableNo == STR ) {
                    
                    pInstr.token_groups[ 0 ][ 1 ].str = lowerToUp( pInstr.token_groups[ 0 ][ 1 ].str ) ; // let C'eof' be up
                    
                    for ( int i = 0 ; i < pInstr.token_groups[ 0 ][ 1 ].str.length() ; i ++ ) {
                        
                        machineCode += tool.decNumToHexStr( ( int ) pInstr.token_groups[ 0 ][ 1 ].str[ i ] ) ;
                        
                    } // for()
                    
                } // else if()
                
            } // if()
            else if ( pInstr.instr == "WORD" || pInstr.instr == "word" ) {
                
                // the length must be 6
                
                machineCode = tool.decNumToHexStr( tool.atoi( pInstr.token_groups[ 0 ][ 0 ].str ) ) ;
                
                while ( machineCode.length() < 6 ) {
                    
                    machineCode = "0" + machineCode ;
                    
                } // while()
                
            } // else if()
            else if ( pInstr.instr == "BASE" || pInstr.instr == "base" ) {
                
                for ( int i = 0 ; i < g.regTable.size() ; i ++ ) {
                    
                    if ( g.regTable[ i ].name == "b" ) {
                        
                        int index = g.findInSymTable( pInstr.token_groups[ 0 ][ 0 ].str ) ;
                        
                        if ( index != -1 ) {

                            g.regTable[ i ].content = tool.decNumToHexStr( g.symbolTable[ index ].addr ) ;
                            
                        } // if()
                        else {

                            g.regTable[ i ].content = pInstr.token_groups[ 0 ][ 0 ].str ;
                            
                        } // else()
                        
                    } // if()
                    
                } // for()
                
            } // else if()
            
            return machineCode ;
            
        } // if()
        
        if ( pInstr.format == SIC_FORMAT && g.version == "SIC" ) {
            
            if ( tool.isINSTR( pInstr.instr ) ) {
                
                opcode = tool.hexStrToBinaryStr( g.isd.getOpcode( pInstr.instr ) ) ;
                
                while ( opcode.length() < 8 ) {
                    
                    opcode = "0" + opcode ;
                    
                } // while()
                
                machineCode += opcode ;
                
            } // if()
            
            // assert : opcode is finish, then we try to find wether used X
            
            bool hasX = false ;
            for ( int i = 0 ; i < pInstr.token_groups.size() ; i ++ ) {
                
                if ( pInstr.token_groups[ i ].size() == 1 ) {
                    
                    if ( pInstr.token_groups[ i ][ 0 ].str == "X" || pInstr.token_groups[ i ][ 0 ].str == "x" ) {
                        
                        hasX = true ;
                        
                    } // if()
                    
                } // if()
                
            } // for()
            
            if ( hasX ) {
                
                machineCode += "1" ;
                
            } // if()
            else {
                
                machineCode += "0" ;
                
            } // else()
            
            // assert : x is finish, now is turn to the address
            
            // first find the address of the first operand ( hex )
            
            // second hex -> binary
            
            string tmpAddr = "" ;
            
            if ( pInstr.token_groups.size() == 0 ) { // there is no operands
                
                tmpAddr = "0000" ;
                
            } // if()
            else if ( pInstr.token_groups.size() == 1 || pInstr.token_groups.size() == 2 ) {
                 
                int litIndex = ( int ) pInstr.token_groups.size() - 1 ;
                string tmpLitName = "" ;
                bool isFind = false ;
                
                if ( pInstr.token_groups[ litIndex ][ 0 ].str == "=" ) {
                    
                    if ( pInstr.token_groups[ litIndex ].size() == 2 ) {
                        tmpLitName = pInstr.token_groups[ litIndex ][ 1 ].str ;
                    } // if()
                    else {
                        tmpLitName = pInstr.token_groups[ litIndex ][ 2 ].str ;
                    } // else()
                    
                    for ( int i = 0 ; i < 100 && !isFind ; i ++ ) {
                        
                        if ( tmpLitName == g.literalTable[ i ].name ) {
                            
                            tmpAddr = tool.decNumToHexStr( g.literalTable[ i ].addr ) ;
                            
                        } // if()
                        
                    } // for()
                    
                } // if()
                else { // this is a symbol
                    
                    tmpLitName = pInstr.token_groups[ 0 ][ 0 ].str ;
                    
                    int indexInSymTable = g.findInSymTable( tmpLitName ) ;
                    
                    if ( indexInSymTable != -1 ) { // truely exist
                        
                        tmpAddr = tool.decNumToHexStr( g.symbolTable[ indexInSymTable ].addr ) ;
                        
                    } // if()
                    
                } // else()
                
            } // else if()
            else if ( pInstr.token_groups.size() == 2 ) {
                
                if ( pInstr.token_groups[ 1 ][ 0 ].str == "=" ) {
                    
                    string tmpLitName = "" ;
                    if ( pInstr.token_groups[ 1 ].size() == 2 ) {
                        tmpLitName = pInstr.token_groups[ 1 ][ 1 ].str ;
                    } // if()
                    else {
                        tmpLitName = pInstr.token_groups[ 1 ][ 2 ].str ;
                    } // else()
                    
                    bool isFind = false ;
                    for ( int i = 0 ; i < 100 && !isFind ; i ++ ) {
                        
                        if ( tmpLitName == g.literalTable[ i ].name ) {
                            
                            tmpAddr = tool.decNumToHexStr( g.literalTable[ i ].addr ) ;
                            
                        } // if()
                        
                    } // for()
                    
                } // if()
                
            } // else if()
            else {
                
                tmpAddr = tool.decNumToHexStr( g.symbolTable[ pInstr.token_groups[ 0 ][ 0 ].no - 1 ].addr ) ;
                
            } // else()
            
            if ( tmpAddr == "" ) { // doesn't found the label
                
                cout << "### Error: symble[ " << pInstr.token_groups[ 0 ][ 0 ].str << " ] does not defined! ###" << endl ;
                
                return "" ;
                
            } // if()
            
            // remove the first char
            string tmp = "" ;
            tmpAddr = tool.hexStrToBinaryStr( tmpAddr ) ;
            for ( int i = 1 ; i < tmpAddr.length() ; i ++ ) {
                
                tmp += tmpAddr[ i ] ;
                
            } // for()
            
            machineCode += tmp ;
            
        } // if()
        else if ( pInstr.format == SICXE_FORMAT1 ) {
            
            opcode = tool.hexStrToBinaryStr( g.isd.getOpcode( pInstr.instr ) ) ;
            
            while ( opcode.length() != 8 ) {
                
                opcode = "0" + opcode ;
                
            } // while()
            
            machineCode = opcode ;
            
        } // else if()
        else if ( pInstr.format == SICXE_FORMAT2 ) {
            
            opcode = tool.hexStrToBinaryStr( g.isd.getOpcode( pInstr.instr ) ) ;
            
            while ( opcode.length() != 8 ) {
                
                opcode = "0" + opcode ;
                
            } // while()
            
            machineCode = opcode ;
            
            // assert : now we need to put in r1 and r2
            
            string r1 = "" ;
            string r2 = "" ;
            
            if ( isReg( pInstr.token_groups[ 0 ][ 0 ].str ) == -1 || ( pInstr.token_groups.size() > 1 && isReg( pInstr.token_groups[ 1 ][ 0 ].str ) == -1 ) ) {
                
                // first deal with r1
                
                for ( int i = 0 ; i < pInstrSet.size() && r1 == "" ; i ++ ) {
                    
                    if ( pInstr.token_groups.size() == 0 ) { // there is no operands
                        
                        r1 = "0" ;
                        
                    } // if()
                    else if ( pInstr.token_groups[ 0 ][ 0 ].str == pInstrSet[ i ].label ) {
                        
                        r1 = tool.decNumToHexStr( pInstrSet[ i ].loc ) ;
                        
                        if ( r1.length() != 1 ) {
                            
                            cout << "### Error: format2 r1 corrupt! ###" << endl << endl ;
                            
                        } // if()
                        
                    } // else if()
                    
                } // for()
                
                if ( r1 == "" ) { // doesn't found the label
                    
                    if ( tool.atoi( pInstr.token_groups[ 0 ][ 0 ].str ) != -1 ) {
                        
                        r1 = pInstr.token_groups[ 0 ][ 0 ].str ;
                        
                    } // if()
                    else {
                        

                        cout << "### Error: symble[ " << pInstr.token_groups[ 0 ][ 0 ].str << " ] does not defined! ###" << endl ;
                        
                        return "" ;
                        
                    } // else()
                    
                } // if()
                
                // second deal with r2
                
                for ( int i = 0 ; i < pInstrSet.size() && r2 == "" ; i ++ ) {
                    
                    if ( pInstr.token_groups.size() == 0 ) { // there is no operands
                        
                        r2 = "0" ;
                        
                    } // if()
                    else if ( pInstr.token_groups.size() > 1 && pInstr.token_groups[ 1 ][ 0 ].str == pInstrSet[ i ].label ) {
                        
                        r2 = tool.decNumToHexStr( pInstrSet[ i ].loc ) ;
                        
                        if ( r2.length() != 1 ) {
                            
                            cout << "### Error: format2 r1 corrupt! ###" << endl << endl ;
                            
                        } // if()
                        
                    } // else if()
                    
                } // for()
                
                if ( pInstr.amount > 1 && r2 == "" ) { // doesn't found the label
                    
                    cout << "### Error: symble[ " << pInstr.token_groups[ 1 ][ 0 ].str << " ] does not defined! ###" << endl ;
                    
                    return "" ;
                    
                } // if()
                
            } // if()
            else {
                
                if ( isReg( pInstr.token_groups[ 0 ][ 0 ].str ) != -1 || isReg( pInstr.token_groups[ 1 ][ 0 ].str ) != -1 ) {
                    // both register
                    if ( isReg( pInstr.token_groups[ 0 ][ 0 ].str ) != -1 ) {
                        
                        r1 = tool.itoa( isReg( pInstr.token_groups[ 0 ][ 0 ].str ) ) ;
                        
                    } // if()
                    
                    if ( pInstr.token_groups.size() > 1 ) {
                        
                        if ( ( isReg( pInstr.token_groups[ 1 ][ 0 ].str ) != -1 ) ) {
                            
                            r2 = tool.itoa( isReg( pInstr.token_groups[ 1 ][ 0 ].str ) ) ;
                            
                        } // if()
                        
                    } // if()
                    
                } // if()
                else {
                    
                    // first deal with r1
                    
                    for ( int i = 0 ; i < pInstrSet.size() && r1 == "" ; i ++ ) {
                        
                        if ( pInstr.token_groups.size() == 0 ) { // there is no operands
                            
                            r1 = "0" ;
                            
                        } // if()
                        else if ( pInstr.token_groups[ 0 ][ 0 ].str == pInstrSet[ i ].label ) {
                            
                            r1 = tool.decNumToHexStr( pInstrSet[ i ].loc ) ;
                            
                            if ( r1.length() != 1 ) {
                                
                                cout << "### Error: format2 r1 corrupt! ###" << endl << endl ;
                                
                            } // if()
                            
                        } // else if()
                        
                    } // for()
                    
                    if ( r1 == "" ) { // doesn't found the label
                        
                        cout << "### Error: symble[ " << pInstr.token_groups[ 0 ][ 0 ].str << " ] does not defined! ###" << endl ;
                        
                        return "" ;
                        
                    } // if()
                    
                    // deal with r2
                    
                    for ( int i = 0 ; i < pInstrSet.size() && r2 == "" ; i ++ ) {
                        
                        if ( pInstr.token_groups.size() == 0 ) { // there is no operands
                            
                            r2 = "0" ;
                            
                        } // if()
                        else if ( pInstr.token_groups[ 1 ][ 0 ].str == pInstrSet[ i ].label ) {
                            
                            r2 = tool.decNumToHexStr( pInstrSet[ i ].loc ) ;
                            
                            if ( r2.length() != 1 ) {
                                
                                cout << "### Error: format2 r1 corrupt! ###" << endl << endl ;
                                
                            } // if()
                            
                        } // else if()
                        
                    } // for()
                    
                    if ( r2 == "" ) { // doesn't found the label, that means r2 may be a constant
                        
                        int table = -1 ;
                        int useless = -1 ;
                        
                        if ( pInstr.token_groups[ 1 ].size() == 1 ) { // is not X'a1'
                            
                            processTable.findTokenInTable( pInstr.token_groups[ 1 ][ 0 ], table, useless ) ;
                            
                        } // if()
                        else {
                            
                            processTable.findTokenInTable( pInstr.token_groups[ 1 ][ 1 ], table, useless ) ;
                            
                        } // else()
                        
                        
                        if ( table == INT ) {
                            
                            if ( pInstr.token_groups[ 1 ].size() == 1 ) { // is not X'a1'
                                
                                if ( pInstr.token_groups[ 1 ][ 0 ].str.length() == 1 ) {
                                    
                                    r2 = pInstr.token_groups[ 1 ][ 0 ].str ;
                                    
                                } // if()
                                else {
                                    
                                    cout << "### Error: format2 r2 corrupt! ###" << endl << endl ;
                                    
                                } // else()
                                
                            } // if()
                            else {
                                
                                if ( pInstr.token_groups[ 1 ][ 1 ].str.length() == 1 ) {
                                    
                                    r2 = pInstr.token_groups[ 1 ][ 1 ].str ;
                                    
                                } // if()
                                else {
                                    
                                    cout << "### Error: format2 r2 corrupt! ###" << endl << endl ;
                                    
                                } // else()
                                
                            } // else()
                            
                        } // if()
                        else {
                            
                            cout << "### Error: format2 r2 corrupt! ###" << endl << endl ;
                            
                        } // else()
                        
                    } // if()
                    
                } // else()
                
            } // else()
            
            if ( r1 == "" ) r1 = "0" ;
            
            if ( r2 == "" ) r2 = "0" ;
            
            machineCode = machineCode + tool.hexStrToBinaryStr( r1 ) + tool.hexStrToBinaryStr( r2 ) ;
            
        } // else if()
        else if ( pInstr.format == SICXE_FORMAT3 || pInstr.format == SICXE_FORMAT4 ) {
            
            int addr1 = 0 ;
            int addr2 = 0 ;
            string disp = "" ;
            
            opcode = tool.hexStrToBinaryStr( g.isd.getOpcode( pInstr.instr ) ) ;
            
            while ( opcode.length() != 6 ) {
                
                if ( opcode.length() < 6 ) {
                    
                    opcode = "0" + opcode ;
                    
                } // if()
                else if ( opcode.length() > 6 ) {
                    
                    string tmp = "" ;
                    for ( int i = 0 ; i < 6 ; i ++ ) {
                        tmp += opcode[ i ] ;
                    } // for()
                    
                    opcode = tmp ;
                    
                } // else if()
                
            } // while()
            
            machineCode = opcode ;
            
            // assert : now deal with the n i x b p e
            if ( pInstr.token_groups.size() == 2 ) {
                
                machineCode += getNIXBPE( pInstr.format, pInstr.token_groups[ 0 ][ 0 ].str[ 0 ], pInstr.token_groups, false, true ) ;
                
                // computate the disp
                int index = g.findInSymTable( pInstr.token_groups[ 0 ][ 0 ].str ) ;
                int index2 = g.findInSymTable( pInstr.token_groups[ 1 ][ 0 ].str ) ;
                
                if ( tool.isREG( pInstr.token_groups[ 0 ][ 0 ].str ) || tool.isREG( pInstr.token_groups[ 1 ][ 0 ].str ) || index != -1 || index2 != -1 ) {
                    
                    int regIndex = -1 ;
                    for ( int i = 0 ; i < g.regTable.size() && regIndex == -1 ; i ++ ) {
                        
                        if ( g.regTable[ i ].name == "b" ) regIndex = i ;
                        
                    } // for()
                    
                    if ( tool.isREG( pInstr.token_groups[ 0 ][ 0 ].str ) ) {
                        
                        addr1 = tool.hexStrToDecNum( g.regTable[ regIndex ].content ) ;
                        
                    } // if()
                    else {
                        
                        addr1 = g.symbolTable[ index ].addr ;
                        
                    } // else()
                    
                    if ( tool.isREG( pInstr.token_groups[ 1 ][ 0 ].str ) ) {
                        
                        addr2 = tool.hexStrToDecNum( g.regTable[ regIndex ].content ) ;
                        
                    } // if()
                    else {
                        
                        addr2 = g.symbolTable[ index ].addr ;
                        
                    } // else()
                    
                } // if()
                else {
                    
                    if ( pInstr.token_groups[ 0 ][ 0 ].str == g.symbolTable[ pInstr.token_groups[ 0 ][ 0 ].no ].name ) {
                        
                        addr1 = g.symbolTable[ pInstr.token_groups[ 0 ][ 0 ].no ].addr ;
                        
                    } // if()
                    
                    if ( pInstr.token_groups[ 1 ][ 0 ].str == g.symbolTable[ pInstr.token_groups[ 1 ][ 0 ].no ].name ) {
                        
                        addr2 = g.symbolTable[ pInstr.token_groups[ 1 ][ 0 ].no ].addr ;
                        
                    } // if()

                    
                    if ( addr1 == 0 || addr2 == 0 ) {
                        
                        cout << "### Error: didn't find the symbol! ###" << endl ;
                        
                    } // else()
                    
                } // else()
                
            } // if()
            else { // here is only one operand
                
                if ( pInstr.token_groups.size() != 0 && pInstr.token_groups[ 0 ][ 0 ].str[ 0 ] == '#' ) {
                    
                    string tmpStr = "" ; // remove the '#'
                    for ( int i = 0 ; i < pInstr.token_groups[ 0 ][ 1 ].str.length() ; i ++ ) {
                        tmpStr += pInstr.token_groups[ 0 ][ 1 ].str[ i ] ;
                    } // for()
                    
                    int indexInSymTable = g.findInSymTable( tmpStr ) ;
                    
                    if ( pInstr.token_groups.size() != 0 && indexInSymTable != -1 ) {
                        
                        addr1 = g.symbolTable[ indexInSymTable ].addr ;
                        
                        addr2 = pInstr.loc + pInstr.length ;
                        
                    } // if()
                    
                    if ( indexInSymTable != -1 ) {
                        
                        if ( pInstr.format == SICXE_FORMAT3 ) {
                            
                            machineCode += getNIXBPE( pInstr.format, pInstr.token_groups[ 0 ][ 0 ].str[ 0 ], pInstr.token_groups, true, false ) ;
                            
                        } // if()
                        else {
                            
                            machineCode += getNIXBPE( pInstr.format, pInstr.token_groups[ 0 ][ 0 ].str[ 0 ], pInstr.token_groups, false, false ) ;
                            
                        } // else()
                        
                    } // if()
                    else {
                        
                        machineCode += getNIXBPE( pInstr.format, pInstr.token_groups[ 0 ][ 0 ].str[ 0 ], pInstr.token_groups, false, false ) ;
                        
                        addr1 = tool.atoi( tmpStr ) ;
                        
                        addr2 = 0 ;
                        
                    } // else()
                    
                } // if()
                else if ( pInstr.token_groups.size() != 0 && pInstr.token_groups[ 0 ][ 0 ].str == "@" ) {
                    
                    //  pInstr.token_groups[ 0 ][ 1 ] must be a symbol
                    string tmpStr = pInstr.token_groups[ 0 ][ 1 ].str ;
                    
                    int indexInSymTable = g.findInSymTable( tmpStr ) ;
                    
                    if ( pInstr.token_groups.size() != 0 && indexInSymTable != -1 ) {
                        
                        addr1 = g.symbolTable[ indexInSymTable ].addr ;
                        
                    } // if()
                    
                    machineCode += getNIXBPE( pInstr.format, pInstr.token_groups[ 0 ][ 0 ].str[ 0 ], pInstr.token_groups, true, false ) ;
                    
                    addr2 = pInstr.loc + pInstr.length ;
                    
                } // else if()
                else if ( pInstr.token_groups.size() != 0 && pInstr.token_groups[ 0 ][ 0 ].str == "=" ) {
                    
                    if ( pInstr.instr[ pInstr.instr.length() - 1 ] == 'b' || pInstr.instr[ pInstr.instr.length() - 1 ] == 'B' ) {
                        
                        addr2 = tool.atoi( g.regTable[ 4 ].content ) ; // program counter
                        
                    } // if()
                    else {
                        
                        addr2 = pInstr.loc + pInstr.length ; // program counter
                        
                    } // else()
                    
                    string tmpLitName = "" ;
                    
                    if ( pInstr.token_groups[ 0 ].size() == 2 ) { // = 3227
                        
                        tmpLitName = pInstr.token_groups[ 0 ][ 1 ].str ;
                        
                    } // if()
                    else {
                        
                        tmpLitName = pInstr.token_groups[ 0 ][ 2 ].str ;
                        
                    } // else()
                    
                    int indexInLitTable = g.findInLitTable( tmpLitName ) ;
                    
                    if ( indexInLitTable != -1 ) {
                        
                        addr1 = g.literalTable[ indexInLitTable ].addr ;
                        
                    } // if()
                    
                    machineCode += getNIXBPE( pInstr.format, pInstr.token_groups[ 0 ][ 0 ].str[ 0 ], pInstr.token_groups, true, false ) ;
                    
                } // else if()
                else { // the normal one count the displacement with the progeam counter
                    
                    if ( pInstr.format == SICXE_FORMAT3 && pInstr.token_groups.size() != 0 && lowerToUp( pInstr.instr ) != "COMP"
                        && lowerToUp( pInstr.instr ) != "LDCH" && lowerToUp( pInstr.instr ) != "LDF" && lowerToUp( pInstr.instr ) != "LDL"
                        && lowerToUp( pInstr.instr ) != "MUL" && lowerToUp( pInstr.instr ) != "WD" ) { // try
                        
                        machineCode += getNIXBPE( pInstr.format, pInstr.token_groups[ 0 ][ 0 ].str[ 0 ], pInstr.token_groups, true, false ) ;
                        
                        addr2 = pInstr.loc + pInstr.length ; // program counter
                        
                    } // if()
                    else if ( pInstr.token_groups.size() != 0 ) {
                        
                        machineCode += getNIXBPE( pInstr.format, pInstr.token_groups[ 0 ][ 0 ].str[ 0 ], pInstr.token_groups, false, false ) ;
                        
                    } // else()
                    else { // has no operand
                        
                        machineCode += getNIXBPE( pInstr.format, '\0', pInstr.token_groups, false, false ) ;
                        
                    } // else()
                    
                    if ( pInstr.token_groups.size() != 0 ) { // has at least one operand
                        
                        int indexInSymTable = g.findInSymTable( pInstr.token_groups[ 0 ][ 0 ].str ) ;
                        
                        if ( indexInSymTable != -1 ) {
                            
                            addr1 = g.symbolTable[ indexInSymTable ].addr ;
                            
                        } // if()
                        else {
                            
                            addr1 = tool.atoi( pInstr.token_groups[ 0 ][ 0 ].str ) ;
                            
                        } // else()
                        
                    } // if()
                    else { // no operand
                        
                        addr1 = 0 ;
                        
                    } // else()
                    
                    if ( addr1 == -1 ) { // may be an integer
                        
                        if ( pInstr.token_groups[ 0 ][ 0 ].table == INT ) {
                            
                            addr1 = tool.atoi( pInstr.token_groups[ 0 ][ 0 ].str ) ;
                            
                        } // if()
                        else {
                            
                            cout << "### Error: fail to find symbol! ###" << endl << endl ;
                            
                            return "" ;
                            
                        } // else()
                        
                    } // if()
                    
                } // else()
                
            } // else()
            
            // assert : now put in the 12 bit disp
            
            disp = tool.binarySub2Bin( tool.decNumToHexStr( addr1 ), tool.decNumToHexStr( addr2 ) ) ;
            
            int maxDispLen = 0 ;
            if ( pInstr.format == SICXE_FORMAT3 ) {
                
                maxDispLen = 12 ;
                
            } // if()
            else if ( pInstr.format == SICXE_FORMAT4 ) {
                
                maxDispLen = 20 ;
                
            } // else if()
            
            if ( disp.length() != maxDispLen ) {
            
                while ( disp.length() < maxDispLen ) {
                    
                    if ( addr1 - addr2 >= 0 ) {

                        disp = "0" + disp ;
                        
                    } // if()
                    else {
                        
                        disp = "1" + disp ;
                        
                    } // else()
                    
                } // while()
                
                if ( disp.length() > maxDispLen ) {
                        
                    string tmp = "" ;
                    for ( int tmpCount = 0 ; tmpCount < maxDispLen ; tmpCount ++ ) {
                        
                        tmp = disp[ disp.length() - tmpCount - 1 ] + tmp ;
                        
                    } // for()
                    
                    disp = tmp ;
                    
                } // if()
                
            } // if()
            
            machineCode += disp ;
            
        } // else if()
        else if ( pInstr.instr[ 0 ] == '=' ) {
            
            if ( pInstr.token_groups[ 0 ].size() == 2 ) {
                
                // = 3277
                machineCode = pInstr.token_groups[ 0 ][ 1 ].str ;
                
            } // if()
            else {
                
                if ( pInstr.token_groups[ 0 ][ 2 ].table == INT ) {
                    
                    // X'F1'
                    
                    for ( int count = 0 ; count < pInstr.token_groups[ 0 ][ 2 ].str.length() ; count ++ ) {
                        
                        machineCode += pInstr.token_groups[ 0 ][ 2 ].str[ count ] ;
                        
                    } // for()
                    
                    
                } // if()
                else if ( pInstr.token_groups[ 0 ][ 2 ].table == STR ) {
                    
                    for ( int count = 0 ; count < pInstr.token_groups[ 0 ][ 2 ].str.length() ; count ++ ) {
                        
                        string tmp = tool.decNumToHexStr( (int ) pInstr.token_groups[ 0 ][ 2 ].str[ count ] ) ;
                        machineCode += tmp ;
                        
                        tmp = "" ;
                        
                    } // for()
                    
                } // else if()
                    
            } // else()
            
            return machineCode ;
            
        } // else if()
        
        machineCode = tool.BinaryStrToHexStr( machineCode ) ;
        
        return  machineCode ;
        
    } // generateMachineCode
    
public :
    
    Syntax_Analyze( string verName ) {
        
        if ( verName == "SIC" ) {
            
            g.version = verName ;
            
        } // if()
        else if ( verName == "SICXE" ) {
            
            g.version = verName ;
            
        } // else if()
        else {
            
            cout << "### Error: wrong version, should be [SIC] or [SICXE]! ###" << endl << endl ;
            
        } // else()
        
        // start to pack the instruction
        
        for ( int i = 0 ; i < g.originInstrSet.size() ; i ++ ) {
            
            Packed_instr tmp ;
            tmp.packed( g.originInstrSet[ i ] ) ;
            pInstrSet.push_back( tmp ) ;
            
            if ( tmp.token_groups.size() != 0 && tmp.token_groups[ 0 ][ 0 ].str == "=" ) { // need to deal with the literal
                
                g.addToLiteralTable( tmp.token_groups ) ;
                /*
                if ( pInstrSet[ i ].token_groups[ 0 ][ 2 ].table == STR ) {
                    
                    pInstrSet[ i ].token_groups[ 0 ][ 2 ].str = lowerToUp( pInstrSet[ i ].token_groups[ 0 ][ 2 ].str ) ;
                    
                } // if()
                */
            } // if()
            
            if ( tmp.instr == "LTORG" || tmp.instr == "ltorg" || tmp.instr == "END" || tmp.instr == "end" ) { // put the literal in
                
                int tableNo = -1 ;
                int useless = -1 ;
                
                tmp.label = "*" ;
                tmp.instr = "" ;
                vector<literalInfo> tmpLitSet ;
                
                tmpLitSet = g.addressLiteral() ;
                
                for ( int j = 0 ; j < tmpLitSet.size() ; j ++ ) {
                    
                    tmp.loc = g.locctr ;
//
                    int index = g.findInLitTable( tmpLitSet[ j ].name ) ;
                    if ( index != -1 ) {
                        
                        g.literalTable[ index ].addr = g.locctr ;
                        
                    } // if()
                    
                    tmp.token_groups = tmpLitSet[ j ].tokenGroup ;
                    
                    for ( int k = 0 ; k < tmp.token_groups[ 0 ].size() ; k ++ ) {
                        
                        tmp.instr += tmp.token_groups[ 0 ][ k ].str ;
                        
                    } // for()
                    
                    int num = -1 ;
                    if ( tmp.token_groups.size() > 2 ) {
                        
                        num = 1 ;
                        
                    } // if()
                    else {
                        
                        num = 2 ;
                        
                    } // else()
                    processTable.findTokenInTable( tmp.token_groups[ 0 ][ num ], tableNo, useless ) ;
                    
                    if ( tableNo == INT ) {
                        
                        tmp.length = ( int ) tmp.token_groups[ 0 ][ num ].str.length() / 2 ;
                        
                    } // if()
                    else if ( tableNo == STR ) {
                        
                        tmp.length = ( int ) tmp.token_groups[ 0 ][ num ].str.length() ;
                        
                    } // else if()
                    
                    pInstrSet.push_back( tmp ) ;
                    
                    g.locctr += tmp.length ;
                    
                    tmp.instr = "" ;
                    
                } // for()
                
            } // if()
            
        } // for()
        
    } // constructor
    
    void pass2() {
        
        for ( int i = 0 ; i < pInstrSet.size() ; i ++ ) {
            
            pInstrSet[ i ].machineCode = generateMachineCode( pInstrSet[ i ] ) ;
            
            pInstrSet[ i ].print() ; // test
            
        } // for()
        
    } // pass2()
    
    void saveMachineCodeFile() {
        
        fstream file ;
        
        string outputFileName = "my_" + process_data.getFileName() + "_output.txt" ;
        
        file.open( outputFileName, ios :: out ) ;
        
        if ( !file ) {
            
            cout << "### Error: file to open the [" << outputFileName << "]! ###" << endl << endl ;
            
        } // if()
        else {
            
            file << setw( 4 ) << "Line" << setw( 6 ) << "Loc" << setw( 20 ) << "Source statement" << setw( 26 ) << "Object code" << endl ;
            file << "----  -----  ---------------------------    ---------------" << endl ;
            
            int lineNo = 0 ;
            for ( int i = 0 ; i < pInstrSet.size() ; i ++ ) {
                
                lineNo += 5 ;
                
                file << setw( 4 ) << lineNo ;
                
                if ( pInstrSet[ i ].amount == -1 ) { // this line is a comment
                    
                    file << setw( 36 ) << pInstrSet[ i ].label << endl ;
                    
                } // if()
                else {
                    
                    if ( pInstrSet[ i ].instr != "END" && pInstrSet[ i ].instr != "end" ) {
                        
                        string locStr = "" ;
                        
                        locStr = tool.decNumToHexStr( pInstrSet[ i ].loc ) ;
                        
                        while ( locStr.length() < 4 ) {
                            
                            locStr = "0" + locStr ;
                            
                        } // while()
                        
                        file << setw( 6 ) << locStr ;
                        
                    } // if()
                    else {
                        
                        file << setw( 6 ) << "\t" ;
                        
                    } // else()
                    
                    file << setw( 4 )  << "\t" << pInstrSet[ i ].label << setw( 6 )  << "\t" ; // label
                    if ( pInstrSet[ i ].label == "" ) file << setw( 4 ) << "\t" ;
                    
                    if ( pInstrSet[ i ].format == SICXE_FORMAT4 ) {
                        
                        file << "+" ;
                        
                    } // if()
                    
                    file << pInstrSet[ i ].instr << setw( 6 ) << '\t' ;
                    for ( int tokenNo = 0 ; tokenNo < pInstrSet[ i ].amount && pInstrSet[ i ].label != "*" ; tokenNo ++ ) {
                        
                        for ( int j = 0 ; j < pInstrSet[ i ].token_groups[ tokenNo ].size() ; j ++ ) {
                            
                            if ( pInstrSet[ i ].token_groups[ tokenNo ].size() != 1 && j == 0
                                && pInstrSet[ i ].token_groups[ tokenNo ][ j ].str[ 0 ] == '\'' ) {
                                
                                int table = -1 ;
                                int useless = -1 ;
                                
                                processTable.findTokenInTable( pInstrSet[ i ].token_groups[ tokenNo ][ 1 ], table, useless ) ;
                                
                                if ( table == INT ) {
                                    
                                    file << "X" ;
                                    
                                } // if()
                                else if ( table == STR ) {
                                    
                                    file << "C" ;
                                    
                                } // else if()
                                
                            } // if()
                            
                            file << pInstrSet[ i ].token_groups[ tokenNo ][ j ].str ;
                            
                        } // for()
                        
                        if ( tokenNo + 1 < pInstrSet[ i ].amount ) {
                            
                            file << "," ;
                            
                        } // if()
                        
                    } // for()
                    
                    file << setw( 6 ) << "\t" << pInstrSet[ i ].machineCode << endl ;
                    
                } // else()
                
            } // for()
            
        } // else()
        
        file.close() ;
        
        cout << "*** Successlfully create [" << outputFileName << "]! ***" << endl << endl ;
        
    } // saveMachineCodeFile)_
    
} ; // Syntax_Analyze

// ------------------------------------------- Syntax analyze -------------------------------------------

string getOption() {
    
    string command = "" ;
    
    cout << endl << "Please choose which version (1) SIC (2) SICXE : " ;
    cin >> command ;
    cout << endl ;
    
    while ( command != "1" && command != "2" ) {
        
        cout << "### Please enter (1) or (2)! ###" << endl << endl ;
        cout << "Please choose which version (1) SIC (2) SICXE : " ;
        cin >> command ;
        
    } // while()
    
    if ( command == "1" ) command = "SIC" ;
    else if ( command == "2" ) command = "SICXE" ;
    
    return command ;
    
} // showOption()

int main() {
    
    if ( process_data.readFile() ) {
        
        // process_data.printBuffer() ;
        string version = getOption() ;
        Lexical_Analysis lex_analysis( g.originInstrSet ) ;
        Syntax_Analyze syn_analysis( version ) ;
        syn_analysis.pass2() ;
        syn_analysis.saveMachineCodeFile() ;
        // process_data.saveFile( process_data.buffer ) ;
        
    } // if()
    
} // main()

