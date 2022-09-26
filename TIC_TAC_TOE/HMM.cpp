#include "stdafx.h"
#include "stdio.h"
#include<stdlib.h>
#include<math.h>
#include <string.h>
#include<ctype.h>
#include <windows.h>

#include "HMM.h"
int   N=5;
int  M=32;
int T;
long double alpha_array[151][6];
long double beta_array[151][6];
long double gamma_array[151][6];
long double zie_array[151][6][6];
long double B_it_array[6][33];
long double A_it_array[6][6];
long double P_it_array[6];
int obs_seq_array[21][151];
long double delta[151][6];
int sie[151][6];
long double p_star;
long double prev_p_star;
int qt_star[151];
long double new_B_it_array[6][33];
long double new_A_it_array[6][6];
long double ci_values[21][150][12];
long double codebook[32][12];
long double average_B_it_array[6][33];
long double average_A_it_array[6][6];
long double intial_B_it_array[6][33];
long double intial_A_it_array[6][6];
long double P_comparision_array[10];
long double Prob_o_lemda_forword;
long double final_B_it_array[10][6][33];
long double final_A_it_array[10][6][6];
int  PLAYER_1=1;
int  PLAYER_2 =2;
  
int  SIDE =3; // Length of the board
 char board[3][3];
// Computer will move with 'O'
// and human with 'X'
char PLAYER_1MOVE ='O';
char  PLAYER_2MOVE ='X';
FILE *output;
void reading_lembda()
{ 

	for(int j=1;j<=5;j++)
	{
	if(j==1)
        P_it_array[j]=1;
		else
			P_it_array[j]=0;
	}
	for(int i=1;i<=5;i++)
	{
		
		for(int j=1;j<=32;j++)   
	{
		long double c1=1;
		long double c2=32;
		long double c;
		c=c1/c2;
		B_it_array[i][j]=c;
	}

	}
	int temp1=1;
	for(int i=1;i<=5;i++)
	{
	for(int j=1;j<=5;j++)
	{
		if(i<=4)
		{
		if(temp1==j)
       A_it_array[i][j]=0.8;
		else if(temp1+1==j)
			 A_it_array[i][j]=0.2;
		else
			A_it_array[i][j]=0;
		}
		else
		{
             if(j==5)
       A_it_array[i][j]=1;
		else 
			 A_it_array[i][j]=0;
		}
	}
	temp1++;
	}
	
	
	
}
void intialization()
{    p_star=0;

for(int t=0;t<=T;t++)
{
	for(int i=0;i<=N;i++)
	{
		alpha_array[t][i]=0;
		beta_array[t][i]=0;
		gamma_array[t][i]=0;
			delta[t][i]=0;
			sie[t][i]=0;
	}
	qt_star[t]=0;
}
for(int i=0;i<=N;i++)
	{
		for(int j=0;j<=N;j++)
	    {
		  for(int t=0;t<=T;t++)
           {
	          zie_array[t][i][j]=0;
           }
	   }
	}

}
void forword_process( int o)
{   int temp;
   

	
	for(int i=1;i<=N;i++)
	{    temp=obs_seq_array[o][1];
		alpha_array[1][i]=P_it_array[i]*B_it_array[i][temp];
		
	}
	for(int t=1;t<=T-1;t++)
	{
		for(int j=1;j<=N;j++)
		{
			double sum=0;
			for(int i=1;i<=N;i++)
			{
              sum=sum+alpha_array[t][i]*A_it_array[i][j];
			}
			 temp=obs_seq_array[o][t+1];
			alpha_array[t+1][j]=sum*B_it_array[j][temp];
			
		}
		}
	
	Prob_o_lemda_forword=0;
	for(int i=1;i<=N;i++)
	{
		Prob_o_lemda_forword=Prob_o_lemda_forword+alpha_array[T][i];
	}
	
	
	
	
}

void backword_process(int o)
{   int temp;
    
	
	
	for(int i=1;i<=N;i++)
	{    
		beta_array[T][i]=1;
	}
	for(int t=T-1;t>0;t--)
	{
		for(int i=1;i<=N;i++)
		{
			long double sum=0;
			
			for(int j=1;j<=N;j++)
			{
				 temp=obs_seq_array[o][t+1];
                sum=sum+A_it_array[i][j]*B_it_array[j][temp]*beta_array[t+1][j];
             
			}
			
			beta_array[t][i]=sum;
			
			
		}

	}
	
	long double Prob_o_lemda=0;
	for(int i=1;i<=N;i++)
	{
		 temp=obs_seq_array[o][1];
		Prob_o_lemda=Prob_o_lemda+P_it_array[i]*beta_array[0][i]*B_it_array[i][temp];
	}
	
	
}
void viterbi_algo(int o)
{
	int temp;
   
	
	temp=obs_seq_array[o][1];
	for(int i=1;i<=N;i++)
	{
      delta[1][i]=P_it_array[i]*B_it_array[i][temp];
	  sie[1][i]=0;
	}
	long double mx;
	int index;
	for(int t=2;t<=T;t++)
	{
		for(int j=1;j<=N;j++)
		{
			for(int i=1;i<=N;i++)
			{
				if(i==1)
		      {
				  mx=delta[t-1][i]*A_it_array[i][j];
				  index=i;
		      }
			  else if(delta[t-1][i]*A_it_array[i][j]>mx)
			  {
                
				  mx=delta[t-1][i]*A_it_array[i][j];
				  index=i;
			  }
			}
			temp=obs_seq_array[o][t];
			delta[t][j]=mx*B_it_array[j][temp];
			sie[t][j]=index;
		}
	}
	for(int i=1;i<=N;i++)
	{
		if(i==1)
		{
			 p_star=delta[T][i];
	        qt_star[T]=i;
		}
		else if(delta[T][i]>p_star)
		{
			p_star=delta[T][i];
	        qt_star[T]=i;
		}
	}
	
	for(int t=T-1;t>0;t--)
	{
		temp=qt_star[t+1];
		qt_star[t]=sie[t+1][temp];
	}
	
	
	
	
	
}
	
	

void lembda_reconstruction(int o)
{
	 int temp;
	long double denomerator;
	  long double numerator;
	 for(int t=1;t<=T-1;t++)
  { 
	    denomerator=0;
	  for(int i1=1;i1<=N;i1++)
	  {
		  for(int j1=1;j1<=N;j1++)
		  {   temp=obs_seq_array[o][t+1];
		     long double x=alpha_array[t][i1]*B_it_array[j1][temp];
			 long   double y=x*A_it_array[i1][j1];
			  denomerator=denomerator+y*beta_array[t+1][j1];

		  }
	  }
		 for(int i=1;i<=N;i++)
	  {
		  for(int j=1;j<=N;j++)
		  {
	  temp=obs_seq_array[o][t+1];
	  long double x=alpha_array[t][i]*B_it_array[j][temp];
			long   double y=x*A_it_array[i][j];
			numerator=y*beta_array[t+1][j];
			zie_array[t][i][j]=numerator/denomerator;
			
		  }
	   }
	 }
	
	 long double sum;
	 for(int t=1;t<=T-1;t++)
  {
	  for(int i=1;i<=N;i++)
	  { sum=0;
		  for(int j=1;j<=N;j++)
		  {   
			  sum=sum+zie_array[t][i][j];
		  }
       gamma_array[t][i]=sum;
	 
	  }
	 
  }
	 
	 for(int i=1;i<=N;i++)
  {
	  for(int j=1;j<=N;j++)
	  {
		 numerator=0;
		denomerator=0;
		  for(int t=1;t<=T-1;t++)
		  {   
			  numerator=numerator+zie_array[t][i][j];
			 
			 
		  }
		  for(int t=1;t<=T-1;t++)
		  {   
			 
			  denomerator=denomerator+gamma_array[t][i];
			 
		  }
		 
			  new_A_it_array[i][j]=numerator/denomerator;
		
			  
			 
	  }
	   
	  
  }
	  for(int i=1;i<=N;i++)
  {
	  for(int j=1;j<=M;j++)
	  {
		 numerator=0.0;
		denomerator=0.0;
		  for(int t=1;t<=T-1;t++)
		  {    temp=obs_seq_array[o][t];
			  if(j==temp)
			  {
			  numerator=numerator+gamma_array[t][i];
			  }
			  denomerator=denomerator+gamma_array[t][i];
			 
		  }
		
	
		  if(numerator/denomerator>=1.000000e-030)
			  new_B_it_array[i][j]=numerator/denomerator;
		  else
          new_B_it_array[i][j]=1.000000e-030;
		
			  
			 
	  }
	  
	  
  }
  
	
}
void copy_matrix()
{
	for(int i=1;i<=N;i++)
	{
		for(int j=1;j<=N;j++)
		{
			A_it_array[i][j]=new_A_it_array[i][j];
		}
	}
	for(int i=1;i<=N;i++)
	{
		for(int j=1;j<=M;j++)
		{
			B_it_array[i][j]=new_B_it_array[i][j];
		}
	}
}



void reading_codebok()
{
	FILE *inputfile;// for holding the pointer to input file
	long double y;
	inputfile=fopen("new_codebook.txt","r");
	for(int i=0;i<32;i++)
	{
		for(int j=0;j<12;j++)
		{  fscanf(inputfile,"%Lf ",&y);
			codebook[i][j]=y;
			
		}
		fscanf(inputfile,"\n ");
		
	}
	fclose(inputfile);
}
void generate_observation_sequence(int o,int index)
{  double tokhura_weights[12]={1.0,3.0,7.0,13.0,19.0,22.0,25.0,33.0,42.0,50.0,56.0,61.0};// array for tokhura weights
    long double dis;
	long double min_dis;
	int obs_no;
	for(int t=0;t<T;t++)
	{
		for(int i=0;i<32;i++)
		{   dis=0;
			for(int j=0;j<12;j++)
			{
				dis=dis+tokhura_weights[j]*(codebook[i][j]-ci_values[index][t][j])*(codebook[i][j]-ci_values[index][t][j]);
			}
			if(i==0)
			{
				 min_dis=dis;
				 obs_no=i+1;
			}
			else if(dis<min_dis)
			{
				min_dis=dis;
				 obs_no=i+1;
			}
		}
       obs_seq_array[o][t+1]=obs_no;
	   
	}
}
void stochastic_check()
{
	for(int i=1;i<=5;i++)
	{
		long double sum=0;
		int index;
		long double max;
		for(int j=1;j<=5;j++)
		{
			sum=sum+A_it_array[i][j];
			if(j==1)
			{
				max=A_it_array[i][j];
				index=j;
			}
			else
			{
				if(A_it_array[i][j]>max)
				{
					max=A_it_array[i][j];
					index=j;
				}
			}
		}
		if(sum<1)
		{
			sum=1-sum;
			A_it_array[i][index]=A_it_array[i][index]+sum;
		}
		else if(sum>1)
		{
			sum=sum-1;
			A_it_array[i][index]=A_it_array[i][index]-sum;
		}
	}
	for(int i=1;i<=5;i++)
	{
		long double sum=0;
		int index;
		long double max;
		for(int j=1;j<=32;j++)
		{
			sum=sum+B_it_array[i][j];
			if(j==1)
			{
				max=B_it_array[i][j];
				index=j;
			}
			else
			{
				if(B_it_array[i][j]>max)
				{
					max=B_it_array[i][j];
					index=j;
				}
			}
		}
		if(sum<1)
		{
			sum=1-sum;
			B_it_array[i][index]=B_it_array[i][index]+sum;
		}
		else if(sum>1)
		{
			sum=sum-1;
			B_it_array[i][index]=B_it_array[i][index]-sum;
		}
	}
}

void intialization_matrix()
{
	for(int i=1;i<=N;i++)
	{
		for(int j=1;j<=N;j++)
		{
			A_it_array[i][j]=intial_A_it_array[i][j];
		}
	}
	for(int i=1;i<=N;i++)
	{
		for(int j=1;j<=M;j++)
		{
			B_it_array[i][j]=intial_B_it_array[i][j];
		}
	}
}
// A function to show the current board status
int speack_digit()
{ reading_codebok();
	char ch1[1000];
	long double d1;
   printf("enter the voice recording is started for 3 sec\n");
		 system("R\\Recording_Module.exe 3 test_live.wav test_live.txt");
		 FILE *inputfile;
	    FILE *output;
		output=fopen("test_op.txt","w");
	FILE *finalfile;
	finalfile=fopen("finalfile2.txt","r");
	for(int i=0;i<=9;i++)
	 {
		 
		 fgets(ch1,1000,finalfile);
	fscanf(finalfile,"\n");
		  for(int i1=1;i1<=5;i1++)
		  {
			  for(int j1=1;j1<=5;j1++)
			  {
				   fscanf(finalfile,"%Lf ",&d1);
				  
				   final_A_it_array[i][i1][j1]=d1;
			  }
			   fscanf(finalfile,"\n");
			  
		  }
		   fgets(ch1,1000,finalfile);
	fscanf(finalfile,"\n");
	for(int i1=1;i1<=5;i1++)
		  {
			  for(int j1=1;j1<=32;j1++)
			  {
				   fscanf(finalfile,"%Le ",&d1);
				   final_B_it_array[i][i1][j1]=d1;
			  }
			   fscanf(finalfile,"\n");
			    
		  }
		 
		  fgets(ch1,1000,finalfile);
	fscanf(finalfile,"\n");
	}
	inputfile=fopen("test_live.txt","r");
	


	char ch[30];//temporary char array to hold every  line of file for checking
	long double dcshift=0;//for calculating avg dcshift
	long double count1=0;// no of values in dc file
	//loop for calculating dc shift
	FILE *dcfile;// for holding dcshift file
	
	dcfile=fopen("dctest.txt","r");
	
	
	while(!feof(dcfile))
	{
     fgets(ch,30,dcfile);
	 if(!isalpha(ch[0]))
	 {
		 count1++;
		 
		 int num1=atoi(ch);
		 
		 dcshift=dcshift+num1;
		 
	 }

	}
	// take avg of dc shift for use
	dcshift=dcshift/count1;
	int count=0;
	long double x_values[321];// for holding sample of 1 frames
	long double r=0;// for caluculating Ri values
	long double max_xi=0;// for normalization purpose find max x value of sample
	int sample_no=0;// for track for sample count no
	int j,i;// helping varibles i loop
	long double z;// temporaray  varible for help in normalization
	int index_r=0;// for keep track of index for r_values array
	long double r_values[13];// temporaray array for holding ri values of each frame of each file
	long double c_values[13];// temporaray array for holding ci values of each frame of each file 
	int no_frames=0;// for count of num of frames
	long double temp;// temporaray varible to hold some values
	long double a_values[13][13];// temporary double array to store ai values of each frame of each file
	long double kv[13];// temporary double array to store k  values for help in calculating a_values
	long double e1=0;// temporaray varible for helping in finding energy for steady part
	long double e2=0;
	long double ei=0;
	int frame_no=0;
	int steady_frame;
	long double ste=0;
	double input_values[100000];
	int row=0;
int col=0;
	int no_values=0 ;
	// loop for finding the staedy frames pick left 2 maximum ste and right 2
	
	
	
	int x;
	 while(!feof(inputfile))
    {
		 fgets(ch,30,inputfile);
	 if(!isalpha(ch[0]))
	 {
	  
		      x=atoi(ch);
			
			 input_values[no_values]=x;
			    no_values++;
	
			      if(x<0)
			         {
				      x=0-x;
			         }
			        if(x>max_xi)//find maxi for normalization
			         {
				      max_xi=x;
			         }
	 }
					
		
}
	
	  
	  count=0;
	 no_frames=0;
	 double y;
	 for(int it=0;it<no_values;it++)
	{
        count++;
		y=input_values[it];
		e1=e1+y*y;
		if(count==320)
			 {  
				 frame_no++;
			    // e1=e1/320;
			    if(frame_no==1)
					ei=e1;
			  
				 if(e1>ste)
				 {
					 ste=e1;
					 steady_frame=frame_no;
					
				 }
				 it=it-240;
				 count=0;
				 e1=0;
		    }
	 }
	
	 steady_frame=steady_frame-40;
	 int b=0;
	  count=0;
	for(int it=0;it<no_values;it++)
	{
		     count++;
			double x=input_values[it];
			e2=e2+x*x;
			// no_frames>steady_frame&&no_frames<(steady_frame+85)
			if(b==1&&row<150)// pick steady part manully or for 5 frames from steady part
			{
				
				    if(count==1)//just to reset values so another frame values work fine ..like array overwriting
				    {
					
					 sample_no=0;
					 index_r=0;  
				    }
			        x_values[sample_no]=x;//store the value
			        
			   sample_no++;
		 if(count==320)
			  {   
                       // do dcshift+normalization take limit 5000
			       
			//  formulla=x*5000/max_xi;
			  
	         for(sample_no=0;sample_no<320;sample_no++)
	          {
		       z=x_values[sample_no]-dcshift;
		       z=z*5000;
			   z=z/max_xi;
			   x_values[sample_no]=z;
	          }
	      //calculate ri values for each frame
			 
			// autocorelation formula
			// r=r+x[i]*x[i+k];
			// where k lag;
			 
	      for(j=0;j<13;j++)
	        {
		     r=0;
	      for(sample_no=0;sample_no<320-j;sample_no++)
	        { 
			 int y=sample_no+j;
		      r=r+x_values[sample_no]*x_values[y];	
	       }
	    r_values[index_r]=r;// store r values
	    index_r++;
	    }
	   double s=0;// temporay varible for help in ai values find
	   double e;
		   e=r_values[0];//for holding e values
	  // calulation ai values by levinson durbin algo for p=12;
	 //  here a_values[12][i]=ai for i=1,2,..12 because p=12
	   
	   for(i=1;i<=12;i++)
	   {
		s=0;
       for(j=1;j<=i-1;j++)// for summetion j=1 to i-1
	    {
		 s=s+a_values[i-1][j]*r_values[i-j];
	    }
	   //ki=ri-s/previos energy(e(i-1))
	    kv[i]=(r_values[i]-s)/e;
	    a_values[i][i]=kv[i];// store value
	    for(j=1;j<=i-1;j++)//for summetion j=1 to i-1
	    {

		  a_values[i][j]=a_values[i-1][j]-(kv[i]*a_values[i-1][i-j]);
	    }
	    e=(1-(kv[i]*kv[i]))*e;// calculate next energy
	  }
	
	// now for cepstral coefficentsci
	   //co=log(singma*singma); singma=r[0]
	temp=r_values[0]*r_values[0];
	temp=log(temp);
	c_values[0]=temp;
	double m ,k,wm;// temp varible for helping calculation
	
	     for(i=1;i<=12;i++)//for m 1 to 12 p=12;
	      {
		        c_values[i]=0;
		        m=i;
		    for(j=1;j<=i-1;j++)//summetion from j=k=1 to i-1=m-1 
		           {      k=j;
			            c_values[i]=c_values[i]+(k/m)*c_values[j]*a_values[12][i-j];// using formula
	     	       }
		     c_values[i]=c_values[i]+a_values[12][i];
		 temp=3.14*i/12;// now applying raise sine window to ci values by formula w(m)=1+(q*sin((pie*m)/q))/2 here q=p=12 and m=i from 1 to 12
			 wm=1+6*sin(temp);
		    ci_values[1][row][col]=c_values[i]*wm;// storing after appplying raise sine window
			  
		     col++;
	        }
		 //for index adjustment for array overwriting correctly
	                  row++;
					  
	                  col=0;
	        
		          }

				
			}
			if(count==320)
			{
             no_frames++;
			 count=0;
			 if(e2>4*ei)
			 {
				 b=1;
			 }
			 else
				 b=0;
			 e2=0;
			  it=it-240;
			}
			
	}
	T=row;
	   obs_seq_array[1][0]=T;
	fclose(dcfile);


	 generate_observation_sequence(1,1);
	 
	 for(i=0;i<=9;i++)
	 {
		 for(int i1=1;i1<=5;i1++)
		  {
			  if(i1==1)
              P_it_array[i1]=1;

			  else
			  P_it_array[i1]=0;

			 
			  
		  }
		  for(int i1=1;i1<=5;i1++)
		  {
			  for(int j1=1;j1<=5;j1++)
			  {
				  
				  
				   A_it_array[i1][j1]=final_A_it_array[i][i1][j1];
				   
			  }
			  
			   
		  }
		 
		  for(int i1=1;i1<=5;i1++)
		  {
			  for(int j1=1;j1<=32;j1++)
			  {
				  
				   B_it_array[i1][j1]=final_B_it_array[i][i1][j1];
				   
			  }
			  
			   
		  }
		  forword_process(1);
	 P_comparision_array[i]=Prob_o_lemda_forword;
	
    }
	 long double mx;
	 int index;
	 
	 for(i=0;i<10;i++)
	 {
		 if(i==0)
		 {
			 mx=P_comparision_array[i];
		 index=i;
		 }
		 else if(mx<P_comparision_array[i])
		 {
			  mx=P_comparision_array[i];
		    index=i;
		 }
	 }
	 printf("the digit is :-%d\n",index);
	 Sleep(1000);
	fclose(inputfile);
	 fclose(finalfile);
	 fclose(output);
	 return index;
}
// 0-yes 1-no 2-start 3-stop
int speack_word()
{
	FILE *codefile;// for holding the pointer to input file
	long double value;
	codefile=fopen("word_codebook.txt","r");
	for(int i=0;i<32;i++)
	{
		for(int j=0;j<12;j++)
		{  fscanf(codefile,"%Lf ",&value);
			codebook[i][j]=value;
			
		}
		fscanf(codefile,"\n ");
		
	}
	fclose(codefile);
	char ch1[1000];
	long double d1;
   printf("enter the voice recording is started for 3 sec\n");
		 system("R\\Recording_Module.exe 3 test_live.wav test_live.txt");
		 FILE *inputfile;
	    FILE *output;
		output=fopen("test_op.txt","w");
	FILE *finalfile;
	finalfile=fopen("finalfile_word.txt","r");
	for(int i=0;i<=3;i++)
	 {
		 
		 fgets(ch1,1000,finalfile);
	fscanf(finalfile,"\n");
		  for(int i1=1;i1<=5;i1++)
		  {
			  for(int j1=1;j1<=5;j1++)
			  {
				   fscanf(finalfile,"%Lf ",&d1);
				  
				   final_A_it_array[i][i1][j1]=d1;
			  }
			   fscanf(finalfile,"\n");
			  
		  }
		   fgets(ch1,1000,finalfile);
	fscanf(finalfile,"\n");
	for(int i1=1;i1<=5;i1++)
		  {
			  for(int j1=1;j1<=32;j1++)
			  {
				   fscanf(finalfile,"%Le ",&d1);
				   final_B_it_array[i][i1][j1]=d1;
			  }
			   fscanf(finalfile,"\n");
			    
		  }
		 
		  fgets(ch1,1000,finalfile);
	fscanf(finalfile,"\n");
	}
	inputfile=fopen("test_live.txt","r");
	


	char ch[30];//temporary char array to hold every  line of file for checking
	long double dcshift=0;//for calculating avg dcshift
	long double count1=0;// no of values in dc file
	//loop for calculating dc shift
	FILE *dcfile;// for holding dcshift file
	
	dcfile=fopen("dctest.txt","r");
	
	
	while(!feof(dcfile))
	{
     fgets(ch,30,dcfile);
	 if(!isalpha(ch[0]))
	 {
		 count1++;
		 
		 int num1=atoi(ch);
		 
		 dcshift=dcshift+num1;
		 
	 }

	}
	// take avg of dc shift for use
	dcshift=dcshift/count1;
	int count=0;
	long double x_values[321];// for holding sample of 1 frames
	long double r=0;// for caluculating Ri values
	long double max_xi=0;// for normalization purpose find max x value of sample
	int sample_no=0;// for track for sample count no
	int j,i;// helping varibles i loop
	long double z;// temporaray  varible for help in normalization
	int index_r=0;// for keep track of index for r_values array
	long double r_values[13];// temporaray array for holding ri values of each frame of each file
	long double c_values[13];// temporaray array for holding ci values of each frame of each file 
	int no_frames=0;// for count of num of frames
	long double temp;// temporaray varible to hold some values
	long double a_values[13][13];// temporary double array to store ai values of each frame of each file
	long double kv[13];// temporary double array to store k  values for help in calculating a_values
	long double e1=0;// temporaray varible for helping in finding energy for steady part
	long double e2=0;
	long double ei=0;
	int frame_no=0;
	int steady_frame;
	long double ste=0;
	double input_values[100000];
	int row=0;
int col=0;
	int no_values=0 ;
	// loop for finding the staedy frames pick left 2 maximum ste and right 2
	
	
	
	int x;
	 while(!feof(inputfile))
    {
		 fgets(ch,30,inputfile);
	 if(!isalpha(ch[0]))
	 {
	  
		      x=atoi(ch);
			
			 input_values[no_values]=x;
			    no_values++;
	
			      if(x<0)
			         {
				      x=0-x;
			         }
			        if(x>max_xi)//find maxi for normalization
			         {
				      max_xi=x;
			         }
	 }
					
		
}
	
	  
	  count=0;
	 no_frames=0;
	 double y;
	 for(int it=0;it<no_values;it++)
	{
        count++;
		y=input_values[it];
		e1=e1+y*y;
		if(count==320)
			 {  
				 frame_no++;
			    // e1=e1/320;
			    if(frame_no==1)
					ei=e1;
			  
				 if(e1>ste)
				 {
					 ste=e1;
					 steady_frame=frame_no;
					
				 }
				 it=it-240;
				 count=0;
				 e1=0;
		    }
	 }
	
	 steady_frame=steady_frame-40;
	 int b=0;
	  count=0;
	for(int it=0;it<no_values;it++)
	{
		     count++;
			double x=input_values[it];
			e2=e2+x*x;
			// no_frames>steady_frame&&no_frames<(steady_frame+85)
			if(b==1&&row<150)// pick steady part manully or for 5 frames from steady part
			{
				
				    if(count==1)//just to reset values so another frame values work fine ..like array overwriting
				    {
					
					 sample_no=0;
					 index_r=0;  
				    }
			        x_values[sample_no]=x;//store the value
			        
			   sample_no++;
		 if(count==320)
			  {   
                       // do dcshift+normalization take limit 5000
			       
			//  formulla=x*5000/max_xi;
			  
	         for(sample_no=0;sample_no<320;sample_no++)
	          {
		       z=x_values[sample_no]-dcshift;
		       z=z*5000;
			   z=z/max_xi;
			   x_values[sample_no]=z;
	          }
	      //calculate ri values for each frame
			 
			// autocorelation formula
			// r=r+x[i]*x[i+k];
			// where k lag;
			 
	      for(j=0;j<13;j++)
	        {
		     r=0;
	      for(sample_no=0;sample_no<320-j;sample_no++)
	        { 
			 int y=sample_no+j;
		      r=r+x_values[sample_no]*x_values[y];	
	       }
	    r_values[index_r]=r;// store r values
	    index_r++;
	    }
	   double s=0;// temporay varible for help in ai values find
	   double e;
		   e=r_values[0];//for holding e values
	  // calulation ai values by levinson durbin algo for p=12;
	 //  here a_values[12][i]=ai for i=1,2,..12 because p=12
	   
	   for(i=1;i<=12;i++)
	   {
		s=0;
       for(j=1;j<=i-1;j++)// for summetion j=1 to i-1
	    {
		 s=s+a_values[i-1][j]*r_values[i-j];
	    }
	   //ki=ri-s/previos energy(e(i-1))
	    kv[i]=(r_values[i]-s)/e;
	    a_values[i][i]=kv[i];// store value
	    for(j=1;j<=i-1;j++)//for summetion j=1 to i-1
	    {

		  a_values[i][j]=a_values[i-1][j]-(kv[i]*a_values[i-1][i-j]);
	    }
	    e=(1-(kv[i]*kv[i]))*e;// calculate next energy
	  }
	
	// now for cepstral coefficentsci
	   //co=log(singma*singma); singma=r[0]
	temp=r_values[0]*r_values[0];
	temp=log(temp);
	c_values[0]=temp;
	double m ,k,wm;// temp varible for helping calculation
	
	     for(i=1;i<=12;i++)//for m 1 to 12 p=12;
	      {
		        c_values[i]=0;
		        m=i;
		    for(j=1;j<=i-1;j++)//summetion from j=k=1 to i-1=m-1 
		           {      k=j;
			            c_values[i]=c_values[i]+(k/m)*c_values[j]*a_values[12][i-j];// using formula
	     	       }
		     c_values[i]=c_values[i]+a_values[12][i];
		 temp=3.14*i/12;// now applying raise sine window to ci values by formula w(m)=1+(q*sin((pie*m)/q))/2 here q=p=12 and m=i from 1 to 12
			 wm=1+6*sin(temp);
		    ci_values[1][row][col]=c_values[i]*wm;// storing after appplying raise sine window
			  
		     col++;
	        }
		 //for index adjustment for array overwriting correctly
	                  row++;
					  
	                  col=0;
	        
		          }

				
			}
			if(count==320)
			{
             no_frames++;
			 count=0;
			 if(e2>4*ei)
			 {
				 b=1;
			 }
			 else
				 b=0;
			 e2=0;
			  it=it-240;
			}
			
	}
	T=row;
	   obs_seq_array[1][0]=T;
	fclose(dcfile);




	  generate_observation_sequence(1,1);
	 
	 for(i=0;i<=3;i++)
	 {
		 for(int i1=1;i1<=5;i1++)
		  {
			  if(i1==1)
              P_it_array[i1]=1;

			  else
			  P_it_array[i1]=0;

			 
			  
		  }
		  for(int i1=1;i1<=5;i1++)
		  {
			  for(int j1=1;j1<=5;j1++)
			  {
				  
				  
				   A_it_array[i1][j1]=final_A_it_array[i][i1][j1];
				   
			  }
			  
			   
		  }
		 
		  for(int i1=1;i1<=5;i1++)
		  {
			  for(int j1=1;j1<=32;j1++)
			  {
				  
				   B_it_array[i1][j1]=final_B_it_array[i][i1][j1];
				   
			  }
			  
			   
		  }
		  forword_process(1);
	 P_comparision_array[i]=Prob_o_lemda_forword;
	
    }
	 long double mx;
	 int index;
	 
	 for(i=0;i<4;i++)
	 {
		 if(i==0)
		 {
			 mx=P_comparision_array[i];
		 index=i;
		 }
		 else if(mx<P_comparision_array[i])
		 {
			  mx=P_comparision_array[i];
		    index=i;
		 }
	 }
	// printf("the digit is :-%d\n",index);

	fclose(inputfile);
	 fclose(finalfile);
	 fclose(output);
	 return index;
}
void print_Board(char board[][3])
{
    printf("\n\n");
      
    printf("\t\t\t  %c | %c  | %c  \n", board[0][0],
                             board[0][1], board[0][2]);
    printf("\t\t\t--------------\n");
    printf("\t\t\t  %c | %c  | %c  \n", board[1][0],
                             board[1][1], board[1][2]);
    printf("\t\t\t--------------\n");
    printf("\t\t\t  %c | %c  | %c  \n\n", board[2][0],
                             board[2][1], board[2][2]);
   
    return;
}
  

void showInstructions()
{
    printf("\t\t\t  Tic-Tac-Toe\n\n");
    printf("Choose a cell numbered from 1 to 9 as below"
            " and play\n\n");
      
    printf("\t\t\t  1 | 2  | 3  \n");
    printf("\t\t\t--------------\n");
    printf("\t\t\t  4 | 5  | 6  \n");
    printf("\t\t\t--------------\n");
    printf("\t\t\t  7 | 8  | 9  \n\n");
      
    printf("-\t-\t-\t-\t-\t-\t-\t-\t-\t-\n\n");
  
    return;
}
  
  
 
void initialise(char board[][3])
{
    // 
      
   
    for (int i=0; i<SIDE; i++)
    {
        for (int j=0; j<SIDE; j++)
            board[i][j] = ' ';
    }
      
    
      
    return;
}

// checking for end of game
int end_game(char board[][3])
{
    for (int i=0; i<SIDE; i++)
    {
        if (board[i][0] == board[i][1] &&
            board[i][1] == board[i][2] && 
            board[i][0] != ' ')
            return 1;
    }
	for (int i=0; i<SIDE; i++)
    {
        if (board[0][i] == board[1][i] &&
            board[1][i] == board[2][i] && 
            board[0][i] != ' ')
            return 1;
    }
	if (board[0][0] == board[1][1] &&
        board[1][1] == board[2][2] && 
        board[0][0] != ' ')
        return 1;
          
    if (board[0][2] == board[1][1] &&
        board[1][1] == board[2][0] &&
         board[0][2] != ' ')
        return 1;
    return 0;
}
  

  
// A function to play Tic-Tac-Toe
int playTicTacToe(int Turn)
{
    // A 3*3 Tic-Tac-Toe board for playing 
    
      
   
      
    // Initialise the game
    initialise(board);
      
    // Show the instructions before playing
    showInstructions();
      
    int moves = 0, x, y,z,z1=1;
	int quite=1;
      int b;
	  Sleep(10000);
	  while(z1!=2)
	  {
	  printf("speak start for starting the game in few seconds when recording started\n");
	   //system("pause");
			Sleep(10000);
			z1=speack_word();
	  }
    // Keep playing till the game is over or it is a draw
    while (end_game(board) == 0 && 
            moves != SIDE*SIDE&&quite)
    {
        if (Turn == PLAYER_1)
        {
            printf("PLAYER_1 turn:-\n");
			b=1;
			while(b)
			{
            printf("SPEAK THE CELL NUM in few seconds when recording started \n");
			//system("pause");
			Sleep(10000);
            z=speack_digit();
			printf("  THE CELL NUM IS %d \n",z);
			
			 x=(z-1)/3;
			y=(z-1)-3*x;
			if(z!=0&&board[x][y] ==' ')
			{
			printf("speak yes if num is right ,else no,speak stop for quiting the game\n");
			//system("pause");
			Sleep(10000);
			z1=speack_word();
				if(z1==0)
					b=0;
				if(z1==3)
					quite=0;
			}
			else
			{
				printf("num i wrong\n");
			}
			}
			
		
		  
            board[x][y] = PLAYER_1MOVE;
            printf("PLAYER_1 has put a %c in cell %d\n",
                    PLAYER_1MOVE,x*SIDE+y+1 );
            print_Board(board);
            moves ++;
            Turn = PLAYER_2;
        }
          
        else if (Turn == PLAYER_2)
        {
         
            printf("PLAYER_2  turn:-\n");
           b=1;
			while(b)
			{
            printf("SPEAK THE CELL NUM in few sec when recording started\n");
		   Sleep(10000);
			//system("pause");
            z=speack_digit();
			x=(z-1)/3;
			y=(z-1)-3*x;
			printf("  THE CELL NUM IS %d \n",z);
			if(z!=0&&board[x][y] ==' ')
			{
			printf("speak yes if num is right ,else no,speak stop for quiting the game\n");
			//system("pause");
			Sleep(10000);
			z1=speack_word();
				if(z1==0)
					b=0;
				if(z1==3)
					quite=0;
			}
			else
			{
				printf("num i wrong\n");
			}
			}
			
            board[x][y] = PLAYER_2MOVE;
            printf ("PLAYER_2 has put a %c in cell %d\n",
                   PLAYER_2MOVE, x*SIDE+y+1);
            print_Board(board);
            moves ++;
            Turn = PLAYER_1;
        }
    }
  
    // If the game has drawn
    if ((end_game(board) == 0 && 
            moves == SIDE * SIDE)||quite==0)
	{
        printf("It's a draw\n");
		Turn=0;
	}

    else
    {
        // Toggling the user to declare the actual
        // winner
        if (Turn == PLAYER_1)
            Turn = PLAYER_2;
        else if (Turn == PLAYER_2)
            Turn = PLAYER_1;
          
         if (Turn == PLAYER_1)
        printf("PLAYER_1 has won\n");
    else
        printf("PL2YER_1 has won\n");
    }
	printf("for quit the game speak stop,else for playing again speak start\n");
	Sleep(10000);
			z1=speack_word();
			if(z1==2)
           return playTicTacToe(1);
    return Turn;
}
  int start()
  {
	 return playTicTacToe(1);
  }
  void training()
  {  
	  int num;
	  printf("enter the digit num you want to train from 0 to 9\n");
	  scanf("%d",&num);
	  printf("speak the digit 10 times when recording started\n");
	  Sleep(5000);
	  char s[100]="R\\Recording_Module.exe 3 0_0.wav 0_0.txt";
	  s[25]=48+num;
	  s[33]=48+num;
	  for(int i=0;i<=9;i++)
	  {
		  
		  s[27]=48+i;
		  s[35]=48+i;
		  system(s);
		  
	  }
	  printf("recorded files are presented in the project folder you can check\n");
	  int opt;
	  printf("if recordings are fine enter 1 else enter 0 for go back\n");
	  printf("or cut them by cool edit and let their name remains same\n");
	  scanf("%d",&opt);
	  if(opt==1)
	  {
		  
	  printf("wait while it's training....\n");
	  FILE *inputfile;// for holding the pointer to input file
	FILE *finalfile;// for holding the pointer to input file
	finalfile=fopen("finalfile.txt","w");
	FILE *output;
	
	long double value;
	
	 reading_codebok();
	for(int i=1;i<=5;i++)
	{
		for(int j=1;j<=5;j++)
		{
			average_A_it_array[i][j]=0;
		}
	}
	for(int i=1;i<=5;i++)
	{
		for(int j=1;j<=32;j++)
		{
			average_B_it_array[i][j]=0;
		}
	}
	char out[29]="output_0_0.txt";
		  out[7]=48+num;
		  output=fopen(out,"w");
	for(int ifn=0;ifn<=9;ifn++)
	  {
		  char filename[20]="0_1.txt";
		  
		  filename[0]=48+num;
		  filename[2]=48+ifn;
		  inputfile=fopen(filename,"r");
		  char ch[30];//temporary char array to hold every  line of file for checking
	long double dcshift=0;//for calculating avg dcshift
	long double count1=0;// no of values in dc file
	//loop for calculating dc shift
	FILE *dcfile;// for holding dcshift file
	
	dcfile=fopen("dctest.txt","r");
	
	
	while(!feof(dcfile))
	{
     fgets(ch,30,dcfile);
	 if(!isalpha(ch[0]))
	 {
		 count1++;
		 
		 int num1=atoi(ch);
		 
		 dcshift=dcshift+num1;
		 
	 }

	}
	// take avg of dc shift for use
	dcshift=dcshift/count1;
	int count=0;
	long double x_values[321];// for holding sample of 1 frames
	long double r=0;// for caluculating Ri values
	long double max_xi=0;// for normalization purpose find max x value of sample
	int sample_no=0;// for track for sample count no
	int j,i;// helping varibles i loop
	long double z;// temporaray  varible for help in normalization
	int index_r=0;// for keep track of index for r_values array
	long double r_values[13];// temporaray array for holding ri values of each frame of each file
	long double c_values[13];// temporaray array for holding ci values of each frame of each file 
	int no_frames=0;// for count of num of frames
	long double temp;// temporaray varible to hold some values
	long double a_values[13][13];// temporary double array to store ai values of each frame of each file
	long double kv[13];// temporary double array to store k  values for help in calculating a_values
	long double e1=0;// temporaray varible for helping in finding energy for steady part
	long double e2=0;
	long double ei=0;
	int frame_no=0;
	int steady_frame;
	long double ste=0;
	double input_values[100000];
	int row=0;
int col=0;
	int no_values=0 ;
	// loop for finding the staedy frames pick left 2 maximum ste and right 2
	
	
	
	int x;
	 while(!feof(inputfile))
    {
		 fgets(ch,30,inputfile);
	 if(!isalpha(ch[0]))
	 {
	  
		      x=atoi(ch);
			
			 input_values[no_values]=x;
			    no_values++;
	
			      if(x<0)
			         {
				      x=0-x;
			         }
			        if(x>max_xi)//find maxi for normalization
			         {
				      max_xi=x;
			         }
	 }
					
		
}
	
	  
	  count=0;
	 no_frames=0;
	 double y;
	 for(int it=0;it<no_values;it++)
	{
        count++;
		y=input_values[it];
		e1=e1+y*y;
		if(count==320)
			 {  
				 frame_no++;
			    // e1=e1/320;
			    if(frame_no==1)
					ei=e1;
			  
				 if(e1>ste)
				 {
					 ste=e1;
					 steady_frame=frame_no;
					
				 }
				 it=it-240;
				 count=0;
				 e1=0;
		    }
	 }
	
	 steady_frame=steady_frame-40;
	 int b=0;
	  count=0;
	for(int it=0;it<no_values;it++)
	{
		     count++;
			double x=input_values[it];
			e2=e2+x*x;
			// no_frames>steady_frame&&no_frames<(steady_frame+85)
			if(b==1&&row<150)// pick steady part manully or for 5 frames from steady part
			{
				
				    if(count==1)//just to reset values so another frame values work fine ..like array overwriting
				    {
					
					 sample_no=0;
					 index_r=0;  
				    }
			        x_values[sample_no]=x;//store the value
			        
			   sample_no++;
		 if(count==320)
			  {   
                       // do dcshift+normalization take limit 5000
			       
			//  formulla=x*5000/max_xi;
			  
	         for(sample_no=0;sample_no<320;sample_no++)
	          {
		       z=x_values[sample_no]-dcshift;
		       z=z*5000;
			   z=z/max_xi;
			   x_values[sample_no]=z;
	          }
	      //calculate ri values for each frame
			 
			// autocorelation formula
			// r=r+x[i]*x[i+k];
			// where k lag;
			 
	      for(j=0;j<13;j++)
	        {
		     r=0;
	      for(sample_no=0;sample_no<320-j;sample_no++)
	        { 
			 int y=sample_no+j;
		      r=r+x_values[sample_no]*x_values[y];	
	       }
	    r_values[index_r]=r;// store r values
	    index_r++;
	    }
	   double s=0;// temporay varible for help in ai values find
	   double e;
		   e=r_values[0];//for holding e values
	  // calulation ai values by levinson durbin algo for p=12;
	 //  here a_values[12][i]=ai for i=1,2,..12 because p=12
	   
	   for(i=1;i<=12;i++)
	   {
		s=0;
       for(j=1;j<=i-1;j++)// for summetion j=1 to i-1
	    {
		 s=s+a_values[i-1][j]*r_values[i-j];
	    }
	   //ki=ri-s/previos energy(e(i-1))
	    kv[i]=(r_values[i]-s)/e;
	    a_values[i][i]=kv[i];// store value
	    for(j=1;j<=i-1;j++)//for summetion j=1 to i-1
	    {

		  a_values[i][j]=a_values[i-1][j]-(kv[i]*a_values[i-1][i-j]);
	    }
	    e=(1-(kv[i]*kv[i]))*e;// calculate next energy
	  }
	
	// now for cepstral coefficentsci
	   //co=log(singma*singma); singma=r[0]
	temp=r_values[0]*r_values[0];
	temp=log(temp);
	c_values[0]=temp;
	double m ,k,wm;// temp varible for helping calculation
	
	     for(i=1;i<=12;i++)//for m 1 to 12 p=12;
	      {
		        c_values[i]=0;
		        m=i;
		    for(j=1;j<=i-1;j++)//summetion from j=k=1 to i-1=m-1 
		           {      k=j;
			            c_values[i]=c_values[i]+(k/m)*c_values[j]*a_values[12][i-j];// using formula
	     	       }
		     c_values[i]=c_values[i]+a_values[12][i];
		 temp=3.14*i/12;// now applying raise sine window to ci values by formula w(m)=1+(q*sin((pie*m)/q))/2 here q=p=12 and m=i from 1 to 12
			 wm=1+6*sin(temp);
		    ci_values[ifn][row][col]=c_values[i]*wm;// storing after appplying raise sine window
			  
		     col++;
	        }
		 //for index adjustment for array overwriting correctly
	                  row++;
					  
	                  col=0;
	        
		          }

				
			}
			if(count==320)
			{
             no_frames++;
			 count=0;
			 if(e2>4*ei)
			 {
				 b=1;
			 }
			 else
				 b=0;
			 e2=0;
			  it=it-240;
			}
			
	}
	T=row;
	   obs_seq_array[ifn][0]=T;
	    generate_observation_sequence(ifn,ifn);
		fprintf(output," %d file \n",ifn);
	fprintf(output," obsevartion sequence is \n");
		for(j=1;j<=T;j++)
	{
	fprintf(output,"%d ",obs_seq_array[ifn][j]);
	}
	fprintf(output,"\n");
		reading_lembda();
		for(i=1;i<=200;i++)
	{
		intialization();
		viterbi_algo(ifn);
		backword_process(ifn);
	    forword_process(ifn);
	
	lembda_reconstruction(ifn);
	
	copy_matrix();
	stochastic_check();
	}
		fprintf(output,"P* AND QT* FOR SEQUENCE is:-\n");
		 fprintf(output,"%Le \n",p_star);
	for(int t=1;t<=T;t++)
	{
		
		fprintf(output,"%d ",qt_star[t]);
	}
	
	fprintf(output,"\n");
		for(i=1;i<=5;i++)
	{
		for(j=1;j<=5;j++)
		{
			average_A_it_array[i][j]=average_A_it_array[i][j]+A_it_array[i][j];
		}
	}
	for(i=1;i<=5;i++)
	{
		for(j=1;j<=32;j++)
		{
			average_B_it_array[i][j]=average_B_it_array[i][j]+B_it_array[i][j];
		}
	}
	
	
	fclose(dcfile);
	}
	long double divide=10;
	for(int i=1;i<=5;i++)
	{
		for(int j=1;j<=5;j++)
		{
			average_A_it_array[i][j]=average_A_it_array[i][j]/divide;
			intial_A_it_array[i][j]= average_A_it_array[i][j];
		}
		
	}
	
	for(int i=1;i<=5;i++)
	{
		for(int j=1;j<=32;j++)
		{
			average_B_it_array[i][j]=average_B_it_array[i][j]/divide;
			intial_B_it_array[i][j]= average_B_it_array[i][j];
		}
		
	}
	char out1[29]="output_0_1.txt";
		  
	      out1[7]=48+num;
		  output=fopen(out1,"w");
	for(int i=1;i<=5;i++)
	   {
		for(int j=1;j<=5;j++)
		{
			average_A_it_array[i][j]=0;
		}
	  }
	for(int i=1;i<=5;i++)
	{
		for(int j=1;j<=32;j++)
		{
			average_B_it_array[i][j]=0;
		}
	}
	for(int ifn=0;ifn<=9;ifn++)
	  {
		  char filename[20]="0_1.txt";
		  filename[0]=48+num;
		  filename[2]=48+ifn;
		  inputfile=fopen(filename,"r");
		  T=obs_seq_array[ifn][0];
		  intialization_matrix();
	for(int i=1;i<=200;i++)
	{
		intialization();
		viterbi_algo(ifn);
		backword_process(ifn);
	    forword_process(ifn);
	
	lembda_reconstruction(ifn);
	
	copy_matrix();
	stochastic_check();
	}
	fprintf(output,"P* AND QT* FOR SEQUENCE is:-\n");
		 fprintf(output,"%Le \n",p_star);
	for(int t=1;t<=T;t++)
	{
		
		fprintf(output,"%d ",qt_star[t]);
	}
	
	fprintf(output,"\n");
	for(int i=1;i<=5;i++)
	{
		for(int j=1;j<=5;j++)
		{
			average_A_it_array[i][j]=average_A_it_array[i][j]+A_it_array[i][j];
		}
	}
	for(int i=1;i<=5;i++)
	{
		for(int j=1;j<=32;j++)
		{
			average_B_it_array[i][j]=average_B_it_array[i][j]+B_it_array[i][j];
		}
	}
	
	
	}
	long double divide1=10;
	for(int i=1;i<=5;i++)
	{
		for(int j=1;j<=5;j++)
		{
			average_A_it_array[i][j]=average_A_it_array[i][j]/divide1;
			intial_A_it_array[i][j]= average_A_it_array[i][j];
		}
		
	}
	
	for(int i=1;i<=5;i++)
	{
		for(int j=1;j<=32;j++)
		{
			average_B_it_array[i][j]=average_B_it_array[i][j]/divide1;
			intial_B_it_array[i][j]= average_B_it_array[i][j];
		}
		
	}
	fprintf(finalfile,"final  matrix a is:\n ");
	for(int i=1;i<=5;i++)
	{
		for(int j=1;j<=5;j++)
		{
			 fprintf(finalfile,"%Lf  ",average_A_it_array[i][j]);
			 
		}
		fprintf(finalfile,"\n  ");
	}
	 fprintf(finalfile,"final matrix b is:\n ");
	for(int i=1;i<=5;i++)
	{
		for(int j=1;j<=32;j++)
		{
			fprintf(finalfile,"%Le  ", average_B_it_array[i][j]);
			
		}
		fprintf(finalfile,"\n  ");
	}
	
	fclose(finalfile);
	fclose(inputfile);
	printf("FINAL MODEL IS IN finalfile.txt\n");
	printf("the p start q star in output_num_0 for first time avg output_num_1 for second time avg\n");
	Sleep(5000);
	}
  }
  void end_print()
  {
	  printf("for seeing other modules go to form page and click on the module button\n");
	  printf("after clicking the button come here to this console for watching the operations\n");
	  printf("for closing the program close the console\n");
  }