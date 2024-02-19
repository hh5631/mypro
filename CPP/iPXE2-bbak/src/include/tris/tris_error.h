#ifndef _TRIS_ERROR_H
#define _TRIS_ERROR_H

//GEDB没有数据
#define ERROR_NO_GEDB "Error-100: Machine Not Supported Or Not Target Machine"
//不在支持列表内
#define ERROR_NOT_SUPPORT "Error-101: Machine Not Supported"
//SLING没有数据
#define ERROR_NO_SLING "Error-102: Machine Not Supported Or Not Target Machine"
//非目标机器
#define ERROR_NOT_TARGET "Error-103: Incorrect DMI Or Not Target Machine"
//无法连接到GEDB
#define ERROR_CONNECT_GEDB "Error-104: Error When Connect Product Server"

//合并消息
#define ERROR_NO_GEDB_OR_NOT_SUPPORT "Error-100,101: Machine Not Supported Or Not Target Machine"
#define ERROR_NO_SLING_OR_NOT_SUPPORT "Error-101,102: Machine Not Supported Or Not Target Machine"

#define ERROR_EMPTY_USERNAME "Error-105: Username Can Not Be Empty"

//无法连接到Gateway
#define ERROR_CONNECT_GW "Error-200: Error When Connect Boot Server"
//无法连接到Tris
#define ERROR_CONNECT_TRIS "Error-201: Error When Connect TDS Server"
//key stuck
#define ERROR_KEY_STUCK "Error-202: A key is stuck"
//isn不符合规则. 规则如下: 9位ISN, 15位ISN (9位SN + # + 5位PN), 24位ISN (1-10位为SN, 11-15位为PN)
#define ERROR_INVALID_SN "Error-203: Invalid SN"
//isn=9, gedb无法获取pn, 输入pn参数小于5位
#define ERROR_INVALID_PN "Error-204: Invalid PN"
//tester账号不存在
#define ERROR_INVALID_ACCOUNT "Error-205: Invalid User Account"
//获取profile list失败
#define ERROR_GET_PROFILE "Error-206: Failed To Get Profile List From Gateway"
//解析profile list失败
#define ERROR_PARSE_PROFILE "Error-207: Failed To Extract Profile List"
//初始化网络失败
#define ERROR_INIT_NETWORK "Error-208: Failed To Up Network Device"
//设置Profile失败
#define ERROR_SET_PROFILE "Error-209: Failed To Set Current Profile"

#endif
