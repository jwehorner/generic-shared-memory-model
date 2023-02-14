# Generic Shared Memory Model

## Contents

* [About](#about)
* [Usage](#usage)
* [Contact](#contact)

## About

The Generic Shared Memory Model is a generic C++ class which provides an interface to an arbitrary shared memory segment, whose type is specified by the template argument and whose string name is provided to the constructor. The class provides methods for connecting to and disconnecting from the shared memory segment as well as for reading from and writing to the entire segment. References to the segment are stored internally, so only a reference to the object is required to access the segment.

## Usage

An example of how to use the class to access a shared memory segment of type float is as follows:
```c++
#include <GenericSharedMemoryModel.hpp>

int main() {
	// Instantiate the model with the required type and specify the name of the shared memory segment.
	GenericSharedMemoryModel<float> *model = new GenericSharedMemoryModel<float>("SharedMemoryName");

	// Connect the shared memory segment.
	model->connect();

	// If the segment was connected successfully,
	if (model->is_connected()) {

		// Print the current value.
		std::cout << "Model connected." << std::endl;
		std::cout << "Current value: " 	<< std::to_string(model->get_data()) << std::endl;

		// Change the value in the segment.
		model->write_data(42.42); // Could also be done using model->data = 42.42

		// Print the new value.
		std::cout << "New value: " 		<< std::to_string(model->get_data()) << std::endl;
	}
	// Else if the segment did not connect successfully,
	else {
		// Print an error and return.
		std::cout << "Model could not connect." << std::endl;
		return -1;
	}

	// Disconnect from the shared memory segment then exit.
	// Disconnect is also called in the destructor for the class to it is not calamitous if you forget.
	model->disconnect();
	return 0;
}
```

## Contact

James Horner - jwehorner@gmail.com or James.Horner@nrc-cnrc.gc.ca
