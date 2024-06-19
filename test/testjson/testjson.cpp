#include "json.hpp"
using json = nlohmann::json;
#include <iostream>
#include <vector>
#include <map>
#include <string>
using namespace std;

string func1()
{
    json js;
    js["msg_type"] = 2;
    js["from"] = "zhang san";
    js["to"] = "li si";
    js["msg"] = "hello, what are you doing now?";
    string s = js.dump();

    return s;
    // cout<<s.c_str()<<endl;
}

string func2()
{
    json js;
    js["id"] = {1,2,3,4,5};
    js["name"] = "zhang san";
    js["msg"]["zhang san"] = "hello world";
    js["msg"]["li si"] = "hello china";
    return js.dump();
}

string func3()
{
    json js;
    vector<int> vec;
    vec.push_back(1);
    vec.push_back(2);
    vec.push_back(3);
    js["list"] = vec;
    map<int, string> m;
    m.insert({1, "黄山"});
    m.insert({2, "华山"});
    m.insert({3, "泰山"});
    js["path"] = m;
    string sjson = js.dump();
    return sjson;
    // cout<<js<<endl;
}

int main()
{
    string sjson = func2();
    json js2 = json::parse(sjson);
    string name = js2["name"];
    cout << "name:" << name << endl;
    func2();
    string sjson1 = func3();
    json js3 = json::parse(sjson1);
    vector<int> v = js3["list"];
    for(int val:v)
    {
        cout<<val<<" ";
    }
    cout<<endl;
    map<int, string> m2 = js3["path"];
    for(auto p:m2)
    {
        cout<<p.first<<" "<<p.second<<endl;
    }
    cout<<endl;
    
    return 0;
}