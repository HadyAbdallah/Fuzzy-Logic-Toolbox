//لَا أَبْرَحُ حَتَّىٰ أَبْلُغَ
//آلَحمْدُلِلّـَّه
//If you never try. You will never know

#include <bits/stdc++.h>
using namespace std;
using ll = long long ;
const int N= 1000003 ;

class fuzzy_set
{
public:
    string name;
    double min_range, max_range ,centroid;
    string type;
    vector<double> values;
};

class fuzzy_variables
{
public:
    string name;
    string type;
    double value = 0.0;
    double min_range, max_range;
    vector<fuzzy_set> sets;
};



class fuzzy_rule
{
public:
    string out_variable, out_set;
    vector<fuzzy_variables> variables;
    vector<int>variables_indexs;
    vector<string> operators;
    vector<string> sets;
    vector<int>sets_indexs;
    int out_variable_idx , out_set_idx;
};



class fuzzy_system
{
public:
    string name , description;
    vector<fuzzy_variables> in_variables;
    vector<fuzzy_variables> out_variables;
    vector<fuzzy_rule> rules;

    vector<vector<double>> member_ship_vector;
    vector<double> rules_res;

    void member_ship(double x , int idx)
    {
        vector<double> res;
        vector<fuzzy_set> fs = in_variables[idx].sets;

        for (int i=0 ; i<fs.size() ; i++)
        {
            if (fs[i].type == "TRAP")
            {
                if (x >= fs[i].values[0] && x <= fs[i].values[3])
                {
                    if (x >= fs[i].values[1] && x <= fs[i].values[2])
                    {
                        res.push_back(1);
                    }
                    else {
                        if (x >= fs[i].values[0] && x <= fs[i].values[1])
                        {
                            double slp = 1 / (fs[i].values[1] - fs[i].values[0]);
                            double b = -1 * fs[i].values[0] * slp;
                            res.push_back(slp * x + b);
                        }
                        else if (x >= fs[i].values[2] && x <= fs[i].values[3])
                        {
                            double slp = -1 / (fs[i].values[3] - fs[i].values[2]);
                            double b = -1 * fs[i].values[3] * slp;
                            res.push_back(slp * x + b);
                        }
                    }
                }
                else res.push_back(0.0);

            }

            else
            {
                if (x >= fs[i].values[0] && x <= fs[i].values[2])
                {
                    if (x >= fs[i].values[0] && x <= fs[i].values[1])
                    {
                        double slp = 1 / (fs[i].values[1] - fs[i].values[0]);
                        double b = -1 * fs[i].values[0] * slp;
                        res.push_back(slp * x + b);
                    }
                    else {
                        double slp = -1 / (fs[i].values[2] - fs[i].values[1]);
                        double b = -1 * fs[i].values[2] * slp;
                        res.push_back(slp * x + b);
                    }
                }
                else res.push_back(0.0);
            }
        }
        member_ship_vector.push_back(res);
    }


    void inference_rule ()
    {
        vector<fuzzy_rule> temp_rules = rules;
        for (auto rule : temp_rules)
        {
            for (int j = 0 ; j<rule.operators.size() ; j++)
            {
                if(rule.operators[j] == "and_not" or rule.operators[j] == "or_not")
                {
                    rule.variables[j+1].value = 1-member_ship_vector[rule.variables_indexs[j+1]][rule.sets_indexs[j+1]];
                    if (rule.operators[j] == "and_not")
                        rule.operators[j] = "and";
                    else
                        rule.operators[j] = "or";

                }
            }
            for (int j = 0 ; j<rule.operators.size() ; j++)
            {
                if(rule.operators[j] == "and")
                {
                    if (rule.variables[j].value ==0 and rule.variables[j+1].value ==0)
                    {
                        rule.variables[j].value = min(member_ship_vector[rule.variables_indexs[j]][rule.sets_indexs[j]] , member_ship_vector[rule.variables_indexs[j+1]][rule.sets_indexs[j+1]]);
                    }
                    else if (rule.variables[j].value !=0)
                    {
                        rule.variables[j].value = min(rule.variables[j].value , member_ship_vector[rule.variables_indexs[j+1]][rule.sets_indexs[j+1]]);

                    }
                    else if (rule.variables[j+1].value !=0)
                    {
                        rule.variables[j].value = min(rule.variables[j+1].value , member_ship_vector[rule.variables_indexs[j]][rule.sets_indexs[j]]);

                    }
                    else
                    {
                        rule.variables[j].value = min(rule.variables[j+1].value , rule.variables[j].value);

                    }
                    rule.variables.erase(rule.variables.begin()+(j+1));
                    rule.variables_indexs.erase(rule.variables_indexs.begin()+(j+1));
                    rule.sets.erase(rule.sets.begin()+(j+1));
                    rule.sets_indexs.erase(rule.sets_indexs.begin()+(j+1));
                    rule.operators.erase(rule.operators.begin()+j);
                    j--;
                }
            }
            for (int j = 0 ; j<rule.operators.size() ; j++)
            {
                if(rule.operators[j] == "or")
                {
                    if (rule.variables[j].value ==0 and rule.variables[j+1].value ==0)
                    {
                        rule.variables[j].value = max(member_ship_vector[rule.variables_indexs[j]][rule.sets_indexs[j]] , member_ship_vector[rule.variables_indexs[j+1]][rule.sets_indexs[j+1]]);
                    }
                    else if (rule.variables[j].value !=0)
                    {
                        rule.variables[j].value = max(rule.variables[j].value , member_ship_vector[rule.variables_indexs[j+1]][rule.sets_indexs[j+1]]);
                    }
                    else if (rule.variables[j+1].value !=0)
                    {
                        rule.variables[j].value = max(rule.variables[j+1].value , member_ship_vector[rule.variables_indexs[j]][rule.sets_indexs[j]]);
                    }
                    else
                    {
                        rule.variables[j].value = max(rule.variables[j+1].value , rule.variables[j].value);
                    }
                    rule.variables.erase(rule.variables.begin()+(j+1));
                    rule.variables_indexs.erase(rule.variables_indexs.begin()+(j+1));
                    rule.sets.erase(rule.sets.begin()+(j+1));
                    rule.sets_indexs.erase(rule.sets_indexs.begin()+(j+1));
                    rule.operators.erase(rule.operators.begin()+j);
                    j--;
                }
            }
            rules_res.push_back(rule.variables[0].value);
        }
    }

    double Defuzzification ()
    {
        double sum =0;
        double denum =0;
        double ans=0;
        for (int i = 0; i < rules.size(); ++i)
        {
            sum += rules_res[i]*out_variables[rules[i].out_variable_idx].sets[rules[i].out_set_idx].centroid;
            denum += rules_res[i];
        }
        ans=(double)sum/denum;
        return ans;
    }

    string get_outputSet(double x)
    {
        vector<double> res;
        vector<fuzzy_set> fs = out_variables[0].sets;

        for (int i=0 ; i<fs.size() ; i++)
        {
            if (fs[i].type == "TRAP")
            {
                if (x >= fs[i].values[0] && x <= fs[i].values[3])
                {
                    if (x >= fs[i].values[1] && x <= fs[i].values[2])
                    {
                        res.push_back(1);
                    }
                    else
                    {
                        if (x >= fs[i].values[0] && x <= fs[i].values[1])
                        {
                            double slp = 1 / (fs[i].values[1] - fs[i].values[0]);
                            double b = -1 * fs[i].values[0] * slp;
                            res.push_back(slp * x + b);
                        }
                        else if (x >= fs[i].values[2] && x <= fs[i].values[3])
                        {
                            double slp = -1 / (fs[i].values[3] - fs[i].values[2]);
                            double b = -1 * fs[i].values[3] * slp;
                            res.push_back(slp * x + b);
                        }
                    }
                }
                else res.push_back(0.0);

            }

            else
            {
                if (x >= fs[i].values[0] && x <= fs[i].values[2])
                {
                    if (x >= fs[i].values[0] && x <= fs[i].values[1])
                    {
                        double slp = 1 / (fs[i].values[1] - fs[i].values[0]);
                        double b = -1 * fs[i].values[0] * slp;
                        res.push_back(slp * x + b);
                    }
                    else
                    {
                        double slp = -1 / (fs[i].values[2] - fs[i].values[1]);
                        double b = -1 * fs[i].values[2] * slp;
                        res.push_back(slp * x + b);
                    }
                }
                else res.push_back(0.0);
            }
        }
        double mx = -1000.0;
        int index = -1;
        for (int i = 0; i < res.size(); ++i)
        {
            if(res[i]>=mx)
            {
                mx=res[i];
                index=i;
            }
        }
        return out_variables[0].sets[index].name;
    }

    long long checkVariables(string s, string type)
    {
        if(type == "IN")
        {
            long long idx = -1;
            for (int j=0  ; j<in_variables.size() ; j++)
            {
                if (in_variables[j].name == s)
                {
                    idx = j;
                    return idx;
                }
            }
            return idx;
        }
        else
        {
            long long idx = -1;
            for (int j=0  ; j<out_variables.size() ; j++)
            {
                if (out_variables[j].name == s)
                {
                    idx = j;
                    return idx;
                }
            }
            return idx;
        }
    }

    int checkSet(string s, string type, long long idx)
    {
        if(type == "IN")
        {
            vector<fuzzy_set>ss = in_variables[idx].sets;
            for(long long i = 0 ; i < ss.size() ; i++)
            {
                if(ss[i].name == s)
                {
                    return i;
                }
            }
            return -1;
        }
        else
        {
            vector<fuzzy_set> ss = out_variables[idx].sets;
            for(long long i = 0 ; i < ss.size() ; i++)
            {
                if(ss[i].name == s)
                {
                    return i;
                }
            }
            return -1;
        }

    }

};

int main()
{

    string menu1= "Fuzzy Logic Toolbox\n"
                  "===================\n"
                  "1- Create a new fuzzy system\n"
                  "2- Quit\n";


    cout<<menu1;

    int choice1;
    cin>>choice1;
    cin.ignore();

    while (choice1!=1 and choice1!=2)
    {
        cout<<"Wrong choice, please try again and choose either 1 or 2.\n";
        cin>>choice1;
        cin.ignore();
    }

    while (choice1!=2)
    {
        cout<<"Enter the system menu name and a brief description:\n";
        cout<<"------------------------------------------------\n";

        fuzzy_system f1;
        getline(cin,f1.name);
        getline(cin,f1.description);


        string menu= "Main Menu:\n"
                     "==========\n"
                     "1- Add in_variables.\n"
                     "2- Add fuzzy sets to an existing variable.\n"
                     "3- Add rules.\n"
                     "4- Run the simulation on crisp values.\n";

        cout << menu;

        string choice2;
        cin>>choice2;

        while (choice2!="1" && choice2!="2" && choice2!="3" && choice2!="4" && choice2!="Close")
        {
            cout<<"Wrong choice, please try again and choose in range of [1:4] or Close statement.\n";
            cin>>choice2;
            //cin.ignore();
        }

        while (choice2!="Close")
        {
            if (choice2=="1")
            {
                cout<<"Enter the variable menu name, type (IN/OUT) and range ([lower, upper]): (Press x to finish)\n";
                cout<<"------------------------------------------------------------------------------------------\n";

                fuzzy_variables variable;
                cin>>variable.name;
                while (variable.name!="x")
                {
                    char a, b, c;
                    cin>> variable.type >> a >> variable.min_range >> b >> variable.max_range >> c;
                    if(variable.type == "IN")
                        f1.in_variables.push_back(variable);
                    else if(variable.type == "OUT")
                        f1.out_variables.push_back(variable);
                    else cout<<"Invalid type.\n";

                    cin>>variable.name;
                }
                cout << menu;
                cin>>choice2;
            }
            else if (choice2 == "2")
            {
                cout<<"Enter the variable menu name: \n";
                cout<<"--------------------------\n";
                string val_name;
                cin>>val_name;

                int index = f1.checkVariables(val_name , "IN");
                string s;

                if(index != -1)
                    s = "IN";
                else
                {
                    index = f1.checkVariables(val_name , "OUT");
                    if (index != -1)
                        s = "OUT";
                }
                if (index != -1)
                {
                    string set_name , set_type;
                    double v1,v2,v3,v4;

                    cout<<"Enter the fuzzy set name, type (TRI/TRAP) and values: (Press x to finish) :\n";
                    cout<<"-----------------------------------------------------------------------------\n";
                    cin>>set_name;

                    while (set_name!="x")
                    {
                        cin>>set_type;
                        if(set_type=="TRI" and s== "IN")
                        {
                            cin>>v1>>v2>>v3;
                            double sum = (double )(v1+v2+v3)/3;
                            f1.in_variables[index].sets.push_back({set_name, f1.in_variables[index].min_range, f1.in_variables[index].max_range,sum, set_type, {v1, v2, v3}});
                        }
                        else if(set_type == "TRAP" and s == "IN")
                        {
                            cin>>v1>>v2>>v3>>v4;
                            double sum = (double )(v1+v2+v3+v4)/4;
                            f1.in_variables[index].sets.push_back({set_name, f1.in_variables[index].min_range, f1.in_variables[index].max_range,sum , set_type, {v1, v2, v3 , v4}});
                        }
                        else if (set_type=="TRI" and s== "OUT")
                        {
                            cin>>v1>>v2>>v3;
                            double sum = (double )(v1+v2+v3)/3;
                            f1.out_variables[index].sets.push_back({set_name, f1.out_variables[index].min_range, f1.out_variables[index].max_range,sum, set_type, {v1, v2, v3}});
                        }
                        else if (set_type=="TRAP" and s== "OUT")
                        {
                            cin>>v1>>v2>>v3>>v4;
                            double sum = (double )(v1+v2+v3+v4)/4;
                            f1.out_variables[index].sets.push_back({set_name, f1.out_variables[index].min_range, f1.out_variables[index].max_range,sum ,set_type, {v1, v2, v3 , v4}});
                        }
                        else cout<<"Invalid type.\n";

                        cin>>set_name;
                    }
                    cout << menu;
                    cin>>choice2;
                }
                else cout<<"Variable not found.\n";
            }
            else if(choice2=="3")
            {
                cout<<"Enter the rules in this format: (Press x to finish)\nIN_variable set operator IN_variable set => OUT_variable set\n";
                cout<<"--------------------------------------------------------------------\n";

                string rule;
                while (getline(cin, rule) && rule != "x")
                {
                    fuzzy_rule frule;
                    long long cnt = 1;
                    string currentS = "";

                    long long id = -1;
                    for(long long idx = 0 ; idx < rule.length() ; idx++)
                    {

                        if(rule[idx] == ' ')
                        {
                            if(cnt == 1)
                            {
                                id = f1.checkVariables(currentS, "IN");
                                if(id != -1)
                                {
                                    frule.variables.push_back(f1.in_variables[id]);
                                    frule.variables_indexs.push_back(id);
                                    cnt++;
                                }
                                else
                                {
                                    cout << "Invalid Input Variable, enter new rule: " << endl;
                                    cout << endl;
                                    break;
                                }

                            }
                            else if(cnt == 2)
                            {
                                int idx_set = f1.checkSet(currentS, "IN", id);
                                if(idx_set != -1)
                                {
                                    frule.sets.push_back(currentS);
                                    frule.sets_indexs.push_back(idx_set);
                                    cnt++;
                                }
                                else
                                {
                                    cout << "Invalid Input Set, enter new rule: " << endl;
                                    cout << endl;
                                    break;
                                }


                            }
                            else if(cnt == 3)
                            {
                                if(currentS != "=>")
                                {
                                    if(currentS == "and" || currentS == "or" || currentS == "and_not" || currentS == "or_not")
                                    {
                                        frule.operators.push_back(currentS);
                                        cnt = 1;
                                    }
                                    else
                                    {
                                        cout << "Invalid Operator, enter new rule: " << endl;
                                        cout << endl;
                                        break;
                                    }

                                }
                                else
                                {
                                    cnt = 4;
                                }

                            }
                            else if(cnt == 4)
                            {

                                id = f1.checkVariables(currentS, "OUT");
                                if(id != -1)
                                {
                                    frule.out_variable = currentS;
                                    frule.out_variable_idx = id;
                                    cnt++;
                                }
                                else
                                {
                                    cout << "Invalid Output Variable, enter new rule: " << endl;
                                    cout << endl;
                                    break;
                                }

                            }
                            currentS = "";
                        }
                        else
                        {
                            currentS += rule[idx];

                        }

                        if(idx == rule.length()-1)
                        {
                            int idx_set = f1.checkSet(currentS, "OUT", id);
                            if(idx_set != -1)
                            {
                                frule.out_set = currentS;
                                frule.out_set_idx = idx_set;
                                f1.rules.push_back(frule);
                            }
                            else
                            {
                                cout << "Invalid Output Set, enter new rule: " << endl;
                                cout << endl;
                                break;
                            }
                        }
                    }

                }


                cout<<menu;
                cin>>choice2;
            }
            else if (choice2 == "4")
            {
                if (f1.in_variables.empty() || f1.rules.empty())
                    cout << "CAN'T START THE SIMULATION! Please add the fuzzy sets and rules first.\n";

                else
                {
                    cout << "Enter the crisp values:\n";
                    vector<double> crisp_values(f1.in_variables.size());
                    double crisp_val;
                    for (int i = 0; i < f1.in_variables.size(); ++i)
                    {
                        cout << f1.in_variables[i].name << ": ";
                        cin >> crisp_val;
                        f1.member_ship(crisp_val , i);
                    }
                    cout << "Running the simulation...\n";
                    cout << "Fuzzification => done\n";

                    f1.inference_rule();
                    cout << "Inference => done\n";

                    double fin_ans = f1.Defuzzification();
                    cout << "Defuzzification => done\n";

                    string z = f1.get_outputSet(fin_ans);
                    cout<< "The predicted risk is " << z << "("<<fin_ans <<")"<<endl;
                }
                cout << menu;
                cin >> choice2;

            }
        }
        cout<<menu1;
        cin>>choice1;
    }
}
