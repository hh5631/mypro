/*
 * @Descripttion:
 * @version:
 * @Author: lirioing
 * @Date: 2022-07-12 15:20:25
 * @LastEditors: lirioing
 * @LastEditTime: 2022-07-14 10:17:37
 */
import request from '@/utils/request'
// 获取采购发票列表
export function getInvoiceList(params) {
  return request({
    url: '/biz/procurement/invoice/list',
    method: 'get',
    params
  })
}
// 查询明细
export function getInvoiceDetail(id) {
  return request({
    url: '/biz/procurement/invoice/' + id,
    method: 'get'
  })
}
// 删除
export function delInvoice(id) {
  return request({
    url: '/biz/procurement/invoice/' + id,
    method: 'delete'
  })
}
// 新增或者编辑
export function saveOrUpdateInvoice(params) {
  return request({
    url: '/biz/procurement/invoice/saveOrUpdate',
    method: 'post',
    data: params
  })
}
