//
//  main.cpp
//  PJ4
//
//  Created by Jim Zenn on 3/9/19.
//  Copyright © 2019 UCLA. All rights reserved.
//

#include <iostream>

#include "Trie.h"
#include "provided.h"

using namespace std;

int main() {
    Trie<int> t;
    t.insert("ABCD", 1);  // {1}
    t.insert("ABCD", 2);  // {1, 2}
    t.insert("ABCD", 3);  // {1, 2, 3}
    t.insert("AXCD", 4);  // {4}
    t.insert("AXCD", 5);  // {4, 5}
    t.insert("AXYD", 6);  // {6}
    t.insert("AXYD", 7);  // {6, 7}
    
    string query, result;
    
    // Trie tests
    
    // findExact tests
    
    query = "ABCD";
    result = "";
    for (auto i : t.find(query, true))
        result += to_string(i) + " ";
    assert(result == "1 2 3 ");
    
    query = "AXCD";
    result = "";
    for (auto i : t.find(query, true))
        result += to_string(i) + " ";
    assert(result == "4 5 ");
    
    query = "AXYD";
    result = "";
    for (auto i : t.find(query, true))
        result += to_string(i) + " ";
    assert(result == "6 7 ");
    
    query = "ABCE";
    result = "";
    for (auto i : t.find(query, true))
        result += to_string(i) + " ";
    assert(result == "");
    
    // findSNiP tests
    query = "ABCD";
    result = "";
    for (auto i : t.find(query, false))
        result += to_string(i) + " ";
    assert(result == "1 2 3 4 5 ");
    
    query = "AXYQ";
    result = "";
    for (auto i : t.find(query, false))
        result += to_string(i) + " ";
    assert(result == "6 7 ");
    
    query = "AXCD";
    result = "";
    for (auto i : t.find(query, false))
        result += to_string(i) + " ";
    assert(result == "1 2 3 4 5 6 7 ");
    
    query = "QBCD";
    result = "";
    for (auto i : t.find(query, false))
        result += to_string(i) + " ";
    assert(result == "");
    
    // Genome Test
    
    // extract
    
    Genome g("oryx", "GCTCGGNACACATCCGCCGCGGACGGGACGGGATTCGGGCTGTCGATTGTCTCACAGATCGTCGACGTACATGACTGGGA");
    string f1, f2, f3 = "oops";
    
    bool result1 = g.extract(0, 5, f1);
    assert(result1);
    assert(f1 == "GCTCG");
    
    bool result2 = g.extract(74, 6, f2);
    assert(result2);
    assert(f2 == "CTGGGA");
    
    bool result3 = g.extract(74, 7, f3);
    assert(!result3);
    assert(f3 == "oops");
    
    // GenomeMatcher Test
    
    vector<DNAMatch> matches;
    GenomeMatcher matcher(4);
    matcher.addGenome(Genome("Genome 1", "CGGTGTACNACGACTGGGGATAGAATATCTTGACGTCGTACCGGTTGTAGTCGTTCGACCGAAGGGTTCCGCGCCAGTAC"));
    matcher.addGenome(Genome("Genome 2", "TAACAGAGCGGTNATATTGTTACGAATCACGTGCGAGACTTAGAGCCAGAATATGAAGTAGTGATTCAGCAACCAAGCGG"));
    matcher.addGenome(Genome("Genome 3", "TTTTGAGCCAGCGACGCGGCTTGCTTAACGAAGCGGAAGAGTAGGTTGGACACATTNGGCGGCACAGCGCTTTTGAGCCA"));
    bool success;
    success = matcher.findGenomesWithThisDNA("GAAG", 4, true, matches);
    assert(success);
    assert(matches[0].genomeName == "Genome 1");
    assert(matches[0].position == 60);
    assert(matches[0].length == 4);
    assert(matches[1].genomeName == "Genome 2");
    assert(matches[1].position == 54);
    assert(matches[1].length == 4);
    assert(matches[2].genomeName == "Genome 3");
    assert(matches[2].position == 29);
    assert(matches[2].length == 4);
    success = matcher.findGenomesWithThisDNA("GAATAC", 4, true, matches);
    assert(success);
    assert(matches[0].genomeName == "Genome 1");
    assert(matches[0].position == 22);
    assert(matches[0].length == 5);
    assert(matches[1].genomeName == "Genome 2");
    assert(matches[1].position == 48);
    assert(matches[1].length == 5);
    success = matcher.findGenomesWithThisDNA("GAATAC", 6, true, matches);
    assert(!success);
    success = matcher.findGenomesWithThisDNA("GAATAC", 6, false, matches);
    assert(success);
    assert(matches[0].genomeName == "Genome 1");
    assert(matches[0].position == 22);
    assert(matches[0].length == 6);
    assert(matches[1].genomeName == "Genome 2");
    assert(matches[1].position == 48);
    assert(matches[1].length == 6);
    success = matcher.findGenomesWithThisDNA("GTATAT", 6, false, matches);
    assert(success);
    assert(matches[0].genomeName == "Genome 1");
    assert(matches[0].position == 22);
    assert(matches[0].length == 6);
    assert(matches[1].genomeName == "Genome 2");
    assert(matches[1].position == 48);
    assert(matches[1].length == 6);
    success = matcher.findGenomesWithThisDNA("GAATACG", 6, false, matches);
    assert(success);
    assert(matches[0].genomeName == "Genome 1");
    assert(matches[0].position == 22);
    assert(matches[0].length == 6);
    assert(matches[1].genomeName == "Genome 2");
    assert(matches[1].position == 48);
    assert(matches[1].length == 7);
    success = matcher.findGenomesWithThisDNA("GAAGGGTT", 5, false, matches);
    assert(success);
    assert(matches[0].genomeName == "Genome 1");
    assert(matches[0].position == 60);
    assert(matches[0].length == 8);
    assert(matches[1].genomeName == "Genome 2");
    assert(matches[1].position == 54);
    assert(matches[1].length == 5);
    assert(matches[2].genomeName == "Genome 3");
    assert(matches[2].position == 35);
    assert(matches[2].length == 7);
    success = matcher.findGenomesWithThisDNA("GAAGGGTT", 6, false, matches);
    assert(success);
    assert(matches[0].genomeName == "Genome 1");
    assert(matches[0].position == 60);
    assert(matches[0].length == 8);
    assert(matches[1].genomeName == "Genome 3");
    assert(matches[1].position == 35);
    assert(matches[1].length == 7);
    success = matcher.findGenomesWithThisDNA("ACGTGCGAGACTTAGAGCC", 12, false, matches);
    assert(success);
    assert(matches[0].genomeName == "Genome 2");
    assert(matches[0].position == 28);
    assert(matches[0].length == 19);
    success = matcher.findGenomesWithThisDNA("ACGTGCGAGACTTAGAGCG", 12, false, matches);
    assert(success);
    assert(matches[0].genomeName == "Genome 2");
    assert(matches[0].position == 28);
    assert(matches[0].length == 19);
    success = matcher.findGenomesWithThisDNA("GAAG", 3, true, matches);
    assert(!success);
    success = matcher.findGenomesWithThisDNA("GAAG", 5, true, matches);
    assert(!success);
    
    cout << "Pass all tests!" << endl;
    
    return 0;
}
