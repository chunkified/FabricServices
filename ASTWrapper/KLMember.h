// Copyright (c) 2010-2017 Fabric Software Inc. All rights reserved.

#ifndef __ASTWrapper_KLMember__
#define __ASTWrapper_KLMember__

#include "KLCommented.h"

#include <string>

namespace FabricServices
{

  namespace ASTWrapper
  {

    class KLMember : public KLCommented
    {
      friend class KLStruct;
      
    public:

      virtual ~KLMember();

      virtual KLDeclType getDeclType() const;
      virtual bool isOfDeclType(KLDeclType type) const;

      const std::string & getName() const;
      std::string getType(bool includeNameSpace = false) const;
      std::string getTypeNoArray(bool includeNameSpace = false) const;
      std::string getTypeArraySuffix() const;

    protected:

      KLMember(const KLFile* klFile, const KLNameSpace * nameSpace, JSONData data);

    private:
      
      std::string m_name;
      std::string m_type;
    };

  };

};


#endif // __ASTWrapper_KLMember__
