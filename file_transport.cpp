#ifndef CORRELATOR_PLUGINS_FILE_TRANSPORT_H
#define CORRELATOR_PLUGINS_FILE_TRANSPORT_H

#include <string>
#include <vector>
#include <fstream>
#include <cstdio>
#include <iostream>
#include <sys/stat.h>

#include <epl_plugin.hpp>

using com::apama::epl::EPLPlugin;
using com::apama::epl::data_t;
using com::apama::epl::list_t;
using com::apama::epl::get;

using std::ifstream;
using std::ofstream;
using std::string;

class FileTransport : public EPLPlugin<FileTransport>
{
public:
	FileTransport() 
		: base_plugin_t("FileTransportPlugin"), default_root_dir(discover_default_root_dir())
	{}
	~FileTransport() {}

	static void initialize(base_plugin_t::method_data_t &md)
	{
		md.registerMethod<decltype(&FileTransport::get_default_root_dir), &FileTransport::get_default_root_dir>("get_default_root_dir", "action<> returns string");
		md.registerMethod<decltype(&FileTransport::read), &FileTransport::read>("read", "action<string> returns sequence<string>");
		md.registerMethod<decltype(&FileTransport::write), &FileTransport::write>("write", "action<string, sequence<string>");
		md.registerMethod<decltype(&FileTransport::exists), &FileTransport::exists>("exists", "action<string> returns bool");
		md.registerMethod<decltype(&FileTransport::copy), &FileTransport::copy>("copy", "action<string, string>");
		md.registerMethod<decltype(&FileTransport::move), &FileTransport::move>("move", "action<string, string>");
		md.registerMethod<decltype(&FileTransport::remove), &FileTransport::remove>("remove", "action<string, string>");
	}

	string get_default_root_dir() { return default_root_dir; }

	list_t read(const string &path)
	{
		list_t contents;
		ifstream ifs;
		ifs.open(build_path(path));
		for(string line; std::getline(ifs, line);)
		{
			contents.push_back(line);
		}
		return contents;
	}

	void write(const string &path, const list_t &contents)
	{
		std::ofstream ofs;
		ofs.open(build_path(path));
		for(auto it = contents.begin(); it != contents.end(); ++it)
		{
			ofs << get<const char*>(*it) << std::endl;
		}
	}

	bool exists(const string &path)
	{
		struct stat buffer;
		return (stat (path.c_str(), &buffer) == 0);
	}

	void copy(const string &path, const string &target)
	{
		ifstream source(path, std::ios::binary);
		ofstream dest(target, std::ios::binary);
		dest << source.rdbuf();
		source.close();
		dest.close();
	}

	void move(const string &path, const string target)
	{
		copy(path, target);
		remove(path);
	}

	void remove(const string &path)
	{
		if (std::remove(path.c_str()) != 0) { throw std::runtime_error("Unable to remove file: " + path); }
	}

private:
	string build_path(const string &path)
	{
		return default_root_dir + path;
	}

	string discover_default_root_dir()
	{
		char* root_dir = getenv("APAMA_FILEPLUGIN_ROOT_DIR");
		if (root_dir) {
			return root_dir;
		}
		return getenv("APAMA_WORK");
	}

	string default_root_dir;

};

APAMA_DECLARE_EPL_PLUGIN(FileTransport)

#endif

