
import request from '@/utils/request'

// 折旧列表查询
export function getAssetDepreciationList(params) {
  return request({
    url: 'biz/assetDepreciation/list',
    method: 'get',
    params
  })
}

// 消息列表查询
export function getbyCondition(params) {
  return request({
    url: 'message/byCondition',
    method: 'get',
    params
  })
}

// 删除消息
export function deleteMessage(params) {
  return request({
    url: '/message//del/',
    method: 'delete',
    params
  })
}

// 查询列表
export function postSetDepreciation(data) {
  return request({
    url: '/biz/assetDepreciation/update',
    method: 'post',
    data
  })
}

// 消息列表查询
export function getNumByReadType(params) {
  return request({
    url: 'message/numByReadType',
    method: 'get',
    params
  })
}

// 全部已读
export function putAllRead() {
  return request({
    url: '/message/allRead/',
    method: 'put'
  })
}

// 查看一条
export function putUpdateReadStatus(params) {
  return request({
    url: '/message/updateReadStatus',
    method: 'put',
    params
  })
}

// 折旧历史
export function getAssDepHisList(params) {
  return request({
    url: '/biz/assetDepreciationHistory/list',
    method: 'get',
    params
  })
}

// /material/batchUpdateDepreciationInfo
// 查询列表
export function postBaUpDepInfo(data) {
  return request({
    url: '/system/material/batchUpdateDepreciationInfo',
    method: 'post',
    data
  })
}
