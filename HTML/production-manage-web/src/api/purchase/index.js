/*
 * @Descripttion:
 * @version:
 * @Author: lirioing
 * @Date: 2022-07-13 16:31:13
 * @LastEditors: lirioing
 * @LastEditTime: 2022-08-19 17:03:30
 */
import request from '@/utils/request'

// 采购计划列表查询
export function getList(params) {
  return request({
    url: 'biz/procurement/plan/list',
    method: 'get',
    params
  })
}
// 根据id查询明细
export function getDetail(planId) {
  return request({
    url: 'biz/procurement/plan/' + planId,
    method: 'get'
  })
}
//  新增、编辑采购计划
export function saveOrUpdatePlan(data) {
  return request({
    url: 'biz/procurement/plan/saveOrUpdate',
    method: 'post',
    data: data
  })
}
// 废弃单据
export function delPlan(id) {
  return request({
    url: 'biz/procurement/plan/toVoid/' + id,
    method: 'delete'
  })
}
// 采购合同列表查询
export function getContractList(params) {
  return request({
    url: '/biz/procurement/contract/list',
    method: 'get',
    params
  })
}
// 新增或修改采购合同
export function saveOrUpdateContract(data) {
  return request({
    url: '/biz/procurement/contract/saveOrUpdate',
    method: 'post',
    data
  })
}
// 查看合同号是否被使用
export function checkContract(contractNo) {
  return request({
    url: '/biz/procurement/contract/exist/' + contractNo,
    method: 'get'
  })
}
// 采购合同详情
export function getContractDetail(id) {
  return request({
    url: '/biz/procurement/contract/' + id,
    method: 'get'
  })
}

// 删除采购合同
export function delContract(id) {
  return request({
    url: '/biz/procurement/contract/' + id,
    method: 'delete'
  })
}
// 采购申请列表查询
export function getApplyList(params) {
  return request({
    url: 'biz/procurement/order/list',
    method: 'get',
    params
  })
}
// 选择采购计划列表数据
export function getPlanSelector(params) {
  return request({
    url: '/biz/procurement/plan/selector',
    method: 'get',
    params
  })
}
// 保存采购申请
export function saveOrUpdateApply(data) {
  return request({
    url: '/biz/procurement/order/saveOrUpdate',
    method: 'post',
    data
  })
}
// 根据id查询申请明细
export function getApplyDetail(id) {
  return request({
    url: '/biz/procurement/order/' + id,
    method: 'get'
  })
}
// 废弃采购申请
export function delApply(id) {
  return request({
    url: '/biz/procurement/order/toVoid/' + id,
    method: 'delete'
  })
}
// 撤回采购申请
export function recallApply(id) {
  return request({
    url: '/biz/procurement/order/withdraw/' + id,
    method: 'post'
  })
}
// 获取审批流程列表
export function getProcessList(id) {
  return request({
    url: `/camunda/task/comments/${id}/list`,
    method: 'get'
  })
}
// 获取审批流程列表
export function getDefinitions(id) {
  return request({
    url: `/camunda/processDefinition/getDefinitions/` + id,
    method: 'get'
  })
}
// 删除采购申请
export function deleteApply(id) {
  return request({
    url: '/biz/procurement/order/del/?orderId=' + id,
    method: 'delete'
  })
}
// 获取业务流水号
export function getNo(id) {
  return request({
    url: `/system/orderNo/getNo`,
    method: 'get'
  })
}

export function getPlanDetailAfterDel(ids) {
  return request({
    url: `/biz/procurement/order/getPlanDetailAfterDel`,
    method: 'post',
    data: ids
  })
}
