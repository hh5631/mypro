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
        label="读取状态"
        type="index"
        width="80"
      >
        <template #default="scope">
          <el-tag v-if="scope.row.readStatus===1">已读</el-tag>
          <el-tag v-else type="danger">未读</el-tag>
        </template>
      </el-table-column>

      <el-table-column
        label="业务类型"
        width="150"
        align="left"
        prop="bizType"
        :show-overflow-tooltip="true"
      >
        <template slot-scope="{row}">
          {{ dictionary.returnDictLabel(dict.type.sys_biz_type,row.bizType) }}
        </template>
      </el-table-column>
      <el-table-column
        label="发起时间"
        align="left"
        prop="receiveTime"
        width="200"
        :show-overflow-tooltip="true"
      />
      <el-table-column
        label="消息标题"
        align="left"
        prop="title"
        :show-overflow-tooltip="true"
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
            @click="toDetail(scope.row)"
          >查看</el-button>
          <el-button
            size="small"
            type="text"
            @click="toDelete(scope.row)"
          >删除</el-button>

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
      append-to-body
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
import { deleteMessage, putUpdateReadStatus } from '@/api/assetChange/index'
export default {
  dicts: ['sys_biz_type'],
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
      // 加载动画
      tableLoading: false,
      tagColor: '',
      tagType: '',
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

  methods: {

    getList() {
      // console.log('执行')
      this.$emit('getList', this.queryParams2)
    },
    getMsgNum(unread, read) {
      // 点击之后刷新数据
      // console.log('走555')
      this.$emit('getMsgNum', unread)
      this.$emit('getMsgNum', read)
    },

    toDetail(val) {
      putUpdateReadStatus({ ids: val.id }).then(res => {
        if (res.code === 200) {
          // console.log('zou444')
          this.getMsgNum(0, 1)
          this.getList()
        }
      })
      this.message = val
      this.dialogVisible = true
    },
    toDelete(val) {
      deleteMessage({ id: val.id }).then(res => {
        if (res.code === 200) {
          this.$message.success('删除成功')
          this.getMsgNum(0, 1)
          this.getList()
        } else {
          this.$message.error('删除失败')
        }
      })
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
