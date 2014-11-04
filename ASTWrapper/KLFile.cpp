// Copyright 2010-2014 Fabric Engine Inc. All rights reserved.

#include "KLFile.h"

#include <boost/filesystem/path.hpp>

using namespace FabricServices::ASTWrapper;

KLFile::KLFile(const KLExtension* extension, const char * filePath, const char * klCode)
{
  m_extension = extension;
  m_filePath = filePath;
  m_klCode = klCode;

  const FabricCore::Client * client = m_extension->getASTManager()->getClient();

  boost::filesystem::path path = m_filePath;
  m_fileName = path.stem().string() + ".kl";

  FabricCore::RTVal jsonVal = GetKLJSONAST(*client, klCode, false);
  std::string jsonStr = jsonVal.getStringCString();

  FabricCore::Variant variant = FabricCore::Variant::CreateFromJSON(jsonStr.c_str());

  for(uint32_t i=0;i<variant.getArraySize();i++)
  {
    const FabricCore::Variant * element = variant.getArrayElement(i);
    if(!element->isDict())
      continue;
    const FabricCore::Variant * etVar = element->getDictValue("type");
    if(!etVar)
      continue;
    if(!etVar->isString())
      continue;

    std::string et = etVar->getStringData();
    if(et == "RequireGlobal")
    {
      KLRequire * e = new KLRequire(this, element);
      m_requires.push_back(e);
    }
    else if(et == "Alias")
    {
      KLAlias * e = new KLAlias(this, element);
      m_aliases.push_back(e);
    }
    else if(et == "GlobalConstDecl")
    {
      KLConstant * e = new KLConstant(this, element);
      m_constants.push_back(e);
    }
    else if(et == "Function")
    {
      KLFunction * e = new KLFunction(this, element);
      m_functions.push_back(e);
    }
    else if(et == "Operator")
    {
      KLOperator * e = new KLOperator(this, element);
      m_operators.push_back(e);
    }
    else if(et == "ASTStructDecl")
    {
      KLStruct * e = new KLStruct(this, element);
      m_types.push_back(e);
    }
    else if(et == "MethodOpImpl")
    {
      KLMethod * e = new KLMethod(this, element);
      std::string thisType = e->getThisType();
      const KLType * klType = KLType::getKLTypeByName(thisType.c_str());
      if(klType)
        klType->pushMethod(e);
      else
        m_functions.push_back(e);
    }
    else if(et == "ASTInterface")
    {
      KLInterface * e = new KLInterface(this, element);
      m_types.push_back(e);
    }
    else if(et == "ASTObjectDecl")
    {
      KLObject * e = new KLObject(this, element);
      m_types.push_back(e);
    }
    else if(et == "ComparisonOpImpl" ||
      et == "AssignOpImpl" ||
      et == "BinOpImpl" ||
      et == "ASTUniOpDecl")
    {
      KLTypeOp * e = new KLTypeOp(this, element);

      std::string thisType = e->getLhs();
      const KLType * klType = KLType::getKLTypeByName(thisType.c_str());
      if(klType)
        klType->pushTypeOp(e);
      else
        m_functions.push_back(e);
    }
    else
    {
      std::string message = "Unknown AST token '"+et+"'.";
      throw(FabricCore::Exception(message.c_str(), message.length()));
      return;
    }
  }
}

KLFile::~KLFile()
{
  for(uint32_t i=0;i<m_requires.size();i++)
    delete(m_requires[i]);
  for(uint32_t i=0;i<m_aliases.size();i++)
    delete(m_aliases[i]);
  for(uint32_t i=0;i<m_constants.size();i++)
    delete(m_constants[i]);
  for(uint32_t i=0;i<m_types.size();i++)
    delete(m_types[i]);
  for(uint32_t i=0;i<m_functions.size();i++)
    delete(m_functions[i]);
  for(uint32_t i=0;i<m_operators.size();i++)
    delete(m_operators[i]);
}

const KLExtension* KLFile::getExtension() const
{
  return m_extension;
}

const char * KLFile::getFilePath() const
{
  return m_filePath.c_str();
}

const char * KLFile::getFileName() const
{
  return m_fileName.c_str();
}

const char * KLFile::getKLCode() const
{
  return m_klCode.c_str();
}

std::vector<const KLRequire*> KLFile::getRequires() const
{
  return m_requires;
}

std::vector<const KLAlias*> KLFile::getAliases() const
{
  return m_aliases;
}

std::vector<const KLConstant*> KLFile::getConstants() const
{
  return m_constants;
}

std::vector<const KLType*> KLFile::getTypes() const
{
  return m_types;
}

std::vector<const KLFunction*> KLFile::getFunctions() const
{
  return m_functions;
}

std::vector<const KLInterface*> KLFile::getInterfaces() const
{
  std::vector<const KLInterface*> result;
  for(uint32_t i=0;i<m_types.size();i++)
  {
    if(m_types[i]->getKLType() == std::string("interface"))
      result.push_back((const KLInterface*)m_types[i]);
  }
  return result;
}

std::vector<const KLStruct*> KLFile::getStructs() const
{
  std::vector<const KLStruct*> result;
  for(uint32_t i=0;i<m_types.size();i++)
  {
    if(m_types[i]->getKLType() == std::string("struct"))
      result.push_back((const KLStruct*)m_types[i]);
  }
  return result;
}

std::vector<const KLObject*> KLFile::getObjects() const
{
  std::vector<const KLObject*> result;
  for(uint32_t i=0;i<m_types.size();i++)
  {
    if(m_types[i]->getKLType() == std::string("object"))
      result.push_back((const KLObject*)m_types[i]);
  }
  return result;
}

std::vector<const KLOperator*> KLFile::getOperators() const
{
  return m_operators;
}
