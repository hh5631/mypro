/*
 * @Descripttion:
 * @version:
 * @Author: lirioing
 * @Date: 2022-07-05 14:05:22
 * @LastEditors: lirioing
 * @LastEditTime: 2022-07-11 15:43:57
 */
import request from '@/utils/request'

// 查询列表
export function listWarehouse(query) {
  return request({
    url: '/system/warehouse/list',
    method: 'get',
    params: query
  })
}

// 查询明细
export function getWarehouse(id) {
  return request({
    url: '/system/warehouse/' + id,
    method: 'get'
  })
}

// 新增或更新
export function saveOrUpdate(data) {
  return request({
    url: '/system/warehouse/saveOrUpdate',
    method: 'post',
    data: data
  })
}

// 仓库停用
export function warehouseStop(id, enableFlag) {
  return request({
    url: '/system/warehouse/enable/' + id + '?enableFlag=' + enableFlag,
    method: 'post'
  })
}

// 删除
export function delWarehouse(id) {
  return request({
    url: '/system/warehouse/' + id,
    method: 'delete'
  })
}
// 判断代码是否重复
export function judgeExist(id) {
  return request({
    url: '/system/warehouse/exist/' + id,
    method: 'get'
  })
}
