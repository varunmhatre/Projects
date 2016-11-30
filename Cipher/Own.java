import java.util.*;

public class Own{
 
    public static void main(String[] args) {
        Scanner sc=new Scanner(System.in);
        System.out.println("Enter string:");
        String pt=sc.next();
	int n=pt.length();
        String ct1="";
        char a[]=pt.toCharArray();

        for(int i=0;i<n;i++)
        {
            	a[i]=(char)(((3*(a[i]-97)+2)%26)+97);
            	ct1+=a[i];
        }       
        System.out.println("\nAfter Custom Subsitution Cipher:\n"+ct1);
        
        String ct2="";
        int row;
	System.out.println("\nEnter number of columns:");
	int col=sc.nextInt();
	if(n%col!=0)
	{
		row=(int)(n/col);
		row++;
	}
	else
		row=n/col;

        char x[][]=new char [row][col];    
  
        for(int i=0,k=0;i<row;i++)
            	for(int j=0;j<col;j++)
                	if(k<n)
                	{
                   		x[i][j]=ct1.charAt(k);
                   		k++;
                	}
			else
				break;	

	for(int i=1;i<col;i+=2)
            	for(int j=0;j<row;j++)
			if(x[j][i]!='\u0000')
            			ct2+=x[j][i];
     
	for(int i=0;i<col;i+=2)
            	for(int j=0;j<row;j++)
			if(x[j][i]!='\u0000')
            			ct2+=x[j][i];

	System.out.println("\nAfter Custom Columnar Transposition:\n"+ct2);

	String dt1="";
        char c2[][]=new char [row][col]; 
   
	for(int i=0,k=0;i<row;i++)
            	for(int j=0;j<col;j++)
                	if(k<n)
                	{
                   		c2[i][j]='1';
                   		k++;
                	}
			else
				break;	

	
	int n2=0,r=0,c=1;
	while(n2<n)
	{
		if(c<col)
		{
			if(r<row)
			{
				if(c2[r][c]=='1')
					c2[r][c]=ct2.charAt(n2);
				else
					n2--;
				r++;
				n2++;
			}		
			else
			{
				r=0;
				c+=2;
			}
		}
		else
		{
			c=0;
		}
	}
	
        for(int i=0,k=0;i<row;i++)
            	for(int j=0;j<col;j++)
                	if(k<n)
                	{
                   		dt1+=c2[i][j];
                   		k++;
                	}
			else
				break;	
		
	System.out.println("\nDecrypting Custom Columnar Transposition:\n"+dt1);

	String dt2="";
        char a2[]=dt1.toCharArray();

        for(int i=0;i<n;i++)
        {
            	a2[i]=(char)(((((a2[i]-97)-2)/3)%26)+97);
            	dt2+=a2[i];
        }       
        System.out.println("\nDecrypting Custom Subsitution Cipher:\n"+dt2);        
    }
}
