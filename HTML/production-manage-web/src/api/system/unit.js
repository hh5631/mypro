/*
 * @Author: liquan 1106970756@qq.com
 * @Date: 2022-04-22 14:50:35
 * @LastEditors: liquan 1106970756@qq.com
 * @LastEditTime: 2022-09-06 10:27:48
 * @FilePath: \production-manage-web\src\api\system\unit.js
 * @Description:
 *
 * Copyright (c) 2022 by liquan 1106970756@qq.com, All Rights Reserved.
 */
import request from '@/utils/request'

// 查询单位信息
export function unitInfo(userId) {
  return request({
    url: 'system/company/' + userId,
    method: 'get'
  })
}

// 删除单位信息
export function delUnit(userId) {
  return request({
    url: 'system/company/' + userId,
    method: 'delete'
  })
}

//  新增、编辑单位信息
export function setUnitInfo(data) {
  return request({
    url: 'system/company/addOrUpdate',
    method: 'post',
    data: data
  })
}

export function addFile(data) {
  return request({
    url: 'system/company/addFile',
    method: 'post',
    data: data
  })
}

export function delFile(data) {
  return request({
    url: 'system/company/delFile',
    method: 'post',
    data: data
  })
}
export function getRealPath(path) {
  const myPath = path.replaceAll('&', '%26')
  return request({
    url: 'file/realPath?path=' + myPath,
    method: 'get'
  })
}
