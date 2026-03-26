#include <iostream>
#include <vector>
#include <utility> 
#include <algorithm> 
#include <string>
#include <cctype>
#include <unordered_map>
#include <queue>
#include <cmath>
#include <sstream>
#include <iomanip>
#include <ctime>

using namespace std;

string to_lower(string s){
    for (char &c : s) {
        c = std::tolower(static_cast<unsigned char>(c));
    }
    return s;
}

class Node {
    public:
    time_t key;
    string content;
    Node* left;
    Node* right;
    int height;

    Node(time_t k, string msg){
        key=k;
        content = msg;
        left=nullptr;
        right=nullptr;
        height=0;
    }
};


class AVLTree {
private:
    Node* root;

    int height(Node* N) {
        if (N == nullptr)
            return -1;
        return N->height;
    }
    
    Node* leftRotate(Node* y){
        Node* x = y->right;
        Node* T1 = x->left;
        
        x->left = y;
        y->right = T1;
        
        y->height = max(height(y->left), height(y->right)) + 1;
        x->height = max(height(x->left), height(x->right)) + 1;
        
        return x;
    }
    
    Node* rightRotate(Node* x){
        Node* y = x->left;
        Node* T2 = y->right;
        
        y->right = x;
        x->left = T2;
        
        x->height = max(height(x->left), height(x->right)) + 1;
        y->height = max(height(y->left), height(y->right)) + 1;

        return y;
    }
    
    int getBalance(Node* N) {
        if (N == nullptr)
            return 0;
        return height(N->left) - height(N->right);
    }

    
    Node* insertHelper(Node* node, time_t key, string msg) {
        if (node == nullptr)
            return new Node(key, msg);

        if (key < node->key)
            node->left = insertHelper(node->left, key, msg);
        else if (key > node->key)
            node->right = insertHelper(node->right, key, msg);
        else
            return node;

        node->height = 1 + max(height(node->left), height(node->right));

        int balance = getBalance(node);
        
        //Left Left Case
        if (balance > 1 && getBalance(node->left) >= 0)
            return rightRotate(node);

        // Left Right Case
        if (balance > 1 && getBalance(node->left) < 0) {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }

        // Right Right Case
        if (balance < -1 && getBalance(node->right) <= 0)
            return leftRotate(node);

        // Right Left Case
        if (balance < -1 && getBalance(node->right) > 0) {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }

        return node;
    }
    

    
    void deleteTree(Node* node) {
        if (node != nullptr) {
            deleteTree(node->left);
            deleteTree(node->right);
            delete node;
        }
    }

public:
    
    AVLTree() {
        root = nullptr;
    }

    
    ~AVLTree() {
        deleteTree(root);
    }

    
    void insert(string msg) {
        root = insertHelper(root, time(0), msg);
    }

    vector<string> inorder_traversal(Node* root){
        if(root==nullptr){return {};}
        vector<string> ans=inorder_traversal(root->left);
        ans.push_back(root->content);
        vector<string> temp = inorder_traversal(root->right);
        ans.insert(ans.end(), temp.begin(), temp.end());
        return ans;
    }
    void output_post(int n){
        vector<string> ans = inorder_traversal(root);
        if(n==-1){
            n=ans.size();
        }
        else{
            n=min(n, (int)ans.size());
        }
        
        for(int i=0; i<n; i++){
            cout << ans[ans.size()-i-1] << '\n';
        }
    }

};



class user{
    public:
    int id;
    string username;
    vector<int> friends;
    AVLTree tree;
    user(string name){
        username=name;
    }
};

class Graph{
    public:
    vector<user*> list;
    int num_users;
    unordered_map<string, int> map;
    Graph(){
        num_users=0;
    }
    ~Graph(){
        for(int i = 0; i < list.size(); i++){
            delete list[i];
        }
    }
    void add_user(string username){
        if (map.find(username) != map.end()){
            cout << "user already exists" << '\n';
            return;
        }
        user* temp = new user(username);
        temp->id = num_users;
        map[username]=num_users;
        list.push_back(temp);
        num_users++;
    }
    void add_friends(string u1, string u2){
        if (map.find(u1) == map.end() || map.find(u2) == map.end()){
            cout << "user doesnt exist" << '\n';
            return;            
        } 
        if(u1==u2){
            cout << "you cannot be your friend" << '\n';
            return;
        }
        int id1 = map[u1];
        int id2 = map[u2];
        for(int i=0; i<list[id1]->friends.size(); i++){
            if(list[id1]->friends[i]==id2){
                cout << "they are already friends" << '\n';
                return;
            }
        }
        list[id1]->friends.push_back(id2);
        list[id2]->friends.push_back(id1);
    }
    void list_friends(string u){
        if (map.find(u) == map.end()){
            cout << "user doesnt exist" << '\n';
            return;
        }
        int id = map[u];
        user*& person = list[id];
        vector<string> f;
        for(int i=0; i<person->friends.size(); i++){
            f.push_back(list[person->friends[i]]->username);
        }
        sort(f.begin(), f.end());
        for(int i=0; i<f.size(); i++){
            cout << f[i] << '\n';
        }
    }
    int degrees_of_separation(string u1, string u2){
        if (map.find(u1) == map.end() || map.find(u2) == map.end()){
            return -1;
        }
        if(u1==u2){
            return 0;
        }
        int id1 = map[u1];
        int id2 = map[u2];
        queue<int> q;
        q.push(id1);
        unordered_map<int, int> um;
        um[id1]=0;
        int dist=0;
        while(!q.empty()){
            int id = q.front();
            for(int i=0; i<list[id]->friends.size(); i++){
                if(um.find(list[id]->friends[i])==um.end()){
                    um[list[id]->friends[i]]=um[id]+1;
                    q.push(list[id]->friends[i]);
                }
                if(list[list[id]->friends[i]]->username==u2){
                    return um[list[id]->friends[i]];
                }
            }
            q.pop();
        }
        return -1;
    }
    void suggest_friends(string u, int n){
        if (map.find(u) == map.end()){
            cout << "user doesnt exist" << '\n';
            return;
        }
        if(n == 0){
            return;
        }
        unordered_map<int, int> um;
        int m = list[map[u]]->friends.size();
        vector<pair<int, string>> order(num_users);
        for(int i=0; i<num_users; i++){
            order[i]={0, ""};
        }
        um[map[u]]=1;
        for(int i=0; i<m; i++){
            um[list[map[u]]->friends[i]]=1;
        }
        for(int i=0; i<m; i++){
            int t = list[list[map[u]]->friends[i]]->friends.size();
            for(int j=0; j<t; j++){
                if(um.find(list[list[map[u]]->friends[i]]->friends[j])==um.end()){
                    order[list[list[map[u]]->friends[i]]->friends[j]].second = list[list[list[map[u]]->friends[i]]->friends[j]]->username;
                    order[list[list[map[u]]->friends[i]]->friends[j]].first--;
                }
            }
        }
        sort(order.begin(), order.end());
        n=min(n, (int)order.size());
        for(int i=0; i<n; i++){
            if(order[i].first!=0){
                cout << order[i].second << '\n';
            }
            else{
                break;
            }
        }
    }
};

int main(){

    Graph g;
    string line;
    while(getline(cin, line)){
        if(line.empty()){
            continue;
        }
        stringstream ss(line);
        string op;
        ss >> op;
        if(op== "ADD_USER"){
            string username;
            ss >> username;
            username = to_lower(username);
            g.add_user(username);
        }
        else if(op=="ADD_FRIEND"){
            string user1;
            string user2;
            ss >> user1;
            ss >> user2;
            user1 = to_lower(user1);
            user2 = to_lower(user2);
            g.add_friends(user1, user2);
        }
        else if(op=="LIST_FRIENDS"){
            string username;
            ss >> username;
            username = to_lower(username);
            g.list_friends(username);
        }
        else if(op=="SUGGEST_FRIENDS"){
            string username;
            ss >> username;
            string sn;
            int n;
            if (!(ss >> sn)) {
                cout << "Error: Missing argument n" << '\n';
                continue;
            }
            try {
                n = stoi(sn);
            } catch (const std::invalid_argument& e) {
                cout << "Error: Invalid number n" << sn << '\n';
                continue;
            }
            username = to_lower(username);
            g.suggest_friends(username, n);
        }
        else if(op=="DEGREES_OF_SEPARATION"){
            string user1;
            string user2;
            ss >> user1;
            ss >> user2;
            user1 = to_lower(user1);
            user2 = to_lower(user2);
            cout << g.degrees_of_separation(user1, user2) << '\n';
        }
        else if(op=="OUTPUT_POSTS"){
            string username;
            ss >> username;
            string sn;
            int n;
            if (!(ss >> sn)) {
                cout << "Error: Missing argument n" << '\n';
                continue;
            }

            // FIX: Add try-catch block
            try {
                n = stoi(sn);
            } catch (const std::invalid_argument& e) {
                cout << "Error: Invalid number n" << sn << '\n';
                continue;
            }
            username = to_lower(username);
            if(g.map.find(username)==g.map.end()){
                cout << "user doesnt exist" << '\n';
                continue;
            }
            g.list[g.map[username]]->tree.output_post(n);
        }
        else if(op=="ADD_POST"){
            string username;
            ss >> username;
            username = to_lower(username);
            if(g.map.find(username)==g.map.end()){
                cout << "user doesnt exist" << '\n';
                continue;
            }

            string content_line;
            getline(ss, content_line); 
            size_t first_quote = content_line.find_first_of('"');
            size_t last_quote = content_line.find_last_of('"');
            if (first_quote == string::npos || last_quote == string::npos || first_quote == last_quote) {
                cout << "content must be enclosed in double quotation marks" << '\n';
                continue;
            }
            string content = content_line.substr(first_quote + 1, last_quote - first_quote - 1);

            if (content.empty()) {
                cout << " Post content cannot be empty" << '\n';
                continue;
            }
            
            g.list[g.map[username]]->tree.insert(to_lower(content));
        }
        else if(op=="EXIT"){
            break;
        }
        else{
            cout << "Input a valid command" << '\n';
        }
    }

    return 0;
}