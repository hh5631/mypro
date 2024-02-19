import request from '@/utils/request'

// 获取流程分类
export function getProcessCategoryList(params) {
  return request({
    url: '/camunda/category/list',
    method: 'get',
    params
  })
}
// 新增流程分类
export function addProcessCategory(data) {
  return request({
    url: '/camunda/category/add',
    method: 'post',
    data
  })
}
// 编辑流程分类
export function updateProcessCategory(data) {
  return request({
    url: '/camunda/category/update',
    method: 'post',
    data
  })
}
// 删除流程分类
export function delProcessCategory(categoryId) {
  return request({
    url: `/camunda/category/${categoryId}`,
    method: 'delete'
  })
}
// 激活挂起流程
export function activeFlow(processDefinitionId) {
  return request({
    url: `/camunda/processDefinition/suspendOrActiveApply/${processDefinitionId}`,
    method: 'post'
  })
}

// 获取表单列表
export function getFormList(params) {
  return request({
    url: `/camunda/design/form/list`,
    method: 'get',
    params
  })
}
// 新增表单
export function addForm(data) {
  return request({
    url: `/camunda/design/form/add`,
    method: 'post',
    data
  })
}
// 编辑表单
export function updateForm(data) {
  return request({
    url: `/camunda/design/form/update`,
    method: 'post',
    data
  })
}
// 删除表单
export function delForm(formId) {
  return request({
    url: `/camunda/design/form/${formId}`,
    method: 'delete'
  })
}
// 判断表单名称是否重复
export function judgeFormNameRepeat(formName) {
  return request({
    url: `/camunda/design/form/exist/${formName}`,
    method: 'get'
  })
}
// 获取表单详情
export function getFormDetail(formId) {
  return request({
    url: `/camunda/design/form/${formId}`,
    method: 'get'
  })
}
// 判断表单是否与流程绑定
export function judgeFormBindDesign(formId) {
  return request({
    url: `/camunda/design/form/isBindProcess/${formId}`,
    method: 'get'
  })
}
// 获取流程列表
export function getFlowList(params) {
  return request({
    url: `/camunda/processDefinition/list`,
    method: 'get',
    params
  })
}
// 获取流程列表
export function judgeFlowNameRepeat(processName) {
  return request({
    url: `/camunda/processDefinition/exist/${processName}`,
    method: 'get'
  })
}
// 获取
export function selectLikeCountByProcessName(processName) {
  return request({
    url: `/camunda/processDefinition/selectLikeCountByProcessName/${processName}`,
    method: 'get'
  })
}
// 检查业务类型是否重复
export function judgeBizTypeRepeat(bizType) {
  return request({
    url: `/camunda/processDefinition/exist/bizType/${bizType}`,
    method: 'get'
  })
}
// 部署流程
export function addFlow(data) {
  return request({
    url: '/camunda/processDefinition/addDeploymentByString',
    method: 'post',
    data
  })
}
// 查询xml
export function getFlowXml(params) {
  return request({
    url: '/camunda/processDefinition/getDefinitionXML',
    method: 'get',
    params
  })
}
// 删除流程
export function deleteFlow(deploymentId) {
  return request({
    url: `/camunda/processDefinition/remove/${deploymentId}`,
    method: 'delete'
  })
}
// 表单归类
export function formMove(data) {
  return request({
    url: '/camunda/design/form/categoryMove',
    method: 'post',
    data
  })
}
// 流程归类
export function processMove(data) {
  return request({
    url: '/camunda/processDefinition/categoryMove',
    method: 'post',
    data
  })
}

// 获取流程发起列表
export function getFlowLaunchList(params) {
  return request({
    url: `/camunda/processLaunch/list`,
    method: 'get',
    params
  })
}
// 发起流程
export function startFlow(data) {
  return request({
    url: `/camunda/processLaunch/start`,
    method: 'post',
    data
  })
}
// 更新流程表单 数据
export function updateFlowData(taskId, data) {
  return request({
    url: `/camunda/processLaunch/update?taskId=${taskId}`,
    method: 'post',
    data
  })
}
// 查询我的待办
export function getMyTask(params) {
  return request({
    url: `/camunda/task/myTodoTasks`,
    method: 'get',
    params
  })
}
// 查询流转记录
export function getFlowRecord(processInstanceId) {
  return request({
    url: `/camunda/task/comments/${processInstanceId}/list`,
    method: 'get'
  })
}
// 查询已办任务
export function getTaskHistory(params) {
  return request({
    url: `/camunda/task/history`,
    method: 'get',
    params
  })
}
// 查询我发起的任务
export function getMyInitiatedTasks(params) {
  return request({
    url: `/camunda/task/myInitiatedTasks`,
    method: 'get',
    params
  })
}
// 撤回任务 状态为待审批的时候可以撤回
export function withdrawTask(processInstanceId) {
  return request({
    url: `/camunda/task/withdraw/${processInstanceId}`,
    method: 'post'
  })
}
// 作废任务 状态为待提交, 已撤回可以作废流程
export function toVoidTask(id) {
  return request({
    url: `/camunda/task/toVoid/${id}`,
    method: 'delete'
  })
}
// 流程图高亮
export function getHighLine(params) {
  return request({
    url: '/camunda/history/getHighLine',
    method: 'get',
    params
  })
}
// // 激活挂起流程
// export function activeFlow(key) {
//   return request({
//     url: `/api-camunda/processDefinition/suspendOrActiveApply/${key}`,
//     method: 'post'
//   })
// }
// // 流程表单查询
// export function getFlowFormList() {
//   return request({
//     url: `/api-camunda/workflow/list`,
//     method: 'get'
//   })
// }

// 完成任务
export function dealProcess(data) {
  return request({
    url: `/camunda/task/complete/${data.taskId}`,
    method: 'post',
    data
  })
}
