/*-----------------------------------------------
 * Mp3 ID3v1 Tag Editor (Version 1.0)
 * Programmed by Mike Rosenberg
 * Group: Mike Rosenberg, James Chau, Ryan Wegner
 * Final Project for Professor Sengupta's CS240
 * Completed 4/15/2013
 * http://mikerosenberger.com
 *---------------------------------------------*/
#include <iostream>
#include <fstream>  
#include <string> 
using namespace std;
//Create a Structure to store MP3s tagged info
struct MP3Tag
{
	string Artist;
	string Title;
	string Album;
	string Year;
	string Comment;
};
//Add Functions to be used
int getsize(FILE *file);
bool checkmp3(string filename);
string ReadThirty(FILE *file, int readloc);
string Header(FILE *file, int readloc);
string Year(FILE *file, int readloc);
void ReadMP3(FILE *file, MP3Tag tag);
void WriteArtist(FILE *file, int filesize, char Artist[]);
void WriteTitle(FILE *file, int filesize, char Title[]);
void WriteAlbum(FILE *file, int filesize, char Album[]);
void WriteYear(FILE *file, int filesize, char Year[]);
void WriteComment(FILE *file, int filesize, char Comment[]);
//Begin the program
int main(int argc, char *argv[])  
{
	string fileloc = "";
	if(argc<2)  
	{  
		cout<<"Drag and drop MP3 files to the Application!"<<endl;  
		//If not file dragged to exe warn user
		cin.get(); 
		return 0;  
	}  
	else
	{
		FILE *ifile;
		for(short i=1;i<argc;i++)  
		{  
			//If file dragged attempt to open file for Reading
			cout<<"Opening File: "<<argv[i]<<endl;
			fileloc = argv[i];
			ifile = fopen(argv[i], "r+");
		}
		//Assuming Opening has initiated:
		if (ifile)
		{
			if (!checkmp3(fileloc))
			{
				cout << "ERROR! File Is Not An MP3 and Can Not Be Read. Please Only Use MP3 Files"<<endl;
				//If File Extension is not .mp3 file clearly cannot be read/edited. Warn user and close
				cin.get();
				fclose(ifile);
				return 0;
			}
			else
			{
				cout << "File " <<fileloc<<" Opened Successfully"<<endl; //If .mp3 opened successfully
				int readloc = getsize(ifile)-128; //Go to byte location of information
				int filesize = getsize(ifile);//stores the full file size needed for writing data later
				string tag = Header(ifile, readloc); //read The Tag to make sure the audio file is tagged
				if (tag == "TAG")
				{
					cout<<"File Has A Valid MP3 Tag And Will Be Read Now"<<endl<<endl;
					readloc+=3; //move past Tag location to next information
					MP3Tag tag; //create an MP3 Structure
					//Calls Function to read all data from the MP3 file
					ReadMP3(ifile, tag);
					//At this point the user will be prompted with options to continue
					cout << "You May Now Edit the File's Tags" <<endl;
					cout << "0: Exit Program"<<endl;
					cout << "1. Edit Artist"<<endl;
					cout << "2. Edit Title"<<endl;
					cout << "3. Edit Album"<<endl;
					cout << "4. Edit Year"<<endl;
					cout << "5. Edit Comment"<<endl;
					cout << "6. ReRead Info"<<endl<<endl;
					short r, reboot=0; //reboot to keep options in a loop, r to accept input
					while (reboot<1)
					{
					cout << "Please Enter Your Choice And Then Press Enter: " ;
					cin>>r;
					switch (r)  //switch-case to set what happens on user input
						{
							case 0:
								fclose(ifile);
								return (0); //if close option end the program after closing the file
								break;
							case 1:
								char artist [31];
								cout << "Enter Desired Artist Name"<<endl;
								cin.ignore(); //sets break time for user to enter input
								cin.getline(artist, 31); //1 byte larger then needed to make up for enter key press
								WriteArtist(ifile, filesize, artist); //calls write function
								break;
							case 2:
								char title[31];
								cout << "Enter Desired Title"<<endl;
								cin.ignore();
								cin.getline(title, 31);
								WriteTitle(ifile, filesize, title); //same info as case 1
								break;
							case 3:
								char album[31];
								cout << "Enter Desired Album Title"<<endl;
								cin.ignore();
								cin.getline(album, 31);
								WriteAlbum(ifile, filesize, album);//same info as case 1
								break;
							case 4:
								char year[5];
								cout << "Enter Desired Release Year"<<endl;
								cin.ignore();
								cin.getline(year, 5);
								WriteYear(ifile, filesize, year); //same info as case 1
								break;
							case 5:
								char comment[31];
								cout << "Enter Desired Comments"<<endl;
								cin.ignore();
								cin.getline(comment, 31);
								WriteComment(ifile, filesize, comment); //same info as case 1
								break;
							case 6:
								cout <<endl;
								ReadMP3(ifile, tag); //If user enters 6 rereads mp3 info
								break;
							default:
								cout << "The Number You Entered Is Not A Valid Option. Please Try Again" <<endl;
								break; //if not option gives error message
						}
					}
					cin.get();
					fclose(ifile);//closes file and ends program
					return (0);
				}
				else
				{
					cout << "File Is Not Tagged With Information. Please Try A Different MP3"<<endl;
					cin.get();
					fclose(ifile);// If file is not tagged gives error message, closes file and ends program
					return 0;
				}
			}
		}
		else
		{
			cout << "Error Opening File. Press The Enter Key To Exit" <<endl;
			cin.get(); //If opening error end program
			return 0;
		}
		cin.get();
	}
return(0);
}

// Functions used by the program begin here: 
// *****************************************

int getsize(FILE *file)
{
	//returns the size of the file as an int
	int loc = ftell(file);
	fseek(file,0,SEEK_END);
	int size = ftell(file);
	fseek(file, loc, SEEK_SET);
	return size;
}

bool checkmp3(string filename)
{
	//Checks if the file extension is mp3
	int filetype = filename.length() - 4;
	string filetipe = filename.substr(filetype, 4);
	if (filetipe == ".mp3")
	{
		return true;
	}
	else
	{
		return false;
	}
}

string Header(FILE *file, int readloc)
{
	//Checks for ID3v1 Tag Header 4bytes long...
	//currently only supports v1
	//v1+ coming later
	char magic[4];
	fseek(file , readloc, SEEK_SET);
	fread(magic, 1, 4, file);
	string str(magic);
	string mag = str.substr(0,3);
	return mag;
}
string Year(FILE *file, int readloc)
{
	//Reads Year Of Track...4bytes long
	char magic[4];
	fseek(file , readloc, SEEK_SET);
	fread(magic, 1, 4, file);
	string str(magic);
	return str;
}
string ReadThirty(FILE *file, int readloc)
{
	//Used to read Title, Artist, Album, Comment
	//Each is 30 bytes long
	char magic[30];
	fseek(file , readloc, SEEK_SET);
	fread(magic, 1, 30, file);
	string str(magic);
	return str;
}
void ReadMP3(FILE *file, MP3Tag tag)
{
	//The following code reads the information for x bytes and then increases the readlocation x spaces
	//so the next information can be read
	int readloc = getsize(file)-125;
	tag.Title = ReadThirty(file, readloc);
	readloc+=30;
	tag.Artist = ReadThirty(file, readloc);
	readloc+=30;
	tag.Album = ReadThirty(file, readloc);
	readloc+=30;
	tag.Year = Year(file, readloc);
	readloc+=4;
	tag.Comment = ReadThirty(file, readloc);
	cout << "Artist: " <<tag.Artist<<endl;
	cout << "Title: " <<tag.Title<<endl;
	cout << "Album: "<<tag.Album<<endl;
	cout << "Year: "<<tag.Year<<endl;
	cout << "Comment: "<<tag.Comment<<endl<<endl;
}
void WriteArtist(FILE *file, int filesize, char Artist[])
{
	int writeloc = filesize-95;
	fseek(file, writeloc, SEEK_SET);
	fwrite(Artist, 1, 30, file); //goes to the storage of Artist data and writes new data
	cout << Artist << " set as The Artist"<<endl;
}
void WriteTitle(FILE *file, int filesize, char Title[])
{
	int writeloc = filesize-125;
	fseek(file, writeloc, SEEK_SET);
	fwrite(Title, 1, 30, file); //goes to the storage of Title data and writes new data
	cout << Title << " set as The Title"<<endl;
}
void WriteAlbum(FILE *file, int filesize, char Album[])
{
	int writeloc = filesize-65;
	fseek(file, writeloc, SEEK_SET);
	fwrite(Album, 1, 30, file); //goes to the storage of Album data and writes new data
	cout << Album << " set as The Album"<<endl;
}
void WriteYear(FILE *file, int filesize, char Year[])
{
	int writeloc = filesize-35;
	fseek(file, writeloc, SEEK_SET);
	fwrite(Year, 1, 4, file); //goes to the storage of Year data and writes new data
	cout << Year << " set as The Release Year"<<endl;
}
void WriteComment(FILE *file, int filesize, char Comment[])
{
	int writeloc = filesize-31;
	fseek(file, writeloc, SEEK_SET);
	fwrite(Comment, 1, 30, file); //goes to the storage of Comment data and writes new data
	cout << Comment << " set as The Comment"<<endl;
}



