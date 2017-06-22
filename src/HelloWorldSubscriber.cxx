// Copyright 2016 Proyectos y Sistemas de Mantenimiento SL (eProsima).
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/*! 
 * @file HelloWorldSubscriber.cpp
 * This file contains the implementation of the subscriber functions.
 *
 * This file was generated by the tool fastcdrgen.
 */

#include <fastrtps/participant/Participant.h>
#include <fastrtps/attributes/ParticipantAttributes.h>
#include <fastrtps/subscriber/Subscriber.h>
#include <fastrtps/attributes/SubscriberAttributes.h>

#include <fastrtps/Domain.h>

#include "HelloWorldSubscriber.h"
using namespace std;


HelloWorldSubscriber::HelloWorldSubscriber() : mp_participant(nullptr), mp_subscriber(nullptr) {}

HelloWorldSubscriber::~HelloWorldSubscriber() {	Domain::removeParticipant(mp_participant);}

bool HelloWorldSubscriber::init()
{
	// Create RTPSParticipant
	
	ParticipantAttributes PParam;
	PParam.rtps.builtin.domainId = 0; //MUST BE THE SAME AS IN THE PUBLISHER
	PParam.rtps.builtin.leaseDuration = c_TimeInfinite;
	PParam.rtps.setName("Participant_subscriber"); //You can put the name you want
	mp_participant = Domain::createParticipant(PParam);
	if(mp_participant == nullptr)
			return false;
	
	//Register the type
	
	Domain::registerType(mp_participant,(TopicDataType*) &myType);		
			
	// Create Subscriber
			
	SubscriberAttributes Rparam;
	Rparam.topic.topicKind = NO_KEY;
	Rparam.topic.topicDataType = myType.getName(); //Must be registered before the creation of the subscriber
	Rparam.topic.topicName = "HelloWorldPubSubTopic";
	mp_subscriber = Domain::createSubscriber(mp_participant,Rparam,(SubscriberListener*)&m_listener);
	if(mp_subscriber == nullptr)
		return false;
	return true;
}

void HelloWorldSubscriber::SubListener::onSubscriptionMatched(Subscriber* sub,MatchingInfo& info)
{
	if (info.status == MATCHED_MATCHING)
	{
		n_matched++;
		cout << "Subscriber matched" << endl;
	}
	else
	{
		n_matched--;
		cout << "Subscriber unmatched" << endl;
	}
}

void HelloWorldSubscriber::SubListener::onNewDataMessage(Subscriber* sub)
{
		// Take data
		HelloWorld st;
		
		if(sub->takeNextData(&st, &m_info))
		{
			if(m_info.sampleKind == ALIVE)
			{
				// Print your structure data here.
				++n_msg;
				cout << "data=" << st.msg()<< endl;
				cout << "Sample received, count=" << n_msg << endl;
			}
		}
}

void HelloWorldSubscriber::run()
{
	cout << "Waiting for Data, press Enter to stop the Subscriber. "<<endl;
	std::cin.ignore();
	cout << "Shutting down the Subscriber." << endl;
}

