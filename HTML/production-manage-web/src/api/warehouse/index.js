/*
 * @Author: liquan 1106970756@qq.com
 * @Date: 2022-07-25 16:17:17
 * @LastEditors: lirioing
 * @LastEditTime: 2022-08-24 16:41:16
 * @FilePath: \production-manage-web\src\api\warehouse\index.js
 * @Description:
 *
 * Copyright (c) 2022 by liquan 1106970756@qq.com, All Rights Reserved.
 */
import request from '@/utils/request'

/*  库存台账*/
// 库存台账列表查询
export function getStockInfoList (params) {
  return request({
    url: '/biz/stockInfo/list',
    method: 'get',
    params
  })
}
// 库存台账根据id查询明细
export function getStockDetail (id) {
  return request({
    url: `/biz/stockInfo/${id}`,
    method: 'get'
  })
}
/**
 * 修改库存台账中的安全台账
 * @param {*} id
 * @param {*} safeNum
 * @returns
 */
export function changeSafeNum ({ id, safeNum }) {
  return request({
    url: `/biz/stockInfo/update`,
    method: 'POST',
    data: {
      id,
      safeNum
    }
  })
}
// 库存台账获取库存变化历史
export function getStockHistory (params) {
  return request({
    url: `/biz/stockInfo/his`,
    method: 'get',
    params
  })
}
// 库存台账库存信息导出
export function exportStockInfo () {
  return request({
    url: `/biz/stockInfo/export`,
    headers: { 'Content-Type': 'application/json' },
    responseType: 'blob',
    method: 'post'
  })
}
/* 入库管理*/
// 入库单列表查询
export function getWarehousingOrder (params) {
  return request({
    url: '/biz/warehousingOrder/list',
    params
  })
}
// 入库单id查询
export function getWarehousingOrderById (id) {
  return request({
    url: '/biz/warehousingOrder/' + id,
    method: 'get'
  })
}
// 新增入库单
export function addWarehousingOrder (data) {
  return request({
    url: '/biz/warehousingOrder/add',
    method: 'post',
    data
  })
}
// 入库单删除
export function deleteWarehousingOrderById (id) {
  return request({
    url: '/biz/warehousingOrder/' + id,
    method: 'delete'
  })
}
// 入库单导出
export function exportWarehousingOrder (data) {
  return request({
    url: '/biz/warehousingOrder/export',
    headers: { 'Content-Type': 'application/json' },
    responseType: 'blob',
    method: 'post',
    data
  })
}
// 出库单列表查询
export function getOutboundOrderOrder (data) {
  return request({
    url: '/biz/outboundOrder/list',
    method: 'post',
    data
  })
}
// 出库单删除
export function deleteOutboundOrderOrderById (id) {
  return request({
    url: '/biz/outboundOrder/' + id,
    method: 'delete'
  })
}

// 出库单导出
export function exportOutboundOrder (data) {
  return request({
    url: '/biz/outboundOrder/export',
    headers: { 'Content-Type': 'application/json' },
    responseType: 'blob',
    method: 'post',
    data
  })
}
// 出库单id查询
export function getOutboundOrderById (id) {
  return request({
    url: '/biz/outboundOrder/' + id,
    method: 'get'
  })
}
// 入库单确认入库
export function confirmWarehousingOrder (data) {
  return request({
    url: '/biz/warehousingOrder/confirm',
    method: 'post',
    data
  })
}
// 入库单确认入库
export function confirmWarehousingOrderById (id) {
  return request({
    url: '/biz/warehousingOrder/confirm/' + id,
    method: 'get'
  })
}
// 入库单废弃
export function toVoidWarehousingOrder (id) {
  return request({
    url: '/biz/warehousingOrder/toVoid/' + id,
    method: 'post'
  })
}
// 新增出库单
export function addOutboundOrder (data) {
  return request({
    url: '/biz/outboundOrder/add',
    method: 'post',
    data
  })
}
// 修改出库单(此为确认出库接口)
export function updateOutboundOrder (data) {
  return request({
    url: '/biz/outboundOrder/update',
    method: 'post',
    data
  })
}
// 出库单确认(此接口不用)
export function confirmOutboundOrder (id) {
  return request({
    url: '/biz/outboundOrder/confirm/' + id,
    method: 'get'
  })
}
// 操作日志
export function getLog (params) {
  return request({
    url: '/biz/common-biz-log/querPage',
    method: 'get',
    params
  })
}
// 修改入库单
export function updateWarehousingOrder (data) {
  return request({
    url: '/biz/warehousingOrder/update',
    method: 'post',
    data
  })
}

// 查询物料列表
export function getIdleAssetList (data) {
  return request({
    url: '/biz/outboundOrder/getIdleAssetList',
    method: 'post',
    data
  })
}
// 出库单废弃
export function abandonOutboundOrderOrderById (id) {
  return request({
    url: '/biz/outboundOrder/abandon/' + id,
    method: 'get'
  })
}
// 根据出库单Id及物料Id查询资产信息
export function getAssetByOutIdAndMaterialId (params) {
  return request({
    url: '/biz/assetCard/getAssetByOutIdAndMaterialId',
    method: 'get',
    params
  })
}
// 获取审核通过的采购单
export function getApplyList () {
  return request({
    url: '/biz/warehousingOrder/purchase',
    method: 'get'
  })
}
// 获取审核通过的采购单
export function getApplyDetail (id) {
  return request({
    url: '/biz/warehousingOrder/purchase/detail/' + id,
    method: 'get'
  })
}
export function getDetailsById (id) {
  return request({
    url: 'biz/wareGet/getDetailsById' + id,
    method: 'get'
  })
}

// 出库明细资产查询
export function getAssetCardByMaterialCodeAndWarehouseId (data) {
  return request({
    url: '/biz/assetCard/getAssetCardByMaterialCodeAndWarehouseId',
    method: 'post',
    data
  })
}
// 退库台账确认退库
export function ensureWareBack (params, data) {
  return request({
    url: '/biz/wareBack/ensureWareBack',
    method: 'post',
    params,
    data
  })
}
// 退库台账列表
export function getWareBackByAccount (data) {
  return request({
    url: '/biz/wareBack/getWareBackByAccount',
    method: 'post',
    data
  })
}
