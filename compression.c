#include <stdio.h>   // Attempt at compression with sorting
#include <stdlib.h> // Works best on 24 bit bitmaps
#include <conio.h> // Assumes header is max 64 bytes 
#include <string.h>
#include <time.h>
#define TC _textcolor
FILE *fp1,*fp2;

int pr=0,cunter=0;
//		The following arrays contain count information for the different nods
int cnt2[7][2]= {{0,6},{1,5},{2,4},{3,3},{4,2},{5,1},{6,0},};	
int cnt3[21][3]={{0,0,5},{0,1,4},{0,2,3},{0,3,2},{0,4,1},{0,5,0},{1,0,4},{1,1,3},{1,2,2},{1,3,1},{1,4,0},{2,0,3},
			    		{2,1,2},{2,2,1},{2,3,0},{3,0,2},{3,1,1},{3,2,0},{4,0,1},{4,1,0},{5,0,0},};
int cnt4[35][4]={{0,0,0,4},{0,0,1,3},{0,0,2,2},{0,0,3,1},{0,0,4,0},{0,1,0,3},{0,1,1,2},{0,1,2,1},{0,1,3,0},{0,2,0,2},
						{0,2,1,1},{0,2,2,0},{0,3,0,1},{0,3,1,0},{0,4,0,0},{1,0,0,3},{1,0,1,2},{1,0,2,1},{1,0,3,0},{1,1,0,2},
						{1,1,1,1},{1,1,2,0},{1,2,0,1},{1,2,1,0},{1,3,0,0},{2,0,0,2},{2,0,1,1},{2,0,2,0},{2,1,0,1},{2,1,1,0},
						{2,2,0,0},{3,0,0,1},{3,0,1,0},{3,1,0,0},{4,0,0,0},};
int cnt5[35][5]={{0,0,0,0,3},{0,0,0,1,2},{0,0,0,2,1},{0,0,0,3,0},{0,0,1,0,2},{0,0,1,1,1},{0,0,1,2,0},{0,0,2,0,1},         
			{0,0,2,1,0},{0,0,3,0,0},{0,1,0,0,2},{0,1,0,1,1},{0,1,0,2,0},{0,1,1,0,1},{0,1,1,1,0},{0,1,2,0,0},{0,2,0,0,1},
			{0,2,0,1,0},{0,2,1,0,0},{0,3,0,0,0},{1,0,0,0,2},{1,0,0,1,1},{1,0,0,2,0},{1,0,1,0,1},{1,0,1,1,0},{1,0,2,0,0},
			{1,1,0,0,1},{1,1,0,1,0},{1,1,1,0,0},{1,2,0,0,0},{2,0,0,0,1},{2,0,0,1,0},{2,0,1,0,0},{2,1,0,0,0},{3,0,0,0,0},};
int cnt6[21][6]={{0,0,0,0,1,1},{0,0,0,1,0,1},{0,0,0,1,1,0},{0,0,1,0,0,1},{0,0,1,0,1,0},{0,0,1,1,0,0},{0,1,0,0,0,1},
					    {0,1,0,0,1,0},{0,1,0,1,0,0},{0,1,1,0,0,0},{1,0,0,0,0,1},{1,0,0,0,1,0},{1,0,0,1,0,0},{1,0,1,0,0,0},
						{1,1,0,0,0,0},{0,0,0,0,0,2},{0,0,0,0,2,0},{0,0,0,2,0,0},{0,0,2,0,0,0},{0,2,0,0,0,0},{2,0,0,0,0,0},};
int cnt7[7][7]={{0,0,0,0,0,0,1},{0,0,0,0,0,1,0},{0,0,0,0,1,0,0},{0,0,0,1,0,0,0},{0,0,1,0,0,0,0},
					    {0,1,0,0,0,0,0},{1,0,0,0,0,0,0},};

// Print functions
void PRINT(int *ARR,int min,int max,int tc){int j;TC(tc);for(j=min;j<max;j++){printf("%d ",ARR[j]);}printf("\n");TC(15);}
void PRINTB(int *ARR,int min,int max,int tc){int j;TC(tc);for(j=min;j<max;j++){if(ARR[j]==0){TC(8);}else{TC(15);}
printf("%d",ARR[j]);}printf("\n");TC(15);}

// Sort function
void sort(int *ARR){int j,k,tmp;for(k=1;k<8;k++){tmp=ARR[k];j=k-1;while(j>=0&&ARR[j]>tmp){ARR[j+1]=ARR[j];j--;}ARR[j+1]=tmp;}}

// Counts bits in binary version of supplied number
int bitcnt(int num,int bits){int j;for(j=bits-1;j>=0;j--){if(num&1<<j){return(j+1);}}return(1);}

// Converts a decimal to a binary number
void dec2bin(int n,int *ARR,int length){int j,k;for(j=0;j<length;j++){ARR[j]=0;}
									   for(j=length-1,k=0;j>=0;j--,k++){if(n&1<<j){ARR[k]=1;}else{ARR[k]=0;}}}

// Generates powers of 2
int pow2(int pow){int j,two=1;for(j=0;j<pow;j++){two*=2;}return(two);}

// Converts a binary array to an integer
int bin2dec(int *ARR,int n){int j,k,num=0;for(j=n-1,k=0;j>=0;j--,k++){if(ARR[j]==1){num+=pow2(k);}}return(num);}

// Prepares the nod and mxnumb info in the header
void prepheaderNOD(int num,int *ARR){int j,TMP[3];dec2bin(num-1,TMP,3);for(j=0;j<3;j++){ARR[j]=TMP[j];}}
void prepheaderDIF(int num,int *ARR){int j,TMP[5];dec2bin(num-1,TMP,5);for(j=3;j<8;j++){ARR[j]=TMP[j-3];}}

// Process the count info
int processcounts(int nod,int *COUNTS,int *cntout){int j,k,tmp,TMP[6]={0},cb=5,cnt=0;// returns count bits
if(nod==4||nod==5){cb=6;}		if(nod==2||nod==7){cb=3;}		if(nod==8||nod==1){return(0);}
if(nod==2){for(k=0;k<7; k++){tmp=0;for(j=0;j<nod;j++){if(COUNTS[j]==cnt2[k][j]){tmp++;}else{break;}if(tmp==nod){cnt=k;break;}}}}
if(nod==3){for(k=0;k<21;k++){tmp=0;for(j=0;j<nod;j++){if(COUNTS[j]==cnt3[k][j]){tmp++;}else{break;}if(tmp==nod){cnt=k;break;}}}}
if(nod==4){for(k=0;k<35;k++){tmp=0;for(j=0;j<nod;j++){if(COUNTS[j]==cnt4[k][j]){tmp++;}else{break;}if(tmp==nod){cnt=k;break;}}}}
if(nod==5){for(k=0;k<35;k++){tmp=0;for(j=0;j<nod;j++){if(COUNTS[j]==cnt5[k][j]){tmp++;}else{break;}if(tmp==nod){cnt=k;break;}}}}
if(nod==6){for(k=0;k<21;k++){tmp=0;for(j=0;j<nod;j++){if(COUNTS[j]==cnt6[k][j]){tmp++;}else{break;}if(tmp==nod){cnt=k;break;}}}}
if(nod==7){for(k=0;k<7; k++){tmp=0;for(j=0;j<nod;j++){if(COUNTS[j]==cnt7[k][j]){tmp++;}else{break;}if(tmp==nod){cnt=k;break;}}}}
dec2bin(cnt,TMP,cb);for(k=0;k<cb;k++){cntout[k]=TMP[k];}return(cb);}

// Calculate how many times to read from file
int adjust(int num,int adj){if(num%adj!=0){num=((num/adj)+1)*adj;}return(num);}

// Splits the number into 2 8 bit numbers (from 24 bit) for output 
void splitnum(int num,int *lilarr,int colflag,int revflag){int c1=0,c2=0;if(revflag==0){num=16777215-num;}
if(num==0){lilarr[0]=0;lilarr[1]=0;lilarr[2]=0;}else{while(num>=65536){num-=65536;c1++;}while(num>=256){num-=256;c2++;}//if(revflag==0){c1=255-c1;c2=255-c2;num=255-num;}
if(colflag==1){lilarr[0]=c1;	lilarr[1]=c2;	lilarr[2]=num;}//012
if(colflag==2){lilarr[0]=c1;	lilarr[2]=c2;	lilarr[1]=num;}//021
if(colflag==3){lilarr[1]=c1;	lilarr[0]=c2;	lilarr[2]=num;}//102
if(colflag==4){lilarr[2]=c1;	lilarr[0]=c2;	lilarr[1]=num;}//120//swapped with 6
if(colflag==5){lilarr[2]=c1;	lilarr[1]=c2;	lilarr[0]=num;}//210
if(colflag==6){lilarr[1]=c1;	lilarr[2]=c2;	lilarr[0]=num;}}}//201//swapped with 4

int getdata(int colflag,int revflag){int num=0;
if(colflag==1){num+=fgetc(fp1)*65536;	num+=fgetc(fp1)*256;   num+=fgetc(fp1);      }
if(colflag==2){num+=fgetc(fp1)*65536;	num+=fgetc(fp1);       num+=fgetc(fp1)*256;  }
if(colflag==3){num+=fgetc(fp1)*256;		num+=fgetc(fp1)*65536; num+=fgetc(fp1);      }
if(colflag==4){num+=fgetc(fp1)*256;		num+=fgetc(fp1);	   num+=fgetc(fp1)*65536;}
if(colflag==5){num+=fgetc(fp1);			num+=fgetc(fp1)*256;   num+=fgetc(fp1)*65536;}
if(colflag==6){num+=fgetc(fp1);			num+=fgetc(fp1)*65536; num+=fgetc(fp1)*256;	 }
if(revflag==0){return(16777215-num);}else{return(num);}}

int getdata0(){int num=0;num+=fgetc(fp1)*65536;num+=fgetc(fp1)*256;num+=fgetc(fp1);return(num);}

// Process data, read from compressed data file, prepare buffer
int process(int *allout,int *buffer,int times,int nb,int buffb){int j,k,m,tempar[256]={0};
		for(j=0;j<buffb;j++){allout[j]=buffer[j];}
		for(j=0;j<times;j++){dec2bin(getdata0(),tempar,24);
		for(k=(j*24),m=0;m<24;k++,m++){allout[k+buffb]=tempar[m];}}buffb=(times*24)-(nb-buffb);
		for(j=nb,k=0;j<nb+buffb;j++,k++){buffer[k]=allout[j];}return(buffb);}

int reversebit(int num){int j,result=0;for(j=0;j<24;j++){result=(result<<1)|(num&1);num>>=1;}return(result);}

int permsearch(int ordtmp,int *perms,int size){int lower=0,upper=size-1,add;
while(lower<=upper){add=lower+((upper-lower)/2);	if(perms[add]==ordtmp){	return(add); }else{
 													if(perms[add]<ordtmp){	lower= add+1;}else{
																			upper=add-1;}}}return(-1);}
//	Generate a number from the order supplied
int pow10(int *order){int j,num=0,ten=1;for(j=0;j<8;j++){num+=(ten*order[j]);ten*=10;}return(num);}


int main(int argc,char *argv[]){int j,k,m,n;		// used in for loops
double a=0.0,b=0.0,compratio=0.0;					// used for showing compression rates
int ob=0,cb=0,nb=0,buffb=0,allbits=0;				// # of bits in order, counts, numbers (or diffs), buffer size, # number of totalbits
int mxnum=0,mxnumb=0,mxdiff=0,mxdiffb=0;			// biggest number, # of bits in biggest number, biggest diff, # of bits in biggest diff
int tnum=0,ordtmp=0,permordtmp=0,tmp1=0,tmp2=0;
int tempar[256]={0},difftmp[256]={0},headertmp[8]={0};		// temporary variables
int flag=0,ordflag=0,numflag=0,revflag=1,colflag=4;			// flag variables
int prg1=0,prg2=0;			// used for progress indicator
long int fend=0;			// file end (file size)
int nod=0;					// number of differences or numbers in frame
int bytecnt=0;				// number of bytes written
int sb1=1,sb2=5,sb3=8;		// shortcut bits	// cumulative sums of shortcuts bits
int sbs1=pow2(sb1),sbs2=sbs1+pow2(sb2),sbs3=sbs2+pow2(sb3);//printf("sbss  %d  %d  %d\n",sbs1,sbs2,sbs3); // Calculate limits for shortcut stores
int counter=0,x=0,cnt=0;	// counters
int hcnt=0,hdrval[64]={0},hdrmrk[64]={0};					// used for bitmap header info, arrays used to store info
int ordcnt=0,ordsnum=0;		// used to count order shortcuts
int num=12345678;			// used to generate order permutations
long int rem=0;				// remainder of file minus header mod 24
int buffa[32]={0};			// used to count the leftover numbers that won't fit into a last full frame, buffa is written to end of file
int data=0;					// used for data read from file
//int choice=0;				// used for choice whether to compress or decompress
int allords[40320][2]={0,0},topords[40320][2]={0,0},perms[40320]={0},number[8]={0};	// used in generating orders
int ordlist[16]={0},counts[8]={0},diffs[8]={0};
int ordstore5[sbs1],ordstore6[sbs2-sbs1],ordstore7[sbs3-sbs2],allordstore[sbs3];	// used for storing order shortcuts
int part1[8]={0},part2[8]={0},part3[3]={0};		// used to store parts of header
int first3bytes[24]={0};	// 1st 3 bytes of compressed data, contains values for; remainder(5), # of shortcuts(3x4), revflag(1) 
int stats[8][24]={0,0},sum=0;		// store statistics
int header[32]={0},hdbyte=54;		// header info (nod and mxnumb), header byte size
int buffer[256]={0};		// buffer info, extra info that is below 8 bits that can't be immediately written
int list[8]={0},bulist[8]={0};		// list contains original # list, bulist is a backup - used to generate counts
int cntout[6]={0};			// cntout is an address to counts stored in predefined arrays for each nod
int lilarr[3]={0};			// used to output data to file
int ordhead=0,ordint=0;		// contains order info for shortcuts					// store order as a decimal number			
int times=0;				// used in determining how many bytes need to be read from file
int numout[192]={0};		// used in outputting number data to file
int ordout[16]={0},order[16]={0};		// stores order info (16 bit lookup table number)
int numlist[8]={0},diff_num[8]={0};		// contains original numbers 	// stores diffs
int allout[256]={0};		// contains all info (cb,ob,nb)
int foundit=0,bb1[8]={0},bb2[8]={0},bb3=0;		// foundit used for searching in permutations, other used for error check
char filein[128]="blotus1.bmp",fileout[128]={0},exten[6]="cmp.",tempc[128]="",file1[128],ext[16]={0};strcpy(file1,filein);
//int prevnod=0,prevdiffs[8]={0},dup=0;				// used for duplicates - not working
//		Deal with command arguements // sb1 has to be > 0 maybe 0 for no shortcuts
if(argc>1){printf("Input file is: %s\n",argv[1]);strcpy(filein,argv[1]);}	//else{printf("Need a file name..\n");exit(0);}
if(argc==5){sb1=atoi(argv[2])-1;sb2=atoi(argv[3])-1;sb3=atoi(argv[4])-1;}
if(argc==3){revflag=atoi(argv[2]);}if(argc==6){revflag=atoi(argv[5]);}
clock_t start,end;start=clock();
//		Generate array of all permutations of orders
while(num<=87654321){tnum=num;flag=0;for(j=0;j<8;j++){number[j]=tnum%10;if(number[j]==9||number[j]==0){flag=1;break;}tnum/=10;}
	if(flag==0){for(k=0;k<8;k++){for(j=k+1;j<8;j++){if(number[k]==number[j]){flag=1;break;}}if(flag==1){break;}}}
	if(flag==0){perms[cnt]=num-11111111;cnt++;}num+=9;}
//printf("Compress: 0 Decompress: 1\t\t");scanf("%d",&choice);if(choice==0){printf("File to compress: ");scanf("%s",filein);

//		File naming and creation
	strcpy(tempc,filein);	strcpy(fileout,exten);	strcat(fileout,tempc);		printf("Output file is: %s\n\n",fileout);
	if((fp1=fopen(filein, "rb"))==NULL){printf("File open fail..bailing out...\n\n");exit(0);}
	if((fp2=fopen(fileout,"wb"))==NULL){printf("File open fail..bailing out...\n\n");exit(0);}
tmp1=strlen(filein);printf("len=%d\n",tmp1);
char *tmp=strchr(filein,46);printf("lento=%d\n",tmp2);
memccpy(ext,filein,46,128);
printf("\n%s\n",ext);

fseek(fp1,+0,SEEK_SET);	//fseek(fp1,+10,SEEK_CUR);hdbyte=fgetc(fp1);printf("header size: %d\n",hdbyte);fseek(fp1,+0,SEEK_SET);
fseek(fp1,+2,SEEK_CUR);fread(&num,4,1,fp1);		printf("File size: %d  (from header)\n",num);
fseek(fp1,+4,SEEK_CUR);fread(&hdbyte,4,1,fp1);	printf("Header size: %d\n",hdbyte);if(hdbyte>64){hdbyte=54;}
fseek(fp1,+0,SEEK_SET);

//		Need to provide the remainder to be able to read the file properly at the other end
fseek(fp1,+0,SEEK_END);fend=ftell(fp1);printf("File size: %d bytes\n\n",fend);			// Get file size
rem=(fend-hdbyte)%24;fend-=rem;if(pr==1){printf("Remainder: %d\n\nbuffa:\t",rem);PRINT(buffa,0,rem,5);}	// For recording buffer info
if(rem>0){fseek(fp1,-rem,SEEK_CUR);for(j=0;j<rem;j++){buffa[j]=fgetc(fp1);}}fseek(fp1,+0,SEEK_SET); // buffer extra bytes to write back later

//		Prpeare info for first 2 bytes (remainder,short cut sizes)
dec2bin(rem,tempar,5);		for(j=0;     j<5;j++)    {first3bytes[j]=tempar[j];}
dec2bin(sb1-1,tempar,3);	for(j=0,k=5; j<3;j++,k++){first3bytes[k]=tempar[j];}
dec2bin(sb2-1,tempar,3);	for(j=0,k=8; j<3;j++,k++){first3bytes[k]=tempar[j];}
dec2bin(sb3-1,tempar,3);	for(j=0,k=11;j<3;j++,k++){first3bytes[k]=tempar[j];}
dec2bin(colflag-1,tempar,3);for(j=0,k=14;j<3;j++,k++){first3bytes[k]=tempar[j];}
first3bytes[17]=revflag;
dec2bin(hdbyte-1,tempar,6);	for(j=0,k=18;j<6;j++,k++){first3bytes[k]=tempar[j];}
printf("# %d   %d %d %d %d %d %d %d\n",argc,rem,sb1,sb2,sb3,colflag,revflag,hdbyte);//PRINT(first3bytes,0,24,4);
//		Write rem and sc db info to output file
for(j=0;j<3;j++){for(k=j*8,m=0;m<8;k++,m++){part1[m]=first3bytes[k];}data=bin2dec(part1,8);fputc(data,fp2);}bytecnt+=3;
sbs1=pow2(sb1);sbs2=sbs1+pow2(sb2);sbs3=sbs2+pow2(sb3);
//printf("sb1=%d sb2=%d sb3=%d sbs1=%d sbs2=%d sbs3=%d\n",sb1,sb2,sb3,sbs1,sbs2,sbs3);

//		Start reading in the input file header	1st array indicates which fields have info, 2nd shows field data
for(j=0;j<hdbyte;j++){hdrmrk[j]=hdrval[j]=0;}
for(j=0;j<hdbyte;j++){data=fgetc(fp1);		if(data!=0){hdrmrk[j]=1;hdrval[hcnt]=data;hcnt++;}} // only write the non zero data
times=adjust(hdbyte,8)/8;//PRINT(hdrval,0,hcnt,4);printf("times=%d\n",times);
//		Convert to numbers and write to file
for(j=0;j<times;j++){for(k=0;k<8;k++){tempar[k]=hdrmrk[(j*8)+k];}
	num=bin2dec(tempar,8);fputc(num,fp2);bytecnt++;} // End reading bmp file header
for(j=0;j<hcnt;j++){fputc(hdrval[j],fp2);bytecnt++;}prg1=prg2=fend/32;//for(j=0;j<31;j++){printf("%c",95);}printf("\n");

//		Start reading in the pixel data of the input file to process shortcuts. Process is explained in compression section
while(ftell(fp1)<fend){//if(ftell(fp1)>=prg2){prg2+=prg1;printf("%c",219);}
for(k=0;k<8;k++){list[k]=getdata(colflag,revflag);}if(revflag==1){list[k]=reversebit(list[k]);}
	for(j=0;j<8;j++){bulist[j]=list[j];order[j]=j;}sort(list);
	cnt=0;while(cnt<8){for(k=0;k<8;k++){if(list[cnt]==bulist[k]&&bulist[k]!=-1){order[cnt]=k;bulist[k]=-1;break;}}cnt++;}
	nod=0;for(k=0;k<8;k++){if(list[k]!=list[k+1]){nod++;}else{continue;}}
	if(nod>1){ordtmp=pow10(order);//ordtmp=0;ten=1;for(k=0;k<8;k++){ordtmp+=(ten*order[k]);ten*=10;}
//		Add order if it is not already in list, count if already in list
	foundit=permsearch(ordtmp,perms,40320);if(foundit!=-1){topords[foundit][0]++;}}if(ftell(fp1)>=fend){break;}} // end of collecting order info

ordsnum=0;for(j=0;j<40320;j++){ // Populate allords with the orders found (more than zero times)
if(topords[j][0]>0){allords[ordsnum][1]=perms[j];allords[ordsnum][0]=topords[j][0];ordsnum++;topords[j][0]=0;}}

//		Get the most frequent orders
ordcnt=0;cnt=1;int pcnt=cnt;printf("\n");	//	Adjust cnt to fit sc stores, to speed up sorting of topords
	while(ordcnt<sbs3){ordcnt=0;for(j=0;j<ordsnum;j++){if(allords[j][0]>cnt){ordcnt++;}}//printf("ordcnt: %d  %d\n",ordcnt,ordsnum);
	if(ordcnt<sbs3){cnt=pcnt;break;}if(ordcnt>=sbs3){pcnt=cnt;cnt++;ordcnt=0;}}

//		Count how many orders occur more than once and put those >=cnt in topords
ordcnt=0;for(j=0;j<ordsnum;j++){if(allords[j][0]>cnt){	topords[ordcnt][0]=allords[j][0];
														topords[ordcnt][1]=allords[j][1];	ordcnt++;}}
	if(pr==1){printf("\nTotal orders: %d\n\nSort remaining top %d (orders occuring more than %d times)\n\n",ordsnum,ordcnt);}
	if(ordcnt<sbs3){printf("\n\nThe number of orders (%d) is less than the space for them (%d), errors may occur).\n\n",ordcnt,sbs3);
	printf("\n\nTry decreasing the shortcut store sizes.\n\n");} // maybe 1st 2 bytes of allordstore can be a revised sbs3
//		Sort top orders
	for(k=1;k<ordcnt;k++){tmp1=topords[k][0];tmp2=topords[k][1];j=k-1;while(j>=0&&topords[j][0]>tmp1){
	topords[j+1][0]=topords[j][0];topords[j+1][1]=topords[j][1];j--;}topords[j+1][0]=tmp1;topords[j+1][1]=tmp2;}

//		Get all filtered, copy reverse topords into allordstore
	for(j=ordcnt-1,k=0;k<sbs3;j--,k++){allordstore[k]=topords[j][1];} // PRINT(allordstore,0,sbs3,4);
		if(pr==1){int sum=0;for(j=0,k=ordcnt-1;j<sbs3;j++,k--){sum+=topords[k][0];
				printf("%d\t%8d\t%8d\t%d\n",j,topords[k][0],topords[k][1],sum);}printf("\n\n");printf("ordcnt: %d\n\n",ordcnt);}

//		Generate short cuts to sorted permorders file for added shortcuts
for(j=0;j<sbs3;j++){permordtmp=permsearch(allordstore[j],perms,40320);dec2bin(permordtmp,ordout,16);//
//		Write shortcuts out to file
for(k=0;k<8;k++){part1[k]=ordout[k];part2[k]=ordout[k+8];}
tmp1=bin2dec(part1,8);tmp2=bin2dec(part2,8);fputc(tmp1,fp2);fputc(tmp2,fp2);bytecnt+=2;}

//		Start reading data for compression
fseek(fp1,+0,SEEK_SET);fseek(fp1,+54,SEEK_CUR);		// go to start of data (skipping header) and start reading the input file
prg2=prg1;printf("\n");while(ftell(fp1)<fend){//if(ftell(fp1)>=prg2){prg2+=prg1;printf("%c",219);}
for(k=0;k<8;k++){list[k]=getdata(colflag,revflag);}if(revflag==1){list[k]=reversebit(list[k]);}

//		Backup and sort the list of the 8 numbers, setup default order
for(j=0;j<8;j++){numlist[j]=bulist[j]=list[j];order[j]=j;}sort(list);mxnum=list[7];mxnumb=bitcnt(mxnum,24);

// 		Calculate list order (where the sorted numbers appear in the original array)
cnt=0;while(cnt<8){for(k=0;k<8;k++){if(list[cnt]==bulist[k]&&bulist[k]!=-1){order[cnt]=k;bulist[k]=-1;break;}}cnt++;}

//		Calculate counts (the amount of times each number appears in the original array). Processing counts allows us to work out the individual numbers that appear in the original array
for(j=k=nod=0;k<8;k++){if(list[k]!=list[k+1]){diff_num[nod]=list[k];counts[nod]=j;nod++;j=0;}else{j++;}}
prepheaderNOD(nod,header); // add nod info to start of header

//		Excludes cases of nod=1 and nod=8 as they don't need count info (7 and 00000000 respectively)
cb=0;if(nod!=1&&nod!=8){cb=processcounts(nod,counts,cntout);} // cntout is an address to counts stored in arrays

//		Calculate the differences between the consecutive different numbers, we use this to regenerate the ordered diferent numbers
diffs[0]=diff_num[0];mxdiff=diff_num[0];for(k=1;k<nod;k++){diffs[k]=(diff_num[k]-diff_num[k-1]);
	if(diffs[k]>mxdiff){mxdiff=diffs[k];}}mxdiffb=bitcnt(mxdiff,24);nb=nod*mxdiffb;if(mxdiffb<=mxnumb){stats[nod-1][mxnumb-1]++;}

//		Search for order in shortcuts // Change order to a decimal number for searching // NOD==1 needs no order
ob=0;if(nod!=1){ordtmp=pow10(order);ordflag=0;//ordflag=ordtmp=0;ten=1;for(k=0;k<8;k++){ordtmp+=(ten*order[k]);ten*=10;}//printf("ordtmp: %d\n",ordtmp);
	if(sb1>0){					for(j=0;j<sbs1;j++){			if(ordtmp==allordstore[j]){dec2bin(j,tempar,sb1);ordflag=1;
		prepheaderNOD(6,ordout);for(n=3,m=0;m<sb1;n++,m++){ordout[n]=tempar[m];}ob=sb1+3;break;}}}
	if(ordflag==0&&sb2>0){		for(k=0,j=sbs1;j<sbs2;j++,k++){	if(ordtmp==allordstore[j]){dec2bin(j-sbs1,tempar,sb2);ordflag=1;
		prepheaderNOD(7,ordout);for(n=3,m=0;m<sb2;n++,m++){ordout[n]=tempar[m];}ob=sb2+3;break;}}}
	if(ordflag==0&&sb3>0){		for(k=0,j=sbs2;j<sbs3;j++,k++){	if(ordtmp==allordstore[j]){dec2bin(j-sbs2,tempar,sb3);ordflag=1;
		prepheaderNOD(8,ordout);for(n=3,m=0;m<sb3;n++,m++){ordout[n]=tempar[m];}ob=sb3+3;break;}}}

//		If not in shortcuts search for order in all permutations list
if(ordflag==0){ob=16;permordtmp=permsearch(ordtmp,perms,40320);dec2bin(permordtmp,ordout,ob);}}//

if(ob+cb+nb<8*mxnumb){numflag=1;}else{numflag=0;}	//	decide whether to compress or not

//		Prepare num array	mxnumb is used if order and compression not used
if(numflag==0){ob=cb=0;nb=mxnumb*8;for(j=0;j<3;j++){header[j]=1;}prepheaderDIF(mxnumb,header);
tmp1=0;for(j=0;j<8;j++){dec2bin(numlist[j],tempar,mxnumb);for(k=0;k<mxnumb;k++){numout[tmp1+k]=tempar[k];}tmp1+=mxnumb;}}

//		Prepare diff array
if(numflag==1){if(nod==1){ob=cb=0;}if(nod==8){cb=0;}for(j=0;j<nod;j++){dec2bin(diffs[j],tempar,mxdiffb);
for(k=0;k<mxdiffb;k++){numout[(j*mxdiffb)+k]=tempar[k];}}prepheaderDIF(mxdiffb,header); // Add mxdiffb info to header

//		This allows us to use nod=8 cases for compression, we put the nod=8 mxdiffb data into the spare space in the mxdiffb field of other nods
if(numflag==1&&nod==8){
if(mxdiffb<9){				nod=5;prepheaderNOD(nod,header);mxdiffb+=24;prepheaderDIF(mxdiffb,header);}
if(mxdiffb<17&&mxdiffb>8){	nod=6;prepheaderNOD(nod,header);mxdiffb+=16;prepheaderDIF(mxdiffb,header);}
if(mxdiffb<25&&mxdiffb>16){	nod=7;prepheaderNOD(nod,header);mxdiffb+=8; prepheaderDIF(mxdiffb,header);}}}

//		Duplicates - not working
//if(numflag==1&&nod==prevnod){cnt=0;for(j=0;j<nod;j++){if(prevdiffs[j]==diffs[j]){cnt++;}else{break;}}
//if(cnt==nod){prepheaderNOD(1,header);prepheaderDIF(25,header);ob=cb=nb=0;}
//for(j=0;j<nod;j++){prevdiffs[j]=diffs[j];}}prevnod=nod;

pr=0; //		Prepare allout array
for(j=0;				j<buffb;			j++)	{allout[j]=buffer[j];	if(pr==1){TC(4);printf("%d",allout[j]);}}
for(k=0,j=buffb;		j<buffb+8;			j++,k++){allout[j]=header[k];	if(pr==1){TC(2);printf("%d",allout[j]);}}
for(k=0,j=buffb+8;		j<buffb+8+ob;		j++,k++){allout[j]=ordout[k];	if(pr==1){TC(5);printf("%d",allout[j]);}}
for(k=0,j=buffb+8+ob;	j<buffb+8+ob+cb;	j++,k++){allout[j]=cntout[k];	if(pr==1){TC(6);printf("%d",allout[j]);}}
for(k=0,j=buffb+8+ob+cb;j<buffb+8+ob+cb+nb;	j++,k++){allout[j]=numout[k];	if(pr==1){TC(1);printf("%d",allout[j]);}}
if(pr==1){TC(15);printf("\n");pr=0;}
allbits=buffb+8+ob+cb+nb;buffb=allbits%8;for(k=allbits-buffb,j=0;j<allbits;j++,k++){buffer[j]=allout[k];}
	for(j=0;j<adjust(allbits-buffb,8)/8;j++){for(k=0;k<8;k++){tempar[k]=allout[(j*8)+k];}num=bin2dec(tempar,8);
	fputc(num,fp2);bytecnt++;}if(ftell(fp1)>=fend){break;}}
//		Write out remaining buffers
if(buffb>0){num=bin2dec(buffer,8);fputc(num,fp2);bytecnt++;}
if(rem>0){for(j=0;j<rem;j++){fputc(buffa[j],fp2);bytecnt++;}}
a=(long int)bytecnt;b=(long int)fend;compratio=a/b;
printf("\nFinshed compression\n\n\n");
printf("Original is: %d\nCompressed:  %d \tCompression is: %.2f%%\tratio: 1:%.2f\n",fend+rem,bytecnt,(1-compratio)*100,b/a);
printf("Compressed file is %3.2f%% the size of the original.\n\n",(a/b)*100);
fclose(fp1);fclose(fp2); // }
//		Print stats
if(pr==1){for(j=0;j<8;j++){printf("NOD: %d\t",j+1);for(k=0;k<24;k++){printf("%5d ",stats[j][k]);}printf("\n");}}

//		Decompress
//if(choice==1){printf("File to decompress: ");scanf("%s",filein);
bytecnt=fend=buffb=0;for(j=0;j<54;j++){hdrmrk[j]=hdrval[j]=0;} // File naming and creation
strcpy(filein,tempc);if(argc>1){strcpy(filein,argv[1]);}
strcpy(tempc,filein);strcpy(fileout,"out.");	strcpy(filein,"cmp.");
strcat(filein,tempc);strcat(fileout,tempc);		printf("\nOutput file is: %s\n\n",fileout);
		if((fp1=fopen(filein, "rb"))==NULL)		{printf("1 File open fail..bailing out...\n\n");exit(0);}
		if((fp2=fopen(fileout,"wb"))==NULL)		{printf("2 File open fail..bailing out...\n\n");exit(0);}

//		Process rem and sc db size info
tmp1=getdata0();dec2bin(tmp1,tempar,24);
for(j=0;     j<5;j++)    {part1[j]=tempar[j];}rem=bin2dec(part1,5);
for(j=0,k=5; j<3;j++,k++){part1[j]=tempar[k];}sb1=bin2dec(part1,3)+1;
for(j=0,k=8; j<3;j++,k++){part1[j]=tempar[k];}sb2=bin2dec(part1,3)+1;
for(j=0,k=11;j<3;j++,k++){part1[j]=tempar[k];}sb3=bin2dec(part1,3)+1;
for(j=0,k=14;j<3;j++,k++){part1[j]=tempar[k];}colflag=bin2dec(part1,3)+1;
revflag=first3bytes[17];
for(j=0,k=18;j<6;j++,k++){part1[j]=tempar[k];}hdbyte=bin2dec(part1,6)+1;
sbs1=pow2(sb1);sbs2=sbs1+pow2(sb2);sbs3=sbs2+pow2(sb3);//printf("hdbyte=%d\n",hdbyte);
//		Calculate compressed file size
fseek(fp1,+0,SEEK_END);fend=ftell(fp1)-rem;fseek(fp1,-rem,SEEK_CUR);printf("Compressed file size:   %d bytes\n\n",fend);
//		Read in remainder data (leftover data, mod 24)
	for(j=0;j<rem;j++){buffa[j]=fgetc(fp1);}fseek(fp1,+0,SEEK_SET);fseek(fp1,+3,SEEK_CUR);// goto start of file after first 3 bytes

//		Start reading in the input file header to determine file header info
cnt=0;for(k=0;k<7;k++){data=fgetc(fp1);dec2bin(data,tempar,8);for(j=0;j<8;j++){if(tempar[j]==1){cnt++;hdrmrk[(k*8)+j]=1;}}}
//		Write file header info to file
	for(j=0;j<hdbyte;j++){	if(hdrmrk[j]==1){data=fgetc(fp1);	fputc(data,fp2);	bytecnt++;}
							if(hdrmrk[j]==0){data=0;			fputc(0,fp2);		bytecnt++;}}//printf("\n");
//		Read in shortcut info
	for(j=0;j<sbs3;j++){tmp1=fgetc(fp1);tmp2=fgetc(fp1);dec2bin(tmp1,part1,8);dec2bin(tmp2,part2,8);
	for(k=0;k<8;k++){tempar[k]=part1[k];tempar[k+8]=part2[k];}tnum=bin2dec(tempar,16);allordstore[j]=perms[tnum];}

	for(j=0;j<sbs1;			j++)	{ordstore5[j]=allordstore[j];}	//	Create 3 databases of shortcuts for each group of shortcuts
	for(j=0,k=sbs1;k<sbs2;	j++,k++){ordstore6[j]=allordstore[k];}
	for(j=0,k=sbs2;k<sbs3;	j++,k++){ordstore7[j]=allordstore[k];}

//		Start reading in compressed data from file
buffb=0;prg2=prg1=fend/32;while(ftell(fp1)<fend){//if(ftell(fp1)>=prg2){prg2+=prg1;printf("%c",219);}
//		Prepare buffer
if(buffb<8) {for(j=0;j<buffb;j++){header[j]=buffer[j];}data=fgetc(fp1);dec2bin(data,headertmp,8);
			for(j=buffb,n=0;n<8;j++,n++)	{header[j]=headertmp[n];}
			for(j=0,k=8;j<buffb+8;j++,k++)	{buffer[j]=header[k];}						goto next;}
if(buffb==8){for(j=0;j<buffb;j++)			{header[j]=buffer[j];}			buffb =0;	goto next;}
if(buffb>8) {for(j=0;j<buffb;j++)			{header[j]=buffer[j];}
			for(j=0;j<buffb;j++)			{tempar[j]=buffer[j];}
			for(k=8,j=0;j<buffb;k++,j++)	{buffer[j]=tempar[k];}			buffb-=8;
if(buffb>=8)								{fseek(fp1,-1,SEEK_CUR);		buffb-=8;}	goto next;}

next:
		for(j=0,k=0;k<3;j++,k++){part1[k]=header[j];}nod=bin2dec(part1,3)+1;	// Get header info
		for(j=3,k=0;k<5;j++,k++){part2[k]=header[j];}mxnumb=bin2dec(part2,5)+1;
		for(j=8,k=0;k<3;j++,k++){part3[k]=header[j];}ordhead=bin2dec(part3,3);	// This might not necessarily work - if header size <11, covered below: if(buffb<3){...

//		Determine whether data is compressed or not
if(nod==8){numflag=0;}else{numflag=1;}		//dup=0;if(nod==1&&mxnumb==25){dup=1;goto dupe;}	// used for duplicates - not working

//		No compression, just write out numbers at mxnumb
if(numflag==0){nb=mxnumb*8;times=adjust(nb-buffb,24)/24;buffb=process(allout,buffer,times,nb,buffb);	// Process numbers
	for(j=0;j<8;j++){for(k=mxnumb*j,n=0;n<mxnumb;k++,n++){part1[n]=allout[k];}
	data=bin2dec(part1,mxnumb);splitnum(data,lilarr,colflag,revflag);for(m=0;m<3;m++){fputc(lilarr[m],fp2);}bytecnt+=3;}}

if(numflag==1){	//		Compression, deal with orders, counts and differences
				//if(nod==5&&mxnumb>24){nod=8;mxnumb=(mxnumb)-24;} // Get mxdiffb info for nod=8 cases
				//if(nod==6&&mxnumb>24){nod=8;mxnumb=(mxnumb)-16;}
				//if(nod==7&&mxnumb>24){nod=8;mxnumb=(mxnumb)-8;}
if(mxnumb>24){if(nod>4&&nod<8){mxnumb=mxnumb-((8-nod)*8);nod=8;}}

//		If header info incomplete for order data - only reads ahead (doesn't advance through file)
if(buffb<3){data=fgetc(fp1);fseek(fp1,-1,SEEK_CUR);dec2bin(data,tempar,8);
		for(j=0;j<buffb;j++){part3[j]=buffer[j];}for(j=buffb,k=0;k<3;j++,k++){part3[j]=tempar[k];}ordhead=bin2dec(part3,3);}
//		Determine bit sizes for orders, counts and differences
	ob=16;if(ordhead==5){ob=sb1+3;}  if(ordhead==6){ob=sb2+3;}  if(ordhead==7){ob=sb3+3;}
	cb=0; if(nod==2||nod==7){cb=3;}if(nod==3||nod==6){cb=5;}if(nod==4||nod==5){cb=6;} if(nod==1||nod==8){cb=0;}	// cb=0 as default	ob=16 as default
	nb=mxnumb*nod;allbits=ob+cb+nb;times=adjust(allbits-buffb,24)/24;
if(nod==1){allbits=mxnumb;cb=ob=0;if(mxnumb<buffb){times=0;}else{times=1;}}
buffb=process(allout,buffer,times,allbits,buffb);	// Process diffs
		for(j=ob+cb,k=0;j<allbits;j++,k++){difftmp[k]=allout[j];}
		for(j=0;j<nb/mxnumb;j++){for(k=(j*mxnumb),n=0;n<mxnumb;k++,n++){tempar[n]=difftmp[k];}diffs[j]=bin2dec(tempar,mxnumb);}
//
//		Prepare order info
		for(j=0,k=0;j<ob;j++,k++){order[k]=allout[j];}//printf("%d  getting here?  %d %d\n",ftell(fp2),ob,ordhead);//PRINT(order,0,8,4);
if(ordhead <5){for(j=0;j<ob;j++){ordlist[j]=order[j];}			ordtmp=bin2dec(ordlist,16);		ordint=perms[ordtmp];}
if(ordhead==5){for(j=0;j<ob;j++){ordlist[j]=order[j+3];}		ordtmp=bin2dec(ordlist,sb1);	ordint=ordstore5[ordtmp];}
if(ordhead==6){for(j=0;j<ob;j++){ordlist[j]=order[j+3];}		ordtmp=bin2dec(ordlist,sb2);	ordint=ordstore6[ordtmp];}
if(ordhead==7){for(j=0;j<ob;j++){ordlist[j]=order[j+3];}		ordtmp=bin2dec(ordlist,sb3);	ordint=ordstore7[ordtmp];}
		for(j=7,k=0;j>=0;j--,k++){ordlist[k]=ordint%10;ordint/=10;}
//		Prepare count info
		for(j=ob,k=0;j<ob+cb;j++,k++){counts[k]=allout[j];}cnt=bin2dec(counts,cb);
if(nod==2){for(j=0;j<nod;j++){counts[j]=cnt2[cnt][j];}}			if(nod==3){for(j=0;j<nod;j++){counts[j]=cnt3[cnt][j];}}
if(nod==4){for(j=0;j<nod;j++){counts[j]=cnt4[cnt][j];}}			if(nod==5){for(j=0;j<nod;j++){counts[j]=cnt5[cnt][j];}}
if(nod==6){for(j=0;j<nod;j++){counts[j]=cnt6[cnt][j];}}			if(nod==7){for(j=0;j<nod;j++){counts[j]=cnt7[cnt][j];}}

//		Recreate the original array using; the different number list, the counts list and the order list.
//		Setup order(s) and counts for nod 1 and 8
if(nod==1){for(j=0;j<8;j++){ordlist[j]=j;}counts[0]=7;}if(nod==8){for(j=0;j<8;j++){counts[j]=0;}}
x=0;for(k=0;k<nod;k++){diff_num[k]=x+=diffs[k];}
x=0;for(k=0;k<nod;k++){;for(j=0;j<counts[k]+1;j++){allout[x]=diff_num[k];x++;}}
x=0;while(x<8){for(k=0;k<8;k++){if(ordlist[k]==x){numout[x]=allout[k];}}x++;}

//		Write out to file 	// change lilarr[k] to 255 to see which parts of a file are compressed
for(j=0;j<8;j++){splitnum(numout[j],lilarr,colflag,revflag);for(k=0;k<3;k++){fputc(lilarr[k],fp2);}bytecnt+=3;}
}}/*}*/if(rem>0){for(j=0;j<rem;j++){fputc(buffa[j],fp2);bytecnt++;}}
printf("\nFinished decompression \n\nOutput file size: %d\n\n\n",bytecnt);fclose(fp1);fclose(fp2);
//		Check compression aginst original
counter=0;			if((fp1=fopen(file1,"rb"))==NULL)  {printf("File open fail..bailing out...\n\n");exit(0);}
					if((fp2=fopen(fileout,"rb"))==NULL){printf("File open fail..bailing out...\n\n");exit(0);}

fseek(fp1,+0,SEEK_END);fend=ftell(fp1);printf("Checking for errors.\n\n",fend);
fseek(fp1,+54,SEEK_SET);fseek(fp2,+54,SEEK_SET);
while(ftell(fp1)<fend){fread(&bb1,sizeof(int),3,fp1);fread(&bb2,sizeof(int),3,fp2);

for(j=0;j<3;j++){bb3=bb1[j]-bb2[j];dec2bin(bb1[j],tempar,32);dec2bin(bb2[j],difftmp,32);
if(bb1[j]!=bb2[j]&&ftell(fp1)<fend){counter++;printf("%5d %9d\t\tdiff:\t\t%10d\t%10d\t\t%d\n",counter,ftell(fp1)+j,bb1[j],bb2[j],bb3);
PRINTB(tempar,0,32,4);PRINTB(difftmp,0,32,4);}}}
printf("\nErrors:   %d\n",counter);printf("\nCounter:   %d\n",cunter);
end=clock();printf("The interval was: %f seconds\n",(double)(end-start)/(double)CLOCKS_PER_SEC);
fclose(fp1);fclose(fp2);exit(0);}
