/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (C) 2018-2020 Oplus. All rights reserved.
 */
#ifndef _OPPO_PROJECT_H_
#define _OPPO_PROJECT_H_

#define MAX_OCP	6
#define MAX_LEN 8
#define FEATURE_COUNT 10
#define OCDT_VERSION_1_0	(1)
#define OCDT_VERSION_2_0	(2)

#define ALIGN4(s) ((sizeof(s) + 3)&(~0x3))

#define FEATURE1_OPEARTOR_OPEN_MASK 0000
#define FEATURE1_FOREIGN_MASK 0001
#define FEATURE1_OPEARTOR_CMCC_MASK 0010
#define FEATURE1_OPEARTOR_CT_MASK 0011
#define FEATURE1_OPEARTOR_CU_MASK 0100
#define FEATURE1_OPEARTOR_MAX_MASK 1111

#ifdef CONFIG_OPLUS_SYSTEM_KERNEL_QCOM
enum PCB_VERSION {
    UNKNOWN_VERSION = 0,
    PRE_EVB1,
    PRE_EVB2,
    EVB1 = 8,
    EVB2,
    EVB3,
    EVB4,
    EVB5,
    EVB6,
    T0 = 16,
    T1,
    T2,
    T3,
    T4,
    T5,
    T6,
    EVT1 = 24,
    EVT2,
    EVT3,
    EVT4,
    EVT5,
    EVT6,
    DVT1 = 32,
    DVT2,
    DVT3,
    DVT4,
    DVT5,
    DVT6,
    PVT1 = 40,
    PVT2,
    PVT3,
    PVT4,
    PVT5,
    PVT6,
    MP1 = 48,
    MP2,
    MP3,
    MP4,
    MP5,
    MP6,
    PCB_MAX,
};

//Reboot modes
enum {
    UNKOWN_MODE         = 0x00,
    RECOVERY_MODE       = 0x1,
    FASTBOOT_MODE       = 0x2,
    ALARM_BOOT          = 0x3,
    DM_VERITY_LOGGING   = 0x4,
    DM_VERITY_ENFORCING = 0x5,
    DM_VERITY_KEYSCLEAR = 0x6,
    SILENCE_MODE        = 0x21,
    SAU_MODE            = 0x22,
    RF_MODE             = 0x23,
    WLAN_MODE           = 0x24,
    MOS_MODE            = 0x25,
    FACTORY_MODE        = 0x26,
    REBOOT_KERNEL       = 0x27,
    REBOOT_MODEM        = 0x28,
    REBOOT_ANDROID      = 0x29,
//Liang.Zhang@TECH.BSP.Storage.OPPO_DOCTOR, 2019/9/13, Modify for PHOENIX_PROJECT and OPPO_DOCTOR use
//#if defined(PHOENIX_PROJECT) || defined(OPPO_DOCTOR)
    REBOOT_SBL_DDR_CUS  = 0x2C,
    REBOOT_AGINGTEST    = 0x2D,
    REBOOT_SBLTEST_FAIL = 0x2E,
//#endif
    NORMAL_MODE         = 0x3E,
    REBOOT_NORMAL       = 0x3F,
    EMERGENCY_DLOAD     = 0xFF,
};
#else
enum PCB_VERSION {
	PCB_UNKNOWN,
	PCB_VERSION_EVB1,
	PCB_VERSION_T0,
	PCB_VERSION_T1,
	PCB_VERSION_EVT1,
	PCB_VERSION_EVT2,
	PCB_VERSION_EVT3,
	PCB_VERSION_DVT1,
	PCB_VERSION_DVT2,
	PCB_VERSION_DVT3,
	PCB_VERSION_PVT1,
	PCB_VERSION_MP1,
	PCB_VERSION_MP2,
	PCB_VERSION_MP3,
	PCB_VERSION_MAX,
};
#endif

#define OPPO_ENG_VERSION_NOT_INIT      -1
enum OPPO_ENG_VERSION {
    RELEASE                = 0x00,
    AGING                   = 0x01,
    CTA                     = 0x02,
    PERFORMANCE          = 0x03,
    PREVERSION              = 0x04,
    ALL_NET_CMCC_TEST      = 0x05,
    ALL_NET_CMCC_FIELD      = 0x06,
    ALL_NET_CU_TEST         = 0x07,
    ALL_NET_CU_FIELD        = 0x08,
    ALL_NET_CT_TEST         = 0x09,
    ALL_NET_CT_FIELD        = 0x0A,
    HIGH_TEMP_AGING       = 0x0B,
	FACTORY				   = 0x0C,
};

typedef  struct
{
	unsigned int ProjectNo;
	unsigned int DtsiNo;
	unsigned int AudioIdx;
	unsigned int Feature[FEATURE_COUNT];
} ProjectDataBCDT;

typedef  struct
{
	unsigned int	 Version;
	unsigned int	 Is_confidential;
} ProjectDataECDT;

typedef  struct
{
	unsigned int	  OppoBootMode;
	unsigned int	  RF;
	unsigned int	  PCB;
	unsigned char	  PmicOcp[MAX_OCP];
	unsigned char	  Operator;
	unsigned char	  Reserved[15];
} ProjectDataSCDT;

typedef  struct
{
  unsigned int	 Version;
  ProjectDataBCDT nDataBCDT;
  ProjectDataSCDT nDataSCDT;
  ProjectDataECDT nDataECDT;
} ProjectInfoOCDT;

enum{
    HW_VERSION__UNKNOWN,
    HW_VERSION__10,		//EVB
    HW_VERSION__11,		//T0
    HW_VERSION__12,		//EVT-1
    HW_VERSION__13,		//EVT-2
    HW_VERSION__14,		//DVT-1
    HW_VERSION__15,		//DVT-2
    HW_VERSION__16,		//PVT
    HW_VERSION__17,		//MP
    HW_VERSION_MAX,
};

enum OPPO_MODEM {
    RF_VERSION__UNKNOWN = 0x00,
    RF_VERSION__11      = 0x01,
    RF_VERSION__12      = 0x02,
    RF_VERSION__13      = 0x03,
    RF_VERSION__14      = 0x04,
    RF_VERSION__15      = 0x05,
    RF_VERSION__16      = 0x06,
    RF_VERSION__17      = 0x07,
    RF_VERSION__18      = 0x08,
    RF_VERSION__19      = 0x08,
    RF_VERSION__20      = 0x09,
    RF_VERSION__21      = 0x10,
    RF_VERSION__22      = 0x11,
    RF_VERSION__23      = 0x12,
    RF_VERSION__31      = 0x13,
    RF_VERSION__32      = 0x14,
    RF_VERSION__33      = 0x15,
    RF_VERSION__1A      = 0x0A,
};

#define GET_PCB_VERSION_OLDCDT() (get_PCB_Version_oldcdt())
#define GET_PCB_VERSION_STRING_OLDCDT() (get_PCB_Version_String_oldcdt())

#define GET_MODEM_VERSION_OLDCDT() (get_Modem_Version_oldcdt())
#define GET_OPERATOR_VERSION_OLDCDT() (get_Operator_Version_oldcdt())

enum OPPO_PROJECT {
    OPPO_UNKOWN = 0,
    OPPO_18041 = 18041,
    OPPO_18081 = 18081,
    OPPO_18085 = 18085,
    OPPO_18181 = 18181,
    OPPO_18097 = 18097,
    OPPO_18621 = 18621,
    OPPO_18099 = 18099,
    OPPO_18383 = 18383,
    OPPO_19651 = 19651,
    OPPO_19691 = 19691,
};

enum OPPO_OPERATOR {
    OPERATOR_UNKOWN                       = 0,
    OPERATOR_OPEN_MARKET                 = 1,
    OPERATOR_CHINA_MOBILE                 = 2,
    OPERATOR_CHINA_UNICOM                = 3,
    OPERATOR_CHINA_TELECOM               = 4,
    OPERATOR_FOREIGN                       = 5,
    OPERATOR_FOREIGN_WCDMA              = 6,   /*qifeng.liu 2014.08.07 FOR MAC*/
    OPERATOR_FOREIGN_RESERVED           = 7,    /*shaoming 2014/10/04 add for 14085's dual sim version*/
    OPERATOR_ALL_CHINA_CARRIER           = 8,    /*instead of TELECOM CARRIER because of history Tong.han@Bsp.Group.Tp add for all china carrier phone, 2015/03/23*/
    OPERATOR_ALL_CHINA_CARRIER_MOBILE  = 9,    /*rendong.shi@Bsp.Group.Tp add for all china carrier MOBILE phone, 2016/01/07*/
    OPERATOR_RM_FOREIGN                   = 30,  //linzhenming add for realme foreign  18623
    OPERATOR_RM_FOREIGN_INDIA             = 31,  //linzhenming add for realme foreign  india 18621
    OPERATOR_RM_FOREIGN_VIETNAM_128GB  = 32,  //linzhenming add for realme foreign  vietnam 18625 
    OPERATOR_RM_FOREIGN_VIETNAM_64GB   = 33,  //linzhenming add for realme foreign  vietnam 18627
    OPERATOR_RM_FOREIGN_EUROPE           = 34,  //linzhenming add for realme foreign  EUROPE 18633
};

enum{
    SMALLBOARD_VERSION__0,
    SMALLBOARD_VERSION__1,
    SMALLBOARD_VERSION__2,
    SMALLBOARD_VERSION__3,
    SMALLBOARD_VERSION__4,
    SMALLBOARD_VERSION__5,
    SMALLBOARD_VERSION__6,
    SMALLBOARD_VERSION__UNKNOWN = 100,
};

enum F_INDEX {
	IDX_1 = 1,
	IDX_2,
	IDX_3,
	IDX_4,
	IDX_5,
	IDX_6,
	IDX_7,
	IDX_8,
	IDX_9,
	IDX_10,
};

typedef enum OPPO_PROJECT OPPO_PROJECT;
#define OCPCOUNTMAX 4
typedef struct
{
    unsigned int    nProject;
    unsigned int    nModem;
    unsigned int    nOperator;
    unsigned int    nPCBVersion;
    unsigned int    nENGVersion;
    unsigned int    isConfidential;
} ProjectInfoCDTType_oldcdt;

struct pcb_match {
	enum PCB_VERSION version;
	char *str;
};
unsigned int get_cdt_version(void);
unsigned int get_eng_version(void);
unsigned int is_new_cdt(void);
#ifdef CONFIG_OPLUS_SYSTEM_KERNEL_QCOM
unsigned int get_serialID(void);
#else
void get_serialID(char *serialno);
#endif

//cdt interface for Q or R
unsigned int get_project(void);
unsigned int get_project_oldcdt(void);
unsigned int is_project(OPPO_PROJECT project);
unsigned int is_project_oldcdt(OPPO_PROJECT project);
unsigned int get_PCB_Version_oldcdt(void);
unsigned int get_Modem_Version_oldcdt(void);
unsigned int get_Operator_Version_oldcdt(void);
unsigned int get_eng_version_oldcdt(void);
unsigned int get_Oppo_Boot_Mode(void);
unsigned int get_PCB_Version(void);
unsigned int get_audio(void);
unsigned int get_dtsiNo(void);
uint32_t get_oppo_feature(enum F_INDEX index);

//cdt interface for P->R
int32_t get_Modem_Version(void);
int32_t get_Operator_Version(void);

//eng cdt data for P or Q or R
bool is_confidential(void);
int is_confidential_oldcdt(void);
bool oppo_daily_build(void);
bool oppo_daily_build_oldcdt(void);
int oppo_project_init_oldcdt(void);

#endif
