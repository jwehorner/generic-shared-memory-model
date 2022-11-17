#ifndef SHAREDMEMORYMODEL_H
#define SHAREDMEMORYMODEL_H

#include <string>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <tchar.h>
#else
#include <fcntl.h>      //Needed for read/write definitions
#include <unistd.h>     //Needed for close()
#include <sys/mman.h>   //For POSIX shared memory via shm_open
#endif

//Object Oriented model of shared memory which provides methods for connecting/disconnecting as well as state variables.
/**
 * @brief 	Class GenericSharedMemoryModel is used for management of a connection to a shared memory segment of any type.
 * @details Class GenericSharedMemoryModel provides an easy to use interface with a generic shared memory segment that can be used 
 * 			in any C++ application. The class has functions for connecting to and disconnecting from shared memory along with a function for 
 * 			taking a snapshot of the shared memory segement at any given time. Because the structure that is mapped to shared memory is 
 * 			also public, it can be interacted with directly by the user and therefore can be used to set values in shared memory, unlike 
 * 			the struct returned be get_data().
 * @param 	T datatype of the shared memory segment to connect to.
 */
template<typename T>
class GenericSharedMemoryModel {
public:
    ///Constructor for the GenericSharedMemoryModel class that initialises members, but does not connect shared memory.
    GenericSharedMemoryModel(const std::string name) : name(name) {m_is_connected = false;};
	
    ///Destructor for the GenericSharedMemoryModel class that disconnects from shared memory if the object is deleted.
    ~GenericSharedMemoryModel() {disconnect();};

    /**
     * @brief Function connect() is used to connect the GenericSharedMemoryModel object to the shared memory segment.
     * @returns Boolean true when the shared memory segement was successfully connected, false otherwise.
     */
    bool connect();

    /**
     * @brief Function disconnect() is used to disconnect the GenericSharedMemoryModel object from the shared memory segment.
     * @returns Boolean true when the shared memory segement was successfully disconnected, false otherwise.
     */
    bool disconnect();

    /**
     * @brief Function is_connected() is used to check if the shared memory is connected.
     * @returns Boolean true when the shared memory is connected, false otherwise.
     */
    bool is_connected() const {return m_is_connected;};

    /**
     * @brief Function get_data() is used to get a read only snapshot of the shared memory segement.
     * @returns T structure that is a snapshot of the shared memory segement at the time of the function call.
     * @note While data is public, it would be best to use get_data() if read only access is needed to shared memory.
     */
    T get_data() const {return *data;};

    /**
     * @brief Function write_data() is used to write a new value of the T into the shared memory segement.
     * @param  new_data T structure to be written into the shared memory segement.
     */
	void write_data(const T new_data) const {memcpy(data, &new_data, sizeof(T));}

    ///Public member for the structure that is mapped to the shared memory segement upon the calling of connect().
    T* data;

private:
    ///Private member for the status of the connection to shared memory.
    bool m_is_connected;
    /**
     * @brief Name of the shared memory segement.
     */
    std::string name;

#ifdef WIN32
    ///Handle to file mapping object for the connection to shared memory in Windows.
    HANDLE hMapFile;
#else
    //ID of the shared memory segment in a UNIX environment.
    int hMapFile;
#endif

};

template<typename T>
bool GenericSharedMemoryModel<T>::connect()
{
    //If shared memory is not connected already,
    if(!m_is_connected) {
#ifdef _WIN32
        //Store the value of the handle returned by CreateFileMapping so it can be error-checked.
        hMapFile = CreateFileMapping(                               
			INVALID_HANDLE_VALUE,		// Create new file mapping object.
			NULL,	                    // default security
			PAGE_READWRITE,		        // read/write access
			0,						    // maximum object size (high-order DWORD)
			sizeof(T),		// maximum object size (low-order DWORD)
			name.c_str());				// name of mapping object 
        
        //If the handle is invalid,
        if (hMapFile == NULL){
            //Print an error message and return failure.
            printf("Couldn't connect to shared memory with name: %s\n", name.c_str());
            m_is_connected = false;
            return false;
        }

        //If the handle is valid, try to map the handle to the T structure.
		//Cast the new file to the struct from FRL types so we can read/write easily.
        data = (T*)MapViewOfFile(
			hMapFile,            //assign the map object to the shared data struct.
			FILE_MAP_ALL_ACCESS, //read/write permission
			0,
			0,
			sizeof(T));

        //If the mapping returned an invalid memory location,
        if (data == NULL){
            //Print an error message, close the file handle, and return failure.
            printf("Couldn't map view of file to shared memory with name: %s\n", name.c_str());
            CloseHandle(hMapFile);
            m_is_connected = false;
            return false;
        }        

        //Set the connection state to true.
        m_is_connected = true;
#else
        //Get an ID for the shared memory segement with the name.
        hMapFile = shm_open(sharedMemName, O_CREAT | O_RDWR, 0666); //Create file descriptor for shared mem. Create the mem if it doesn't already exist. Set permissions to 666

        //If the ID is invalid,
        if (hMapFile < 0) {
            //Could not create the shared memory file descriptor via shm_open()
            //Print an error message and return failure.
            printf("Couldn't connect to shared memory with name: %s\n", name.c_str());
			m_is_connected = false;
            return false;
        }

        //Try to map the shared memory segement to a T structure.
        ftruncate(hMapFile, sizeof (T));
        data = (T *) mmap(NULL, sizeof(T), PROT_READ | PROT_WRITE, MAP_SHARED, hMapFile, 0);

        //If the mapping is unsuccessful,
        if (data == MAP_FAILED) {
            //Print an error message, close the file handle, and return failure.
            printf("Couldn't map view of file to shared memory with name: %s\n", name.c_str());
            m_is_connected = false;
            return false;
        }

        //Set the connection state to true.
        m_is_connected = true;
#endif
    }
    //If already connected or the connection process completed without returning false,
    //Return successfully.
    return m_is_connected;
}

template<typename T>
bool GenericSharedMemoryModel<T>::disconnect()
{
    //If shared memory is connected currently,
    if(m_is_connected){
#ifdef _WIN32
        UnmapViewOfFile(data);
        CloseHandle(hMapFile);
        m_is_connected = false;
#else
        munmap(data, sizeof (T));
        close(hMapFile);
        m_is_connected = false;
#endif
    }
    return true;
}

#endif // SHAREDMEMORYMODEL_H
