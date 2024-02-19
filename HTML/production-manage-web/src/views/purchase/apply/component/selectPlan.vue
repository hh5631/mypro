<template>
  <div class="dig-select">
    <div class="dig-left">
      <el-input v-model.trim="planKeyWords" maxlength="15" placeholder="请输入计划单编号或标题" size="mini" clearable @keyup.enter.native="searchPlan">
        <i slot="suffix" class="el-input__icon el-icon-search" @click="searchPlan" />
      </el-input>
      <div class="head-container">
        <el-table
          ref="titleTable"
          v-loading="loading"
          row-key="id"
          highlight-current-row
          :data="titleTable"
          style="margin-top: 10px;"
          max-height="430"
          @current-change="selectPlanChange"
        >
          <el-table-column key="userName" label="标题" align="center" prop="title" :show-overflow-tooltip="true" />
          <el-table-column key="orderNo" label="计划单编号" align="center" prop="orderNo" :show-overflow-tooltip="true" />
        </el-table>
      </div>
    </div>
    <div class="dig-right">
      <el-input v-model.trim="materialKeyWords" style="width:40%" maxlength="15" placeholder="请输入资产名称" size="mini" clearable @keyup.enter.native="searchMaterial">
        <i slot="suffix" class="el-input__icon el-icon-search" @click="searchMaterial" />
      </el-input>
      <el-table
        ref="assetTable"
        v-loading="loading"
        row-key="id"
        highlight-current-row
        :data="assetTable"
        style="margin-top: 10px;"
        max-height="600"
        @selection-change="handleSelectionChange"
      >
        <el-table-column type="selection" width="50" :reserve-selection="true" align="center" />
        <el-table-column type="index" width="100" label="序号" />
        <el-table-column key="materialName" label="资产名称" align="center" prop="materialName" :show-overflow-tooltip="true" />
        <el-table-column key="planQty" label="待采购数量" align="center" prop="planQty" />
        <!-- <el-table-column key="" label="已申购数量" align="center" prop="applyNum" /> -->
        <el-table-column key="supplierName" show-overflow-tooltip label="参考供应商" align="center" prop="supplierName" />
      </el-table>
    </div>
  </div>
</template>

<script>
import { getPlanSelector } from '@/api/purchase/index'
let assetTable = []
let titleTable = []
export default {
  props: {
    // tableData: {
    //   type: Array,
    //   default: () => []
    // },
    selectionData: {
      type: Array,
      default: () => []
    },
    delPlans: {
      type: Array,
      default: () => []
    },
    isDetail: {
      type: String,
      default: ''
    }
  },
  data() {
    return {
      planKeyWords: '',
      materialKeyWords: '',
      code: '',
      loading: false,
      // currentTitleRow: {},
      titleTable: [], // 左侧计划列表
      assetTable: [], // 右侧资产列表
      selectedRow: [],
      selectIds: []
    }
  },
  created() {
    // this.getList()
  },
  methods: {
    // 获取选中id
    getSelectId() {
      this.selectIds = []
      this.selectionData.forEach(item => {
        item.idArr.forEach(id => {
          this.selectIds.push(id)
        })
      })

      this.searchMaterial()
      this.getList()
    },
    getList() {
      this.titleTable = []
      this.assetTable = []
      getPlanSelector({}).then(res => {
        if (res.code === 200) {
          if (this.isDetail === '0') {
            const newData = this.assignDelPlans(this.$deepClone(res.data))
            titleTable = this.$deepClone(newData)
            this.titleTable = this.$deepClone(newData)
          } else {
            titleTable = this.$deepClone(res.data)
            this.titleTable = this.$deepClone(res.data)
          }
          this.titleTable.forEach(e => {
            e.details.forEach((i) => {
              i.orderNo = e.orderNo
              // this.$set(i, 'orderNo', e.orderNo)
            })
            e.details = e.details.filter((e) => { return !e.subscriptionFlag && !this.selectIds.includes(e.id) })
          })
          this.titleTable = this.titleTable.filter(e => { return e.details.length > 0 })
          if (this.titleTable.length > 0) {
            this.$refs.titleTable.setCurrentRow(this.titleTable[0])
            // 当前选中行id
            assetTable = titleTable[0].details.filter((e) => { return !e.subscriptionFlag && !this.selectIds.includes(e.id) })
          } else {
            assetTable = []
            this.assetTable = []
          }
        } else {
          titleTable = []
          this.titleTable = []
          assetTable = []
          this.assetTable = []
          this.$message.error(res.msg)
        }
      }).catch(() => {
        titleTable = []
        this.titleTable = []
        assetTable = []
        this.assetTable = []
      })
    },
    // 合并原有且已删除的计划
    assignDelPlans (data) {
      this.delPlans.forEach((delPlanItem) => {
        // 查找有没有对应计划
        const findPlanIndex = data.findIndex(item => item.id === delPlanItem.id)
        if (findPlanIndex === -1) {
          // 没有对应计划，则push
          data.push(this.$deepClone(delPlanItem))
        } else {
          // 有对应计划，则push到对应计划的资产列表
          delPlanItem.details.forEach(detailItem => {
            const findDetailIndex = data[findPlanIndex].details.findIndex(item => item.id === detailItem.id)
            if (findDetailIndex === -1) {
              // 没有，直接push
              data[findPlanIndex].details.push({ ...detailItem, subscriptionFlag: false })
            } else {
              // 有，重新设置；subscriptionFlag 是否已申购 true 已关联采购申请, 无法选择 | false 可以选
              data[findPlanIndex].details[findDetailIndex].subscriptionFlag = false
            }
          })
        }
      })
      return data
    },
    searchMaterial() {
      if (this.materialKeyWords === '') { // 如果搜索栏为空就执行getList方法获取最初始的列表
        this.assetTable = assetTable.filter((e) => { return !e.subscriptionFlag && !this.selectIds.includes(e.id) })
      } else {
      // 搜索
        this.assetTable = assetTable.filter(item => { return (item.materialName.toString().indexOf(this.materialKeyWords) >= 0 || item.materialId.toString().indexOf(this.materialKeyWords) >= 0) && !this.selectIds.includes(item.id) }
        )
      }
    },
    selectPlanChange(row) {
      assetTable = row.details
      // assetTable = row.details.filter((e) => { return !e.subscriptionFlag && !this.selectIds.includes(e.id) })
      this.searchMaterial()
      // this.currentTitleRow = row
      // this.activeId = row.id
    },

    searchPlan() {
      if (this.planKeyWords === '') { // 如果搜索栏为空就执行getList方法获取最初始的列表
        this.getList()
      } else {
      // 搜索
        this.titleTable = titleTable.filter(item => { return item.title.toString().indexOf(this.planKeyWords) >= 0 || item.orderNo.toString().indexOf(this.planKeyWords) >= 0 })
        if (this.titleTable.length > 0) {
          // 筛选左侧第一条的右侧
          this.assetTable = this.titleTable[0].details
        } else {
          this.assetTable = []
        }
      }
    },
    // 表格行选中时触发的回调函数
    handleSelectionChange(value) {
      this.selectedRow = [...value]
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
