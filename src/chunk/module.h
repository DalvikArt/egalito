#ifndef EGALITO_CHUNK_MODULE_H
#define EGALITO_CHUNK_MODULE_H

#include "chunk.h"
#include "chunklist.h"
#include "archive/chunktypes.h"

class Library;
class ElfSpace;
class FunctionList;
class PLTList;
class JumpTableList;
class DataRegionList;
class MarkerList;
class VTableList;
class ExternalSymbolList;

class Module : public ChunkSerializerImpl<TYPE_Module,
    CompositeChunkImpl<Chunk>> {
private:
    std::string name;
    Library *library;
    ElfSpace *elfSpace;
private:
    FunctionList *functionList;
    PLTList *pltList;
    JumpTableList *jumpTableList;
    DataRegionList *dataRegionList;
    MarkerList *markerList;
    VTableList *vtableList;
    ExternalSymbolList *externalSymbolList;
public:
    Module() : library(nullptr), elfSpace(nullptr), functionList(nullptr),
        pltList(nullptr), jumpTableList(nullptr), dataRegionList(nullptr),
        markerList(nullptr), vtableList(nullptr), externalSymbolList(nullptr) {}

    std::string getName() const { return name; }
    void setName(const std::string &name) { this->name = name; }

    void setElfSpace(ElfSpace *elfSpace);
    ElfSpace *getElfSpace() const { return elfSpace; }
    void setLibrary(Library *library);
    Library *getLibrary() const { return library; }

    FunctionList *getFunctionList() const { return functionList; }
    PLTList *getPLTList() const { return pltList; }
    JumpTableList *getJumpTableList() const { return jumpTableList; }
    DataRegionList *getDataRegionList() const { return dataRegionList; }
    MarkerList *getMarkerList() const { return markerList; }
    VTableList *getVTableList() const { return vtableList; }
    ExternalSymbolList *getExternalSymbolList() const
        { return externalSymbolList; }

    void setFunctionList(FunctionList *list) { functionList = list; }
    void setPLTList(PLTList *list) { pltList = list; }
    void setJumpTableList(JumpTableList *list) { jumpTableList = list; }
    void setDataRegionList(DataRegionList *list) { dataRegionList = list; }
    void setMarkerList(MarkerList *list) { markerList = list; }
    void setVTableList(VTableList *list) { vtableList = list; }
    void setExternalSymbolList(ExternalSymbolList *list)
        { externalSymbolList = list; }

    virtual void setSize(size_t newSize) {}  // ignored
    virtual void addToSize(diff_t add) {}  // ignored

    virtual void serialize(ChunkSerializerOperations &op,
        ArchiveStreamWriter &writer);
    virtual bool deserialize(ChunkSerializerOperations &op,
        ArchiveStreamReader &reader);

    virtual void accept(ChunkVisitor *visitor);
};

#endif
