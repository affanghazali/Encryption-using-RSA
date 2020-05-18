.686 ;Target processor. Use instructions for Pentium class machines
.MODEL FLAT, C ;Use the flat memory model. Use C calling conventions
;INCLUDE Irvine32.inc
.STACK 2048 ;Define a stack segment of 1KB (Not required for this example)
GetEncryptedData PROTO C , data : SDWORD , D :SDWORD , N : SDWORD;



TRYERROR PROTO C , D : SDWORD  , N :SDWORD ,  Data : SDWORD
gcd PROTO C  ,  A :SDWORD ,  H : SDWORD 
powerP PROTO C , D : SDWORD  , N :SDWORD ,  Data : SDWORD
OUT_WIDTH = 8
ENDING_POWER = 10

.data
	P DWORD ?		;first prime
	Q DWORD ?		;second prime
	N DWORD ?		;stores the product of two primes
	E DWORD ?		;exponent which is not a factor of N
	D DWORD ?		;private key
	DataTobeProcessed DWORD ?
	PHAYI DWORD  ?  ;(P-1)(Q-1)
	TestData DWORD ?
	Testval  DWORD ?
	EncryptedData DWORD ?
	PlainText DWORD ?



.CODE			;Indicates the start of a code segment.

FormulaForPrime PROC , val : DWORD			;this function randomly generate two prime numbers by using a generic formula (P and Q)
	mov eax,  val							
	mul eax 
	add eax  ,  val 
	add eax  ,  41 
	ret 


FormulaForPrime ENDP

GCDCal PROC  , A : DWORD ,  B : DWORD
	mov edx , 0  ;  
	L1:
		mov eax ,  A
		mov ebx  , B
		div ebx
		
		cmp edx  , 0 
		je L2 

		mov ebx  ,  B
		mov A , ebx
		mov B , edx
	JMP L1  

	L2 :
		mov eax  ,  B
		ret
GCDCal ENDP

PublicKey PROC ,  val : DWORD , val1 :DWORD			;this function will generate the public key i.e E and N
	mov eax  ,  val									;two arguements are passed which are two prime numbers
	mov ebx , val1 
	mul ebx
	mov N , eax 

	mov ecx  , PHAYI 
	mov edi  ,  5  ;  
	mov eax  , 0  
	L1 :
		INVOKE gcd ,  edi , PHAYI 
		cmp  eax  , 1 
		je L2
		inc edi 
		cmp edi  , PHAYI

		jnl L2

	JMP L1
	L2:
	mov E ,  edi

	ret 

PublicKey ENDP



PrivateKey  PROC , valP : DWORD ,  valQ : DWORD ,  valE : DWORD			;this function will generate the private key i.e D
																		;three arguements are passed to this functions, 2 primes
	mov eax , PHAYI   
	mov ecx  ,  2
	mul ecx   ;  k =  2 k can be any prime number ranging from 1-10 although all the calculations here are done by taking k=2!
	add eax ,  1 
	mov ebx  , valE
	div ebx 
	mov D  ,  eax 

	ret

PrivateKey  ENDP


Encryption PROC , valE : DWORD , valN : DWORD , Data : DWORD
	
	mov eax , Data ; H = 8 
	INVOKE powerP , valE , valN , eax 
	mov EncryptedData , eax

	
	ret 
Encryption ENDP



Decrypt PROC  , valD : DWORD ,  valN : DWORD , data : DWORD 

	INVOKE TRYERROR , valD , valN , data
	ret
Decrypt ENDP

CalculatePHAYI PROC , valP : DWORD , valQ:DWORD
	mov eax  ,  valP													;and an exponent
	mov ebx ,  valQ 

	sub eax  , 1 
	sub ebx  ,  1 
	
	
	mul ebx
	mov PHAYI ,  eax 

	ret
CalculatePHAYI ENDP

fileProcessing Proc , Data : DWORD

	mov eax  ,  Data
	mov ebx  ,  ' '
	cmp eax , ebx 
	je L2 
	mov DataTobeProcessed ,  eax 
	ret
	L2:
	mov eax  , 160
	mov DataTobeProcessed , 160
	ret
fileProcessing ENDP

Entrypoint PROC  ,  RandP : DWORD  , RandQ  :DWORD , Data :DWORD , Mode :DWORD			;Entry point function that initialize P and Q using randomize function
	mov eax  ,  RandP
	mov P   ,  eax  
	mov eax  ,  RandQ
	mov Q   ,  eax  
	;INVOKE showChar ,  Data
	INVOKE fileProcessing  , Data
	mov eax, DataTobeProcessed
	cmp eax , 160
	je L3
	mov eax  , Data 
	sub eax , 105
	mov Data  , eax 
	
	INVOKE CalculatePHAYI , P , Q
	INVOKE PublicKey ,  P ,Q
	INVOKE PrivateKey ,  P ,  Q , E
	cmp Mode , 1
	je L1
		INVOKE Encryption ,  E  ,N , Data
		INVOKE GetEncryptedData , EncryptedData , D , N 
		jmp L4
	L1:
		INVOKE Decrypt ,  D  ,N , EncryptedData
	
	L3:
		INVOKE GetEncryptedData , 32, D , N 
	L4:
		ret
Entrypoint ENDP
END