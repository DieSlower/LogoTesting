/*
* The Construct Open Source Game and Simulation Engine
* Copyright (C) 2013
*
* This library is free software; you can redistribute it and/or modify it under
* the terms of the GNU Lesser General Public License as published by the Free
* Software Foundation; either version 2.1 of the License, or (at your option)
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
* Author: Maxim Serebrennik
*/

#ifndef COREDLL_GAME_STATIC_MESH_ACTOR_H
#define COREDLL_GAME_STATIC_MESH_ACTOR_H

#include <iostream>

#include "export.h"

#include "vrsCoreEnum.h"

#include <dtDAL/enginepropertytypes.h>
#include <dtDAL/functor.h>

#include <dtActors/gamemeshactor.h>


namespace vrsCore
{

	//A Generic Static Mesh Game Actor, that inherit dtActors::GameMeshActor, but adds Render
	//Bin Controlls to STAGE proxyes
	class CORE_EXPORT GameStaticMeshActor : public dtActors::GameMeshActor
	{
	public:

		//Default Actors Name
		static const std::string ACTOR_DEFAULT_NAME;

		//Default Actors Class Name
		static const std::string ACTOR_CLASS_NAME;

		// Constructs the actor.
		GameStaticMeshActor(dtActors::GameMeshActorProxy& proxy);
		
		//Set the Rendering Bin Number
		void SetRenderBin(int BinNum);

		//Get the Rendering Bin Number
		int GetRenderBin(void) const { return mRenderBinNum; }

		//Set the Rendering Bin Number Sort Type
		void SetRenderBinSortType(vrsCore::RENDER_BIN_TYPE& BinSortType);

		//Get the Rendering Bin Number Sort Type
		vrsCore::RENDER_BIN_TYPE& GetRenderBinSortType(void) { return *mBinSortType; }

		// Called when the actor has been added to the game manager.
		// You can respond to OnEnteredWorld on either the proxy or actor or both.
		virtual void OnEnteredWorld();

				/**
		*Calls Load File if we entered the scene....
		*/
        virtual void AddedToScene(dtCore::Scene* scene);

	//protected:
		virtual ~GameStaticMeshActor() { } ;

	private:

		//Set the RenderBin Number and Sort Type
		void	ChangeRenderBinProp(int BinNum, vrsCore::RENDER_BIN_TYPE& BinType);


		int		mRenderBinNum;		//Holds the Rendering Bin Number
									//Holds the Rendering Bin Sorting Type
		vrsCore::RENDER_BIN_TYPE*	 mBinSortType;


	};

	/**
	 * Our proxy class for the actor.  The proxy contains properties,
	 * invokables, and the actor.
	 */
	class CORE_EXPORT GameStaticMeshActorProxy : public dtActors::GameMeshActorProxy
	{
	   public:
		  // Constructs the proxy.
		  GameStaticMeshActorProxy();

		  // Creates the properties that are custom to the actor proxy.
		  virtual void BuildPropertyMap();

		  //Build Invokables to hook the proxy into the GM
		  //In this actor we will only use the default ones...
		  //virtual void BuildInvokables();

		  virtual ~GameStaticMeshActorProxy() { };

	   protected:
	      

		  // Creates an instance of our actor
		  virtual void CreateActor();

		  // Called when this proxy is added to the game manager (ie, the "world")
		  // You can respond to OnEnteredWorld on either the proxy or actor or both.
		  virtual void OnEnteredWorld();
	};
}
#endif //COREDLL_GAME_STATIC_MESH_ACTOR_H
