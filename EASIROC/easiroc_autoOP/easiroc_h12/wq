#include<iostream>
#include<string>

void fullcheck(const char* directory_name, const char* file_name, int data_num){
	std::stringstream new_directory;
	new_directory<<"mkdir /home/smcha2019/EASIROC/Data/"<<directory_name;
	std::string New_Directory = new_directory.str();
	system(New_Directory.c_str());
	for(int num=0; num<data_num; num++){
		std::stringstream Name;
		std::string name;
		Name<<"./"<<directory_name<<"/"<<file_name<<num<<".dat";
		std::cout<<Name.str()<<std::endl;
		fullcheck6(Name.str());
	}
}

