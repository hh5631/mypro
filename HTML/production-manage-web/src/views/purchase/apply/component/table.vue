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
      <!-- <el-table-column
        v-if="columnShow('计划编号')"
        key="orderNo"
        label="计划编号"
        align="left"
        prop="orderNo"
        :show-overflow-tooltip="true"
      /> -->
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
        key="updateTime"
        label="申请时间"
        align="left"
        prop="updateTime"
        :show-overflow-tooltip="true"
      />
      <!-- <el-table-column
        v-if="columnShow('状态')"
        key="statusComplete"
        label="状态"
        align="left"
        prop="status"
        :show-overflow-tooltip="true"
      >
        <template slot-scope="scope">
          <el-tag :color="getTagColor(scope.row.status)" :type="getTagType(scope.row.status)">
            {{ getStatus(scope.row.status) }}
          </el-tag>
        </template>
      </el-table-column> -->
      <el-table-column
        v-if="columnShow('状态')"
        key="status"
        sortable="custom"
        label="状态"
        align="left"
        prop="status"
        :show-overflow-tooltip="true"
      >
        <template slot-scope="scope">
          <el-tag :type="getTagType1(scope.row.status)" style="width: 64px;">
            {{ getStatus1(scope.row.status) }}
          </el-tag>
        </template>
      </el-table-column>
      <el-table-column
        label="操作"
        align="left"
        class-name="small-padding"
      >
        <template slot-scope="scope">
          <el-button
            v-if="scope.row.status!=='0'"
            size="small"
            type="text"
            @click="handleUpdate(scope.row, 0)"
          >详情</el-button>
          <el-button
            v-if="scope.row.status==='0'"
            size="small"
            type="text"
            @click="handleUpdate(scope.row, 1)"
          >编辑</el-button>
          <!--          <el-button-->
          <!--            v-if="scope.row.status==='4'"-->
          <!--            size="small"-->
          <!--            type="text"-->
          <!--            @click="handleDelete(scope.row)"-->
          <!--          >废弃</el-button>-->
          <el-button
            v-if="scope.row.status==='1'&&scope.row.createBy===userName"
            size="small"
            type="text"
            @click="handleRecall(scope.row)"
          >撤回</el-button>
          <el-button
            v-if="scope.row.status==='0'"
            size="small"
            type="text"
            style="color: #ef5826"
            @click="handleDel(scope.row)"
          >删除</el-button>
        </template>
      </el-table-column>
    </el-table>
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
// 采购申请主页面
import { recallApply, deleteApply } from '@/api/purchase/index'
export default {
  props: {
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
      tableLoading: false,
      tagColor: '',
      tagType: '',
      markNum: 0,
      userName: this.$store.state.user.name
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
          default:
            return '不通过' // status == 4 || status == 99
        }
      }
    }
  },
  watch: {
    markKey(val) {
      this.markNum = Number(val)
      //
    },
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
        return '#C2C4D1'
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
          return '通过'
        case '4':
          return '不通过' // status == 4
        case '5':
          return '已领用'
        default:
          return '不通过' // status == 99
      }
    },
    handleSelectionChange(selection) {
      this.$emit('handleSelectionChange', selection)
    },
    columnShow(val) {
      return this.checkedColum.includes(val)
    },
    getList() {
      this.$emit('getList')
    },
    handleUpdate(row, index) {
      if (index === 1) {
        //  编辑
        //
        this.$router.push({ path: '/applyDetail', query: { id: row.id, isDetail: 0, mark: this.markNum }})
      } else {
        //  详情
        //
        this.$router.push({ path: '/applyDetail', query: { id: row.id, isDetail: 1, mark: this.markNum }})
      }
    },
    // handleDelete(row) {
    //   const planId = row.id
    //   this.$modal
    //     .confirm('确定废弃此单据？', '确定废弃')
    //     .then(function() {
    //       return delApply(planId)
    //     })
    //     .then(() => {
    //       this.$modal.msgSuccess('废弃成功')
    //       this.$emit('deleteSuccess', true)
    //     })
    //     .catch(() => {})
    // },
    handleRecall(row) {
      const planId = row.id
      this.$modal
        .confirm('确定撤回此单据？', '确定撤回')
        .then(function() {
          return recallApply(planId)
        })
        .then(() => {
          this.$modal.msgSuccess('撤回成功')
          this.$emit('deleteSuccess', true)
        })
        .catch(() => {})
    },
    handleDel(row) {
      const planId = row.id
      this.$modal
        .confirm('确定删除此草稿？', '确定删除')
        .then(function() {
          return deleteApply(planId)
        })
        .then(() => {
          this.$modal.msgSuccess('删除成功')
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
.chooseCount {
  color: #7d8592;
  margin-top: 20px;
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
