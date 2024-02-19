<template>
  <div class="dig-select">
    <div class="dig-left">
      <el-input v-model.trim="planKeyWords" maxlength="15" placeholder="请输入业务单号" size="mini" @keyup.enter.native="searchPlan">
        <i slot="suffix" class="el-input__icon el-icon-search" @click="searchPlan" />
      </el-input>
      <div class="head-container">
        <el-table
          ref="titleTable"
          v-loading="loading"
          :row-key="getRowKeys"
          highlight-current-row
          :data="titleTable"
          style="margin-top: 10px;"
          max-height="600"
          @current-change="selectPlanChange"
        >
          <el-table-column key="orderNo" label="业务单号" align="center" prop="orderNo" :show-overflow-tooltip="true" />
        </el-table>
      </div>
    </div>
    <div class="dig-right">
      <!-- <el-input v-model.trim="materialKeyWords" style="width:40%" maxlength="15" placeholder="请输入资产名称" size="mini" @keyup.enter.native="searchMaterial">
        <i slot="suffix" class="el-input__icon el-icon-search" @click="searchMaterial" />
      </el-input> -->
      <el-table
        ref="assetTable"
        v-loading="loading"
        highlight-current-row
        :data="assetTable"
        style="margin-top: 10px;"
        max-height="600"
        @current-change="handleSelectionChange"
      >
        <!-- <el-table-column type="selection" width="50" :reserve-selection="true" align="center" /> -->
        <!-- <el-table-column type="index" width="100" label="序号" /> -->
        <el-table-column prop="materialCategory" label="物料分类" align="center" />
        <el-table-column prop="materialCode" label="物料编码" align="center" />
        <el-table-column prop="materialName" label="物料名称" align="center" />
        <el-table-column prop="brandName" label="品牌" align="center" />
        <el-table-column prop="materialStandard" label="规格型号" align="center" />
        <el-table-column prop="purchaseQty" label="计划入库数" align="center" />
        <el-table-column prop="unitName" label="计量单位" align="center" />
      </el-table>
    </div>
  </div>
</template>

<script>
// import { getApplyList, getApplyDetail } from '@/api/purchase/index'
import { getApplyList, getApplyDetail } from '@/api/warehouse/index'
export default {
  data() {
    return {
      planKeyWords: '',
      materialKeyWords: '',
      code: '',
      loading: false,
      currentTitleRow: {},
      titleTable: [],
      assetTable: [],
      selectedRow: [],
      currentData: ''
    }
  },
  // created() {
  //   this.getList()
  // },
  methods: {
    getList() {
      getApplyList().then(res => {
        if (res.code === 200) {
          this.titleTable = res.data
          this.$refs.titleTable.setCurrentRow(this.titleTable[0])
          this.selectPlanChange(this.titleTable[0])
        } else {
          this.$meessage.error(res.msg)
        }
      })
    },
    selectPlanChange(row) {
      this.currentTitleRow = row
      getApplyDetail(row.id).then(
        res => {
          if (res.code === 200) {
            //
            this.assetTable = res.data.detailVOS.map(item => {
              return {
                materialId: item.materialId,
                materialCategory: item.material.categoryNameLevel4 || item.material.categoryNameLevel3 || item.material.categoryNameLevel2 || item.material.categoryNameLevel1 || item.material.categoryName,
                materialCode: item.material.code,
                materialName: item.material.name,
                brandName: item.material.brand,
                materialStandard: item.material.standard,
                purchaseQty: item.purchaseQty,
                unitName: item.material.unit,
                planQty: '',
                qty: 0// 前端默认为0

              }
            })
            const obj = {
              row: this.currentTitleRow,
              tableArr: this.assetTable
            }
            // console.log(obj)
            this.currentData = JSON.stringify(obj)
            // this.$emit('selectChange', JSON.stringify(obj))
          }
        }
      )
    },
    getRowKeys(row) {
      return row.id
    },
    searchPlan() {
      if (this.planKeyWords === '') { // 如果搜索栏为空就执行getList方法获取最初始的列表
        this.getList()
      } else {
        // 搜索
        this.titleTable = this.titleTable.filter(item => { return item.orderNo.toString().indexOf(this.planKeyWords) >= 0 }

        )
      }
    },
    searchMaterial() {
      if (this.materialKeyWords === '') { // 如果搜索栏为空就执行getList方法获取最初始的列表
        this.assetTable = this.currentTitleRow.details
      } else {
        // 搜索
        this.assetTable = this.assetTable.filter(item => { return item.materialName.toString().indexOf(this.materialKeyWords) >= 0 || item.materialId.toString().indexOf(this.materialKeyWords) >= 0 }
        )
      }
    },
    // 表格行选中时触发的回调函数
    handleSelectionChange(value) {
      const obj = {
        row: this.currentTitleRow,
        tableArr: [value]
      }
      this.currentData = JSON.stringify(obj)
      // this.$emit('selectChange', value)
      this.selectedRow = value
    }
  }
}
</script>

<style lang="scss" scoped>
.dig-select {
  height: 500px;
  display: flex;
  .dig-left {
    width: 360px;
    height: 100%;
    padding: 16px;
    border-right: 1px solid #DDDDDD;
    overflow-y: scroll;
  }
  .dig-right {
    width: 840px;
    height: 100%;
    padding: 16px;
    overflow-y: scroll;
  }
}
</style>
