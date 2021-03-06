/*
* True Reality Open Source Game and Simulation Engine
* Copyright � 2018 Acid Rain Studios LLC
*
* This library is free software; you can redistribute it and/or modify it under
* the terms of the GNU Lesser General Public License as published by the Free
* Software Foundation; either version 3.0 of the License, or (at your option)
* any later version.
*
* This library is distributed in the hope that it will be useful, but WITHOUT
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
* FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
* details.
*
* You should have received a copy of the GNU Lesser General Public License
* along with this library; if not, write to the Free Software Foundation, Inc.,
* 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
*
* @author Maxim Serebrennik
*/

#include <trVersion/Utils.h>

#include <iostream>
#include <cstdlib>
#include <sstream>

#include <osg/ArgumentParser>
#include <osgDB/FileNameUtils>

#include <trUtil/Exception.h>
#include <trUtil/Logging/Log.h>

/*
 * Parses the command line variables that are passed in to the executable
 */
void ParseCmdLineArgs(int& argc, char** argv, std::string& logFileName, std::string& logLevel, bool &updateVer, bool &incVer, int &majVer, int &minVer, std::string &yymmVer, int &buildVer)
{
    osg::ArgumentParser arguments(&argc, argv);
    
    arguments.getApplicationUsage()->setApplicationName(PROGRAM_NAME);
    
    arguments.getApplicationUsage()->addCommandLineOption("\n--setBuild                 ", "Sets the softwares Build version");
    arguments.getApplicationUsage()->addCommandLineOption("\n--setYYMM                  ", "Sets the softwares YYMM version");
    arguments.getApplicationUsage()->addCommandLineOption("\n--setMinor                 ", "Sets the softwares Minor version");
    arguments.getApplicationUsage()->addCommandLineOption("\n--setMajor                 ", "Sets the softwares Major version");
    arguments.getApplicationUsage()->addCommandLineOption("\n--setVer <Maj> <Min> <YYMM> <Build>", "Sets the softwares version");
    arguments.getApplicationUsage()->addCommandLineOption("\n--incVer                   ", "Increments the softwares version");
    arguments.getApplicationUsage()->addCommandLineOption("\n--updateVer                ", "Update the softwares version using .hg Revision and current YYMM");
    arguments.getApplicationUsage()->addCommandLineOption("\n--logFileName <filename>   ", "The name of the log file to use.  Defaults to TrueRealityLog.html");
    arguments.getApplicationUsage()->addCommandLineOption("\n--logLevel <level>         ", "Logging level to use. \nLevel options are: " + trUtil::Logging::LOG_DEBUG_STR + ", " +
    trUtil::Logging::LOG_INFO_STR + ", " +
    trUtil::Logging::LOG_WARNING_STR + ", " +
    trUtil::Logging::LOG_ERROR_STR + "");
    arguments.getApplicationUsage()->addCommandLineOption("\n--help, /help, -h, /h, /?  ", "Show this help screen.");
    
    if (arguments.read("--help") == true ||
        arguments.read("/help") == true ||
        arguments.read("-h") == true ||
        arguments.read("/h") == true ||
        arguments.read("/?") == true)
    {
        arguments.getApplicationUsage()->write(std::cout);
        exit(0);
    }
    
    if (arguments.read("--updateVer") == true)
    {
        updateVer = true;
    }
    
    if (arguments.read("--incVer") == true)
    {
        incVer = true;
    }
    
    arguments.read("--logFileName", logFileName);
    arguments.read("--logLevel", logLevel);
    
    arguments.read("--setBuild", buildVer);
    arguments.read("--setYYMM",  yymmVer);
    arguments.read("--setMinor", minVer);
    arguments.read("--setMajor", majVer);
    arguments.read("--setVer", majVer, minVer, yymmVer, buildVer);
}