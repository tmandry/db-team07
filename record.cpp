#include "record.h"
#include <vector.h>

RecordIterator begin() const {
  RecordIterator it = Record::values_.begin();
  return it;
};

RecordIterator end() const {
  RecordIterator it = Record::values_.end();
  return it;
};


template <typename T>
T get(string field) {
	for(RecordIterator it = begin(); it != end(); it++) {
		
	}
};


template <typename T>
void set(string field, T value) {

};
