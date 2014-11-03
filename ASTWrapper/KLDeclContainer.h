// Copyright 2010-2014 Fabric Engine Inc. All rights reserved.

#ifndef __ASTWrapper_KLDeclContainer__
#define __ASTWrapper_KLDeclContainer__

#include "KLRequire.h"
#include "KLAlias.h"
#include "KLConstant.h"
#include "KLType.h"
#include "KLInterface.h"
#include "KLStruct.h"
#include "KLObject.h"
#include "KLFunction.h"
#include "KLOperator.h"

#include <string>

namespace FabricServices
{

  namespace ASTWrapper
  {

    class KLDeclContainer
    {
    public:

      KLDeclContainer();
      virtual ~KLDeclContainer();

      // decl vector getters
      virtual std::vector<const KLRequire*> getRequires() const = 0;
      virtual std::vector<const KLAlias*> getAliases() const = 0;
      virtual std::vector<const KLConstant*> getConstants() const = 0;
      virtual std::vector<const KLType*> getTypes() const = 0;
      virtual std::vector<const KLFunction*> getFunctions() const = 0;

      // decl vector getter overloads
      virtual std::vector<const KLInterface*> getInterfaces() const = 0;
      virtual std::vector<const KLStruct*> getStructs() const = 0;
      virtual std::vector<const KLObject*> getObjects() const = 0;
      virtual std::vector<const KLOperator*> getOperators() const = 0;

      // single decl getters
      virtual const KLConstant* getConstant(const char * name) const = 0;
      virtual const KLFunction* getFunction(const char * name) const = 0;
      virtual const KLFunction* getOperator(const char * name) const = 0;
    };

  };

};

#endif // __ASTWrapper_KLDeclContainer__
