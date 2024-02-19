<!--
 * @Descripttion:
 * @version:
 * @Author: lirioing
 * @Date: 2022-07-04 14:54:09
 * @LastEditors: liquan 1106970756@qq.com
 * @LastEditTime: 2022-08-18 16:14:43
-->
<template>
  <div class="selectLeader">
    <div class="search">
      <el-input v-model.trim="queryParams.code" class="search-item" clearable placeholder="请输入物料编码" @keyup.enter.native="handleQuery" />
      <el-input v-model.trim="queryParams.name" class="search-item" clearable placeholder="请输入物料名称" @keyup.enter.native="handleQuery" />
      <el-input v-model.trim="queryParams.brandName" class="search-item" clearable placeholder="请输入品牌" @keyup.enter.native="handleQuery" />
      <el-input v-model.trim="queryParams.materialStandard" class="search-item" clearable placeholder="请输入规格型号" @keyup.enter.native="handleQuery" />
      <el-button size="small" @click="resetQuery">重置</el-button>
      <el-button size="small" @click="handleQuery">搜索</el-button>
    </div>
    <el-table
      ref="table"
      v-loading="loading"
      highlight-current-row
      :data="tableData"
      style="margin-top: 10px;"
      max-height="500"
      :row-key="getRowKeys"
      @selection-change="handleSelectionChange"
    >
      <el-table-column type="selection" width="50" :reserve-selection="true" align="center" />
      <!-- <el-table-column type="index" width="100" label="序号" /> -->
      <el-table-column key="code" label="物料编码" align="center" prop="code" :show-overflow-tooltip="true" />
      <el-table-column key="name" label="物料名称" align="center" prop="name" :show-overflow-tooltip="true" />
      <el-table-column key="brand" label="品牌" align="center" prop="brand" :show-overflow-tooltip="true" />
      <el-table-column key="standard" label="规格型号" align="center" prop="standard" :show-overflow-tooltip="true" />
      <el-table-column key="unit" label="计量单位" align="center" prop="unit" :show-overflow-tooltip="true" />
      <!-- <el-table-column key="num" label="闲置数量" align="center" prop="num" :show-overflow-tooltip="true" />
      <el-table-column key="warehouseName" label="归属仓库" align="center" prop="warehouseName" :show-overflow-tooltip="true" /> -->
    </el-table>
  </div>
</template>

<script>
import { listWarehouse } from '@/api/basicdata/warehouse'
import { getMaterialList } from '@/api/basicdata/material.js'
export default {
  data() {
    return {
      title: '',
      addApplyDig: false,
      tableData: [],
      selectedData: [], // 选中的数据
      queryParams: {
        pageNum: 0,
        pageSize: 100000,
        code: undefined,
        name: undefined,
        brandName: undefined,
        materialStandard: undefined
      },
      warehouseData: []
    }
  },
  created() {
    this.getList()
    this.getListWarehouse()
  },
  methods: {
    getList() {
      this.loading = true
      getMaterialList(this.queryParams).then(
        (response) => {
          this.loading = false
          this.tableData = response.data.records
          this.total = response.data.total
        }
      )
    },
    getListWarehouse() {
      /* 获取仓库信息 */
      listWarehouse({ pageSize: 999 }).then(response => {
        this.warehouseData = response.data.records
      })
    },
    handleQuery() {
      this.queryParams.pageNum = 1
      this.getList()
    },
    resetQuery() {
      this.queryParams = {
        pageNum: 0,
        pageSize: 1000,
        code: '',
        name: '',
        brandName: '',
        materialStandard: ''
      }
      this.handleQuery()
    },
    handleSelectionChange(val) {
      this.selectedData = val
    },
    getRowKeys(row) {
      return row.id + Math.random()
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
.search{
  display: flex;
  justify-content:space-between;
  padding-bottom: 10px;
  .search-item {
    width: 220px;
  }
}
</style>
