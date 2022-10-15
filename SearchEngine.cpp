#include <bits/stdc++.h>

using namespace std;

class WebPage {
  string name;
  int impression, clicked;
  double CTR, PR, score, PRtrial;

public:
  vector<string> keywords;
  vector<WebPage *> ToWeb;
  vector<WebPage *> FromWeb;
  WebPage() {
    name = "";
    impression = 0;
    clicked = 0;
    CTR = 0.0;
    PR = 0.0;
    score = 0.0;
    PRtrial = 0.0;
    keywords.resize(0);
    ToWeb.resize(0);
    FromWeb.resize(0);
  }
  void set_name_keywords(string nameUpdate, vector<string> keywords_para) {
    name = nameUpdate;
    keywords = keywords_para;
  }
  void set_Impression(int n) { impression = n; }
  void set_clicked(int n) { clicked = n; }
  void set_PR(double n) { PR = n; }
  void set_PRtrial(double n) { PRtrial = n; }
  void increment_click() { clicked++; }
  void increment_impression() { impression++; }
  void set_CTR() { CTR = (double(clicked) / double(impression)) * 100.0; }
  void set_score() {
    double q = (0.1 * impression) / (1.0 + 0.1 * impression);
    double first = (1.0 - q) * PR;
    double second = q * CTR;
    double addi = 0.4 * PR;
    score = addi + ((first + second) * 0.6);
  }
  void set_manual_score() { score = 0.0; }
  string get_name() { return name; }
  int get_impression() { return impression; }
  int get_clicked() { return clicked; }
  double get_CTR() { return CTR; }
  double get_PR() { return PR; }
  double get_PRtrial() { return PRtrial; }
  double get_score() { return score; }
};

bool startFace(vector<WebPage> &arr);
void searchFace(vector<WebPage> &arr);
void resultFace(vector<WebPage *> &matched, vector<WebPage> &arr);
void viewFace(vector<WebPage *> &matched, vector<WebPage> &arr);

int FindByName_index(vector<WebPage> web, string name) {
  for (int i = 0; i < web.size(); i++) {
    if (web[i].get_name() == name)
      return i; // assuming if will be always true
  }
}
void addEdge(vector<WebPage> &web, string name1, string name2) {
  int i;
  int j;
  i = FindByName_index(web, name1);
  j = FindByName_index(web, name2);
  WebPage *p1 = &web[i];
  WebPage *p2 = &web[j];
  p1->ToWeb.push_back(p2);
  p2->FromWeb.push_back(p1);
}
map<string, vector<string>> read_keywords() {
  map<string, vector<string>> arr;
  fstream fin;
  fin.open("KeywordsInputFile.csv", ios::in);
  vector<string> row;
  string line, word, name;

  while (getline(fin, line)) {
    row.clear();
    stringstream s(line);
    getline(s, name, ',');
    while (getline(s, word, ',')) {
      row.push_back(word);
    }
    arr.insert({name, row});
  }
  fin.close();
  return arr;
}
map<string, int> read_impression() {
  map<string, int> arr;
  fstream fin;
  fin.open("ImpressionsInputFile.csv", ios::in);
  string line, word, name;

  while (getline(fin, line)) {
    stringstream s(line);
    getline(s, name, ',');
    getline(s, word, ',');
    arr.insert({name, stoi(word)});
  }
  fin.close();
  return arr;
}
map<string, int> read_clicked() {
  map<string, int> arr;
  fstream fin;
  fin.open("ClicksInputFile.csv", ios::in);
  string line, word, name;

  while (getline(fin, line)) {
    stringstream s(line);
    getline(s, name, ',');
    getline(s, word, ',');
    arr.insert({name, stoi(word)});
  }
  fin.close();
  return arr;
}
multimap<string, string> read_edges() {
  multimap<string, string> arr;
  fstream fin;
  fin.open("WebGraphInputFile.csv", ios::in);
  string line, word, name;

  while (getline(fin, line)) {
    stringstream s(line);
    getline(s, name, ',');
    getline(s, word, ',');
    arr.insert({name, word});
  }
  fin.close();
  return arr;
}
void update_impression(string name, int impression) {
  fstream fin, fout;
  fin.open("ImpressionsInputFile.csv", ios::in);
  fout.open("ImpressionsOutputFile.csv", ios::out);
  int count = 0, i;
  string roll1;
  string line, word;
  vector<string> row;

  // Traverse the file
  while (getline(fin, line)) {
    row.clear();
    stringstream s(line);
    while (getline(s, word, ',')) {
      row.push_back(word);
    }
    roll1 = row[0];
    int row_size = row.size();
    if (roll1 == name) {
      count = 1;
      stringstream convert;
      convert << impression;
      row[1] = convert.str();

      if (!fin.eof()) {
        for (i = 0; i < row_size - 1; i++) {
          fout << row[i] << ",";
        }
        fout << row[row_size - 1] << "\n";
      }
    }

    else {
      if (!fin.eof()) {
        for (i = 0; i < row_size - 1; i++) {
          fout << row[i] << ",";
        }
        fout << row[row_size - 1] << "\n";
      }
    }
    if (fin.eof())
      break;
  }
  if (count == 0)
    cout << "Record not found\n";

  fin.close();
  fout.close();

  remove("ImpressionsInputFile.csv");
  rename("ImpressionsOutputFile.csv", "ImpressionsInputFile.csv");
}
void update_clicked(string name, int clicked, vector<WebPage> &arr) {
  fstream fin, fout;
  fin.open("ClicksInputFile.csv", ios::in);
  fout.open("ClicksOutputFile.csv", ios::out);
  int count = 0, i;
  string roll1;
  string line, word;
  vector<string> row;

  // Traverse the file
  while (getline(fin, line)) {
    row.clear();
    stringstream s(line);
    while (getline(s, word, ',')) {
      row.push_back(word);
    }
    roll1 = row[0];
    int row_size = row.size();
    if (roll1 == name) {
      count = 1;
      stringstream convert;
      convert << clicked;
      row[1] = convert.str();

      if (!fin.eof()) {
        for (i = 0; i < row_size - 1; i++) {
          fout << row[i] << ",";
        }
        fout << row[row_size - 1] << "\n";
      }
    }

    else {
      if (!fin.eof()) {
        for (i = 0; i < row_size - 1; i++) {
          fout << row[i] << ",";
        }
        fout << row[row_size - 1] << "\n";
      }
    }
    if (fin.eof())
      break;
  }
  if (count == 0)
    cout << "Record not found\n";

  fin.close();
  fout.close();

  remove("ClicksInputFile.csv");
  rename("ClicksOutputFile", "ClicksInputFile.csv");
}
void calculate_PR(vector<WebPage> &arr) {
  for (int i = 0; i < arr.size(); i++) {
    arr[i].set_PR(1.0 / arr.size());
    arr[i].set_PRtrial(1.0 / arr.size());
  }

  for (int j = 1; j < arr.size() - 1; j++) {
    for (int i = 0; i < arr.size(); i++) {
      double PRe = 0.0;
      for (int k = 0; k < arr[i].FromWeb.size(); k++) {
        PRe += (arr[i].FromWeb[k]->get_PR() / arr[i].FromWeb[k]->ToWeb.size());
      }

      arr[i].set_PRtrial(PRe);
    }
    for (int i = 0; i < arr.size(); i++)
      arr[i].set_PR(arr[i].get_PRtrial());
  }

  multimap<double, int> numbers;
  multimap<double, int>::iterator it = numbers.begin();
  for (int i = 0; i < arr.size(); i++)
    numbers.insert(pair<double, int>(arr[i].get_PR(), i));

  it = numbers.begin();
  int i = 1;
  for (it = numbers.begin(); it != numbers.end(); it++) {
    arr[it->second].set_PR(i);
    i++;
  }
}
vector<WebPage *> search_query(string str, vector<WebPage> &web) {
  vector<string> strings;
  string AND = "";
  // Handling the quotes
  if (str[0] == '"' && str[str.length() - 1] == '"') {
    str.erase(0, 1);
    str.erase(str.length() - 1, 1);
    strings.push_back(str);
  }

  else {
    // Handling OR case
    while (str.find("OR") <= str.length())
      if (str.find("OR") <= str.length())
        str.erase(str.find("OR"), 3);

    // Handling AND case
    if (str.find("AND") <= str.length()) {
      int j = str.find("AND");
      str.erase(j, 4);
      AND = str.substr(j, str.length() - 1);
      str.erase(j, str.length() - 1);
    }

    // separate sentence to single words
    while (str.find(' ') <= str.length()) {
      strings.push_back(str.substr(0, str.find(' ')));
      str.erase(0, str.substr(0, str.find(' ')).length() + 1);
    }
    if (!str.empty())
      strings.push_back(str);
  }

  vector<WebPage *> matched;
  vector<bool> visited(web.size(), false);
  // searching
  for (int i = 0; i < strings.size(); i++) {
    for (int j = 0; j < web.size(); j++) {
      if (AND == "") {
        if (find(web[j].keywords.begin(), web[j].keywords.end(), strings[i]) !=
            web[j].keywords.end()) {
          if (visited[j] == false)
            matched.push_back(&web[j]);
          visited[j] = true;
        }
      } else {
        if ((find(web[j].keywords.begin(), web[j].keywords.end(), strings[i]) !=
             web[j].keywords.end()) &&
            (find(web[j].keywords.begin(), web[j].keywords.end(), AND) !=
             web[j].keywords.end())) {
          if (visited[j] == false)
            matched.push_back(&web[j]);
          visited[j] = true;
        }
      }
    }
  }
  return matched;
}
vector<WebPage *> sort_result(vector<WebPage *> &matched) {
  vector<WebPage *> sorted;
  for (int i = 0; i < matched.size(); i++) {
    matched[i]->set_CTR();
    matched[i]->set_score();
  }

  for (int j = 0; j < matched.size(); j++) {
    double max_score = 0.0;
    int index;
    for (int i = 0; i < matched.size(); i++) {
      if (matched[i]->get_score() > max_score) {
        max_score = matched[i]->get_score();
        index = i;
      }
    }
    sorted.push_back(matched[index]);
    matched[index]->set_manual_score();
  }
  return sorted;
}
bool startFace(vector<WebPage> &arr) {
  cout << "Welcome!" << endl;
  cout << "What would you like to do?" << endl;
  cout << "1. New Search" << endl;
  cout << "2. Exit " << endl;
  cout << endl;
  cout << "Type in your choice: ";
  int num;
  cin >> num;
  while (num != 1 && num != 2) {
    cout << "Enter a valid choice: ";
    cin >> num;
  }
  if (num == 2)
    return false;
  if (num == 1) {
    searchFace(arr);
    return true;
  }
}
void searchFace(vector<WebPage> &arr) {
  cout << endl;
  cout << "What do you want to search about? " << endl;
  cout << "Keyword: ";
  vector<WebPage *> matched;
  string str;
  while (str.empty())
    std::getline(cin, str);
  matched = search_query(str, arr);
  if (matched.size() == 0) {
    cout << "No websites for your search query" << endl;
    searchFace(arr);
    return;
  } else {
    matched = sort_result(matched);
    resultFace(matched, arr);
    return;
  }
}
void resultFace(vector<WebPage *> &matched, vector<WebPage> &arr) {
  cout << endl;
  cout << "Search results: " << endl;
  for (int i = 1; i <= matched.size(); i++) {
    matched[i - 1]->set_CTR();
    matched[i - 1]->set_score();
    matched[i - 1]->increment_impression();
    update_impression(matched[i - 1]->get_name(),
                      matched[i - 1]->get_impression());
    cout << i << ". " << matched[i - 1]->get_name() << endl;
  }
  cout << endl;
  cout << "Would you like to " << endl;
  cout << "1. Choose a webpage to open" << endl;
  cout << "2. New Search" << endl;
  cout << "3. Exit" << endl;
  cout << endl;
  cout << "Type in your choice: ";
  int num2;
  cin >> num2;
  while (num2 != 1 && num2 != 2 && num2 != 3) {
    cout << "Enter a valid choice: ";
    cin >> num2;
  }
  if (num2 == 3)
    return;
  else if (num2 == 2) {
    searchFace(arr);
    return;
  } else if (num2 == 1) {
    viewFace(matched, arr);
    return;
  }
}
void viewFace(vector<WebPage *> &matched, vector<WebPage> &arr) {
  cout << "Choose the webpage you want to view: ";
  int num3;
  cin >> num3;
  while (num3 < 1 || num3 > matched.size()) {
    cout << "Enter a valid choice: ";
    cin >> num3;
  }
  matched[num3 - 1]->increment_click();
  update_clicked(matched[num3 - 1]->get_name(),
                 matched[num3 - 1]->get_clicked(), arr);
  cout << endl;
  cout << "You are now viewing " << matched[num3 - 1]->get_name() << endl;
  cout << "Would you like to " << endl;
  cout << "1. Back to search results" << endl;
  cout << "2. New search" << endl;
  cout << "3. Exit" << endl;
  cout << endl;
  cout << "Type in your choice: ";
  int sec_num;
  cin >> sec_num;
  while (sec_num != 1 && sec_num != 2 && sec_num != 3) {
    cout << "Enter a valid choice: ";
    cin >> sec_num;
  }
  if (sec_num == 3)
    return;
  else if (sec_num == 1) {
    resultFace(matched, arr);
    return;
  } else if (sec_num == 2) {
    searchFace(arr);
    return;
  }
}
vector<WebPage> readingFiles() {
  map<string, vector<string>> arr = read_keywords();

  vector<WebPage> webpages(arr.size());

  map<string, vector<string>>::iterator it = arr.begin();
  int i = 0;
  for (it = arr.begin(); it != arr.end(); it++) {
    webpages[i].set_name_keywords(it->first, it->second);
    i++;
  }

  map<string, int> imp = read_impression();

  map<string, int>::iterator itr = imp.begin();
  i = 0;
  for (itr = imp.begin(); itr != imp.end(); itr++) {
    webpages[i].set_Impression(itr->second);
    i++;
  }

  map<string, int> clk = read_clicked();

  map<string, int>::iterator itre = clk.begin();
  i = 0;
  for (itre = clk.begin(); itre != clk.end(); itre++) {
    webpages[i].set_clicked(itre->second);
    i++;
  }

  multimap<string, string> edges = read_edges();
  multimap<string, string>::iterator ed = edges.begin();
  for (ed = edges.begin(); ed != edges.end(); ed++)
    addEdge(webpages, ed->first, ed->second);

  calculate_PR(webpages);
  return webpages;
}

int main() {
  vector<WebPage> webpages = readingFiles();
  if (!startFace(webpages)) {
    cout << "Thank you for using our service!" << endl;
    return 0;
  }
}