/*
 * @Descripttion:
 * @version:
 * @Author: lirioing
 * @Date: 2022-08-01 15:43:01
 * @LastEditors: lirioing
 * @LastEditTime: 2022-08-26 15:28:09
 */
import request from '@/utils/request'
// 查询资产卡片
export function getCanHandleAssetCard(query) {
  return request({
    url: '/biz/assetCard/getCanHandleAssetCard',
    method: 'get',
    params: query
  })
}
// 查询资产卡片详情
export function getAssetDetailById(params) {
  return request({
    url: '/biz/assetCard/getAssetDetailById',
    method: 'post',
    params
  })
}
// 获取仓库信息-资产卡片生成
export function getStockList(params) {
  return request({
    url: '/biz/assetCard/getStockList',
    method: 'post',
    params
  })
}
// 获取仓库信息明细-资产卡片生成
export function getStockVoById(params) {
  return request({
    url: '/biz/assetCard/getStockVoById',
    method: 'post',
    params
  })
}
// 删除资产卡片
export function delassetCard(id) {
  return request({
    url: '/biz/assetCard/' + id,
    method: 'delete'
  })
}
// 生成资产卡片
export function createAssetCard(data) {
  return request({
    url: '/biz/assetCard/createAssetCard',
    method: 'post',
    data
  })
}
// 绑定rfid更新rfid
export function batchUpdateRfId(data) {
  return request({
    url: '/biz/assetCard/batchUpdateRfId',
    method: 'post',
    data
  })
}

// 更新绑定状态
export function updateAssetCardState(params) {
  return request({
    url: '/biz/assetCard/updateBindStatusByCardId',
    method: 'post',
    params
  })
}
// 接口生成rfid
export function getMaxRfid() {
  return request({
    url: '/biz/assetCard/getMaxRfid',
    method: 'get'
  })
}
/**
 * 把状态设置为可领
 * @returns
 */
export function changeAssetState(ids) {
  return request({
    url: '/biz/assetCard/changeStatus',
    method: 'POST',
    data: ids
  })
}
