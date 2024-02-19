<template>
  <div class="selectLeader">
    <div class="selectTop">
      <h3>资产明细</h3>
      <el-button
        v-hasPermi="['warehousem:warehousingOrder:printLabel']"
        type="primary"
        size="small"
        @click="handlePrint"
      >
        <i
          class="iconfont icon-dayin"
          style="font-size: 14px; margin-right: 8px"
        />
        <span class="btn-text">打印标签</span>
      </el-button>
    </div>
    <el-table
      ref="asstesData"
      style="width: 100%"
      border
      :data="tableFormData"
      highlight-current-row
      max-height="600"
    >
      <el-table-column
        key="internalCode"
        label="资产编码"
        align="center"
        prop="internalCode"
        :show-overflow-tooltip="true"
      />
      <el-table-column
        key="name"
        label="资产名称"
        align="center"
        prop="name"
        :show-overflow-tooltip="true"
      />
      <el-table-column
        key="brandName"
        label="品牌"
        align="center"
        prop="brandName"
        :show-overflow-tooltip="true"
      />
      <el-table-column
        key="standard"
        label="规格型号"
        align="center"
        prop="standard"
        :show-overflow-tooltip="true"
      />
      <el-table-column
        label="操作"
        align="center"
      >
        <template slot-scope="scope">
          <span>
            <el-button
              v-hasPermi="['warehousem:warehousingOrder:bindCard']"
              type="text"
              size="small"
              @click="bindCard(scope.row)"
            >绑卡</el-button>
          </span>
        </template>
      </el-table-column>
    </el-table>
  </div>
</template>
<script>
import { bindCardFun } from '@/views/assetmanagement/labelPrint/bindCard.js'
import { printLabelPromise } from '@/views/assetmanagement/labelPrint/print.js'
import { initialPrintData, handlePrintData } from '@/utils/jri.js'
export default {
  props: {
    bindCardData: {
      type: Array,
      default: () => []
    }
  },
  data() {
    return {
      printData: initialPrintData
    }
  },
  computed: {
    tableFormData () {
      return this.bindCardData
    }
  },
  created() {
  },
  methods: {
    // 绑卡
    async bindCard(row) {
      bindCardFun(this, row)
    },
    /** 打印标签 */
    async handlePrint() {
      this.printData.labels = handlePrintData(this.tableFormData)

      // 打印
      await printLabelPromise(this.printData)
    }
  }
}
</script>

<style lang="scss" scoped>
.selectLeader {
  width: 100%;
  height: 100%;
  padding: 16px;
  overflow-y: scroll;
  .selectTop {
    display: flex;
    justify-content: space-between;
    align-items: center;

  }
}
.search {
  display: flex;
  align-items: center;
  .label {
    margin: 0 8px 0 24px;
  }
}
.detail-title {
  font-size: 16px;
  font-weight: 500;
  color: #6a7697;
  margin-bottom: 15px;
  &.sub-title{
    margin-top: 15px;
  }
}
.noborder {
  ::v-deep .el-input__inner {
    border: none;
  }
}
::v-deep .el-table th.must>.cell:after {
	content: ' *';
	color: #ff1818;
}
</style>
