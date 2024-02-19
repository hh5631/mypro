<template>
  <div class="my-table-box">
    <!-- table -->
    <el-table
      ref="multipleTable"
      v-loading="tableLoading"
      :data="tableDatas"
      height="100%"
      size="mini"
      highlight-current-row
      @sort-change="sortChange"
    >
      <el-table-column
        label="序号"
        type="index"
      >
        <template #default="scope">
          <span>{{
            (queryParams.pageNum - 1) * queryParams.pageSize + scope.$index + 1
          }}</span>
        </template>
      </el-table-column>
      <el-table-column
        label="折旧方法"
        align="left"
        prop="depreciationType"
        :show-overflow-tooltip="true"
      >
        <template slot-scope="{row}">
          {{ depreciationArray[row.depreciationType] }}
        </template>
      </el-table-column>
      <el-table-column
        label="使用年限"
        align="left"
        prop="serviceLife"
        :show-overflow-tooltip="true"
      />
      <el-table-column
        label="折旧金额（元）"
        align="left"
        prop="depreciationValue"
        :show-overflow-tooltip="true"
      />
      <el-table-column
        label="折旧日期"
        align="left"
        prop="depreciationDate"
        :show-overflow-tooltip="true"
      />
      <el-table-column
        label="净值（元）"
        align="left"
        prop="netResidualValue"
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
        label="折旧率"
        align="left"
        prop="depreciationRate"
        :show-overflow-tooltip="true"
      >
        <template #default="scope">
          <span>{{ scope.row.depreciationRate }}%</span>
        </template>
      </el-table-column>
    </el-table>

    <!-- 分页 -->
    <div class="common-pagination">
      <!-- <div class="chooseCount">已选1条</div> -->
      <pagination
        v-show="total > 0"
        :total="total"
        :page.sync="queryParams2.pageNum"
        :limit.sync="queryParams2.pageSize"
        @pagination="getList"
      />
    </div>
    <el-dialog
      title="消息详情"
      :visible.sync="dialogVisible"
      width="60%"
      :modal="false"
      :before-close="handleClose"
    >
      <div>
        <div class="msgRow">
          <span class="msgTitle">标题:</span>
          <span>{{ message.title }}</span>
        </div>
        <div class="msgRow">
          <span class="msgTitle">发送时间:</span>
          <span>{{ message.receiveTime }}</span>
        </div>
        <div class="msgRow">
          <span class="msgTitle">消息内容:</span>
          <span>{{ message.content }}</span>
        </div>
      </div>

    </el-dialog>
  </div>
</template>
<script>
export default {
  props: {
    // 接收数据
    tableDatas: {
      type: Array,
      default: () => []
    },
    total: {
      type: Number,
      default: 0
    },
    type: {
      type: Number,
      default: 1
    },
    queryParams: {
      type: Object,
      default: () => { }
    }

  },
  data() {
    return {
      tableData: [],
      tableLoading: false,
      depreciationArray: ['平均年限法', '双倍余额递减法', '年数总和法'],
      dialogVisible: false,
      message: {},

      queryParams2: {
        pageNum: 0,
        pageSize: 10
      }
    }
  },

  watch: {
    // 监听不直接使用父级数据
    tableDatas(val) {
      this.tableData = val
      // this.total = val.length
    }

  },
  created() {
    // this.getList()
  },
  methods: {
    getList() {
      this.$emit('nextList', this.queryParams2)
    },

    handleClose(done) {
      this.dialogVisible = false
    },

    sortChange(column) {
      this.$emit('sortChange', column)
    }
  }
}
</script>
<style lang="scss" scoped>
  ::v-deep .el-dialog__body{
        border: 1px solid red;
      }
.my-table-box {
  display: flex;
  flex-direction: column;
  flex: 1;
  margin-top: 8px;
  z-index: 999;
}
.el-tag{
  // width: 72px;
  text-align: center;
  font-size: 14px;
}
::v-deep .el-tag.el-tag--info{
  color: #7D8592;
}
::v-deep .el-tag.el-tag--danger{
  color:#EF5826;
}
::v-deep .el-tag{
  color: #4686EF;
}
::v-deep .el-tag.el-tag--success{
  color: #3DB954;
}

.msgRow{
  margin-bottom: 20px;
  .msgTitle{
  margin-right: 12px;
}
}

</style>
