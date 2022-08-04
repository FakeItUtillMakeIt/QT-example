#include "DirOperation.h"

#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>

DirOperation::DirOperation()
{
}

DirOperation::~DirOperation()
{
}

bool DirOperation::CreateDir(string path)
{
	if (path == "")
		return false;
	return create_directory(boost::filesystem::path(path));
}

string DirOperation::GetExePath()
{
	return boost::filesystem::initial_path<boost::filesystem::path>().string();
}

string DirOperation::ConcatePath(string path1, string path2)
{
	boost::filesystem::path p1(path1);
	boost::filesystem::path p2(path2);
	p1/=p2;
	return p1.string();
}