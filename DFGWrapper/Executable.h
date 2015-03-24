// Copyright 2010-2015 Fabric Software Inc. All rights reserved.

#ifndef __DFGWrapper_Executable__
#define __DFGWrapper_Executable__

#include <FabricCore.h>
#include <string>
#include <vector>

#include "Port.h"

namespace FabricServices
{

  namespace DFGWrapper
  {

    class Executable
    {
      friend class Node;
      friend class View;

    public:

      Executable(const Executable & other);
      virtual ~Executable();

      FabricCore::DFGBinding getWrappedCoreBinding() const
        { return m_binding; }
      char const *getExecPath() const
        { return m_execPath.c_str(); }

      bool isValid();

      std::string getDesc();
      std::string getObjectType();
      std::string getTitle();
      std::vector<Port> getPorts();
      Port getPort(char const * name);
      Port getPort(uint32_t index);
      Port addPort(char const *title, FabricCore::DFGPortType portType, char const *dataType = 0);
      void removePort(Port port);
      void removePort(char const * name);
      void removePort(uint32_t index);

      FEC_DFGCacheRule getCacheRule() const;
      void setCacheRule(FEC_DFGCacheRule rule);

      std::vector<std::string> getErrors();

      std::string exportJSON();

      std::string getMetadata(char const * key);
      void setMetadata(char const * key, char const * metadata, bool canUndo);

      void addExtensionDependency(char const * ext);

      std::string getImportPathname();
      void setImportPathname( char const *importPathname );

      void attachPreset(char const *parentPresetPath, char const *desiredName);

    protected:
      
      Executable();
      Executable( FabricCore::DFGBinding binding, char const *execPath );

    private:

      FabricCore::DFGBinding m_binding;
      std::string m_execPath;
      std::string m_objectType;
    };

  };

};


#endif // __DFGWrapper_Executable__
