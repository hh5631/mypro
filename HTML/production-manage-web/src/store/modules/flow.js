/*
 * @Author: liquan 1106970756@qq.com
 * @Date: 2022-04-22 14:40:27
 * @LastEditors: liquan 1106970756@qq.com
 * @LastEditTime: 2022-08-16 09:13:28
 * @FilePath: \production-manage-web\src\store\modules\flow.js
 * @Description:
 *
 * Copyright (c) 2022 by liquan 1106970756@qq.com, All Rights Reserved.
 */
import { getProcessCategoryList } from '@/api/process'
export default {
  namespaced: true,
  state: {
    processCategoryList: null, // 流程分类
    vformdata: [],
    nowTaskId: ''
    // formDesignJson: null// 发起表单数据
  },
  actions: {},
  mutations: {
    SET_FORM(state, val) {
      if (!state.vformdata[state.nowTaskId]) {
        state.vformdata[state.nowTaskId] = {}
      }
      state.vformdata[state.nowTaskId].taskId = state.nowTaskId
      state.vformdata[state.nowTaskId].formId = val
    },
    SET_FORM_TYPE(state, taskType) {
      if (!state.vformdata[state.nowTaskId]) {
        state.vformdata[state.nowTaskId] = {}
      }
      state.vformdata[state.nowTaskId].taskId = state.nowTaskId
      state.vformdata[state.nowTaskId].taskType = taskType
    },
    // SET_DESIGNFORM(state, val) {
    //   state.formDesignJson = val
    // },
    SET_NOWTASKID(state, val) {
      state.nowTaskId = val
    },
    RESET_VFORMDATA(state, val) {
      state.vformdata = []
    },
    // 获取流程分类
    SET_PC_LIST(state) {
      getProcessCategoryList({ pageSize: 100000, pageNum: 1 }).then((da) => {
        state.processCategoryList = da.data.records || []
      })
    }
  }
}
