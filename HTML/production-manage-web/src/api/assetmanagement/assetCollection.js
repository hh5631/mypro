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
export function getWareGetByCondition(data) {
  return request({
    url: '/biz/wareGet/getWareGetByCondition',
    method: 'post',
    data
  })
}
// 删除领用
export function deleteWareGet(id) {
  return request({
    url: '/biz/wareGet/' + id,
    method: 'delete'
  })
}
// 新增
export function addOrUpdateAsset(params, data) {
  return request({
    url: '/biz/wareGet/addOrUpdate',
    method: 'post',
    params,
    data
  })
}
// 领用明细
export function getDetailsById(id) {
  return request({
    url: 'biz/wareGet/getDetailsById?id=' + id,
    method: 'get'
  })
}
// 领用详情
export function getDetailById(id) {
  return request({
    url: 'biz/wareGet/getDetailById?id=' + id,
    method: 'get'
  })
}
