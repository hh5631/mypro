/*
 * @Author: liquan 1106970756@qq.com
 * @Date: 2022-04-24 14:27:07
 * @LastEditors: liquan 1106970756@qq.com
 * @LastEditTime: 2022-09-07 16:19:24
 * @FilePath: \production-manage-web\src\utils\process.js
 * @Description:
 *
 * Copyright (c) 2022 by liquan 1106970756@qq.com, All Rights Reserved.
 */
import { getProcessCategoryList, getFormList, startFlow } from '@/api/process'

/* 发起任务(流程) */
export function startProcess(_that, ref, params) {
  return new Promise(resolve => {
    const { processKey, formId, bindId } = { ...params }
    _that.$refs[ref].getFormData().then(formData => {
      // 取消发起弹框
      // _that.$confirm('确认发起吗?', '警告', {
      //   confirmButtonText: '确定',
      //   cancelButtonText: '取消',
      //   type: 'warning'
      // }).then(() => {
      const data = { processKey, formId, bindId, formDataJson: JSON.stringify(formData) }
      const loading = _that.$loading({
        lock: true,
        text: '提交中',
        spinner: 'el-icon-loading',
        background: 'rgba(0, 0, 0, 0.7)'
      })
      startFlow(data)
        .then(da => {
          _that.$message.success(da.msg || '操作成功')
        }).finally(() => {
          loading.close()
          resolve()
        })
        .catch(function () { })
    })
  }).catch(error => {
    _that.$message.error(error)
  })
  // })
}

/* 表单分类级联 */
export function getFormCascaderData() {
  return new Promise(resolve => {
    Promise.all([getProcessCategoryList({ pageSize: 100000, pageNum: 1 }), getFormList({ pageSize: 9999 })]).then(da => {
      const resData = da[0].data.records.map(item => {
        return { label: item.categoryName, value: item.id, children: [] }
      })
      const formList = da[1].data.records
      resData.filter(t =>
        formList.some(f => {
          if (f.categoryId === t.value) {
            t.children.push({ label: f.formName, value: f.id, formJson: f.formJson })
          }
        })
      )
      resolve(resData)
    })
  })
}
