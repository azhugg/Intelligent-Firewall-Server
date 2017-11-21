; IPHlpAPI include file by Thomas Bleeker [exagone]
; http://exagone.cjb.net
;
; !!!!!!!!!!!!!!!!!!! BETA VERSION !!!!!!!!!!!!!!!!!!!!!!!!!!!!
; There may be some big errors in this include file so it's just
; a beta so I could include it into my example zip file. Final
; version will be released later
;
AddIPAddress PROTO :DWORD,:DWORD,:DWORD,:DWORD,:DWORD
AllocateAndGetArpEntTableFromStack PROTO :DWORD,:DWORD,:DWORD,:DWORD,:DWORD
AllocateAndGetIfTableFromStack PROTO :DWORD,:DWORD,:DWORD,:DWORD,:DWORD
AllocateAndGetIpAddrTableFromStack PROTO :DWORD,:DWORD,:DWORD,:DWORD
AllocateAndGetIpForwardTableFromStack PROTO :DWORD,:DWORD,:DWORD,:DWORD
AllocateAndGetIpNetTableFromStack PROTO :DWORD,:DWORD,:DWORD,:DWORD,:DWORD
AllocateAndGetTcpExTableFromStack PROTO :DWORD,:DWORD,:DWORD,:DWORD
AllocateAndGetTcpTableFromStack PROTO :DWORD,:DWORD,:DWORD,:DWORD
AllocateAndGetUdpExTableFromStack PROTO :DWORD,:DWORD,:DWORD,:DWORD
AllocateAndGetUdpTableFromStack PROTO :DWORD,:DWORD,:DWORD,:DWORD
CreateIpForwardEntry PROTO :DWORD
CreateIpNetEntry PROTO :DWORD
CreateProxyArpEntry PROTO :DWORD,:DWORD,:DWORD
DeleteIPAddress PROTO :DWORD
DeleteIpForwardEntry PROTO :DWORD
DeleteIpNetEntry PROTO :DWORD
DeleteProxyArpEntry PROTO :DWORD,:DWORD,:DWORD
DisableMediaSense PROTO :DWORD,:DWORD
EnableRouter PROTO :DWORD,:DWORD
FlushIpNetTable PROTO :DWORD
FlushIpNetTableFromStack PROTO :DWORD
GetAdapterIndex PROTO :DWORD,:DWORD
GetAdapterOrderMap PROTO
GetAdaptersAddresses PROTO :DWORD,:DWORD,:DWORD,:DWORD,:DWORD
GetAdaptersInfo PROTO :DWORD,:DWORD
GetBestInterface PROTO :DWORD,:DWORD
GetBestInterfaceFromStack PROTO :DWORD,:DWORD
GetBestRoute PROTO :DWORD,:DWORD,:DWORD
GetBestRouteFromStack PROTO :DWORD,:DWORD,:DWORD
GetFriendlyIfIndex PROTO :DWORD
GetIcmpStatistics PROTO :DWORD
GetIcmpStatsFromStack PROTO :DWORD
GetIfEntry PROTO :DWORD
GetIfEntryFromStack PROTO :DWORD,:DWORD,:DWORD
GetIfTable PROTO :DWORD,:DWORD,:DWORD
GetIfTableFromStack PROTO :DWORD,:DWORD,:DWORD,:DWORD
GetIgmpList PROTO :DWORD,:DWORD,:DWORD
GetInterfaceInfo PROTO :DWORD,:DWORD
GetIpAddrTable PROTO :DWORD,:DWORD,:DWORD
GetIpAddrTableFromStack PROTO :DWORD,:DWORD,:DWORD
GetIpForwardTable PROTO :DWORD,:DWORD,:DWORD
GetIpForwardTableFromStack PROTO :DWORD,:DWORD,:DWORD
GetIpNetTable PROTO :DWORD,:DWORD,:DWORD
GetIpNetTableFromStack PROTO :DWORD,:DWORD,:DWORD,:DWORD
GetIpStatistics PROTO :DWORD
GetIpStatsFromStack PROTO :DWORD
GetNetworkParams PROTO :DWORD,:DWORD
GetNumberOfInterfaces PROTO :DWORD
GetPerAdapterInfo PROTO :DWORD,:DWORD,:DWORD
GetRTTAndHopCount PROTO :DWORD,:DWORD,:DWORD,:DWORD
GetTcpStatistics PROTO :DWORD
GetTcpStatsFromStack PROTO :DWORD
GetTcpTable PROTO :DWORD,:DWORD,:DWORD
GetTcpTableFromStack PROTO :DWORD,:DWORD,:DWORD
GetUdpStatistics PROTO :DWORD
GetUdpStatsFromStack PROTO :DWORD
GetUdpTable PROTO :DWORD,:DWORD,:DWORD
GetUdpTableFromStack PROTO :DWORD,:DWORD,:DWORD
GetUniDirectionalAdapterInfo PROTO :DWORD,:DWORD
IcmpCloseHandle PROTO :DWORD
IcmpCreateFile PROTO
IcmpParseReplies PROTO :DWORD,:DWORD
IcmpSendEcho PROTO :DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD
IcmpSendEcho2 PROTO :DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD
InternalCreateIpForwardEntry PROTO :DWORD
InternalCreateIpNetEntry PROTO :DWORD
InternalDeleteIpForwardEntry PROTO :DWORD
InternalDeleteIpNetEntry PROTO :DWORD
InternalGetIfTable PROTO :DWORD,:DWORD,:DWORD
InternalGetIpAddrTable PROTO :DWORD,:DWORD,:DWORD
InternalGetIpForwardTable PROTO :DWORD,:DWORD,:DWORD
InternalGetIpNetTable PROTO :DWORD,:DWORD,:DWORD
InternalGetTcpTable PROTO :DWORD,:DWORD,:DWORD
InternalGetUdpTable PROTO :DWORD,:DWORD,:DWORD
InternalSetIfEntry PROTO :DWORD
InternalSetIpForwardEntry PROTO :DWORD
InternalSetIpNetEntry PROTO :DWORD
InternalSetIpStats PROTO :DWORD
InternalSetTcpEntry PROTO :DWORD
IpReleaseAddress PROTO :DWORD
IpRenewAddress PROTO :DWORD
IsLocalAddress PROTO :DWORD
NTPTimeToNTFileTime PROTO :DWORD,:DWORD,:DWORD
NTTimeToNTPTime PROTO :DWORD,:DWORD
NhGetGuidFromInterfaceName PROTO :DWORD,:DWORD,:DWORD,:DWORD
NhGetInterfaceNameFromDeviceGuid PROTO :DWORD,:DWORD,:DWORD,:DWORD,:DWORD
NhGetInterfaceNameFromGuid PROTO :DWORD,:DWORD,:DWORD,:DWORD,:DWORD
NhpAllocateAndGetInterfaceInfoFromStack PROTO :DWORD,:DWORD,:DWORD,:DWORD,:DWORD
NhpGetInterfaceIndexFromStack PROTO :DWORD,:DWORD
NotifyAddrChange PROTO :DWORD,:DWORD
NotifyRouteChange PROTO :DWORD,:DWORD
NotifyRouteChangeEx PROTO :DWORD,:DWORD,:DWORD
PfAddFiltersToInterface PROTO :DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD
PfAddGlobalFilterToInterface PROTO :DWORD,:DWORD
PfBindInterfaceToIPAddress PROTO :DWORD,:DWORD,:DWORD
PfBindInterfaceToIndex PROTO :DWORD,:DWORD,:DWORD,:DWORD
PfCreateInterface PROTO :DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD
PfDeleteInterface PROTO :DWORD
PfDeleteLog PROTO
PfGetInterfaceStatistics PROTO :DWORD,:DWORD,:DWORD,:DWORD
PfMakeLog PROTO :DWORD
PfRebindFilters PROTO :DWORD,:DWORD
PfRemoveFilterHandles PROTO :DWORD,:DWORD,:DWORD
PfRemoveFiltersFromInterface PROTO :DWORD,:DWORD,:DWORD,:DWORD,:DWORD
PfRemoveGlobalFilterFromInterface PROTO :DWORD,:DWORD
PfSetLogBuffer PROTO :DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD
PfTestPacket PROTO :DWORD,:DWORD,:DWORD,:DWORD,:DWORD
PfUnBindInterface PROTO :DWORD
RestoreMediaSense PROTO :DWORD,:DWORD
SendARP PROTO :DWORD,:DWORD,:DWORD,:DWORD
SetAdapterIpAddress PROTO :DWORD,:DWORD,:DWORD,:DWORD,:DWORD
SetBlockRoutes PROTO :DWORD,:DWORD,:DWORD
SetIfEntry PROTO :DWORD
SetIfEntryToStack PROTO :DWORD,:DWORD
SetIpForwardEntry PROTO :DWORD
SetIpForwardEntryToStack PROTO :DWORD
SetIpMultihopRouteEntryToStack PROTO :DWORD
SetIpNetEntry PROTO :DWORD
SetIpNetEntryToStack PROTO :DWORD,:DWORD
SetIpRouteEntryToStack PROTO :DWORD
SetIpStatistics PROTO :DWORD
SetIpStatsToStack PROTO :DWORD
SetIpTTL PROTO :DWORD
SetProxyArpEntryToStack PROTO :DWORD,:DWORD,:DWORD,:DWORD,:DWORD
SetRouteWithRef PROTO :DWORD
SetTcpEntry PROTO :DWORD
SetTcpEntryToStack PROTO :DWORD
UnenableRouter PROTO :DWORD,:DWORD
do_echo_rep PROTO :DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD
do_echo_req PROTO :DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD
register_icmp PROTO


MAX_ADAPTER_DESCRIPTION_LENGTH  	equ		128
MAX_ADAPTER_NAME_LENGTH         	equ		256
MAX_ADAPTER_ADDRESS_LENGTH      	equ		8  
MAX_ADAPTER_NAME 					equ		128
DEFAULT_MINIMUM_ENTITIES        	equ		32 
MAX_HOSTNAME_LEN                	equ		128
MAX_DOMAIN_NAME_LEN             	equ		128
MAX_SCOPE_ID_LEN               		equ		256

IFNDEF SOCKET_ADDRESS
	SOCKET_ADDRESS STRUCT
	   lpSockaddr 		dd		?
	   iSockaddrLength 	dd		?
	SOCKET_ADDRESS ENDS
ENDIF

 BROADCAST_NODETYPE             equ 1
 PEER_TO_PEER_NODETYPE          equ 2
 MIXED_NODETYPE                 equ 4
 HYBRID_NODETYPE                equ 8

; IP_ADDRESS_STRING - store an IP address as a dotted decimal string
IP_ADDRESS_STRING STRUCT
	String	db	4*4 dup (?)
IP_ADDRESS_STRING ENDS

IP_MASK_STRING textequ <IP_ADDRESS_STRING>


; IP_ADDR_STRING - store an IP address with its corresponding subnet mask,
; both as dotted decimal strings

IP_ADDR_STRING STRUCT
    Next		dd				   ? 	;ptr to IP_ADDR_STRING
    IpAddress	IP_ADDRESS_STRING <4*4 dup (?)>	;
    IpMask		IP_MASK_STRING	  <>	
	Context		dd				   ?
IP_ADDR_STRING ENDS

; ADAPTER_INFO - per-adapter information. All IP addresses are stored as

IP_ADAPTER_INFO STRUCT DWORD
    Next				dd		?		;ptr to IP_ADAPTER_INFO
    ComboIndex			dd		?	
    AdapterName			db		MAX_ADAPTER_NAME_LENGTH + 4 dup (?)
    Description			db		MAX_ADAPTER_DESCRIPTION_LENGTH + 4 dup (?)
    AddressLength		dd		?
    Address				db		MAX_ADAPTER_ADDRESS_LENGTH dup (?)
    Index				dd		?
    uType				dd		?
    DhcpEnabled			dd		?
    CurrentIpAddress	dd		?		;ptr to IP_ADDR_STRING
    IpAddressList		IP_ADDR_STRING <?>
    GatewayList         IP_ADDR_STRING <?>
    DhcpServer          IP_ADDR_STRING <?>
    HaveWins			dd		?
    PrimaryWinsServer	IP_ADDR_STRING <?>
    SecondaryWinsServer	IP_ADDR_STRING <?>
    LeaseObtained		dd		?	
    LeaseExpires		dd		?
IP_ADAPTER_INFO ENDS


IpPrefixOriginOther 			  equ		0
IpPrefixOriginManual			  equ		1
IpPrefixOriginWellKnown			  equ		2
IpPrefixOriginDhcp				  equ		3
IpPrefixOriginRouterAdvertisement equ		4


IpSuffixOriginOther 				equ		0
IpSuffixOriginManual                equ		1
IpSuffixOriginWellKnown             equ		2
IpSuffixOriginDhcp                  equ		3
IpSuffixOriginLinkLayerAddress      equ		4
IpSuffixOriginRandom				equ		5


IpDadStateInvalid        		equ		0
IpDadStateTentative             equ		1
IpDadStateDuplicate             equ		2
IpDadStateDeprecated            equ		3
IpDadStatePreferred             equ		4


IP_ADAPTER_UNICAST_ADDRESS STRUCT
    UNION
        Alignment	dq		?
        STRUCT
           	dwLength	dd		?
            Flags	dd		?
        ENDS
    ENDS
    Next		dd		?		;ptr to IP_ADAPTER_UNICAST_ADDRESS struct
    Address		SOCKET_ADDRESS	<?>
    PrefixOrigin	dd		?
    SuffixOrigin	dd		?
    DadState		dd		?

    ValidLifetime	dd		?
    PreferredLifetime dd	?
    LeaseLifetime	dd		?
IP_ADAPTER_UNICAST_ADDRESS ENDS

IP_ADAPTER_ANYCAST_ADDRESS STRUCT
    UNION
        Alignment	dq		?
        STRUCT
            dwLength	dd		?
            Flags	dd		?
        ENDS
    ENDS
    Next		dd		?		;ptr to IP_ADAPTER_ANYCAST_ADDRESS struct
    Address		SOCKET_ADDRESS <?>
IP_ADAPTER_ANYCAST_ADDRESS ENDS

IP_ADAPTER_MULTICAST_ADDRES textequ <IP_ADAPTER_ANYCAST_ADDRESS>


IP_ADAPTER_ADDRESS_DNS_ELIGIBLE 		equ	1
IP_ADAPTER_ADDRESS_TRANSIENT    		equ 2

IP_ADAPTER_ADDRESSES STRUCT
    UNION
        Alignment	dq		?
        STRUCT
            dwLength	dd		?
            IfIndex	dd		?
        ENDS
    ENDS
    Next			dd		?		;ptr to IP_ADAPTER_ADDRESSES struct
    AdapterName		db		MAX_ADAPTER_NAME_LENGTH + 4 dup (?)
    FirstUnicastAddress	dd	?		;ptr to IP_ADAPTER_UNICAST_ADDRESS struct
    
    FirstAnycastAddress   dd ?		;ptr to IP_ADAPTER_UNICAST_ADDRESS struct
    FirstMulticastAddress dd ?		;ptr to IP_ADAPTER_UNICAST_ADDRESS struct
IP_ADAPTER_ADDRESSES ENDS

GAA_FLAG_SKIP_UNICAST    	equ	1
GAA_FLAG_SKIP_ANYCAST      	equ	2
GAA_FLAG_SKIP_MULTICAST    	equ	3


;IP_PER_ADAPTER_INFO - per-adapter IP information such as DNS server list.


IP_PER_ADAPTER_INFO STRUCT
    AutoconfigEnabled		dd		?
    AutoconfigActive		dd		?
    CurrentDnsServer		dd		? ;ptr to IP_ADDR_STRING struct
    DnsServerList		IP_ADDR_STRING <?>
IP_PER_ADAPTER_INFO ENDS


; FIXED_INFO - the set of IP-related information which does not depend on DHCP

FIXED_INFO	STRUCT
    HostName	db	MAX_HOSTNAME_LEN + 4 dup (?)
    DomainName	db	MAX_DOMAIN_NAME_LEN + 4 dup (?)
    CurrentDnsServer	dd		?		;ptr to IP_ADDR_STRING struct
    DnsServerList		IP_ADDR_STRING <?>
    NodeType		dd		?
    ScopeId			db		MAX_SCOPE_ID_LEN + 4 dup (?)
    EnableRouting	dd		?
    EnableProxy		dd		?
    EnableDns		dd		?
FIXED_INFO	ENDS


IP_ADAPTER_INDEX_MAP STRUCT
    Index		dd		?
    Name		dw	MAX_ADAPTER_NAME dup (?)		;or pointer??
IP_ADAPTER_INDEX_MAP ENDS


IP_INTERFACE_INFO STRUCT
    Adapter IP_ADAPTER_INDEX_MAP <?>
IP_INTERFACE_INFO ENDS

IP_UNIDIRECTIONAL_ADAPTER_ADDRESS STRUCT
    NumAdapters		dd		?
    Address			dd		?		;IP address as dword
IP_UNIDIRECTIONAL_ADAPTER_ADDRESS ENDS


IF_OTHER_ADAPTERTYPE 		equ	0
IF_ETHERNET_ADAPTERTYPE 	equ 1
IF_TOKEN_RING_ADAPTERTYPE 	equ	2
IF_FDDI_ADAPTERTYPE 		equ	3
IF_PPP_ADAPTERTYPE 			equ	4
IF_LOOPBACK_ADAPTERTYPE 	equ 5
IF_SLIP_ADAPTERTYPE 		equ 6 

