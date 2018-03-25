#include<iostream>
#include<vector>
#include<iomanip>
#include<string>
#include<fstream>
#include<Windows.h>
#include<sstream>
using namespace std;


//declaring 2D array to get the matrix from the file

int ** arr;//this contains the matrix of the file


HANDLE hconsole=0;

int ** new_population;
int ** current_population;
int ** temp_for_current_population;
int ** array_containing_both_new_and_old_population;
int result_chromosome_1_index;
int result_chromosome_2_index;
int result_chromosome_3_index;
float * array_containing_probabilities_of_chromosomes;
float * array_containing_probabilities_of_chromosomes_1;
int ** two_new_chromosomes;
bool break_flag;


//function used to generate a random number between a given range
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
float calculate_fitness(int arr1[])
{
	bool add_impurity;
	add_impurity = false;
	float sum;
	sum = 0;
	for(int i = 0; i<311 ; i++)
	{
		sum = sum + arr[arr1[i]][arr1[i+1]];
	}

	sum = sum + arr[arr1[311]][arr1[0]];

	//this check is being performed if
	//the chromosome is invalid 
	//then decreasing its fitness to 
	//to decrease its probability of
	//selection
	for(int i=0;i<312;i++)
	{
		for(int j = i+1;j<312;j++)
		{
			if(arr1[i] == arr1[j])
			{
				add_impurity = true;
				break;
			}
		}
		if(add_impurity == true)
		{
			sum = sum + 100;
			break;
		}

	}

	return sum;
}
int** single_point_crossover(int chromosome_1[],int chromosome_2[])
{
	int ** temp;
	temp = new int*[2];
	for(int  i = 0 ;i < 2;i++)
	{
		temp[i] = new int [312]; 
	}



	int random_number = 0;
	random_number = random(1,312);
	int temp_array_size = 312 - random_number;
	int *Arr = new int [temp_array_size]();
	int t;
	t = 0 ;
	for(int i = random_number ; i < 312 ; i ++)
	{
		Arr[t++] = chromosome_1[i]; 
	}
	for(int i = random_number ; i< 312 ;i ++)
	{
		chromosome_1[i] = chromosome_2 [i]; 
	}

	t = 0;
	for(int  i = random_number ;i <312 ; i++)
	{
		chromosome_2[i] = Arr[t++];
	}

	for(int i = 0 ;i < 312 ; i++)
	{
		temp[0][i] = chromosome_1[i];
	}

	for(int i = 0 ;i < 312 ; i++)
	{
		temp[1][i] = chromosome_2[i];
	}



	return temp;
}
int** two_point_crossover(int chromosome_1[],int chromosome_2[])
{
	int ** temp;
	temp = new int*[2];
	for(int  i = 0 ;i < 2;i++)
	{
		temp[i] = new int [312]; 
	}


	int pointOne = random(0,312);
	int pointTwo = random(0,312);

	while(pointOne < pointTwo){            //To ensure that pointTwo is always greater than pointOne
		pointOne = random(0,312);
		pointTwo = random(0,312);
	}


	for(int i=0; i<pointOne; i++){           //First part of parent chromosome directly goes to children
		temp[0][i]=chromosome_1[i];
		temp[1][i] = chromosome_2[i];
	}


	for(int j=pointOne; j<pointTwo; j++){            //Second part goes with inversion
		
		temp[0][j] = chromosome_2[j];
		temp[1][j] = chromosome_1[j];
	}

	for(int k=pointTwo; k<312; k++){              //Third part of parent chromosome goes directly into children
	
		temp[0][k] = chromosome_1[k];
		temp[1][k] = chromosome_2[k];
	}

return temp;

}
int** uniform_point_crossover(int chromosome_1[],int chromosome_2[])
{
	int ** temp;
	temp = new int*[2];
	for(int  i = 0 ;i < 2;i++)
	{
		temp[i] = new int [312]; 
	}

	int window=20;
	//Bits are to be randomnly copied from the parents

	int randomNumber = random(0,window);    //This is the window size 
	int previous =0;


	for(int i=0; i<312; i++){                           // To Avoid any problem in next loop
		temp[0][i] = chromosome_1[i];
		temp[1][i] = chromosome_2[i];
	}

	bool flag=true;

	for(int i=0; (previous+randomNumber)<312    && flag==true ; i++){
	
		for(int k= previous; k<=(randomNumber+previous)    && k<312; k++){
		
			temp[0][k] = chromosome_1[k];
			temp[1][k]= chromosome_2[k];
		}

		previous+=randomNumber;
		 randomNumber = random(0,window); 
		 if((previous+randomNumber)>=312){
			 randomNumber= 312-previous;
			 flag = false;
		 }


		for(int p=previous; p<=(randomNumber+previous) && p<312; p++){
			temp[0][p] = chromosome_2[p];
			temp[1][p] = chromosome_1[p];
		}

		previous+=randomNumber;
		 randomNumber = random(0,window); 
		 if((previous+randomNumber)>=312){
			 randomNumber= 312-previous;
			 flag = false;
		 }



	
	}


	return temp;
}
int** first_order_crossover(int chromosome_1[],int chromosome_2[])
{
	int ** temp;
	temp = new int*[2];
	for(int  i = 0 ;i < 2;i++)
	{
		temp[i] = new int [312]; 
	}


	int firstCutPoint = random(1,312);
	int secondCutPoint = random(1,312);

	while(firstCutPoint>=secondCutPoint){      //Loop to ensure that first cut point is <= second cut point

		firstCutPoint = random(1,312);
		secondCutPoint = random(1,312);
	}

	for(int i=firstCutPoint; i<=secondCutPoint; i++){
		temp[0][i] = chromosome_1[i];                                         //Copy the data from first Parent between cutpoints to first child
	}



	int indextobecopied =0;
	int number =0;
	bool flag = false;

	int termination = (secondCutPoint - firstCutPoint) +1;

	int k=secondCutPoint+1;
	for(int i=secondCutPoint+1; termination<312  ; i++){

		flag = false;
		indextobecopied = i%312;
		number = chromosome_2[indextobecopied];

		for(int j=firstCutPoint; j<=secondCutPoint; j++){
			if(number == temp[0][j]  ){
				flag = true;
			}
		}


		if(!flag){
			temp[0][k%312] = number;
			k++;
			flag =false;
			termination++;
		}

	}

	//========================================================================================
	//Second Child

	int firstCutPoint1 = random(1,312);
	int secondCutPoint1 = random(1,312);

	while(firstCutPoint1>=secondCutPoint1){      //Loop to ensure that first cut point is <= second cut point

		firstCutPoint1 = random(1,312);
		secondCutPoint1 = random(1,312);
	}

	for(int i=firstCutPoint1; i<=secondCutPoint1; i++){
		temp[1][i] = chromosome_2[i];                                         //Copy the data from first Parent between cutpoints to first child
	}



	int indextobecopied1 =0;
	int number1 =0;
	bool flag1 = false;

	int termination1 = (secondCutPoint1 - firstCutPoint1) +1;

	int k1=secondCutPoint1+1;
	for(int i1=secondCutPoint1+1; termination1<312  ; i1++){

		flag1 = false;
		indextobecopied1 = i1%312;
		number1= chromosome_1[indextobecopied1];

		for(int j=firstCutPoint1; j<=secondCutPoint1; j++){
			if(number1 == temp[1][j]  ){
				flag1 = true;
			}
		}


		if(!flag1){
			temp[1][k1%312] = number1;
			k1++;
			flag1 =false;
			termination1++;
		}

	}

	return temp;
}
void  doing_mutation(int ** arr1,int mutation_rate,int size_of_population)
{
	//here i will do the mutation for the 20 chromosomes 
	int random_number_1 = random(1,100);
	int random_number_2 ; 
	if(mutation_rate<=random_number_1)
	{
		for(int i = 0 ;i<size_of_population;i++)
		{
			random_number_2 =random(0,311);
			arr1[i][random_number_2] = random(0,311);
		}
	}	
}
int check_population(int i)
{
	if(i%2 == 1)
	{
		i = i +1;
	}
	return i;
}
int main()
{

	hconsole=GetStdHandle(STD_OUTPUT_HANDLE);

	arr = new int * [312];
	for(int  i = 0; i<312;i++)
	{
		arr[i] = new int [312];
	}



	SetConsoleTextAttribute(hconsole,13);

	//reading the matrix from the file

	ifstream fin;
	fin.open("finalOutput.txt");
	string whole_file[312];
	int index;
	index = 0;
	while(!fin.eof())
	{
		getline(fin,whole_file[index],'\n');
		index++;
	}

	//now here populate the cost matrix named "arr" using whole_file

	int temp;
	int i_index_of_arr;
	int j_index_of_arr;
	i_index_of_arr = 0;
	j_index_of_arr = 0;
	for(int i=0;i<312;i++)
	{
		j_index_of_arr = 0;
		std::stringstream ss(whole_file[i]);
		while(ss >> temp)
		{
			arr[i_index_of_arr][j_index_of_arr] = temp;
			j_index_of_arr++;

			if(ss.peek()==',')
			{
				ss.ignore();
			}
		}
		i_index_of_arr++;
	}








	//population generation
	//creating chromosomes based on user requirement
	int size_of_population ;
	size_of_population = 0;
	cout<<"Enter the size of the population ?"<<endl;
	cin>>size_of_population;
	size_of_population = check_population(size_of_population);






	current_population =new int *[size_of_population]();
	for(int i = 0;i<size_of_population;i++)
	{
		current_population[i] = new int [312];
	}
	new_population = new int *[size_of_population]();
	for(int i = 0 ;i < size_of_population ; i ++)
	{
		new_population[i] = new int [312]; 
	}

	array_containing_both_new_and_old_population = new int *[2*size_of_population];
	for(int i =0 ;i <(2*size_of_population) ;i ++)
	{
		array_containing_both_new_and_old_population[i] = new int[312];
	}









	int initializer;
	initializer = 0;
	//initializing the initial population
	for(int j = 0;j<size_of_population;j++)
	{
		initializer = 0;
		for(int k=0;k<312;k++)
		{
			current_population[j][k] = initializer++; 
		}
	}

	//now i will apply the random shufling algorithm in each choromosome to make real chromosomes
	//doing this process twice in order to have a good shuffling
	for(int i=0; i<size_of_population;++i)
	{
		for(int j=0;j<312;j++)
		{
			swap(current_population[i][j],current_population[i][random(0,312)]);
		}

	}

	for(int i=0; i<size_of_population;++i)
	{
		for(int j=0;j<312;j++)
		{
			swap(current_population[i][j],current_population[i][random(0,312)]);
		}

	}



	int selection_criteria;
	cout<<"Select Selection Criteria!"<<endl;
	cout<<"1:Stochastic Selection!"<<endl;
	cout<<"2:Rank Selection"<<endl;
	cout<<"3:Tournament Selection"<<endl;
	cout<<"4:Roulett(Bonus) Selection"<<endl;
	cin>>selection_criteria;



	if(selection_criteria ==1)
	{

		//for storing two new choromosomes after
		//cross over

		two_new_chromosomes = new int* [2];
		for(int i = 0 ;i < 2 ;i ++)
		{
			two_new_chromosomes[i] = new int [312];
		}

		/////////////////////////////////////

		break_flag = false;




		int iterations;
		cout<<"How many iterations you want ?"<<endl;
		cin>>iterations;
		int cross_over_criteria;
		cout<<"Select CrossOver Criteria!"<<endl;
		cout<<"1:One Point Crossover!"<<endl;
		cout<<"2:Two Point Crossover!"<<endl;
		cout<<"3:Uniform Crossover!"<<endl;
		cout<<"4:First Order Crossover!"<<endl;
		cin>>cross_over_criteria;


		float best_fitness_required;
		cout<<"Find the max fitness you want to achieve ?"<<endl;
		cin>>best_fitness_required;

		int loop_control_variable_1;
		loop_control_variable_1 = iterations;







		int mutation_rate;

		cout<<"Enter Mutation Rate"<<endl;
		cin>>mutation_rate;

		float random_number_1;
		float random_number_2;
		random_number_1 = 0;
		random_number_2 = 0;


		int chromosome_1_index;
		int chromosome_2_index;
		chromosome_1_index = 0;
		chromosome_2_index = 0;


		int chromosome_1_crossover[312] = {0};
		int chromosome_2_crossover[312] = {0};

		int start;
		int end;


		int index_i_of_two_new_chromosome ;
		int index_j_of_two_new_chromosome;


		int arr1[312] = {0};

		int k;
		k=0;
		int m;
		int n;
		m = 0;
		n = 0;

		float total_fitness = 0;
		int z;
		bool check_1;
		int loop_control_variable; 
		float * array_containing_fitness_of_chromosomes = new float [size_of_population]();
		float * array_containing_fitness_of_new_chromosomes;
		int arry1[312] = {0};
		float * array_containing_new_and_old_fitnesses ;
		int l;
		int indexing1;
		int firstflag;
		int * result_choromosome ;
		int * result_choromosome_1;
		int * best_chromosome_so_far;
		bool break_flag_1;
		int smallest;
		int best_fitness_so_far;
		best_fitness_so_far = 0;

		result_choromosome = new int[312]();
		result_choromosome_1 = new int[312]();
		best_chromosome_so_far = new int [312]();

		int u;
		int v;
		int minimum_distance;
		minimum_distance =0;
		int * temp_for_storing_chromosome = new int[312]();

		if(cross_over_criteria == 1)
		{

			while(loop_control_variable_1>0)
			{

				for(int i = 0 ; i <312 ;i++)
				{
					arr1[i] = 0;
				}
				k=0;
				for(int i=0;i<size_of_population;i++)
				{
					k = 0;
					for(int j=0;j<312;j++)
					{
						arr1[k++] = current_population[i][j];
					}
					array_containing_fitness_of_chromosomes[i] = (1/calculate_fitness(arr1));
				}

	

				//Stochastic selection
				total_fitness = 0;

				for(int i = 0 ;i <size_of_population;i++)
				{
					total_fitness  = total_fitness + array_containing_fitness_of_chromosomes[i];
				}
				array_containing_probabilities_of_chromosomes = new float [size_of_population]();

				for(int i = 0 ; i < size_of_population ; i++)
				{
					array_containing_probabilities_of_chromosomes[i]  = ((array_containing_fitness_of_chromosomes[i] / total_fitness)*100);
				}

				//specifying the ranges in the probability ranges in the array
				for (int i = 1;i < size_of_population ; i++)
				{
					array_containing_probabilities_of_chromosomes[i] = array_containing_probabilities_of_chromosomes[i] + array_containing_probabilities_of_chromosomes[i-1];
				}

				array_containing_probabilities_of_chromosomes_1 = new float [size_of_population+1]();
				array_containing_probabilities_of_chromosomes_1[0] = 0;
				z = 0;
				for(int i = 1;i<size_of_population+1;i++)
				{
					array_containing_probabilities_of_chromosomes_1[i] = array_containing_probabilities_of_chromosomes[z++];
				}

				check_1 = true;
				loop_control_variable = size_of_population/2; 

				index_i_of_two_new_chromosome = 0;
				index_j_of_two_new_chromosome = 0;
				start = 0;
				end = 2;

				//	cout<<"3"<<endl;
				
				//in this loop i am creating the new population
				//based on the current population
				//using one point crossover
				while(loop_control_variable > 0)
				{
	
					random_number_1 = random(1,100);
					random_number_2 = random(1,100);
					for(int i = size_of_population+1;i>0;i--)
					{
						if((random_number_1 <= array_containing_probabilities_of_chromosomes_1[i]) && (random_number_1 >= array_containing_probabilities_of_chromosomes_1[i-1]) )
						{
							for(int j = 0 ; j<312;j++)
							{
								chromosome_1_crossover[j] = current_population[i-1][j];
							}
							break;
						}

					}

					for(int i = size_of_population+1;i>0;i--)
					{
						if((random_number_2 <= array_containing_probabilities_of_chromosomes_1[i]) && (random_number_2 >= array_containing_probabilities_of_chromosomes_1[i-1]) )
						{
							for(int j = 0 ; j<312;j++)
							{
								chromosome_2_crossover[j] = current_population[i-1][j];
							}
							break;
						}

					}

	

					two_new_chromosomes = single_point_crossover(chromosome_1_crossover,chromosome_2_crossover);

					for(int i = start;i<end;i++)
					{
						for(int  j = 0 ;j < 312; j ++)
						{
							new_population[i][j] = two_new_chromosomes[index_i_of_two_new_chromosome][index_j_of_two_new_chromosome++];
						}
						index_i_of_two_new_chromosome++;
						index_j_of_two_new_chromosome = 0;
					}

					start = start + 2;
					end = end+2;
					index_i_of_two_new_chromosome = 0;
					index_j_of_two_new_chromosome = 0;

					loop_control_variable--;
				}

				//			cout<<"5"<<endl;
				doing_mutation(new_population,mutation_rate,size_of_population);
				array_containing_fitness_of_new_chromosomes = new float[size_of_population]();


				for(int  i = 0 ; i <312 ; i++)
				{
					arry1[i] = 0;

				}
				z=0;
				k=0;
				for(int i=0;i<size_of_population;i++)
				{
					z = 0;
					k=0;
					for(int j=0;j<312;j++)
					{
						arry1[k++] = new_population[i][j];
					}
					array_containing_fitness_of_new_chromosomes[i] = (1/calculate_fitness(arry1));
				}

				//				cout<<"6"<<endl;

				//combining the 2 populations
				for(int i = 0 ; i <size_of_population;i++)
				{
					for(int j = 0; j<312 ; j++)
					{
						array_containing_both_new_and_old_population[i][j] = current_population[i][j];
					}
				}

				u = 0;
				for(int i = size_of_population ; i <(2*size_of_population);i++)
				{
					for(int j = 0;j<312 ;j++)
					{
						array_containing_both_new_and_old_population[i][j] = new_population[u][j];
					}
					u++;
				}


				//combining the fitnesses of the 2 populations
				array_containing_new_and_old_fitnesses = new float[2*size_of_population]();
				for(int  i = 0 ;i <size_of_population ; i++)
				{
					array_containing_new_and_old_fitnesses[i] = array_containing_fitness_of_chromosomes[i];
				}
				l = 0;
				for(int i = size_of_population;i<(2*size_of_population);i++)
				{
					array_containing_new_and_old_fitnesses[i]  = array_containing_fitness_of_new_chromosomes[l++];
				}

				//sorting the array which contains the fitness of old and new chromosomes
				//and also the corresponding chromosomes in array containing new and old 
				//chromosomes
				for(int  i = 0 ; i <(2*size_of_population)-1;i++ )
				{
					for(int j = 0;j<(2*size_of_population)-1-i;j++)
					{
						if(array_containing_new_and_old_fitnesses[j]<array_containing_new_and_old_fitnesses[j+1])
						{
							swap(array_containing_new_and_old_fitnesses[j],array_containing_new_and_old_fitnesses[j+1]);
							for(int k = 0 ; k < 312 ;k ++)
							{
								temp_for_storing_chromosome[k] = array_containing_both_new_and_old_population[j][k];
								array_containing_both_new_and_old_population[j][k] = array_containing_both_new_and_old_population[j+1][k];
								array_containing_both_new_and_old_population[j+1][k] = array_containing_both_new_and_old_population[j][k];

							}

						}
					}

				}

				int smallest_index;
				bool flag;
				if((best_fitness_required - array_containing_new_and_old_fitnesses[0])>0)
				{
					smallest = best_fitness_required - array_containing_new_and_old_fitnesses[0];
					smallest_index = 0;
					flag = true;
				}

				//updating the best chromosome so far 
				for(int  i = 1 ;i < (2*size_of_population);i++)
				{
					int temp = best_fitness_required - array_containing_new_and_old_fitnesses[i];
					if(temp > 0) 
					{
						if(temp<smallest)
						{
							smallest = temp;
							smallest_index = i;
							flag = true;


						}
					}
				}

				for(int  i = 0 ; i<312 ; i++)
				{
					best_chromosome_so_far[i] = array_containing_both_new_and_old_population[smallest_index][i];
					result_chromosome_1_index = smallest_index; 
				}





				//updating the population for next iteration

				for(int  i = 0 ; i <size_of_population ; i++)
				{
					for(int j = 0;j <312 ; j ++)
					{
						current_population[i][j]= array_containing_both_new_and_old_population[i][j];
					}
				}



				for(int i = 0 ; i <size_of_population;i++)
				{
					if(array_containing_fitness_of_new_chromosomes[i] >= best_fitness_required)
					{

						for(int  j = 0 ;j < 312 ; j ++)
						{
							result_choromosome[j] = new_population[i][j];
						}
						result_chromosome_2_index = i;
						break_flag = true;
						break;
					}
				}

				if(break_flag == true)
				{
					cout<<"Congratulations Required Fitness Chromosome found!"<<endl;
					for(int i = 0;i<312;i++)
					{
						cout<<result_choromosome[i]<<"|";
					}
					cout<<endl;

					cout<<"Fitness: "<<array_containing_fitness_of_new_chromosomes[result_chromosome_2_index]<<endl;
					cout<<"Iterations: "<<endl;
					cout<<(iterations - loop_control_variable_1)<<endl;
					cout<<"The Pictorial representation of the chromosome is "<<endl;

					break;
				}

				if(break_flag == false)
				{

					for(int i = 0 ; i <size_of_population;i++)
					{
						if(array_containing_fitness_of_chromosomes[i] >= best_fitness_required)
						{

							for(int  j = 0 ;j < 312 ; j ++)
							{
								result_choromosome_1[j] = current_population[i][j];
							}
							result_chromosome_3_index = i;
							break_flag_1 = true;
							break;
						}
					}

					if(break_flag_1 == true)
					{
						cout<<"Congratulations Required Fitness Chromosome found!"<<endl;
						for(int i = 0;i<312;i++)
						{
							cout<<result_choromosome_1[i]<<"|";
						}
						cout<<endl;

						cout<<"Fitness: "<<array_containing_fitness_of_chromosomes[result_chromosome_3_index]<<endl;
						cout<<"Iterations "<<endl;
						cout<<(iterations-loop_control_variable_1)<<endl;
						cout<<"The Pictorial representation of the chromosome is "<<endl;

						break;
					}

				}








				loop_control_variable_1--;

				if(loop_control_variable_1 == 0)
				{
					cout<<"Iterations ended"<<endl;
					cout<<"Best chromosome so far is "<<endl;
					for(int i = 0 ; i <312 ;i++)
					{
						cout<<best_chromosome_so_far[i]<<"|";
					}
					cout<<endl;

					cout<<"Fitness :"<<array_containing_new_and_old_fitnesses[result_chromosome_1_index]<<endl;

					cout<<endl;
					cout<<"The Pictorial Representation of the chromosome is !"<<endl;
					break;
				}
			}


		}
		else if(cross_over_criteria == 2)
		{
			while(loop_control_variable_1>0)
			{

				//				cout<<"1"<<endl;
				for(int i = 0 ; i <312 ;i++)
				{
					arr1[i] = 0;
				}
				k=0;
				for(int i=0;i<size_of_population;i++)
				{
					k = 0;
					for(int j=0;j<312;j++)
					{
						arr1[k++] = current_population[i][j];
					}
					array_containing_fitness_of_chromosomes[i] = (1/calculate_fitness(arr1));
				}

				//cout<<"2"<<endl;


				//Stochastic selection
				total_fitness = 0;

				for(int i = 0 ;i <size_of_population;i++)
				{
					total_fitness  = total_fitness + array_containing_fitness_of_chromosomes[i];
				}
				array_containing_probabilities_of_chromosomes = new float [size_of_population]();

				for(int i = 0 ; i < size_of_population ; i++)
				{
					array_containing_probabilities_of_chromosomes[i]  = ((array_containing_fitness_of_chromosomes[i] / total_fitness)*100);
				}

				//specifying the ranges in the probability ranges in the array
				for (int i = 1;i < size_of_population ; i++)
				{
					array_containing_probabilities_of_chromosomes[i] = array_containing_probabilities_of_chromosomes[i] + array_containing_probabilities_of_chromosomes[i-1];
				}

				array_containing_probabilities_of_chromosomes_1 = new float [size_of_population+1]();
				array_containing_probabilities_of_chromosomes_1[0] = 0;
				z = 0;
				for(int i = 1;i<size_of_population+1;i++)
				{
					array_containing_probabilities_of_chromosomes_1[i] = array_containing_probabilities_of_chromosomes[z++];
				}

				check_1 = true;
				loop_control_variable = size_of_population/2; 

				index_i_of_two_new_chromosome = 0;
				index_j_of_two_new_chromosome = 0;
				start = 0;
				end = 2;

				//	cout<<"3"<<endl;

				//in this loop i am creating the new population
				//based on the current population
				//using one point crossover
				while(loop_control_variable > 0)
				{
					//			cout<<"4"<<endl;

					random_number_1 = random(1,100);
					random_number_2 = random(1,100);
					for(int i = size_of_population+1;i>0;i--)
					{
						if((random_number_1 <= array_containing_probabilities_of_chromosomes_1[i]) && (random_number_1 >= array_containing_probabilities_of_chromosomes_1[i-1]) )
						{
							for(int j = 0 ; j<312;j++)
							{
								chromosome_1_crossover[j] = current_population[i-1][j];
							}
							break;
						}

					}

					for(int i = size_of_population+1;i>0;i--)
					{
						if((random_number_2 <= array_containing_probabilities_of_chromosomes_1[i]) && (random_number_2 >= array_containing_probabilities_of_chromosomes_1[i-1]) )
						{
							for(int j = 0 ; j<312;j++)
							{
								chromosome_2_crossover[j] = current_population[i-1][j];
							}
							break;
						}

					}



					two_new_chromosomes = two_point_crossover(chromosome_1_crossover,chromosome_2_crossover);

					for(int i = start;i<end;i++)
					{
						for(int  j = 0 ;j < 312; j ++)
						{
							new_population[i][j] = two_new_chromosomes[index_i_of_two_new_chromosome][index_j_of_two_new_chromosome++];
						}
						index_i_of_two_new_chromosome++;
						index_j_of_two_new_chromosome = 0;
					}

					start = start + 2;
					end = end+2;
					index_i_of_two_new_chromosome = 0;
					index_j_of_two_new_chromosome = 0;

					loop_control_variable--;
				}

				//			cout<<"5"<<endl;
				doing_mutation(new_population,mutation_rate,size_of_population);
				array_containing_fitness_of_new_chromosomes = new float[size_of_population]();


				for(int  i = 0 ; i <312 ; i++)
				{
					arry1[i] = 0;

				}
				z=0;
				k=0;
				for(int i=0;i<size_of_population;i++)
				{
					z = 0;
					k=0;
					for(int j=0;j<312;j++)
					{
						arry1[k++] = new_population[i][j];
					}
					array_containing_fitness_of_new_chromosomes[i] = (1/calculate_fitness(arry1));
				}

				//				cout<<"6"<<endl;

				//combining the 2 populations
				for(int i = 0 ; i <size_of_population;i++)
				{
					for(int j = 0; j<312 ; j++)
					{
						array_containing_both_new_and_old_population[i][j] = current_population[i][j];
					}
				}

				u = 0;
				for(int i = size_of_population ; i <(2*size_of_population);i++)
				{
					for(int j = 0;j<312 ;j++)
					{
						array_containing_both_new_and_old_population[i][j] = new_population[u][j];
					}
					u++;
				}


				//combining the fitnesses of the 2 populations
				array_containing_new_and_old_fitnesses = new float[2*size_of_population]();
				for(int  i = 0 ;i <size_of_population ; i++)
				{
					array_containing_new_and_old_fitnesses[i] = array_containing_fitness_of_chromosomes[i];
				}
				l = 0;
				for(int i = size_of_population;i<(2*size_of_population);i++)
				{
					array_containing_new_and_old_fitnesses[i]  = array_containing_fitness_of_new_chromosomes[l++];
				}

				//sorting the array which contains the fitness of old and new chromosomes
				//and also the corresponding chromosomes in array containing new and old 
				//chromosomes
				for(int  i = 0 ; i <(2*size_of_population)-1;i++ )
				{
					for(int j = 0;j<(2*size_of_population)-1-i;j++)
					{
						if(array_containing_new_and_old_fitnesses[j]<array_containing_new_and_old_fitnesses[j+1])
						{
							swap(array_containing_new_and_old_fitnesses[j],array_containing_new_and_old_fitnesses[j+1]);
							for(int k = 0 ; k < 312 ;k ++)
							{
								temp_for_storing_chromosome[k] = array_containing_both_new_and_old_population[j][k];
								array_containing_both_new_and_old_population[j][k] = array_containing_both_new_and_old_population[j+1][k];
								array_containing_both_new_and_old_population[j+1][k] = array_containing_both_new_and_old_population[j][k];

							}

						}
					}

				}

				int smallest_index;
				bool flag;
				if((best_fitness_required - array_containing_new_and_old_fitnesses[0])>0)
				{
					smallest = best_fitness_required - array_containing_new_and_old_fitnesses[0];
					smallest_index = 0;
					flag = true;
				}

				//updating the best chromosome so far 
				for(int  i = 1 ;i < (2*size_of_population);i++)
				{
					int temp = best_fitness_required - array_containing_new_and_old_fitnesses[i];
					if(temp > 0) 
					{
						if(temp<smallest)
						{
							smallest = temp;
							smallest_index = i;
							flag = true;


						}
					}
				}

				for(int  i = 0 ; i<312 ; i++)
				{
					best_chromosome_so_far[i] = array_containing_both_new_and_old_population[smallest_index][i];
					result_chromosome_1_index = smallest_index; 
				}





				//updating the population for next iteration

				for(int  i = 0 ; i <size_of_population ; i++)
				{
					for(int j = 0;j <312 ; j ++)
					{
						current_population[i][j]= array_containing_both_new_and_old_population[i][j];
					}
				}



				for(int i = 0 ; i <size_of_population;i++)
				{
					if(array_containing_fitness_of_new_chromosomes[i] >= best_fitness_required)
					{

						for(int  j = 0 ;j < 312 ; j ++)
						{
							result_choromosome[j] = new_population[i][j];
						}
						result_chromosome_2_index = i;
						break_flag = true;
						break;
					}
				}

				if(break_flag == true)
				{
					cout<<"Congratulations Required Fitness Chromosome found!"<<endl;
					for(int i = 0;i<312;i++)
					{
						cout<<result_choromosome[i]<<"|";
					}
					cout<<endl;

					cout<<"Fitness: "<<array_containing_fitness_of_new_chromosomes[result_chromosome_2_index]<<endl;
					cout<<"Iterations: "<<endl;
					cout<<(iterations - loop_control_variable_1)<<endl;
					cout<<"The Pictorial representation of the chromosome is "<<endl;

					break;
				}

				if(break_flag == false)
				{

					for(int i = 0 ; i <size_of_population;i++)
					{
						if(array_containing_fitness_of_chromosomes[i] >= best_fitness_required)
						{

							for(int  j = 0 ;j < 312 ; j ++)
							{
								result_choromosome_1[j] = current_population[i][j];
							}
							result_chromosome_3_index = i;
							break_flag_1 = true;
							break;
						}
					}

					if(break_flag_1 == true)
					{
						cout<<"Congratulations Required Fitness Chromosome found!"<<endl;
						for(int i = 0;i<312;i++)
						{
							cout<<result_choromosome_1[i]<<"|";
						}
						cout<<endl;

						cout<<"Fitness: "<<array_containing_fitness_of_chromosomes[result_chromosome_3_index]<<endl;
						cout<<"Iterations "<<endl;
						cout<<(iterations-loop_control_variable_1)<<endl;
						cout<<"The Pictorial representation of the chromosome is "<<endl;

						break;
					}

				}








				loop_control_variable_1--;

				if(loop_control_variable_1 == 0)
				{
					cout<<"Iterations ended"<<endl;
					cout<<"Best chromosome so far is "<<endl;
					for(int i = 0 ; i <312 ;i++)
					{
						cout<<best_chromosome_so_far[i]<<"|";
					}
					cout<<endl;

					cout<<"Fitness :"<<array_containing_new_and_old_fitnesses[result_chromosome_1_index]<<endl;

					cout<<endl;
					cout<<"The Pictorial Representation of the chromosome is !"<<endl;
					break;
				}
			}



		}
		else if(cross_over_criteria == 3)
		{
			while(loop_control_variable_1>0)
			{

				//				cout<<"1"<<endl;
				for(int i = 0 ; i <312 ;i++)
				{
					arr1[i] = 0;
				}
				k=0;
				for(int i=0;i<size_of_population;i++)
				{
					k = 0;
					for(int j=0;j<312;j++)
					{
						arr1[k++] = current_population[i][j];
					}
					array_containing_fitness_of_chromosomes[i] = (1/calculate_fitness(arr1));
				}

				//cout<<"2"<<endl;


				//Stochastic selection
				total_fitness = 0;

				for(int i = 0 ;i <size_of_population;i++)
				{
					total_fitness  = total_fitness + array_containing_fitness_of_chromosomes[i];
				}
				array_containing_probabilities_of_chromosomes = new float [size_of_population]();

				for(int i = 0 ; i < size_of_population ; i++)
				{
					array_containing_probabilities_of_chromosomes[i]  = ((array_containing_fitness_of_chromosomes[i] / total_fitness)*100);
				}

				//specifying the ranges in the probability ranges in the array
				for (int i = 1;i < size_of_population ; i++)
				{
					array_containing_probabilities_of_chromosomes[i] = array_containing_probabilities_of_chromosomes[i] + array_containing_probabilities_of_chromosomes[i-1];
				}

				array_containing_probabilities_of_chromosomes_1 = new float [size_of_population+1]();
				array_containing_probabilities_of_chromosomes_1[0] = 0;
				z = 0;
				for(int i = 1;i<size_of_population+1;i++)
				{
					array_containing_probabilities_of_chromosomes_1[i] = array_containing_probabilities_of_chromosomes[z++];
				}

				check_1 = true;
				loop_control_variable = size_of_population/2; 

				index_i_of_two_new_chromosome = 0;
				index_j_of_two_new_chromosome = 0;
				start = 0;
				end = 2;

				//	cout<<"3"<<endl;

				//in this loop i am creating the new population
				//based on the current population
				//using one point crossover
				while(loop_control_variable > 0)
				{
					//			cout<<"4"<<endl;

					random_number_1 = random(1,100);
					random_number_2 = random(1,100);
					for(int i = size_of_population+1;i>0;i--)
					{
						if((random_number_1 <= array_containing_probabilities_of_chromosomes_1[i]) && (random_number_1 >= array_containing_probabilities_of_chromosomes_1[i-1]) )
						{
							for(int j = 0 ; j<312;j++)
							{
								chromosome_1_crossover[j] = current_population[i-1][j];
							}
							break;
						}

					}

					for(int i = size_of_population+1;i>0;i--)
					{
						if((random_number_2 <= array_containing_probabilities_of_chromosomes_1[i]) && (random_number_2 >= array_containing_probabilities_of_chromosomes_1[i-1]) )
						{
							for(int j = 0 ; j<312;j++)
							{
								chromosome_2_crossover[j] = current_population[i-1][j];
							}
							break;
						}

					}



					two_new_chromosomes = uniform_point_crossover(chromosome_1_crossover,chromosome_2_crossover);

					for(int i = start;i<end;i++)
					{
						for(int  j = 0 ;j < 312; j ++)
						{
							new_population[i][j] = two_new_chromosomes[index_i_of_two_new_chromosome][index_j_of_two_new_chromosome++];
						}
						index_i_of_two_new_chromosome++;
						index_j_of_two_new_chromosome = 0;
					}

					start = start + 2;
					end = end+2;
					index_i_of_two_new_chromosome = 0;
					index_j_of_two_new_chromosome = 0;

					loop_control_variable--;
				}

				//			cout<<"5"<<endl;
				doing_mutation(new_population,mutation_rate,size_of_population);
				array_containing_fitness_of_new_chromosomes = new float[size_of_population]();


				for(int  i = 0 ; i <312 ; i++)
				{
					arry1[i] = 0;

				}
				z=0;
				k=0;
				for(int i=0;i<size_of_population;i++)
				{
					z = 0;
					k=0;
					for(int j=0;j<312;j++)
					{
						arry1[k++] = new_population[i][j];
					}
					array_containing_fitness_of_new_chromosomes[i] = (1/calculate_fitness(arry1));
				}

				//				cout<<"6"<<endl;

				//combining the 2 populations
				for(int i = 0 ; i <size_of_population;i++)
				{
					for(int j = 0; j<312 ; j++)
					{
						array_containing_both_new_and_old_population[i][j] = current_population[i][j];
					}
				}

				u = 0;
				for(int i = size_of_population ; i <(2*size_of_population);i++)
				{
					for(int j = 0;j<312 ;j++)
					{
						array_containing_both_new_and_old_population[i][j] = new_population[u][j];
					}
					u++;
				}


				//combining the fitnesses of the 2 populations
				array_containing_new_and_old_fitnesses = new float[2*size_of_population]();
				for(int  i = 0 ;i <size_of_population ; i++)
				{
					array_containing_new_and_old_fitnesses[i] = array_containing_fitness_of_chromosomes[i];
				}
				l = 0;
				for(int i = size_of_population;i<(2*size_of_population);i++)
				{
					array_containing_new_and_old_fitnesses[i]  = array_containing_fitness_of_new_chromosomes[l++];
				}

				//sorting the array which contains the fitness of old and new chromosomes
				//and also the corresponding chromosomes in array containing new and old 
				//chromosomes
				for(int  i = 0 ; i <(2*size_of_population)-1;i++ )
				{
					for(int j = 0;j<(2*size_of_population)-1-i;j++)
					{
						if(array_containing_new_and_old_fitnesses[j]<array_containing_new_and_old_fitnesses[j+1])
						{
							swap(array_containing_new_and_old_fitnesses[j],array_containing_new_and_old_fitnesses[j+1]);
							for(int k = 0 ; k < 312 ;k ++)
							{
								temp_for_storing_chromosome[k] = array_containing_both_new_and_old_population[j][k];
								array_containing_both_new_and_old_population[j][k] = array_containing_both_new_and_old_population[j+1][k];
								array_containing_both_new_and_old_population[j+1][k] = array_containing_both_new_and_old_population[j][k];

							}

						}
					}

				}

				int smallest_index;
				bool flag;
				if((best_fitness_required - array_containing_new_and_old_fitnesses[0])>0)
				{
					smallest = best_fitness_required - array_containing_new_and_old_fitnesses[0];
					smallest_index = 0;
					flag = true;
				}

				//updating the best chromosome so far 
				for(int  i = 1 ;i < (2*size_of_population);i++)
				{
					int temp = best_fitness_required - array_containing_new_and_old_fitnesses[i];
					if(temp > 0) 
					{
						if(temp<smallest)
						{
							smallest = temp;
							smallest_index = i;
							flag = true;


						}
					}
				}

				for(int  i = 0 ; i<312 ; i++)
				{
					best_chromosome_so_far[i] = array_containing_both_new_and_old_population[smallest_index][i];
					result_chromosome_1_index = smallest_index; 
				}





				//updating the population for next iteration

				for(int  i = 0 ; i <size_of_population ; i++)
				{
					for(int j = 0;j <312 ; j ++)
					{
						current_population[i][j]= array_containing_both_new_and_old_population[i][j];
					}
				}



				for(int i = 0 ; i <size_of_population;i++)
				{
					if(array_containing_fitness_of_new_chromosomes[i] >= best_fitness_required)
					{

						for(int  j = 0 ;j < 312 ; j ++)
						{
							result_choromosome[j] = new_population[i][j];
						}
						result_chromosome_2_index = i;
						break_flag = true;
						break;
					}
				}

				if(break_flag == true)
				{
					cout<<"Congratulations Required Fitness Chromosome found!"<<endl;
					for(int i = 0;i<312;i++)
					{
						cout<<result_choromosome[i]<<"|";
					}
					cout<<endl;

					cout<<"Fitness: "<<array_containing_fitness_of_new_chromosomes[result_chromosome_2_index]<<endl;
					cout<<"Iterations: "<<endl;
					cout<<(iterations - loop_control_variable_1)<<endl;
					cout<<"The Pictorial representation of the chromosome is "<<endl;

					break;
				}

				if(break_flag == false)
				{

					for(int i = 0 ; i <size_of_population;i++)
					{
						if(array_containing_fitness_of_chromosomes[i] >= best_fitness_required)
						{

							for(int  j = 0 ;j < 312 ; j ++)
							{
								result_choromosome_1[j] = current_population[i][j];
							}
							result_chromosome_3_index = i;
							break_flag_1 = true;
							break;
						}
					}

					if(break_flag_1 == true)
					{
						cout<<"Congratulations Required Fitness Chromosome found!"<<endl;
						for(int i = 0;i<312;i++)
						{
							cout<<result_choromosome_1[i]<<"|";
						}
						cout<<endl;

						cout<<"Fitness: "<<array_containing_fitness_of_chromosomes[result_chromosome_3_index]<<endl;
						cout<<"Iterations "<<endl;
						cout<<(iterations-loop_control_variable_1)<<endl;
						cout<<"The Pictorial representation of the chromosome is "<<endl;

						break;
					}

				}








				loop_control_variable_1--;

				if(loop_control_variable_1 == 0)
				{
					cout<<"Iterations ended"<<endl;
					cout<<"Best chromosome so far is "<<endl;
					for(int i = 0 ; i <312 ;i++)
					{
						cout<<best_chromosome_so_far[i]<<"|";
					}
					cout<<endl;

					cout<<"Fitness :"<<array_containing_new_and_old_fitnesses[result_chromosome_1_index]<<endl;

					cout<<endl;
					cout<<"The Pictorial Representation of the chromosome is !"<<endl;
					break;
				}
			}




		}
		else if(cross_over_criteria == 4)
		{
			while(loop_control_variable_1>0)
			{

				//				cout<<"1"<<endl;
				for(int i = 0 ; i <312 ;i++)
				{
					arr1[i] = 0;
				}
				k=0;
				for(int i=0;i<size_of_population;i++)
				{
					k = 0;
					for(int j=0;j<312;j++)
					{
						arr1[k++] = current_population[i][j];
					}
					array_containing_fitness_of_chromosomes[i] = (1/calculate_fitness(arr1));
				}

				//cout<<"2"<<endl;


				//Stochastic selection
				total_fitness = 0;

				for(int i = 0 ;i <size_of_population;i++)
				{
					total_fitness  = total_fitness + array_containing_fitness_of_chromosomes[i];
				}
				array_containing_probabilities_of_chromosomes = new float [size_of_population]();

				for(int i = 0 ; i < size_of_population ; i++)
				{
					array_containing_probabilities_of_chromosomes[i]  = ((array_containing_fitness_of_chromosomes[i] / total_fitness)*100);
				}

				//specifying the ranges in the probability ranges in the array
				for (int i = 1;i < size_of_population ; i++)
				{
					array_containing_probabilities_of_chromosomes[i] = array_containing_probabilities_of_chromosomes[i] + array_containing_probabilities_of_chromosomes[i-1];
				}

				array_containing_probabilities_of_chromosomes_1 = new float [size_of_population+1]();
				array_containing_probabilities_of_chromosomes_1[0] = 0;
				z = 0;
				for(int i = 1;i<size_of_population+1;i++)
				{
					array_containing_probabilities_of_chromosomes_1[i] = array_containing_probabilities_of_chromosomes[z++];
				}

				check_1 = true;
				loop_control_variable = size_of_population/2; 

				index_i_of_two_new_chromosome = 0;
				index_j_of_two_new_chromosome = 0;
				start = 0;
				end = 2;

				//	cout<<"3"<<endl;

				//in this loop i am creating the new population
				//based on the current population
				//using one point crossover
				while(loop_control_variable > 0)
				{
					//			cout<<"4"<<endl;

					random_number_1 = random(1,100);
					random_number_2 = random(1,100);
					for(int i = size_of_population+1;i>0;i--)
					{
						if((random_number_1 <= array_containing_probabilities_of_chromosomes_1[i]) && (random_number_1 >= array_containing_probabilities_of_chromosomes_1[i-1]) )
						{
							for(int j = 0 ; j<312;j++)
							{
								chromosome_1_crossover[j] = current_population[i-1][j];
							}
							break;
						}

					}

					for(int i = size_of_population+1;i>0;i--)
					{
						if((random_number_2 <= array_containing_probabilities_of_chromosomes_1[i]) && (random_number_2 >= array_containing_probabilities_of_chromosomes_1[i-1]) )
						{
							for(int j = 0 ; j<312;j++)
							{
								chromosome_2_crossover[j] = current_population[i-1][j];
							}
							break;
						}

					}



					two_new_chromosomes = first_order_crossover(chromosome_1_crossover,chromosome_2_crossover);

					for(int i = start;i<end;i++)
					{
						for(int  j = 0 ;j < 312; j ++)
						{
							new_population[i][j] = two_new_chromosomes[index_i_of_two_new_chromosome][index_j_of_two_new_chromosome++];
						}
						index_i_of_two_new_chromosome++;
						index_j_of_two_new_chromosome = 0;
					}

					start = start + 2;
					end = end+2;
					index_i_of_two_new_chromosome = 0;
					index_j_of_two_new_chromosome = 0;

					loop_control_variable--;
				}

				//			cout<<"5"<<endl;
				doing_mutation(new_population,mutation_rate,size_of_population);
				array_containing_fitness_of_new_chromosomes = new float[size_of_population]();


				for(int  i = 0 ; i <312 ; i++)
				{
					arry1[i] = 0;

				}
				z=0;
				k=0;
				for(int i=0;i<size_of_population;i++)
				{
					z = 0;
					k=0;
					for(int j=0;j<312;j++)
					{
						arry1[k++] = new_population[i][j];
					}
					array_containing_fitness_of_new_chromosomes[i] = (1/calculate_fitness(arry1));
				}

				//				cout<<"6"<<endl;

				//combining the 2 populations
				for(int i = 0 ; i <size_of_population;i++)
				{
					for(int j = 0; j<312 ; j++)
					{
						array_containing_both_new_and_old_population[i][j] = current_population[i][j];
					}
				}

				u = 0;
				for(int i = size_of_population ; i <(2*size_of_population);i++)
				{
					for(int j = 0;j<312 ;j++)
					{
						array_containing_both_new_and_old_population[i][j] = new_population[u][j];
					}
					u++;
				}


				//combining the fitnesses of the 2 populations
				array_containing_new_and_old_fitnesses = new float[2*size_of_population]();
				for(int  i = 0 ;i <size_of_population ; i++)
				{
					array_containing_new_and_old_fitnesses[i] = array_containing_fitness_of_chromosomes[i];
				}
				l = 0;
				for(int i = size_of_population;i<(2*size_of_population);i++)
				{
					array_containing_new_and_old_fitnesses[i]  = array_containing_fitness_of_new_chromosomes[l++];
				}

				//sorting the array which contains the fitness of old and new chromosomes
				//and also the corresponding chromosomes in array containing new and old 
				//chromosomes
				for(int  i = 0 ; i <(2*size_of_population)-1;i++ )
				{
					for(int j = 0;j<(2*size_of_population)-1-i;j++)
					{
						if(array_containing_new_and_old_fitnesses[j]<array_containing_new_and_old_fitnesses[j+1])
						{
							swap(array_containing_new_and_old_fitnesses[j],array_containing_new_and_old_fitnesses[j+1]);
							for(int k = 0 ; k < 312 ;k ++)
							{
								temp_for_storing_chromosome[k] = array_containing_both_new_and_old_population[j][k];
								array_containing_both_new_and_old_population[j][k] = array_containing_both_new_and_old_population[j+1][k];
								array_containing_both_new_and_old_population[j+1][k] = array_containing_both_new_and_old_population[j][k];

							}

						}
					}

				}

				int smallest_index;
				bool flag;
				if((best_fitness_required - array_containing_new_and_old_fitnesses[0])>0)
				{
					smallest = best_fitness_required - array_containing_new_and_old_fitnesses[0];
					smallest_index = 0;
					flag = true;
				}

				//updating the best chromosome so far 
				for(int  i = 1 ;i < (2*size_of_population);i++)
				{
					int temp = best_fitness_required - array_containing_new_and_old_fitnesses[i];
					if(temp > 0) 
					{
						if(temp<smallest)
						{
							smallest = temp;
							smallest_index = i;
							flag = true;


						}
					}
				}

				for(int  i = 0 ; i<312 ; i++)
				{
					best_chromosome_so_far[i] = array_containing_both_new_and_old_population[smallest_index][i];
					result_chromosome_1_index = smallest_index; 
				}





				//updating the population for next iteration

				for(int  i = 0 ; i <size_of_population ; i++)
				{
					for(int j = 0;j <312 ; j ++)
					{
						current_population[i][j]= array_containing_both_new_and_old_population[i][j];
					}
				}



				for(int i = 0 ; i <size_of_population;i++)
				{
					if(array_containing_fitness_of_new_chromosomes[i] >= best_fitness_required)
					{

						for(int  j = 0 ;j < 312 ; j ++)
						{
							result_choromosome[j] = new_population[i][j];
						}
						result_chromosome_2_index = i;
						break_flag = true;
						break;
					}
				}

				if(break_flag == true)
				{
					cout<<"Congratulations Required Fitness Chromosome found!"<<endl;
					for(int i = 0;i<312;i++)
					{
						cout<<result_choromosome[i]<<"|";
					}
					cout<<endl;

					cout<<"Fitness: "<<array_containing_fitness_of_new_chromosomes[result_chromosome_2_index]<<endl;
					cout<<"Iterations: "<<endl;
					cout<<(iterations - loop_control_variable_1)<<endl;
					cout<<"The Pictorial representation of the chromosome is "<<endl;

					break;
				}

				if(break_flag == false)
				{

					for(int i = 0 ; i <size_of_population;i++)
					{
						if(array_containing_fitness_of_chromosomes[i] >= best_fitness_required)
						{

							for(int  j = 0 ;j < 312 ; j ++)
							{
								result_choromosome_1[j] = current_population[i][j];
							}
							result_chromosome_3_index = i;
							break_flag_1 = true;
							break;
						}
					}

					if(break_flag_1 == true)
					{
						cout<<"Congratulations Required Fitness Chromosome found!"<<endl;
						for(int i = 0;i<312;i++)
						{
							cout<<result_choromosome_1[i]<<"|";
						}
						cout<<endl;

						cout<<"Fitness: "<<array_containing_fitness_of_chromosomes[result_chromosome_3_index]<<endl;
						cout<<"Iterations "<<endl;
						cout<<(iterations-loop_control_variable_1)<<endl;
						cout<<"The Pictorial representation of the chromosome is "<<endl;

						break;
					}

				}








				loop_control_variable_1--;

				if(loop_control_variable_1 == 0)
				{
					cout<<"Iterations ended"<<endl;
					cout<<"Best chromosome so far is "<<endl;
					for(int i = 0 ; i <312 ;i++)
					{
						cout<<best_chromosome_so_far[i]<<"|";
					}
					cout<<endl;

					cout<<"Fitness :"<<array_containing_new_and_old_fitnesses[result_chromosome_1_index]<<endl;

					cout<<endl;
					cout<<"The Pictorial Representation of the chromosome is !"<<endl;
					break;
				}
			}
		}
	}
	else if(selection_criteria ==  2)
	{

		int cross_over_criteria1;
		cout<<"Select CrossOver Criteria!"<<endl;
		cout<<"1:One Point Crossover!"<<endl;
		cout<<"2:Two Point Crossover!"<<endl;
		cout<<"3:Uniform Crossover!"<<endl;
		cout<<"4:First order Crossover!"<<endl;
		cin>>cross_over_criteria1;


		////////////////////////////


		//for storing two new choromosomes after
		//cross over

		two_new_chromosomes = new int* [2];
		for(int i = 0 ;i < 2 ;i ++)
		{
			two_new_chromosomes[i] = new int [312];
		}

		/////////////////////////////////////

		break_flag = false;




		int iterations;
		cout<<"How many iterations you want ?"<<endl;
		cin>>iterations;

		float best_fitness_required;
		cout<<"Find the max fitness you want to achieve ?"<<endl;
		cin>>best_fitness_required;


		int mutation_rate;
		cout<<"Enter Mutation Rate"<<endl;
		cin>>mutation_rate;


		int loop_control_variable_1;
		loop_control_variable_1 = iterations;


		float random_number_1;
		float random_number_2;
		random_number_1 = 0;
		random_number_2 = 0;


		int chromosome_1_index;
		int chromosome_2_index;
		chromosome_1_index = 0;
		chromosome_2_index = 0;


		int chromosome_1_crossover[312] = {0};
		int chromosome_2_crossover[312] = {0};

		int start;
		int end;


		int index_i_of_two_new_chromosome ;
		int index_j_of_two_new_chromosome;


		int arr1[312] = {0};

		int k;
		k=0;
		int m;
		int n;
		m = 0;
		n = 0;

		float total_fitness = 0;
		int z;
		bool check_1;
		int loop_control_variable; 
		float * array_containing_fitness_of_chromosomes = new float [size_of_population]();
		float * array_containing_fitness_of_new_chromosomes;
		int arry1[312] = {0};
		float * array_containing_new_and_old_fitnesses ;
		int l;
		int indexing1;
		int firstflag;
		int * result_choromosome ;
		int * result_choromosome_1;
		int * best_chromosome_so_far;
		bool break_flag_1;
		int smallest;
		int best_fitness_so_far;
		best_fitness_so_far = 0;

		result_choromosome = new int[312]();
		result_choromosome_1 = new int[312]();
		best_chromosome_so_far = new int [312]();

		int u;
		int v;
		int minimum_distance;
		minimum_distance =0;
		int * temp_for_storing_chromosome = new int[312]();










		////////////////////////////
		int array1 [312] = {0};
		int o;
		float * array_containing_fitness_of_current_chromosomes = new float[size_of_population]();
		float totalfitness;
		float * array_containing_probabilities_of_current_chromosomes;

		int rank_variable;
		float * array_containing_rank;
		float total_rank;
		if(cross_over_criteria1 == 1)
		{

			while( loop_control_variable_1 >0)
			{

				for(int i = 0 ; i <312 ;i++)
				{
					array1[i] = 0;
				}
				o=0;
				for(int i=0;i<size_of_population;i++)
				{
					o = 0;
					for(int j=0;j<312;j++)
					{
						array1[o++] = current_population[i][j];
					}
					array_containing_fitness_of_current_chromosomes[i] = (1/calculate_fitness(array1));
				}



				//Stochastic selection
				totalfitness = 0;

				for(int i = 0 ;i <size_of_population;i++)
				{
					totalfitness  = totalfitness + array_containing_fitness_of_current_chromosomes[i];
				}
				array_containing_probabilities_of_current_chromosomes = new float [size_of_population]();

				for(int i = 0 ; i < size_of_population ; i++)
				{
					array_containing_probabilities_of_current_chromosomes[i]  = ((array_containing_fitness_of_current_chromosomes[i] / totalfitness)*100);
				}

				//specifying the commulative sum in the probability ranges in the array
				for (int i = 1;i < size_of_population ; i++)
				{
					array_containing_probabilities_of_current_chromosomes[i] = array_containing_probabilities_of_current_chromosomes[i] + array_containing_probabilities_of_current_chromosomes[i-1];
				}

				//Rank Selection begins here
				for(int  i = 0 ; i <size_of_population - 1 ;i++)
				{
					for(int j = 0 ; j < size_of_population-1-i; j++)
					{
						if(array_containing_fitness_of_current_chromosomes[j]<array_containing_probabilities_of_current_chromosomes[j+1])
						{
							swap(array_containing_probabilities_of_current_chromosomes[j],array_containing_probabilities_of_current_chromosomes[j+1]);
						}
					}
				}



				rank_variable = 1;
				array_containing_rank = new float[size_of_population]();
				for(int i = size_of_population-1; i>0;i--)
				{
					array_containing_rank[i] = rank_variable;
					if(array_containing_probabilities_of_current_chromosomes[i] != array_containing_probabilities_of_current_chromosomes[i-1])
					{
						rank_variable++;
					}

				}
				array_containing_rank[0] = rank_variable;

				total_rank = 0;
				for(int  i = 0 ;i <size_of_population ;i++)
				{
					total_rank = total_rank + array_containing_rank[i];
				}


				array_containing_probabilities_of_chromosomes = new float [size_of_population]();

				for(int i = 0 ; i < size_of_population ; i++)
				{
					array_containing_probabilities_of_chromosomes[i]  = ((array_containing_rank[i] / total_rank)*100);
				}


				//specifying the ranges in the probability ranges in the array
				for (int i = 1;i < size_of_population ; i++)
				{
					array_containing_probabilities_of_chromosomes[i] = array_containing_probabilities_of_chromosomes[i] + array_containing_probabilities_of_chromosomes[i-1];
				}

				array_containing_probabilities_of_chromosomes_1 = new float [size_of_population+1]();
				array_containing_probabilities_of_chromosomes_1[0] = 0;
				z = 0;
				for(int i = 1;i<size_of_population+1;i++)
				{
					array_containing_probabilities_of_chromosomes_1[i] = array_containing_probabilities_of_chromosomes[z++];
				}

				check_1 = true;
				loop_control_variable = size_of_population/2; 

				index_i_of_two_new_chromosome = 0;
				index_j_of_two_new_chromosome = 0;
				start = 0;
				end = 2;


				//in this loop i am creating the new population
				//based on the current population
				//using one point crossover
				while(loop_control_variable > 0)
				{

					random_number_1 = random(1,100);
					random_number_2 = random(1,100);
					for(int i = size_of_population+1;i>0;i--)
					{
						if((random_number_1 <= array_containing_probabilities_of_chromosomes_1[i]) && (random_number_1 >= array_containing_probabilities_of_chromosomes_1[i-1]) )
						{
							for(int j = 0 ; j<312;j++)
							{
								chromosome_1_crossover[j] = current_population[i-1][j];
							}
							break;
						}

					}

					for(int i = size_of_population+1;i>0;i--)
					{
						if((random_number_2 <= array_containing_probabilities_of_chromosomes_1[i]) && (random_number_2 >= array_containing_probabilities_of_chromosomes_1[i-1]) )
						{
							for(int j = 0 ; j<312;j++)
							{
								chromosome_2_crossover[j] = current_population[i-1][j];
							}
							break;
						}

					}



					two_new_chromosomes = single_point_crossover(chromosome_1_crossover,chromosome_2_crossover);

					for(int i = start;i<end;i++)
					{
						for(int  j = 0 ;j < 312; j ++)
						{
							new_population[i][j] = two_new_chromosomes[index_i_of_two_new_chromosome][index_j_of_two_new_chromosome++];
						}
						index_i_of_two_new_chromosome++;
						index_j_of_two_new_chromosome = 0;
					}

					start = start + 2;
					end = end+2;
					index_i_of_two_new_chromosome = 0;
					index_j_of_two_new_chromosome = 0;

					loop_control_variable--;
				}

				//			cout<<"5"<<endl;
				doing_mutation(new_population,mutation_rate,size_of_population);
				array_containing_fitness_of_new_chromosomes = new float[size_of_population]();


				for(int  i = 0 ; i <312 ; i++)
				{
					arry1[i] = 0;

				}
				z=0;
				k=0;
				for(int i=0;i<size_of_population;i++)
				{
					z = 0;
					k=0;
					for(int j=0;j<312;j++)
					{
						arry1[k++] = new_population[i][j];
					}
					array_containing_fitness_of_new_chromosomes[i] = (1/calculate_fitness(arry1));
				}

				//				cout<<"6"<<endl;

				//combining the 2 populations
				for(int i = 0 ; i <size_of_population;i++)
				{
					for(int j = 0; j<312 ; j++)
					{
						array_containing_both_new_and_old_population[i][j] = current_population[i][j];
					}
				}

				u = 0;
				for(int i = size_of_population ; i <(2*size_of_population);i++)
				{
					for(int j = 0;j<312 ;j++)
					{
						array_containing_both_new_and_old_population[i][j] = new_population[u][j];
					}
					u++;
				}


				//combining the fitnesses of the 2 populations
				array_containing_new_and_old_fitnesses = new float[2*size_of_population]();
				for(int  i = 0 ;i <size_of_population ; i++)
				{
					array_containing_new_and_old_fitnesses[i] = array_containing_fitness_of_chromosomes[i];
				}
				l = 0;
				for(int i = size_of_population;i<(2*size_of_population);i++)
				{
					array_containing_new_and_old_fitnesses[i]  = array_containing_fitness_of_new_chromosomes[l++];
				}

				//sorting the array which contains the fitness of old and new chromosomes
				//and also the corresponding chromosomes in array containing new and old 
				//chromosomes
				for(int  i = 0 ; i <(2*size_of_population)-1;i++ )
				{
					for(int j = 0;j<(2*size_of_population)-1-i;j++)
					{
						if(array_containing_new_and_old_fitnesses[j]<array_containing_new_and_old_fitnesses[j+1])
						{
							swap(array_containing_new_and_old_fitnesses[j],array_containing_new_and_old_fitnesses[j+1]);
							for(int k = 0 ; k < 312 ;k ++)
							{
								temp_for_storing_chromosome[k] = array_containing_both_new_and_old_population[j][k];
								array_containing_both_new_and_old_population[j][k] = array_containing_both_new_and_old_population[j+1][k];
								array_containing_both_new_and_old_population[j+1][k] = array_containing_both_new_and_old_population[j][k];

							}

						}
					}

				}

				int smallest_index;
				bool flag;
				if((best_fitness_required - array_containing_new_and_old_fitnesses[0])>0)
				{
					smallest = best_fitness_required - array_containing_new_and_old_fitnesses[0];
					smallest_index = 0;
					flag = true;
				}

				//updating the best chromosome so far 
				for(int  i = 1 ;i < (2*size_of_population);i++)
				{
					int temp = best_fitness_required - array_containing_new_and_old_fitnesses[i];
					if(temp > 0) 
					{
						if(temp<smallest)
						{
							smallest = temp;
							smallest_index = i;
							flag = true;


						}
					}
				}

				for(int  i = 0 ; i<312 ; i++)
				{
					best_chromosome_so_far[i] = array_containing_both_new_and_old_population[smallest_index][i];
					result_chromosome_1_index = smallest_index; 
				}





				//updating the population for next iteration

				for(int  i = 0 ; i <size_of_population ; i++)
				{
					for(int j = 0;j <312 ; j ++)
					{
						current_population[i][j]= array_containing_both_new_and_old_population[i][j];
					}
				}



				for(int i = 0 ; i <size_of_population;i++)
				{
					if(array_containing_fitness_of_new_chromosomes[i] >= best_fitness_required)
					{

						for(int  j = 0 ;j < 312 ; j ++)
						{
							result_choromosome[j] = new_population[i][j];
						}
						result_chromosome_2_index = i;
						break_flag = true;
						break;
					}
				}

				if(break_flag == true)
				{
					cout<<"Congratulations Required Fitness Chromosome found!"<<endl;
					for(int i = 0;i<312;i++)
					{
						cout<<result_choromosome[i]<<"|";
					}
					cout<<endl;

					cout<<"Fitness: "<<array_containing_fitness_of_new_chromosomes[result_chromosome_2_index]<<endl;
					cout<<"Iterations: "<<endl;
					cout<<(iterations - loop_control_variable_1)<<endl;
					cout<<"The Pictorial representation of the chromosome is "<<endl;

					break;
				}

				if(break_flag == false)
				{

					for(int i = 0 ; i <size_of_population;i++)
					{
						if(array_containing_fitness_of_chromosomes[i] >= best_fitness_required)
						{

							for(int  j = 0 ;j < 312 ; j ++)
							{
								result_choromosome_1[j] = current_population[i][j];
							}
							result_chromosome_3_index = i;
							break_flag_1 = true;
							break;
						}
					}

					if(break_flag_1 == true)
					{
						cout<<"Congratulations Required Fitness Chromosome found!"<<endl;
						for(int i = 0;i<312;i++)
						{
							cout<<result_choromosome_1[i]<<"|";
						}
						cout<<endl;

						cout<<"Fitness: "<<array_containing_fitness_of_chromosomes[result_chromosome_3_index]<<endl;
						cout<<"Iterations "<<endl;
						cout<<(iterations-loop_control_variable_1)<<endl;
						cout<<"The Pictorial representation of the chromosome is "<<endl;

						break;
					}

				}








				//now i ll assign rank to each probability
				loop_control_variable_1 --;

				if(loop_control_variable_1 == 0)
				{
					cout<<"Iterations ended"<<endl;
					cout<<"Best chromosome so far is "<<endl;
					for(int i = 0 ; i <312 ;i++)
					{
						cout<<best_chromosome_so_far[i]<<"|";
					}
					cout<<endl;

					cout<<"Fitness :"<<array_containing_new_and_old_fitnesses[result_chromosome_1_index]<<endl;

					cout<<endl;
					cout<<"The Pictorial Representation of the chromosome is !"<<endl;
					break;
				}























			}



		}
		else if(cross_over_criteria1 == 2)
		{
			while( loop_control_variable_1 >0)
			{

				for(int i = 0 ; i <312 ;i++)
				{
					array1[i] = 0;
				}
				o=0;
				for(int i=0;i<size_of_population;i++)
				{
					o = 0;
					for(int j=0;j<312;j++)
					{
						array1[o++] = current_population[i][j];
					}
					array_containing_fitness_of_current_chromosomes[i] = (1/calculate_fitness(array1));
				}



				//Stochastic selection
				totalfitness = 0;

				for(int i = 0 ;i <size_of_population;i++)
				{
					totalfitness  = totalfitness + array_containing_fitness_of_current_chromosomes[i];
				}
				array_containing_probabilities_of_current_chromosomes = new float [size_of_population]();

				for(int i = 0 ; i < size_of_population ; i++)
				{
					array_containing_probabilities_of_current_chromosomes[i]  = ((array_containing_fitness_of_current_chromosomes[i] / totalfitness)*100);
				}

				//specifying the commulative sum in the probability ranges in the array
				for (int i = 1;i < size_of_population ; i++)
				{
					array_containing_probabilities_of_current_chromosomes[i] = array_containing_probabilities_of_current_chromosomes[i] + array_containing_probabilities_of_current_chromosomes[i-1];
				}

				//Rank Selection begins here
				for(int  i = 0 ; i <size_of_population - 1 ;i++)
				{
					for(int j = 0 ; j < size_of_population-1-i; j++)
					{
						if(array_containing_fitness_of_current_chromosomes[j]<array_containing_probabilities_of_current_chromosomes[j+1])
						{
							swap(array_containing_probabilities_of_current_chromosomes[j],array_containing_probabilities_of_current_chromosomes[j+1]);
						}
					}
				}



				rank_variable = 1;
				array_containing_rank = new float[size_of_population]();
				for(int i = size_of_population-1; i>0;i--)
				{
					array_containing_rank[i] = rank_variable;
					if(array_containing_probabilities_of_current_chromosomes[i] != array_containing_probabilities_of_current_chromosomes[i-1])
					{
						rank_variable++;
					}

				}
				array_containing_rank[0] = rank_variable;

				total_rank = 0;
				for(int  i = 0 ;i <size_of_population ;i++)
				{
					total_rank = total_rank + array_containing_rank[i];
				}


				array_containing_probabilities_of_chromosomes = new float [size_of_population]();

				for(int i = 0 ; i < size_of_population ; i++)
				{
					array_containing_probabilities_of_chromosomes[i]  = ((array_containing_rank[i] / total_rank)*100);
				}


				//specifying the ranges in the probability ranges in the array
				for (int i = 1;i < size_of_population ; i++)
				{
					array_containing_probabilities_of_chromosomes[i] = array_containing_probabilities_of_chromosomes[i] + array_containing_probabilities_of_chromosomes[i-1];
				}

				array_containing_probabilities_of_chromosomes_1 = new float [size_of_population+1]();
				array_containing_probabilities_of_chromosomes_1[0] = 0;
				z = 0;
				for(int i = 1;i<size_of_population+1;i++)
				{
					array_containing_probabilities_of_chromosomes_1[i] = array_containing_probabilities_of_chromosomes[z++];
				}

				check_1 = true;
				loop_control_variable = size_of_population/2; 

				index_i_of_two_new_chromosome = 0;
				index_j_of_two_new_chromosome = 0;
				start = 0;
				end = 2;


				//in this loop i am creating the new population
				//based on the current population
				//using one point crossover
				while(loop_control_variable > 0)
				{

					random_number_1 = random(1,100);
					random_number_2 = random(1,100);
					for(int i = size_of_population+1;i>0;i--)
					{
						if((random_number_1 <= array_containing_probabilities_of_chromosomes_1[i]) && (random_number_1 >= array_containing_probabilities_of_chromosomes_1[i-1]) )
						{
							for(int j = 0 ; j<312;j++)
							{
								chromosome_1_crossover[j] = current_population[i-1][j];
							}
							break;
						}

					}

					for(int i = size_of_population+1;i>0;i--)
					{
						if((random_number_2 <= array_containing_probabilities_of_chromosomes_1[i]) && (random_number_2 >= array_containing_probabilities_of_chromosomes_1[i-1]) )
						{
							for(int j = 0 ; j<312;j++)
							{
								chromosome_2_crossover[j] = current_population[i-1][j];
							}
							break;
						}

					}



					two_new_chromosomes = two_point_crossover(chromosome_1_crossover,chromosome_2_crossover);

					for(int i = start;i<end;i++)
					{
						for(int  j = 0 ;j < 312; j ++)
						{
							new_population[i][j] = two_new_chromosomes[index_i_of_two_new_chromosome][index_j_of_two_new_chromosome++];
						}
						index_i_of_two_new_chromosome++;
						index_j_of_two_new_chromosome = 0;
					}

					start = start + 2;
					end = end+2;
					index_i_of_two_new_chromosome = 0;
					index_j_of_two_new_chromosome = 0;

					loop_control_variable--;
				}

				//			cout<<"5"<<endl;
				doing_mutation(new_population,mutation_rate,size_of_population);
				array_containing_fitness_of_new_chromosomes = new float[size_of_population]();


				for(int  i = 0 ; i <312 ; i++)
				{
					arry1[i] = 0;

				}
				z=0;
				k=0;
				for(int i=0;i<size_of_population;i++)
				{
					z = 0;
					k=0;
					for(int j=0;j<312;j++)
					{
						arry1[k++] = new_population[i][j];
					}
					array_containing_fitness_of_new_chromosomes[i] = (1/calculate_fitness(arry1));
				}

				//				cout<<"6"<<endl;

				//combining the 2 populations
				for(int i = 0 ; i <size_of_population;i++)
				{
					for(int j = 0; j<312 ; j++)
					{
						array_containing_both_new_and_old_population[i][j] = current_population[i][j];
					}
				}

				u = 0;
				for(int i = size_of_population ; i <(2*size_of_population);i++)
				{
					for(int j = 0;j<312 ;j++)
					{
						array_containing_both_new_and_old_population[i][j] = new_population[u][j];
					}
					u++;
				}


				//combining the fitnesses of the 2 populations
				array_containing_new_and_old_fitnesses = new float[2*size_of_population]();
				for(int  i = 0 ;i <size_of_population ; i++)
				{
					array_containing_new_and_old_fitnesses[i] = array_containing_fitness_of_chromosomes[i];
				}
				l = 0;
				for(int i = size_of_population;i<(2*size_of_population);i++)
				{
					array_containing_new_and_old_fitnesses[i]  = array_containing_fitness_of_new_chromosomes[l++];
				}

				//sorting the array which contains the fitness of old and new chromosomes
				//and also the corresponding chromosomes in array containing new and old 
				//chromosomes
				for(int  i = 0 ; i <(2*size_of_population)-1;i++ )
				{
					for(int j = 0;j<(2*size_of_population)-1-i;j++)
					{
						if(array_containing_new_and_old_fitnesses[j]<array_containing_new_and_old_fitnesses[j+1])
						{
							swap(array_containing_new_and_old_fitnesses[j],array_containing_new_and_old_fitnesses[j+1]);
							for(int k = 0 ; k < 312 ;k ++)
							{
								temp_for_storing_chromosome[k] = array_containing_both_new_and_old_population[j][k];
								array_containing_both_new_and_old_population[j][k] = array_containing_both_new_and_old_population[j+1][k];
								array_containing_both_new_and_old_population[j+1][k] = array_containing_both_new_and_old_population[j][k];

							}

						}
					}

				}

				int smallest_index;
				bool flag;
				if((best_fitness_required - array_containing_new_and_old_fitnesses[0])>0)
				{
					smallest = best_fitness_required - array_containing_new_and_old_fitnesses[0];
					smallest_index = 0;
					flag = true;
				}

				//updating the best chromosome so far 
				for(int  i = 1 ;i < (2*size_of_population);i++)
				{
					int temp = best_fitness_required - array_containing_new_and_old_fitnesses[i];
					if(temp > 0) 
					{
						if(temp<smallest)
						{
							smallest = temp;
							smallest_index = i;
							flag = true;


						}
					}
				}

				for(int  i = 0 ; i<312 ; i++)
				{
					best_chromosome_so_far[i] = array_containing_both_new_and_old_population[smallest_index][i];
					result_chromosome_1_index = smallest_index; 
				}





				//updating the population for next iteration

				for(int  i = 0 ; i <size_of_population ; i++)
				{
					for(int j = 0;j <312 ; j ++)
					{
						current_population[i][j]= array_containing_both_new_and_old_population[i][j];
					}
				}



				for(int i = 0 ; i <size_of_population;i++)
				{
					if(array_containing_fitness_of_new_chromosomes[i] >= best_fitness_required)
					{

						for(int  j = 0 ;j < 312 ; j ++)
						{
							result_choromosome[j] = new_population[i][j];
						}
						result_chromosome_2_index = i;
						break_flag = true;
						break;
					}
				}

				if(break_flag == true)
				{
					cout<<"Congratulations Required Fitness Chromosome found!"<<endl;
					for(int i = 0;i<312;i++)
					{
						cout<<result_choromosome[i]<<"|";
					}
					cout<<endl;

					cout<<"Fitness: "<<array_containing_fitness_of_new_chromosomes[result_chromosome_2_index]<<endl;
					cout<<"Iterations: "<<endl;
					cout<<(iterations - loop_control_variable_1)<<endl;
					cout<<"The Pictorial representation of the chromosome is "<<endl;

					break;
				}

				if(break_flag == false)
				{

					for(int i = 0 ; i <size_of_population;i++)
					{
						if(array_containing_fitness_of_chromosomes[i] >= best_fitness_required)
						{

							for(int  j = 0 ;j < 312 ; j ++)
							{
								result_choromosome_1[j] = current_population[i][j];
							}
							result_chromosome_3_index = i;
							break_flag_1 = true;
							break;
						}
					}

					if(break_flag_1 == true)
					{
						cout<<"Congratulations Required Fitness Chromosome found!"<<endl;
						for(int i = 0;i<312;i++)
						{
							cout<<result_choromosome_1[i]<<"|";
						}
						cout<<endl;

						cout<<"Fitness: "<<array_containing_fitness_of_chromosomes[result_chromosome_3_index]<<endl;
						cout<<"Iterations "<<endl;
						cout<<(iterations-loop_control_variable_1)<<endl;
						cout<<"The Pictorial representation of the chromosome is "<<endl;

						break;
					}

				}








				//now i ll assign rank to each probability
				loop_control_variable_1 --;

				if(loop_control_variable_1 == 0)
				{
					cout<<"Iterations ended"<<endl;
					cout<<"Best chromosome so far is "<<endl;
					for(int i = 0 ; i <312 ;i++)
					{
						cout<<best_chromosome_so_far[i]<<"|";
					}
					cout<<endl;

					cout<<"Fitness :"<<array_containing_new_and_old_fitnesses[result_chromosome_1_index]<<endl;

					cout<<endl;
					cout<<"The Pictorial Representation of the chromosome is !"<<endl;
					break;
				}























			}





		}
		else if(cross_over_criteria1 ==3)
		{
			while( loop_control_variable_1 >0)
			{

				for(int i = 0 ; i <312 ;i++)
				{
					array1[i] = 0;
				}
				o=0;
				for(int i=0;i<size_of_population;i++)
				{
					o = 0;
					for(int j=0;j<312;j++)
					{
						array1[o++] = current_population[i][j];
					}
					array_containing_fitness_of_current_chromosomes[i] = (1/calculate_fitness(array1));
				}



				//Stochastic selection
				totalfitness = 0;

				for(int i = 0 ;i <size_of_population;i++)
				{
					totalfitness  = totalfitness + array_containing_fitness_of_current_chromosomes[i];
				}
				array_containing_probabilities_of_current_chromosomes = new float [size_of_population]();

				for(int i = 0 ; i < size_of_population ; i++)
				{
					array_containing_probabilities_of_current_chromosomes[i]  = ((array_containing_fitness_of_current_chromosomes[i] / totalfitness)*100);
				}

				//specifying the commulative sum in the probability ranges in the array
				for (int i = 1;i < size_of_population ; i++)
				{
					array_containing_probabilities_of_current_chromosomes[i] = array_containing_probabilities_of_current_chromosomes[i] + array_containing_probabilities_of_current_chromosomes[i-1];
				}

				//Rank Selection begins here
				for(int  i = 0 ; i <size_of_population - 1 ;i++)
				{
					for(int j = 0 ; j < size_of_population-1-i; j++)
					{
						if(array_containing_fitness_of_current_chromosomes[j]<array_containing_probabilities_of_current_chromosomes[j+1])
						{
							swap(array_containing_probabilities_of_current_chromosomes[j],array_containing_probabilities_of_current_chromosomes[j+1]);
						}
					}
				}



				rank_variable = 1;
				array_containing_rank = new float[size_of_population]();
				for(int i = size_of_population-1; i>0;i--)
				{
					array_containing_rank[i] = rank_variable;
					if(array_containing_probabilities_of_current_chromosomes[i] != array_containing_probabilities_of_current_chromosomes[i-1])
					{
						rank_variable++;
					}

				}
				array_containing_rank[0] = rank_variable;

				total_rank = 0;
				for(int  i = 0 ;i <size_of_population ;i++)
				{
					total_rank = total_rank + array_containing_rank[i];
				}


				array_containing_probabilities_of_chromosomes = new float [size_of_population]();

				for(int i = 0 ; i < size_of_population ; i++)
				{
					array_containing_probabilities_of_chromosomes[i]  = ((array_containing_rank[i] / total_rank)*100);
				}


				//specifying the ranges in the probability ranges in the array
				for (int i = 1;i < size_of_population ; i++)
				{
					array_containing_probabilities_of_chromosomes[i] = array_containing_probabilities_of_chromosomes[i] + array_containing_probabilities_of_chromosomes[i-1];
				}

				array_containing_probabilities_of_chromosomes_1 = new float [size_of_population+1]();
				array_containing_probabilities_of_chromosomes_1[0] = 0;
				z = 0;
				for(int i = 1;i<size_of_population+1;i++)
				{
					array_containing_probabilities_of_chromosomes_1[i] = array_containing_probabilities_of_chromosomes[z++];
				}

				check_1 = true;
				loop_control_variable = size_of_population/2; 

				index_i_of_two_new_chromosome = 0;
				index_j_of_two_new_chromosome = 0;
				start = 0;
				end = 2;


				//in this loop i am creating the new population
				//based on the current population
				//using one point crossover
				while(loop_control_variable > 0)
				{

					random_number_1 = random(1,100);
					random_number_2 = random(1,100);
					for(int i = size_of_population+1;i>0;i--)
					{
						if((random_number_1 <= array_containing_probabilities_of_chromosomes_1[i]) && (random_number_1 >= array_containing_probabilities_of_chromosomes_1[i-1]) )
						{
							for(int j = 0 ; j<312;j++)
							{
								chromosome_1_crossover[j] = current_population[i-1][j];
							}
							break;
						}

					}

					for(int i = size_of_population+1;i>0;i--)
					{
						if((random_number_2 <= array_containing_probabilities_of_chromosomes_1[i]) && (random_number_2 >= array_containing_probabilities_of_chromosomes_1[i-1]) )
						{
							for(int j = 0 ; j<312;j++)
							{
								chromosome_2_crossover[j] = current_population[i-1][j];
							}
							break;
						}

					}



					two_new_chromosomes = uniform_point_crossover(chromosome_1_crossover,chromosome_2_crossover);

					for(int i = start;i<end;i++)
					{
						for(int  j = 0 ;j < 312; j ++)
						{
							new_population[i][j] = two_new_chromosomes[index_i_of_two_new_chromosome][index_j_of_two_new_chromosome++];
						}
						index_i_of_two_new_chromosome++;
						index_j_of_two_new_chromosome = 0;
					}

					start = start + 2;
					end = end+2;
					index_i_of_two_new_chromosome = 0;
					index_j_of_two_new_chromosome = 0;

					loop_control_variable--;
				}

				//			cout<<"5"<<endl;
				doing_mutation(new_population,mutation_rate,size_of_population);
				array_containing_fitness_of_new_chromosomes = new float[size_of_population]();


				for(int  i = 0 ; i <312 ; i++)
				{
					arry1[i] = 0;

				}
				z=0;
				k=0;
				for(int i=0;i<size_of_population;i++)
				{
					z = 0;
					k=0;
					for(int j=0;j<312;j++)
					{
						arry1[k++] = new_population[i][j];
					}
					array_containing_fitness_of_new_chromosomes[i] = (1/calculate_fitness(arry1));
				}

				//				cout<<"6"<<endl;

				//combining the 2 populations
				for(int i = 0 ; i <size_of_population;i++)
				{
					for(int j = 0; j<312 ; j++)
					{
						array_containing_both_new_and_old_population[i][j] = current_population[i][j];
					}
				}

				u = 0;
				for(int i = size_of_population ; i <(2*size_of_population);i++)
				{
					for(int j = 0;j<312 ;j++)
					{
						array_containing_both_new_and_old_population[i][j] = new_population[u][j];
					}
					u++;
				}


				//combining the fitnesses of the 2 populations
				array_containing_new_and_old_fitnesses = new float[2*size_of_population]();
				for(int  i = 0 ;i <size_of_population ; i++)
				{
					array_containing_new_and_old_fitnesses[i] = array_containing_fitness_of_chromosomes[i];
				}
				l = 0;
				for(int i = size_of_population;i<(2*size_of_population);i++)
				{
					array_containing_new_and_old_fitnesses[i]  = array_containing_fitness_of_new_chromosomes[l++];
				}

				//sorting the array which contains the fitness of old and new chromosomes
				//and also the corresponding chromosomes in array containing new and old 
				//chromosomes
				for(int  i = 0 ; i <(2*size_of_population)-1;i++ )
				{
					for(int j = 0;j<(2*size_of_population)-1-i;j++)
					{
						if(array_containing_new_and_old_fitnesses[j]<array_containing_new_and_old_fitnesses[j+1])
						{
							swap(array_containing_new_and_old_fitnesses[j],array_containing_new_and_old_fitnesses[j+1]);
							for(int k = 0 ; k < 312 ;k ++)
							{
								temp_for_storing_chromosome[k] = array_containing_both_new_and_old_population[j][k];
								array_containing_both_new_and_old_population[j][k] = array_containing_both_new_and_old_population[j+1][k];
								array_containing_both_new_and_old_population[j+1][k] = array_containing_both_new_and_old_population[j][k];

							}

						}
					}

				}

				int smallest_index;
				bool flag;
				if((best_fitness_required - array_containing_new_and_old_fitnesses[0])>0)
				{
					smallest = best_fitness_required - array_containing_new_and_old_fitnesses[0];
					smallest_index = 0;
					flag = true;
				}

				//updating the best chromosome so far 
				for(int  i = 1 ;i < (2*size_of_population);i++)
				{
					int temp = best_fitness_required - array_containing_new_and_old_fitnesses[i];
					if(temp > 0) 
					{
						if(temp<smallest)
						{
							smallest = temp;
							smallest_index = i;
							flag = true;


						}
					}
				}

				for(int  i = 0 ; i<312 ; i++)
				{
					best_chromosome_so_far[i] = array_containing_both_new_and_old_population[smallest_index][i];
					result_chromosome_1_index = smallest_index; 
				}





				//updating the population for next iteration

				for(int  i = 0 ; i <size_of_population ; i++)
				{
					for(int j = 0;j <312 ; j ++)
					{
						current_population[i][j]= array_containing_both_new_and_old_population[i][j];
					}
				}



				for(int i = 0 ; i <size_of_population;i++)
				{
					if(array_containing_fitness_of_new_chromosomes[i] >= best_fitness_required)
					{

						for(int  j = 0 ;j < 312 ; j ++)
						{
							result_choromosome[j] = new_population[i][j];
						}
						result_chromosome_2_index = i;
						break_flag = true;
						break;
					}
				}

				if(break_flag == true)
				{
					cout<<"Congratulations Required Fitness Chromosome found!"<<endl;
					for(int i = 0;i<312;i++)
					{
						cout<<result_choromosome[i]<<"|";
					}
					cout<<endl;

					cout<<"Fitness: "<<array_containing_fitness_of_new_chromosomes[result_chromosome_2_index]<<endl;
					cout<<"Iterations: "<<endl;
					cout<<(iterations - loop_control_variable_1)<<endl;
					cout<<"The Pictorial representation of the chromosome is "<<endl;

					break;
				}

				if(break_flag == false)
				{

					for(int i = 0 ; i <size_of_population;i++)
					{
						if(array_containing_fitness_of_chromosomes[i] >= best_fitness_required)
						{

							for(int  j = 0 ;j < 312 ; j ++)
							{
								result_choromosome_1[j] = current_population[i][j];
							}
							result_chromosome_3_index = i;
							break_flag_1 = true;
							break;
						}
					}

					if(break_flag_1 == true)
					{
						cout<<"Congratulations Required Fitness Chromosome found!"<<endl;
						for(int i = 0;i<312;i++)
						{
							cout<<result_choromosome_1[i]<<"|";
						}
						cout<<endl;

						cout<<"Fitness: "<<array_containing_fitness_of_chromosomes[result_chromosome_3_index]<<endl;
						cout<<"Iterations "<<endl;
						cout<<(iterations-loop_control_variable_1)<<endl;
						cout<<"The Pictorial representation of the chromosome is "<<endl;

						break;
					}

				}








				//now i ll assign rank to each probability
				loop_control_variable_1 --;

				if(loop_control_variable_1 == 0)
				{
					cout<<"Iterations ended"<<endl;
					cout<<"Best chromosome so far is "<<endl;
					for(int i = 0 ; i <312 ;i++)
					{
						cout<<best_chromosome_so_far[i]<<"|";
					}
					cout<<endl;

					cout<<"Fitness :"<<array_containing_new_and_old_fitnesses[result_chromosome_1_index]<<endl;

					cout<<endl;
					cout<<"The Pictorial Representation of the chromosome is !"<<endl;
					break;
				}























			}


		}
		else if(cross_over_criteria1 == 4)
		{
			while( loop_control_variable_1 >0)
			{

				for(int i = 0 ; i <312 ;i++)
				{
					array1[i] = 0;
				}
				o=0;
				for(int i=0;i<size_of_population;i++)
				{
					o = 0;
					for(int j=0;j<312;j++)
					{
						array1[o++] = current_population[i][j];
					}
					array_containing_fitness_of_current_chromosomes[i] = (1/calculate_fitness(array1));
				}



				//Stochastic selection
				totalfitness = 0;

				for(int i = 0 ;i <size_of_population;i++)
				{
					totalfitness  = totalfitness + array_containing_fitness_of_current_chromosomes[i];
				}
				array_containing_probabilities_of_current_chromosomes = new float [size_of_population]();

				for(int i = 0 ; i < size_of_population ; i++)
				{
					array_containing_probabilities_of_current_chromosomes[i]  = ((array_containing_fitness_of_current_chromosomes[i] / totalfitness)*100);
				}

				//specifying the commulative sum in the probability ranges in the array
				for (int i = 1;i < size_of_population ; i++)
				{
					array_containing_probabilities_of_current_chromosomes[i] = array_containing_probabilities_of_current_chromosomes[i] + array_containing_probabilities_of_current_chromosomes[i-1];
				}

				//Rank Selection begins here
				for(int  i = 0 ; i <size_of_population - 1 ;i++)
				{
					for(int j = 0 ; j < size_of_population-1-i; j++)
					{
						if(array_containing_fitness_of_current_chromosomes[j]<array_containing_probabilities_of_current_chromosomes[j+1])
						{
							swap(array_containing_probabilities_of_current_chromosomes[j],array_containing_probabilities_of_current_chromosomes[j+1]);
						}
					}
				}



				rank_variable = 1;
				array_containing_rank = new float[size_of_population]();
				for(int i = size_of_population-1; i>0;i--)
				{
					array_containing_rank[i] = rank_variable;
					if(array_containing_probabilities_of_current_chromosomes[i] != array_containing_probabilities_of_current_chromosomes[i-1])
					{
						rank_variable++;
					}

				}
				array_containing_rank[0] = rank_variable;

				total_rank = 0;
				for(int  i = 0 ;i <size_of_population ;i++)
				{
					total_rank = total_rank + array_containing_rank[i];
				}


				array_containing_probabilities_of_chromosomes = new float [size_of_population]();

				for(int i = 0 ; i < size_of_population ; i++)
				{
					array_containing_probabilities_of_chromosomes[i]  = ((array_containing_rank[i] / total_rank)*100);
				}


				//specifying the ranges in the probability ranges in the array
				for (int i = 1;i < size_of_population ; i++)
				{
					array_containing_probabilities_of_chromosomes[i] = array_containing_probabilities_of_chromosomes[i] + array_containing_probabilities_of_chromosomes[i-1];
				}

				array_containing_probabilities_of_chromosomes_1 = new float [size_of_population+1]();
				array_containing_probabilities_of_chromosomes_1[0] = 0;
				z = 0;
				for(int i = 1;i<size_of_population+1;i++)
				{
					array_containing_probabilities_of_chromosomes_1[i] = array_containing_probabilities_of_chromosomes[z++];
				}

				check_1 = true;
				loop_control_variable = size_of_population/2; 

				index_i_of_two_new_chromosome = 0;
				index_j_of_two_new_chromosome = 0;
				start = 0;
				end = 2;


				//in this loop i am creating the new population
				//based on the current population
				//using one point crossover
				while(loop_control_variable > 0)
				{

					random_number_1 = random(1,100);
					random_number_2 = random(1,100);
					for(int i = size_of_population+1;i>0;i--)
					{
						if((random_number_1 <= array_containing_probabilities_of_chromosomes_1[i]) && (random_number_1 >= array_containing_probabilities_of_chromosomes_1[i-1]) )
						{
							for(int j = 0 ; j<312;j++)
							{
								chromosome_1_crossover[j] = current_population[i-1][j];
							}
							break;
						}

					}

					for(int i = size_of_population+1;i>0;i--)
					{
						if((random_number_2 <= array_containing_probabilities_of_chromosomes_1[i]) && (random_number_2 >= array_containing_probabilities_of_chromosomes_1[i-1]) )
						{
							for(int j = 0 ; j<312;j++)
							{
								chromosome_2_crossover[j] = current_population[i-1][j];
							}
							break;
						}

					}



					two_new_chromosomes = first_order_crossover(chromosome_1_crossover,chromosome_2_crossover);

					for(int i = start;i<end;i++)
					{
						for(int  j = 0 ;j < 312; j ++)
						{
							new_population[i][j] = two_new_chromosomes[index_i_of_two_new_chromosome][index_j_of_two_new_chromosome++];
						}
						index_i_of_two_new_chromosome++;
						index_j_of_two_new_chromosome = 0;
					}

					start = start + 2;
					end = end+2;
					index_i_of_two_new_chromosome = 0;
					index_j_of_two_new_chromosome = 0;

					loop_control_variable--;
				}

				//			cout<<"5"<<endl;
				doing_mutation(new_population,mutation_rate,size_of_population);
				array_containing_fitness_of_new_chromosomes = new float[size_of_population]();


				for(int  i = 0 ; i <312 ; i++)
				{
					arry1[i] = 0;

				}
				z=0;
				k=0;
				for(int i=0;i<size_of_population;i++)
				{
					z = 0;
					k=0;
					for(int j=0;j<312;j++)
					{
						arry1[k++] = new_population[i][j];
					}
					array_containing_fitness_of_new_chromosomes[i] = (1/calculate_fitness(arry1));
				}

				//				cout<<"6"<<endl;

				//combining the 2 populations
				for(int i = 0 ; i <size_of_population;i++)
				{
					for(int j = 0; j<312 ; j++)
					{
						array_containing_both_new_and_old_population[i][j] = current_population[i][j];
					}
				}

				u = 0;
				for(int i = size_of_population ; i <(2*size_of_population);i++)
				{
					for(int j = 0;j<312 ;j++)
					{
						array_containing_both_new_and_old_population[i][j] = new_population[u][j];
					}
					u++;
				}


				//combining the fitnesses of the 2 populations
				array_containing_new_and_old_fitnesses = new float[2*size_of_population]();
				for(int  i = 0 ;i <size_of_population ; i++)
				{
					array_containing_new_and_old_fitnesses[i] = array_containing_fitness_of_chromosomes[i];
				}
				l = 0;
				for(int i = size_of_population;i<(2*size_of_population);i++)
				{
					array_containing_new_and_old_fitnesses[i]  = array_containing_fitness_of_new_chromosomes[l++];
				}

				//sorting the array which contains the fitness of old and new chromosomes
				//and also the corresponding chromosomes in array containing new and old 
				//chromosomes
				for(int  i = 0 ; i <(2*size_of_population)-1;i++ )
				{
					for(int j = 0;j<(2*size_of_population)-1-i;j++)
					{
						if(array_containing_new_and_old_fitnesses[j]<array_containing_new_and_old_fitnesses[j+1])
						{
							swap(array_containing_new_and_old_fitnesses[j],array_containing_new_and_old_fitnesses[j+1]);
							for(int k = 0 ; k < 312 ;k ++)
							{
								temp_for_storing_chromosome[k] = array_containing_both_new_and_old_population[j][k];
								array_containing_both_new_and_old_population[j][k] = array_containing_both_new_and_old_population[j+1][k];
								array_containing_both_new_and_old_population[j+1][k] = array_containing_both_new_and_old_population[j][k];

							}

						}
					}

				}

				int smallest_index;
				bool flag;
				if((best_fitness_required - array_containing_new_and_old_fitnesses[0])>0)
				{
					smallest = best_fitness_required - array_containing_new_and_old_fitnesses[0];
					smallest_index = 0;
					flag = true;
				}

				//updating the best chromosome so far 
				for(int  i = 1 ;i < (2*size_of_population);i++)
				{
					int temp = best_fitness_required - array_containing_new_and_old_fitnesses[i];
					if(temp > 0) 
					{
						if(temp<smallest)
						{
							smallest = temp;
							smallest_index = i;
							flag = true;


						}
					}
				}

				for(int  i = 0 ; i<312 ; i++)
				{
					best_chromosome_so_far[i] = array_containing_both_new_and_old_population[smallest_index][i];
					result_chromosome_1_index = smallest_index; 
				}





				//updating the population for next iteration

				for(int  i = 0 ; i <size_of_population ; i++)
				{
					for(int j = 0;j <312 ; j ++)
					{
						current_population[i][j]= array_containing_both_new_and_old_population[i][j];
					}
				}



				for(int i = 0 ; i <size_of_population;i++)
				{
					if(array_containing_fitness_of_new_chromosomes[i] >= best_fitness_required)
					{

						for(int  j = 0 ;j < 312 ; j ++)
						{
							result_choromosome[j] = new_population[i][j];
						}
						result_chromosome_2_index = i;
						break_flag = true;
						break;
					}
				}

				if(break_flag == true)
				{
					cout<<"Congratulations Required Fitness Chromosome found!"<<endl;
					for(int i = 0;i<312;i++)
					{
						cout<<result_choromosome[i]<<"|";
					}
					cout<<endl;

					cout<<"Fitness: "<<array_containing_fitness_of_new_chromosomes[result_chromosome_2_index]<<endl;
					cout<<"Iterations: "<<endl;
					cout<<(iterations - loop_control_variable_1)<<endl;
					cout<<"The Pictorial representation of the chromosome is "<<endl;

					break;
				}

				if(break_flag == false)
				{

					for(int i = 0 ; i <size_of_population;i++)
					{
						if(array_containing_fitness_of_chromosomes[i] >= best_fitness_required)
						{

							for(int  j = 0 ;j < 312 ; j ++)
							{
								result_choromosome_1[j] = current_population[i][j];
							}
							result_chromosome_3_index = i;
							break_flag_1 = true;
							break;
						}
					}

					if(break_flag_1 == true)
					{
						cout<<"Congratulations Required Fitness Chromosome found!"<<endl;
						for(int i = 0;i<312;i++)
						{
							cout<<result_choromosome_1[i]<<"|";
						}
						cout<<endl;

						cout<<"Fitness: "<<array_containing_fitness_of_chromosomes[result_chromosome_3_index]<<endl;
						cout<<"Iterations "<<endl;
						cout<<(iterations-loop_control_variable_1)<<endl;
						cout<<"The Pictorial representation of the chromosome is "<<endl;

						break;
					}

				}








				//now i ll assign rank to each probability
				loop_control_variable_1 --;

				if(loop_control_variable_1 == 0)
				{
					cout<<"Iterations ended"<<endl;
					cout<<"Best chromosome so far is "<<endl;
					for(int i = 0 ; i <312 ;i++)
					{
						cout<<best_chromosome_so_far[i]<<"|";
					}
					cout<<endl;

					cout<<"Fitness :"<<array_containing_new_and_old_fitnesses[result_chromosome_1_index]<<endl;

					cout<<endl;
					cout<<"The Pictorial Representation of the chromosome is !"<<endl;
					break;
				}




			}
		}

	}
	else if(selection_criteria == 3)
	{

		//for storing two new choromosomes after
		//cross over

		two_new_chromosomes = new int* [2];
		for(int i = 0 ;i < 2 ;i ++)
		{
			two_new_chromosomes[i] = new int [312];
		}

		/////////////////////////////////////

		break_flag = false;




		int iterations;
		cout<<"How many iterations you want ?"<<endl;
		cin>>iterations;
		int cross_over_criteria;
		cout<<"Select CrossOver Criteria!"<<endl;
		cout<<"1:One Point Crossover!"<<endl;
		cout<<"2:Two Point Crossover!"<<endl;
		cout<<"3:Uniform Crossover!"<<endl;
		cout<<"4:Fist Order Crossover!"<<endl;
		cin>>cross_over_criteria;


		float best_fitness_required;
		cout<<"Find the max fitness you want to achieve ?"<<endl;
		cin>>best_fitness_required;

		int loop_control_variable_1;
		loop_control_variable_1 = iterations;







		int mutation_rate;
		cout<<"Enter Mutation Rate"<<endl;
		cin>>mutation_rate;


		int tournament_size;
		cout<<"Enter Tournament Size ?"<<endl;
		cin>>tournament_size;
		tournament_size = check_population(tournament_size);

		int random_number_1;
		int random_number_2;
		random_number_1 = 0;
		random_number_2 = 0;


		int chromosome_1_index;
		int chromosome_2_index;
		chromosome_1_index = 0;
		chromosome_2_index = 0;


		int chromosome_1_crossover[312] = {0};
		int chromosome_2_crossover[312] = {0};

		int start;
		int end;


		int index_i_of_two_new_chromosome ;
		int index_j_of_two_new_chromosome;


		int arr1[312] = {0};

		int k;
		k=0;
		int m;
		int n;
		m = 0;
		n = 0;

		float total_fitness = 0;
		int z;
		bool check_1;
		int loop_control_variable; 
		float * array_containing_fitness_of_chromosomes = new float [size_of_population]();
		float * array_containing_fitness_of_new_chromosomes;
		int arry1[312] = {0};
		float * array_containing_new_and_old_fitnesses ;
		int l;
		int indexing1;
		int firstflag;
		int * result_choromosome ;
		int * result_choromosome_1;
		int * best_chromosome_so_far;
		bool break_flag_1;
		int smallest;
		int best_fitness_so_far;
		best_fitness_so_far = 0;

		result_choromosome = new int[312]();
		result_choromosome_1 = new int[312]();
		best_chromosome_so_far = new int [312]();
		int tournament_selection_loop_control_variable;
		tournament_selection_loop_control_variable = size_of_population;
		int u;
		int v;
		int minimum_distance;
		minimum_distance =0;
		int * temp_for_storing_chromosome = new int[312]();

		temp_for_current_population = new int*[size_of_population]();
		for(int i = 0;i<size_of_population;i++)
		{
			temp_for_current_population[i] = new int [312]();
		}

		int ** temp_for_tournament_population;
		temp_for_tournament_population = new int *[tournament_size]();
		for(int i = 0;i<tournament_size;i++)
		{
			temp_for_tournament_population[i] = new int [312]();
		}
		float * array_containing_fitness_of_tournament_selection_population;
		array_containing_fitness_of_tournament_selection_population = new float[tournament_size]();

		int temp_random;
		int index_i;
		int temp_r[312];
		int current_index_in_temp_for_current_population;
		current_index_in_temp_for_current_population = 0;

		/*
		for(int  i  = 0;i<size_of_population;i++)
		{
		for(int  j = 0;j<312;j++)
		{
		cout<<current_population[i][j]<<"|";
		}
		cout<<endl;
		}
		*/
		if(cross_over_criteria == 1)
		{
			cout<<"1"<<endl;
			while(loop_control_variable_1>0)
			{


				check_1 = true;
				loop_control_variable = size_of_population/2; 

				index_i_of_two_new_chromosome = 0;
				index_j_of_two_new_chromosome = 0;
				start = 0;
				end = 2;

				//	cout<<"3"<<endl;

				//in this loop i am creating the new population
				//based on the current population
				//using one point crossover

				index_i = 0;
				temp_random = 0;
				//cout<<"3"<<endl;

				while(tournament_selection_loop_control_variable > 0)
				{
					//cout<<"4"<<endl;
					index_i = 0;

					for(int i = 0 ;i < tournament_size ;i++)
					{
						temp_random = random(0,size_of_population-1);
						for(int j = 0;j<312 ;j++)
						{
							temp_for_tournament_population[index_i][j] = current_population[temp_random][j]; 
						}
						index_i++;

					}
					//cout<<"5"<<endl;

					for(int i = 0 ; i<312;i++)
					{
						temp_r[i] =0;
					}

					/*for(int  i  = 0;i<size_of_population;i++)
					{
					for(int  j = 0;j<312;j++)
					{
					cout<<temp_for_tournament_population[i][j]<<"|";
					}
					cout<<endl;
					}
					*/


					for(int  i = 0 ;i<tournament_size;i++)
					{
						for(int  j = 0;j<312;j++)
						{
							temp_r[j] = temp_for_tournament_population[i][j];
						}
						array_containing_fitness_of_tournament_selection_population[i] = (1/calculate_fitness(temp_r));
					}


					for(int i = 0;i<tournament_size;i++)
					{
						for(int  j = 0;j<tournament_size-1-i;j++)
						{
							if(array_containing_fitness_of_tournament_selection_population[j]<array_containing_fitness_of_tournament_selection_population[j+1])
							{
								swap(array_containing_fitness_of_tournament_selection_population[j],array_containing_fitness_of_tournament_selection_population[j+1]);
								for(int k = 0;k<312;k++)
								{
									temp_r[k] = temp_for_tournament_population[j][k];
									temp_for_tournament_population[j][k] = temp_for_tournament_population[j+1][k];
									temp_for_tournament_population[j+1][k] = temp_r[k];
								}
							}
						}
					}

					for(int i = 0 ;i<312 ;i++)
					{
						temp_for_current_population[current_index_in_temp_for_current_population][i] = temp_for_tournament_population[0][i];
					}
					current_index_in_temp_for_current_population++;

					tournament_selection_loop_control_variable --;

				}

				for(int  i = 0 ; i < size_of_population;i++)
				{
					for(int j = 0 ;j < 312 ;j++)
					{
						current_population[i][j] = temp_for_current_population[i][j];
					}
				}
				/*
				for(int  i = 0  ;i <size_of_population ;i++)
				{
				for(int j =0;j<312;j++)
				{
				cout<<current_population[i][j]<<"|";
				}
				}
				*/

				//cout<<"9"<<endl;






				while(loop_control_variable > 0)
				{


					//			cout<<"4"<<endl;

					random_number_1 = random(0,size_of_population-1);
					random_number_2 = random(0,size_of_population-1);
					for(int i = 0 ;i<312;i++)
					{
						chromosome_1_crossover[i] = current_population[random_number_1][i]; 
					}
					for(int i = 0;i<312;i++)
					{
						chromosome_2_crossover[i] = current_population[random_number_2][i];
					}

					two_new_chromosomes = single_point_crossover(chromosome_1_crossover,chromosome_2_crossover);



					for(int i = start;i<end;i++)
					{
						for(int  j = 0 ;j < 312; j ++)
						{
							new_population[i][j] = two_new_chromosomes[index_i_of_two_new_chromosome][index_j_of_two_new_chromosome++];
						}
						index_i_of_two_new_chromosome++;
						index_j_of_two_new_chromosome = 0;
					}
					//cout<<"10"<<endl;


					start = start + 2;
					end = end+2;
					index_i_of_two_new_chromosome = 0;
					index_j_of_two_new_chromosome = 0;

					loop_control_variable--;

				}
				//cout<<"11"<<endl;

				//			cout<<"5"<<endl;
				doing_mutation(new_population,mutation_rate,size_of_population);
				array_containing_fitness_of_new_chromosomes = new float[size_of_population]();


				for(int  i = 0 ; i <312 ; i++)
				{
					arry1[i] = 0;

				}
				z=0;
				k=0;
				for(int i=0;i<size_of_population;i++)
				{
					z = 0;
					k=0;
					for(int j=0;j<312;j++)
					{
						arry1[k++] = new_population[i][j];
					}
					array_containing_fitness_of_new_chromosomes[i] = (1/calculate_fitness(arry1));
				}

				//				cout<<"6"<<endl;

				//combining the 2 populations
				for(int i = 0 ; i <size_of_population;i++)
				{
					for(int j = 0; j<312 ; j++)
					{
						array_containing_both_new_and_old_population[i][j] = current_population[i][j];
					}
				}

				u = 0;
				for(int i = size_of_population ; i <(2*size_of_population);i++)
				{
					for(int j = 0;j<312 ;j++)
					{
						array_containing_both_new_and_old_population[i][j] = new_population[u][j];
					}
					u++;
				}


				//combining the fitnesses of the 2 populations
				array_containing_new_and_old_fitnesses = new float[2*size_of_population]();
				for(int  i = 0 ;i <size_of_population ; i++)
				{
					array_containing_new_and_old_fitnesses[i] = array_containing_fitness_of_chromosomes[i];
				}
				l = 0;
				for(int i = size_of_population;i<(2*size_of_population);i++)
				{
					array_containing_new_and_old_fitnesses[i]  = array_containing_fitness_of_new_chromosomes[l++];
				}

				//sorting the array which contains the fitness of old and new chromosomes
				//and also the corresponding chromosomes in array containing new and old 
				//chromosomes
				for(int  i = 0 ; i <(2*size_of_population)-1;i++ )
				{
					for(int j = 0;j<(2*size_of_population)-1-i;j++)
					{
						if(array_containing_new_and_old_fitnesses[j]<array_containing_new_and_old_fitnesses[j+1])
						{
							swap(array_containing_new_and_old_fitnesses[j],array_containing_new_and_old_fitnesses[j+1]);
							for(int k = 0 ; k < 312 ;k ++)
							{
								temp_for_storing_chromosome[k] = array_containing_both_new_and_old_population[j][k];
								array_containing_both_new_and_old_population[j][k] = array_containing_both_new_and_old_population[j+1][k];
								array_containing_both_new_and_old_population[j+1][k] = array_containing_both_new_and_old_population[j][k];

							}

						}
					}

				}

				int smallest_index;
				bool flag;
				if((best_fitness_required - array_containing_new_and_old_fitnesses[0])>0)
				{
					smallest = best_fitness_required - array_containing_new_and_old_fitnesses[0];
					smallest_index = 0;
					flag = true;
				}

				//updating the best chromosome so far 
				for(int  i = 1 ;i < (2*size_of_population);i++)
				{
					int temp = best_fitness_required - array_containing_new_and_old_fitnesses[i];
					if(temp > 0) 
					{
						if(temp<smallest)
						{
							smallest = temp;
							smallest_index = i;
							flag = true;


						}
					}
				}

				for(int  i = 0 ; i<312 ; i++)
				{
					best_chromosome_so_far[i] = array_containing_both_new_and_old_population[smallest_index][i];
					result_chromosome_1_index = smallest_index; 
				}





				//updating the population for next iteration

				for(int  i = 0 ; i <size_of_population ; i++)
				{
					for(int j = 0;j <312 ; j ++)
					{
						current_population[i][j]= array_containing_both_new_and_old_population[i][j];
					}
				}



				for(int i = 0 ; i <size_of_population;i++)
				{
					if(array_containing_fitness_of_new_chromosomes[i] >= best_fitness_required)
					{

						for(int  j = 0 ;j < 312 ; j ++)
						{
							result_choromosome[j] = new_population[i][j];
						}
						result_chromosome_2_index = i;
						break_flag = true;
						break;
					}
				}

				if(break_flag == true)
				{
					cout<<"Congratulations Required Fitness Chromosome found!"<<endl;
					for(int i = 0;i<312;i++)
					{
						cout<<result_choromosome[i]<<"|";
					}
					cout<<endl;

					cout<<"Fitness: "<<array_containing_fitness_of_new_chromosomes[result_chromosome_2_index]<<endl;
					cout<<"Iterations: "<<endl;
					cout<<(iterations - loop_control_variable_1)<<endl;
					cout<<"The Pictorial representation of the chromosome is "<<endl;

					break;
				}

				if(break_flag == false)
				{

					for(int i = 0 ; i <size_of_population;i++)
					{
						if(array_containing_fitness_of_chromosomes[i] >= best_fitness_required)
						{

							for(int  j = 0 ;j < 312 ; j ++)
							{
								result_choromosome_1[j] = current_population[i][j];
							}
							result_chromosome_3_index = i;
							break_flag_1 = true;
							break;
						}
					}

					if(break_flag_1 == true)
					{
						cout<<"Congratulations Required Fitness Chromosome found!"<<endl;
						for(int i = 0;i<312;i++)
						{
							cout<<result_choromosome_1[i]<<"|";
						}
						cout<<endl;

						cout<<"Fitness: "<<array_containing_fitness_of_chromosomes[result_chromosome_3_index]<<endl;
						cout<<"Iterations "<<endl;
						cout<<(iterations-loop_control_variable_1)<<endl;
						cout<<"The Pictorial representation of the chromosome is "<<endl;

						break;
					}

				}








				loop_control_variable_1--;

				if(loop_control_variable_1 == 0)
				{
					cout<<"Iterations ended"<<endl;
					cout<<"Best chromosome so far is "<<endl;
					for(int i = 0 ; i <312 ;i++)
					{
						cout<<best_chromosome_so_far[i]<<"|";
					}
					cout<<endl;

					cout<<"Fitness :"<<array_containing_new_and_old_fitnesses[result_chromosome_1_index]<<endl;

					cout<<endl;
					cout<<"The Pictorial Representation of the chromosome is !"<<endl;
					break;
				}
			}
		}
		else if(cross_over_criteria ==2)
		{
			cout<<"1"<<endl;
			while(loop_control_variable_1>0)
			{


				check_1 = true;
				loop_control_variable = size_of_population/2; 

				index_i_of_two_new_chromosome = 0;
				index_j_of_two_new_chromosome = 0;
				start = 0;
				end = 2;

				//	cout<<"3"<<endl;

				//in this loop i am creating the new population
				//based on the current population
				//using one point crossover

				index_i = 0;
				temp_random = 0;
				//cout<<"3"<<endl;

				while(tournament_selection_loop_control_variable > 0)
				{
					//cout<<"4"<<endl;
					index_i = 0;

					for(int i = 0 ;i < tournament_size ;i++)
					{
						temp_random = random(0,size_of_population-1);
						for(int j = 0;j<312 ;j++)
						{
							temp_for_tournament_population[index_i][j] = current_population[temp_random][j]; 
						}
						index_i++;

					}
					//cout<<"5"<<endl;

					for(int i = 0 ; i<312;i++)
					{
						temp_r[i] =0;
					}

					/*for(int  i  = 0;i<size_of_population;i++)
					{
					for(int  j = 0;j<312;j++)
					{
					cout<<temp_for_tournament_population[i][j]<<"|";
					}
					cout<<endl;
					}
					*/


					for(int  i = 0 ;i<tournament_size;i++)
					{
						for(int  j = 0;j<312;j++)
						{
							temp_r[j] = temp_for_tournament_population[i][j];
						}
						array_containing_fitness_of_tournament_selection_population[i] = (1/calculate_fitness(temp_r));
					}


					for(int i = 0;i<tournament_size;i++)
					{
						for(int  j = 0;j<tournament_size-1-i;j++)
						{
							if(array_containing_fitness_of_tournament_selection_population[j]<array_containing_fitness_of_tournament_selection_population[j+1])
							{
								swap(array_containing_fitness_of_tournament_selection_population[j],array_containing_fitness_of_tournament_selection_population[j+1]);
								for(int k = 0;k<312;k++)
								{
									temp_r[k] = temp_for_tournament_population[j][k];
									temp_for_tournament_population[j][k] = temp_for_tournament_population[j+1][k];
									temp_for_tournament_population[j+1][k] = temp_r[k];
								}
							}
						}
					}

					for(int i = 0 ;i<312 ;i++)
					{
						temp_for_current_population[current_index_in_temp_for_current_population][i] = temp_for_tournament_population[0][i];
					}
					current_index_in_temp_for_current_population++;

					tournament_selection_loop_control_variable --;

				}

				for(int  i = 0 ; i < size_of_population;i++)
				{
					for(int j = 0 ;j < 312 ;j++)
					{
						current_population[i][j] = temp_for_current_population[i][j];
					}
				}
				/*
				for(int  i = 0  ;i <size_of_population ;i++)
				{
				for(int j =0;j<312;j++)
				{
				cout<<current_population[i][j]<<"|";
				}
				}
				*/

				//cout<<"9"<<endl;






				while(loop_control_variable > 0)
				{


					//			cout<<"4"<<endl;

					random_number_1 = random(0,size_of_population-1);
					random_number_2 = random(0,size_of_population-1);
					for(int i = 0 ;i<312;i++)
					{
						chromosome_1_crossover[i] = current_population[random_number_1][i]; 
					}
					for(int i = 0;i<312;i++)
					{
						chromosome_2_crossover[i] = current_population[random_number_2][i];
					}

					two_new_chromosomes = two_point_crossover(chromosome_1_crossover,chromosome_2_crossover);



					for(int i = start;i<end;i++)
					{
						for(int  j = 0 ;j < 312; j ++)
						{
							new_population[i][j] = two_new_chromosomes[index_i_of_two_new_chromosome][index_j_of_two_new_chromosome++];
						}
						index_i_of_two_new_chromosome++;
						index_j_of_two_new_chromosome = 0;
					}
					//cout<<"10"<<endl;


					start = start + 2;
					end = end+2;
					index_i_of_two_new_chromosome = 0;
					index_j_of_two_new_chromosome = 0;

					loop_control_variable--;

				}
				//cout<<"11"<<endl;

				//			cout<<"5"<<endl;
				doing_mutation(new_population,mutation_rate,size_of_population);
				array_containing_fitness_of_new_chromosomes = new float[size_of_population]();


				for(int  i = 0 ; i <312 ; i++)
				{
					arry1[i] = 0;

				}
				z=0;
				k=0;
				for(int i=0;i<size_of_population;i++)
				{
					z = 0;
					k=0;
					for(int j=0;j<312;j++)
					{
						arry1[k++] = new_population[i][j];
					}
					array_containing_fitness_of_new_chromosomes[i] = (1/calculate_fitness(arry1));
				}

				//				cout<<"6"<<endl;

				//combining the 2 populations
				for(int i = 0 ; i <size_of_population;i++)
				{
					for(int j = 0; j<312 ; j++)
					{
						array_containing_both_new_and_old_population[i][j] = current_population[i][j];
					}
				}

				u = 0;
				for(int i = size_of_population ; i <(2*size_of_population);i++)
				{
					for(int j = 0;j<312 ;j++)
					{
						array_containing_both_new_and_old_population[i][j] = new_population[u][j];
					}
					u++;
				}


				//combining the fitnesses of the 2 populations
				array_containing_new_and_old_fitnesses = new float[2*size_of_population]();
				for(int  i = 0 ;i <size_of_population ; i++)
				{
					array_containing_new_and_old_fitnesses[i] = array_containing_fitness_of_chromosomes[i];
				}
				l = 0;
				for(int i = size_of_population;i<(2*size_of_population);i++)
				{
					array_containing_new_and_old_fitnesses[i]  = array_containing_fitness_of_new_chromosomes[l++];
				}

				//sorting the array which contains the fitness of old and new chromosomes
				//and also the corresponding chromosomes in array containing new and old 
				//chromosomes
				for(int  i = 0 ; i <(2*size_of_population)-1;i++ )
				{
					for(int j = 0;j<(2*size_of_population)-1-i;j++)
					{
						if(array_containing_new_and_old_fitnesses[j]<array_containing_new_and_old_fitnesses[j+1])
						{
							swap(array_containing_new_and_old_fitnesses[j],array_containing_new_and_old_fitnesses[j+1]);
							for(int k = 0 ; k < 312 ;k ++)
							{
								temp_for_storing_chromosome[k] = array_containing_both_new_and_old_population[j][k];
								array_containing_both_new_and_old_population[j][k] = array_containing_both_new_and_old_population[j+1][k];
								array_containing_both_new_and_old_population[j+1][k] = array_containing_both_new_and_old_population[j][k];

							}

						}
					}

				}

				int smallest_index;
				bool flag;
				if((best_fitness_required - array_containing_new_and_old_fitnesses[0])>0)
				{
					smallest = best_fitness_required - array_containing_new_and_old_fitnesses[0];
					smallest_index = 0;
					flag = true;
				}

				//updating the best chromosome so far 
				for(int  i = 1 ;i < (2*size_of_population);i++)
				{
					int temp = best_fitness_required - array_containing_new_and_old_fitnesses[i];
					if(temp > 0) 
					{
						if(temp<smallest)
						{
							smallest = temp;
							smallest_index = i;
							flag = true;


						}
					}
				}

				for(int  i = 0 ; i<312 ; i++)
				{
					best_chromosome_so_far[i] = array_containing_both_new_and_old_population[smallest_index][i];
					result_chromosome_1_index = smallest_index; 
				}





				//updating the population for next iteration

				for(int  i = 0 ; i <size_of_population ; i++)
				{
					for(int j = 0;j <312 ; j ++)
					{
						current_population[i][j]= array_containing_both_new_and_old_population[i][j];
					}
				}



				for(int i = 0 ; i <size_of_population;i++)
				{
					if(array_containing_fitness_of_new_chromosomes[i] >= best_fitness_required)
					{

						for(int  j = 0 ;j < 312 ; j ++)
						{
							result_choromosome[j] = new_population[i][j];
						}
						result_chromosome_2_index = i;
						break_flag = true;
						break;
					}
				}

				if(break_flag == true)
				{
					cout<<"Congratulations Required Fitness Chromosome found!"<<endl;
					for(int i = 0;i<312;i++)
					{
						cout<<result_choromosome[i]<<"|";
					}
					cout<<endl;

					cout<<"Fitness: "<<array_containing_fitness_of_new_chromosomes[result_chromosome_2_index]<<endl;
					cout<<"Iterations: "<<endl;
					cout<<(iterations - loop_control_variable_1)<<endl;
					cout<<"The Pictorial representation of the chromosome is "<<endl;

					break;
				}

				if(break_flag == false)
				{

					for(int i = 0 ; i <size_of_population;i++)
					{
						if(array_containing_fitness_of_chromosomes[i] >= best_fitness_required)
						{

							for(int  j = 0 ;j < 312 ; j ++)
							{
								result_choromosome_1[j] = current_population[i][j];
							}
							result_chromosome_3_index = i;
							break_flag_1 = true;
							break;
						}
					}

					if(break_flag_1 == true)
					{
						cout<<"Congratulations Required Fitness Chromosome found!"<<endl;
						for(int i = 0;i<312;i++)
						{
							cout<<result_choromosome_1[i]<<"|";
						}
						cout<<endl;

						cout<<"Fitness: "<<array_containing_fitness_of_chromosomes[result_chromosome_3_index]<<endl;
						cout<<"Iterations "<<endl;
						cout<<(iterations-loop_control_variable_1)<<endl;
						cout<<"The Pictorial representation of the chromosome is "<<endl;

						break;
					}

				}








				loop_control_variable_1--;

				if(loop_control_variable_1 == 0)
				{
					cout<<"Iterations ended"<<endl;
					cout<<"Best chromosome so far is "<<endl;
					for(int i = 0 ; i <312 ;i++)
					{
						cout<<best_chromosome_so_far[i]<<"|";
					}
					cout<<endl;

					cout<<"Fitness :"<<array_containing_new_and_old_fitnesses[result_chromosome_1_index]<<endl;

					cout<<endl;
					cout<<"The Pictorial Representation of the chromosome is !"<<endl;
					break;
				}
			}
		}
		else if(cross_over_criteria ==3)
		{
			cout<<"1"<<endl;
			while(loop_control_variable_1>0)
			{


				check_1 = true;
				loop_control_variable = size_of_population/2; 

				index_i_of_two_new_chromosome = 0;
				index_j_of_two_new_chromosome = 0;
				start = 0;
				end = 2;

				//	cout<<"3"<<endl;

				//in this loop i am creating the new population
				//based on the current population
				//using one point crossover

				index_i = 0;
				temp_random = 0;
				//cout<<"3"<<endl;

				while(tournament_selection_loop_control_variable > 0)
				{
					//cout<<"4"<<endl;
					index_i = 0;

					for(int i = 0 ;i < tournament_size ;i++)
					{
						temp_random = random(0,size_of_population-1);
						for(int j = 0;j<312 ;j++)
						{
							temp_for_tournament_population[index_i][j] = current_population[temp_random][j]; 
						}
						index_i++;

					}
					//cout<<"5"<<endl;

					for(int i = 0 ; i<312;i++)
					{
						temp_r[i] =0;
					}

					/*for(int  i  = 0;i<size_of_population;i++)
					{
					for(int  j = 0;j<312;j++)
					{
					cout<<temp_for_tournament_population[i][j]<<"|";
					}
					cout<<endl;
					}
					*/


					for(int  i = 0 ;i<tournament_size;i++)
					{
						for(int  j = 0;j<312;j++)
						{
							temp_r[j] = temp_for_tournament_population[i][j];
						}
						array_containing_fitness_of_tournament_selection_population[i] = (1/calculate_fitness(temp_r));
					}


					for(int i = 0;i<tournament_size;i++)
					{
						for(int  j = 0;j<tournament_size-1-i;j++)
						{
							if(array_containing_fitness_of_tournament_selection_population[j]<array_containing_fitness_of_tournament_selection_population[j+1])
							{
								swap(array_containing_fitness_of_tournament_selection_population[j],array_containing_fitness_of_tournament_selection_population[j+1]);
								for(int k = 0;k<312;k++)
								{
									temp_r[k] = temp_for_tournament_population[j][k];
									temp_for_tournament_population[j][k] = temp_for_tournament_population[j+1][k];
									temp_for_tournament_population[j+1][k] = temp_r[k];
								}
							}
						}
					}

					for(int i = 0 ;i<312 ;i++)
					{
						temp_for_current_population[current_index_in_temp_for_current_population][i] = temp_for_tournament_population[0][i];
					}
					current_index_in_temp_for_current_population++;

					tournament_selection_loop_control_variable --;

				}

				for(int  i = 0 ; i < size_of_population;i++)
				{
					for(int j = 0 ;j < 312 ;j++)
					{
						current_population[i][j] = temp_for_current_population[i][j];
					}
				}
				/*
				for(int  i = 0  ;i <size_of_population ;i++)
				{
				for(int j =0;j<312;j++)
				{
				cout<<current_population[i][j]<<"|";
				}
				}
				*/

				//cout<<"9"<<endl;






				while(loop_control_variable > 0)
				{


					//			cout<<"4"<<endl;

					random_number_1 = random(0,size_of_population-1);
					random_number_2 = random(0,size_of_population-1);
					for(int i = 0 ;i<312;i++)
					{
						chromosome_1_crossover[i] = current_population[random_number_1][i]; 
					}
					for(int i = 0;i<312;i++)
					{
						chromosome_2_crossover[i] = current_population[random_number_2][i];
					}

					two_new_chromosomes =uniform_point_crossover(chromosome_1_crossover,chromosome_2_crossover);



					for(int i = start;i<end;i++)
					{
						for(int  j = 0 ;j < 312; j ++)
						{
							new_population[i][j] = two_new_chromosomes[index_i_of_two_new_chromosome][index_j_of_two_new_chromosome++];
						}
						index_i_of_two_new_chromosome++;
						index_j_of_two_new_chromosome = 0;
					}
					//cout<<"10"<<endl;


					start = start + 2;
					end = end+2;
					index_i_of_two_new_chromosome = 0;
					index_j_of_two_new_chromosome = 0;

					loop_control_variable--;

				}
				//cout<<"11"<<endl;

				//			cout<<"5"<<endl;
				doing_mutation(new_population,mutation_rate,size_of_population);
				array_containing_fitness_of_new_chromosomes = new float[size_of_population]();


				for(int  i = 0 ; i <312 ; i++)
				{
					arry1[i] = 0;

				}
				z=0;
				k=0;
				for(int i=0;i<size_of_population;i++)
				{
					z = 0;
					k=0;
					for(int j=0;j<312;j++)
					{
						arry1[k++] = new_population[i][j];
					}
					array_containing_fitness_of_new_chromosomes[i] = (1/calculate_fitness(arry1));
				}

				//				cout<<"6"<<endl;

				//combining the 2 populations
				for(int i = 0 ; i <size_of_population;i++)
				{
					for(int j = 0; j<312 ; j++)
					{
						array_containing_both_new_and_old_population[i][j] = current_population[i][j];
					}
				}

				u = 0;
				for(int i = size_of_population ; i <(2*size_of_population);i++)
				{
					for(int j = 0;j<312 ;j++)
					{
						array_containing_both_new_and_old_population[i][j] = new_population[u][j];
					}
					u++;
				}


				//combining the fitnesses of the 2 populations
				array_containing_new_and_old_fitnesses = new float[2*size_of_population]();
				for(int  i = 0 ;i <size_of_population ; i++)
				{
					array_containing_new_and_old_fitnesses[i] = array_containing_fitness_of_chromosomes[i];
				}
				l = 0;
				for(int i = size_of_population;i<(2*size_of_population);i++)
				{
					array_containing_new_and_old_fitnesses[i]  = array_containing_fitness_of_new_chromosomes[l++];
				}

				//sorting the array which contains the fitness of old and new chromosomes
				//and also the corresponding chromosomes in array containing new and old 
				//chromosomes
				for(int  i = 0 ; i <(2*size_of_population)-1;i++ )
				{
					for(int j = 0;j<(2*size_of_population)-1-i;j++)
					{
						if(array_containing_new_and_old_fitnesses[j]<array_containing_new_and_old_fitnesses[j+1])
						{
							swap(array_containing_new_and_old_fitnesses[j],array_containing_new_and_old_fitnesses[j+1]);
							for(int k = 0 ; k < 312 ;k ++)
							{
								temp_for_storing_chromosome[k] = array_containing_both_new_and_old_population[j][k];
								array_containing_both_new_and_old_population[j][k] = array_containing_both_new_and_old_population[j+1][k];
								array_containing_both_new_and_old_population[j+1][k] = array_containing_both_new_and_old_population[j][k];

							}

						}
					}

				}

				int smallest_index;
				bool flag;
				if((best_fitness_required - array_containing_new_and_old_fitnesses[0])>0)
				{
					smallest = best_fitness_required - array_containing_new_and_old_fitnesses[0];
					smallest_index = 0;
					flag = true;
				}

				//updating the best chromosome so far 
				for(int  i = 1 ;i < (2*size_of_population);i++)
				{
					int temp = best_fitness_required - array_containing_new_and_old_fitnesses[i];
					if(temp > 0) 
					{
						if(temp<smallest)
						{
							smallest = temp;
							smallest_index = i;
							flag = true;


						}
					}
				}

				for(int  i = 0 ; i<312 ; i++)
				{
					best_chromosome_so_far[i] = array_containing_both_new_and_old_population[smallest_index][i];
					result_chromosome_1_index = smallest_index; 
				}





				//updating the population for next iteration

				for(int  i = 0 ; i <size_of_population ; i++)
				{
					for(int j = 0;j <312 ; j ++)
					{
						current_population[i][j]= array_containing_both_new_and_old_population[i][j];
					}
				}



				for(int i = 0 ; i <size_of_population;i++)
				{
					if(array_containing_fitness_of_new_chromosomes[i] >= best_fitness_required)
					{

						for(int  j = 0 ;j < 312 ; j ++)
						{
							result_choromosome[j] = new_population[i][j];
						}
						result_chromosome_2_index = i;
						break_flag = true;
						break;
					}
				}

				if(break_flag == true)
				{
					cout<<"Congratulations Required Fitness Chromosome found!"<<endl;
					for(int i = 0;i<312;i++)
					{
						cout<<result_choromosome[i]<<"|";
					}
					cout<<endl;

					cout<<"Fitness: "<<array_containing_fitness_of_new_chromosomes[result_chromosome_2_index]<<endl;
					cout<<"Iterations: "<<endl;
					cout<<(iterations - loop_control_variable_1)<<endl;
					cout<<"The Pictorial representation of the chromosome is "<<endl;

					break;
				}

				if(break_flag == false)
				{

					for(int i = 0 ; i <size_of_population;i++)
					{
						if(array_containing_fitness_of_chromosomes[i] >= best_fitness_required)
						{

							for(int  j = 0 ;j < 312 ; j ++)
							{
								result_choromosome_1[j] = current_population[i][j];
							}
							result_chromosome_3_index = i;
							break_flag_1 = true;
							break;
						}
					}

					if(break_flag_1 == true)
					{
						cout<<"Congratulations Required Fitness Chromosome found!"<<endl;
						for(int i = 0;i<312;i++)
						{
							cout<<result_choromosome_1[i]<<"|";
						}
						cout<<endl;

						cout<<"Fitness: "<<array_containing_fitness_of_chromosomes[result_chromosome_3_index]<<endl;
						cout<<"Iterations "<<endl;
						cout<<(iterations-loop_control_variable_1)<<endl;
						cout<<"The Pictorial representation of the chromosome is "<<endl;

						break;
					}

				}








				loop_control_variable_1--;

				if(loop_control_variable_1 == 0)
				{
					cout<<"Iterations ended"<<endl;
					cout<<"Best chromosome so far is "<<endl;
					for(int i = 0 ; i <312 ;i++)
					{
						cout<<best_chromosome_so_far[i]<<"|";
					}
					cout<<endl;

					cout<<"Fitness :"<<array_containing_new_and_old_fitnesses[result_chromosome_1_index]<<endl;

					cout<<endl;
					cout<<"The Pictorial Representation of the chromosome is !"<<endl;
					break;
				}
			}
		}
		else if(cross_over_criteria ==4)
		{
			cout<<"1"<<endl;
			while(loop_control_variable_1>0)
			{


				check_1 = true;
				loop_control_variable = size_of_population/2; 

				index_i_of_two_new_chromosome = 0;
				index_j_of_two_new_chromosome = 0;
				start = 0;
				end = 2;

				//	cout<<"3"<<endl;

				//in this loop i am creating the new population
				//based on the current population
				//using one point crossover

				index_i = 0;
				temp_random = 0;
				//cout<<"3"<<endl;

				while(tournament_selection_loop_control_variable > 0)
				{
					//cout<<"4"<<endl;
					index_i = 0;

					for(int i = 0 ;i < tournament_size ;i++)
					{
						temp_random = random(0,size_of_population-1);
						for(int j = 0;j<312 ;j++)
						{
							temp_for_tournament_population[index_i][j] = current_population[temp_random][j]; 
						}
						index_i++;

					}
					//cout<<"5"<<endl;

					for(int i = 0 ; i<312;i++)
					{
						temp_r[i] =0;
					}

					/*for(int  i  = 0;i<size_of_population;i++)
					{
					for(int  j = 0;j<312;j++)
					{
					cout<<temp_for_tournament_population[i][j]<<"|";
					}
					cout<<endl;
					}
					*/


					for(int  i = 0 ;i<tournament_size;i++)
					{
						for(int  j = 0;j<312;j++)
						{
							temp_r[j] = temp_for_tournament_population[i][j];
						}
						array_containing_fitness_of_tournament_selection_population[i] = (1/calculate_fitness(temp_r));
					}


					for(int i = 0;i<tournament_size;i++)
					{
						for(int  j = 0;j<tournament_size-1-i;j++)
						{
							if(array_containing_fitness_of_tournament_selection_population[j]<array_containing_fitness_of_tournament_selection_population[j+1])
							{
								swap(array_containing_fitness_of_tournament_selection_population[j],array_containing_fitness_of_tournament_selection_population[j+1]);
								for(int k = 0;k<312;k++)
								{
									temp_r[k] = temp_for_tournament_population[j][k];
									temp_for_tournament_population[j][k] = temp_for_tournament_population[j+1][k];
									temp_for_tournament_population[j+1][k] = temp_r[k];
								}
							}
						}
					}

					for(int i = 0 ;i<312 ;i++)
					{
						temp_for_current_population[current_index_in_temp_for_current_population][i] = temp_for_tournament_population[0][i];
					}
					current_index_in_temp_for_current_population++;

					tournament_selection_loop_control_variable --;

				}

				for(int  i = 0 ; i < size_of_population;i++)
				{
					for(int j = 0 ;j < 312 ;j++)
					{
						current_population[i][j] = temp_for_current_population[i][j];
					}
				}
				/*
				for(int  i = 0  ;i <size_of_population ;i++)
				{
				for(int j =0;j<312;j++)
				{
				cout<<current_population[i][j]<<"|";
				}
				}
				*/

				//cout<<"9"<<endl;






				while(loop_control_variable > 0)
				{


					//			cout<<"4"<<endl;

					random_number_1 = random(0,size_of_population-1);
					random_number_2 = random(0,size_of_population-1);
					for(int i = 0 ;i<312;i++)
					{
						chromosome_1_crossover[i] = current_population[random_number_1][i]; 
					}
					for(int i = 0;i<312;i++)
					{
						chromosome_2_crossover[i] = current_population[random_number_2][i];
					}

					two_new_chromosomes = first_order_crossover(chromosome_1_crossover,chromosome_2_crossover);



					for(int i = start;i<end;i++)
					{
						for(int  j = 0 ;j < 312; j ++)
						{
							new_population[i][j] = two_new_chromosomes[index_i_of_two_new_chromosome][index_j_of_two_new_chromosome++];
						}
						index_i_of_two_new_chromosome++;
						index_j_of_two_new_chromosome = 0;
					}
					//cout<<"10"<<endl;


					start = start + 2;
					end = end+2;
					index_i_of_two_new_chromosome = 0;
					index_j_of_two_new_chromosome = 0;

					loop_control_variable--;

				}
				//cout<<"11"<<endl;

				//			cout<<"5"<<endl;
				doing_mutation(new_population,mutation_rate,size_of_population);
				array_containing_fitness_of_new_chromosomes = new float[size_of_population]();


				for(int  i = 0 ; i <312 ; i++)
				{
					arry1[i] = 0;

				}
				z=0;
				k=0;
				for(int i=0;i<size_of_population;i++)
				{
					z = 0;
					k=0;
					for(int j=0;j<312;j++)
					{
						arry1[k++] = new_population[i][j];
					}
					array_containing_fitness_of_new_chromosomes[i] = (1/calculate_fitness(arry1));
				}

				//				cout<<"6"<<endl;

				//combining the 2 populations
				for(int i = 0 ; i <size_of_population;i++)
				{
					for(int j = 0; j<312 ; j++)
					{
						array_containing_both_new_and_old_population[i][j] = current_population[i][j];
					}
				}

				u = 0;
				for(int i = size_of_population ; i <(2*size_of_population);i++)
				{
					for(int j = 0;j<312 ;j++)
					{
						array_containing_both_new_and_old_population[i][j] = new_population[u][j];
					}
					u++;
				}


				//combining the fitnesses of the 2 populations
				array_containing_new_and_old_fitnesses = new float[2*size_of_population]();
				for(int  i = 0 ;i <size_of_population ; i++)
				{
					array_containing_new_and_old_fitnesses[i] = array_containing_fitness_of_chromosomes[i];
				}
				l = 0;
				for(int i = size_of_population;i<(2*size_of_population);i++)
				{
					array_containing_new_and_old_fitnesses[i]  = array_containing_fitness_of_new_chromosomes[l++];
				}

				//sorting the array which contains the fitness of old and new chromosomes
				//and also the corresponding chromosomes in array containing new and old 
				//chromosomes
				for(int  i = 0 ; i <(2*size_of_population)-1;i++ )
				{
					for(int j = 0;j<(2*size_of_population)-1-i;j++)
					{
						if(array_containing_new_and_old_fitnesses[j]<array_containing_new_and_old_fitnesses[j+1])
						{
							swap(array_containing_new_and_old_fitnesses[j],array_containing_new_and_old_fitnesses[j+1]);
							for(int k = 0 ; k < 312 ;k ++)
							{
								temp_for_storing_chromosome[k] = array_containing_both_new_and_old_population[j][k];
								array_containing_both_new_and_old_population[j][k] = array_containing_both_new_and_old_population[j+1][k];
								array_containing_both_new_and_old_population[j+1][k] = array_containing_both_new_and_old_population[j][k];

							}

						}
					}

				}

				int smallest_index;
				bool flag;
				if((best_fitness_required - array_containing_new_and_old_fitnesses[0])>0)
				{
					smallest = best_fitness_required - array_containing_new_and_old_fitnesses[0];
					smallest_index = 0;
					flag = true;
				}

				//updating the best chromosome so far 
				for(int  i = 1 ;i < (2*size_of_population);i++)
				{
					int temp = best_fitness_required - array_containing_new_and_old_fitnesses[i];
					if(temp > 0) 
					{
						if(temp<smallest)
						{
							smallest = temp;
							smallest_index = i;
							flag = true;


						}
					}
				}

				for(int  i = 0 ; i<312 ; i++)
				{
					best_chromosome_so_far[i] = array_containing_both_new_and_old_population[smallest_index][i];
					result_chromosome_1_index = smallest_index; 
				}





				//updating the population for next iteration

				for(int  i = 0 ; i <size_of_population ; i++)
				{
					for(int j = 0;j <312 ; j ++)
					{
						current_population[i][j]= array_containing_both_new_and_old_population[i][j];
					}
				}



				for(int i = 0 ; i <size_of_population;i++)
				{
					if(array_containing_fitness_of_new_chromosomes[i] >= best_fitness_required)
					{

						for(int  j = 0 ;j < 312 ; j ++)
						{
							result_choromosome[j] = new_population[i][j];
						}
						result_chromosome_2_index = i;
						break_flag = true;
						break;
					}
				}

				if(break_flag == true)
				{
					cout<<"Congratulations Required Fitness Chromosome found!"<<endl;
					for(int i = 0;i<312;i++)
					{
						cout<<result_choromosome[i]<<"|";
					}
					cout<<endl;

					cout<<"Fitness: "<<array_containing_fitness_of_new_chromosomes[result_chromosome_2_index]<<endl;
					cout<<"Iterations: "<<endl;
					cout<<(iterations - loop_control_variable_1)<<endl;
					cout<<"The Pictorial representation of the chromosome is "<<endl;

					break;
				}

				if(break_flag == false)
				{

					for(int i = 0 ; i <size_of_population;i++)
					{
						if(array_containing_fitness_of_chromosomes[i] >= best_fitness_required)
						{

							for(int  j = 0 ;j < 312 ; j ++)
							{
								result_choromosome_1[j] = current_population[i][j];
							}
							result_chromosome_3_index = i;
							break_flag_1 = true;
							break;
						}
					}

					if(break_flag_1 == true)
					{
						cout<<"Congratulations Required Fitness Chromosome found!"<<endl;
						for(int i = 0;i<312;i++)
						{
							cout<<result_choromosome_1[i]<<"|";
						}
						cout<<endl;

						cout<<"Fitness: "<<array_containing_fitness_of_chromosomes[result_chromosome_3_index]<<endl;
						cout<<"Iterations "<<endl;
						cout<<(iterations-loop_control_variable_1)<<endl;
						cout<<"The Pictorial representation of the chromosome is "<<endl;

						break;
					}

				}








				loop_control_variable_1--;

				if(loop_control_variable_1 == 0)
				{
					cout<<"Iterations ended"<<endl;
					cout<<"Best chromosome so far is "<<endl;
					for(int i = 0 ; i <312 ;i++)
					{
						cout<<best_chromosome_so_far[i]<<"|";
					}
					cout<<endl;

					cout<<"Fitness :"<<array_containing_new_and_old_fitnesses[result_chromosome_1_index]<<endl;

					cout<<endl;
					cout<<"The Pictorial Representation of the chromosome is !"<<endl;
					break;
				}
			}
		}
	}
	else if(selection_criteria == 4)
	{

	
		//for storing two new choromosomes after
		//cross over

		two_new_chromosomes = new int* [2];
		for(int i = 0 ;i < 2 ;i ++)
		{
			two_new_chromosomes[i] = new int [312];
		}

		/////////////////////////////////////

		break_flag = false;




		int iterations;
		cout<<"How many iterations you want ?"<<endl;
		cin>>iterations;
		int cross_over_criteria;
		cout<<"Select CrossOver Criteria!"<<endl;
		cout<<"1:One Point Crossover!"<<endl;
		cout<<"2:Two Point Crossover!"<<endl;
		cout<<"3:Uniform Crossover!"<<endl;
		cout<<"4:First Order Crossover!"<<endl;
		cin>>cross_over_criteria;


		float best_fitness_required;
		cout<<"Find the max fitness you want to achieve ?"<<endl;
		cin>>best_fitness_required;

		int loop_control_variable_1;
		loop_control_variable_1 = iterations;

		int reulett_loop_control_variable;
		reulett_loop_control_variable = iterations;





		int mutation_rate;

		cout<<"Enter Mutation Rate"<<endl;
		cin>>mutation_rate;

		int random_number_1;
		int random_number_2;
		random_number_1 = 0;
		random_number_2 = 0;


		int chromosome_1_index;
		int chromosome_2_index;
		chromosome_1_index = 0;
		chromosome_2_index = 0;


		int chromosome_1_crossover[312] = {0};
		int chromosome_2_crossover[312] = {0};

		int start;
		int end;


		int index_i_of_two_new_chromosome ;
		int index_j_of_two_new_chromosome;


		int arr1[312] = {0};

		int k;
		k=0;
		int m;
		int n;
		m = 0;
		n = 0;

		float total_fitness = 0;
		int z;
		bool check_1;
		int loop_control_variable; 
		float * array_containing_fitness_of_chromosomes = new float [size_of_population]();
		float * array_containing_fitness_of_new_chromosomes;
		int arry1[312] = {0};
		float * array_containing_new_and_old_fitnesses ;
		int l;
		int indexing1;
		int firstflag;
		int * result_choromosome ;
		int * result_choromosome_1;
		int * best_chromosome_so_far;
		bool break_flag_1;
		int smallest;
		int best_fitness_so_far;
		best_fitness_so_far = 0;

		result_choromosome = new int[312]();
		result_choromosome_1 = new int[312]();
		best_chromosome_so_far = new int [312]();

		int u;
		int v;
		int minimum_distance;
		minimum_distance =0;
		int * temp_for_storing_chromosome = new int[312]();
		int temp_random;
		int temp_fitness;
		int chromosome_to_pick;
		int indexing_in_temp_for_current_population;
		temp_random  = 0;
		temp_fitness = 0;
		chromosome_to_pick = 0;
		indexing_in_temp_for_current_population = 0;

		temp_for_current_population = new int *[size_of_population]();
		for(int  i = 0 ;i<size_of_population;i++)
		{
			temp_for_current_population[i] = new int[312](); 
		}
		if(cross_over_criteria == 1)
		{
			while(loop_control_variable_1>0)
			{
		
				//				cout<<"1"<<endl;
				for(int i = 0 ; i <312 ;i++)
				{
					arr1[i] = 0;
				}
				k=0;
				for(int i=0;i<size_of_population;i++)
				{
					k = 0;
					for(int j=0;j<312;j++)
					{
						arr1[k++] = current_population[i][j];
					}
					array_containing_fitness_of_chromosomes[i] = (1/calculate_fitness(arr1));
				}

				//cout<<"2"<<endl;


	
				//Reulett Selection
				total_fitness = 0;
				for(int i = 0 ;i <size_of_population;i++)
				{
					total_fitness  = total_fitness + array_containing_fitness_of_chromosomes[i];
				}
						indexing_in_temp_for_current_population =0;
	
				while(reulett_loop_control_variable>0)
				{
	
					temp_random = random(1,total_fitness);
					for(int  i = 0 ;i < size_of_population ; i++)
					{
						temp_fitness = temp_fitness + array_containing_fitness_of_chromosomes[i];
						if(temp_fitness>temp_random)
						{
							chromosome_to_pick = i;
							break;
						}
					}

					
					for(int  i = 0 ; i <312;i++)
					{
						temp_for_current_population[indexing_in_temp_for_current_population][i] = current_population[chromosome_to_pick][i];
					}
					indexing_in_temp_for_current_population++;
					reulett_loop_control_variable--;
				}

								//cout<<"6"<<endl;
	
				for(int i = 0 ;i< size_of_population ;i++)
				{
					for(int j = 0;j<312;j++)
					{
						current_population[i][j] = temp_for_current_population[i][j];
					}

				}



				
				//in this loop i am creating the new population
				//based on the current population
				//using one point crossover


								loop_control_variable = size_of_population/2;
								start = 0;
								end = 2;
				while(loop_control_variable > 0)
				{
					//			cout<<"4"<<endl;

					random_number_1 = random(1,size_of_population-1);
					random_number_2 = random(1,size_of_population-1);

					for(int i = 0;i<312 ;i++)
					{
						chromosome_1_crossover[i] =current_population[random_number_1][i];
					}
					
					
					for(int i = 0;i<312 ;i++)
					{
						chromosome_2_crossover[i] =current_population[random_number_2][i];
					}
					
					
					


					two_new_chromosomes = single_point_crossover(chromosome_1_crossover,chromosome_2_crossover);
					index_i_of_two_new_chromosome = 0;
					index_j_of_two_new_chromosome = 0;
					for(int i = start;i<end;i++)
					{
						for(int  j = 0 ;j < 312; j ++)
						{
							new_population[i][j] = two_new_chromosomes[index_i_of_two_new_chromosome][index_j_of_two_new_chromosome++];
						}
						index_i_of_two_new_chromosome++;
						index_j_of_two_new_chromosome = 0;
					}

					start = start + 2;
					end = end+2;
					index_i_of_two_new_chromosome = 0;
					index_j_of_two_new_chromosome = 0;

					loop_control_variable--;
				
				}

				//			cout<<"5"<<endl;
				doing_mutation(new_population,mutation_rate,size_of_population);
				array_containing_fitness_of_new_chromosomes = new float[size_of_population]();


				for(int  i = 0 ; i <312 ; i++)
				{
					arry1[i] = 0;

				}
				z=0;
				k=0;
				for(int i=0;i<size_of_population;i++)
				{
					z = 0;
					k=0;
					for(int j=0;j<312;j++)
					{
						arry1[k++] = new_population[i][j];
					}
					array_containing_fitness_of_new_chromosomes[i] = (1/calculate_fitness(arry1));
				}

				//				cout<<"6"<<endl;

				//combining the 2 populations
				for(int i = 0 ; i <size_of_population;i++)
				{
					for(int j = 0; j<312 ; j++)
					{
						array_containing_both_new_and_old_population[i][j] = current_population[i][j];
					}
				}

				u = 0;
				for(int i = size_of_population ; i <(2*size_of_population);i++)
				{
					for(int j = 0;j<312 ;j++)
					{
						array_containing_both_new_and_old_population[i][j] = new_population[u][j];
					}
					u++;
				}


				//combining the fitnesses of the 2 populations
				array_containing_new_and_old_fitnesses = new float[2*size_of_population]();
				for(int  i = 0 ;i <size_of_population ; i++)
				{
					array_containing_new_and_old_fitnesses[i] = array_containing_fitness_of_chromosomes[i];
				}
				l = 0;
				for(int i = size_of_population;i<(2*size_of_population);i++)
				{
					array_containing_new_and_old_fitnesses[i]  = array_containing_fitness_of_new_chromosomes[l++];
				}

				//sorting the array which contains the fitness of old and new chromosomes
				//and also the corresponding chromosomes in array containing new and old 
				//chromosomes
				for(int  i = 0 ; i <(2*size_of_population)-1;i++ )
				{
					for(int j = 0;j<(2*size_of_population)-1-i;j++)
					{
						if(array_containing_new_and_old_fitnesses[j]<array_containing_new_and_old_fitnesses[j+1])
						{
							swap(array_containing_new_and_old_fitnesses[j],array_containing_new_and_old_fitnesses[j+1]);
							for(int k = 0 ; k < 312 ;k ++)
							{
								temp_for_storing_chromosome[k] = array_containing_both_new_and_old_population[j][k];
								array_containing_both_new_and_old_population[j][k] = array_containing_both_new_and_old_population[j+1][k];
								array_containing_both_new_and_old_population[j+1][k] = array_containing_both_new_and_old_population[j][k];

							}

						}
					}

				}

				int smallest_index;
				bool flag;
				if((best_fitness_required - array_containing_new_and_old_fitnesses[0])>0)
				{
					smallest = best_fitness_required - array_containing_new_and_old_fitnesses[0];
					smallest_index = 0;
					flag = true;
				}

				//updating the best chromosome so far 
				for(int  i = 1 ;i < (2*size_of_population);i++)
				{
					int temp = best_fitness_required - array_containing_new_and_old_fitnesses[i];
					if(temp > 0) 
					{
						if(temp<smallest)
						{
							smallest = temp;
							smallest_index = i;
							flag = true;


						}
					}
				}

				for(int  i = 0 ; i<312 ; i++)
				{
					best_chromosome_so_far[i] = array_containing_both_new_and_old_population[smallest_index][i];
					result_chromosome_1_index = smallest_index; 
				}





				//updating the population for next iteration

				for(int  i = 0 ; i <size_of_population ; i++)
				{
					for(int j = 0;j <312 ; j ++)
					{
						current_population[i][j]= array_containing_both_new_and_old_population[i][j];
					}
				}



				for(int i = 0 ; i <size_of_population;i++)
				{
					if(array_containing_fitness_of_new_chromosomes[i] >= best_fitness_required)
					{

						for(int  j = 0 ;j < 312 ; j ++)
						{
							result_choromosome[j] = new_population[i][j];
						}
						result_chromosome_2_index = i;
						break_flag = true;
						break;
					}
				}

				if(break_flag == true)
				{
					cout<<"Congratulations Required Fitness Chromosome found!"<<endl;
					for(int i = 0;i<312;i++)
					{
						cout<<result_choromosome[i]<<"|";
					}
					cout<<endl;

					cout<<"Fitness: "<<array_containing_fitness_of_new_chromosomes[result_chromosome_2_index]<<endl;
					cout<<"Iterations: "<<endl;
					cout<<(iterations - loop_control_variable_1)<<endl;
					cout<<"The Pictorial representation of the chromosome is "<<endl;

					break;
				}

				if(break_flag == false)
				{

					for(int i = 0 ; i <size_of_population;i++)
					{
						if(array_containing_fitness_of_chromosomes[i] >= best_fitness_required)
						{

							for(int  j = 0 ;j < 312 ; j ++)
							{
								result_choromosome_1[j] = current_population[i][j];
							}
							result_chromosome_3_index = i;
							break_flag_1 = true;
							break;
						}
					}

					if(break_flag_1 == true)
					{
						cout<<"Congratulations Required Fitness Chromosome found!"<<endl;
						for(int i = 0;i<312;i++)
						{
							cout<<result_choromosome_1[i]<<"|";
						}
						cout<<endl;

						cout<<"Fitness: "<<array_containing_fitness_of_chromosomes[result_chromosome_3_index]<<endl;
						cout<<"Iterations "<<endl;
						cout<<(iterations-loop_control_variable_1)<<endl;
						cout<<"The Pictorial representation of the chromosome is "<<endl;

						break;
					}

				}








				loop_control_variable_1--;

				if(loop_control_variable_1 == 0)
				{
					cout<<"Iterations ended"<<endl;
					cout<<"Best chromosome so far is "<<endl;
					for(int i = 0 ; i <312 ;i++)
					{
						cout<<best_chromosome_so_far[i]<<"|";
					}
					cout<<endl;

					cout<<"Fitness :"<<array_containing_new_and_old_fitnesses[result_chromosome_1_index]<<endl;

					cout<<endl;
					cout<<"The Pictorial Representation of the chromosome is !"<<endl;
					break;
				}
			}


		}
		else if(cross_over_criteria == 2)
		{
			while(loop_control_variable_1>0)
			{
		
				//				cout<<"1"<<endl;
				for(int i = 0 ; i <312 ;i++)
				{
					arr1[i] = 0;
				}
				k=0;
				for(int i=0;i<size_of_population;i++)
				{
					k = 0;
					for(int j=0;j<312;j++)
					{
						arr1[k++] = current_population[i][j];
					}
					array_containing_fitness_of_chromosomes[i] = (1/calculate_fitness(arr1));
				}

				//cout<<"2"<<endl;


	
				//Reulett Selection
				total_fitness = 0;
				for(int i = 0 ;i <size_of_population;i++)
				{
					total_fitness  = total_fitness + array_containing_fitness_of_chromosomes[i];
				}
						indexing_in_temp_for_current_population =0;
	
				while(reulett_loop_control_variable>0)
				{
	
					temp_random = random(1,total_fitness);
					for(int  i = 0 ;i < size_of_population ; i++)
					{
						temp_fitness = temp_fitness + array_containing_fitness_of_chromosomes[i];
						if(temp_fitness>temp_random)
						{
							chromosome_to_pick = i;
							break;
						}
					}

					
					for(int  i = 0 ; i <312;i++)
					{
						temp_for_current_population[indexing_in_temp_for_current_population][i] = current_population[chromosome_to_pick][i];
					}
					indexing_in_temp_for_current_population++;
					reulett_loop_control_variable--;
				}

								//cout<<"6"<<endl;
	
				for(int i = 0 ;i< size_of_population ;i++)
				{
					for(int j = 0;j<312;j++)
					{
						current_population[i][j] = temp_for_current_population[i][j];
					}

				}



				
				//in this loop i am creating the new population
				//based on the current population
				//using one point crossover


								loop_control_variable = size_of_population/2;
								start = 0;
								end = 2;
				while(loop_control_variable > 0)
				{
					//			cout<<"4"<<endl;

					random_number_1 = random(1,size_of_population-1);
					random_number_2 = random(1,size_of_population-1);

					for(int i = 0;i<312 ;i++)
					{
						chromosome_1_crossover[i] =current_population[random_number_1][i];
					}
					
					
					for(int i = 0;i<312 ;i++)
					{
						chromosome_2_crossover[i] =current_population[random_number_2][i];
					}
					
					
					


					two_new_chromosomes = two_point_crossover(chromosome_1_crossover,chromosome_2_crossover);
					index_i_of_two_new_chromosome = 0;
					index_j_of_two_new_chromosome = 0;
					for(int i = start;i<end;i++)
					{
						for(int  j = 0 ;j < 312; j ++)
						{
							new_population[i][j] = two_new_chromosomes[index_i_of_two_new_chromosome][index_j_of_two_new_chromosome++];
						}
						index_i_of_two_new_chromosome++;
						index_j_of_two_new_chromosome = 0;
					}

					start = start + 2;
					end = end+2;
					index_i_of_two_new_chromosome = 0;
					index_j_of_two_new_chromosome = 0;

					loop_control_variable--;
				
				}

				//			cout<<"5"<<endl;
				doing_mutation(new_population,mutation_rate,size_of_population);
				array_containing_fitness_of_new_chromosomes = new float[size_of_population]();


				for(int  i = 0 ; i <312 ; i++)
				{
					arry1[i] = 0;

				}
				z=0;
				k=0;
				for(int i=0;i<size_of_population;i++)
				{
					z = 0;
					k=0;
					for(int j=0;j<312;j++)
					{
						arry1[k++] = new_population[i][j];
					}
					array_containing_fitness_of_new_chromosomes[i] = (1/calculate_fitness(arry1));
				}

				//				cout<<"6"<<endl;

				//combining the 2 populations
				for(int i = 0 ; i <size_of_population;i++)
				{
					for(int j = 0; j<312 ; j++)
					{
						array_containing_both_new_and_old_population[i][j] = current_population[i][j];
					}
				}

				u = 0;
				for(int i = size_of_population ; i <(2*size_of_population);i++)
				{
					for(int j = 0;j<312 ;j++)
					{
						array_containing_both_new_and_old_population[i][j] = new_population[u][j];
					}
					u++;
				}


				//combining the fitnesses of the 2 populations
				array_containing_new_and_old_fitnesses = new float[2*size_of_population]();
				for(int  i = 0 ;i <size_of_population ; i++)
				{
					array_containing_new_and_old_fitnesses[i] = array_containing_fitness_of_chromosomes[i];
				}
				l = 0;
				for(int i = size_of_population;i<(2*size_of_population);i++)
				{
					array_containing_new_and_old_fitnesses[i]  = array_containing_fitness_of_new_chromosomes[l++];
				}

				//sorting the array which contains the fitness of old and new chromosomes
				//and also the corresponding chromosomes in array containing new and old 
				//chromosomes
				for(int  i = 0 ; i <(2*size_of_population)-1;i++ )
				{
					for(int j = 0;j<(2*size_of_population)-1-i;j++)
					{
						if(array_containing_new_and_old_fitnesses[j]<array_containing_new_and_old_fitnesses[j+1])
						{
							swap(array_containing_new_and_old_fitnesses[j],array_containing_new_and_old_fitnesses[j+1]);
							for(int k = 0 ; k < 312 ;k ++)
							{
								temp_for_storing_chromosome[k] = array_containing_both_new_and_old_population[j][k];
								array_containing_both_new_and_old_population[j][k] = array_containing_both_new_and_old_population[j+1][k];
								array_containing_both_new_and_old_population[j+1][k] = array_containing_both_new_and_old_population[j][k];

							}

						}
					}

				}

				int smallest_index;
				bool flag;
				if((best_fitness_required - array_containing_new_and_old_fitnesses[0])>0)
				{
					smallest = best_fitness_required - array_containing_new_and_old_fitnesses[0];
					smallest_index = 0;
					flag = true;
				}

				//updating the best chromosome so far 
				for(int  i = 1 ;i < (2*size_of_population);i++)
				{
					int temp = best_fitness_required - array_containing_new_and_old_fitnesses[i];
					if(temp > 0) 
					{
						if(temp<smallest)
						{
							smallest = temp;
							smallest_index = i;
							flag = true;


						}
					}
				}

				for(int  i = 0 ; i<312 ; i++)
				{
					best_chromosome_so_far[i] = array_containing_both_new_and_old_population[smallest_index][i];
					result_chromosome_1_index = smallest_index; 
				}





				//updating the population for next iteration

				for(int  i = 0 ; i <size_of_population ; i++)
				{
					for(int j = 0;j <312 ; j ++)
					{
						current_population[i][j]= array_containing_both_new_and_old_population[i][j];
					}
				}



				for(int i = 0 ; i <size_of_population;i++)
				{
					if(array_containing_fitness_of_new_chromosomes[i] >= best_fitness_required)
					{

						for(int  j = 0 ;j < 312 ; j ++)
						{
							result_choromosome[j] = new_population[i][j];
						}
						result_chromosome_2_index = i;
						break_flag = true;
						break;
					}
				}

				if(break_flag == true)
				{
					cout<<"Congratulations Required Fitness Chromosome found!"<<endl;
					for(int i = 0;i<312;i++)
					{
						cout<<result_choromosome[i]<<"|";
					}
					cout<<endl;

					cout<<"Fitness: "<<array_containing_fitness_of_new_chromosomes[result_chromosome_2_index]<<endl;
					cout<<"Iterations: "<<endl;
					cout<<(iterations - loop_control_variable_1)<<endl;
					cout<<"The Pictorial representation of the chromosome is "<<endl;

					break;
				}

				if(break_flag == false)
				{

					for(int i = 0 ; i <size_of_population;i++)
					{
						if(array_containing_fitness_of_chromosomes[i] >= best_fitness_required)
						{

							for(int  j = 0 ;j < 312 ; j ++)
							{
								result_choromosome_1[j] = current_population[i][j];
							}
							result_chromosome_3_index = i;
							break_flag_1 = true;
							break;
						}
					}

					if(break_flag_1 == true)
					{
						cout<<"Congratulations Required Fitness Chromosome found!"<<endl;
						for(int i = 0;i<312;i++)
						{
							cout<<result_choromosome_1[i]<<"|";
						}
						cout<<endl;

						cout<<"Fitness: "<<array_containing_fitness_of_chromosomes[result_chromosome_3_index]<<endl;
						cout<<"Iterations "<<endl;
						cout<<(iterations-loop_control_variable_1)<<endl;
						cout<<"The Pictorial representation of the chromosome is "<<endl;

						break;
					}

				}








				loop_control_variable_1--;

				if(loop_control_variable_1 == 0)
				{
					cout<<"Iterations ended"<<endl;
					cout<<"Best chromosome so far is "<<endl;
					for(int i = 0 ; i <312 ;i++)
					{
						cout<<best_chromosome_so_far[i]<<"|";
					}
					cout<<endl;

					cout<<"Fitness :"<<array_containing_new_and_old_fitnesses[result_chromosome_1_index]<<endl;

					cout<<endl;
					cout<<"The Pictorial Representation of the chromosome is !"<<endl;
					break;
				}
			}



		}
		else if(cross_over_criteria == 3)
		{
		while(loop_control_variable_1>0)
			{
		
				//				cout<<"1"<<endl;
				for(int i = 0 ; i <312 ;i++)
				{
					arr1[i] = 0;
				}
				k=0;
				for(int i=0;i<size_of_population;i++)
				{
					k = 0;
					for(int j=0;j<312;j++)
					{
						arr1[k++] = current_population[i][j];
					}
					array_containing_fitness_of_chromosomes[i] = (1/calculate_fitness(arr1));
				}

				//cout<<"2"<<endl;


	
				//Reulett Selection
				total_fitness = 0;
				for(int i = 0 ;i <size_of_population;i++)
				{
					total_fitness  = total_fitness + array_containing_fitness_of_chromosomes[i];
				}
						indexing_in_temp_for_current_population =0;
	
				while(reulett_loop_control_variable>0)
				{
	
					temp_random = random(1,total_fitness);
					for(int  i = 0 ;i < size_of_population ; i++)
					{
						temp_fitness = temp_fitness + array_containing_fitness_of_chromosomes[i];
						if(temp_fitness>temp_random)
						{
							chromosome_to_pick = i;
							break;
						}
					}

					
					for(int  i = 0 ; i <312;i++)
					{
						temp_for_current_population[indexing_in_temp_for_current_population][i] = current_population[chromosome_to_pick][i];
					}
					indexing_in_temp_for_current_population++;
					reulett_loop_control_variable--;
				}

								//cout<<"6"<<endl;
	
				for(int i = 0 ;i< size_of_population ;i++)
				{
					for(int j = 0;j<312;j++)
					{
						current_population[i][j] = temp_for_current_population[i][j];
					}

				}



				
				//in this loop i am creating the new population
				//based on the current population
				//using one point crossover


								loop_control_variable = size_of_population/2;
								start = 0;
								end = 2;
				while(loop_control_variable > 0)
				{
					//			cout<<"4"<<endl;

					random_number_1 = random(1,size_of_population-1);
					random_number_2 = random(1,size_of_population-1);

					for(int i = 0;i<312 ;i++)
					{
						chromosome_1_crossover[i] =current_population[random_number_1][i];
					}
					
					
					for(int i = 0;i<312 ;i++)
					{
						chromosome_2_crossover[i] =current_population[random_number_2][i];
					}
					
					
					


					two_new_chromosomes = uniform_point_crossover(chromosome_1_crossover,chromosome_2_crossover);
					index_i_of_two_new_chromosome = 0;
					index_j_of_two_new_chromosome = 0;
					for(int i = start;i<end;i++)
					{
						for(int  j = 0 ;j < 312; j ++)
						{
							new_population[i][j] = two_new_chromosomes[index_i_of_two_new_chromosome][index_j_of_two_new_chromosome++];
						}
						index_i_of_two_new_chromosome++;
						index_j_of_two_new_chromosome = 0;
					}

					start = start + 2;
					end = end+2;
					index_i_of_two_new_chromosome = 0;
					index_j_of_two_new_chromosome = 0;

					loop_control_variable--;
				
				}

				//			cout<<"5"<<endl;
				doing_mutation(new_population,mutation_rate,size_of_population);
				array_containing_fitness_of_new_chromosomes = new float[size_of_population]();


				for(int  i = 0 ; i <312 ; i++)
				{
					arry1[i] = 0;

				}
				z=0;
				k=0;
				for(int i=0;i<size_of_population;i++)
				{
					z = 0;
					k=0;
					for(int j=0;j<312;j++)
					{
						arry1[k++] = new_population[i][j];
					}
					array_containing_fitness_of_new_chromosomes[i] = (1/calculate_fitness(arry1));
				}

				//				cout<<"6"<<endl;

				//combining the 2 populations
				for(int i = 0 ; i <size_of_population;i++)
				{
					for(int j = 0; j<312 ; j++)
					{
						array_containing_both_new_and_old_population[i][j] = current_population[i][j];
					}
				}

				u = 0;
				for(int i = size_of_population ; i <(2*size_of_population);i++)
				{
					for(int j = 0;j<312 ;j++)
					{
						array_containing_both_new_and_old_population[i][j] = new_population[u][j];
					}
					u++;
				}


				//combining the fitnesses of the 2 populations
				array_containing_new_and_old_fitnesses = new float[2*size_of_population]();
				for(int  i = 0 ;i <size_of_population ; i++)
				{
					array_containing_new_and_old_fitnesses[i] = array_containing_fitness_of_chromosomes[i];
				}
				l = 0;
				for(int i = size_of_population;i<(2*size_of_population);i++)
				{
					array_containing_new_and_old_fitnesses[i]  = array_containing_fitness_of_new_chromosomes[l++];
				}

				//sorting the array which contains the fitness of old and new chromosomes
				//and also the corresponding chromosomes in array containing new and old 
				//chromosomes
				for(int  i = 0 ; i <(2*size_of_population)-1;i++ )
				{
					for(int j = 0;j<(2*size_of_population)-1-i;j++)
					{
						if(array_containing_new_and_old_fitnesses[j]<array_containing_new_and_old_fitnesses[j+1])
						{
							swap(array_containing_new_and_old_fitnesses[j],array_containing_new_and_old_fitnesses[j+1]);
							for(int k = 0 ; k < 312 ;k ++)
							{
								temp_for_storing_chromosome[k] = array_containing_both_new_and_old_population[j][k];
								array_containing_both_new_and_old_population[j][k] = array_containing_both_new_and_old_population[j+1][k];
								array_containing_both_new_and_old_population[j+1][k] = array_containing_both_new_and_old_population[j][k];

							}

						}
					}

				}

				int smallest_index;
				bool flag;
				if((best_fitness_required - array_containing_new_and_old_fitnesses[0])>0)
				{
					smallest = best_fitness_required - array_containing_new_and_old_fitnesses[0];
					smallest_index = 0;
					flag = true;
				}

				//updating the best chromosome so far 
				for(int  i = 1 ;i < (2*size_of_population);i++)
				{
					int temp = best_fitness_required - array_containing_new_and_old_fitnesses[i];
					if(temp > 0) 
					{
						if(temp<smallest)
						{
							smallest = temp;
							smallest_index = i;
							flag = true;


						}
					}
				}

				for(int  i = 0 ; i<312 ; i++)
				{
					best_chromosome_so_far[i] = array_containing_both_new_and_old_population[smallest_index][i];
					result_chromosome_1_index = smallest_index; 
				}





				//updating the population for next iteration

				for(int  i = 0 ; i <size_of_population ; i++)
				{
					for(int j = 0;j <312 ; j ++)
					{
						current_population[i][j]= array_containing_both_new_and_old_population[i][j];
					}
				}



				for(int i = 0 ; i <size_of_population;i++)
				{
					if(array_containing_fitness_of_new_chromosomes[i] >= best_fitness_required)
					{

						for(int  j = 0 ;j < 312 ; j ++)
						{
							result_choromosome[j] = new_population[i][j];
						}
						result_chromosome_2_index = i;
						break_flag = true;
						break;
					}
				}

				if(break_flag == true)
				{
					cout<<"Congratulations Required Fitness Chromosome found!"<<endl;
					for(int i = 0;i<312;i++)
					{
						cout<<result_choromosome[i]<<"|";
					}
					cout<<endl;

					cout<<"Fitness: "<<array_containing_fitness_of_new_chromosomes[result_chromosome_2_index]<<endl;
					cout<<"Iterations: "<<endl;
					cout<<(iterations - loop_control_variable_1)<<endl;
					cout<<"The Pictorial representation of the chromosome is "<<endl;

					break;
				}

				if(break_flag == false)
				{

					for(int i = 0 ; i <size_of_population;i++)
					{
						if(array_containing_fitness_of_chromosomes[i] >= best_fitness_required)
						{

							for(int  j = 0 ;j < 312 ; j ++)
							{
								result_choromosome_1[j] = current_population[i][j];
							}
							result_chromosome_3_index = i;
							break_flag_1 = true;
							break;
						}
					}

					if(break_flag_1 == true)
					{
						cout<<"Congratulations Required Fitness Chromosome found!"<<endl;
						for(int i = 0;i<312;i++)
						{
							cout<<result_choromosome_1[i]<<"|";
						}
						cout<<endl;

						cout<<"Fitness: "<<array_containing_fitness_of_chromosomes[result_chromosome_3_index]<<endl;
						cout<<"Iterations "<<endl;
						cout<<(iterations-loop_control_variable_1)<<endl;
						cout<<"The Pictorial representation of the chromosome is "<<endl;

						break;
					}

				}








				loop_control_variable_1--;

				if(loop_control_variable_1 == 0)
				{
					cout<<"Iterations ended"<<endl;
					cout<<"Best chromosome so far is "<<endl;
					for(int i = 0 ; i <312 ;i++)
					{
						cout<<best_chromosome_so_far[i]<<"|";
					}
					cout<<endl;

					cout<<"Fitness :"<<array_containing_new_and_old_fitnesses[result_chromosome_1_index]<<endl;

					cout<<endl;
					cout<<"The Pictorial Representation of the chromosome is !"<<endl;
					break;
				}
			

			}

		}
		else if(cross_over_criteria == 4)
		{
			while(loop_control_variable_1>0)
			{

				for(int i = 0 ; i <312 ;i++)
				{
					arr1[i] = 0;
				}
				k=0;
				for(int i=0;i<size_of_population;i++)
				{
					k = 0;
					for(int j=0;j<312;j++)
					{
						arr1[k++] = current_population[i][j];
					}
					array_containing_fitness_of_chromosomes[i] = (1/calculate_fitness(arr1));
				}

				//Reulett selection
				total_fitness = 0;

				for(int i = 0 ;i <size_of_population;i++)
				{
					total_fitness  = total_fitness + array_containing_fitness_of_chromosomes[i];
				}

				indexing_in_temp_for_current_population =0;

				index_i_of_two_new_chromosome =0;
				index_j_of_two_new_chromosome =0;
				while(reulett_loop_control_variable>0)
				{

					temp_random = random(1,total_fitness);
					for(int  i = 0 ;i < size_of_population ; i++)
					{
						temp_fitness = temp_fitness + array_containing_fitness_of_chromosomes[i];
						if(temp_fitness>temp_random)
						{
							chromosome_to_pick = i;
							break;
						}
					}


					for(int  i = 0 ; i <312;i++)
					{
						temp_for_current_population[indexing_in_temp_for_current_population][i] = current_population[chromosome_to_pick][i];
					}
					indexing_in_temp_for_current_population++;
					reulett_loop_control_variable--;
				}


				for(int i = 0 ;i< size_of_population ;i++)
				{
					for(int j = 0;j<312;j++)
					{
						current_population[i][j] = temp_for_current_population[i][j];
					}

				}



				//in this loop i am creating the new population
				//based on the current population
				//using one point crossover


				loop_control_variable = size_of_population/2;
				start = 0;
				end = 2;
				while(loop_control_variable > 0)
				{
					//			cout<<"4"<<endl;

					random_number_1 = random(1,size_of_population-1);
					random_number_2 = random(1,size_of_population-1);

					for(int i = 0;i<312 ;i++)
					{
						chromosome_1_crossover[i] =current_population[random_number_1][i];
					}


					for(int i = 0;i<312 ;i++)
					{
						chromosome_2_crossover[i] =current_population[random_number_2][i];
					}

				
					two_new_chromosomes = first_order_crossover(chromosome_1_crossover,chromosome_2_crossover);
					for(int i = start;i<end;i++)
					{
						for(int  j = 0 ;j < 312; j ++)
						{
							new_population[i][j] = two_new_chromosomes[index_i_of_two_new_chromosome][index_j_of_two_new_chromosome++];
						}
						index_i_of_two_new_chromosome++;
						index_j_of_two_new_chromosome = 0;
					}

					start = start + 2;
					end = end+2;
					index_i_of_two_new_chromosome = 0;
					index_j_of_two_new_chromosome = 0;

					loop_control_variable--;
				}
				
							cout<<"7"<<endl;
				doing_mutation(new_population,mutation_rate,size_of_population);
				array_containing_fitness_of_new_chromosomes = new float[size_of_population]();


				for(int  i = 0 ; i <312 ; i++)
				{
					arry1[i] = 0;

				}
				z=0;
				k=0;
				for(int i=0;i<size_of_population;i++)
				{
					z = 0;
					k=0;
					for(int j=0;j<312;j++)
					{
						arry1[k++] = new_population[i][j];
					}
					array_containing_fitness_of_new_chromosomes[i] = (1/calculate_fitness(arry1));
				}

				//				cout<<"6"<<endl;

				//combining the 2 populations
				for(int i = 0 ; i <size_of_population;i++)
				{
					for(int j = 0; j<312 ; j++)
					{
						array_containing_both_new_and_old_population[i][j] = current_population[i][j];
					}
				}

				u = 0;
				for(int i = size_of_population ; i <(2*size_of_population);i++)
				{
					for(int j = 0;j<312 ;j++)
					{
						array_containing_both_new_and_old_population[i][j] = new_population[u][j];
					}
					u++;
				}


				//combining the fitnesses of the 2 populations
				array_containing_new_and_old_fitnesses = new float[2*size_of_population]();
				for(int  i = 0 ;i <size_of_population ; i++)
				{
					array_containing_new_and_old_fitnesses[i] = array_containing_fitness_of_chromosomes[i];
				}
				l = 0;
				for(int i = size_of_population;i<(2*size_of_population);i++)
				{
					array_containing_new_and_old_fitnesses[i]  = array_containing_fitness_of_new_chromosomes[l++];
				}

				//sorting the array which contains the fitness of old and new chromosomes
				//and also the corresponding chromosomes in array containing new and old 
				//chromosomes
				for(int  i = 0 ; i <(2*size_of_population)-1;i++ )
				{
					for(int j = 0;j<(2*size_of_population)-1-i;j++)
					{
						if(array_containing_new_and_old_fitnesses[j]<array_containing_new_and_old_fitnesses[j+1])
						{
							swap(array_containing_new_and_old_fitnesses[j],array_containing_new_and_old_fitnesses[j+1]);
							for(int k = 0 ; k < 312 ;k ++)
							{
								temp_for_storing_chromosome[k] = array_containing_both_new_and_old_population[j][k];
								array_containing_both_new_and_old_population[j][k] = array_containing_both_new_and_old_population[j+1][k];
								array_containing_both_new_and_old_population[j+1][k] = array_containing_both_new_and_old_population[j][k];

							}

						}
					}

				}

				int smallest_index;
				bool flag;
				if((best_fitness_required - array_containing_new_and_old_fitnesses[0])>0)
				{
					smallest = best_fitness_required - array_containing_new_and_old_fitnesses[0];
					smallest_index = 0;
					flag = true;
				}

				//updating the best chromosome so far 
				for(int  i = 1 ;i < (2*size_of_population);i++)
				{
					int temp = best_fitness_required - array_containing_new_and_old_fitnesses[i];
					if(temp > 0) 
					{
						if(temp<smallest)
						{
							smallest = temp;
							smallest_index = i;
							flag = true;


						}
					}
				}

				for(int  i = 0 ; i<312 ; i++)
				{
					best_chromosome_so_far[i] = array_containing_both_new_and_old_population[smallest_index][i];
					result_chromosome_1_index = smallest_index; 
				}





				//updating the population for next iteration

				for(int  i = 0 ; i <size_of_population ; i++)
				{
					for(int j = 0;j <312 ; j ++)
					{
						current_population[i][j]= array_containing_both_new_and_old_population[i][j];
					}
				}



				for(int i = 0 ; i <size_of_population;i++)
				{
					if(array_containing_fitness_of_new_chromosomes[i] >= best_fitness_required)
					{

						for(int  j = 0 ;j < 312 ; j ++)
						{
							result_choromosome[j] = new_population[i][j];
						}
						result_chromosome_2_index = i;
						break_flag = true;
						break;
					}
				}

				if(break_flag == true)
				{
					cout<<"Congratulations Required Fitness Chromosome found!"<<endl;
					for(int i = 0;i<312;i++)
					{
						cout<<result_choromosome[i]<<"|";
					}
					cout<<endl;

					cout<<"Fitness: "<<array_containing_fitness_of_new_chromosomes[result_chromosome_2_index]<<endl;
					cout<<"Iterations: "<<endl;
					cout<<(iterations - loop_control_variable_1)<<endl;
					cout<<"The Pictorial representation of the chromosome is "<<endl;

					break;
				}

				if(break_flag == false)
				{

					for(int i = 0 ; i <size_of_population;i++)
					{
						if(array_containing_fitness_of_chromosomes[i] >= best_fitness_required)
						{

							for(int  j = 0 ;j < 312 ; j ++)
							{
								result_choromosome_1[j] = current_population[i][j];
							}
							result_chromosome_3_index = i;
							break_flag_1 = true;
							break;
						}
					}

					if(break_flag_1 == true)
					{
						cout<<"Congratulations Required Fitness Chromosome found!"<<endl;
						for(int i = 0;i<312;i++)
						{
							cout<<result_choromosome_1[i]<<"|";
						}
						cout<<endl;

						cout<<"Fitness: "<<array_containing_fitness_of_chromosomes[result_chromosome_3_index]<<endl;
						cout<<"Iterations "<<endl;
						cout<<(iterations-loop_control_variable_1)<<endl;
						cout<<"The Pictorial representation of the chromosome is "<<endl;

						break;
					}

				}








				loop_control_variable_1--;

				if(loop_control_variable_1 == 0)
				{
					cout<<"Iterations ended"<<endl;
					cout<<"Best chromosome so far is "<<endl;
					for(int i = 0 ; i <312 ;i++)
					{
						cout<<best_chromosome_so_far[i]<<"|";
					}
					cout<<endl;

					cout<<"Fitness :"<<array_containing_new_and_old_fitnesses[result_chromosome_1_index]<<endl;

					cout<<endl;
					cout<<"The Pictorial Representation of the chromosome is !"<<endl;
					break;
				}
			
			}
		
		}

	}



	system("pause");
	return 0;
}

