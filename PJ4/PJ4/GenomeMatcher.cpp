//
//  GenomeMatcher.cpp
//  PJ4
//
//  Created by Jim Zenn on 3/9/19.
//  Copyright © 2019 UCLA. All rights reserved.
//

#include "Trie.h"
#include "provided.h"

#include <fstream>
#include <iostream>
#include <string>
#include <map>
#include <vector>

using namespace std;

class GenomeMatcherImpl {
public:
  GenomeMatcherImpl(int minSearchLength);
  void addGenome(const Genome &genome);
  int minimumSearchLength() const;
  bool findGenomesWithThisDNA(const string &fragment, int minimumLength,
                              bool exactMatchOnly,
                              vector<DNAMatch> &matches) const;
  bool findRelatedGenomes(const Genome &query, int fragmentMatchLength,
                          bool exactMatchOnly, double matchPercentThreshold,
                          vector<GenomeMatch> &results) const;

private:
  int prefixMatch(string const &a, string const &b,
                  bool const &exactMatchOnly) const;
  vector<string> fragmentGenome(Genome const &genome,
                                int const &fragmentLength) const;
  class GenomeRef {
  public:
    GenomeRef(int const &index, int const &keyPosition)
        : m_index(index), m_position(keyPosition) {}
    int name() const { return m_index; }
    int position() const { return m_position; }

  private:
    int m_index;  // genome name
    int m_position; // position of the searchKey in the genome
  };
  int const m_minimumSearchLength; // will be referred to as K in comments
  Trie<GenomeRef> m_trie;
  vector<Genome> m_library;
};

GenomeMatcherImpl::GenomeMatcherImpl(int minSearchLength)
    : m_minimumSearchLength(minSearchLength) {}

void GenomeMatcherImpl::addGenome(const Genome &genome) {
  int const index = static_cast<int>(m_library.size());
  m_library.push_back(genome);
  // iterate through every substring of length minSearchLength().
  // Each such substring will be used as a key to find this genome in the trie
  // genome index.
  string key;
  int const keyLength = minimumSearchLength();
  for (int keyPos = 0; keyPos + keyLength < genome.length(); keyPos++) {
    // extract the key
    genome.extract(keyPos, keyLength, key);
    // index the genome's reference, which contains the genome's name and
    // the index key's position in the genome
    m_trie.insert(key, GenomeRef(index, keyPos));
  }
}

int GenomeMatcherImpl::minimumSearchLength() const {
  return m_minimumSearchLength;
}

bool GenomeMatcherImpl::findGenomesWithThisDNA(
    const string &fragment, int minimumLength, bool exactMatchOnly,
    vector<DNAMatch> &matches) const {
  if (fragment.size() < minimumLength)
    return false;
  if (minimumLength < minimumSearchLength())
    return false;
  matches.clear();

  int const fragmentLength = static_cast<int>(fragment.size());
  // use the first K-chars substring of the fragment as the key to search the
  // trie index, which gives us a collection of candidate genomes.
  // These candidates contains a K-char segment which matches first K-char
  // substring of the given fragment.
  string const key = fragment.substr(0, minimumSearchLength());
  vector<GenomeRef> candidateRefs = m_trie.find(key, exactMatchOnly);
  // filter these candidates
  map<string, DNAMatch> matchMap;
  for (auto candidateRef : candidateRefs) {
    Genome const candidateGenome = m_library.at(candidateRef.name());
    int const matchPosition = candidateRef.position();
    // get a segment that matches the length of the given fragment
    // starting from the key matching position
    string candidateSegment;
    // Notice that it is possible the tail length of the genome starting from
    // the match position is shorter than the fragment length.
    int const remainingLength = candidateGenome.length() - matchPosition;
    int const candidateSegmentLength = min(remainingLength, fragmentLength);
    candidateGenome.extract(matchPosition, candidateSegmentLength,
                            candidateSegment);
    // match the longest prefix between candidateSegment and fragment
    int matchedLength = prefixMatch(candidateSegment, fragment, exactMatchOnly);
    // if the matched prefix is long enough (greater than minimumLength)
    if (matchedLength >= minimumLength) {
      DNAMatch match;
      match.genomeName = candidateGenome.name();
      match.length = matchedLength;
      match.position = matchPosition;
      // check if there is already a segment in this genome that matches
      // this fragment
      if (matchMap.find(candidateGenome.name()) == matchMap.end())
        // this is the first segment in this genome that matches the
        // given fragment; store this match
        matchMap.emplace(candidateGenome.name(), match);
      else {
        // there is already a segment in this genome that matches the
        // given fragment; in this case, store the longer segment match
        DNAMatch existingMatch = matchMap.at(candidateGenome.name());
        if (existingMatch.length < matchedLength)
          matchMap[candidateGenome.name()] = match;
      }
    }
  }
  for (auto const &pair : matchMap)
    // store the all the matches found
    matches.push_back(pair.second);
  return !matches.empty();
}

bool GenomeMatcherImpl::findRelatedGenomes(const Genome &query,
                                           int fragmentMatchLength,
                                           bool exactMatchOnly,
                                           double matchPercentThreshold,
                                           vector<GenomeMatch> &results) const {
  if (fragmentMatchLength < minimumSearchLength())
    return false;
  results.clear();
  // fragment the query genome into adjacent pieces; each with the length of
  // fragmentMatchLength.
  vector<string> const fragments = fragmentGenome(query, fragmentMatchLength);
  map<string, int> genomeMatchCountMap;
  for (auto const &fragment : fragments) {
    vector<DNAMatch> dnaMatches;
    findGenomesWithThisDNA(fragment, fragmentMatchLength, exactMatchOnly,
                           dnaMatches);
    for (auto const &dnaMatch : dnaMatches) {
      string const genomeName = dnaMatch.genomeName;
      if (genomeMatchCountMap.find(genomeName) == genomeMatchCountMap.end()) {
        // this is the first fragment match for this genome
        genomeMatchCountMap[genomeName] = 1;
      } else {
        genomeMatchCountMap[genomeName]++;
      }
    }
  }
  // calculate the match percentage for each genome
  for (auto const &pair : genomeMatchCountMap) {
    // unpack the map pair
    string const genomeName = pair.first;
    int const fragmentMatchPercentage = 100 * pair.second / fragments.size();
    // construct a genome match
    if (fragmentMatchPercentage >= matchPercentThreshold) {
      GenomeMatch genomeMatch;
      genomeMatch.genomeName = genomeName;
      genomeMatch.percentMatch = fragmentMatchPercentage;
      // store the match result
      results.push_back(genomeMatch);
    }
  }
  return !results.empty();
}

vector<string> GenomeMatcherImpl::fragmentGenome(Genome const &genome,
                                  int const &fragmentLength) const
// fragment genome into fragmentLength pieces
{
  vector<string> fragments;
  for (int fragmentStart = 0; fragmentStart + fragmentLength <= genome.length();
       fragmentStart += fragmentLength) {
    string fragment;
    genome.extract(fragmentStart, fragmentLength, fragment);
    fragments.push_back(fragment);
  }
  return fragments;
}

int GenomeMatcherImpl::prefixMatch(string const &a, string const &b,
                                   bool const &exactMatchOnly) const {
  if (a.size() == 0 || b.size() == 0)
    return 0;
  if (a[0] != b[0])
    return 0;
  int const maxMatchLength = static_cast<int>(min(a.size(), b.size()));
  // matching prefix cannot be longer then the shorter string
  bool SNiPed = exactMatchOnly ? true : false;
  // if not exactMatchOnly, the prefix has one chance to mismatch; otherwise,
  // there is no chance for SNiP at all.
  int matchedLength = 1;
  for (int i = 1; i < maxMatchLength; i++) {
    if (a[i] != b[i] && SNiPed)
      break;
    if (a[i] != b[i] && !SNiPed)
      SNiPed = true;
    matchedLength += 1;
  }
  return matchedLength;
}

//******************** GenomeMatcher functions ********************************

// These functions simply delegate to GenomeMatcherImpl's functions.
// You probably don't want to change any of this code.

GenomeMatcher::GenomeMatcher(int minSearchLength) {
  m_impl = new GenomeMatcherImpl(minSearchLength);
}

GenomeMatcher::~GenomeMatcher() { delete m_impl; }

void GenomeMatcher::addGenome(const Genome &genome) {
  m_impl->addGenome(genome);
}

int GenomeMatcher::minimumSearchLength() const {
  return m_impl->minimumSearchLength();
}

bool GenomeMatcher::findGenomesWithThisDNA(const string &fragment,
                                           int minimumLength,
                                           bool exactMatchOnly,
                                           vector<DNAMatch> &matches) const {
  return m_impl->findGenomesWithThisDNA(fragment, minimumLength, exactMatchOnly,
                                        matches);
}

bool GenomeMatcher::findRelatedGenomes(const Genome &query,
                                       int fragmentMatchLength,
                                       bool exactMatchOnly,
                                       double matchPercentThreshold,
                                       vector<GenomeMatch> &results) const {
  return m_impl->findRelatedGenomes(query, fragmentMatchLength, exactMatchOnly,
                                    matchPercentThreshold, results);
}
