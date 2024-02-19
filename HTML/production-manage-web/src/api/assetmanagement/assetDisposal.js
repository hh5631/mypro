import request from '@/utils/request'
// 查询列表
export function getAssetHandleList(data) {
  return request({
    url: '/biz/assetHandle/list',
    method: 'post',
    data
  })
}
// 新增或编辑
export function addOrUpdateAssetHandle(params, data) {
  return request({
    url: '/biz/assetHandle/addOrUpdate',
    method: 'post',
    params,
    data
  })
}
// 处置明细
export function getAssetHandleById(id) {
  return request({
    url: 'biz/assetHandle/' + id,
    method: 'get'
  })
}
// 删除领用
export function deleteAssetHandle(id) {
  return request({
    url: '/biz/assetHandle/' + id,
    method: 'delete'
  })
}
