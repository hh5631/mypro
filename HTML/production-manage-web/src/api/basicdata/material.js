/*
 * @Descripttion:
 * @version:
 * @Author: lirioing
 * @Date: 2022-07-13 16:31:13
 * @LastEditors: lirioing
 * @LastEditTime: 2022-08-22 09:44:45
 */
import request from '@/utils/request'

// 物料列表查询
export function getMaterialList(data) {
  return request({
    url: '/system/material/getMaterialByCondition',
    method: 'post',
    data
  })
}

// 新增或编辑物料
export function addOrUpdateMaterialList(data) {
  return request({
    url: '/system/material/addOrUpdate',
    method: 'post',
    data
  })
}
// 根据id获取物料详情
export function getMaterialById(id) {
  return request({
    url: '/system/material/getMaterialById?id=' + id,
    method: 'post'
  })
}
// 导入物料
export function importMaterialList(data) {
  return request({
    url: '/system/material/importData',
    responseType: 'blob',
    method: 'post',
    data
  })
}
// 删除物料
export function delMaterial(id) {
  return request({
    url: '/system/material/' + id,
    method: 'delete'
  })
}
// 停用或启用切换
export function changeMaterialStatus(id) {
  return request({
    url: '/system/material/enable/' + id,
    method: 'post'
  })
}
// 自动生成物料编码
export function getMaterialCode(params) {
  return request({
    url: '/biz/orderNo/getMaterialCode',
    method: 'get',
    params
  })
}
