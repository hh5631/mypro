/*
 * @Descripttion:
 * @version:
 * @Author: lirioing
 * @Date: 2022-07-05 10:31:22
 * @LastEditors: lirioing
 * @LastEditTime: 2022-07-19 16:50:38
 */
import request from '@/utils/request'

// 查询品牌列表
export function listBrand(query) {
  return request({
    url: '/system/brand/getBrandByCondition',
    method: 'post',
    data: query
  })
}
// 查询岗位详细
// export function getPost(postId) {
//   return request({
//     url: '/system/post/' + postId,
//     method: 'get'
//   })
// }

// 新增或编辑品牌
export function addOrUpdate(data) {
  return request({
    url: '/system/brand/addOrUpdate',
    method: 'post',
    data: data
  })
}

// 修改状态
export function updateState(id) {
  return request({
    url: '/system/brand/enable/' + id,
    method: 'post'
  })
}

// 删除品牌
export function delBrand(brandId) {
  return request({
    url: '/system/brand/' + brandId,
    method: 'delete'
  })
}
