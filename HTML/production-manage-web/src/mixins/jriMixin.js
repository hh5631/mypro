/*
 * @Author: liquan 1106970756@qq.com
 * @Date: 2022-07-25 11:06:09
 * @LastEditors: liquan 1106970756@qq.com
 * @LastEditTime: 2022-07-25 14:43:13
 * @FilePath: \production-manage-web\src\mixins\jriMixin.js
 * @Description:
 *
 * Copyright (c) 2022 by liquan 1106970756@qq.com, All Rights Reserved.
 */
export const jriMixin = {
  data() {
    return {
      queryParams: {
        pageNum: 1,
        pageSize: 10
      },
      total: 0
    }
  },
  watch: {
    tableData: {
      handler(val) {
        if (val?.length === 0) {
          if (this.queryParams?.pageNum > 1) {
            this.queryParams.pageNum = 1
            this.getList()
          }
        }
      },
      deep: true
    }
  },
  methods: {
    resetQuery() {
      const res = JSON.parse(JSON.stringify(this.queryParams))
      Object.keys(res).forEach(item => {
        if (item !== 'pageSize' && item !== 'pageNum') {
          res[item] = ''
        }
      })
      this.queryParams = res
      this.handleQuery()
    },
    handleQuery() {
      this.queryParams.pageNum = 1
      this.getList()
    }
  }
}
