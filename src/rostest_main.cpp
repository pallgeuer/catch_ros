
#define CATCH_CONFIG_RUNNER
#include <catch_ros/catch.hpp>

#include "ros_junit_reporter.h"

#include <boost/filesystem.hpp>

#include <ros/init.h>

namespace fs = boost::filesystem;

int main( int argc, char** argv )
{
	ros::init(argc, argv, "catch_test");

	Catch::Session session;

	// writing to session.configData() here sets defaults
	// this is the preferred way to set them

	// Parse gtest-style output options
	std::string test_output;
	for(int i = 1; i < argc; ++i)
	{
		if(strncmp(argv[i], "--gtest_output=xml:", 19) == 0)
		{
			test_output = argv[i] + 19;
		}
	}

	if(!test_output.empty())
	{
		session.configData().reporterName = "ros_junit";
		session.configData().outputFilename = test_output;
	}

	int returnCode = session.applyCommandLine( argc, argv, Catch::Session::OnUnusedOptions::Ignore );
	if( returnCode != 0 ) // Indicates a command line error
		return returnCode;

	// The catkin scripts calling tests do not create the output directory for
	// us :-(
	if(!session.configData().outputFilename.empty())
	{
		fs::path outputPath = session.configData().outputFilename;

		fs::path outputDir = outputPath.parent_path();
		if(!fs::exists(outputDir))
		{
			fs::create_directories(outputDir);
		}
	}

	return session.run();
}