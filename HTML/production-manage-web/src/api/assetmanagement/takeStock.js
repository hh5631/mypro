import request from '@/utils/request'
// 查询列表
export function getCheckTaskList(params) {
  return request({
    url: '/biz/checkTask/list',
    method: 'get',
    params
  })
}
// 新增
export function saveOrUpdateCheckTask(data) {
  return request({
    url: '/biz/checkTask/saveOrUpdate',
    method: 'post',
    data
  })
}
// 根据id查询明细
export function getCheckTaskDetailsById(id) {
  return request({
    url: '/biz/checkTask/' + id,
    method: 'get'
  })
}
// 获取各类型盘点数量
export function getcheckDetailNum(params) {
  return request({
    url: '/biz/check-detail/num',
    method: 'get',
    params
  })
}
// 根据条件获取盘点明细
export function getcheckDetailList(params) {
  return request({
    url: '/biz/check-detail/byCondition',
    method: 'get',
    params
  })
}
// 盘点
export function startCheck(params) {
  return request({
    url: '/biz/check-detail/startCheck',
    method: 'post',
    params
  })
}
// 添加盘盈资产
export function addCheckPlus(params) {
  return request({
    url: '/biz/check-detail-plus/add',
    method: 'post',
    params
  })
}
// 完成盘点任务
export function completeTask(id) {
  return request({
    url: '/biz/checkTask/completeTask/' + id,
    method: 'put'
  })
}
// 修改盘盈资产
export function updateCheckPlus(params) {
  return request({
    url: '/biz/check-detail-plus/update',
    method: 'post',
    params
  })
}
// 删除盘盈明细
export function delCheckDetailPlus(params) {
  return request({
    url: '/biz/check-detail-plus/del',
    method: 'delete',
    params
  })
}
// 批量处理盘盈
export function batchCheckDetailPlus(params) {
  return request({
    url: '/biz/check-detail-plus/batch/process',
    method: 'post',
    params
  })
}
// 开始盘点任务
export function startTask(id) {
  return request({
    url: '/biz/checkTask/startTask/' + id,
    method: 'put'
  })
}
// 批量处理盘亏
export function batchCheckDetailLoss(params) {
  return request({
    url: '/biz/check-detail/loss/batch/process',
    method: 'post',
    params
  })
}
// 根据业务单号查询处置状态
export function getHandleStatus(params) {
  return request({
    url: '/biz/assetHandle/handleStatusByBizNo',
    method: 'get',
    params
  })
}
