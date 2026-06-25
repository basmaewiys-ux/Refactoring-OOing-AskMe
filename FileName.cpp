#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <map>
#include <fstream>
#include <cassert>
#include <algorithm>

using namespace std;

/////////////////////////////// Helper Methods ///////////////////////////////
vector<string> ReadFileLines(string path) {
    vector<string> lines;
    fstream file_handler(path.c_str());

    if (file_handler.fail()) {
        cout << "\n\nERROR: Can't open the file\n\n";
        return lines;
    }
    string line;
    while (getline(file_handler, line)) {
        if (line.size() == 0)
            continue;
        lines.push_back(line);
    }
    file_handler.close();
    return lines;
}

void WriteFileLines(string path, vector<string> lines, bool append = true) {
    auto status = ios::in | ios::out | ios::app;
    if (!append)
        status = ios::in | ios::out | ios::trunc;

    fstream file_handler(path.c_str(), status);
    if (file_handler.fail()) {
        cout << "\n\nERROR: Can't open the file\n\n";
        return;
    }
    for (auto line : lines)
        file_handler << line << "\n";

    file_handler.close();
}

vector<string> SplitString(string s, string delimiter = ",") {
    vector<string> strs;
    int pos = 0;
    string substr;
    while ((pos = (int)s.find(delimiter)) != -1) {
        substr = s.substr(0, pos);
        strs.push_back(substr);
        s.erase(0, pos + delimiter.length());
    }
    strs.push_back(s);
    return strs;
}

int ToInt(string str) {
    istringstream iss(str);
    int num;
    iss >> num;
    return num;
}

int ReadInt(int low, int high) {
    cout << "\nEnter number in range " << low << " - " << high << ": ";
    int value;
    cin >> value;

    if (low <= value && value <= high)
        return value;

    cout << "ERROR: invalid number...Try again\n";
    return ReadInt(low, high);
}

int ShowReadMenu(vector<string> choices) {
    cout << "\nMenu:\n";
    for (int ch = 0; ch < (int)choices.size(); ++ch) {
        cout << "\t" << ch + 1 << ": " << choices[ch] << "\n";
    }
    return ReadInt(1, choices.size());
}

///////////////////////////////// Classes /////////////////////////////////

class Question {
private:
    int question_id;
    int parent_question_id;
    int from_user_id;
    int to_user_id;
    int is_anonymous_questions;
    string question_text;
    string answer_text;

public:
    Question() {
        question_id = parent_question_id = from_user_id = to_user_id = -1;
        is_anonymous_questions = 1;
    }

    Question(string line) {
        vector<string> substrs = SplitString(line);
        assert(substrs.size() == 7);

        question_id = ToInt(substrs[0]);
        parent_question_id = ToInt(substrs[1]);
        from_user_id = ToInt(substrs[2]);
        to_user_id = ToInt(substrs[3]);
        is_anonymous_questions = ToInt(substrs[4]);
        question_text = substrs[5];
        answer_text = substrs[6];
    }

    string ToString() {
        ostringstream oss;
        oss << question_id << "," << parent_question_id << "," << from_user_id << ","
            << to_user_id << "," << is_anonymous_questions << "," << question_text << "," << answer_text;
        return oss.str();
    }

    int GetQuestionId() { return question_id; }
    void SetQuestionId(int id) { question_id = id; }

    int GetParentQuestionId() { return parent_question_id; }
    void SetParentQuestionId(int id) { parent_question_id = id; }

    int GetFromUserId() { return from_user_id; }
    void SetFromUserId(int id) { from_user_id = id; }

    int GetToUserId() { return to_user_id; }
    void SetToUserId(int id) { to_user_id = id; }

    int GetIsAnonymous() { return is_anonymous_questions; }
    void SetIsAnonymous(int status) { is_anonymous_questions = status; }

    string GetQuestionText() { return question_text; }
    void SetQuestionText(string text) { question_text = text; }

    string GetAnswerText() { return answer_text; }
    void SetAnswerText(string text) { answer_text = text; }

    void PrintToQuestion() {
        string prefix = "";
        if (parent_question_id != -1)
            prefix = "\tThread: ";

        cout << prefix << "Question Id (" << question_id << ")";
        if (!is_anonymous_questions)
            cout << " from user id(" << from_user_id << ")";
        cout << "\t Question: " << question_text << "\n";

        if (answer_text != "")
            cout << prefix << "\tAnswer: " << answer_text << "\n";
        cout << "\n";
    }

    void PrintFromQuestion() {
        cout << "Question Id (" << question_id << ")";
        if (!is_anonymous_questions)
            cout << " !AQ";

        cout << " to user id(" << to_user_id << ")";
        cout << "\t Question: " << question_text;

        if (answer_text != "")
            cout << "\tAnswer: " << answer_text << "\n";
        else
            cout << "\tNOT Answered YET\n";
    }

    void PrintFeedQuestion() {
        if (parent_question_id != -1)
            cout << "Thread Parent Question ID (" << parent_question_id << ") ";

        cout << "Question Id (" << question_id << ")";
        if (!is_anonymous_questions)
            cout << " from user id(" << from_user_id << ")";

        cout << " To user id(" << to_user_id << ")";
        cout << "\t Question: " << question_text << "\n";
        if (answer_text != "")
            cout << "\tAnswer: " << answer_text << "\n";
    }
};

class User {
private:
    int user_id;
    string user_name;
    string password;
    string name;
    string email;
    int allow_anonymous_questions;

    vector<int> questions_id_from_me;
    map<int, vector<int>> questionid_questionidsThead_to_map;

public:
    User() {
        user_id = allow_anonymous_questions = -1;
    }

    User(string line) {
        vector<string> substrs = SplitString(line);
        assert(substrs.size() == 6);

        user_id = ToInt(substrs[0]);
        user_name = substrs[1];
        password = substrs[2];
        name = substrs[3];
        email = substrs[4];
        allow_anonymous_questions = ToInt(substrs[5]);
    }

    string ToString() {
        ostringstream oss;
        oss << user_id << "," << user_name << "," << password << "," << name << "," << email << "," << allow_anonymous_questions;
        return oss.str();
    }

    int GetUserId() { return user_id; }
    void SetUserId(int id) { user_id = id; }

    string GetUserName() { return user_name; }
    string GetPassword() { return password; }
    string GetName() { return name; }
    int GetAllowAnonymous() { return allow_anonymous_questions; }

    vector<int>& GetQuestionsIdFromMe() { return questions_id_from_me; }
    map<int, vector<int>>& GetThreadMap() { return questionid_questionidsThead_to_map; }

    void Print() {
        cout << "User " << user_id << ", " << user_name << " " << password << ", " << name << ", " << email << "\n";
    }
};

class QuestionsManager {
private:
    map<int, vector<int>> questionid_questionidsThead_to_map;
    map<int, Question> questionid_questionobject_map;
    int last_id;

public:
    QuestionsManager() {
        last_id = 0;
    }

    void LoadDatabase() {
        last_id = 0;
        questionid_questionidsThead_to_map.clear();
        questionid_questionobject_map.clear();

        vector<string> lines = ReadFileLines("18_questions.txt");
        for (auto& line : lines) {
            Question question(line);
            last_id = max(last_id, question.GetQuestionId());

            questionid_questionobject_map[question.GetQuestionId()] = question;

            if (question.GetParentQuestionId() == -1)
                questionid_questionidsThead_to_map[question.GetQuestionId()].push_back(question.GetQuestionId());
            else
                questionid_questionidsThead_to_map[question.GetParentQuestionId()].push_back(question.GetQuestionId());
        }
    }

    void FillUserQuestions(User& user) {
        user.GetQuestionsIdFromMe().clear();
        user.GetThreadMap().clear();

        for (auto& pair : questionid_questionidsThead_to_map) {
            for (auto& question_id : pair.second) {
                Question& question = questionid_questionobject_map[question_id];

                if (question.GetFromUserId() == user.GetUserId())
                    user.GetQuestionsIdFromMe().push_back(question.GetQuestionId());

                if (question.GetToUserId() == user.GetUserId()) {
                    if (question.GetParentQuestionId() == -1)
                        user.GetThreadMap()[question.GetQuestionId()].push_back(question.GetQuestionId());
                    else
                        user.GetThreadMap()[question.GetParentQuestionId()].push_back(question.GetQuestionId());
                }
            }
        }
    }

    void PrintUserToQuestions(User& user) {
        cout << "\n";
        if (user.GetThreadMap().size() == 0)
            cout << "No Questions";

        for (auto& pair : user.GetThreadMap()) {
            for (auto& question_id : pair.second) {
                Question& question = questionid_questionobject_map[question_id];
                question.PrintToQuestion();
            }
        }
        cout << "\n";
    }

    void PrintUserFromQuestions(User& user) {
        cout << "\n";
        if (user.GetQuestionsIdFromMe().size() == 0)
            cout << "No Questions";

        for (auto& question_id : user.GetQuestionsIdFromMe()) {
            Question& question = questionid_questionobject_map[question_id];
            question.PrintFromQuestion();
        }
        cout << "\n";
    }

    int ReadQuestionIdAny(User& user) {
        int question_id;
        cout << "Enter Question id or -1 to cancel: ";
        cin >> question_id;

        if (question_id == -1)
            return -1;

        if (!questionid_questionobject_map.count(question_id)) {
            cout << "\nERROR: No question with such ID. Try again\n\n";
            return ReadQuestionIdAny(user);
        }
        Question& question = questionid_questionobject_map[question_id];

        if (question.GetToUserId() != user.GetUserId()) {
            cout << "\nERROR: Invalid question ID. Try again\n\n";
            return ReadQuestionIdAny(user);
        }
        return question_id;
    }

    int ReadQuestionIdThread(User& user) {
        int question_id;
        cout << "For thread question: Enter Question id or -1 for new question: ";
        cin >> question_id;

        if (question_id == -1)
            return -1;

        if (!questionid_questionidsThead_to_map.count(question_id)) {
            cout << "No thread question with such ID. Try again\n";
            return ReadQuestionIdThread(user);
        }
        return question_id;
    }

    void AnswerQuestion(User& user) {
        int question_id = ReadQuestionIdAny(user);
        if (question_id == -1)
            return;

        Question& question = questionid_questionobject_map[question_id];
        question.PrintToQuestion();

        if (question.GetAnswerText() != "")
            cout << "\nWarning: Already answered. Answer will be updated\n";

        cout << "Enter answer: ";
        string ans;
        getline(cin, ans);
        getline(cin, ans);
        question.SetAnswerText(ans);
    }

    void DeleteQuestion(User& user) {
        int question_id = ReadQuestionIdAny(user);
        if (question_id == -1)
            return;

        vector<int> ids_to_remove;

        if (questionid_questionidsThead_to_map.count(question_id)) {
            ids_to_remove = questionid_questionidsThead_to_map[question_id];
            questionid_questionidsThead_to_map.erase(question_id);
        }
        else {
            ids_to_remove.push_back(question_id);

            for (auto& pair : questionid_questionidsThead_to_map) {
                vector<int>& vec = pair.second;
                for (int pos = 0; pos < (int)vec.size(); ++pos) {
                    if (question_id == vec[pos]) {
                        vec.erase(vec.begin() + pos);
                        break;
                    }
                }
            }
        }

        for (auto id : ids_to_remove) {
            questionid_questionobject_map.erase(id);
        }
    }

    void AskQuestion(User& user, pair<int, int> to_user_pair) {
        Question question;

        if (!to_user_pair.second) {
            cout << "Note: Anonymous questions are not allowed for this user\n";
            question.SetIsAnonymous(0);
        }
        else {
            int status;
            cout << "Is anonymous questions?: (0 or 1): ";
            cin >> status;
            question.SetIsAnonymous(status);
        }

        question.SetParentQuestionId(ReadQuestionIdThread(user));

        cout << "Enter question text: ";
        string text;
        getline(cin, text);
        getline(cin, text);
        question.SetQuestionText(text);

        question.SetFromUserId(user.GetUserId());
        question.SetToUserId(to_user_pair.first);

        question.SetQuestionId(++last_id);
        questionid_questionobject_map[question.GetQuestionId()] = question;

        if (question.GetParentQuestionId() == -1)
            questionid_questionidsThead_to_map[question.GetQuestionId()].push_back(question.GetQuestionId());
        else
            questionid_questionidsThead_to_map[question.GetParentQuestionId()].push_back(question.GetQuestionId());
    }

    void ListFeed() {
        for (auto& pair : questionid_questionobject_map) {
            Question& question = pair.second;
            if (question.GetAnswerText() == "")
                continue;
            question.PrintFeedQuestion();
        }
    }

    void UpdateDatabase() {
        vector<string> lines;
        for (auto& pair : questionid_questionobject_map)
            lines.push_back(pair.second.ToString());

        WriteFileLines("18_questions.txt", lines, false);
    }
};

class UsersManager {
private:
    map<string, User> userame_userobject_map;
    User current_user;
    int last_id;

public:
    UsersManager() {
        last_id = 0;
    }

    User& GetCurrentUser() { return current_user; }

    void LoadDatabase() {
        last_id = 0;
        userame_userobject_map.clear();

        vector<string> lines = ReadFileLines("18_users.txt");
        for (auto& line : lines) {
            User user(line);
            userame_userobject_map[user.GetUserName()] = user;
            last_id = max(last_id, user.GetUserId());
        }
    }

    void AccessSystem() {
        int choice = ShowReadMenu({ "Login", "Sign Up" });
        if (choice == 1)
            DoLogin();
        else
            DoSignUp();
    }

    void DoLogin() {
        LoadDatabase();
        while (true) {
            string uname, pass;
            cout << "Enter user name & password: ";
            cin >> uname >> pass;

            if (!userame_userobject_map.count(uname)) {
                cout << "\nInvalid user name or password. Try again\n\n";
                continue;
            }
            User user_exist = userame_userobject_map[uname];

            if (pass != user_exist.GetPassword()) {
                cout << "\nInvalid user name or password. Try again\n\n";
                continue;
            }
            current_user = user_exist;
            break;
        }
    }

    void DoSignUp() {
        string uname, pass, name, email;
        int allow_anon;

        while (true) {
            cout << "Enter user name. (No spaces): ";
            cin >> uname;

            if (userame_userobject_map.count(uname))
                cout << "Already used. Try again\n";
            else
                break;
        }
        cout << "Enter password: "; cin >> pass;
        cout << "Enter name: "; cin >> name;
        cout << "Enter email: "; cin >> email;
        cout << "Allow anonymous questions? (0 or 1): "; cin >> allow_anon;

        string line = to_string(last_id + 1) + "," + uname + "," + pass + "," + name + "," + email + "," + to_string(allow_anon);
        current_user = User(line);

        last_id++;
        userame_userobject_map[current_user.GetUserName()] = current_user;

        UpdateDatabase(current_user);
    }

    void ListUsersNamesIds() {
        for (auto& pair : userame_userobject_map)
            cout << "ID: " << pair.second.GetUserId() << "\t\tName: " << pair.second.GetName() << "\n";
    }

    pair<int, int> ReadUserId() {
        int user_id;
        cout << "Enter User id or -1 to cancel: ";
        cin >> user_id;

        if (user_id == -1)
            return make_pair(-1, -1);

        for (auto& pair : userame_userobject_map) {
            if (pair.second.GetUserId() == user_id)
                return make_pair(user_id, pair.second.GetAllowAnonymous());
        }

        cout << "Invalid User ID. Try again\n";
        return ReadUserId();
    }

    void UpdateDatabase(User& user) {
        string line = user.ToString();
        vector<string> lines(1, line);
        WriteFileLines("18_users.txt", lines);
    }
};

class AskMeSystem {
private:
    UsersManager users_manager;
    QuestionsManager questions_manager;

public:
    void LoadDatabase(bool fill_user_questions = false) {
        users_manager.LoadDatabase();
        questions_manager.LoadDatabase();

        if (fill_user_questions)
            questions_manager.FillUserQuestions(users_manager.GetCurrentUser());
    }

    void Run() {
        LoadDatabase();
        users_manager.AccessSystem();
        questions_manager.FillUserQuestions(users_manager.GetCurrentUser());

        vector<string> menu;
        menu.push_back("Print Questions To Me");
        menu.push_back("Print Questions From Me");
        menu.push_back("Answer Question");
        menu.push_back("Delete Question");
        menu.push_back("Ask Question");
        menu.push_back("List System Users");
        menu.push_back("Feed");
        menu.push_back("Logout");

        while (true) {
            int choice = ShowReadMenu(menu);
            LoadDatabase(true);

            if (choice == 1)
                questions_manager.PrintUserToQuestions(users_manager.GetCurrentUser());
            else if (choice == 2)
                questions_manager.PrintUserFromQuestions(users_manager.GetCurrentUser());
            else if (choice == 3) {
                questions_manager.AnswerQuestion(users_manager.GetCurrentUser());
                questions_manager.UpdateDatabase();
            }
            else if (choice == 4) {
                questions_manager.DeleteQuestion(users_manager.GetCurrentUser());
                questions_manager.FillUserQuestions(users_manager.GetCurrentUser());
                questions_manager.UpdateDatabase();
            }
            else if (choice == 5) {
                pair<int, int> to_user_pair = users_manager.ReadUserId();
                if (to_user_pair.first != -1) {
                    questions_manager.AskQuestion(users_manager.GetCurrentUser(), to_user_pair);
                    questions_manager.UpdateDatabase();
                }
            }
            else if (choice == 6)
                users_manager.ListUsersNamesIds();
            else if (choice == 7)
                questions_manager.ListFeed();
            else
                break;
        }
        Run();
    }
};

int main() {
    AskMeSystem service;
    service.Run();

    return 0;
}