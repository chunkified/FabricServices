// Copyright 2010-2014 Fabric Engine Inc. All rights reserved.

#include "KLMethod.h"
#include "KLType.h"

using namespace FabricServices::ASTWrapper;

KLMethod::KLMethod(JSONData data, const std::string & thisType)
: KLFunction(data)
{
  const char * dictThisType = getStringDictValue("thisType");
  if(dictThisType)
    m_thisType = dictThisType;
  else
    m_thisType = thisType;

  const char * thisUsage = getStringDictValue("thisUsage");
  if(thisUsage)
    m_thisUsage = thisUsage;
  else
    m_thisUsage = "in";

  m_isVirtual = -1;
}

KLMethod::~KLMethod()
{
}

const std::string & KLMethod::getThisType() const
{
  return m_thisType;
}

const std::string & KLMethod::getThisUsage() const
{
  return m_thisUsage;
}

bool KLMethod::isMethod() const
{
  return true;
}

bool KLMethod::isVirtual() const
{
  if(m_isVirtual == -1)
  {
    m_isVirtual = 0;

    const KLType* thisType = KLType::getKLTypeByName(m_thisType.c_str());
    if(thisType)
    {
      if(std::string(thisType->getKLType()) == "interface")
      {
        const KLMethod* method = thisType->getMethod(getLabel().c_str());
        if(method == this)
          m_isVirtual = 1;
      }
      else
      {
        // todo: this will have to implement the virtual keyword at some point
        std::vector<const KLType*> parents = thisType->getParents();
        for(uint32_t i=0;i<parents.size();i++)
        {
          std::vector<const KLType*> granParents = parents[i]->getParents();
          parents.insert(parents.end(), granParents.begin(), granParents.end());

          if(std::string(parents[i]->getKLType()) == "interface")
          {
            const KLMethod* method = parents[i]->getMethod(getLabel().c_str());
            if(method == this)
            {
              m_isVirtual = 1;
              break;
            }
          }
        }
      }
    }
  }
  return m_isVirtual > 0;
}

std::string KLMethod::getPrefix() const
{
  // filter out constructors / destructors
  if(m_thisType == getName() || "~" + m_thisType == getName())
    return "";

  return m_thisType + ".";
}

std::string KLMethod::getSuffix() const
{
  if(m_thisUsage == "in")
    return "?";
  return "!";
}

const KLComment * KLMethod::getComments() const
{
  const KLComment * comments = KLFunction::getComments();
  if(comments)
    return comments;

  // todo... check the parents.... ideally walk up to the interface

  return NULL;
}
