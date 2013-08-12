#ifndef __SIMPLE_FW_H__
#define __SIMPLE_FW_H__

//OGL Headers
#include <glload/gl_3_3.h>
#include <glload/gll.hpp>

//Windowing Headers
#include <GL/freeglut.h>

//CPP11 Headers
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>


#ifdef LOAD_X11
#define APIENTRY
#endif


namespace simple_ogl {
	/*
	struct error_t {
		error_t(std::string _s, cl_int _e) : s(_s), e(_e) {}
		void what(){
			std::cout << "ERROR: " << s << " cl_err(" << e << ")\n";
		}
		std::string s;
		cl_int      e;
	};

	inline void check_error(std::string msg, cl_int err){
		if (err != CL_SUCCESS) {
			throw  error_t(msg, err);
		}
	}

	inline void check_resource(bool cond, std::string msg, cl_int err=-1){
		if (!cond){
			throw  error_t(msg, err);
		}
	}
	*/

#define LOG_MSG(x) std::cout << x << std::endl;

        void APIENTRY debug_func(GLenum        source,
	                         GLenum        type,
	                         GLuint        id,
	                         GLenum        severity,
	                         GLsizei       length,
	                         const GLchar* message,
				 GLvoid*       userParam) {
		std::string srcName;
		switch(source)
		{
			case GL_DEBUG_SOURCE_API_ARB:             srcName = "API";             break;
			case GL_DEBUG_SOURCE_WINDOW_SYSTEM_ARB:   srcName = "Window System";   break;
			case GL_DEBUG_SOURCE_SHADER_COMPILER_ARB: srcName = "Shader Compiler"; break;
			case GL_DEBUG_SOURCE_THIRD_PARTY_ARB:     srcName = "Third Party";     break;
			case GL_DEBUG_SOURCE_APPLICATION_ARB:     srcName = "Application";     break;
			case GL_DEBUG_SOURCE_OTHER_ARB:           srcName = "Other";           break;
		}

		std::string errorType;
		switch(type)
		{
			case GL_DEBUG_TYPE_ERROR_ARB:               errorType = "Error";                    break;
			case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR_ARB: errorType = "Deprecated Functionality"; break;
			case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR_ARB:  errorType = "Undefined Behavior";       break;
			case GL_DEBUG_TYPE_PORTABILITY_ARB:         errorType = "Portability";              break;
			case GL_DEBUG_TYPE_PERFORMANCE_ARB:         errorType = "Performance";              break;
			case GL_DEBUG_TYPE_OTHER_ARB:               errorType = "Other";                    break;
		}

		std::string typeSeverity;
		switch(severity)
		{
			case GL_DEBUG_SEVERITY_HIGH_ARB:   typeSeverity = "High";   break;
			case GL_DEBUG_SEVERITY_MEDIUM_ARB: typeSeverity = "Medium"; break;
			case GL_DEBUG_SEVERITY_LOW_ARB:    typeSeverity = "Low";    break;
		}

		std::cout << errorType.c_str() << " from " <<  srcName.c_str() << ",\t" <<  typeSeverity.c_str() 
			<< " priority\nMessage: " <<  message << std::endl;
	}


	void create_window(int argc, char** argv, std::string wname, int width, int height, int x, int y, int display_mode, int major, int minor, int profile){
		glutInit(&argc,argv); //Can pass argc, argv
		glutInitDisplayMode(display_mode);
		glutInitContextVersion(major,minor);
		glutInitContextProfile(profile);

#ifdef DEBUG
		glutInitContextFlags(GLUT_DEBUG);
#endif
		glutInitWindowSize(width,height);
		glutInitWindowPosition(x,y);
		int window = glutCreateWindow(wname.c_str());

		glload::LoadFunctions();

		glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);

		if(!glload::IsVersionGEQ(major, minor))
		{
			std::cout << "Your OpenGL version is " << glload::GetMajorVersion() << ", " <<  glload::GetMinorVersion()
				<< " You must have at least OpenGL 3.3 to run this tutorial\n";
			glutDestroyWindow(window);
			exit(1);
		}

		if(glext_ARB_debug_output)
		{
			glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB);
			glDebugMessageCallbackARB(debug_func, (void*)15);
		}

	}

	GLuint create_shader(GLenum stype, const std::string filename) {
		std::ifstream fs(filename);
		std::string shader_string(std::istreambuf_iterator<char>(fs), (std::istreambuf_iterator<char>()));
		const char* shader_chars = shader_string.c_str();

		GLuint sid = glCreateShader(stype);
		glShaderSource(sid,1,&shader_chars,NULL);
		glCompileShader(sid);

		GLint status;
		glGetShaderiv(sid, GL_COMPILE_STATUS, &status);
		if(status == GL_FALSE){
			GLint info_log_length;
			glGetShaderiv(sid, GL_INFO_LOG_LENGTH, &info_log_length);

			GLchar* info_log = new GLchar[info_log_length + 1];
			glGetShaderInfoLog(sid, info_log_length, NULL, info_log);
			std::string info;
			switch(stype){
				case GL_VERTEX_SHADER:   info = "vertex";   break;
				case GL_FRAGMENT_SHADER: info = "fragment"; break;
				default:                 info = "unknown";  break;
			}
			std::cout << "Compile Error in " << info << " shader:\n" << info_log << std::endl;
			delete [] info_log;
		}
		return sid;
	}
		       
	GLuint create_program(std::vector<GLuint>& shaders){
		GLuint pid = glCreateProgram();
		std::for_each(shaders.begin(), shaders.end(), [pid] (GLuint x) -> void { glAttachShader(pid, x); });
		glLinkProgram(pid);

		GLint status;
		glGetProgramiv(pid, GL_LINK_STATUS, &status);
		if(status == GL_FALSE){
			GLint info_log_length;
			glGetProgramiv(pid, GL_INFO_LOG_LENGTH, &info_log_length);

			GLchar* info_log = new GLchar[info_log_length + 1];
			glGetProgramInfoLog(pid, info_log_length, NULL, info_log);
			std::cout << "Link Error in " << info_log << std::endl;
			delete [] info_log;
		}

		std::for_each(shaders.begin(), shaders.end(), [pid] (GLuint x) -> void { glDetachShader(pid, x); });
		std::for_each(shaders.begin(), shaders.end(), [] (GLuint x) -> void { glDeleteShader(x); });

		return pid;
	} 
	


}//namespace simple_ogl

#endif
