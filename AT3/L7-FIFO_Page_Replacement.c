/*********************************************************
   ----- 48450 -- week 7 ------ 
This is a program to practice FIFO Page Replacement algorithm

This code is based on https://www.javatpoint.com/fifo-page-replacement-algorithm-in-c

*/

#include<stdio.h> 
 
int main()  
{    
    int reference_string[] = {7, 0, 1, 2, 0, 3, 0, 4, 2, 3, 0, 3, 0, 3, 2, 1, 2, 0, 1, 7, 0, 1, 7, 5};// 22 pages
    //int reference_string[]= {7,0,1,2,0,3,0,4,2,3,0,3,0,3,2,1,2,0,1,1,0,7}; // 22 pages
    int pageFaults = 0;  
    int frames = 3;  
    int m, n, s, pages; 
    int a,b;  
    a =sizeof(reference_string);
    b =sizeof(reference_string[0]);
    pages = a/b;
    printf("pages=%d\n",pages);
    
    printf("Incoming \t Frame 1 \t Frame 2 \t Frame 3 ");  
    int temp[frames];  
    for(m = 0; m < frames; m++)  //for loop: set up to temp[0]=temp[1]=temp[2]=-1
    {  
        temp[m] = -1;  
    }  

    for(m = 0; m < pages; m++) // for-loop: pages string from 0 to 21   
    {  
        s = 0;   
        for(n = 0; n < frames; n++)  // for-loop for frames (0-2) given the current page
        {  
            if(reference_string[m] == temp[n]) //if the page = the current fame content/value 
            {  
                s++;  		//s = 1, indciate "don't change this frame content/value" 
                pageFaults--;  // -1 of pageFaults to balance the pageFaults++ 
            }  
        }  
        pageFaults++;  // increase 1 regardless "reference_string[m] == temp[n]"

        if((pageFaults <= frames) && (s == 0)) // string is counted within the 1st 3 frames  
        {  
            temp[m] = reference_string[m]; //just load the currnet page number into temp[m] 
        }  
        else if(s == 0)  // neither the page = farme content/value nor the 1st 3 frames!
        {  
	/* Note: the explanation of "temp[(pageFaults - 1) % frames] = reference_string[m];"
    replace the current content/value of array temp[] with reference_string[m]
	Ex: 3%3 = 0,temp[0]; 4%3=1,temp[1]; 5%3=2,temp[2]; 6%3=0,temp[0]; 7%3=1,temp[1]; 
	 8%3=2,temp[2]; 9%3=0,temp[0] with the pageFaults 3,4,5,6,7,8,9 . . . . . .  
	****************************/
    /* Question 4: Can you describe the function of the following code line? */
    /* Question 5: If the string sequence is changed to 
    int reference_string[] = {7,0,1,2,0,3,0,4,2,3,0,3,0,3,2,1,2,0,1,1,0,7}; 
    22 pages, what is the result?*/
        temp[(pageFaults - 1) % frames] = reference_string[m]; 
        }  

	/********* print out the results***********/
        printf("\n");  
        printf("%d\t\t",reference_string[m]);  //print the current page
        for(n = 0; n < frames; n++)  //for-loop to print out the contents of frames 1 to 3
        {  
            if(temp[n] != -1)  
                printf(" %d\t\t", temp[n]);  
            else  
                printf(" - \t\t");  
        }  
    }  // end of (m = 0; m < pages; m++)

    printf("\n\nTotal Page Faults:\t%d\n", pageFaults);  
    return 0;  
}  
