<template>
  <div class="my-table-box">
    <el-table
      ref="multipleTable"
      v-loading="tableLoading"
      :data="tableData"
      max-height="930"
      highlight-current-row
      @current-change="handleCurrentChange"
      @sort-change="sortChange"
    >

      <el-table-column
        label="序号"
        type="index"
      ><template #default="scope">
        <span>{{
          (queryParams.pageNum - 1) * queryParams.pageSize + scope.$index + 1
        }}</span>
      </template>
      </el-table-column>

      <el-table-column
        label="资产编号"
        align="left"
        prop="internalCode"
        width="180"
        :show-overflow-tooltip="true"
      />
      <el-table-column
        label="资产名称"
        align="left"
        prop="assetName"
        :show-overflow-tooltip="true"
      />
      <el-table-column
        label="折旧方法"
        align="left"
        prop="depreciationType"
        :show-overflow-tooltip="true"
      >
        <template slot-scope="{row}">
          {{ dictionary.returnDictLabel(dict.type.sys_depreciation_type,row.depreciationType) }}
        </template>
      </el-table-column>
      <el-table-column
        label="折旧日期"
        align="left"
        prop="startDate"
        :show-overflow-tooltip="true"
      />
      <el-table-column
        label="存放位置"
        align="left"
        prop="placeName"
        :show-overflow-tooltip="true"
      />
      <el-table-column
        label="购置单价(元)"
        align="left"
        prop="price"
        :show-overflow-tooltip="true"
        :sort-method="(a, b) => {return a.invoicePrice- b.invoicePrice}"
      />
      <el-table-column
        label="使用年限"
        align="left"
        prop="serviceLife"
        :show-overflow-tooltip="true"
      />
      <el-table-column
        label="购买日期"
        align="left"
        prop="purchaseDate"
        :show-overflow-tooltip="true"
      />
      <el-table-column
        label="残值率"
        align="left"
        prop="residualValueRate"
        :show-overflow-tooltip="true"
      >
        <template #default="scope">
          <span>{{ scope.row.residualValueRate }}%</span>
        </template>
      </el-table-column>
      <el-table-column
        label="预计净残值(元)"
        align="left"
        prop="expectedNetResidualValue"
        :show-overflow-tooltip="true"
      />
      <el-table-column
        label="已使用月份"
        align="left"
        prop="usedMonth"
        :show-overflow-tooltip="true"
      />
      <el-table-column
        label="本月折旧(元)"
        align="left"
        prop="monthDepreciationValue"
        :show-overflow-tooltip="true"
      />
      <el-table-column
        label="累计折旧(元)"
        align="left"
        prop="accumulatedDepreciation"
        :show-overflow-tooltip="true"
      />
      <el-table-column
        label="净值(元)"
        align="left"
        prop="netResidualValue"
        :show-overflow-tooltip="true"
      />
      <el-table-column
        label="备注"
        align="left"
        prop="remark"
        :show-overflow-tooltip="true"
      />
      <el-table-column
        label="操作"
        type="index"
        width="80"
      >
        <template #default="scope">
          <span class="detailDep" @click="depreciationDetail(scope)">
            详情
          </span>
        </template>
      </el-table-column>
    </el-table>
    <div class="common-pagination">
      <!-- <div class="chooseCount">已选{{ selectedLength }}条</div> -->
      <pagination
        v-show="total > 0"
        :total="total"
        :page.sync="queryParams.pageNum"
        :limit.sync="queryParams.pageSize"
        @pagination="getList"
      />
    </div>
    <el-dialog
      :title="detailTitle"
      :visible.sync="detailShow"
      width="800px"
      append-to-body
      :close-on-click-modal="false"
      class="dialogDetail"
    >
      <Detail ref="addInvoice" class="message" :id-key="idKey" />

    </el-dialog>
  </div>
</template>
<script>
import Detail from './childComponent/depreciationDetail.vue'

export default {
  components: { Detail },
  dicts: ['sys_depreciation_type'],
  props: {
    tableData: {
      type: Array,
      default: () => []
    },
    total: {
      type: Number,
      default: 0
    },
    queryParamsKey: {
      type: Object,
      default: () => { }
    }
  },
  data() {
    return {
      tableLoading: false,
      // total: 1,
      queryParams: {
        pageNum: 1,
        pageSize: 10
      },
      detailTitle: '',
      idKey: null,
      // 选中的数量
      selectedLength: 0,
      detailShow: false,
      currentRow: null,
      depreciationArray: ['平均年限法', '双倍余额递减法', '年数总和法']
    }
  },
  watch: {
    queryParamsKey (val) {
      this.queryParams.pageNum = val.pageNum
      this.queryParams.pageSize = val.pageSize
    } },
  methods: {

    handleCurrentChange(val) {
      this.currentRow = val
      this.$emit('select', val)
    },
    getList() {
      this.$emit('nextList', this.queryParams)
    },

    sortChange(column) {
      this.$emit('sortChange', column)
    },
    nextList(queryParams) {
      this.queryParams.pageNum = queryParams.pageNum
      this.queryParams.pageSize = queryParams.pageSize
      this.getList()
    }, depreciationDetail(val) {
      // 详情
      this.idKey = val.row.id
      this.detailShow = true
      this.detailTitle = '折旧历史'
    }
  }

}

</script>
<style lang="scss" scoped>
.dialogDetail{
  ::v-deep .el-dialog__body{
       padding: 0 16px 16px 16px;
      }

}
.my-table-box {
  display: flex;
  flex-direction: column;
  flex: 1;
  margin-top: 8px;
}
.detailDep{
  cursor: pointer;
  color:#4686ef;
}

</style>
