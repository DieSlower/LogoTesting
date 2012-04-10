/*
* //////////////////////////////////////C++//////////////////////////////////////
* 
* Virtual Reality Simulation Lab
* 
* This file is part of Virtual Reality Simulation Lab software. You can redistribute 
* it and/or modify it only with the authors permission. All unlicensed use and modification
* of this file are prohibited. 
* 
* This does not restrict the user from creating custom modules and additional files to 
* interface with the VR-Sim Lab.
* 
* This software was developed by Maxim Serebrennik for Various projects he worked on.
* It is sold and distributed only with his permission. 
* 
* For more information e-mail him at MaximSter@gmail.com
* 
* @author Maxim Serebrennik 
*////////////////////////////////////////////////////////////////////////////////

#ifndef COREDLL_SKYDOME_ACTOR_CPP
#define COREDLL_SKYDOME_ACTOR_CPP

#include "..\\Include\ActorSkyDome.h"

namespace vrsCore
{
	
	///////////////////////////////////////////////////////////////////////////////
	const std::string SkyDomeActor::ACTOR_DEFAULT_NAME("SkyDome Actor");
	const std::string SkyDomeActor::ACTOR_CLASS_NAME("vrsCore::SkyDomeActor");
	///////////////////////////////////////////////////////////////////////////////
	SkyDomeActor::SkyDomeActor(vrsCore::GameStaticMeshActorProxy& proxy)
	:vrsCore::GameStaticMeshActor(proxy)
	
	{
	   SetName(ACTOR_DEFAULT_NAME); //Set the instences Name
	   SetDescription(ACTOR_DEFAULT_NAME); //Set the Actor Description

	   //Set the Default Render Bin Options for this actor
	   SetRenderBin(vrsCore::FIRST_RENDER_BIN);
	   SetRenderBinSortType(vrsCore::RENDER_BIN_TYPE::DEPTH_SORTED_BIN);
	}

	///////////////////////////////////////////////////////////////////////////////
	void SkyDomeActor::OnTickLocal(const dtGame::TickMessage& tickMessage) 
	{
		mDeltaSimTime = tickMessage.GetDeltaSimTime();

		if (mPropertiesUpdated)
		{
			GetGameActorProxy().NotifyFullActorUpdate();
			mPropertiesUpdated = false;
		}
	}

	///////////////////////////////////////////////////////////////////////////////
	void SkyDomeActor::OnTickRemote(const dtGame::TickMessage& tickMessage)
	{
	   mDeltaSimTime = tickMessage.GetDeltaSimTime();
	}

	///////////////////////////////////////////////////////////////////////////////
	void SkyDomeActor::ProcessMessage(const dtGame::Message& message)
	{
		if (message.GetMessageType() == dtGame::MessageType::INFO_GAME_EVENT)
		{
			const dtGame::GameEventMessage& eventMsg = static_cast<const dtGame::GameEventMessage&>(message);

			if (eventMsg.GetGameEvent() != NULL)
			{
				/*
				if(eventMsg.GetGameEvent()->GetName() ==)
				{
					std::vector<dtCore::DeltaDrawable*> Actors;
					GetGameActorProxy().GetGameManager()->GetActorsInScene(Actors);

					std::cout << "actors" << std::endl;
				}*/
			}
		}
		else if (message.GetMessageType() == dtGame::MessageType::INFO_MAP_LOADED)
		{

		}
		else if (message.GetMessageType() == dtGame::MessageType::SYSTEM_FRAME_SYNCH)
		{
			GetGameActorProxy().GetGameManager()->GetApplication().GetCamera()->GetTransform(mCameraTransform);
			mCameraTransform.GetTranslation(mPos);

			GetTransform(mDomeTransform);
			mDomeTransform.SetTranslation(mPos);
			SetTransform(mDomeTransform);	
		}
		
	}
	///////////////////////////////////////////////////////////////////////////////
	void SkyDomeActor::OnEnteredWorld()
	{
		vrsCore::GameStaticMeshActor::OnEnteredWorld();
		
		//Set a Default Fog Color
		GetGameActorProxy().GetGameManager()->GetApplication().GetCamera()->SetClearColor(1, 1, 1, 0);
		
		//Setup the FOG options
		osg::Fog* fog = new osg::Fog();
		GetGameActorProxy().GetGameManager()->GetApplication().GetCamera()->GetOSGCamera()->getOrCreateStateSet()->setAttributeAndModes(fog,	osg::StateAttribute::OVERRIDE | osg::StateAttribute::ON);
		fog->setMode(osg::Fog::LINEAR);
		fog->setColor(osg::Vec4(1.,1.,1., 1.));
		fog->setStart(4000);
		fog->setEnd(4800);

		//Diable the Depth Buffer
		GetOSGNode()->getStateSet()->setMode(GL_DEPTH_TEST, osg::StateAttribute::OFF);
	}
	///////////////////////////////////////////////////////////////////////////////
	void SkyDomeActor::AddedToScene(dtCore::Scene* scene)
	{
		#ifdef _DEBUG
		std::cout << "SkyDomeActor::AddedToScene" << std::endl;
		#endif //_DEBUG

		vrsCore::GameStaticMeshActor::AddedToScene(scene);
	}

	///////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////
	SkyDomeActorProxy::SkyDomeActorProxy()
	{
		SetClassName(SkyDomeActor::ACTOR_CLASS_NAME); //Set the actors Class Name
	}

	///////////////////////////////////////////////////////////////////////////////
	void SkyDomeActorProxy::BuildPropertyMap()
	{
		const std::string Actor_GROUP = "SkyDome Properties"; //Property group name

		//dtDAL::ActorProxy::BuildPropertyMap();
		//TransformableActorProxy::BuildPropertyMap();
		//dtDAL::PhysicalActorProxy::BuildPropertyMap();
		//dtGame::GameActorProxy::BuildPropertyMap();
		vrsCore::GameStaticMeshActorProxy::BuildPropertyMap();

		SkyDomeActor* actor = static_cast<SkyDomeActor*>(GetActor());
	   /*
		// "Is Player" property
		AddProperty(new dtDAL::BooleanActorProperty("IsPlayer","Is Player",
			dtDAL::BooleanActorProperty::SetFuncType(actor, &SkyDomeActor::SetActorIsPlayer),
			dtDAL::BooleanActorProperty::GetFuncType(actor, &SkyDomeActor::GetActorIsPlayer),
			"Sets this actor as the main human player", Actor_GROUP));
	*/

		/*
		// Check if "static mesh" property exist.....
		dtDAL::ResourceActorProperty* ActopMeshProperty;
		GetProperty("static mesh", ActopMeshProperty);
		assert(ActopMeshProperty);

		// Automatically default the mesh to the one we want
		dtDAL::ResourceDescriptor meshDescriptor("StaticMeshes:Vehicle:Space:AWing:AWing.ive");
		ActopMeshProperty->SetValue(&meshDescriptor);
		*/
	}

	///////////////////////////////////////////////////////////////////////////////
	void SkyDomeActorProxy::CreateActor()
	{
	   SetActor(*new SkyDomeActor(*this));
	}

	///////////////////////////////////////////////////////////////////////////////
	void SkyDomeActorProxy::OnEnteredWorld()
	{
	   // Note we did not create any of these Invokables.  ProcessMessage(), TickLocal(),
	   // and TickRemote() are created for us in GameActorProxy::BuildInvokables().

	   //Register an invokable for Game Events...
	   //RegisterForMessages(dtGame::MessageType::INFO_GAME_EVENT);
	   RegisterForMessagesAboutSelf(dtGame::MessageType::INFO_GAME_EVENT);
	   RegisterForMessages(dtGame::MessageType::SYSTEM_FRAME_SYNCH);
	   RegisterForMessages(dtGame::MessageType::INFO_MAP_LOADED);

	   // Register an invokable for tick messages. Local or Remote only, not both!
	   /*if (IsRemote())
	   {
		  RegisterForMessages(dtGame::MessageType::TICK_REMOTE,
			 dtGame::GameActorProxy::TICK_REMOTE_INVOKABLE);
	   }
	   else
	   {
		  RegisterForMessages(dtGame::MessageType::TICK_LOCAL,
			 dtGame::GameActorProxy::TICK_LOCAL_INVOKABLE);
	   }

	   
	   vrsCore::GameStaticMeshActorProxy::OnEnteredWorld();*/
	}
}


#endif //COREDLL_SKYDOME_ACTOR_CPP