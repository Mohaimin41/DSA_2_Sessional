#include <iostream>
#include <fstream>
#include <random>
#include <chrono>
#include <string>
#include <set>
#include <vector>
#include <unistd.h>
#include <iomanip>
#include <tuple>

#include "my_hashtable.h"

using namespace std;

#define ll long long

ll n;
double load_factor;

ll check_hashing(ll n)
{
    vector<string> test = word_gen(n);
    set<int> bag;
    for (int i = 0; i < n; i++)
        bag.insert(hash_fun1(jenkins_hashcode(test[i]), n));

    ll res = bag.size();
    return res;
}

int main()
{
    ofstream out("out.txt", ios::out);
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();

    srand(seed);

    vector<string> t;

    unsigned long long n;

    cin >> n;
    double load_factor[] = {0.4, 0.5, 0.6, 0.7, 0.8, 0.9};

    out << fixed << setprecision(7);

    ll m = check_hashing(n);
    out << "Unique hashes: " << m << ", percentage: " << (m * 100.0) / (n * 1.0) << "%"
        << "\n";

    vector<tuple<double, ll, double, ll>> result;
    
    int indx = 0;

    for (int a = 0; a < 3; a++)
    {
        out << right;
        if (a == 0)
        {
            out << setw(40) << "Performance of linear probing in various load factors";
        }
        else if (a == 1)
        {
            out << setw(40) << "Performance of quadratic probing in various load factors";
        }
        else if (a == 2)
        {
            out << setw(40) << "Performance of double hashing in various load factors";
        }
        else
        {
            out << setw(40) << "Performance of separate chaining in various load factors";
        }
        out << "\n";
        out << right;
        out << setw(40) << "Before deletion" << setw(40) << "After deletion"
            << "\n";

        out << left;
        out << setw(15) << "Load Factor" << setw(24) << "Avg Search Time" << setw(24) << "Avg number of probes" << setw(24) << "Avg Search Time" << setw(24) << "Avg number of probes"
            << "\n";

        for (auto alpha : load_factor)
        {
            t = word_gen(n * alpha);
            ll l = n * alpha;
            out << setw(15) << alpha;
            my_hashtable test;
            if (a == 0)
            {
                test = my_hashtable(n, LINEAR);
            }
            else if (a == 1)
            {
                test = my_hashtable(n, QUAD);
            }
            else if (a == 2)
            {
                test = my_hashtable(n, DOUBLE_HASH);
            }
            else
            {
                test = my_hashtable(n, SEPARATE_CHAIN);
            }

            // insert
            for (int i = 0; i < l; i++)
                test.insert(t[i], i);

            // random 10% put in a vector;
            vector<string> srch_dump;
            for (int f = 0; f < l * .10; f++)
                srch_dump.push_back(t[rand() % (l)]);

            ll tot_probe = 0;
            auto start = chrono::high_resolution_clock::now();
            // search now
            for (auto it : srch_dump)
            {
                test.search(it);
                tot_probe += test.get_probes();
            }
            auto end = chrono::high_resolution_clock::now();
            auto dur = chrono::duration_cast<chrono::nanoseconds>(end - start);

            out << setw(24) << dur.count() / (srch_dump.size() * 1e6) << setw(24) << tot_probe / srch_dump.size();
            // store in result
            double res_dur1 = dur.count() / (srch_dump.size() * 1e6);
            ll res_prb1 = tot_probe / srch_dump.size();

            // delete now
            vector<string> dlt_dump;
            vector<bool> dlt_indx(l, true);
            for (int f = 0; f < l * .10; f++)
            {
                ll m = rand() % (l);
                dlt_dump.push_back(t[m]);
                dlt_indx[l] = false;
            }

            for (auto it : dlt_dump)
                test.remove(it);

            start = chrono::high_resolution_clock::now();
            tot_probe = 0;
            // search now
            ll sz = dlt_dump.size();
            for (int i = 0; i < sz / 2; i++)
            {
                test.search(dlt_dump[i]);
                tot_probe += test.get_probes();
            }
            // now search for non deleted item
            for (int i = 0, j = 0; j < sz / 2 && i < l; i++)
            {
                if (dlt_indx[i])
                {
                    test.search(t[i]);
                    j++;
                }
                else
                {
                    continue;
                }
            }
            end = chrono::high_resolution_clock::now();
            dur = chrono::duration_cast<chrono::nanoseconds>(end - start);

            out << setw(24) << dur.count() / (dlt_dump.size() * 1e6) << setw(24) << tot_probe / dlt_dump.size() << "\n";
            // store in result
            double res_dur2 = dur.count() / (dlt_dump.size() * 1e6);
            ll res_prb2 = tot_probe / dlt_dump.size();
            
            result.push_back(make_tuple(res_dur1, res_prb1, res_dur2, res_prb2));
            
        }

        out << "======================================================================================================\n";
    }

    out << "\n" << "\n";
    
    int j = 0;
    for (int i = 0; i < 6; i++)
    {
        out << " Performance of various collision resolution methods in load factor: " << load_factor[i] << "\n";

        out << right;
        out << setw(40) << "Before deletion" << setw(55) << "After deletion"
            << "\n";

        out << left;
        out << setw(25) << "Method" << setw(24) << "Avg Search Time" << setw(24) << "Avg number of probes" << setw(24) << "Avg Search Time" << setw(24) << "Avg number of probes"
            << "\n";
        for (; j < (i+1) * 3; j++)
        {
            if (j % 3 == 0)
            {

                out << setw(25) << "Linear Probing";
            }
            else if (j % 3 == 1)
            {
                out << setw(25) << "Quadratic Probing";
            }
            else if (j % 3 == 2)
            {
                out << setw(25) << "Double Hashing";
            }
            else
            {
                out << setw(25) << "Separate Chaining";
            }
            out << setw(25) << get<0>(result[j]) << setw(24) << get<1>(result[j]) << setw(24) << get<2>(result[j]) << setw(24) << get<3>(result[j]) << "\n";
        }
        out << "======================================================================================================\n";
    }
    out.close();
    return 0;
}