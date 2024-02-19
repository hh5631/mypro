<template>
  <div class="my-table-box">
    <!-- table -->
    <el-table
      ref="multipleTable"
      v-loading="tableLoading"
      :data="tableData"
      height="100%"
      size="mini"
      highlight-current-row
      @selection-change="handleSelectionChange"
      @sort-change="sortChange"
    >
      <el-table-column type="selection" align="center" />
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
        v-if="columnShow('计划编号')"
        key="orderNo"
        label="计划编号"
        align="left"
        prop="orderNo"
        :show-overflow-tooltip="true"
      />
      <el-table-column
        v-if="columnShow('采购单编号')"
        key="orderNo"
        label="采购单编号"
        align="left"
        prop="orderNo"
        :show-overflow-tooltip="true"
      />
      <el-table-column
        v-if="columnShow('标题')"
        key="title"
        label="标题"
        align="left"
        prop="title"
        :show-overflow-tooltip="true"
      />
      <el-table-column
        v-if="columnShow('预期完成时间')"
        key="expectedCompletionDate"
        label="预期完成时间"
        align="left"
        prop="expectedCompletionDate"
        :show-overflow-tooltip="true"
      />
      <el-table-column
        v-if="columnShow('资产名称')"
        key="assetName"
        label="资产名称"
        align="left"
        prop="assetName"
        :show-overflow-tooltip="true"
      />
      <el-table-column
        v-if="columnShow('创建人')"
        key="creator"
        label="创建人"
        align="left"
        prop="creator"
        :show-overflow-tooltip="true"
      />
      <el-table-column
        v-if="columnShow('采购负责人')"
        key="procurementLeaderName"
        label="采购负责人"
        align="left"
        prop="procurementLeaderName"
        :show-overflow-tooltip="true"
      />
      <el-table-column
        v-if="columnShow('创建时间')"
        key="createTime"
        label="创建时间"
        align="left"
        prop="createTime"
        :show-overflow-tooltip="true"
      />
      <el-table-column
        v-if="columnShow('申请时间')"
        key="createTime"
        label="申请时间"
        align="left"
        prop="createTime"
        :show-overflow-tooltip="true"
      />
      <el-table-column
        v-if="columnShow('状态')"
        key="status"
        sortable
        label="状态"
        align="left"
        prop="status"
        :show-overflow-tooltip="true"
      >
        <template slot-scope="scope">
          <!-- <el-tag :color="getTagColor(scope.row.status)" :type="getTagType(scope.row.status)">
            {{ getStatus(scope.row.status) }}
          </el-tag> -->
          <el-tag :type="getTagType(scope.row.status)" style="display: flex;justify-content: center; align-items: center;width: 72px;">
            {{ getStatus(scope.row.status) }}
          </el-tag>
        </template>
      </el-table-column>
      <el-table-column
        v-if="columnShow('状态1')"
        key="status"
        sortable="custom"
        label="状态"
        align="left"
        prop="status"
        :show-overflow-tooltip="true"
      >
        <template slot-scope="scope">
          <el-tag :color="getTagColor1(scope.row.status)" :type="getTagType1(scope.row.status)">
            {{ getStatus1(scope.row.status) }}
          </el-tag>
        </template>
      </el-table-column>
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
            @click="handleUpdate(scope.row, 0)"
          >详情</el-button>
          <el-button
            v-if="scope.row.status==='0'&&type === 1"
            size="small"
            type="text"
            @click="handleUpdate(scope.row, 1)"
          >编辑</el-button>
          <el-button
            v-if="scope.row.status==='0'&&type === 1"
            size="small"
            type="text"
            @click="handleDelete(scope.row)"
          >废弃</el-button>
          <el-button
            v-if="scope.row.status==='3'&&type === 2"
            size="small"
            type="text"
            style="color: #ef5826"
            @click="handleDelete(scope.row)"
          >撤回</el-button>
        </template>
      </el-table-column>
    </el-table>
    <!-- 分页 -->
    <div class="common-pagination">
      <!-- <div class="chooseCount">已选1条</div> -->
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
import { delPlan } from '@/api/purchase/index'
export default {
  props: {
    // 接收数据
    tableDatas: {
      type: Array,
      default: () => []
    },
    checkedColum: {
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
    },
    markKey: {
      type: Number,
      default: 0
    }
  },
  data() {
    return {
      tableData: [],
      // 加载动画
      tableLoading: false,
      tagColor: '',
      tagType: ''
    }
  },
  computed: {
    getStatus() {
      return (status) => {
        switch (status) {
          case '0':
            return '待执行'
          case '1':
            return '部分完成'
          case '2':
            return '全部完成'
          case '3':
            return '执行中'
          case '99':
            return '废弃'
          default:
            return ''
        }
      }
    }
  },
  watch: {
    markKey(val) {
      this.markNum = Number(val)
      //
    },
    // 监听不直接使用父级数据
    tableDatas(val) {
      this.tableData = val
      // this.total = val.length
    },
    '$route.query.mark': {
      handler(val) {
        // 监听路由改变值
        if (val) {
          this.markNum = Number(val)
        }
      },
      immediate: true
    }
  },
  created() {
    // this.getList()
  },
  methods: {
    /** tag类型 */
    getTagType(status) {
      if (status === '0') {
        return 'info'
      } else if (status === '1') {
        return ''
      } else if (status === '2') {
        return 'success'
      } else {
        return 'danger'
      }
    },
    getTagType1(status) {
      if (status === '0' || status === '1') {
        return 'info'
      } else if (status === '2') {
        return ''
      } else if (status === '3') {
        return 'success'
      } else if (status === '4' || status === '99') {
        return 'danger'
      }
    },
    /** el-tag颜色 */
    getTagColor(status) {
      if (status === '0') {
        this.tagType = 'info'
        return 'rgba(194, 196, 209, 0.5)'
      } else if (status === '1') {
        this.tagType = ''
        return 'rgba(70, 134, 239, 0.5)'
      } else if (status === '2') {
        this.tagType = 'success'
        return 'rgba(61, 185, 84, 0.5)'
      } else {
        this.tagType = 'danger'
        return 'rgba(239, 88, 38, 0.5)'
      }
    },
    getTagColor1(status) {
      if (status === '0' || status === '1') {
        this.tagType = 'info'
        return 'rgba(194, 196, 209, 0.5)'
      } else if (status === '2') {
        this.tagType = ''
        return 'rgba(70, 134, 239, 0.5)'
      } else if (status === '3') {
        this.tagType = 'success'
        return 'rgba(61, 185, 84, 0.5)'
      } else if (status === '4' || status === '99') {
        this.tagType = 'danger'
        return 'rgba(239, 88, 38, 0.5)'
      }
    },
    getStatus1(status) {
      switch (status) {
        case '0':
          return '草稿'
        case '1':
          return '待审批'
        case '2':
          return '审批中'
        case '3':
          return '审批通过'
        case '4':
          return '审批不通过'
        default:
          return '废弃'
      }
    },
    /** 多选点击 */
    handleSelectionChange() {},
    /** 显示表头标题 */
    columnShow(val) {
      return this.checkedColum.includes(val)
    },
    getList() {
      this.$emit('getList')
    },
    /** 详情和编辑 */
    handleUpdate(row, index) {
      if (index === 1) {
        //  编辑
        this.$router.push({ path: '/editPlan', query: { id: row.id, isDetail: 0, mark: this.markNum }})
      } else {
        //  详情
        this.$router.push({ path: '/purchase/plan/planDetail', query: { id: row.id, isDetail: 1, mark: this.markNum }})
      }
    },
    /** 废弃单据 */
    handleDelete(row) {
      const planId = row.id
      this.$modal
        .confirm('确定废弃此单据？', '确定废弃')
        .then(function() {
          return delPlan(planId)
        })
        .then(() => {
          this.$modal.msgSuccess('废弃成功')
          this.$emit('deleteSuccess', true)
        })
        .catch(() => {})
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
</style>
