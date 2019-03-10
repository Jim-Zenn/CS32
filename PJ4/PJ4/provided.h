//
//  provided.h
//  PJ4
//
//  Created by Jim Zenn on 3/9/19.
//  Copyright © 2019 UCLA. All rights reserved.
//

#ifndef provided_h
#define provided_h

#include <string>
#include <vector>
#include <istream>

using namespace std;

class GenomeImpl;

class Genome
{
public:
    Genome(const string &name, const string &sequence);
    ~Genome();
    Genome(const Genome &other);
    Genome &operator=(const Genome &rhs);
    static bool load(istream &genomeSource, vector<Genome> &genomes);
    int length() const;
    string name() const;
    bool extract(int position, int length, string &fragment) const;
    
private:
    GenomeImpl *m_impl;
};

struct DNAMatch
{
    string genomeName;
    int length;
    int position;
};

struct GenomeMatch
{
    string genomeName;
    double percentMatch;
};

class GenomeMatcherImpl;

class GenomeMatcher
{
public:
    GenomeMatcher(int minSearchLength);
    ~GenomeMatcher();
    void addGenome(const Genome &genome);
    int minimumSearchLength() const;
    bool findGenomesWithThisDNA(const string &fragment, int minimumLength,
                                bool exactMatchOnly,
                                vector<DNAMatch> &matches) const;
    bool findRelatedGenomes(const Genome &query, int fragmentMatchLength,
                            bool exactMatchOnly, double matchPercentThreshold,
                            vector<GenomeMatch> &results) const;
    // We prevent a GenomeMatcher object from being copied or assigned.
    GenomeMatcher(const GenomeMatcher &) = delete;
    GenomeMatcher &operator=(const GenomeMatcher &) = delete;
    
private:
    GenomeMatcherImpl *m_impl;
};

#endif /* provided_h */
