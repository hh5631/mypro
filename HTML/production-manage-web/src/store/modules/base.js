/*
 * @Descripttion:
 * @version:
 * @Author: lirioing
 * @Date: 2022-08-22 09:08:35
 * @LastEditors: lirioing
 * @LastEditTime: 2022-08-22 10:07:14
 */
import { getNo } from '@/api/basicdata/index'
// 多个模块自动生成业务单号
const base = {
  state: {
    orderNo: ''
  },

  mutations: {
    SET_ORDER_NO: (state, orderNo) => {
      state.orderNo = orderNo
    }
  },

  actions: {
    // 各个模块获取业务流水号
    getNo({ commit }, pre) {
      return new Promise((resolve, reject) => {
        getNo({ pre }).then(res => {
          commit('SET_ORDER_NO', res.data)
          resolve(res)
        }).catch(error => {
          reject(error)
        })
      })
    }
  }
}

export default base
