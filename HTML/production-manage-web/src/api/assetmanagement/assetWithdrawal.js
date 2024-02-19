/*
 * @Descripttion:
 * @version:
 * @Author: lirioing
 * @Date: 2022-08-02 08:31:38
 * @LastEditors: lirioing
 * @LastEditTime: 2022-08-18 09:18:09
 */
import request from '@/utils/request'
// 查询列表
export function getWareBackByCondition(data) {
  return request({
    url: '/biz/wareBack/getWareBackByCondition',
    method: 'post',
    data
  })
}
// 退库单详情
export function getWareBackDetailsById(id) {
  return request({
    url: 'biz/wareBack/getDetailById?id=' + id,
    method: 'get'
  })
}
// 删除退库单
export function deleteWareBack(id) {
  return request({
    url: '/biz/wareBack/' + id,
    method: 'delete'
  })
}
// 新增退库
export function addOrUpdateAssetBack(params, data) {
  return request({
    url: '/biz/wareBack/addOrUpdate',
    method: 'post',
    params,
    data
  })
}
// 确认退库
export function ensureWareBack(params, data) {
  return request({
    url: 'biz/wareBack/ensureWareBack',
    method: 'post',
    params,
    data
  })
}
// 新增添加明细
export function getAssetByCurrentUser(queryStr) {
  return request({
    url: 'biz/assetCard/getAssetByCurrentUser?queryStr=' + queryStr,
    method: 'get'
  })
}
