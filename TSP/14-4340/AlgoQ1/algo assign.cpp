#include<iostream>
#include<iomanip>
#include<fstream>
#include<algorithm>
#include<math.h>
#include<time.h>
#include<cmath>
#include<string>
#include<windows.h>
using namespace std;

int random(int min, int max) //range : [min, max)
{
	static bool first = true;
	if ( first ) 
	{  
		srand(time(NULL)); //seeding for the first time only!
		first = false;
	}
	return min + rand() % (max - min);
}

void insertion_sort(int  arr[] ,int s)
{
	int temp;
	int j;
	for(int i=1;i<s;i++)
	{
		temp=arr[i];
		j=i-1;
		while((temp<arr[j]) && (j>=0))
		{
			arr[j+1]=arr[j];
			j--;
		}
		arr[j+1]=temp;
	}
}
void merge_sort(int  arr[] , int low,int mid ,int high)
{

	int i, mid_after, lo;
	int * temp = new int[1000000];



	lo = low;

	i = low;

	mid_after = mid + 1;

	while ((lo <= mid) && (mid_after <= high))

	{

		if (arr[lo] <= arr[mid_after])

		{

			temp[i] = arr[lo];

			lo++;

		}

		else

		{

			temp[i] = arr[mid_after];

			mid_after++;

		}

		i++;

	}

	if (lo > mid)

	{

		for (int k = mid_after; k <= high; k++)

		{

			temp[i] = arr[k];

			i++;

		}

	}

	else

	{

		for (int k = lo; k <= mid; k++)

		{

			temp[i] = arr[k];

			i++;

		}

	}



	for (int k = low; k <= high; k++)

	{

		arr[k] = temp[k];

	}

}
void partitioning_of_merge_sort(int arr[],int low,int high)

{

	int mid=0;



	if(low < high)

	{

		mid = (low + high) / 2;

		partitioning_of_merge_sort(arr, low, mid);

		partitioning_of_merge_sort(arr, mid + 1, high);

		merge_sort(arr, low, mid, high);

	}

}
int partitioning_of_quick_sort(int arr[],int low,int high){

	int pivot = arr[high];
	int  i = (low-1);
	for(int  j = low ; j <= high - 1;j++)
	{
		if(arr[j] <= pivot)
		{
			i++;
			swap(arr[i],arr[j]);
		}
	}
	swap(arr[i+1],arr[high]);
	return (i+1) ;    //returning the position of the pivot 
}
void quick_sort(int arr[],int low,int high)
{
	
	if(low < high)
	{
		int pivot = partitioning_of_quick_sort(arr,low,high);
		quick_sort(arr,low,pivot-1);
		quick_sort(arr,pivot+1,high);

	}
}
int partitioning_of_quick_sort_1(int arr[],int low,int high){

	int pivot = arr[random(low,high)];
	int pivot1 = arr[random(low,high)];
	int pivot2 = arr[random(low,high)];
	int median = max(min(pivot,pivot1), min(max(pivot,pivot1),pivot2));

	int  i = low-1;
	for(int  j = low ; j <= high - 1;j++)
	{
		if(arr[j] <= median)
		{
			i++;
			swap(arr[i],arr[j]);
		}
	}
	swap(arr[i+1],arr[high]);
	return i+1 ;    //returning the position of the pivot 
}
void quick_sort_using_median_of_3(int arr[],int low,int high)
{
	int pivot;
	if(low < high)
	{
		pivot = partitioning_of_quick_sort_1(arr,low,high);
		quick_sort(arr,low,pivot-1);
		quick_sort(arr,pivot+1,high);

	}

}
int get_parent(int element)
{
	return (floor(element/2));
}
int get_left_child(int element)
{
	return (2*element);
}
int get_right_child(int element)
{
	return ((2*element)+1);
}
/*
void max_heapify(int *arr,int index_violating_heap_property,int size)
{
	int largest;
	int left = get_left_child(index_violating_heap_property);
	int right = get_right_child(index_violating_heap_property);
	
	
	cout<<he.arr[left-1];
	cout<<he.arr[index_violating_heap_property-1];
	if(right<=he.size_of_heap)
	{
		cout<<he.arr[right];
	}


	if((left<=he.size_of_heap) && (he.arr[left-1]>he.arr[index_violating_heap_property-1]))
	{
		largest = left-1;
	}
	else 
	{
		largest = index_violating_heap_property;
	}
	if((right<=he.size_of_heap) && (he.arr[right-1]>he.arr[largest]))
	{
		largest = right-1;
	}

	if(largest!=index_violating_heap_property)
	{
		swap(he.arr[index_violating_heap_property-1],he.arr[largest]);
		//max_heapify(he,largest);
	}
}

*/
void max_heapify(int *arr,int size,int index_violating_heap_property)
{
	
	int left_child  = get_left_child(index_violating_heap_property);
	int right_child = get_right_child(index_violating_heap_property);
	int temp;
	temp = arr[index_violating_heap_property];
	while(left_child<=size)
	{
		if(left_child<size && arr[left_child + 1] > arr[left_child])
			left_child = left_child + 1;
		if(temp>arr[left_child])
			break;
		else if(temp <= arr[left_child])
		{
			arr[left_child/2] = arr[left_child];
			left_child = left_child*2;
		}

	}

	arr[left_child/2] = temp;
	return;
	
}
void build_max_heap(int * arr,int size)
{
	for(int i = floor((size)/2);i>=1;i--)
	{
		max_heapify(arr,size,i);
	}

}
void heapsort(int *arr,int size)
{
    for (int i = size; i >= 2; i--)
    {
		swap(arr[1],arr[i]);
        max_heapify(arr, i-1, 1);
    }

}
void iterativequicksort(int arr[],int l,int h)
{

	int *stack = new int[h-l+1]();
	 // Create an auxiliary stack
//    int stack[ h - l + 1 ];
 
    // initialize top of stack
    int top = -1;
 
    // push initial values of l and h to stack
    stack[ ++top ] = l;
    stack[ ++top ] = h;
 
    // Keep popping from stack while is not empty
    while ( top >= 0 )
    {
        // Pop h and l
        h = stack[ top-- ];
        l = stack[ top-- ];
 
        // Set pivot element at its correct position
        // in sorted array
        int p = partitioning_of_quick_sort( arr, l, h );
 
        // If there are elements on left side of pivot,
        // then push left side to stack
        if ( p-1 > l )
        {
            stack[ ++top ] = l;
            stack[ ++top ] = p - 1;
        }
 
        // If there are elements on right side of pivot,
        // then push right side to stack
        if ( p+1 < h )
        {
            stack[ ++top ] = p + 1;
            stack[ ++top ] = h;
        }
    }
}
void iterativequicksortmedianof3(int arr[],int l,int h)
{
	int *stack = new int[h-l+1];
	 // Create an auxiliary stack
  //  int stack[ h - l + 1 ];
 
    // initialize top of stack
    int top = -1;
 
    // push initial values of l and h to stack
    stack[ ++top ] = l;
    stack[ ++top ] = h;
 
    // Keep popping from stack while is not empty
    while ( top >= 0 )
    {
        // Pop h and l
        h = stack[ top-- ];
        l = stack[ top-- ];
 
        // Set pivot element at its correct position
        // in sorted array
        int p = partitioning_of_quick_sort_1( arr, l, h );
 
        // If there are elements on left side of pivot,
        // then push left side to stack
        if ( p-1 > l )
        {
            stack[ ++top ] = l;
            stack[ ++top ] = p - 1;
        }
 
        // If there are elements on right side of pivot,
        // then push right side to stack
        if ( p+1 < h )
        {
            stack[ ++top ] = p + 1;
            stack[ ++top ] = h;
        }
    }
}
int main()
{
	int * array_containing_numbers;
	int array_size_choice ;
	array_size_choice = 0;
	cout<<"Enter the size of array"<<endl;
	cout<<"Choose From the following sizes!"<<endl;
	cout<<"1:10"<<endl;
	cout<<"2:100"<<endl;
	cout<<"3:1000"<<endl;
	cout<<"4:10000"<<endl;
	cout<<"5:100000"<<endl;
	cout<<"6:1000000"<<endl;
	cin>>array_size_choice;

	if(array_size_choice == 10 )
	{
		array_containing_numbers = new int [10](); 
		int choice = 0;
		cout<<"Enter Choice for array type!"<<endl;
		cout<<"1:Completely Random!"<<endl;
		cout<<"2:Sorted!"<<endl;
		cout<<"3:Almost Sorted (90% in increasing order and the rest 10% random)!"<<endl;
		cout<<"4:Reversed Sorted(Sorted Numbers are in reverse order)!"<<endl;
		cin>>choice;


		if(choice == 1)
		{
			for(int i = 0;i<10 ; i++)
			{
				array_containing_numbers[i] = i;
			}
			for(int i = 0 ;i < 10 ;i ++)
			{
				swap(array_containing_numbers[i],array_containing_numbers[random(0,10)]);
			}
			for(int i = 0 ;i < 10 ;i ++)
			{
				swap(array_containing_numbers[i],array_containing_numbers[random(0,10)]);
			}


			int sorting_choice = 0;
			cout<<"Enter Sorting Choice"<<endl;
			cout<<"1:Insertion Sort!"<<endl;
			cout<<"2:Merge Sort!"<<endl;
			cout<<"3:QuickSort!"<<endl;
			cout<<"4:QuickSort2!"<<endl;
			cout<<"5:Heapsort!"<<endl;
			cin>>sorting_choice;
			if(sorting_choice == 1)
			{
				clock_t tStart = clock();
				insertion_sort(array_containing_numbers,10);
				/*
				cout<<"After Applying Insertion Sort on array of size 10 and containing random numbers we have"<<endl;
				for(int i = 0 ; i <10 ; i++)
				{
					cout<<array_containing_numbers[i]<<"|";
				}
				*/
				cout<<endl;
				printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
				cout<<endl;
				
			}
			else if(sorting_choice == 2)
			{
				clock_t tStart = clock();
				partitioning_of_merge_sort(array_containing_numbers,0,10);
				/*cout<<"After Applying Merge Sort on array of size 10 and containing random numbers we have"<<endl;
				for(int i = 1 ; i <11 ; i++)
				{
					cout<<array_containing_numbers[i]<<"|";
				}
				cout<<endl;
				*/
				printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
				cout<<endl;
				

			}
			else if(sorting_choice == 3)
			{
				clock_t tStart = clock();
				//quick_sort(array_containing_numbers,0,9);
				iterativequicksort(array_containing_numbers,0,9);
			
				cout<<endl;
				printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
				cout<<endl;
	

			}
			else if(sorting_choice == 4)
			{
				clock_t tStart = clock();
				//quick_sort_using_median_of_3(array_containing_numbers,0,9);
				iterativequicksortmedianof3(array_containing_numbers,0,9);
			
				/*

				cout<<"After Applying Quick Sort 2 on array of size 10 and containing random numbers we have"<<endl;
				for(int i = 0 ; i <10 ; i++)
				{
					cout<<array_containing_numbers[i]<<"|";
				}
				*/
				cout<<endl;
				printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
				cout<<endl;
			}
			else if(sorting_choice == 5)
			{
				array_containing_numbers = new int [12];
				for(int i = 1;i < 11 ; i++)
				{
					array_containing_numbers[i] = i;
				}
				for(int i = 1 ;i < 11 ;i ++)
				{
					swap(array_containing_numbers[i],array_containing_numbers[random(1,10)]);
				}
				for(int i = 1 ;i < 11 ;i ++)
				{
					swap(array_containing_numbers[i],array_containing_numbers[random(1,10)]);
				}

				/*
				cout<<"Before Applying Heap Sort on the array!"<<endl;
				for(int  i = 1 ; i < 11 ; i ++)
				{
					cout<<array_containing_numbers[i]<<"|";
				}
				*/
				cout<<endl;
				clock_t tStart = clock();
				build_max_heap(array_containing_numbers,11);
				heapsort(array_containing_numbers,11);
				/*
				cout<<"After Applying Heap Sort on array of size 10 and containing random numbers we have"<<endl;
				for(int i = 2 ; i <= 11 ; i++)
				{
					cout<<array_containing_numbers[i]<<"|";
				}
				*/
				cout<<endl;
				printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
				cout<<endl;


			}
		}
		else if(choice == 2)
		{
			for(int i = 0;i<10 ; i++)
			{
				array_containing_numbers[i] = i;
			}

			int sorting_choice = 0;
			cout<<"Enter Sorting Choice"<<endl;
			cout<<"1:Insertion Sort!"<<endl;
			cout<<"2:Merge Sort!"<<endl;
			cout<<"3:QuickSort!"<<endl;
			cout<<"4:QuickSort2!"<<endl;
			cout<<"5:Heapsort!"<<endl;
			cin>>sorting_choice;
			if(sorting_choice == 1)
			{
				clock_t tStart = clock();
				insertion_sort(array_containing_numbers,10);

				/*
				cout<<"After Applying Insertion Sort on array of size 10 and containing numbers in sorted order"<<endl;
				for(int i = 0 ; i <10 ; i++)
				{
					cout<<array_containing_numbers[i]<<"|";
				}
				*/
				cout<<endl;
				printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
				cout<<endl;


			}
			else if(sorting_choice == 2)
			{
				clock_t tStart = clock();
				partitioning_of_merge_sort(array_containing_numbers,0,10);
				/*
				cout<<"After Applying Merge Sort on array of size 10 and containing numbers in sorted order"<<endl;
				for(int i = 0 ; i <10 ; i++)
				{
					cout<<array_containing_numbers[i]<<"|";
				}
				*/
							cout<<endl;
				printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
				cout<<endl;


			}
			else if(sorting_choice == 3)
			{
				clock_t tStart = clock();

//				quick_sort(array_containing_numbers,0,9);
				iterativequicksort(array_containing_numbers,0,9);

				/*
				cout<<"After Applying Quick Sort on array of size 10 and containing numbers in sorted order"<<endl;
				for(int i = 0 ; i <10 ; i++)
				{
					cout<<array_containing_numbers[i]<<"|";
				}
				*/
			cout<<endl;
				printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
				cout<<endl;

			}
			else if(sorting_choice == 4)
			{
				clock_t tStart = clock();
				//quick_sort_using_median_of_3(array_containing_numbers,0,9);
				iterativequicksortmedianof3(array_containing_numbers,0,9);
				
				/*
				cout<<"After Applying Quick Sort 2 on array of size 10 and containing numbers in sorted order"<<endl;
				for(int i = 0 ; i <10 ; i++)
				{
					cout<<array_containing_numbers[i]<<"|";
				}
				*/

			cout<<endl;
				printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
				cout<<endl;


			}
			else if(sorting_choice == 5)
			{
				array_containing_numbers = new int [12];
				for(int i = 1;i < 11 ; i++)
				{
					array_containing_numbers[i] = i;
				}

				/*
				cout<<"Before Applying Heap Sort on the array!"<<endl;
				for(int  i = 1 ; i < 11 ; i ++)
				{
					cout<<array_containing_numbers[i]<<"|";
				}
*/
				cout<<endl;
				clock_t tStart = clock();
				build_max_heap(array_containing_numbers,11);
				heapsort(array_containing_numbers,11);
				/*
				cout<<"After Applying Heap Sort on array of size 10 and containing numbers in increasing order we have"<<endl;
				for(int i = 2 ; i <=11 ; i++)
				{
					cout<<array_containing_numbers[i]<<"|";
				}
				*/
				cout<<endl;
				printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
				cout<<endl;



			}
		}
		else if(choice == 3)
		{
			for(int i = 0 ;i<9 ;i++)
			{
				array_containing_numbers[i] = i;
			}
			array_containing_numbers[9]  = rand()%100;


			int sorting_choice = 0;
			cout<<"Enter Sorting Choice"<<endl;
			cout<<"1:Insertion Sort!"<<endl;
			cout<<"2:Merge Sort!"<<endl;
			cout<<"3:QuickSort!"<<endl;
			cout<<"4:QuickSort2!"<<endl;
			cout<<"5:Heapsort!"<<endl;
			cin>>sorting_choice;
			if(sorting_choice == 1)
			{

				clock_t tStart = clock();
				insertion_sort(array_containing_numbers,10);
				/*cout<<"After Applying Insertion Sort on array of size 10 and containing 90 % numbers in increasing order "<<endl;
				for(int i = 0 ; i <10 ; i++)
				{
					cout<<array_containing_numbers[i]<<"|";
				}
				*/

				cout<<endl;
				printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
				cout<<endl;



			}
			else if(sorting_choice == 2)
			{
				clock_t tStart = clock(); 
				partitioning_of_merge_sort(array_containing_numbers,0,10);
				/*
				cout<<"After Applying Merge Sort on array of size 10 and containing 90% numbers in increasing order "<<endl;
				for(int i = 1 ; i <=10 ; i++)
				{
					cout<<array_containing_numbers[i]<<"|";
				}
				*/
				cout<<endl;
				printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
				cout<<endl;



			}
			else if(sorting_choice == 3)
			{
				clock_t tStart = clock();
				//quick_sort(array_containing_numbers,0,9);
				
				iterativequicksort(array_containing_numbers,0,9);
					/*cout<<"After Applying Quick Sort on array of size 10 and containing 90% numbers in increasing order "<<endl;
				for(int i = 0 ; i <10 ; i++)
				{
					cout<<array_containing_numbers[i]<<"|";
				}
				*/
				cout<<endl;
				printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
				cout<<endl;


			}
			else if(sorting_choice == 4)
			{
				clock_t tStart = clock();
				//quick_sort_using_median_of_3(array_containing_numbers,0,9);
				iterativequicksortmedianof3(array_containing_numbers,0,9);
				/*
				cout<<"After Applying Quick Sort 2 on array of size 10 and containing 90% numbers in increasing order"<<endl;
				for(int i = 0 ; i <10 ; i++)
				{
					cout<<array_containing_numbers[i]<<"|";
				}
				*/

				cout<<endl;
				printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
				cout<<endl;



			}
			else if(sorting_choice == 5)
			{

				array_containing_numbers = new int [12];
				for(int i = 1 ;i<11 ;i++)
				{
					array_containing_numbers[i] = i;
				}
				array_containing_numbers[10]  = rand()%100;

				/*
				cout<<"Before Applying Heap Sort on the array!"<<endl;
				for(int  i = 1 ; i < 11 ; i ++)
				{
					cout<<array_containing_numbers[i]<<"|";
				}
				*/
				cout<<endl;
				clock_t tStart = clock();
				build_max_heap(array_containing_numbers,11);
				heapsort(array_containing_numbers,11);
				/*
				cout<<"After Applying Heap Sort on array of size 10 and containing 90% numbers in increasing order we have"<<endl;
				for(int i = 2 ; i <=11 ; i++)
				{
					cout<<array_containing_numbers[i]<<"|";
				}
				*/
				cout<<endl;
				printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
				cout<<endl;


		
			}
		}
		else if(choice == 4)
		{
			int tmp;
			tmp = 1;
			for(int i = 10 ;i > 0  ;i--)
			{
				array_containing_numbers[tmp++] = i;
			}

			int sorting_choice = 0;
			cout<<"Enter Sorting Choice"<<endl;
			cout<<"1:Insertion Sort!"<<endl;
			cout<<"2:Merge Sort!"<<endl;
			cout<<"3:QuickSort!"<<endl;
			cout<<"4:QuickSort2!"<<endl;
			cout<<"5:Heapsort!"<<endl;
			cin>>sorting_choice;
			if(sorting_choice == 1)
			{
				clock_t tStart = clock();
				insertion_sort(array_containing_numbers,10);
				
				cout<<endl;
				printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
				cout<<endl;



			}
			else if(sorting_choice == 2)
			{
				clock_t tStart = clock();
				partitioning_of_merge_sort(array_containing_numbers,0,10);
				
				cout<<endl;
				printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
				cout<<endl;



			}
			else if(sorting_choice == 3)
			{
				clock_t tStart = clock();
				//quick_sort(array_containing_numbers,0,9);
				iterativequicksort(array_containing_numbers,0,9);
				cout<<endl;
				printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
				cout<<endl;


			}
			else if(sorting_choice == 4)
			{
				clock_t tStart = clock();
				//quick_sort_using_median_of_3(array_containing_numbers,0,9);
				iterativequicksortmedianof3(array_containing_numbers,0,9);
				
				cout<<endl;
				printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
				cout<<endl;



			}
			else if(sorting_choice == 5)
			{
				array_containing_numbers = new int [12];

				tmp = 1;
				for(int i = 10 ;i >=1  ;i--)
				{
					array_containing_numbers[tmp++] = i;
				}

				

				cout<<endl;
				clock_t tStart = clock();
				build_max_heap(array_containing_numbers,11);
				heapsort(array_containing_numbers,11);
				
				cout<<endl;
				printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
				cout<<endl;


			}
		}
	}

	else if(array_size_choice == 100)
	{
		array_containing_numbers = new int [100](); 
		int choice = 0;
		cout<<"Enter Choice for array type!"<<endl;
		cout<<"1:Completely Random!"<<endl;
		cout<<"2:Sorted!"<<endl;
		cout<<"3:Almost Sorted (90% in increasing order and the rest 10% random)!"<<endl;
		cout<<"4:Reversed Sorted(Sorted Numbers are in reverse order)!"<<endl;
		cin>>choice;


		if(choice == 1)
		{
			for(int i = 0;i<100 ; i++)
			{
				array_containing_numbers[i] = i;
			}
			for(int i = 0 ;i < 100 ;i ++)
			{
				swap(array_containing_numbers[i],array_containing_numbers[random(0,100)]);
			}
			for(int i = 0 ;i < 100 ;i ++)
			{
				swap(array_containing_numbers[i],array_containing_numbers[random(0,100)]);
			}


			int sorting_choice = 0;
			cout<<"Enter Sorting Choice"<<endl;
			cout<<"1:Insertion Sort!"<<endl;
			cout<<"2:Merge Sort!"<<endl;
			cout<<"3:QuickSort!"<<endl;
			cout<<"4:QuickSort2!"<<endl;
			cout<<"5:Heapsort!"<<endl;
			cin>>sorting_choice;
			if(sorting_choice == 1)
			{
				clock_t tStart = clock();
				insertion_sort(array_containing_numbers,100);
				printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
				cout<<endl;


			}
			else if(sorting_choice == 2)
			{
				clock_t tStart = clock();
				partitioning_of_merge_sort(array_containing_numbers,0,100);
				printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
				cout<<endl;



			}
			else if(sorting_choice == 3)
			{
				clock_t tStart = clock();
				//quick_sort(array_containing_numbers,0,99);
				iterativequicksort(array_containing_numbers,0,99);
				
				printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
				cout<<endl;


			}
			else if(sorting_choice == 4)
			{
				clock_t tStart = clock();
				//quick_sort_using_median_of_3(array_containing_numbers,0,9);
				iterativequicksortmedianof3(array_containing_numbers,0,99);
				
				printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
				cout<<endl;



			}
			else if(sorting_choice == 5)
			{

				array_containing_numbers = new int [102];
				for(int i = 1;i < 101 ; i++)
				{
					array_containing_numbers[i] = i;
				}
				for(int i = 1 ;i < 101 ;i ++)
				{
					swap(array_containing_numbers[i],array_containing_numbers[random(1,100)]);
				}
				for(int i = 1 ;i < 101 ;i ++)
				{
					swap(array_containing_numbers[i],array_containing_numbers[random(1,100)]);
				}


				cout<<endl;
				clock_t tStart = clock();
				build_max_heap(array_containing_numbers,101);
				heapsort(array_containing_numbers,101);
				
				cout<<endl;
				printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
				cout<<endl;


	
			}
		}
		else if(choice == 2)
		{
			for(int i = 0;i<100 ; i++)
			{
				array_containing_numbers[i] = i;
			}

			int sorting_choice = 0;
			cout<<"Enter Sorting Choice"<<endl;
			cout<<"1:Insertion Sort!"<<endl;
			cout<<"2:Merge Sort!"<<endl;
			cout<<"3:QuickSort!"<<endl;
			cout<<"4:QuickSort2!"<<endl;
			cout<<"5:Heapsort!"<<endl;
			cin>>sorting_choice;
			if(sorting_choice == 1)
			{
				clock_t tStart = clock();
				insertion_sort(array_containing_numbers,100);
				cout<<endl;
				printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
				cout<<endl;



			}
			else if(sorting_choice == 2)
			{
				clock_t tStart = clock();
				partitioning_of_merge_sort(array_containing_numbers,0,100);
				
				cout<<endl;
				printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
				cout<<endl;



			}
			else if(sorting_choice == 3)
			{
				clock_t tStart = clock();
		//	quick_sort(array_containing_numbers,0,99);
				iterativequicksort(array_containing_numbers,0,99);
				cout<<endl;
				printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
				cout<<endl;


			}
			else if(sorting_choice == 4)
			{
				clock_t tStart = clock();
		//		quick_sort_using_median_of_3(array_containing_numbers,0,99);
				iterativequicksortmedianof3(array_containing_numbers,0,99);	
				cout<<endl;
				printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
				cout<<endl;

			}
			else if(sorting_choice == 5)
			{
				array_containing_numbers = new int [102];
				for(int i = 1;i < 101 ; i++)
				{
					array_containing_numbers[i] = i;
				}


				cout<<endl;
				clock_t tStart = clock();
				build_max_heap(array_containing_numbers,101);
				heapsort(array_containing_numbers,101);
				
				cout<<endl;
				printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
				cout<<endl;



			}
		}
		else if(choice == 3)
		{
			for(int i = 0 ;i<90 ;i++)
			{
				array_containing_numbers[i] = i;
			}
			for(int i = 90 ;i<100 ;i ++)
			{
				array_containing_numbers[i]  = rand()%100;
			}

			int sorting_choice = 0;
			cout<<"Enter Sorting Choice"<<endl;
			cout<<"1:Insertion Sort!"<<endl;
			cout<<"2:Merge Sort!"<<endl;
			cout<<"3:QuickSort!"<<endl;
			cout<<"4:QuickSort2!"<<endl;
			cout<<"5:Heapsort!"<<endl;
			cin>>sorting_choice;
			if(sorting_choice == 1)
			{
				clock_t tStart = clock();
				insertion_sort(array_containing_numbers,100);
					cout<<endl;
				printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
				cout<<endl;


			}
			else if(sorting_choice == 2)
			{
				clock_t tStart = clock();
				partitioning_of_merge_sort(array_containing_numbers,0,100);
					cout<<endl;
				printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
				cout<<endl;



			}
			else if(sorting_choice == 3)
			{
				clock_t tStart = clock();
				//quick_sort(array_containing_numbers,0,99);
				iterativequicksort(array_containing_numbers,0,99);
					cout<<endl;
				printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
				cout<<endl;


			}
			else if(sorting_choice == 4)
			{
				clock_t tStart =clock();
				//quick_sort_using_median_of_3(array_containing_numbers,0,99);
				iterativequicksortmedianof3(array_containing_numbers,0,99);
					cout<<endl;
				printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
				cout<<endl;

			}
			else if(sorting_choice == 5)
			{
				array_containing_numbers = new int [102];
				
				for(int i = 1 ;i<91 ;i++)
				{
					array_containing_numbers[i] = i;
				}
				for(int i = 91 ;i<101 ;i ++)
				{
					array_containing_numbers[i]  = rand()%100;
				}

		

				cout<<endl;
				clock_t tStart = clock();
				build_max_heap(array_containing_numbers,101);
				heapsort(array_containing_numbers,101);
				
				cout<<endl;
				printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
				cout<<endl;


			
			}
		}
		else if(choice == 4)
		{
			int tmp;
			tmp = 0;
			for(int i = 100 ;i > 0  ;i--)
			{
				array_containing_numbers[tmp++] = i;
			}

			int sorting_choice = 0;
			cout<<"Enter Sorting Choice"<<endl;
			cout<<"1:Insertion Sort!"<<endl;
			cout<<"2:Merge Sort!"<<endl;
			cout<<"3:QuickSort!"<<endl;
			cout<<"4:QuickSort2!"<<endl;
			cout<<"5:Heapsort!"<<endl;
			cin>>sorting_choice;
			if(sorting_choice == 1)
			{
				clock_t tStart =clock();
				insertion_sort(array_containing_numbers,100);
				cout<<endl;
				printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
				cout<<endl;



			}
			else if(sorting_choice == 2)
			{
				clock_t tStart = clock();
				partitioning_of_merge_sort(array_containing_numbers,0,100);
				cout<<endl;
				printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
				cout<<endl;


			}
			else if(sorting_choice == 3)
			{
				clock_t tStart = clock();
				//quick_sort(array_containing_numbers,0,99);
				iterativequicksort(array_containing_numbers,0,99);
				cout<<endl;
				printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
				cout<<endl;


			}
			else if(sorting_choice == 4)
			{
				clock_t tStart = clock();
				//quick_sort_using_median_of_3(array_containing_numbers,0,99);
				iterativequicksortmedianof3(array_containing_numbers,0,99);
				cout<<endl;
				printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
				cout<<endl;


			}
			else if(sorting_choice == 5)
			{

				array_containing_numbers = new int [102];
				tmp = 1;
				for(int i = 100 ;i >=1  ;i--)
				{
					array_containing_numbers[tmp++] = i;
				}

				

				cout<<endl;
				clock_t tStart = clock();
				build_max_heap(array_containing_numbers,101);
				heapsort(array_containing_numbers,101);
				
				cout<<endl;
				printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
				cout<<endl;
			
			}
		}

	}
	else if(array_size_choice ==1000)
	{
		array_containing_numbers = new int [1000](); 
		int choice = 0;
		cout<<"Enter Choice for array type!"<<endl;
		cout<<"1:Completely Random!"<<endl;
		cout<<"2:Sorted!"<<endl;
		cout<<"3:Almost Sorted (90% in increasing order and the rest 10% random)!"<<endl;
		cout<<"4:Reversed Sorted(Sorted Numbers are in reverse order)!"<<endl;
		cin>>choice;


		if(choice == 1)
		{
			for(int i = 0;i<1000 ; i++)
			{
				array_containing_numbers[i] = i;
			}
			for(int i = 0 ;i < 1000 ;i ++)
			{
				swap(array_containing_numbers[i],array_containing_numbers[random(0,1000)]);
			}
			for(int i = 0 ;i < 1000 ;i ++)
			{
				swap(array_containing_numbers[i],array_containing_numbers[random(0,1000)]);
			}


			int sorting_choice = 0;
			cout<<"Enter Sorting Choice"<<endl;
			cout<<"1:Insertion Sort!"<<endl;
			cout<<"2:Merge Sort!"<<endl;
			cout<<"3:QuickSort!"<<endl;
			cout<<"4:QuickSort2!"<<endl;
			cout<<"5:Heapsort!"<<endl;
			cin>>sorting_choice;
			if(sorting_choice == 1)
			{
				clock_t tStart = clock();
				insertion_sort(array_containing_numbers,1000);
			
						cout<<endl;
				printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
				cout<<endl;
		


			}
			else if(sorting_choice == 2)
			{
				clock_t tStart = clock();
				partitioning_of_merge_sort(array_containing_numbers,0,1000);
						cout<<endl;
				printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
				cout<<endl;
		

			}
			else if(sorting_choice == 3)
			{
				clock_t tStart = clock();
//				quick_sort(array_containing_numbers,0,999);
				iterativequicksort(array_containing_numbers,0,999);
						cout<<endl;
				printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
				cout<<endl;
		

			}
			else if(sorting_choice == 4)
			{
				clock_t tStart = clock();
				//quick_sort_using_median_of_3(array_containing_numbers,0,999);
				iterativequicksortmedianof3(array_containing_numbers,0,999);	
				
				cout<<endl;
				printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
				cout<<endl;
		

			}
			else if(sorting_choice == 5)
			{
				
				array_containing_numbers = new int [1002];
				for(int i = 1;i < 1001 ; i++)
				{
					array_containing_numbers[i] = i;
				}
				for(int i = 1 ;i < 1001 ;i ++)
				{
					swap(array_containing_numbers[i],array_containing_numbers[random(1,1000)]);
				}
				for(int i = 1 ;i < 1001 ;i ++)
				{
					swap(array_containing_numbers[i],array_containing_numbers[random(1,1000)]);
				}

cout<<endl;
				clock_t tStart = clock();
				build_max_heap(array_containing_numbers,1001);
				heapsort(array_containing_numbers,1001);
				
				cout<<endl;
				printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
				cout<<endl;

		
			}
		}
		else if(choice == 2)
		{
			for(int i = 0;i<1000 ; i++)
			{
				array_containing_numbers[i] = i;
			}

			int sorting_choice = 0;
			cout<<"Enter Sorting Choice"<<endl;
			cout<<"1:Insertion Sort!"<<endl;
			cout<<"2:Merge Sort!"<<endl;
			cout<<"3:QuickSort!"<<endl;
			cout<<"4:QuickSort2!"<<endl;
			cout<<"5:Heapsort!"<<endl;
			cin>>sorting_choice;
			if(sorting_choice == 1)
			{
				clock_t tStart = clock();
				insertion_sort(array_containing_numbers,1000);
						cout<<endl;
				printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
				cout<<endl;



			}
			else if(sorting_choice == 2)
			{
				clock_t tStart = clock();
				partitioning_of_merge_sort(array_containing_numbers,0,1000);

						cout<<endl;
				printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
				cout<<endl;

			}
			else if(sorting_choice == 3)
			{
				clock_t tStart = clock();
			//	quick_sort(array_containing_numbers,0,999);
				iterativequicksort(array_containing_numbers,0,999);
						cout<<endl;
				printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
				cout<<endl;


			}
			else if(sorting_choice == 4)
			{
				clock_t tStart = clock();
				//quick_sort_using_median_of_3(array_containing_numbers,0,999);
				iterativequicksortmedianof3(array_containing_numbers,0,999);	
				cout<<endl;
				printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
				cout<<endl;



			}
			else if(sorting_choice == 5)
			{

					array_containing_numbers = new int [1002];
				for(int i = 1;i < 1001 ; i++)
				{
					array_containing_numbers[i] = i;
				}

				cout<<endl;
				clock_t tStart = clock();
				build_max_heap(array_containing_numbers,1001);
				heapsort(array_containing_numbers,1001);
				
				cout<<endl;
				printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
				cout<<endl;



			}
		}
		else if(choice == 3)
		{
			for(int i = 0 ;i<900 ;i++)
			{
				array_containing_numbers[i] = i;
			}
			for(int i = 900 ;i<1000 ;i ++)
			{
				array_containing_numbers[i]  = rand()%100;
			}


			int sorting_choice = 0;
			cout<<"Enter Sorting Choice"<<endl;
			cout<<"1:Insertion Sort!"<<endl;
			cout<<"2:Merge Sort!"<<endl;
			cout<<"3:QuickSort!"<<endl;
			cout<<"4:QuickSort2!"<<endl;
			cout<<"5:Heapsort!"<<endl;
			cin>>sorting_choice;
			if(sorting_choice == 1)
			{
				clock_t tStart = clock();
				insertion_sort(array_containing_numbers,1000);
				cout<<endl;
				printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
				cout<<endl;


			}
			else if(sorting_choice == 2)
			{
				clock_t tStart = clock();
				partitioning_of_merge_sort(array_containing_numbers,0,1000);

				cout<<endl;
				printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
				cout<<endl;

			}
			else if(sorting_choice == 3)
			{
				clock_t tStart = clock();
				//quick_sort(array_containing_numbers,0,999);
				iterativequicksort(array_containing_numbers,0,999);
				cout<<endl;
				printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
				cout<<endl;

			}
			else if(sorting_choice == 4)
			{
				clock_t tStart = clock();
			//	quick_sort_using_median_of_3(array_containing_numbers,0,999);
				iterativequicksortmedianof3(array_containing_numbers,0,999);
				cout<<endl;
				printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
				cout<<endl;



			}
			else if(sorting_choice == 5)
			{
								array_containing_numbers = new int [1002];
				
				for(int i = 1 ;i<901 ;i++)
				{
					array_containing_numbers[i] = i;
				}
				for(int i = 901 ;i<1001 ;i ++)
				{
					array_containing_numbers[i]  = rand()%100;
				}

		

				cout<<endl;
				clock_t tStart = clock();
				build_max_heap(array_containing_numbers,1001);
				heapsort(array_containing_numbers,1001);
				
				cout<<endl;
				printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
				cout<<endl;


			
			}
		}
		else if(choice == 4)
		{
			int tmp;
			tmp = 0;
			for(int i = 1000 ;i > 0  ;i--)
			{
				array_containing_numbers[tmp++] = i;
			}

			int sorting_choice = 0;
			cout<<"Enter Sorting Choice"<<endl;
			cout<<"1:Insertion Sort!"<<endl;
			cout<<"2:Merge Sort!"<<endl;
			cout<<"3:QuickSort!"<<endl;
			cout<<"4:QuickSort2!"<<endl;
			cout<<"5:Heapsort!"<<endl;
			cin>>sorting_choice;
			if(sorting_choice == 1)
			{
				clock_t tStart =clock();
				insertion_sort(array_containing_numbers,1000);
				cout<<endl;
				printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
				cout<<endl;


			}
			else if(sorting_choice == 2)
			{
				clock_t tStart = clock();

				partitioning_of_merge_sort(array_containing_numbers,0,1000);
				cout<<endl;
				printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
				cout<<endl;



			}
			else if(sorting_choice == 3)
			{
				clock_t tStart = clock();
				//quick_sort(array_containing_numbers,0,999);
				iterativequicksort(array_containing_numbers,0,999);
				cout<<endl;
				printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
				cout<<endl;


			}
			else if(sorting_choice == 4)
			{
				clock_t tStart = clock();
				//quick_sort_using_median_of_3(array_containing_numbers,0,999);
				iterativequicksortmedianof3(array_containing_numbers,0,999);
				cout<<endl;
				printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
				cout<<endl;



			}
			else if(sorting_choice == 5)
			{
				
				array_containing_numbers = new int [1002];
				tmp = 1;
				for(int i = 1000 ;i >=1  ;i--)
				{
					array_containing_numbers[tmp++] = i;
				}

				

				cout<<endl;
				clock_t tStart = clock();
				build_max_heap(array_containing_numbers,1001);
				heapsort(array_containing_numbers,1001);
				
				cout<<endl;
				printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
				cout<<endl;
			
			}
		}

	}
	else if(array_size_choice == 10000)
	{
		array_containing_numbers = new int [10000](); 
		int choice = 0;
		cout<<"Enter Choice for array type!"<<endl;
		cout<<"1:Completely Random!"<<endl;
		cout<<"2:Sorted!"<<endl;
		cout<<"3:Almost Sorted (90% in increasing order and the rest 10% random)!"<<endl;
		cout<<"4:Reversed Sorted(Sorted Numbers are in reverse order)!"<<endl;
		cin>>choice;


		if(choice == 1)
		{
			for(int i = 0;i<10000 ; i++)
			{
				array_containing_numbers[i] = i;
			}
			for(int i = 0 ;i < 10000 ;i ++)
			{
				swap(array_containing_numbers[i],array_containing_numbers[random(0,10000)]);
			}
			for(int i = 0 ;i < 10000 ;i ++)
			{
				swap(array_containing_numbers[i],array_containing_numbers[random(0,10000)]);
			}


			int sorting_choice = 0;
			cout<<"Enter Sorting Choice"<<endl;
			cout<<"1:Insertion Sort!"<<endl;
			cout<<"2:Merge Sort!"<<endl;
			cout<<"3:QuickSort!"<<endl;
			cout<<"4:QuickSort2!"<<endl;
			cout<<"5:Heapsort!"<<endl;
			cin>>sorting_choice;
			if(sorting_choice == 1)
			{
				clock_t tStart = clock();
				insertion_sort(array_containing_numbers,10000);
							cout<<endl;
				printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
				cout<<endl;
	


			}
			else if(sorting_choice == 2)
			{
				clock_t tStart = clock();
				partitioning_of_merge_sort(array_containing_numbers,0,10000);

							cout<<endl;
				printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
				cout<<endl;
	
			}
			else if(sorting_choice == 3)
			{
				clock_t tStart = clock();
				//quick_sort(array_containing_numbers,0,9999);
				iterativequicksort(array_containing_numbers,0,9999);
							cout<<endl;
				printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
				cout<<endl;
	
			}
			else if(sorting_choice == 4)
			{
				clock_t tStart = clock();
				//quick_sort_using_median_of_3(array_containing_numbers,0,9999);
				iterativequicksortmedianof3(array_containing_numbers,0,9999);
							cout<<endl;
				printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
				cout<<endl;
	

			}
			else if(sorting_choice == 5)
			{
				
				array_containing_numbers = new int [10002];
				for(int i = 1;i < 10001 ; i++)
				{
					array_containing_numbers[i] = i;
				}
				for(int i = 1 ;i < 10001 ;i ++)
				{
					swap(array_containing_numbers[i],array_containing_numbers[random(1,10000)]);
				}
				for(int i = 1 ;i < 10001 ;i ++)
				{
					swap(array_containing_numbers[i],array_containing_numbers[random(1,10000)]);
				}


				clock_t tStart = clock();
				build_max_heap(array_containing_numbers,10001);
				heapsort(array_containing_numbers,10001);
				
				cout<<endl;
				printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
				cout<<endl;

				

			}
		}
		else if(choice == 2)
		{
			for(int i = 0;i<10000 ; i++)
			{
				array_containing_numbers[i] = i;
			}

			int sorting_choice = 0;
			cout<<"Enter Sorting Choice"<<endl;
			cout<<"1:Insertion Sort!"<<endl;
			cout<<"2:Merge Sort!"<<endl;
			cout<<"3:QuickSort!"<<endl;
			cout<<"4:QuickSort2!"<<endl;
			cout<<"5:Heapsort!"<<endl;
			cin>>sorting_choice;
			if(sorting_choice == 1)
			{
				clock_t tStart = clock();
				insertion_sort(array_containing_numbers,10000);
					cout<<endl;
				printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
				cout<<endl;


			}
			else if(sorting_choice == 2)
			{
				clock_t tStart = clock();
				partitioning_of_merge_sort(array_containing_numbers,0,10000);

					cout<<endl;
				printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
				cout<<endl;

			}
			else if(sorting_choice == 3)
			{
				clock_t tStart = clock();
				//quick_sort(array_containing_numbers,0,9999);
				iterativequicksort(array_containing_numbers,0,9999);	
				cout<<endl;
				printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
				cout<<endl;

			}
			else if(sorting_choice == 4)
			{

				
				clock_t tStart = clock();
				//quick_sort_using_median_of_3(array_containing_numbers,0,9999);
				iterativequicksortmedianof3(array_containing_numbers,0,9999);

					cout<<endl;
				printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
				cout<<endl;
				

			}
			else if(sorting_choice == 5)
			{

				
					array_containing_numbers = new int [10002];
				for(int i = 1;i < 10001 ; i++)
				{
					array_containing_numbers[i] = i;
				}

				clock_t tStart = clock();
				build_max_heap(array_containing_numbers,10001);
				heapsort(array_containing_numbers,10001);
				cout<<endl;
				printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
				cout<<endl;



		
			}
		}
		else if(choice == 3)
		{
			for(int i = 0 ;i<9000 ;i++)
			{
				array_containing_numbers[i] = i;
			}
			for(int i = 9000 ;i<10000 ;i ++)
			{
				array_containing_numbers[i]  = rand()%100;
			}


			int sorting_choice = 0;
			cout<<"Enter Sorting Choice"<<endl;
			cout<<"1:Insertion Sort!"<<endl;
			cout<<"2:Merge Sort!"<<endl;
			cout<<"3:QuickSort!"<<endl;
			cout<<"4:QuickSort2!"<<endl;
			cout<<"5:Heapsort!"<<endl;
			cin>>sorting_choice;
			if(sorting_choice == 1)
			{
				clock_t tStart = clock();
				insertion_sort(array_containing_numbers,10000);
								cout<<endl;
				printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
				cout<<endl;



			}
			else if(sorting_choice == 2)
			{
				clock_t tStart = clock();
				partitioning_of_merge_sort(array_containing_numbers,0,10000);
								cout<<endl;
				printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
				cout<<endl;



			}
			else if(sorting_choice == 3)
			{
				clock_t tStart = clock();
				//quick_sort(array_containing_numbers,0,9999);
				iterativequicksort(array_containing_numbers,0,9999);	
				
				cout<<endl;
				printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
				cout<<endl;


			}
			else if(sorting_choice == 4)
			{
				clock_t tStart = clock();
				//quick_sort_using_median_of_3(array_containing_numbers,0,9999);
				iterativequicksortmedianof3(array_containing_numbers,0,9999);
				
				cout<<endl;
				printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
				cout<<endl;


			}
			else if(sorting_choice == 5)
			{

					array_containing_numbers = new int [10002];
				
				for(int i = 1 ;i<9001 ;i++)
				{
					array_containing_numbers[i] = i;
				}
				for(int i = 9001 ;i<10001 ;i ++)
				{
					array_containing_numbers[i]  = rand()%100;
				}

		
				cout<<endl;
				clock_t tStart = clock();
				build_max_heap(array_containing_numbers,10001);
				heapsort(array_containing_numbers,10001);
				
				cout<<endl;
				printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
				cout<<endl;
			}
		}
		else if(choice == 4)
		{
			int tmp;
			tmp = 0;
			for(int i = 10000 ;i > 0  ;i--)
			{
				array_containing_numbers[tmp++] = i;
			}

			int sorting_choice = 0;
			cout<<"Enter Sorting Choice"<<endl;
			cout<<"1:Insertion Sort!"<<endl;
			cout<<"2:Merge Sort!"<<endl;
			cout<<"3:QuickSort!"<<endl;
			cout<<"4:QuickSort2!"<<endl;
			cout<<"5:Heapsort!"<<endl;
			cin>>sorting_choice;
			if(sorting_choice == 1)
			{
				clock_t tStart = clock();
				insertion_sort(array_containing_numbers,10000);
				cout<<endl;
				printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
				cout<<endl;

			}
			else if(sorting_choice == 2)
			{
				clock_t tStart = clock();
				partitioning_of_merge_sort(array_containing_numbers,0,10000);
				cout<<endl;
				printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
				cout<<endl;

			}
			else if(sorting_choice == 3)
			{
				clock_t tStart = clock();
				//quick_sort(array_containing_numbers,0,9999);
				iterativequicksort(array_containing_numbers,0,9999);
				printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
				cout<<endl;
			}
			else if(sorting_choice == 4)
			{
				clock_t tStart = clock();
				
				//quick_sort_using_median_of_3(array_containing_numbers,0,9999);
				iterativequicksortmedianof3(array_containing_numbers,0,9999);
				
				cout<<endl;
				printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
				cout<<endl;

			}
			else if(sorting_choice == 5)
			{
				
				array_containing_numbers = new int [10002];
				tmp = 1;
				for(int i = 10000 ;i >=1  ;i--)
				{
					array_containing_numbers[tmp++] = i;
				}

				

				clock_t tStart = clock();
				build_max_heap(array_containing_numbers,10001);
				heapsort(array_containing_numbers,10001);
				cout<<endl;
				printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
				cout<<endl;

			}
		}

	}
	else if(array_size_choice == 100000)
	{
		array_containing_numbers = new int [100000](); 
		int choice = 0;
		cout<<"Enter Choice for array type!"<<endl;
		cout<<"1:Completely Random!"<<endl;
		cout<<"2:Sorted!"<<endl;
		cout<<"3:Almost Sorted (90% in increasing order and the rest 10% random)!"<<endl;
		cout<<"4:Reversed Sorted(Sorted Numbers are in reverse order)!"<<endl;
		cin>>choice;


		if(choice == 1)
		{
			for(int i = 0;i<100000 ; i++)
			{
				array_containing_numbers[i] = i;
			}
			for(int i = 0 ;i < 100000 ;i ++)
			{
				swap(array_containing_numbers[i],array_containing_numbers[random(0,100000)]);
			}
			for(int i = 0 ;i < 100000 ;i ++)
			{
				swap(array_containing_numbers[i],array_containing_numbers[random(0,100000)]);
			}


			int sorting_choice = 0;
			cout<<"Enter Sorting Choice"<<endl;
			cout<<"1:Insertion Sort!"<<endl;
			cout<<"2:Merge Sort!"<<endl;
			cout<<"3:QuickSort!"<<endl;
			cout<<"4:QuickSort2!"<<endl;
			cout<<"5:Heapsort!"<<endl;
			cin>>sorting_choice;
			if(sorting_choice == 1)
			{
				clock_t tStart = clock();
				insertion_sort(array_containing_numbers,100000);
								cout<<endl;
				printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
				cout<<endl;



			}
			else if(sorting_choice == 2)
			{
				clock_t tStart = clock();
				partitioning_of_merge_sort(array_containing_numbers,0,100000);
								cout<<endl;
				printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
				cout<<endl;


			}
			else if(sorting_choice == 3)
			{
				clock_t tStart = clock();
			//	quick_sort(array_containing_numbers,0,99999);
				iterativequicksort(array_containing_numbers,0,99999);
				
				cout<<"After Applying Quick Sort on array of size 100000 and containing random numbers we have"<<endl;
								cout<<endl;
				printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
				cout<<endl;


			}
			else if(sorting_choice == 4)
			{
				clock_t tStart = clock();
				//quick_sort_using_median_of_3(array_containing_numbers,0,99999);
				iterativequicksortmedianof3(array_containing_numbers,0,99999);
				
				cout<<"After Applying Quick Sort 2 on array of size 100000 and containing random numbers we have"<<endl;
								cout<<endl;
				printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
				cout<<endl;


			}
			else if(sorting_choice == 5)
			{
				array_containing_numbers = new int [100002];
				for(int i = 1;i < 100001 ; i++)
				{
					array_containing_numbers[i] = i;
				}
				for(int i = 1 ;i < 100001 ;i ++)
				{
					swap(array_containing_numbers[i],array_containing_numbers[random(1,100000)]);
				}
				for(int i = 1 ;i < 100001 ;i ++)
				{
					swap(array_containing_numbers[i],array_containing_numbers[random(1,100000)]);
				}


				cout<<endl;
				clock_t tStart = clock();
				build_max_heap(array_containing_numbers,100001);
				heapsort(array_containing_numbers,100001);
				cout<<endl;
				printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
				cout<<endl;

			
			}
		}
		else if(choice == 2)
		{
			for(int i = 0;i<100000 ; i++)
			{
				array_containing_numbers[i] = i;
			}

			int sorting_choice = 0;
			cout<<"Enter Sorting Choice"<<endl;
			cout<<"1:Insertion Sort!"<<endl;
			cout<<"2:Merge Sort!"<<endl;
			cout<<"3:QuickSort!"<<endl;
			cout<<"4:QuickSort2!"<<endl;
			cout<<"5:Heapsort!"<<endl;
			cin>>sorting_choice;
			if(sorting_choice == 1)
			{
				clock_t tStart = clock();
				insertion_sort(array_containing_numbers,100000);

				cout<<endl;
				printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
				cout<<endl;


			}
			else if(sorting_choice == 2)
			{
				clock_t tStart = clock();
				partitioning_of_merge_sort(array_containing_numbers,0,100000);

				cout<<endl;
				printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
				cout<<endl;

			}
			else if(sorting_choice == 3)
			{
				clock_t tStart = clock();
				//quick_sort(array_containing_numbers,0,99999);
				iterativequicksort(array_containing_numbers,0,99999);
				
				cout<<endl;
				printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
				cout<<endl;

			}
			else if(sorting_choice == 4)
			{
				clock_t tStart = clock();
				//quick_sort_using_median_of_3(array_containing_numbers,0,99999);
				iterativequicksortmedianof3(array_containing_numbers,0,99999);

				cout<<endl;
				printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
				cout<<endl;

			}
			else if(sorting_choice == 5)
			{
				
				array_containing_numbers = new int [100002];
				for(int i = 1;i < 100001 ; i++)
				{
					array_containing_numbers[i] = i;
				}


				cout<<endl;
				clock_t tStart = clock();
				build_max_heap(array_containing_numbers,100001);
				heapsort(array_containing_numbers,100001);
				
				cout<<endl;
				printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
				cout<<endl;



			}
		}
		else if(choice == 3)
		{
			for(int i = 0 ;i<90000 ;i++)
			{
				array_containing_numbers[i] = i;
			}
			for(int i = 90000 ;i<100000 ;i ++)
			{
				array_containing_numbers[i]  = rand()%100;
			}


			int sorting_choice = 0;
			cout<<"Enter Sorting Choice"<<endl;
			cout<<"1:Insertion Sort!"<<endl;
			cout<<"2:Merge Sort!"<<endl;
			cout<<"3:QuickSort!"<<endl;
			cout<<"4:QuickSort2!"<<endl;
			cout<<"5:Heapsort!"<<endl;
			cin>>sorting_choice;
			if(sorting_choice == 1)
			{
				clock_t tStart = clock();
				insertion_sort(array_containing_numbers,100000);
						cout<<endl;
				printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
				cout<<endl;


			}
			else if(sorting_choice == 2)
			{
				clock_t tStart = clock();
				partitioning_of_merge_sort(array_containing_numbers,0,100000);
						cout<<endl;
				printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
				cout<<endl;


			}
			else if(sorting_choice == 3)
			{
				clock_t tStart = clock();
				//quick_sort(array_containing_numbers,0,99999);
				iterativequicksort(array_containing_numbers,0,99999);	
				
				cout<<endl;
				printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
				cout<<endl;

			}
			else if(sorting_choice == 4)
			{
				clock_t tStart = clock();
				//quick_sort_using_median_of_3(array_containing_numbers,0,99999);
				iterativequicksortmedianof3(array_containing_numbers,0,99999);
						cout<<endl;
				printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
				cout<<endl;

			}
			else if(sorting_choice == 5)
			{

				array_containing_numbers = new int [100002];
				
				for(int i = 1 ;i<90001 ;i++)
				{
					array_containing_numbers[i] = i;
				}
				for(int i = 90001 ;i<100001 ;i ++)
				{
					array_containing_numbers[i]  = rand()%100;
				}

		

				cout<<endl;
				clock_t tStart = clock();
				build_max_heap(array_containing_numbers,100001);
				heapsort(array_containing_numbers,100001);
				
				printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
				cout<<endl;
			
				
			}
		}
		else if(choice == 4)
		{
			int tmp;
			tmp = 0;
			for(int i = 100000 ;i > 0  ;i--)
			{
				array_containing_numbers[tmp++] = i;
			}

			int sorting_choice = 0;
			cout<<"Enter Sorting Choice"<<endl;
			cout<<"1:Insertion Sort!"<<endl;
			cout<<"2:Merge Sort!"<<endl;
			cout<<"3:QuickSort!"<<endl;
			cout<<"4:QuickSort2!"<<endl;
			cout<<"5:Heapsort!"<<endl;
			cin>>sorting_choice;
			if(sorting_choice == 1)
			{
				clock_t tStart = clock();
				insertion_sort(array_containing_numbers,100000);
					cout<<endl;
				printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
				cout<<endl;
			

			}
			else if(sorting_choice == 2)
			{
				clock_t tStart = clock();
				partitioning_of_merge_sort(array_containing_numbers,0,100000);

					cout<<endl;
				printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
				cout<<endl;
			
			}
			else if(sorting_choice == 3)
			{
				clock_t tStart = clock();
			//	quick_sort(array_containing_numbers,0,99999);
				iterativequicksort(array_containing_numbers,0,99999);
				cout<<"After Applying Quick Sort on array of size 100000 and containing numbers in reversed order"<<endl;
				printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
				cout<<endl;
			
			}
			else if(sorting_choice == 4)
			{
				clock_t tStart = clock();
				//quick_sort_using_median_of_3(array_containing_numbers,0,99999);
				iterativequicksortmedianof3(array_containing_numbers,0,99999);
					cout<<endl;
				printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
				cout<<endl;
			
			}
			else if(sorting_choice == 5)
			{
					array_containing_numbers = new int [100002];
				tmp = 1;
				for(int i = 100000 ;i >=1  ;i--)
				{
					array_containing_numbers[tmp++] = i;
				}

				

				clock_t tStart = clock();
				build_max_heap(array_containing_numbers,100001);
				heapsort(array_containing_numbers,100001);
				cout<<endl;
				printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
				cout<<endl;


			}
		}

	}
	else if(array_size_choice == 1000000)
	{
		array_containing_numbers = new int [1000000](); 
		int choice = 0;
		cout<<"Enter Choice for array type!"<<endl;
		cout<<"1:Completely Random!"<<endl;
		cout<<"2:Sorted!"<<endl;
		cout<<"3:Almost Sorted (90% in increasing order and the rest 10% random)!"<<endl;
		cout<<"4:Reversed Sorted(Sorted Numbers are in reverse order)!"<<endl;
		cin>>choice;


		if(choice == 1)
		{
			for(int i = 0;i<1000000 ; i++)
			{
				array_containing_numbers[i] = i;
			}
			for(int i = 0 ;i < 1000000 ;i ++)
			{
				swap(array_containing_numbers[i],array_containing_numbers[random(0,1000000)]);
			}
			for(int i = 0 ;i < 1000000 ;i ++)
			{
				swap(array_containing_numbers[i],array_containing_numbers[random(0,1000000)]);
			}


			int sorting_choice = 0;
			cout<<"Enter Sorting Choice"<<endl;
			cout<<"1:Insertion Sort!"<<endl;
			cout<<"2:Merge Sort!"<<endl;
			cout<<"3:QuickSort!"<<endl;
			cout<<"4:QuickSort2!"<<endl;
			cout<<"5:Heapsort!"<<endl;
			cin>>sorting_choice;
			if(sorting_choice == 1)
			{
				clock_t tStart = clock();
				insertion_sort(array_containing_numbers,1000000);
					cout<<endl;
				printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
				cout<<endl;



			}
			else if(sorting_choice == 2)
			{
				clock_t tStart =clock();
				partitioning_of_merge_sort(array_containing_numbers,0,1000000);
					cout<<endl;
				printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
				cout<<endl;


			}
			else if(sorting_choice == 3)
			{
				clock_t tStart = clock();
				//quick_sort(array_containing_numbers,0,999999);
				iterativequicksort(array_containing_numbers,0,999999);	
				cout<<endl;
				printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
				cout<<endl;

			}
			else if(sorting_choice == 4)
			{
				clock_t tStart = clock();
			//	quick_sort_using_median_of_3(array_containing_numbers,0,999999);
				iterativequicksortmedianof3(array_containing_numbers,0,999999);
				cout<<endl;
				printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
				cout<<endl;



			}
			else if(sorting_choice == 5)
			{
						array_containing_numbers = new int [1000002];
				for(int i = 1;i < 1000001 ; i++)
				{
					array_containing_numbers[i] = i;
				}
				for(int i = 1 ;i < 1000001 ;i ++)
				{
					swap(array_containing_numbers[i],array_containing_numbers[random(1,1000000)]);
				}
				for(int i = 1 ;i < 1000001 ;i ++)
				{
					swap(array_containing_numbers[i],array_containing_numbers[random(1,1000000)]);
				}


				cout<<endl;
				clock_t tStart = clock();
				build_max_heap(array_containing_numbers,1000001);
				heapsort(array_containing_numbers,1000001);
				
				cout<<endl;
				printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
				cout<<endl;

	
			}
		}
		else if(choice == 2)
		{
			for(int i = 0;i<1000000 ; i++)
			{
				array_containing_numbers[i] = i;
			}

			int sorting_choice = 0;
			cout<<"Enter Sorting Choice"<<endl;
			cout<<"1:Insertion Sort!"<<endl;
			cout<<"2:Merge Sort!"<<endl;
			cout<<"3:QuickSort!"<<endl;
			cout<<"4:QuickSort2!"<<endl;
			cout<<"5:Heapsort!"<<endl;
			cin>>sorting_choice;
			if(sorting_choice == 1)
			{
				clock_t tStart = clock();
				insertion_sort(array_containing_numbers,1000000);
					cout<<endl;
				printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
				cout<<endl;


			}
			else if(sorting_choice == 2)
			{
				clock_t tStart = clock();
				partitioning_of_merge_sort(array_containing_numbers,0,1000000);
					cout<<endl;
				printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
				cout<<endl;



			}
			else if(sorting_choice == 3)
			{
				clock_t tStart = clock();
				//quick_sort(array_containing_numbers,0,999999);
				iterativequicksort(array_containing_numbers,0,999999);	
				cout<<endl;
				printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
				cout<<endl;


			}
			else if(sorting_choice == 4)
			{
				clock_t tStart = clock();
				//quick_sort_using_median_of_3(array_containing_numbers,0,999999);
				iterativequicksortmedianof3(array_containing_numbers,0,999999);	
				cout<<endl;
				printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
				cout<<endl;


			}
			else if(sorting_choice == 5)
			{
				
				array_containing_numbers = new int [1000002];
				for(int i = 1;i < 1000001 ; i++)
				{
					array_containing_numbers[i] = i;
				}


				cout<<endl;
				clock_t tStart = clock();
				build_max_heap(array_containing_numbers,1000001);
				heapsort(array_containing_numbers,1000001);
				
				cout<<endl;
				printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
				cout<<endl;

			}
		}
		else if(choice == 3)
		{
			for(int i = 0 ;i<900000 ;i++)
			{
				array_containing_numbers[i] = i;
			}
			for(int i = 900000 ;i<1000000 ;i ++)
			{
				array_containing_numbers[i]  = rand()%100;
			}


			int sorting_choice = 0;
			cout<<"Enter Sorting Choice"<<endl;
			cout<<"1:Insertion Sort!"<<endl;
			cout<<"2:Merge Sort!"<<endl;
			cout<<"3:QuickSort!"<<endl;
			cout<<"4:QuickSort2!"<<endl;
			cout<<"5:Heapsort!"<<endl;
			cin>>sorting_choice;
			if(sorting_choice == 1)
			{
				clock_t tStart = clock();
				insertion_sort(array_containing_numbers,1000000);
					cout<<endl;
				printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
				cout<<endl;


			}
			else if(sorting_choice == 2)
			{
				clock_t tStart = clock();
				partitioning_of_merge_sort(array_containing_numbers,0,1000000);

					cout<<endl;
				printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
				cout<<endl;

			}
			else if(sorting_choice == 3)
			{
				clock_t tStart =clock();
			//	quick_sort(array_containing_numbers,0,999999);
				iterativequicksort(array_containing_numbers,0,999999);	
				cout<<endl;
				printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
				cout<<endl;

			}
			else if(sorting_choice == 4)
			{
				clock_t tStart = clock();
				//quick_sort_using_median_of_3(array_containing_numbers,0,999999);
				iterativequicksortmedianof3(array_containing_numbers,0,999999);	
				cout<<endl;
				printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
				cout<<endl;


			}
			else if(sorting_choice == 5)
			{
				
				array_containing_numbers = new int [1000002];
				
				for(int i = 1 ;i<900001 ;i++)
				{
					array_containing_numbers[i] = i;
				}
				for(int i = 900001 ;i<1000001 ;i ++)
				{
					array_containing_numbers[i]  = rand()%100;
				}

		

				clock_t tStart = clock();
				build_max_heap(array_containing_numbers,1000001);
				heapsort(array_containing_numbers,1000001);
				
				cout<<endl;
				printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
				cout<<endl;
			
			}
		}
		else if(choice == 4)
		{
			int tmp;
			tmp = 0;
			for(int i = 1000000 ;i > 0  ;i--)
			{
				array_containing_numbers[tmp++] = i;
			}

			int sorting_choice = 0;
			cout<<"Enter Sorting Choice"<<endl;
			cout<<"1:Insertion Sort!"<<endl;
			cout<<"2:Merge Sort!"<<endl;
			cout<<"3:QuickSort!"<<endl;
			cout<<"4:QuickSort2!"<<endl;
			cout<<"5:Heapsort!"<<endl;
			cin>>sorting_choice;
			if(sorting_choice == 1)
			{
				clock_t tStart = clock();
				insertion_sort(array_containing_numbers,1000000);
				cout<<endl;
				printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
				cout<<endl;
			

			}
			else if(sorting_choice == 2)
			{
				clock_t tStart = clock();
				partitioning_of_merge_sort(array_containing_numbers,0,1000000);

				cout<<endl;
				printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
				cout<<endl;
			
			}
			else if(sorting_choice == 3)
			{
				clock_t tStart = clock();
				//quick_sort(array_containing_numbers,0,999999);
				iterativequicksort(array_containing_numbers,0,999999);
				cout<<endl;
				printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
				cout<<endl;
			
			}
			else if(sorting_choice == 4)
			{
				clock_t tStart = clock();
				//quick_sort_using_median_of_3(array_containing_numbers,0,999999);
				iterativequicksortmedianof3(array_containing_numbers,0,999999);
				cout<<endl;
				printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
				cout<<endl;
			

			}
			else if(sorting_choice == 5)
			{
					array_containing_numbers = new int [1000002];
				tmp = 1;
				for(int i = 1000000 ;i >=1  ;i--)
				{
					array_containing_numbers[tmp++] = i;
				}

				

				clock_t tStart = clock();
				build_max_heap(array_containing_numbers,1000001);
				heapsort(array_containing_numbers,1000001);
				
				printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
				cout<<endl;

			
			}
		}

	}
	system("pause");
	return 0;
}

