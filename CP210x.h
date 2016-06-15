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
#if !defined(__CP210x_H__)
#define __CP210x_H__

#include "Usb.h"

typedef struct usb_device_id {
  unsigned int vid;
  unsigned int pid;
} usb_device_id;


static const struct usb_device_id id_table[] = {
         { 0x045B, 0x0053 }, /* Renesas RX610 RX-Stick */
         { 0x0471, 0x066A }, /* AKTAKOM ACE-1001 cable */
         { 0x0489, 0xE000 }, /* Pirelli Broadband S.p.A, DP-L10 SIP/GSM Mobile */
         { 0x0489, 0xE003 }, /* Pirelli Broadband S.p.A, DP-L10 SIP/GSM Mobile */
         { 0x0745, 0x1000 }, /* CipherLab USB CCD Barcode Scanner 1000 */
         { 0x0846, 0x1100 }, /* NetGear Managed Switch M4100 series, M5300 series, M7100 series */
         { 0x08e6, 0x5501 }, /* Gemalto Prox-PU/CU contactless smartcard reader */
         { 0x08FD, 0x000A }, /* Digianswer A/S , ZigBee/802.15.4 MAC Device */
         { 0x0908, 0x01FF }, /* Siemens RUGGEDCOM USB Serial Console */
         { 0x0BED, 0x1100 }, /* MEI (TM Cashflow-SC Bill/Voucher Acceptor */
         { 0x0BED, 0x1101 }, /* MEI series 2000 Combo Acceptor */
         { 0x0FCF, 0x1003 }, /* Dynastream ANT development board */
         { 0x0FCF, 0x1004 }, /* Dynastream ANT2USB */
         { 0x0FCF, 0x1006 }, /* Dynastream ANT development board */
         { 0x0FDE, 0xCA05 }, /* OWL Wireless Electricity Monitor CM-160 */
         { 0x10A6, 0xAA26 }, /* Knock-off DCU-11 cable */
         { 0x10AB, 0x10C5 }, /* Siemens MC60 Cable */
         { 0x10B5, 0xAC70 }, /* Nokia CA-42 USB */
         { 0x10C4, 0x0F91 }, /* Vstabi */
         { 0x10C4, 0x1101 }, /* Arkham Technology DS101 Bus Monitor */
         { 0x10C4, 0x1601 }, /* Arkham Technology DS101 Adapter */
         { 0x10C4, 0x800A }, /* SPORTident BSM7-D-USB main station */
         { 0x10C4, 0x803B }, /* Pololu USB-serial converter */
         { 0x10C4, 0x8044 }, /* Cygnal Debug Adapter */
         { 0x10C4, 0x804E }, /* Software Bisque Paramount ME build-in converter */
         { 0x10C4, 0x8053 }, /* Enfora EDG1228 */
         { 0x10C4, 0x8054 }, /* Enfora GSM2228 */
         { 0x10C4, 0x8066 }, /* Argussoft In-System Programmer */
         { 0x10C4, 0x806F }, /* IMS USB to RS422 Converter Cable */
         { 0x10C4, 0x807A }, /* Crumb128 board */
         { 0x10C4, 0x80C4 }, /* Cygnal Integrated Products, Inc., Optris infrared thermometer */
         { 0x10C4, 0x80CA }, /* Degree Controls Inc */
         { 0x10C4, 0x80DD }, /* Tracient RFID */
         { 0x10C4, 0x80F6 }, /* Suunto sports instrument */
         { 0x10C4, 0x8115 }, /* Arygon NFC/Mifare Reader */
         { 0x10C4, 0x813D }, /* Burnside Telecom Deskmobile */
         { 0x10C4, 0x813F }, /* Tams Master Easy Control */
         { 0x10C4, 0x814A }, /* West Mountain Radio RIGblaster P&P */
         { 0x10C4, 0x814B }, /* West Mountain Radio RIGtalk */
         { 0x2405, 0x0003 }, /* West Mountain Radio RIGblaster Advantage */
         { 0x10C4, 0x8156 }, /* B&G H3000 link cable */
         { 0x10C4, 0x815E }, /* Helicomm IP-Link 1220-DVM */
         { 0x10C4, 0x815F }, /* Timewave HamLinkUSB */
         { 0x10C4, 0x818B }, /* AVIT Research USB to TTL */
         { 0x10C4, 0x819F }, /* MJS USB Toslink Switcher */
         { 0x10C4, 0x81A6 }, /* ThinkOptics WavIt */
         { 0x10C4, 0x81A9 }, /* Multiplex RC Interface */
         { 0x10C4, 0x81AC }, /* MSD Dash Hawk */
         { 0x10C4, 0x81AD }, /* INSYS USB Modem */
         { 0x10C4, 0x81C8 }, /* Lipowsky Industrie Elektronik GmbH, Baby-JTAG */
         { 0x10C4, 0x81E2 }, /* Lipowsky Industrie Elektronik GmbH, Baby-LIN */
         { 0x10C4, 0x81E7 }, /* Aerocomm Radio */
         { 0x10C4, 0x81E8 }, /* Zephyr Bioharness */
         { 0x10C4, 0x81F2 }, /* C1007 HF band RFID controller */
         { 0x10C4, 0x8218 }, /* Lipowsky Industrie Elektronik GmbH, HARP-1 */
         { 0x10C4, 0x822B }, /* Modem EDGE(GSM Comander 2 */
         { 0x10C4, 0x826B }, /* Cygnal Integrated Products, Inc., Fasttrax GPS demonstration module */
         { 0x10C4, 0x8281 }, /* Nanotec Plug & Drive */
         { 0x10C4, 0x8293 }, /* Telegesis ETRX2USB */
         { 0x10C4, 0x82F9 }, /* Procyon AVS */
         { 0x10C4, 0x8341 }, /* Siemens MC35PU GPRS Modem */
         { 0x10C4, 0x8382 }, /* Cygnal Integrated Products, Inc. */
         { 0x10C4, 0x83A8 }, /* Amber Wireless AMB2560 */
         { 0x10C4, 0x83D8 }, /* DekTec DTA Plus VHF/UHF Booster/Attenuator */
         { 0x10C4, 0x8411 }, /* Kyocera GPS Module */
         { 0x10C4, 0x8418 }, /* IRZ Automation Teleport SG-10 GSM/GPRS Modem */
         { 0x10C4, 0x846E }, /* BEI USB Sensor Interface (VCP */
         { 0x10C4, 0x8477 }, /* Balluff RFID */
         { 0x10C4, 0x85EA }, /* AC-Services IBUS-IF */
         { 0x10C4, 0x85EB }, /* AC-Services CIS-IBUS */
         { 0x10C4, 0x85F8 }, /* Virtenio Preon32 */
         { 0x10C4, 0x8664 }, /* AC-Services CAN-IF */
         { 0x10C4, 0x8665 }, /* AC-Services OBD-IF */
         { 0x10C4, 0x8856 }, /* CEL EM357 ZigBee USB Stick - LR */
         { 0x10C4, 0x8857 }, /* CEL EM357 ZigBee USB Stick */
         { 0x10C4, 0x88A4 }, /* MMB Networks ZigBee USB Device */
         { 0x10C4, 0x88A5 }, /* Planet Innovation Ingeni ZigBee USB Device */
         { 0x10C4, 0x8946 }, /* Ketra N1 Wireless Interface */
         { 0x10C4, 0x8977 }, /* CEL MeshWorks DevKit Device */
         { 0x10C4, 0x8998 }, /* KCF Technologies PRN */
         { 0x10C4, 0x8A2A }, /* HubZ dual ZigBee and Z-Wave dongle */
         { 0x10C4, 0xEA60 }, /* Silicon Labs factory default */
         { 0x10C4, 0xEA61 }, /* Silicon Labs factory default */
         { 0x10C4, 0xEA70 }, /* Silicon Labs factory default */
         { 0x10C4, 0xEA80 }, /* Silicon Labs factory default */
         { 0x10C4, 0xEA71 }, /* Infinity GPS-MIC-1 Radio Monophone */
         { 0x10C4, 0xF001 }, /* Elan Digital Systems USBscope50 */
         { 0x10C4, 0xF002 }, /* Elan Digital Systems USBwave12 */
         { 0x10C4, 0xF003 }, /* Elan Digital Systems USBpulse100 */
         { 0x10C4, 0xF004 }, /* Elan Digital Systems USBcount50 */
         { 0x10C5, 0xEA61 }, /* Silicon Labs MobiData GPRS USB Modem */
         { 0x10CE, 0xEA6A }, /* Silicon Labs MobiData GPRS USB Modem 100EU */
         { 0x13AD, 0x9999 }, /* Baltech card reader */
         { 0x1555, 0x0004 }, /* Owen AC4 USB-RS485 Converter */
         { 0x166A, 0x0201 }, /* Clipsal 5500PACA C-Bus Pascal Automation Controller */
         { 0x166A, 0x0301 }, /* Clipsal 5800PC C-Bus Wireless PC Interface */
         { 0x166A, 0x0303 }, /* Clipsal 5500PCU C-Bus USB interface */
         { 0x166A, 0x0304 }, /* Clipsal 5000CT2 C-Bus Black and White Touchscreen */
         { 0x166A, 0x0305 }, /* Clipsal C-5000CT2 C-Bus Spectrum Colour Touchscreen */
         { 0x166A, 0x0401 }, /* Clipsal L51xx C-Bus Architectural Dimmer */
         { 0x166A, 0x0101 }, /* Clipsal 5560884 C-Bus Multi-room Audio Matrix Switcher */
         { 0x16C0, 0x09B0 }, /* Lunatico Seletek */
         { 0x16C0, 0x09B1 }, /* Lunatico Seletek */
         { 0x16D6, 0x0001 }, /* Jablotron serial interface */
         { 0x16DC, 0x0010 }, /* W-IE-NE-R Plein & Baus GmbH PL512 Power Supply */
         { 0x16DC, 0x0011 }, /* W-IE-NE-R Plein & Baus GmbH RCM Remote Control for MARATON Power Supply */
         { 0x16DC, 0x0012 }, /* W-IE-NE-R Plein & Baus GmbH MPOD Multi Channel Power Supply */
         { 0x16DC, 0x0015 }, /* W-IE-NE-R Plein & Baus GmbH CML Control, Monitoring and Data Logger */
         { 0x17A8, 0x0001 }, /* Kamstrup Optical Eye/3-wire */
         { 0x17A8, 0x0005 }, /* Kamstrup M-Bus Master MultiPort 250D */
         { 0x17F4, 0xAAAA }, /* Wavesense Jazz blood glucose meter */
         { 0x1843, 0x0200 }, /* Vaisala USB Instrument Cable */
         { 0x18EF, 0xE00F }, /* ELV USB-I2C-Interface */
         { 0x1ADB, 0x0001 }, /* Schweitzer Engineering C662 Cable */
         { 0x1B1C, 0x1C00 }, /* Corsair USB Dongle */
         { 0x1BA4, 0x0002 }, /* Silicon Labs 358x factory default */
         { 0x1BE3, 0x07A6 }, /* WAGO 750-923 USB Service Cable */
         { 0x1D6F, 0x0010 }, /* Seluxit ApS RF Dongle */
         { 0x1E29, 0x0102 }, /* Festo CPX-USB */
         { 0x1E29, 0x0501 }, /* Festo CMSP */
         { 0x1FB9, 0x0100 }, /* Lake Shore Model 121 Current Source */
         { 0x1FB9, 0x0200 }, /* Lake Shore Model 218A Temperature Monitor */
         { 0x1FB9, 0x0201 }, /* Lake Shore Model 219 Temperature Monitor */
         { 0x1FB9, 0x0202 }, /* Lake Shore Model 233 Temperature Transmitter */
         { 0x1FB9, 0x0203 }, /* Lake Shore Model 235 Temperature Transmitter */
         { 0x1FB9, 0x0300 }, /* Lake Shore Model 335 Temperature Controller */
         { 0x1FB9, 0x0301 }, /* Lake Shore Model 336 Temperature Controller */
         { 0x1FB9, 0x0302 }, /* Lake Shore Model 350 Temperature Controller */
         { 0x1FB9, 0x0303 }, /* Lake Shore Model 371 AC Bridge */
         { 0x1FB9, 0x0400 }, /* Lake Shore Model 411 Handheld Gaussmeter */
         { 0x1FB9, 0x0401 }, /* Lake Shore Model 425 Gaussmeter */
         { 0x1FB9, 0x0402 }, /* Lake Shore Model 455A Gaussmeter */
         { 0x1FB9, 0x0403 }, /* Lake Shore Model 475A Gaussmeter */
         { 0x1FB9, 0x0404 }, /* Lake Shore Model 465 Three Axis Gaussmeter */
         { 0x1FB9, 0x0600 }, /* Lake Shore Model 625A Superconducting MPS */
         { 0x1FB9, 0x0601 }, /* Lake Shore Model 642A Magnet Power Supply */
         { 0x1FB9, 0x0602 }, /* Lake Shore Model 648 Magnet Power Supply */
         { 0x1FB9, 0x0700 }, /* Lake Shore Model 737 VSM Controller */
         { 0x1FB9, 0x0701 }, /* Lake Shore Model 776 Hall Matrix */
         { 0x3195, 0xF190 }, /* Link Instruments MSO-19 */
         { 0x3195, 0xF280 }, /* Link Instruments MSO-28 */
         { 0x3195, 0xF281 }, /* Link Instruments MSO-28 */
         { 0x413C, 0x9500 }, /* DW700 GPS USB interface */
         { } /* Terminating Entry */
 };

#define CP210x_MAX_ENDPOINTS              3

#define REQTYPE_HOST_TO_INTERFACE   (uint8_t) 0x41
#define REQTYPE_INTERFACE_TO_HOST   (uint8_t) 0xc1

// Commands
#define CP210X_SET_LINE_CTL         (uint8_t) 0x03
#define CP210x_GET_LINE_CTL         (uint8_t) 0x04
#define CP210X_SET_FLOW             (uint8_t) 0x13
#define CP210X_IFC_ENABLE           (uint8_t) 0x00
#define CP210X_SET_BAUDRATE         (uint8_t) 0x1E
#define CP210X_GET_BAUDRATE         (uint8_t) 0x1D
#define CP210X_SET_CHARS            (uint8_t) 0x19

#define CP210X_PARITY_NONE          (uint8_t) 0x00
#define CP210X_PARITY_ODD           (uint8_t) 0x01
#define CP210X_PARITY_EVEN          (uint8_t) 0x02
#define CP210X_PARITY_MARK          (uint8_t) 0x03
#define CP210X_PARITY_SPACE         (uint8_t) 0x04

#define CP210X_STOP_BITS_1          (uint8_t) 0x01
#define CP210X_STOP_BITS_15         (uint8_t) 0x03
#define CP210X_STOP_BITS_2          (uint8_t) 0x02

#define CP210X_FLOW_CONTROL_OFF       0x00
#define CP210X_FLOW_CONTROL_RTS_CTS   0x01
#define CP210X_FLOW_CONTROL_DSR_DTR   0x02
#define CP210X_FLOW_CONTROL_XON_XOFF  0x03


class CP210x;

class CP210xAsyncOper {
public:

        virtual uint8_t OnInit(CP210x *pftdi) {
                return 0;
        };

        virtual uint8_t OnRelease(CP210x *pftdi) {
                return 0;
        };
};


class CP210x : public USBDeviceConfig, public UsbConfigXtracter {
        static const uint8_t epDataInIndex; // DataIn endpoint index
        static const uint8_t epDataOutIndex; // DataOUT endpoint index
        static const uint8_t epInterruptInIndex; // InterruptIN  endpoint index

        CP210xAsyncOper *pAsync;
        USB *pUsb;
        uint8_t bAddress;
        uint8_t bConfNum; // configuration number
        uint8_t bNumIface; // number of interfaces in the configuration
        uint8_t bNumEP; // total number of EP in the configuration
        uint32_t qNextPollTime; // next poll time
        bool bPollEnable; // poll enable flag

        EpInfo epInfo[CP210x_MAX_ENDPOINTS];

        void PrintEndpointDescriptor(const USB_ENDPOINT_DESCRIPTOR* ep_ptr);

public:
        CP210x(USB *pusb, CP210xAsyncOper *pasync);

        uint8_t GetLineCTL(uint8_t data[]);

        uint8_t IFCEnable();
        uint8_t SetBaudRate(uint32_t baud);
        uint8_t GetBaudRate(uint8_t data[]);
        uint8_t SetDataBits(uint8_t dataBits);
        uint8_t SetStopBits(uint8_t stopBits);
        uint8_t SetParity(uint8_t parity);
        uint8_t SetModemControl(uint16_t control);
        uint8_t SetFlowControl(uint8_t flowControl);
     
        // Methods for recieving and sending data
        uint8_t RcvData(uint16_t *bytes_rcvd, uint8_t *dataptr);
        uint8_t SndData(uint16_t nbytes, uint8_t *dataptr);

        // USBDeviceConfig implementation
        uint8_t Init(uint8_t parent, uint8_t port, bool lowspeed);
        uint8_t Release();
        uint8_t Poll();

        virtual uint8_t GetAddress() {
                return bAddress;
        };

        // UsbConfigXtracter implementation
        void EndpointXtract(uint8_t conf, uint8_t iface, uint8_t alt, uint8_t proto, const USB_ENDPOINT_DESCRIPTOR *ep);

        virtual bool VIDPIDOK(uint16_t vid, uint16_t pid) {
               // int sizeOfArray = sizeof(id_table)/sizeof(id_table[0]);
               // for(int i = 0; i < sizeOfArray; i++) {
               //   if (vid == id_table[i].vid && pid == id_table[i].pid) {
                    return true;
               //   }
               // }
               // return false;
        }

};

#endif // __CP210x_H__
