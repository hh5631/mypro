<!--
 * @Descripttion:
 * @version:
 * @Author: lirioing
 * @Date: 2022-07-04 14:54:09
 * @LastEditors: liquan 1106970756@qq.com
 * @LastEditTime: 2022-09-09 20:27:37
-->
<template>
  <div class="selectLeader">
    <div class="search">
      <el-form ref="queryForm" :model="queryParams" :inline="true">
        <el-form-item label="资产名称" prop="materialName">
          <el-input v-model.trim="queryParams.materialName" placeholder="请输入资产名称" @keyup.enter.native="handleQuery" />
        </el-form-item>
        <!-- <el-form-item label="资产编码" prop="materialCode">
          <el-input v-model.trim="queryParams.materialCode" placeholder="请输入资产编码" @keyup.enter.native="handleQuery" />
        </el-form-item> -->
        <el-form-item label="品牌" prop="brandName">
          <el-input v-model.trim="queryParams.brandName" placeholder="请输入品牌" @keyup.enter.native="handleQuery" />
        </el-form-item>
        <el-form-item label="规格型号" prop="standard">
          <el-input v-model.trim="queryParams.standard" placeholder="请输入品牌" @keyup.enter.native="handleQuery" />
        </el-form-item>
        <el-form-item label="存放仓库" prop="warehouseId">
          <el-select v-model="queryParams.warehouseId" placeholder="请选择" clearable size="small">
            <el-option
              v-for="item in warehouseData"
              :key="item.id"
              :label="item.name"
              :value="item.id"
            />
          </el-select>
        </el-form-item>
        <el-form-item>
          <el-button v-debounce size="small" @click="resetQuery">重置</el-button>
          <el-button v-debounce type="primary" class="search-btn" plain size="small" @click="handleQuery">搜索</el-button>
        </el-form-item>
      </el-form>
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
      <el-table-column key="type" label="资产类别" align="center" prop="categoryNameLevel" :show-overflow-tooltip="true" />
      <el-table-column key="materialName" label="资产名称" align="center" prop="materialName" :show-overflow-tooltip="true" />
      <!-- <el-table-column key="materialCode" label="资产编码" align="center" prop="materialCode" :show-overflow-tooltip="true" /> -->
      <el-table-column key="brandName" label="品牌" align="center" prop="brandName" :show-overflow-tooltip="true" />
      <el-table-column key="standard" label="规格型号" align="center" prop="standard" :show-overflow-tooltip="true" />
      <el-table-column key="unitName" label="计量单位" align="center" prop="unitName" :show-overflow-tooltip="true" />
      <el-table-column key="existNum" label="可领用数量" align="center" prop="existNum" :show-overflow-tooltip="true" />
      <el-table-column key="warehouseName" label="存放仓库" align="center" prop="warehouseName" :show-overflow-tooltip="true" />
    </el-table>
  </div>
</template>

<script>
// 出库管理 搜索
import { listWarehouse } from '@/api/basicdata/warehouse'
import { getIdleAssetList } from '@/api/warehouse/index'
export default {
  data() {
    return {
      title: '',
      addApplyDig: false,
      tableData: [],
      selectedData: [], // 选中的数据
      queryParams: {
        pageNum: 0,
        pageSize: 1000,
        warehouseId: '',
        materialName: '',
        // materialCode: '',
        brandName: '',
        standard: ''
      },
      warehouseData: [],
      loading: false
    }
  },
  created() {
    // this.handleQuery()
  },
  methods: {
    getList() {
      this.loading = true
      getIdleAssetList(this.queryParams).then(
        (response) => {
          this.loading = false
          this.tableData = response.data.records.map(item => {
            return {
              ...item, categoryNameLevel: item.categoryNameLevel4 || item.categoryNameLevel3 || item.categoryNameLevel2 || item.categoryNameLevel1
            }
          })
          this.total = response.data.total
        }
      )
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
        warehouseId: '',
        materialName: '',
        // materialCode: '',
        brandName: '',
        standard: ''
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
  flex-wrap: wrap;
  align-items: center;
  .label{
    margin: 0 8px 0 24px;
  }
}
</style>
