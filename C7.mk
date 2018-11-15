##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=C7
ConfigurationName      :=Debug
WorkspacePath          :=/home/lucas/Documentos/UTFPR/Compiladores/C7
ProjectPath            :=/home/lucas/Documentos/UTFPR/Compiladores/C7
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=Lucas Costa
Date                   :=11/15/18
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
Preprocessors          :=
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
CXXFLAGS :=  -g -O0 -Wall $(Preprocessors)
CFLAGS   :=  -g -O0 -Wall $(Preprocessors)
ASFLAGS  := 
AS       := /usr/bin/as


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=$(IntermediateDirectory)/main.cpp$(ObjectSuffix) $(IntermediateDirectory)/lexical.cpp$(ObjectSuffix) $(IntermediateDirectory)/DFA.cpp$(ObjectSuffix) $(IntermediateDirectory)/parser.cpp$(ObjectSuffix) 



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
	@test -d ./Debug || $(MakeDirCommand) ./Debug


$(IntermediateDirectory)/.d:
	@test -d ./Debug || $(MakeDirCommand) ./Debug

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


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Debug/


