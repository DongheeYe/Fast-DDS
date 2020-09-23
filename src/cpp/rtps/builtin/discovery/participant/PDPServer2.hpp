// Copyright 2020 Proyectos y Sistemas de Mantenimiento SL (eProsima).
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

/**
 * @file PDPServer2.hpp
 *
 */

#ifndef _FASTDDS_RTPS_PDPSERVER2_H_
#define _FASTDDS_RTPS_PDPSERVER2_H_
#ifndef DOXYGEN_SHOULD_SKIP_THIS_PUBLIC

#include <fastdds/rtps/builtin/discovery/participant/PDP.h>
#include "../DiscoveryDataFilter.hpp"
#include "../DiscoveryDataBase.hpp"
#include "./DServerEvent2.hpp"

// To be eventually removed together with eprosima::fastrtps
namespace aux = ::eprosima::fastrtps::rtps;

namespace eprosima {
namespace fastdds {
namespace rtps {

/**
 * Class PDPServer2 manages server side of the discovery server mechanism
 *@ingroup DISCOVERY_MODULE
 */
class PDPServer2 : public aux::PDP
{
    friend class DServerEvent2;
    friend class PDPServerListener2;

public:

    /**
     * Constructor
     * @param builtin Pointer to the BuiltinProcols object.
     * @param allocation participant's allocation settings
     * @param durability_kind the kind of persistence we want for the discovery data
     */
    PDPServer2(
            aux::BuiltinProtocols* builtin,
            const aux::RTPSParticipantAllocationAttributes& allocation);
    ~PDPServer2();

    void initializeParticipantProxyData(
            aux::ParticipantProxyData* participant_data) override;

    /**
     * Initialize the PDP.
     * @param part Pointer to the RTPSParticipant.
     * @return True on success
     */
    bool init(
            aux::RTPSParticipantImpl* part) override;

    /**
     * Creates an initializes a new participant proxy from a DATA(p) raw info
     * @param p ParticipantProxyData from DATA msg deserialization
     * @param writer_guid GUID of originating writer
     * @return new ParticipantProxyData * or nullptr on failure
     */
    aux::ParticipantProxyData* createParticipantProxyData(
            const aux::ParticipantProxyData& p,
            const aux::GUID_t& writer_guid) override;

    /**
     * Create the SPDP Writer and Reader
     * @return True if correct.
     */
    bool createPDPEndpoints() override;

    /**
     * This method removes a remote RTPSParticipant and all its writers and readers.
     * @param participant_guid GUID_t of the remote RTPSParticipant.
     * @param reason Why the participant is being removed (dropped vs removed)
     * @return true if correct.
     */
    bool remove_remote_participant(
            const aux::GUID_t& participant_guid,
            aux::ParticipantDiscoveryInfo::DISCOVERY_STATUS reason) override;

    /**
     * Force the sending of our local PDP to all servers
     * @param new_change If true a new change (with new seqNum) is created and sent; if false the last change is re-sent
     * @param dispose Sets change kind to NOT_ALIVE_DISPOSED_UNREGISTERED
     * @param wparams allows to identify the change
     */
    void announceParticipantState(
            bool new_change,
            bool dispose = false,
            aux::WriteParams& wparams = aux::WriteParams::WRITE_PARAM_DEFAULT) override;

    /**
     * These methods wouldn't be needed under perfect server operation (no need of dynamic endpoint allocation)
     * but must be implemented to solve server shutdown situations.
     * @param pdata Pointer to the RTPSParticipantProxyData object.
     */
    void assignRemoteEndpoints(
            aux::ParticipantProxyData* pdata) override;
    void removeRemoteEndpoints(
            aux::ParticipantProxyData* pdata) override;
    void notifyAboveRemoteEndpoints(
            const aux::ParticipantProxyData& pdata) override;

#if HAVE_SQLITE3
    //! Get filename for persistence database file
    std::string GetPersistenceFileName();
#endif // if HAVE_SQLITE3

    //! Wakes up the DServerEvent2 for new matching or trimming
    void awakeServerThread()
    {
        mp_sync->restart_timer();
    }

    //! Discovery database
    fastdds::rtps::DiscoveryDataBase discovery_db;

private:

    /**
     * TimedEvent for server synchronization
     */
    DServerEvent2* mp_sync;

};

} // namespace rtps
} // namespace fastdds
} // namespace eprosima
#endif // ifndef DOXYGEN_SHOULD_SKIP_THIS_PUBLIC
#endif /* _FASTDDS_RTPS_PDPSERVER2_H_ */
