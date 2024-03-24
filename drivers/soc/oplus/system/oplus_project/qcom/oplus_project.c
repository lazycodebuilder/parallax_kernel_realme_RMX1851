// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (C) 2018-2020 Oplus. All rights reserved.
 */
#include <linux/uaccess.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/fs.h>
#include <linux/seq_file.h>
#include <soc/qcom/smem.h>
#include <soc/oplus/system/oplus_project.h>
#include <linux/io.h>
#include <linux/of.h>
#include <linux/syscalls.h>
#include <linux/string.h>

#define UINT2Ptr(n)        (uint32_t *)(n)
#define Ptr2UINT32(p)    (uintptr_t)(p)

struct proc_dir_entry *oppo_info = NULL;
struct proc_dir_entry *oppo_info_temp = NULL;
static ProjectInfoOCDT *format = NULL;

static struct pcb_match pcb_str[] = {
    {.version=PRE_EVB1, .str="PRE_EVB1"},
    {.version=PRE_EVB2, .str="PRE_EVB2"},
    {.version=EVB1, .str="EVB1"},
    {.version=EVB2, .str="EVB2"},
    {.version=EVB3, .str="EVB3"},
    {.version=EVB4, .str="EVB4"},
    {.version=EVB5, .str="EVB5"},
    {.version=EVB6, .str="EVB6"},
    {.version=T0, .str="T0"},
    {.version=T1, .str="T1"},
    {.version=T2, .str="T2"},
    {.version=T3, .str="T3"},
    {.version=T4, .str="T4"},
    {.version=T5, .str="T5"},
    {.version=T6, .str="T6"},
    {.version=EVT1, .str="EVT1"},
    {.version=EVT2, .str="EVT2"},
    {.version=EVT3, .str="EVT3"},
    {.version=EVT4, .str="EVT4"},
    {.version=EVT5, .str="EVT5"},
    {.version=EVT6, .str="EVT6"},
    {.version=DVT1, .str="DVT1"},
    {.version=DVT2, .str="DVT2"},
    {.version=DVT3, .str="DVT3"},
    {.version=DVT4, .str="DVT4"},
    {.version=DVT5, .str="DVT5"},
    {.version=DVT6, .str="DVT6"},
    {.version=PVT1, .str="PVT1"},
    {.version=PVT2, .str="PVT2"},
    {.version=PVT3, .str="PVT3"},
    {.version=PVT4, .str="PVT4"},
    {.version=PVT5, .str="PVT5"},
    {.version=PVT6, .str="PVT6"},
    {.version=MP1, .str="MP1"},
    {.version=MP2, .str="MP2"},
    {.version=MP3, .str="MP3"},
    {.version=MP4, .str="MP4"},
    {.version=MP5, .str="MP5"},
    {.version=MP6, .str="MP6"},
};

static void init_project_version(void)
{
    char *PCB_version_name = NULL;
    uint16_t index = 0;
    
    unsigned int len = (sizeof(ProjectInfoOCDT) + 3)&(~0x3);

        /*get project info from smem*/
        format = (ProjectInfoOCDT *)smem_alloc(SMEM_PROJECT, len, 0, 0);
        if (format == ERR_PTR(-EPROBE_DEFER)) {
                format = NULL;
        }

        do {
            if(pcb_str[index].version == format->nDataSCDT.PCB){
                PCB_version_name = pcb_str[index].str;
                break;
            }
            index++;
        }while(index < sizeof(pcb_str)/sizeof(struct pcb_match));

        pr_err("KE Project:%d, Audio:%d, nRF:%d, PCB:%s\n",
            format->nDataBCDT.ProjectNo,
            format->nDataBCDT.AudioIdx,
            format->nDataSCDT.RF,PCB_version_name);
        pr_err("OCP: %d 0x%x %c %d 0x%x %c\n",
            format->nDataSCDT.PmicOcp[0],
            format->nDataSCDT.PmicOcp[1],
            format->nDataSCDT.PmicOcp[2],
            format->nDataSCDT.PmicOcp[3],
            format->nDataSCDT.PmicOcp[4],
            format->nDataSCDT.PmicOcp[5]);

    if(is_new_cdt()){
		if(oppo_info){
			remove_proc_entry("oppoVersion/operatorName", NULL);
			pr_err("remove proc operatorName\n");
			remove_proc_entry("oppoVersion/modemType", NULL);
			pr_err("remove proc modemType\n");
		}
        if(oppo_info_temp){
			remove_proc_entry("oplusVersion/operatorName", NULL);
			pr_err("remove proc operatorName\n");
			remove_proc_entry("oplusVersion/modemType", NULL);
			pr_err("remove proc modemType\n");
		}
	} else {
		if(oppo_info){
			remove_proc_entry("oppoVersion/RFType", NULL);
			pr_err("remove proc RFType\n");
		}
        if(oppo_info_temp){
			remove_proc_entry("oplusVersion/RFType", NULL);
			pr_err("remove proc RFType\n");
		}
	}

    pr_err("get_project:%d, is_new_cdt:%d, get_PCB_Version:%d, get_Oppo_Boot_Mode:%d, get_Modem_Version:%d\n", 
            get_project(),
            is_new_cdt(),
            get_PCB_Version(),
            get_Oppo_Boot_Mode(),
            get_Modem_Version());
    pr_err("get_Operator_Version:%d, get_dtsiNo:%d, get_audio_project:%d\n",
            get_Operator_Version(),
            get_dtsiNo(),
            get_audio());
    pr_err("oppo project info loading finished\n");

}

static bool cdt_integrity = false;
static int __init cdt_setup(char *str)
{
    if (str[0] == '1')
        cdt_integrity = true;

    return 1;
}
__setup("cdt_integrity=", cdt_setup);

unsigned int get_project(void)
{
 if (format == NULL) {
    init_project_version();
  }
    return format->nDataBCDT.ProjectNo;

}
EXPORT_SYMBOL(get_project);

unsigned int is_project(OPPO_PROJECT_OLDCDT project)
{
    return (get_project() == project?1:0);
}
EXPORT_SYMBOL(is_project);

unsigned int is_new_cdt(void)/*Q and R is new*/
{
  if (format == NULL) {
	init_project_version();
  }
    if(get_cdt_version() == OCDT_VERSION_1_0)
        return 1;
    else
        return 0;
}

unsigned int get_PCB_Version(void)
{
  if (format == NULL) {
    init_project_version();
  }
    return format->nDataSCDT.PCB;
}
EXPORT_SYMBOL(get_PCB_Version);

unsigned int get_Oppo_Boot_Mode(void)
{
  if (format == NULL) {
    init_project_version();
  }
    return format->nDataSCDT.OppoBootMode;
}
EXPORT_SYMBOL(get_Oppo_Boot_Mode);

int32_t get_Modem_Version(void)
{
  if (format == NULL) {
    init_project_version();
  }
    /*cdt return modem,ocdt return RF*/
    return format->nDataSCDT.RF;
}
EXPORT_SYMBOL(get_Modem_Version);

int32_t get_Operator_Version(void)
{
  if (format == NULL) {
    init_project_version();
  }
    return format->nDataSCDT.Operator;
}
EXPORT_SYMBOL(get_Operator_Version);

unsigned int get_dtsiNo(void)
{
    return (format && is_new_cdt()) ? format->nDataBCDT.DtsiNo : 0;
}
EXPORT_SYMBOL(get_dtsiNo);

unsigned int get_audio(void)
{
    return (format && is_new_cdt()) ? format->nDataBCDT.AudioIdx : 0;
}
EXPORT_SYMBOL(get_audio);


/*this module just init for creat files to show which version*/
static ssize_t prjVersion_read_proc(struct file *file, char __user *buf,
                size_t count, loff_t *off)
{
        char page[256] = {0};
        int len = 0;
        len = sprintf(page, "%d", get_project());

        if (len > *off) {
                len -= *off;
        }
        else
                len = 0;
        if (copy_to_user(buf, page, (len < count ? len : count))) {
                return -EFAULT;
        }
        *off += len < count ? len : count;
        return (len < count ? len : count);
}

struct file_operations prjVersion_proc_fops = {
        .read = prjVersion_read_proc,
        .write = NULL,
};


static ssize_t pcbVersion_read_proc(struct file *file, char __user *buf,
                size_t count, loff_t *off)
{
        char page[256] = {0};
        int len = 0;

        len = sprintf(page, "%d", get_PCB_Version());

        if (len > *off) {
                len -= *off;
        }
        else
                len = 0;

        if (copy_to_user(buf, page, (len < count ? len : count))) {
                return -EFAULT;
        }
        *off += len < count ? len : count;
        return (len < count ? len : count);
}

struct file_operations pcbVersion_proc_fops = {
        .read = pcbVersion_read_proc,
};


static ssize_t operatorName_read_proc(struct file *file, char __user *buf,
                size_t count, loff_t *off)
{
        char page[256] = {0};
        int len = 0;

        len = sprintf(page, "%d", get_Operator_Version());

        if (len > *off) {
                len -= *off;
        }
        else
                len = 0;

        if (copy_to_user(buf, page, (len < count ? len : count))) {
                return -EFAULT;
        }
        *off += len < count ? len : count;
        return (len < count ? len : count);
}

struct file_operations operatorName_proc_fops = {
        .read = operatorName_read_proc,
};


static ssize_t modemType_read_proc(struct file *file, char __user *buf,
                size_t count, loff_t *off)
{
        char page[256] = {0};
        int len = 0;

        len = sprintf(page, "%d", get_Modem_Version());

        if (len > *off) {
                len -= *off;
        }
        else
                len = 0;

        if (copy_to_user(buf, page, (len < count ? len : count))) {
                return -EFAULT;
        }
        *off += len < count ? len : count;
        return (len < count ? len : count);
}

struct file_operations modemType_proc_fops = {
        .read = modemType_read_proc,
};


static ssize_t oppoBootmode_read_proc(struct file *file, char __user *buf,
                size_t count, loff_t *off)
{
        char page[256] = {0};
        int len = 0;

        len = sprintf(page, "%d", get_Oppo_Boot_Mode());

        if (len > *off) {
                len -= *off;
        }
        else
                len = 0;

        if (copy_to_user(buf, page, (len < count ? len : count))) {
                return -EFAULT;
        }
        *off += len < count ? len : count;
        return (len < count ? len : count);
}

struct file_operations oppoBootmode_proc_fops = {
        .read = oppoBootmode_read_proc,
};

static int oppo_eng_version = OPPO_ENG_VERSION_NOT_INIT;
static int oppo_confidential = true;
int get_eng_version(void)
{
    int i = 0, eng_len = 3;
    char *substr = NULL;

    if (oppo_eng_version != OPPO_ENG_VERSION_NOT_INIT)
        return oppo_eng_version;

    if (strstr(boot_command_line, "is_confidential=0"))
        oppo_confidential = false;

    oppo_eng_version = RELEASE;
    substr = strstr(boot_command_line, "eng_version=");
    if (!substr) {      //if cmdline does't cover the version, we use normal version as default version
        printk(KERN_EMERG "cmdline does't have the sw_version %s \n", __func__);
        return oppo_eng_version;
    }

    substr += strlen("eng_version=");
    for (i = 0; substr[i] != ' ' && i < eng_len && substr[i] != '\0'; i++) {
        if ((substr[i] >= '0') && (substr[i] <= '9')) {
            oppo_eng_version = oppo_eng_version * 10 + substr[i] - '0';
        } else {
            oppo_eng_version = RELEASE;
            break;
        }
    }

    return oppo_eng_version;
}
EXPORT_SYMBOL(get_eng_version);

bool is_confidential(void)
{
    if (oppo_eng_version != OPPO_ENG_VERSION_NOT_INIT)
        return oppo_confidential;

    get_eng_version();

    return oppo_confidential;
}
EXPORT_SYMBOL(is_confidential);

uint32_t get_oppo_feature(enum F_INDEX index)
{
    if(is_new_cdt()){
      if (format == NULL) {
       init_project_version();
      }
        if (index < 1 || index > FEATURE_COUNT)
            return 0;
        return format?format->nDataBCDT.Feature[index-1]:0;
    }
    else
        return 0;
}
EXPORT_SYMBOL(get_oppo_feature);

static ssize_t eng_version_read_proc(struct file *file, char __user *buf,
                size_t count, loff_t *off)
{
        int ret = 0;
        char page[64] = {0};

        get_eng_version();
        snprintf(page, 63, "%d", oppo_eng_version);
        ret = simple_read_from_buffer(buf, count, off, page, strlen(page));

        return ret;
}

struct file_operations eng_version_proc_fops = {
        .read = eng_version_read_proc,
        .open  = simple_open,
        .owner = THIS_MODULE,
};

static ssize_t confidential_read_proc(struct file *file, char __user *buf,
                size_t count, loff_t *off)
{
        int ret = 0;
        char page[64] = {0};

        snprintf(page, 63, "%d", is_confidential());
        ret = simple_read_from_buffer(buf, count, off, page, strlen(page));

        return ret;
}

struct file_operations confidential_proc_fops = {
        .read = confidential_read_proc,
        .open  = simple_open,
        .owner = THIS_MODULE,
};

int rpmb_is_enable(void)
{
#define RPMB_KEY_PROVISIONED 0x00780178

    unsigned int rmpb_rd = 0;
    void __iomem *rpmb_addr = NULL;
    static unsigned int rpmb_enable = 0;

    if (rpmb_enable)
        return rpmb_enable;

    rpmb_addr = ioremap(RPMB_KEY_PROVISIONED , 4);    
    if (rpmb_addr) {
        rmpb_rd = __raw_readl(rpmb_addr);
        iounmap(rpmb_addr);
        rpmb_enable = (rmpb_rd >> 24) & 0x01;
    } else {
        rpmb_enable = 0;
    }

    return rpmb_enable;
}
EXPORT_SYMBOL(rpmb_is_enable);

#define QFPROM_RAW_SERIAL_NUM 0x00786134 /*different at each platform, please ref boot_images\core\systemdrivers\hwio\scripts\xxx\hwioreg.per
*/

static unsigned int g_serial_id = 0x00; /*maybe can use for debug
*/

static ssize_t serialID_read_proc(struct file *file, char __user *buf,
                size_t count, loff_t *off)
{
        char page[256] = {0};
        int len = 0;
        len = sprintf(page, "0x%x", g_serial_id);

        if (len > *off) {
                len -= *off;
        }
        else
                len = 0;

        if (copy_to_user(buf, page, (len < count ? len : count))) {
                return -EFAULT;
        }
        *off += len < count ? len : count;
        return (len < count ? len : count);
}


struct file_operations serialID_proc_fops = {
        .read = serialID_read_proc,
};

static void dump_ocp_info(struct seq_file *s)
{
  if (format == NULL) {
    init_project_version();
  }

    if (!format)
        return;

    seq_printf(s, "ocp: %d 0x%x %d 0x%x %c %c",
        format->nDataSCDT.PmicOcp[0],
        format->nDataSCDT.PmicOcp[1],
        format->nDataSCDT.PmicOcp[2],
        format->nDataSCDT.PmicOcp[3],
        format->nDataSCDT.PmicOcp[4],
        format->nDataSCDT.PmicOcp[5]);
}

static void dump_serial_info(struct seq_file *s)
{
    seq_printf(s, "0x%x", get_serialID());
}

static void dump_project_test(struct seq_file *s)
{
    return;
}

static void dump_oppo_feature(struct seq_file *s)
{
  if (format == NULL) {
    init_project_version();
  }

    if (!format)
        return;

    seq_printf(s, "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n",
        format->nDataBCDT.Feature[0],
        format->nDataBCDT.Feature[1],
        format->nDataBCDT.Feature[2],
        format->nDataBCDT.Feature[3],
        format->nDataBCDT.Feature[4],
        format->nDataBCDT.Feature[5],
        format->nDataBCDT.Feature[6],
        format->nDataBCDT.Feature[7],
        format->nDataBCDT.Feature[8],
        format->nDataBCDT.Feature[9]);
    return;
}

static void dump_eng_version(struct seq_file *s)
{
    seq_printf(s, "%d", get_eng_version());
    return;
}

static void dump_confidential_status(struct seq_file *s)
{
    seq_printf(s, "%d", is_confidential());
    return;
}

static void dump_secure_type(struct seq_file *s)
{
#define OEM_SEC_BOOT_REG 0x780350

    void __iomem *oem_config_base = NULL;
    uint32_t secure_oem_config = 0;

    oem_config_base = ioremap(OEM_SEC_BOOT_REG, 4);
    if (oem_config_base) {
        secure_oem_config = __raw_readl(oem_config_base);
        iounmap(oem_config_base);
    }

    seq_printf(s, "%d", secure_oem_config);    
}

static void dump_secure_stage(struct seq_file *s)
{
#define OEM_SEC_ENABLE_ANTIROLLBACK_REG 0x78019c

    void __iomem *oem_config_base = NULL;
    uint32_t secure_oem_config = 0;

    oem_config_base = ioremap(OEM_SEC_ENABLE_ANTIROLLBACK_REG, 4);
    if (oem_config_base) {
        secure_oem_config = __raw_readl(oem_config_base);
        iounmap(oem_config_base);
    }

    seq_printf(s, "%d", secure_oem_config);
}

static void update_manifest(struct proc_dir_entry *parent_1, struct proc_dir_entry *parent_2)
{
    static const char* manifest_src[2] = {
        "/vendor/odm/etc/vintf/manifest_ssss.xml",
        "/vendor/odm/etc/vintf/manifest_dsds.xml",
    };
    mm_segment_t fs;
    char * substr = strstr(boot_command_line, "simcardnum.doublesim=");

    if(!substr)
        return;

    substr += strlen("simcardnum.doublesim=");

    fs = get_fs();
    set_fs(KERNEL_DS);

    if (parent_1 && parent_2) {
        if (substr[0] == '0') {
            proc_symlink("manifest", parent_1, manifest_src[0]);//single sim
            proc_symlink("manifest", parent_2, manifest_src[0]);
        }
        else {
            proc_symlink("manifest", parent_1, manifest_src[1]);
            proc_symlink("manifest", parent_2, manifest_src[1]);
        }
    }

    set_fs(fs);
}

static void update_telephony_manifest(struct proc_dir_entry *parent_1, struct proc_dir_entry *parent_2)
{
    static const char* manifest_src[2] = {
        "/vendor/odm/etc/vintf/telephony_manifest_ssss.xml",
        "/vendor/odm/etc/vintf/telephony_manifest_dsds.xml",
    };
    mm_segment_t fs;
    char * substr = strstr(boot_command_line, "simcardnum.doublesim=");

    if(!substr)
        return;

    substr += strlen("simcardnum.doublesim=");

    fs = get_fs();
    set_fs(KERNEL_DS);

    if (parent_1 && parent_2) {
        if (substr[0] == '0') {
            proc_symlink("telephony_manifest", parent_1, manifest_src[0]);//single sim
            proc_symlink("telephony_manifest", parent_2, manifest_src[0]);
        }
        else {
            proc_symlink("telephony_manifest", parent_1, manifest_src[1]);
            proc_symlink("telephony_manifest", parent_2, manifest_src[1]);
        }
    }

    set_fs(fs);
}

static int project_read_func(struct seq_file *s, void *v)
{
    void *p = s->private;

    switch(Ptr2UINT32(p)) {
    case PROJECT_VERSION:
        if (get_project() > 0x20000) {
            seq_printf(s, "%X", get_project());
        } else {
            seq_printf(s, "%d", get_project());
        }
        break;
    case PCB_VERSION:
        seq_printf(s, "%d", get_PCB_Version());
        break;
    case OPPO_BOOTMODE:
        seq_printf(s, "%d", get_Oppo_Boot_Mode());
        break;
    case MODEM_TYPE:
    case RF_INFO:
        seq_printf(s, "%d", get_Modem_Version());
        break;
    case SECURE_TYPE:
        dump_secure_type(s);
        break;
    case SECURE_STAGE:
        dump_secure_stage(s);
        break;
    case OCP_NUMBER:
        dump_ocp_info(s);
        break;
    case SERIAL_NUMBER:
        dump_serial_info(s);
        break;
    case ENG_VERSION:
        dump_eng_version(s);
        break;
    case CONFIDENTIAL_STATUS:
        dump_confidential_status(s);
        break;
    case PROJECT_TEST:
        dump_project_test(s);
        break;
    case CDT_INTEGRITY:
        seq_printf(s, "%d", cdt_integrity);
        break;
    case OPPO_FEATURE:
        dump_oppo_feature(s);
        break;
    case OPERATOR_NAME:
        seq_printf(s, "%d", get_Operator_Version());
        break;
    default:
        seq_printf(s, "not support\n");
        break;
    }

    return 0;
}

unsigned int get_cdt_version(void)
{
  if (format == NULL) {
    init_project_version();
  }

    return format?format->Version:0;
}

static int projects_open(struct inode *inode, struct file *file)
{
    return single_open(file, project_read_func, PDE_DATA(inode));
}

static const struct file_operations project_info_fops = {
    .owner = THIS_MODULE,
    .open  = projects_open,
    .read  = seq_read,
    .release = single_release,
};

static int __init oppo_project_init(void)
{
    int ret = 0;
    struct proc_dir_entry *p_entry;
    void __iomem *serial_id_addr = NULL;

    serial_id_addr = ioremap(QFPROM_RAW_SERIAL_NUM , 4);
    if (serial_id_addr) {
            g_serial_id = __raw_readl(serial_id_addr);
            iounmap(serial_id_addr);
            printk(KERN_EMERG "serialID 0x%x\n", g_serial_id);
    } else
    {
            g_serial_id = 0xffffffff;
    }

    #ifdef VENDOR_EDIT
    // Cong.Dai@psw.bsp.tp, 2019.06.15, add for init engineering version
    get_eng_version();
    #endif /* VENDOR_EDIT */

    oppo_info_temp = proc_mkdir("oplusVersion", NULL);
    oppo_info = proc_mkdir("oppoVersion", NULL);
    if (!oppo_info || !oppo_info_temp) {
        goto error_init;
    }

    p_entry = proc_create_data("prjName", S_IRUGO, oppo_info, &project_info_fops, UINT2Ptr(PROJECT_VERSION));
    if (!p_entry)
        goto error_init;

    p_entry = proc_create_data("pcbVersion", S_IRUGO, oppo_info, &project_info_fops, UINT2Ptr(PCB_VERSION));
    if (!p_entry)
        goto error_init;

    p_entry = proc_create_data("oplusBootmode", S_IRUGO, oppo_info, &project_info_fops, UINT2Ptr(OPPO_BOOTMODE));
    if (!p_entry)
        goto error_init;

    p_entry = proc_create_data("RFType", S_IRUGO, oppo_info, &project_info_fops, UINT2Ptr(RF_INFO));
    if (!p_entry)
        goto error_init;

    p_entry = proc_create_data("modemType", S_IRUGO, oppo_info, &project_info_fops, UINT2Ptr(MODEM_TYPE));
    if (!p_entry)
        goto error_init;

    p_entry = proc_create_data("operatorName", S_IRUGO, oppo_info, &project_info_fops, UINT2Ptr(OPERATOR_NAME));
    if (!p_entry)
        goto error_init;

    p_entry = proc_create_data("secureType", S_IRUGO, oppo_info, &project_info_fops, UINT2Ptr(SECURE_TYPE));
    if (!p_entry)
        goto error_init;

    p_entry = proc_create_data("secureStage", S_IRUGO, oppo_info, &project_info_fops, UINT2Ptr(SECURE_STAGE));
    if (!p_entry)
        goto error_init;

    p_entry = proc_create_data("ocp", S_IRUGO, oppo_info, &project_info_fops, UINT2Ptr(OCP_NUMBER));
    if (!p_entry)
        goto error_init;

    p_entry = proc_create_data("serialID", S_IRUGO, oppo_info, &project_info_fops, UINT2Ptr(SERIAL_NUMBER));
    if (!p_entry)
        goto error_init;

    p_entry = proc_create_data("engVersion", S_IRUGO, oppo_info, &project_info_fops, UINT2Ptr(ENG_VERSION));
    if (!p_entry)
        goto error_init;

    p_entry = proc_create_data("isConfidential", S_IRUGO, oppo_info, &project_info_fops, UINT2Ptr(CONFIDENTIAL_STATUS));
    if (!p_entry)
        goto error_init;

    p_entry = proc_create_data("cdt", S_IRUGO, oppo_info, &project_info_fops, UINT2Ptr(CDT_INTEGRITY));
    if (!p_entry)
        goto error_init;

    p_entry = proc_create_data("feature", S_IRUGO, oppo_info, &project_info_fops, UINT2Ptr(OPPO_FEATURE));
    if (!p_entry)
        goto error_init;

    p_entry = proc_create_data("test", S_IRUGO, oppo_info, &project_info_fops, UINT2Ptr(PROJECT_TEST));
    if (!p_entry)
        goto error_init;

    /*update single or double cards*/
    //update_manifest(oppo_info);
    
    p_entry = proc_create_data("prjName", S_IRUGO, oppo_info_temp, &project_info_fops, UINT2Ptr(PROJECT_VERSION));
    if (!p_entry)
        goto error_init;

    p_entry = proc_create_data("pcbVersion", S_IRUGO, oppo_info_temp, &project_info_fops, UINT2Ptr(PCB_VERSION));
    if (!p_entry)
        goto error_init;

    p_entry = proc_create_data("oplusBootmode", S_IRUGO, oppo_info_temp, &project_info_fops, UINT2Ptr(OPPO_BOOTMODE));
    if (!p_entry)
        goto error_init;

    p_entry = proc_create_data("RFType", S_IRUGO, oppo_info_temp, &project_info_fops, UINT2Ptr(RF_INFO));
    if (!p_entry)
        goto error_init;

    p_entry = proc_create_data("modemType", S_IRUGO, oppo_info_temp, &project_info_fops, UINT2Ptr(MODEM_TYPE));
    if (!p_entry)
        goto error_init;

    p_entry = proc_create_data("operatorName", S_IRUGO, oppo_info_temp, &project_info_fops, UINT2Ptr(OPERATOR_NAME));
    if (!p_entry)
        goto error_init;

    p_entry = proc_create_data("secureType", S_IRUGO, oppo_info_temp, &project_info_fops, UINT2Ptr(SECURE_TYPE));
    if (!p_entry)
        goto error_init;

    p_entry = proc_create_data("secureStage", S_IRUGO, oppo_info_temp, &project_info_fops, UINT2Ptr(SECURE_STAGE));
    if (!p_entry)
        goto error_init;

    p_entry = proc_create_data("ocp", S_IRUGO, oppo_info_temp, &project_info_fops, UINT2Ptr(OCP_NUMBER));
    if (!p_entry)
        goto error_init;

    p_entry = proc_create_data("serialID", S_IRUGO, oppo_info_temp, &project_info_fops, UINT2Ptr(SERIAL_NUMBER));
    if (!p_entry)
        goto error_init;

    p_entry = proc_create_data("engVersion", S_IRUGO, oppo_info_temp, &project_info_fops, UINT2Ptr(ENG_VERSION));
    if (!p_entry)
        goto error_init;

    p_entry = proc_create_data("isConfidential", S_IRUGO, oppo_info_temp, &project_info_fops, UINT2Ptr(CONFIDENTIAL_STATUS));
    if (!p_entry)
        goto error_init;

    p_entry = proc_create_data("cdt", S_IRUGO, oppo_info_temp, &project_info_fops, UINT2Ptr(CDT_INTEGRITY));
    if (!p_entry)
        goto error_init;

    p_entry = proc_create_data("feature", S_IRUGO, oppo_info_temp, &project_info_fops, UINT2Ptr(OPPO_FEATURE));
    if (!p_entry)
        goto error_init;

    p_entry = proc_create_data("test", S_IRUGO, oppo_info_temp, &project_info_fops, UINT2Ptr(PROJECT_TEST));
    if (!p_entry)
        goto error_init;

    /*update single or double cards*/
    update_manifest(oppo_info, oppo_info_temp);
    update_telephony_manifest(oppo_info, oppo_info_temp);

    return 0;

error_init:
    remove_proc_entry("oppoVersion", NULL);
    remove_proc_entry("oplusVersion", NULL);
    return -ENOENT;
}

arch_initcall(oppo_project_init);

MODULE_DESCRIPTION("OPPO project version");
MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("Joshua <>");
