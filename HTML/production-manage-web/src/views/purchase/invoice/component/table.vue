<template>
  <div class="my-table-box">
    <el-table
      ref="multipleTable"
      v-loading="tableLoading"
      :data="tableData"
      height="100%"
      highlight-current-row
      @selection-change="handleSelectionChange"
      @sort-change="sortChange"
    >
      <el-table-column type="selection" align="center" />
      <el-table-column
        label="序号"
        type="index"
      ><template #default="scope">
        <span>{{
          (queryParams.pageNum - 1) * queryParams.pageSize + scope.$index + 1
        }}</span>
      </template></el-table-column>
      <el-table-column
        key="invoiceNo"
        label="1发票号码"
        align="left"
        prop="invoiceNo"
        :show-overflow-tooltip="true"
        sortable="custom"
      />
      <el-table-column
        key="invoiceAmount"
        label="发票金额(元)"
        align="left"
        prop="invoiceAmount"
        :show-overflow-tooltip="true"
        sortable="custom"
        :sort-method="(a, b) => {return a.invoicePrice- b.invoicePrice}"
      />
      <el-table-column
        key="invoiceTaxAmount"
        label="发票税额(元)"
        align="left"
        prop="invoiceTaxAmount"
        :show-overflow-tooltip="true"
      />
      <el-table-column
        key="billingDate"
        label="开票日期"
        align="left"
        prop="billingDate"
        :show-overflow-tooltip="true"
        sortable="custom"
      />
      <el-table-column
        key="invoiceType"
        label="发票类型"
        align="left"
        prop="invoiceType"
        :show-overflow-tooltip="true"
      >
        <template slot-scope="scope">
          <div>
            <span v-if="scope.row.invoiceType==='0'">增值税专用发票</span>
            <span v-else>增值税普通发票</span>
          </div>
        </template>
      </el-table-column>
      <el-table-column
        key="supplierName"
        label="供应商"
        align="left"
        prop="supplierName"
        :show-overflow-tooltip="true"
      />
      <el-table-column
        key="contractAmount"
        label="合同金额（元）"
        align="left"
        prop="contractAmount"
        :show-overflow-tooltip="true"
      />
      <el-table-column
        key="receivedDate"
        label="收到日期"
        align="left"
        prop="receivedDate"
        :show-overflow-tooltip="true"
        sortable="custom"
      />
      <el-table-column
        label="操作"
        align="left"
        width="180"
        class-name="small-padding"
      >
        <template slot-scope="scope">
          <el-button
            size="small"
            type="text"
            @click="update(scope.row, 0)"
          >详情</el-button>
          <el-button
            size="small"
            type="text"
            @click="update(scope.row, 1)"
          >编辑</el-button>
          <el-button
            size="small"
            type="text"
            style="color: #ef5826"
            @click="del(scope.row)"
          >删除</el-button>
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
  </div>
</template>
<script>
export default {
  props: {
    tableData: {
      type: Array,
      default: () => []
    },
    total: {
      type: Number,
      default: 0
    }
  },
  data() {
    return {

      tableLoading: false,
      // total: 1,
      queryParams: {
        pageNum: 0,
        pageSize: 10
      },
      // 选中的数量
      selectedLength: 0
    }
  },
  methods: {
    // 多选框选中数据
    handleSelectionChange(selection) {
      const ids = []
      selection.forEach(item => {
        ids.push(item.id)
      })
      this.selectedLength = selection.length
      this.$emit('select', ids)
    },
    getList() {
      this.$emit('nextList', this.queryParams)
    },
    // 编辑或者查看详情
    update(row, index) {
      this.$emit('update', row, index)
    },
    del(row) {
      this.$emit('delete', row)
    },
    sortChange(column) {
      this.$emit('sortChange', column)
    }
  }

}

</script>
<style lang="scss" scoped>
.my-table-box {
  display: flex;
  flex-direction: column;
  flex: 1;
  margin-top: 8px;
}

</style>
