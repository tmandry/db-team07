#include "record.h"

#include <sstream>

Record::Record() {
  values_ = vector<pair<string, string > >();
}

Record::Record(vector<pair<string, string> > entries) {
  values_ = entries;
}

Record::~Record() {
  delete &values_;
}

Record::RecordIterator Record::begin() const {
  return values_.begin();
}

Record::RecordIterator Record::end() const {
  return values_.end();
}

void Record::join(const Record& other) {
  values_.insert(values_.end(), other.values_.begin(), other.values_.end());
}
