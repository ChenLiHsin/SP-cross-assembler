# SP cross assembler
 cross assembler deal with SIC/SICXE

系統程式 程式作業二 10627122陳俐欣

1.開發平台
>> macOS catalina

2.使用開發環境
>> xcode

3.所選擇的組合語言( x86, SIC )
>> SIC/SICXE

4.說明你的程式設計(功能，流程，使用的data structure)
>> (1) 功能：
			最基本的切token、判斷token的屬性、檢查程式碼是否有錯、
			Literal的處理、pseudo code ( START, END, BYRE, WORD, RESB, RESW, LTORG, BASE)的處理

>> (2) 流程：
			// Lexical analysis
			step1.利用前一支程式作業的功能，將input檔讀入，並切出token。

			// Syntax analysis
			step2.利用已經切好的token進行包裝的動作，判斷該token是label、ins		truction、operand等

			step3.由新包裝好的指令，一行一行的進行細部處理 

					Ex1:若該指令影出現label則將此label hash進symbol table並將上此指令的地址，如此若之後有指令呼叫到此label就可以迅速地找到該地址)

					Ex2:若此指令為pseudo instruction則依照此pseudo instruction的指令做處理

					Ex3:可由此指令中的operand數檢查是否符合此指令的要求

			// machine code generation
			step4.在生成機器碼時，我分成幾個步驟，先由該指令的instruction得到opcode，將此opcode的長度依照此指令的格式進行調整，再來若是SIC則判斷是否用到X暫存器並將"0"或"1"接續放在剛剛的opcode中，			若是SICXE則判斷若是format3,4則近一步的尋找nixbpe的對應條件是否符合，經過此尋找階段獲得的機器碼再接上剛剛的部分machinecode，最後在尋找這條指令所需要的displacement或是address

			step5.將最後整理好且找到機器碼的結果寫檔

	(3) data structure

		// 存放一些共通的資料
		class Global {
			vector<reg> regTable ; // 儲存暫存器的資料（目前的內容）
    		string version ; // 記錄現在是在進行SIC或是SICXE
   			int locctr ; // 記錄現在的指令位址
    		Instruction_Set_Detail isd ; // 存放指令的相關資料(opcode為何)   
    		vector<originInstr> originInstrSet ; // 儲存原始的指令資料
    		vector<string> instrSet, pseudoSet, regSet, delimiterSet ; // 存放從table1~table4的資料   
    		vector<tableInfo> buffer_table_1to4[ 4 ] ;
    		tableInfo buffer_table_5to7[ 100 ][ 3 ] ;    
    		symbolInfo symbolTable[100] ;  // 存放symbol的相關資料（名稱和地址）
    		literalInfo literalTable[100] ; // 存放literal的相關資料(名稱和地址和是否已給地址)

    		void addToSymbolTable() ; // 將新發現的symbol加入symbol table中
    		void addToLiteralTable() ; // 將新發現的literal加入literal table中
    		vector<literalInfo> addressLiteral() ; // 遇到LTORG或是END時將literal中尚未處理的literal定址
		}

		// 一些處理資料時會用倒的工具（數字字串轉換，進位制的轉換）
		class tool {
			void setTableContent() ;
			bool isSpace() ; // 判斷是否為空白
			bool isINSTR() ; // 判斷是否為指令
			bool isPSEUDO() ; // 判斷是否為虛擬指令
			bool isREG() ; // 判斷是否為暫存器
			bool isDELIMITER() ; // 判斷是否為符號
			bool isINT() ; // 判斷是否為數字
			bool isSTR() ; // 判斷是否為字串
			int classifyToken() ; // 判斷此token為什麽
			string itoa() ; // 數字轉成字串
			int atoi() ; // 字串轉成數字
			void skipSpace() ; // 跳過空白
			vector<Token> splitInstr() ; // 將指令切開
			string decNumToHexStr() ; // 十進位數字轉成十六進位字串
			string BinaryStrToHexStr() ; // 二進位字串轉成十六進位字串
			string hexToBin() ; // 十六進位字元轉成二進位字串
			string hexStrToBinaryStr() ; // 十六進位字串轉成二進位字串
			int hexStrToDecNum() ; // 十六進位字串轉成十進位數字
			string binarySub2Bin() ; // 二進位減法
		}

		// 處理基本的table資訊
		class ProcessTable {
			void findTokenInTable() ; // 在table中尋找token的位址
			void addToTable() ;
			bool readFile() ;
			bool saveFile() ;
		}

		// 讀入input檔，初步處理資料
		class ProcessData {
			bool readFile() ;
			void readTable() ;
			bool saveFile() ;
		}

		// 將出補處例好的資料（指令）做進一步的整理
		struct Packed_instr {
			string label ; // the label of this instruction
    		string instr ; // the instruction token of this statement
   			int amount ; // the amount of the token groups
    		vector<vector<Token>> token_groups ;
    		int loc ; // where is the address of this instruction
    		int format ; // which format is this instruciton belongs
    		int length ; // 此指令的長度
    		string machineCode ; // 此指令的機器碼

    		void packed() ; // 將 originInstr 包裝
		} 

		// 切token
		class Lexical_Analysis { // 程式作業一的部分
			void getToken() ;
			void printTokens() ;
		}

		// 翻成機器碼
		class Syntax_Analyze { // 程式作業二的主要部分
			string getNIXBPE() ; // 依據指令的格式尋找該nixbpe的字串
			string generateMachineCode() ; // 得到該指令的機器碼
			void saveMachineCodeFile()() ; // 寫出這次的ouput檔
		}

5.未完成的功能
>> 	(1) 尚未完成完整的虛擬指令處理
	
