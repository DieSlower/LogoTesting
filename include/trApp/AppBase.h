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
#pragma once

#include "Export.h"

#include <trBase/SmrtPtr.h>
#include <trBase/Base.h>
#include <trUtil/RefStr.h>
#include <trManager/SystemManager.h>
#include <trCore/SystemDirector.h>

namespace trApp
{
    /**
     * @class   AppBase
     *
     * @brief   An application base from which all other application classes should be derived.
     */
    class TR_APP_EXPORT AppBase : public trBase::Base
    {
    public:
        using BaseClass = trBase::Base;                 /// Adds an easy and swappable access to the base class

        const static trUtil::RefStr CLASS_TYPE;         /// Holds the class type name for efficient comparisons

        /**
         * @fn  AppBase::AppBase(const std::string& name = CLASS_TYPE);
         *
         * @brief   Default constructor.
         *
         * @param   name    (Optional) The name of the class.
         */
        AppBase(const std::string& name = CLASS_TYPE);

        /**
         * @fn  virtual const std::string& AppBase::GetType() const override;
         *
         * @brief   Gets the type.
         *
         * @return  The type.
         */
        virtual const std::string& GetType() const override;

        /**
         * @fn  virtual void AppBase::Run();
         *
         * @brief   Starts the application loop.
         */
        virtual void Run();

        /**
         * @fn  virtual void AppBase::Quit();
         *
         * @brief   Stops the application loop and exits the application.
         */
        virtual void Quit();

    protected:

        trBase::SmrtPtr<trManager::SystemManager> mSysMan;

        /**
         * @fn  AppBase::~AppBase();
         *
         * @brief   Destructor.
         */
        ~AppBase();

    private:

        
        trBase::SmrtPtr<trCore::SystemDirector> mSysDirector;

    };
}