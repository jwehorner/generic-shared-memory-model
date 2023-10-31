/**
 * 	@file		GenericSharedMemoryModel.hpp
 *	@brief		Definition of the GenericSharedMemoryModel class.
 *	@details	This header file defines the GenericSharedMemoryModel class for use in connecting 
				to a shared memory segment of arbitrary type and name. The class can be used by 
				instantiating an object with the type of the shared memory segment and its name, 
				then mapping the segment using the connect() method. The segment can then be 
				written to or read from using methods or by accessing the structure directly 
				through the data member.
 *	@author		James Horner
 */

#ifndef GENERIC_SHARED_MEMORY_MODEL_H
#define GENERIC_SHARED_MEMORY_MODEL_H

// C++ Standard Library Headers
#include <cstring>
#include <mutex>
#include <string>

// Platform Dependant System Libraries
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <tchar.h>
#else
#include <fcntl.h>      // Needed for read/write definitions
#include <unistd.h>     // Needed for close()
#include <sys/mman.h>   // For POSIX shared memory via shm_open
#include <sys/stat.h>
#endif

/**
 * @brief 	Class GenericSharedMemoryModel is used for management of a connection to a shared memory segment of any type.
 * @details Class GenericSharedMemoryModel provides an easy to use interface with a generic shared memory segment that can be used 
 * 			in any C++ application. The class has functions for connecting to and disconnecting from shared memory along with a function for 
 * 			taking a snapshot of the shared memory segment at any given time. Because the structure that is mapped to shared memory is 
 * 			also public, it can be interacted with directly by the user and therefore can be used to set values in shared memory, unlike 
 * 			the struct returned be get_data().
 * @param 	T datatype of the shared memory segment to connect to.
 */
template<typename T>
class GenericSharedMemoryModel {
public:
    /// Constructor for the GenericSharedMemoryModel class that initialises members, but does not connect shared memory.
    GenericSharedMemoryModel(const std::string name, const bool log_warnings = false) : 
        m_name(name),
        m_log_warnings(log_warnings)
    {
        m_is_connected = false;
    }
	
    /// Destructor for the GenericSharedMemoryModel class that disconnects from shared memory if the object is deleted.
    ~GenericSharedMemoryModel() 
    {
        disconnect();
    }

    /**
     * @brief Function connect() is used to connect the GenericSharedMemoryModel object to the shared memory segment.
     * @returns Boolean true when the shared memory segment was successfully connected, false otherwise.
     */
    bool connect();

    /**
     * @brief Function disconnect() is used to disconnect the GenericSharedMemoryModel object from the shared memory segment.
     * @returns Boolean true when the shared memory segment was successfully disconnected, false otherwise.
     */
    bool disconnect();

    /**
     * @brief Function is_connected() is used to check if the shared memory is connected.
     * @returns Boolean true when the shared memory is connected, false otherwise.
     */
    bool is_connected() {
		// Gain access to the member mutex.
		std::scoped_lock<std::mutex> member_guard(m_member_lock);
		return m_is_connected;
	};

    /**
     * @brief Function get_data() is used to get a read only snapshot of the shared memory segment.
     * @returns T structure that is a snapshot of the shared memory segment at the time of the function call.
     * @note While data is public, it would be best to use get_data() if read only access is needed to shared memory.
     */
    T get_data() {
		// Gain access to the member mutex.
		std::scoped_lock<std::mutex> member_guard(m_member_lock);
		return *data;
	};

    /**
     * @brief Function write_data() is used to write a new value of the T into the shared memory segment.
     * @param  new_data T structure to be written into the shared memory segment.
     */
	void write_data(const T new_data) {
		// Gain access to the member mutex.
		std::scoped_lock<std::mutex> member_guard(m_member_lock);
		memcpy(data, &new_data, sizeof(T));
	}

    /// Public member for the structure that is mapped to the shared memory segment upon the calling of connect().
    T* data;

private:
    /// Private member for the status of the connection to shared memory.
    bool m_is_connected;
    /// Flag for if warnings should be logged to the console (instead of just flagged in return values).
    bool m_log_warnings;
    /// Name of the shared memory segment.
    std::string m_name;
	/// Mutex lock to protect the members of the class when accessing concurrently.
	std::mutex m_member_lock;

#ifdef WIN32
    /// Handle to file mapping object for the connection to shared memory in Windows.
    HANDLE m_file_mapping_handle;
#else
    // ID of the shared memory segment in a UNIX environment.
    int m_file_mapping_handle;
#endif

};

template<typename T>
bool GenericSharedMemoryModel<T>::connect()
{
	// Gain access to the member mutex.
	std::scoped_lock<std::mutex> member_guard(m_member_lock);

    // If shared memory is not connected already,
    if(!m_is_connected) {
#ifdef _WIN32
        // Store the value of the handle returned by CreateFileMappingA so it can be error-checked.
		// 2023-02-14 JH:	Force use of CreateFileMappingA as on some platforms CreateFileMapping 
		//					expands to CreateFileMappingW requiring a wide string which a std::string
		// 					is not always compatible with without conversion.
        m_file_mapping_handle = CreateFileMappingA(                               
			INVALID_HANDLE_VALUE,		// Create new file mapping object.
			NULL,	                    // default security
			PAGE_READWRITE,		        // read/write access
			0,						    // maximum object size (high-order DWORD)
			sizeof(T),		// maximum object size (low-order DWORD)
			m_name.c_str());				// name of mapping object 
        
        // If the handle is invalid,
        if (m_file_mapping_handle == NULL){
            // Return failure.
            if (m_log_warnings) {
                printf("Couldn't connect to shared memory with name: %s\n", m_name.c_str());
            }
            m_is_connected = false;
            return false;
        }

        // If the handle is valid, try to map the handle to the T structure.
		// Cast the new file to the struct from FRL types so we can read/write easily.
        data = (T*)MapViewOfFile(
			m_file_mapping_handle,            // assign the map object to the shared data struct.
			FILE_MAP_ALL_ACCESS, // read/write permission
			0,
			0,
			sizeof(T));

        // If the mapping returned an invalid memory location,
        if (data == NULL){
            // Close the file handle, and return failure.
            if (m_log_warnings) {
                printf("Couldn't map view of file to shared memory with name: %s\n", m_name.c_str());
            }
            CloseHandle(m_file_mapping_handle);
            m_is_connected = false;
            return false;
        }        

        // Set the connection state to true.
        m_is_connected = true;
#else
        // Get an ID for the shared memory segment with the name.
        m_file_mapping_handle = shm_open(m_name.c_str(), O_CREAT | O_RDWR, 0666); // Create file descriptor for shared mem. Create the mem if it doesn't already exist. Set permissions to 666

        // If the ID is invalid,
        if (m_file_mapping_handle < 0) {
            // Could not create the shared memory file descriptor via shm_open()
            if (m_log_warnings) {
                // Print an error message and return failure.
                printf("Couldn't connect to shared memory with name: %s\n", m_name.c_str());
            }
			m_is_connected = false;
            return false;
        }

        // Check if this is the first time the segment is being opened, and if so try to truncate it.
        struct stat mapping_stat;
        if (fstat(m_file_mapping_handle, &mapping_stat) != -1 && mapping_stat.st_size == 0) {
            // Try to truncate the file mapping handle to the correct size.
            if (ftruncate(m_file_mapping_handle, sizeof (T)) != 0) {
                // Could not truncate shared memory to the correct size.
                if (m_log_warnings) {
                    // Print an error message and return failure.
                    printf("Couldn't truncate shared memory because of error: %d\n", errno);
                }
                m_is_connected = false;
                return false;
            }
        }

        // Try to map the shared memory segment to a T structure.
        data = (T *) mmap(NULL, sizeof(T), PROT_READ | PROT_WRITE, MAP_SHARED, m_file_mapping_handle, 0);

        // If the mapping is unsuccessful,
        if (data == MAP_FAILED) {
            // Close the file handle, and return failure.
            if (m_log_warnings) {
                printf("Couldn't map view of file to shared memory with name: %s\n", m_name.c_str());
            }
            m_is_connected = false;
            return false;
        }

        // Set the connection state to true.
        m_is_connected = true;
#endif
    }
    // If already connected or the connection process completed without returning false,
    // Return successfully.
    return m_is_connected;
}

template<typename T>
bool GenericSharedMemoryModel<T>::disconnect()
{
	// Gain access to the member mutex.
	std::scoped_lock<std::mutex> member_guard(m_member_lock);

    // If shared memory is connected currently,
    if(m_is_connected){
#ifdef _WIN32
        UnmapViewOfFile(data);
        CloseHandle(m_file_mapping_handle);
        m_is_connected = false;
#else
        munmap(data, sizeof (T));
        close(m_file_mapping_handle);
        m_is_connected = false;
#endif
    }
    return true;
}

#endif /* GENERIC_SHARED_MEMORY_MODEL_H */
