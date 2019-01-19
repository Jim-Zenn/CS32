/*
 * SSNSet.h
 * Copyright (C) 2019 Jim Zenn <zenn@ucla.edu>
 *
 * Distributed under terms of the MIT license.
 */

#ifndef SSNSET_H
#define SSNSET_H

#include "Set.h"

class SSNSet
{
  public:
    /* SSNSet();  // Create an empty SSN set. Compiler-generated works fine */

    bool add(unsigned long ssn) { return m_set.insert(ssn); };
      // Add an SSN to the SSNSet.  Return true if and only if the SSN
      // was actually added.

    int size() const { return m_set.size(); };
      // Return the number of SSNs in the SSNSet.

    void print() const;
      // Write every SSN in the SSNSet to cout exactly once, one per
      // line.  Write no other text.

  private:
    Set m_set;
};

#endif /* !SSNSET_H */

