#include "p2Defs.h"
#include "j1App.h"
#include "p2Log.h"
#include "j1AssetManager.h"
#include "PhysFS/include/physfs.h"
#include "SDL/include/SDL.h"

#pragma comment( lib, "PhysFS/lib/physfs.lib" )

j1AssetManager::j1AssetManager() : j1Module()
{
	name = ("asset_manager");

	PHYSFS_init(nullptr);	//Initialize PhysFS
	CreatePath(".");		//Create a default path
}

// Destructor
j1AssetManager::~j1AssetManager()
{
	PHYSFS_deinit();		//Deinitialize PhysFS to close all the path and free memory
}

bool j1AssetManager::Awake(pugi::xml_node& config)
{
	LOG("Loading Asset Manager");


	PHYSFS_addToSearchPath("Assets.zip", 1);	//Set the archive that we will read from

	return true;
}

bool j1AssetManager::CreatePath(const char* newDir, const char* mount_point)
{
	bool ret = true;
	
	if (PHYSFS_addToSearchPath(newDir, 1) == 0)		//Mount an archive file into the virtual filesystem
		LOG("Error: %s\n", PHYSFS_getLastError());
	else
		ret = false;

	return ret;
}

// Returns true if the file in path exists
bool j1AssetManager::Exists(const char* file) const
{
	return PHYSFS_exists(file);
}

//  Allocate needed bytes to a buffer and writes all data, returns size of the buffer
uint j1AssetManager::LoadData(const char* file, char** buffer) const
{
	uint ret = 0;

	PHYSFS_file* data_file = PHYSFS_openRead(file);										//Open the file in the path to read

	if (data_file != nullptr)
	{
		int file_lenght = PHYSFS_fileLength(data_file);									//Get the files size
		*buffer = new char[(uint)file_lenght];											//Allocate a buffer of the files size
		uint readed = PHYSFS_read(data_file, *buffer, 1, (uint)file_lenght);			//Read the data
		if (readed != file_lenght)														//The bytes read should be the same as the file size
		{	
			LOG("Error while reading from file %s: %s\n", file, PHYSFS_getLastError());
			RELEASE(buffer);															//Delete the buffer
		}
		else
			ret = readed;																//Return the amout of bytes read
				
		PHYSFS_close(data_file);														//Close the file handle
	}
	else
		LOG("Error while opening file %s: %s\n", file, PHYSFS_getLastError());

	return ret;
}

// Loads an image/fx/music from a memory buffer with SDL
SDL_RWops* j1AssetManager::Load(const char* file) const
{
	char* buffer;
	int size = LoadData(file, &buffer); //We get the size of the data and load it in a buffer

	if (size > 0)
	{
		SDL_RWops* r = SDL_RWFromConstMem(buffer, size); //We prepare a read-only buffer
		
		return r;
	}
	else
		return nullptr;
}

pugi::xml_parse_result j1AssetManager::LoadXML(pugi::xml_document &data_file, const char* path)
{
	char* buffer;
	uint file_size = App->assets->LoadData(path, &buffer);
	pugi::xml_parse_result result = data_file.load_buffer(buffer, file_size); //We load the buffer as an immutable one
	RELEASE(buffer)

	return result;
}
