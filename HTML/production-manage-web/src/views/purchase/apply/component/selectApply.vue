<!--
 * @Descripttion:
 * @version:
 * @Author: lirioing
 * @Date: 2022-07-04 14:54:09
 * @LastEditors: liquan 1106970756@qq.com
 * @LastEditTime: 2022-08-18 16:10:23
-->
<template>
  <div class="selectLeader">
    <el-input v-model.trim="queryParams.name" style="width:40%" maxlength="15" placeholder="请输入供应商名称搜索" size="mini" @keyup.enter.native="handleQuery" />
    <el-button size="small" @click="resetQuery">重置</el-button>
    <el-button size="small" style="margin-left:16px" type="primary" class="search-btn" plain @click="handleQuery">搜索</el-button>
    <el-table
      ref="rowRef"
      v-loading="loading"
      highlight-current-row
      :data="supplierTable"
      style="margin-top: 10px;"
      max-height="500"
      @current-change="handleCurrentChange"
    >
      <!-- <el-table-column type="selection" width="50" :reserve-selection="true" align="center" /> -->
      <el-table-column type="index" width="100" label="序号" />
      <el-table-column key="name" label="供应商名称" align="center" prop="name" :show-overflow-tooltip="true" />
      <el-table-column key="code" label="供应商代码" align="center" prop="code" :show-overflow-tooltip="true" />
      <el-table-column key="contacts" label="供应商联系人" align="center" prop="contacts" />
      <el-table-column key="phone" label="供应商电话" align="center" prop="phone" />
    </el-table>
  </div>
</template>

<script>
import { getSupplierList } from '@/api/basicdata/index'
export default {
  data() {
    return {
      title: '',
      addApplyDig: false,
      supplierTable: [],
      currentRow: {}, // 选中的当前行数据
      queryParams: {
        name: undefined,
        pageNum: 1,
        enable: 0,
        pageSize: 99999
      }
    }
  },
  created() {
    this.getList()
  },
  methods: {
    getList() {
      this.loading = true
      getSupplierList(this.addDateRange(this.queryParams)).then(
        (response) => {
          this.loading = false
          this.supplierTable = response.data.records
          this.total = response.data.total
        }
      )
    },
    handleQuery() {
      this.queryParams.pageNum = 1
      this.getList()
    },
    resetQuery() {
      this.queryParams.name = ''
      this.handleQuery()
    },
    handleCurrentChange(val) {
      this.currentRow = val
    },
    addApply() {
      this.addApplyDig = true
    }
  }
}
</script>

<style lang="scss" scoped>
.selectLeader{
  width: 100%;
  height: 100%;
  padding: 16px;
  overflow-y: scroll;
}
</style>
