##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Release
ProjectName            :=C7
ConfigurationName      :=Release
WorkspacePath          :=/home/lucas/Documentos/UTFPR/Compiladores/C7
ProjectPath            :=/home/lucas/Documentos/UTFPR/Compiladores/C7
IntermediateDirectory  :=./Release
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=Lucas Costa
Date                   :=12/04/18
CodeLitePath           :=/home/lucas/.codelite
LinkerName             :=/usr/bin/g++
SharedObjectLinkerName :=/usr/bin/g++ -shared -fPIC
ObjectSuffix           :=.o
DependSuffix           :=.o.d
PreprocessSuffix       :=.i
DebugSwitch            :=-g 
IncludeSwitch          :=-I
LibrarySwitch          :=-l
OutputSwitch           :=-o 
LibraryPathSwitch      :=-L
PreprocessorSwitch     :=-D
SourceSwitch           :=-c 
OutputFile             :=$(IntermediateDirectory)/$(ProjectName)
Preprocessors          :=$(PreprocessorSwitch)NDEBUG 
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="C7.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). 
IncludePCH             := 
RcIncludePath          := 
Libs                   := 
ArLibs                 :=  
LibPath                := $(LibraryPathSwitch). 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := /usr/bin/ar rcu
CXX      := /usr/bin/g++
CC       := /usr/bin/gcc
CXXFLAGS :=  -O2 -Wall $(Preprocessors)
CFLAGS   :=  -O2 -Wall $(Preprocessors)
ASFLAGS  := 
AS       := /usr/bin/as


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=$(IntermediateDirectory)/main.cpp$(ObjectSuffix) $(IntermediateDirectory)/lexical.cpp$(ObjectSuffix) $(IntermediateDirectory)/DFA.cpp$(ObjectSuffix) $(IntermediateDirectory)/parser.cpp$(ObjectSuffix) $(IntermediateDirectory)/semantic.cpp$(ObjectSuffix) 



Objects=$(Objects0) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild MakeIntermediateDirs
all: $(OutputFile)

$(OutputFile): $(IntermediateDirectory)/.d $(Objects) 
	@$(MakeDirCommand) $(@D)
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	$(LinkerName) $(OutputSwitch)$(OutputFile) @$(ObjectsFileList) $(LibPath) $(Libs) $(LinkOptions)

MakeIntermediateDirs:
	@test -d ./Release || $(MakeDirCommand) ./Release


$(IntermediateDirectory)/.d:
	@test -d ./Release || $(MakeDirCommand) ./Release

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/main.cpp$(ObjectSuffix): main.cpp $(IntermediateDirectory)/main.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/lucas/Documentos/UTFPR/Compiladores/C7/main.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/main.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/main.cpp$(DependSuffix): main.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/main.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/main.cpp$(DependSuffix) -MM main.cpp

$(IntermediateDirectory)/main.cpp$(PreprocessSuffix): main.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/main.cpp$(PreprocessSuffix) main.cpp

$(IntermediateDirectory)/lexical.cpp$(ObjectSuffix): lexical.cpp $(IntermediateDirectory)/lexical.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/lucas/Documentos/UTFPR/Compiladores/C7/lexical.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/lexical.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/lexical.cpp$(DependSuffix): lexical.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/lexical.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/lexical.cpp$(DependSuffix) -MM lexical.cpp

$(IntermediateDirectory)/lexical.cpp$(PreprocessSuffix): lexical.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/lexical.cpp$(PreprocessSuffix) lexical.cpp

$(IntermediateDirectory)/DFA.cpp$(ObjectSuffix): DFA.cpp $(IntermediateDirectory)/DFA.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/lucas/Documentos/UTFPR/Compiladores/C7/DFA.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/DFA.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/DFA.cpp$(DependSuffix): DFA.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/DFA.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/DFA.cpp$(DependSuffix) -MM DFA.cpp

$(IntermediateDirectory)/DFA.cpp$(PreprocessSuffix): DFA.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/DFA.cpp$(PreprocessSuffix) DFA.cpp

$(IntermediateDirectory)/parser.cpp$(ObjectSuffix): parser.cpp $(IntermediateDirectory)/parser.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/lucas/Documentos/UTFPR/Compiladores/C7/parser.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/parser.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/parser.cpp$(DependSuffix): parser.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/parser.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/parser.cpp$(DependSuffix) -MM parser.cpp

$(IntermediateDirectory)/parser.cpp$(PreprocessSuffix): parser.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/parser.cpp$(PreprocessSuffix) parser.cpp

$(IntermediateDirectory)/semantic.cpp$(ObjectSuffix): semantic.cpp $(IntermediateDirectory)/semantic.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/lucas/Documentos/UTFPR/Compiladores/C7/semantic.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/semantic.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/semantic.cpp$(DependSuffix): semantic.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/semantic.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/semantic.cpp$(DependSuffix) -MM semantic.cpp

$(IntermediateDirectory)/semantic.cpp$(PreprocessSuffix): semantic.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/semantic.cpp$(PreprocessSuffix) semantic.cpp


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Release/


