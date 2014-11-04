// Copyright 2010-2014 Fabric Engine Inc. All rights reserved.

#ifndef __ASTWrapper_KLType__
#define __ASTWrapper_KLType__

#include "KLCommented.h"
#include "KLMethod.h"
#include "KLTypeOp.h"

#include <string>
#include <map>

namespace FabricServices
{

  namespace ASTWrapper
  {

    class KLType : public KLCommented
    {
      friend class KLFile;

    public:

      virtual ~KLType();

      const std::string & getName() const;
      virtual const char * getKLType() const = 0;
      virtual std::vector<const KLType*> getParents() const = 0;

      uint32_t getMethodCount() const;
      const KLMethod * getMethod(uint32_t index) const;
      const KLMethod * getMethod(const char * labelOrName) const;
      std::vector<const KLMethod*> getMethods(bool includeInherited = false, bool includeInternal = true, const char * category = 0) const;

      uint32_t getTypeOpCount() const;
      const KLTypeOp * getTypeOp(uint32_t index) const;
      const KLTypeOp * getTypeOp(const char * labelOrName) const;
      std::vector<const KLTypeOp*> getTypeOps() const;

      static const KLType * getKLTypeByName(const char * name);

    protected:

      KLType(const KLFile* klFile, JSONData data);
      void pushMethod(KLMethod * method) const;
      void pushTypeOp(KLTypeOp * typeOp) const;
      mutable std::vector<KLMethod*> m_methods;
      mutable std::map<std::string, uint32_t> m_methodLabelToId;
      mutable std::vector<const KLTypeOp*> m_typeOps;
      mutable std::map<std::string, uint32_t> m_typeOpLabelToId;

    private:
      
      std::string m_name;
    };

  };

};


#endif // __ASTWrapper_KLType__
