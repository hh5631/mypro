import request from '@/utils/request'
import { parseStrEmpty } from '@/utils/jri'

// 供应商列表查询
export function getSupplierList(params) {
  return request({
    url: 'system/supplier/list',
    method: 'get',
    params
  })
}
// 供应商修改状态
export function updateSupplierState(id) {
  return request({
    url: '/system/supplier/enable/' + id,
    method: 'post'
  })
}
// 删除供应商
export function delSupplier(id) {
  return request({
    url: '/system/supplier/' + id,
    method: 'delete'
  })
}
//  新增、编辑供应商信息
export function saveOrUpdateSupplier(data) {
  return request({
    url: 'system/supplier/saveOrUpdate',
    method: 'post',
    data: data
  })
}
// 查询供应商详细
export function getSupplier(id) {
  return request({
    url: '/system/supplier/' + parseStrEmpty(id),
    method: 'get'
  })
}
//  计量单位查询
export function getUnitByCondition(data) {
  return request({
    url: 'system/unit/getUnitByCondition',
    method: 'post',
    data: data
  })
}
//  新增、编辑计量单位信息
export function addOrUpdateUnit(data) {
  return request({
    url: 'system/unit/addOrUpdate',
    method: 'post',
    data: data
  })
}
// 删除计量单位
export function delUnit(id) {
  return request({
    url: '/system/unit/' + id,
    method: 'delete'
  })
}
// 计量单位修改状态
export function updateUnitState(id) {
  return request({
    url: '/system/unit/enable/' + id,
    method: 'post'
  })
}
// 位置列表查询
export function getPlaceList(data) {
  return request({
    url: '/system/place/list',
    method: 'post',
    data
  })
}
// 位置列表查询
export function getPlaceDetail(id) {
  return request({
    url: '/system/place/' + id,
    method: 'get'
  })
}
// 位置树状查询
export function getPlaceTreeSelect() {
  return request({
    url: '/system/place/treeSelect',
    method: 'get'
  })
}
// 新增位置
export function addPlace(data) {
  return request({
    url: '/system/place/add',
    method: 'post',
    data
  })
}
// 修改位置
export function updatePlace(data) {
  return request({
    url: '/system/place/update',
    method: 'post',
    data
  })
}
// 删除位置
export function delPlace(id) {
  return request({
    url: '/system/place/' + id,
    method: 'delete'
  })
}
// 批量停用或启用
export function updateStatusBatch(data) {
  return request({
    url: '/system/place/updateStatusBatch',
    method: 'post',
    data
  })
}
// 停用或启用切换
export function changeStatus(id) {
  return request({
    url: '/system/place/enable/' + id,
    method: 'post'
  })
}
// 资产类别列表查询
export function getFixedAssetList(params) {
  return request({
    url: '/system/fixedAsset/list',
    method: 'get',
    params
  })
}
// 资产类别树状查询
export function getFixedAssetTree() {
  return request({
    url: 'system/fixedAsset/directory/treeListAll',
    method: 'get'
  })
}
// 资产状态切换
export function changeFixedAssetStatus(code) {
  return request({
    url: '/system/fixedAsset/enable/' + code,
    method: 'post'
  })
}
// 新增或修改资产
export function insertOrUpdateFixedAsset(data) {
  return request({
    url: '/system/fixedAsset/insertOrUpdate',
    method: 'post',
    data
  })
}
// 新增检查code是否已经存在
export function checkFixedAsset(code) {
  return request({
    url: '/system/fixedAsset/exist/' + code,
    method: 'get'
  })
}
// 根据id获取资产
export function getFixedAssetByCode(code) {
  return request({
    url: '/system/fixedAsset/' + code,
    method: 'get'
  })
}
// 删除资产
export function delFixedAsset(code) {
  return request({
    url: '/system/fixedAsset/' + code,
    method: 'delete'
  })
}
// 层级查询
export function getMaterialListByLevel(params) {
  return request({
    url: '/system/fixedAsset/listByLevel',
    method: 'get',
    params
  })
}
// 自动生成供应商编码
export function getSupplierCode(params) {
  return request({
    url: '/biz/orderNo/getSupplierCode',
    method: 'get',
    params
  })
}
// 自动生成业务流水号
export function getNo(params) {
  return request({
    url: '/biz/orderNo/getNo',
    method: 'get',
    params
  })
}

// 获取资产类别一级、二级树结构
export function getTree() {
  return request({
    url: '/system/fixedAsset/directory/treeList',
    method: 'get'
  })
}
