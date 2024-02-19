/*
 * @Descripttion:
 * @version:
 * @Author: lirioing
 * @Date: 2022-07-04 14:54:08
 * @LastEditors: lirioing
 * @LastEditTime: 2022-08-22 09:09:05
 */
import Vue from 'vue'
import Vuex from 'vuex'
import app from './modules/app'
import user from './modules/user'
import tagsView from './modules/tagsView'
import permission from './modules/permission'
import settings from './modules/settings'
import flow from './modules/flow'
import getters from './getters'
import base from './modules/base'

Vue.use(Vuex)

const store = new Vuex.Store({
  modules: {
    app,
    user,
    tagsView,
    permission,
    settings,
    flow,
    base
  },
  getters
})

export default store
