/* Copyright (C) 2015 Henrik Larsson
This software may be distributed and modified under the terms of the GNU
General Public License version 2 (GPL2) as published by the Free Software
Foundation and appearing in the file GPL2.TXT included in the packaging of
this file. Please note that GPL2 Section 2[b] requires that all works based
on this software must also be made publicly available under the terms of
the GPL2 ("Copyleft").

Henrik Larsson
Email: laselase (a) gmail.com
*/
#include "CP210x.h"

const uint8_t CP210x::epDataInIndex = 1;
const uint8_t CP210x::epDataOutIndex = 2;
const uint8_t CP210x::epInterruptInIndex = 3;

CP210x::CP210x(USB *p, CP210xAsyncOper *pasync) :
pAsync(pasync),
pUsb(p),
bAddress(0),
bNumEP(1)
{
        for(uint8_t i = 0; i < CP210x_MAX_ENDPOINTS; i++) {
                epInfo[i].epAddr = 0;
                epInfo[i].maxPktSize = (i) ? 0 : 8;
//                epInfo[i].epAttribs = 0;
		epInfo[i].bmSndToggle   = 0;
		epInfo[i].bmRcvToggle   = 0;
                epInfo[i].bmNakPower = (i==epDataInIndex) ? USB_NAK_NOWAIT: USB_NAK_MAX_POWER;
        }
        if(pUsb)
        {
                pUsb->RegisterDeviceClass(this);
        }
}

uint8_t CP210x::Init(uint8_t parent, uint8_t port, bool lowspeed) {
        const uint8_t constBufSize = sizeof (USB_DEVICE_DESCRIPTOR);

        uint8_t buf[constBufSize];
        USB_DEVICE_DESCRIPTOR * udd = reinterpret_cast<USB_DEVICE_DESCRIPTOR*>(buf);
        uint8_t rcode;
        UsbDevice *p = NULL;
        EpInfo *oldep_ptr = NULL;

        uint8_t num_of_conf; // number of configurations

        AddressPool &addrPool = pUsb->GetAddressPool();

        USBTRACE("CP210x Init\r\n");

        if(bAddress)
                return USB_ERROR_CLASS_INSTANCE_ALREADY_IN_USE;
      
        // Get pointer to pseudo device with address 0 assigned
        p = addrPool.GetUsbDevicePtr(0);

        if(!p)
                return USB_ERROR_ADDRESS_NOT_FOUND_IN_POOL;

        if(!p->epinfo) {
                USBTRACE("epinfo\r\n");
                return USB_ERROR_EPINFO_IS_NULL;
        }

        // Save old pointer to EP_RECORD of address 0
        oldep_ptr = p->epinfo;

        // Temporary assign new pointer to epInfo to p->epinfo in order to avoid toggle inconsistence
        p->epinfo = epInfo;

        p->lowspeed = lowspeed;

        // Get device descriptor
        rcode = pUsb->getDevDescr(0, 0, sizeof (USB_DEVICE_DESCRIPTOR), buf);
Serial.print("init 1: "); Serial.println(rcode);
        // Restore p->epinfo
        p->epinfo = oldep_ptr;

        if(rcode)
                goto FailGetDevDescr;
        if(!VIDPIDOK(udd->idVendor, udd->idProduct))
                return USB_DEV_CONFIG_ERROR_DEVICE_NOT_SUPPORTED;

        // Save type of FTDI chip
        //wFTDIType = udd->bcdDevice;

        // Allocate new address according to device class
        bAddress = addrPool.AllocAddress(parent, false, port);

        if(!bAddress)
                return USB_ERROR_OUT_OF_ADDRESS_SPACE_IN_POOL;

        // Extract Max Packet Size from the device descriptor
        epInfo[0].maxPktSize = udd->bMaxPacketSize0;

        // Assign new address to the device
        rcode = pUsb->setAddr(0, 0, bAddress);

        if(rcode) {
                p->lowspeed = false;
                addrPool.FreeAddress(bAddress);
                bAddress = 0;
                USBTRACE2("setAddr:", rcode);
                return rcode;
        }

        USBTRACE2("Addr:", bAddress);

        p->lowspeed = false;
      
        p = addrPool.GetUsbDevicePtr(bAddress);

        if(!p)
                return USB_ERROR_ADDRESS_NOT_FOUND_IN_POOL;

        p->lowspeed = lowspeed;

        num_of_conf = udd->bNumConfigurations;

        // Assign epInfo to epinfo pointer
        rcode = pUsb->setEpInfoEntry(bAddress, 1, epInfo);

        if(rcode)
                goto FailSetDevTblEntry;

        USBTRACE2("NC:", num_of_conf);

        for(uint8_t i = 0; i < num_of_conf; i++) {
                ConfigDescParser < 0xFF, 0x00, 0x00, CP_MASK_COMPARE_ALL> confDescrParser(this);

                rcode = pUsb->getConfDescr(bAddress, 0, i, &confDescrParser);

                if(rcode)
                        goto FailGetConfDescr;

                if(bNumEP > 1)
                        break;
        } // for

        if(bNumEP < 2) {
                return USB_DEV_CONFIG_ERROR_DEVICE_NOT_SUPPORTED;
        }

        USBTRACE2("NumEP:", bNumEP);

        // Assign epInfo to epinfo pointer
        rcode = pUsb->setEpInfoEntry(bAddress, bNumEP, epInfo);

        USBTRACE2("Conf:", bConfNum);

        // Set Configuration Value
        rcode = pUsb->setConf(bAddress, 0, bConfNum);

        if(rcode)
                goto FailSetConfDescr;

        rcode = pAsync->OnInit(this);

        if(rcode)
                goto FailOnInit;

        USBTRACE("CP210x configured\r\n");
      
        bPollEnable = true;
        return 0;

FailGetDevDescr:
#ifdef DEBUG_USB_HOST
        NotifyFailGetDevDescr();
        goto Fail;
#endif

FailSetDevTblEntry:
#ifdef DEBUG_USB_HOST
        NotifyFailSetDevTblEntry();
        goto Fail;
#endif

FailGetConfDescr:
#ifdef DEBUG_USB_HOST
        NotifyFailGetConfDescr();
        goto Fail;
#endif

FailSetConfDescr:
#ifdef DEBUG_USB_HOST
        NotifyFailSetConfDescr();
        goto Fail;
#endif

FailOnInit:
#ifdef DEBUG_USB_HOST
        USBTRACE("OnInit:");

Fail:
        NotifyFail(rcode);
#endif
        Release();
        return rcode;
}

void CP210x::EndpointXtract(uint8_t conf, uint8_t iface, uint8_t alt, uint8_t proto, const USB_ENDPOINT_DESCRIPTOR *pep) {
        ErrorMessage<uint8_t > (PSTR("Conf.Val"), conf);
        ErrorMessage<uint8_t > (PSTR("Iface Num"), iface);
        ErrorMessage<uint8_t > (PSTR("Alt.Set"), alt);
        bConfNum = conf;

        uint8_t index;

        if((pep->bmAttributes & 0x03) == 3 && (pep->bEndpointAddress & 0x80) == 0x80) {
                index = epInterruptInIndex;
        }
        else
        {
                if((pep->bmAttributes & 0x02) == 2)
                {
                    index = ((pep->bEndpointAddress & 0x80) == 0x80) ? epDataInIndex : epDataOutIndex;
                }
                else
                {
                    return;
                }
        }
        // Fill in the endpoint info structure
        epInfo[index].epAddr = (pep->bEndpointAddress & 0x0F);
        epInfo[index].maxPktSize = (uint8_t)pep->wMaxPacketSize;
	epInfo[index].bmSndToggle   = 0;
	epInfo[index].bmRcvToggle   = 0;
        //epInfo[index].epAttribs = 0;

        bNumEP++;
        //PrintEndpointDescriptor(pep);
}

uint8_t CP210x::Release() {
        pUsb->GetAddressPool().FreeAddress(bAddress);

        bAddress = 0;
        bNumEP = 1;
        qNextPollTime = 0;
        bPollEnable = false;
        return pAsync->OnRelease(this);
}

uint8_t CP210x::Poll() {
        uint8_t rcode = 0;
        return rcode;
}

uint8_t CP210x::IFCEnable() {
    return pUsb->ctrlReq(bAddress, 0, REQTYPE_HOST_TO_INTERFACE, CP210X_IFC_ENABLE, (uint8_t) 0x01, (uint8_t) 0x00, (uint16_t) 0x0000, (uint16_t) 0x0000, (uint16_t) 0x0000, NULL, NULL);
}

uint8_t CP210x::SetBaudRate(uint32_t baud) {
      uint8_t data[4];
      
      data[0] = (uint8_t)(baud & 0xFF);
      data[1] = (uint8_t)((baud>>8) & 0xFF);
      data[2] = (uint8_t)((baud>>16) & 0xFF);
      data[3] = (uint8_t)((baud>>24) & 0xFF);
      return pUsb->ctrlReq(bAddress, 0, REQTYPE_HOST_TO_INTERFACE, CP210X_SET_BAUDRATE, (uint8_t)0x00, (uint8_t)0x00, (uint16_t)0x0000, (uint16_t)0x04, (uint16_t)0x04, data, NULL);
}

uint8_t CP210x::GetBaudRate(uint8_t data[]) {
        return pUsb->ctrlReq(bAddress, 0, REQTYPE_INTERFACE_TO_HOST, CP210X_GET_BAUDRATE, (uint8_t)0x00, (uint8_t)0x00, (uint16_t)0x0000, (uint16_t)0x04, (uint16_t)0x04, data, NULL);
}

uint8_t CP210x::GetLineCTL(uint8_t data[]) 
{
      return pUsb->ctrlReq(bAddress, 0, REQTYPE_INTERFACE_TO_HOST, CP210x_GET_LINE_CTL, 0, 0, 0, (uint16_t)0x0002, (uint16_t)0x0002, data, NULL);
}

uint8_t CP210x::SetDataBits(uint8_t dataBits) 
{
      uint8_t data[2]; 
      uint8_t rcode = GetLineCTL(data);
      data[1] = dataBits;
      return pUsb->ctrlReq(bAddress, 0, REQTYPE_HOST_TO_INTERFACE, CP210X_SET_LINE_CTL, data[0], data[1], 0, 0, 0, NULL, NULL);
}


uint8_t CP210x::SetStopBits(uint8_t stopBits)
{
    uint8_t data[2];
    uint8_t rcode = GetLineCTL(data);
    switch(stopBits)
    {
    case CP210X_STOP_BITS_1:
      data[0] &= ~1;              // clear bit 0
      data[0] &= ~(1 << 1);       // clear bit 1
      break;
    case CP210X_STOP_BITS_15:
      data[0] |= 1;               // set bit 0
      data[0] &= ~(1 << 1) ;      // clear bit 1
      break;
    case CP210X_STOP_BITS_2:
      data[0] &= ~1;              // clear bit 0
      data[0] |= (1 << 1);        // set bit 1
      break;
    }
    return pUsb->ctrlReq(bAddress, 0, REQTYPE_HOST_TO_INTERFACE, CP210X_SET_LINE_CTL, data[0], data[1], 0, 0, 0, NULL, NULL);
}

uint8_t CP210x::SetParity(uint8_t parity) 
{
    uint8_t data[2];
    uint8_t rcode = GetLineCTL(data);
    switch(parity)
    {
    case CP210X_PARITY_NONE:
      data[0] &= ~(1 << 4);
      data[0] &= ~(1 << 5);
      data[0] &= ~(1 << 6);
      data[0] &= ~(1 << 7);
      break;
    case CP210X_PARITY_ODD:
      data[0] |= (1 << 4);
      data[0] &= ~(1 << 5);
      data[0] &= ~(1 << 6);
      data[0] &= ~(1 << 7);
      break;
    case CP210X_PARITY_EVEN:
      data[0] &= ~(1 << 4);
      data[0] |= (1 << 5);
      data[0] &= ~(1 << 6);
      data[0] &= ~(1 << 7);
      break;
    case CP210X_PARITY_MARK:
      data[0] |= (1 << 4);
      data[0] |= (1 << 5);
      data[0] &= ~(1 << 6);
      data[0] &= ~(1 << 7);
      break;
    case CP210X_PARITY_SPACE:
      data[0] &= ~(1 << 4);
      data[0] &= ~(1 << 5);
      data[0] |= (1 << 6);
      data[0] &= ~(1 << 7);
      break;
    }
    return pUsb->ctrlReq(bAddress, 0, REQTYPE_HOST_TO_INTERFACE, CP210X_SET_LINE_CTL, data[0], data[1], 0, 0, 0, NULL, NULL);
}


uint8_t CP210x::SetFlowControl(uint8_t flowControl)
{
  switch(flowControl)
    {
    case CP210X_FLOW_CONTROL_OFF:
      {
        uint8_t dataOff[16] = {
          (uint8_t) 0x01, (uint8_t) 0x00, (uint8_t) 0x00, (uint8_t) 0x00, 
          (uint8_t) 0x40, (uint8_t) 0x00, (uint8_t) 0x00, (uint8_t) 0x00,
          (uint8_t) 0x00, (uint8_t) 0x80, (uint8_t) 0x00, (uint8_t) 0x00,
          (uint8_t) 0x00, (uint8_t) 0x20, (uint8_t) 0x00, (uint8_t) 0x00
        };
        return pUsb->ctrlReq(bAddress, 0, REQTYPE_HOST_TO_INTERFACE, CP210X_SET_FLOW, (uint8_t)0x00, (uint8_t)0x00, (uint16_t)0x0000, (uint16_t)0x0010, (uint16_t)0x0010, dataOff, NULL);
        break;
      }
    case CP210X_FLOW_CONTROL_RTS_CTS:
      {
        uint8_t dataRTSCTS[16] = {
          (uint8_t) 0x09, (uint8_t) 0x00, (uint8_t) 0x00, (uint8_t) 0x00,
          (uint8_t) 0x80, (uint8_t) 0x00, (uint8_t) 0x00, (uint8_t) 0x00,
          (uint8_t) 0x00, (uint8_t) 0x80, (uint8_t) 0x00, (uint8_t) 0x00,
          (uint8_t) 0x00, (uint8_t) 0x20, (uint8_t) 0x00, (uint8_t) 0x00
        };
        return pUsb->ctrlReq(bAddress, 0, REQTYPE_HOST_TO_INTERFACE, CP210X_SET_FLOW, (uint8_t)0x00, (uint8_t)0x00, (uint16_t)0x0000, (uint16_t)0x0010, (uint16_t)0x0010, dataRTSCTS, NULL);
        break;
      }
    case CP210X_FLOW_CONTROL_DSR_DTR:
      {
        uint8_t dataDSRDTR[16] = {
          (uint8_t) 0x12, (uint8_t) 0x00, (uint8_t) 0x00, (uint8_t) 0x00,
          (uint8_t) 0x40, (uint8_t) 0x00, (uint8_t) 0x00, (uint8_t) 0x00,
          (uint8_t) 0x00, (uint8_t) 0x80, (uint8_t) 0x00, (uint8_t) 0x00,
          (uint8_t) 0x00, (uint8_t) 0x20, (uint8_t) 0x00, (uint8_t) 0x00
        };
        return pUsb->ctrlReq(bAddress, 0, REQTYPE_HOST_TO_INTERFACE, CP210X_SET_FLOW, (uint8_t)0x00, (uint8_t)0x00, (uint16_t)0x0000, (uint16_t)0x0010, (uint16_t)0x0010, dataDSRDTR, NULL);
        break;
      }
    case CP210X_FLOW_CONTROL_XON_XOFF:
      {
        uint8_t dataXONXOFF[16] = {
          (uint8_t) 0x01, (uint8_t) 0x00, (uint8_t) 0x00, (uint8_t) 0x00,
          (uint8_t) 0x43, (uint8_t) 0x00, (uint8_t) 0x00, (uint8_t) 0x00,
          (uint8_t) 0x00, (uint8_t) 0x80, (uint8_t) 0x00, (uint8_t) 0x00,
          (uint8_t) 0x00, (uint8_t) 0x20, (uint8_t) 0x00, (uint8_t) 0x00
        };
      
        uint8_t dataChars[6] = {
          (uint8_t) 0x00, (uint8_t) 0x00, (uint8_t) 0x00,
          (uint8_t) 0x00, (uint8_t) 0x11, (uint8_t) 0x13
        };
      
        uint8_t rcode = pUsb->ctrlReq(bAddress, 0, REQTYPE_HOST_TO_INTERFACE, CP210X_SET_CHARS, (uint8_t)0x00, (uint8_t)0x00, (uint16_t)0x0000, (uint16_t)0x0006, (uint16_t)0x0006, dataChars, NULL);
        return pUsb->ctrlReq(bAddress, 0, REQTYPE_HOST_TO_INTERFACE, CP210X_SET_FLOW, (uint8_t)0x00, (uint8_t)0x00, (uint16_t)0x0000, (uint16_t)0x10, (uint16_t)0x10, dataXONXOFF, NULL);
        break;
      }
    }
  return (uint8_t) 255;
}


uint8_t CP210x::RcvData(uint16_t *bytes_rcvd, uint8_t *dataptr) {
        return pUsb->inTransfer(bAddress, epInfo[epDataInIndex].epAddr, bytes_rcvd, dataptr);
}

uint8_t CP210x::SndData(uint16_t nbytes, uint8_t *dataptr) {
        return pUsb->outTransfer(bAddress, epInfo[epDataOutIndex].epAddr, nbytes, dataptr);
}

void CP210x::PrintEndpointDescriptor(const USB_ENDPOINT_DESCRIPTOR* ep_ptr) {
        Notify(PSTR("Endpoint descriptor:"), 0x80);
        Notify(PSTR("\r\nLength:\t\t"), 0x80);
        D_PrintHex<uint8_t > (ep_ptr->bLength, 0x80);
        Notify(PSTR("\r\nType:\t\t"), 0x80);
        D_PrintHex<uint8_t > (ep_ptr->bDescriptorType, 0x80);
        Notify(PSTR("\r\nAddress:\t"), 0x80);
        D_PrintHex<uint8_t > (ep_ptr->bEndpointAddress, 0x80);
        Notify(PSTR("\r\nAttributes:\t"), 0x80);
        D_PrintHex<uint8_t > (ep_ptr->bmAttributes, 0x80);
        Notify(PSTR("\r\nMaxPktSize:\t"), 0x80);
        D_PrintHex<uint16_t > (ep_ptr->wMaxPacketSize, 0x80);
        Notify(PSTR("\r\nPoll Intrv:\t"), 0x80);
        D_PrintHex<uint8_t > (ep_ptr->bInterval, 0x80);
        Notify(PSTR("\r\n"), 0x80);
}
