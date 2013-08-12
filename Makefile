CXX=        g++
BOOST_BASE= C:/MinGW/
DEFINES= -D_CRT_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_DEPRECATE -D_SCL_SECURE_NO_WARNINGS -DTIXML_USE_STL -DFREEGLUT_STATIC -DWIN32 -D_LIB -DFREEGLUT_LIB_PRAGMAS=0 -DDEBUG -D_DEBUG
CFLAGS= -MMD -MP -Wall --std=c++11 -m32 -g

OGLBASE= "C:/Users/skaul1/Desktop/OPENGL/Tutorial 0.3.8/glsdk"
IFLAGS=    \
	   -I $(OGLBASE)/glload/include \
	   -I $(OGLBASE)/glimg/include \
	   -I $(OGLBASE)/glm/include \
	   -I $(OGLBASE)/glutil/include \
	   -I $(OGLBASE)/glmesh/include \
           -I $(OGLBASE)/freeglut/include 

LFLAGS=  \
	 -L $(OGLBASE)/glload/lib \
	 -L $(OGLBASE)/glimg/lib \
	 -L $(OGLBASE)/glutil/lib \
	 -L $(OGLBASE)/glmesh/lib \
	 -L $(OGLBASE)/freeglut/lib

LIBS=   \
	-lglloadD \
	-lglimgD \
	-lglutilD \
	-lglmeshD \
	-lfreeglutD \
	-lglu32 \
	-lopengl32 \
	-lgdi32 \
	-lwinmm \
	-luser32

first.exe : first.cpp
	$(CXX) $(CFLAGS) $(DEFINES) $(IFLAGS) $< -o $@ $(LFLAGS) $(LIBS)
