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
void partitioning_of_merge_sort(int arr[],int low,int high,int size_of_sublist)
{

	int mid=0;
	if(high - low >size_of_sublist)

	{

		mid = (low + high) / 2;

		partitioning_of_merge_sort(arr, low, mid,size_of_sublist);

		partitioning_of_merge_sort(arr, mid + 1, high,size_of_sublist);

		merge_sort(arr, low, mid, high);

	}

	else
	{
		insertion_sort(arr,high);
	}


}
void standard_merge_sort(int  arr[] , int low,int mid ,int high)
{

	int i, mid_after, lo;
	int temp[100005];



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
void standard_partitioning_of_merge_sort(int arr[],int low,int high)
{

	int mid=0;



	if(low < high)

	{

		mid = (low + high) / 2;

		standard_partitioning_of_merge_sort(arr, low, mid);

		standard_partitioning_of_merge_sort(arr, mid + 1, high);

		standard_merge_sort(arr, low, mid, high);

	}


}

int main()
{

	int choice;
	cout<<"Enter choice"<<endl;
	cout<<"1:1000"<<endl;
	cout<<"2:10000"<<endl;
	cout<<"3:50000"<<endl;
	cout<<"4:100000"<<endl;
	cin>>choice;


	int value_of_k;
	value_of_k = 0;
	cout<<"Enter the value of k"<<endl;
	cin>>value_of_k;

	int * arr;


	if(choice == 1)
	{
		arr = new int[1000]();
		for(int i = 0 ;i <1000; i++)
		{
			arr[i] = i;
		}
		for(int i = 0 ;i < 1000; i++)
		{
			swap(arr[i],arr[random(1,1000)]);
		}

		for(int i = 0 ;i < 1000; i++)
		{
			swap(arr[i],arr[random(1,1000)]);
		}

		clock_t tStart = clock();
		
	//	standard_partitioning_of_merge_sort(arr,0,1000);
		partitioning_of_merge_sort(arr,0,1000,value_of_k);


		printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);





	}
	else if(choice ==2)
	{
		arr = new int[10000]();
		for(int i = 0 ;i <10000; i++)
		{
			arr[i] = i;
		}

		for(int i = 0 ;i < 10000; i++)
		{
			swap(arr[i],arr[random(1,10000)]);
		}
		for(int i = 0 ;i < 10000; i++)
		{
			swap(arr[i],arr[random(1,10000)]);
		}
		clock_t tStart = clock();
		
		standard_partitioning_of_merge_sort(arr,0,10000);
		//partitioning_of_merge_sort(arr,0,10000,value_of_k);


		printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);



	}
	else if(choice ==3)
	{
		arr = new int[50000]();
		for(int i = 0 ;i <50000; i++)
		{
			arr[i] = i;
		}
		for(int i = 0 ;i < 50000; i++)
		{
			swap(arr[i],arr[random(1,50000)]);
		}
		for(int i = 0 ;i < 50000; i++)
		{
			swap(arr[i],arr[random(1,50000)]);
		}

		clock_t tStart = clock();
		
		standard_partitioning_of_merge_sort(arr,0,50000);
	//	partitioning_of_merge_sort(arr,0,50000,value_of_k);


		printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);




	}
	else if(choice == 4)
	{
		arr = new int[100000]();
		for(int i = 0 ;i <100000; i++)
		{
			arr[i] = i;
		}

		for(int i = 0 ;i < 100000; i++)
		{
			swap(arr[i],arr[random(1,100000)]);
		}
		for(int i = 0 ;i < 100000; i++)
		{
			swap(arr[i],arr[random(1,100000)]);
		}

		clock_t tStart = clock();
		
		standard_partitioning_of_merge_sort(arr,0,100000);
		//partitioning_of_merge_sort(arr,0,100000,value_of_k);


		printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);




	}



	system("pause");
	return 0;
}
