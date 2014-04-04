#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <hwapi_service.h>
#include <stream_in.h>
#include <stream_out.h>
#include <ApiSumServiceMessages.h>

// SApiSumLinkStatusInd
TEST(utSumServiceMsgs, SerializeSApiSumLinkStatusInd)
{
// SLinkStatusIndMuksu
    SLinkStatusIndMuksu linkStatusIndMuksuOne;
    linkStatusIndMuksuOne.bbBusLinkId = EBbBusLinkId_RP30;
    linkStatusIndMuksuOne.rxLinkStatus = ERxChnlSyncStatus_WaitForIdle;
    linkStatusIndMuksuOne.txLinkStatus = ETxChnlSyncStatus_Idle;
            
    //constructor tSApiSumLinkStatusInd
    hwapi::tSApiSumLinkStatusInd tSumLinkStatusInd;
    tSumLinkStatusInd.sumServiceType = EBbBusLinkId_Cpri5;
    tSumLinkStatusInd.linkStatus = linkStatusIndMuksuOne;
    
    //call serialize to convert tSApiSumLinkStatusInd struct to bit stream
    hwapi::wrapper::stream_out out;
    hwapi::wrapper::MessageTraits<hwapi::tSApiSumLinkStatusInd>::serialize(out, tSApiSumLinkStatusInd);

    //convert bit stream to SApiSumSetSharedPortModeReq
    SApiSumSetSharedPortModeReq* sumSetSharedPortModeReqPtr = static_cast<SApiSumSetSharedPortModeReq*>(const_cast<void*>(out.get_buffer()));
    
    //checking result.......
    EXPECT_EQ(tSApiSumLinkStatusInd.sumServiceType, sumSetSharedPortModeReqPtr->linkId);
    EXPECT_EQ(tSApiSumLinkStatusInd.bbBusLinkId, sumSetSharedPortModeReqPtr->linkStatus->bbBusLinkId);
    EXPECT_EQ(tSApiSumLinkStatusInd.rxLinkStatus, sumSetSharedPortModeReqPtr->linkStatus->rxLinkStatus);
    EXPECT_EQ(tSApiSumLinkStatusInd.txLinkStatus, sumSetSharedPortModeReqPtr->linkStatus->txLinkStatus);
                
 //   EXPECT_EQ(tSApiSumLinkStatusInd.linkStatus, sumSetSharedPortModeReqPtr->portMode);
}

// SApiSumDelayValueReq  SApiSumDelayValueResp  Serialize SApiSumSetSharedPortModeReq

TEST(utSumServiceMsgs, SerializeSApiSumDelayValueReq)
{ 
    SDelayValueMuksu msgData;
    msgData.bbBusLinkId = EBbBusLinkId_Cpri5;
    
    //constructor tSApiSumDelayValueReq
    hwapi::tSApiSumDelayValueReq tSumDelayValueReq;
    tSumDelayValueReq.sumServiceType = ESumServiceType_Muksu;
    tSumDelayValueReq.data.push_back(msgData);
    
    
    u32 datas[] = {EBbBusLinkId_Cpri3, EBbBusLinkId_Cpri4};
    memcpy(tSApiSumDelayValueReq->delayValueStatus, datas,  * sizeof(SDelayValueMuksu));
    
    
    //call serialize to convert tSApiSumDelayValueReq struct to bit stream
    hwapi::wrapper::stream_out out;
    hwapi::wrapper::MessageTraits<hwapi::tSApiSumDelayValueReq>::serialize(out, tSumDelayValueReq);

    //convert bit stream to SApiSumDelayValueReq
    SApiSumDelayValueReq* sumDelayValueReqPtr = reinterpret_cast<SApiSumDelayValueReq*>(const_cast<void*>(out.get_buffer()));
    
    //checking result.......
    EXPECT_EQ(tSumDelayValueReq.sumServiceType, sumDelayValueReqPtr->sumServiceType);

    //check the first SDelayValueMuksu struct
    EXPECT_EQ(datas[0],tSumDelayValueReq.data[0].bbBusLinkId);
    EXPECT_EQ(datas[1],tSumDelayValueResp.data[1].bbBusLinkId);
}

TEST(utSumServiceMsgs, SerializeSApiSumSetSharedPortModeReq)
{
    //constructor tSApiSumSetSharedPortModeReq
    hwapi::tSApiSumSetSharedPortModeReq tSumSetSharedPortModeReq;
    tSumSetSharedPortModeReq.linkId = EBbBusLinkId_Cpri5;
    tSumSetSharedPortModeReq.portMode = ESharedPortMode_BbBus;
    
    //call serialize to convert tSApiSumSetSharedPortModeReq struct to bit stream
    hwapi::wrapper::stream_out out;
    hwapi::wrapper::MessageTraits<hwapi::tSApiSumSetSharedPortModeReq>::serialize(out, tSumSetSharedPortModeReq);

    //convert bit stream to SApiSumSetSharedPortModeReq
    SApiSumSetSharedPortModeReq* sumSetSharedPortModeReqPtr = reinterpret_cast<SApiSumSetSharedPortModeReq*>(const_cast<void*>(out.get_buffer()));
    
    //checking result.......
    EXPECT_EQ(tSumSetSharedPortModeReq.linkId, sumSetSharedPortModeReqPtr->linkId);
    EXPECT_EQ(tSumSetSharedPortModeReq.portMode, sumSetSharedPortModeReqPtr->portMode);
}

TEST(utSumServiceMsgs, DeserializeSApiSumSetSharedPortModeResp)
{
    //constructor SApiSumSetSharedPortModeResp
    unsigned char* streams_ptr = new unsigned char[sizeof(SApiSumSetSharedPortModeResp)];
    SApiSumSetSharedPortModeResp* sumSetSharedPortModeResp = (SApiSumSetSharedPortModeResp *)streams_ptr;
    sumSetSharedPortModeResp->linkId = EBbBusLinkId_Cpri0;
    sumSetSharedPortModeResp->portMode = ESharedPortMode_BbBus;
    sumSetSharedPortModeResp->status = EStatus_NoError;
    
    //convert SApiSumSetSharedPortModeResp to bit stream
    hwapi::wrapper::stream_in in(streams_ptr, sizeof(SApiSumSetSharedPortModeResp));
    hwapi::tSApiSumSetSharedPortModeResp tSumSetSharedPortModeResp;
    
    //call deserialize to convert bit stream to tSumSetSharedPortModeResp struct
    hwapi::wrapper::MessageTraits<hwapi::tSApiSumSetSharedPortModeResp>::deserialize(in, tSumSetSharedPortModeResp);
    
    //checking result..........
    EXPECT_EQ(sumSetSharedPortModeResp->linkId,tSumSetSharedPortModeResp.linkId);
    EXPECT_EQ(sumSetSharedPortModeResp->portMode,tSumSetSharedPortModeResp.portMode);
    EXPECT_EQ(sumSetSharedPortModeResp->status,tSumSetSharedPortModeResp.status);
    delete streams_ptr;
}