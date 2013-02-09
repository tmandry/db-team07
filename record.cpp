Record();

Record(vector<pair<string, string> > entries) {

};

~Record() {
  
};


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

};


template <typename T>
void set(string field, T value) {

};