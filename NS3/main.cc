//The ns3 code used to simulate the environment(1AP, multiple nodes connected to it) is listed below.
// Please note that the data rate must be changed manually in the program. The data generated is stored in a CSV file called data.csv. 
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/applications-module.h"
#include "ns3/wifi-module.h"
#include "ns3/mobility-module.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/internet-module.h"
#include "ns3/flow-monitor-module.h"
#include "ns3/random-variable-stream.h"
#include "ns3/netanim-module.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace ns3;
NS_LOG_COMPONENT_DEFINE ("WifiSimulate");
int main (int argc, char *argv[])
{
	//Defining the inital parameters of the program such as payload,number of nodes	
	int numNodes = 10;
	uint32_t payloadSize = 1472 ; 
	
	//Getting user variables
	CommandLine cmd;
	cmd.AddValue ("numNodes", "Number of Nodes", numNodes);
	cmd.AddValue ("Payload", "Size of Payload", payloadSize);
	cmd.Parse (argc,argv);
	
	//The data will be stored in a file called data.csv  
	std::ofstream data_file("data.csv", std::ios::out | std::ios::app);
	data_file << "Number_of_nodes,Payload Size,PacketLoss \n";
	
	//Simulated for nNodes number of nodes
	for(int nNodes=1; nNodes<=numNodes; nNodes++)
{
	//Defining simulation starting and stop time and distance for mobility model.
	double StartTime = 0.0;
	double StopTime = 10.0;
	int distance=10;
	//Defining parameters to be used later in the program. Data Rate and maxPacket for client application.	
	StringValue DataRate;	
	DataRate = StringValue("DsssRate11Mbps");	//must be changed manually to change datarate
	uint32_t maxPacket = 10000 ;
	
	
	// Creating access point and nodes  
	NodeContainer wifiApNode;
	wifiApNode.Create (1);
	NodeContainer wifiStaNodes;
	wifiStaNodes.Create (nNodes);
		
	//Creating the physical channel
	YansWifiChannelHelper channel; 
	YansWifiPhyHelper phy = YansWifiPhyHelper::Default ();
	channel.SetPropagationDelay ("ns3::ConstantSpeedPropagationDelayModel");
	channel.AddPropagationLoss ("ns3::FriisPropagationLossModel");
	phy.SetChannel (channel.Create ());	
	
	//Setting the Wifi Standards
	WifiHelper wifi = WifiHelper::Default ();
	wifi.SetStandard (WIFI_PHY_STANDARD_80211b);
	
	
	// configuring control parameters
        wifi.SetRemoteStationManager ("ns3::ConstantRateWifiManager","DataMode", DataRate,"ControlMode", DataRate);
	NqosWifiMacHelper mac = NqosWifiMacHelper::Default ();
	
	//Defining ssid
	Ssid ssid = Ssid ("WifiSim");
	mac.SetType ("ns3::StaWifiMac","Ssid", SsidValue (ssid),"ActiveProbing", BooleanValue (false));
	NetDeviceContainer staDevices;
	staDevices = wifi.Install (phy, mac, wifiStaNodes);
	mac.SetType ("ns3::ApWifiMac","Ssid", SsidValue (ssid));
	NetDeviceContainer apDevice;
	apDevice = wifi.Install (phy, mac, wifiApNode);
	
	//Defining Mobility models	
	MobilityHelper mobility;
	MobilityHelper mobility1;
        Ptr<ListPositionAllocator> positionAlloc = CreateObject<ListPositionAllocator> ();
        positionAlloc->Add (Vector (distance, 0, 0));
        mobility1.SetPositionAllocator (positionAlloc);
	mobility1.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
	mobility.SetPositionAllocator ("ns3::GridPositionAllocator","MinX", DoubleValue (0.0),"MinY", DoubleValue (0.0),"DeltaX", 						DoubleValue (10.0),"DeltaY", DoubleValue (10.0),"GridWidth", UintegerValue 						(5),"LayoutType",StringValue ("RowFirst")); 
	mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
	mobility.Install (wifiStaNodes);
	mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
	mobility1.Install (wifiApNode);
      
	//Installing the Internet Stack over the devices
	InternetStackHelper stack;
	stack.Install (wifiApNode);
	stack.Install (wifiStaNodes);
	Ipv4AddressHelper address;
	Ipv4Address addr;
	address.SetBase ("10.1.1.0", "255.255.255.0");
	Ipv4InterfaceContainer staNodesInterface;
	Ipv4InterfaceContainer apNodeInterface;
	staNodesInterface = address.Assign (staDevices);
	apNodeInterface = address.Assign (apDevice);
	for(int i = 0 ; i < nNodes; i++)
	{
	addr = staNodesInterface.GetAddress(i);
	std::cout << " Node " << i+1 << "\t "<< "IP Address "<<addr << std::endl;
	}
	addr = apNodeInterface.GetAddress(0);
	
	
	//Installing the client and the server app on the nodes
	ApplicationContainer serverApp;
	UdpServerHelper myServer (4001);  
	serverApp = myServer.Install (wifiStaNodes.Get (0));
	serverApp.Start (Seconds(StartTime));
	serverApp.Stop (Seconds(StopTime));
	UdpClientHelper myClient (apNodeInterface.GetAddress (0), 4001);  	
	myClient.SetAttribute ("MaxPackets",UintegerValue (maxPacket)); 
	myClient.SetAttribute ("Interval", TimeValue (Time ("0.002"))); 
	myClient.SetAttribute ("PacketSize", UintegerValue (payloadSize));
	ApplicationContainer clientApp = myClient.Install (wifiStaNodes); 
	clientApp.Start (Seconds(StartTime));
	clientApp.Stop (Seconds(StopTime+5));
	std::cout << "Simulation Model Generated and UDP communication has started" << '\n';
	
	//Flow Monitor and netanim usage definition so 
	FlowMonitorHelper flowmon;
	Ptr<FlowMonitor> monitor = flowmon.InstallAll();
	Simulator::Stop (Seconds(StopTime+2));
	AnimationInterface anim("Animation.xml");
	//Start of simulation process and monitoring
	Simulator::Run ();
	monitor->CheckForLostPackets ();
	Ptr<Ipv4FlowClassifier> classifier = DynamicCast<Ipv4FlowClassifier> (flowmon.GetClassifier ());
	std::map<FlowId, FlowMonitor::FlowStats> stats = monitor->GetFlowStats ();
	double pac[100];	
	for (std::map<FlowId, FlowMonitor::FlowStats>::const_iterator i = stats.begin (); i != stats.end (); ++i)
	{
       pac[i->first]=i->second.lostPackets;
	Ipv4FlowClassifier::FiveTuple t = classifier->FindFlow (i->first);
	std::cout << "Flow " << i->first  << " (" << t.sourceAddress << " -> " << t.destinationAddress << ")\n";
	std::cout << "  Tx Bytes:   " << i->second.txBytes << "\n";
	std::cout << "  Rx Bytes:   " << i->second.rxBytes << "\n";
	std::cout << "  Packet Loss:   " << i->second.lostPackets << "\n";
	}
		
	//Calculation of average packet loss by taking average of packet loss in each node. Storing this data in a csv file.
	double avgpac=0;
	for (int i=1;i<=nNodes;++i)
	{
		avgpac = avgpac + pac[i];
	}
	avgpac=avgpac/nNodes; 
	std::cout << " Avg Packet Loss:   " << avgpac << "\n";
	data_file << nNodes<<","<<payloadSize<<","<<avgpac<<","<<"\n";

	Simulator::Destroy ();
}
		
	return 0;
}



