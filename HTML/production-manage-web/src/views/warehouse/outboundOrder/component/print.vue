<!--
 * @Descripttion:
 * @version:
 * @Author: lirioing
 * @Date: 2022-07-04 14:54:09
 * @LastEditors: liquan 1106970756@qq.com
 * @LastEditTime: 2022-09-05 16:52:00
-->
<template>
  <div class="selectLeader">
    <div class="search">
      <!-- <div>资产选择</div> -->
      <el-button
        type="primary"
        size="mini"
        @click="handlePrint"
      ><i class="iconfont icon-dayin" style="color: #ffffff" />打印标签</el-button>
    </div>

    <el-table
      ref="table"
      highlight-current-row
      :data="assetsList"
      style="margin-top: 10px;"
      max-height="500"
      :row-key="getRowKeys"
    >
      <el-table-column key="internalCode" label="资产编码" align="center" prop="internalCode" :show-overflow-tooltip="true" />
      <el-table-column key="name" label="资产名称" align="center" prop="name" :show-overflow-tooltip="true" />
      <el-table-column key="brandName" label="品牌" align="center" prop="brandName" :show-overflow-tooltip="true" />
      <el-table-column key="standard" label="规格型号" align="center" prop="standard" :show-overflow-tooltip="true" />
      <el-table-column key="unit" label="计量单位" align="center" prop="unit" :show-overflow-tooltip="true" />
      <el-table-column label="操作" align="center" prop="status" :show-overflow-tooltip="true">
        <template slot-scope="scope">
          <div>
            <el-button type="primary" plain @click="bindCard(scope.row)">绑卡</el-button>
          </div>
        </template>
      </el-table-column>
    </el-table>
  </div>
</template>

<script>
import { printLabelPromise } from '@/views/assetmanagement/labelPrint/print.js'
import { initialPrintData, handlePrintData } from '@/utils/jri.js'
import { bindCardFun } from '@/views/assetmanagement/labelPrint/bindCard.js'
export default {
  props: {
    assetsList: {
      type: Array,
      default: () => []
    }
  },
  data() {
    return {
      title: '',
      addApplyDig: false,
      selectedData: [], // 选中的数据
      queryParams: {
        pageNum: 0,
        pageSize: 1000,
        warehouseId: '',
        materialName: ''
      },
      warehouseData: [],
      printData: initialPrintData
    }
  },
  created() {
    // this.getAssetByOutIdAndMaterialId()
  },
  methods: {
    handleQuery() {
      this.queryParams.pageNum = 1
      this.getList()
    },
    resetQuery() {
      this.queryParams = {
        pageNum: 0,
        pageSize: 1000,
        warehouseId: '',
        materialName: ''
      }
      this.handleQuery()
    },
    handleSelectionChange(val) {
      this.selectedData = val
    },
    getRowKeys(row) {
      return row.id + Math.random()
    },
    // 打印
    async handlePrint() {
      this.printData.labels = handlePrintData(this.assetsList)
      // console.log(this.printData, ' printArr')
      // 打印
      await printLabelPromise(this.printData)
    },

    // 绑卡
    async bindCard(row) {
      bindCardFun(this, row, this.getList)
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
  justify-content: flex-end;
  align-items: center;
}
</style>
