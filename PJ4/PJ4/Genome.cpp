//
//  Genome.cpp
//  PJ4
//
//  Created by Jim Zenn on 3/9/19.
//  Copyright © 2019 UCLA. All rights reserved.
//

#include "provided.h"

#include <iostream>
#include <istream>
#include <string>
#include <vector>
using namespace std;

class GenomeImpl {
public:
  GenomeImpl(const string &name, const string &sequence);
  static bool load(istream &genomeSource, vector<Genome> &genomes);
  int length() const;
  string name() const;
  bool extract(int position, int length, string &fragment) const;

private:
  string const m_name;
  string const m_sequence;
  int const m_length;
};

// NOTE: the type of m_length should really be size_t; Unfortunately, the
// interface requires the return type of length() to be an integer. As a result
// of compromise, a static_cast is used to convert size_t to int.
GenomeImpl::GenomeImpl(const string &name, const string &sequence)
    : m_name(name), m_sequence(sequence),
      m_length(static_cast<int>(sequence.size())) {}

bool GenomeImpl::load(istream &genomeSource, vector<Genome> &genomes) {
  // states:
  // s0 file start
  // s1 was name
  // s2 was genome
  genomes.clear();
  int state = 0;
  string name = "";
  string seq = "";
  string line;
  while (getline(genomeSource, line)) {
    switch (state) {
      case 0:
        if (line.at(0) == '>') {  // name
          name = line.substr(1);
          state = 1;
        } else {  // sequence
          return false;
        }
        break;
      case 1:
        if (line.at(0) == '>') {  // name
          return false;
        } else {  // sequence
          for (auto ch : line)
            if (ch != 'A' && ch != 'T' && ch != 'C' && ch != 'G' && ch != 'N')
              return false;
          seq += line;
          state = 2;
        }
        break;
      case 2:
        if (line.at(0) == '>') {  // name
          genomes.push_back(Genome(name, seq));
          name = line.substr(1);
          seq = "";
          state = 1;
        } else {  // sequence
          for (auto ch : line)
            if (ch != 'A' && ch != 'T' && ch != 'C' && ch != 'G' && ch != 'N')
              return false;
          seq += line;
          state = 2;
        }
        break;
    }
  }
  if (!name.empty() && !seq.empty()) {
    genomes.push_back(Genome(name, seq));
  }
  return true;
}

int GenomeImpl::length() const { return m_length; }

string GenomeImpl::name() const { return m_name; }

bool GenomeImpl::extract(int pos, int len, string &fragment) const {
  if (pos + len > length())
    // cannot extract beyond the end of the genome sequence
    return false;
  fragment = m_sequence.substr(pos, len);
  return true;
}

//******************** Genome functions ************************************

// These functions simply delegate to GenomeImpl's functions.
// You probably don't want to change any of this code.

Genome::Genome(const string &nm, const string &sequence) {
  m_impl = new GenomeImpl(nm, sequence);
}

Genome::~Genome() { delete m_impl; }

Genome::Genome(const Genome &other) { m_impl = new GenomeImpl(*other.m_impl); }

Genome &Genome::operator=(const Genome &rhs) {
  GenomeImpl *newImpl = new GenomeImpl(*rhs.m_impl);
  delete m_impl;
  m_impl = newImpl;
  return *this;
}

bool Genome::load(istream &genomeSource, vector<Genome> &genomes) {
  return GenomeImpl::load(genomeSource, genomes);
}

int Genome::length() const { return m_impl->length(); }

string Genome::name() const { return m_impl->name(); }

bool Genome::extract(int position, int length, string &fragment) const {
  return m_impl->extract(position, length, fragment);
}
