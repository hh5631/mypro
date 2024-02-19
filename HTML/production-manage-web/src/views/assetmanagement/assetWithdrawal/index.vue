<template>
  <div class="post-box">
    <!-- 标题栏 -->
    <tags-view>
      <el-row :gutter="10" type="flex">
        <!-- 打印 -->
        <el-col :span="1.5">
          <el-button
            v-hasPermi="['assetManagement:assetWithdrawal:print']"
            size="small"
            @click="handlePrint"
          >
            <i class="iconfont icon-dayin" />
            <span class="btn-text">打印</span>
          </el-button>
        </el-col>
        <!-- 导出 -->
        <el-col :span="1.5">
          <el-button
            v-hasPermi="['assetManagement:assetWithdrawal:export']"
            size="small"
            @click="handleExport"
          >
            <i class="iconfont icon-daochu" />
            <span class="btn-text">导出</span>
          </el-button>
        </el-col>
        <!-- 新增 -->
        <el-col :span="1.5">
          <el-button
            v-hasPermi="['assetManagement:assetWithdrawal:addOrUpdate']"
            type="primary"
            size="small"
            @click="handleAdd"
          >
            <i
              class="iconfont icon-zengjia"
              style="font-size: 14px; margin-right: 8px"
            />
            <span class="btn-text">新建退库</span>
          </el-button>
        </el-col>
        <!-- 刷新 -->
        <el-col :span="1.5">
          <el-button
            v-hasPermi="['system:post:add']"
            size="small"
            @click="refresh"
          >
            <el-tooltip
              class="item"
              effect="dark"
              content="刷新"
              placement="top-start"
            >
              <i class="iconfont icon-shuaxin no-margin" />
            </el-tooltip>
          </el-button>
        </el-col>
      </el-row>
    </tags-view>
    <!-- content -->
    <el-card class="main-card">
      <!-- 筛选栏 -->
      <el-form ref="queryForm" :model="queryParams" :inline="true">
        <el-form-item label="退库单号" prop="orderNo">
          <el-input
            v-model.trim="queryParams.orderNo"
            placeholder="请输入"
            clearable
            maxlength="15"
            size="small"
            style="width: 180px"
            @keyup.enter.native="handleQuery"
          />
        </el-form-item>
        <el-form-item label="退库标题" prop="title">
          <el-input
            v-model.trim="queryParams.title"
            placeholder="请输入"
            clearable
            maxlength="15"
            size="small"
            style="width: 180px"
            @keyup.enter.native="handleQuery"
          />
        </el-form-item>
        <el-form-item label="退库人" prop="peopleName">
          <el-input
            v-model.trim="queryParams.peopleName"
            placeholder="请输入"
            clearable
            maxlength="15"
            size="small"
            style="width: 180px"
            @keyup.enter.native="handleQuery"
          />
        </el-form-item>
        <el-form-item label="状态" prop="status">
          <el-select
            v-model="queryParams.status"
            style="width: 150px"
            placeholder="请选择"
            clearable
            size="small"
          >
            <el-option
              v-for="dict in dict.type.asset_withdrawal_status"
              :key="dict.value"
              :label="dict.label"
              :value="dict.value"
            />
          </el-select>
        </el-form-item>
        <el-form-item label="创建时间">
          <div class="date_box">
            <el-date-picker
              v-model="getTime"
              size="small"
              style="width: 200px"
              value-format="yyyy-MM-dd"
              type="daterange"
              range-separator="至"
              start-placeholder="开始日期"
              end-placeholder="结束日期"
              :clearable="false"
            /><i class="el-icon-date data_icon" />
          </div>
        </el-form-item>
        <el-form-item>
          <el-button size="small" @click="resetQuery">重置</el-button>
          <el-button
            v-hasPermi="['assetManagement:assetWithdrawal:query']"
            type="primary"
            plain
            size="small"
            class="search-btn"
            @click="handleQuery"
          >搜索</el-button>
        </el-form-item>
      </el-form>
      <!-- 主内容展示table -->
      <el-table
        ref="assetTable"
        v-loading="loading"
        class="asset-collection-table"
        highlight-current-row
        :data="assetListData"
        height="100%"
        @selection-change="handleSelectionChange"
      >
        <el-table-column type="selection" width="45" align="center" />
        <!-- <el-table-column
          label="序号"
          type="index"
          width="55"
          align="center"
        ><template #default="scope">
          <span>{{
            (queryParams.pageNum - 1) * queryParams.pageSize +
              scope.$index +
              1
          }}</span>
        </template></el-table-column> -->
        <el-table-column label="序号" sortable align="left" prop="index" type="index" />
        <el-table-column label="退库单号" align="center" prop="orderNo" />
        <el-table-column label="退库标题" show-overflow-tooltip align="center" prop="title" />
        <el-table-column
          label="创建时间"
          align="center"
          prop="createTime"
          :show-overflow-tooltip="true"
        >
          <template slot-scope="scope">
            <span>{{ dayjsFormat(scope.row.createTime) }}</span>
          </template>
        </el-table-column>
        <el-table-column label="接收部门" align="center" prop="backDept" :show-overflow-tooltip="true" />
        <el-table-column label="退库人" align="center" prop="userName" :show-overflow-tooltip="true" />
        <el-table-column label="状态" align="center" prop="status">
          <template slot-scope="scope">
            <el-tag :type="getStatueType(scope.row.status)" class="status-tag">
              {{ getStatusName(scope.row.status) }}
            </el-tag>
          </template>
        </el-table-column>
        <el-table-column
          label="操作"
          width="200"
          align="center"
          class-name="small-padding"
        >
          <template slot-scope="scope">
            <el-button
              v-hasPermi="['assetManagement:assetWithdrawal:info']"
              size="small"
              type="text"
              style="color: #4686ef"
              @click="handleDetail(scope.row)"
            >详情</el-button>
            <el-button
              v-if="scope.row.status === '0'"
              v-hasPermi="['assetManagement:assetWithdrawal:addOrUpdate']"
              size="small"
              type="text"
              style="color: #4686ef"
              @click="handleEdit(scope.row)"
            >编辑</el-button>
            <el-button
              v-if="scope.row.status === '0'"
              v-hasPermi="['assetManagement:assetWithdrawal:delete']"
              size="small"
              type="text"
              style="color: #ef5826"
              @click="handleDelete(scope.row)"
            >删除</el-button>
          </template>
        </el-table-column>
      </el-table>
      <!-- 分页 -->
      <div class="common-pagination">
        <pagination
          v-show="total > 0"
          :total="total"
          :page.sync="queryParams.pageNum"
          :limit.sync="queryParams.pageSize"
          @pagination="getList"
        />
      </div>
    </el-card>
    <!-- 新增/编辑弹窗 -->
    <el-dialog
      destroy-on-close
      :title="title"
      :visible.sync="open"
      width="1200px"
      append-to-body
      class="addDig"
      :close-on-click-modal="false"
      @close="closeDialog"
    >
      <addDialog
        ref="addDialog"
        :is-edit="isEdit"
        :is-disabled="isDisabled"
        :add-time-now="addTimeNow"
        :warehouse-data="warehouseData"
        :dept-data="deptData"
        :data="addDialogData"
        @successwork="successwork"
        @failwork="failwork"
      />
      <!-- footer -->
      <div slot="footer" class="dialog-footer">
        <div>
          <span
            v-if="isDetail"
            :style="getStyle(rowInfo.status)"
          ><i class="iconfont icon-xiangqingxinxi" />
            {{ getStatusName(rowInfo.status) }}
          </span>
          <span v-else />
        </div>
        <div>
          <el-button size="small" @click="closeDialog">取消</el-button>
          <el-button
            v-if="isEdit"
            size="small"
            @click="save(0)"
          >存草稿</el-button>
          <el-button
            v-if="isEdit"
            size="small"
            type="primary"
            @click="save(1)"
          >保存</el-button>
        </div>
      </div>
    </el-dialog>
    <!-- 打印预览 -->
    <FilePrint ref="FilePrint" type="assetWithdrawal" />
  </div>
</template>
<script>
import TagsView from '@/layout/components/TagsView/index.vue'
import { getWareBackByCondition, getWareBackDetailsById, deleteWareBack } from '@/api/assetmanagement/assetWithdrawal.js'
import { listWarehouse } from '@/api/basicdata/warehouse'
import { listDept } from '@/api/system/dept'
import { dayjsFormat } from '@/utils/index'
import addDialog from './components/addDialog.vue'
import FilePrint from '@/components/FilePrint'
export default {
  name: 'AssetWithdrawal',
  components: { TagsView, addDialog, FilePrint },
  dicts: ['asset_withdrawal_status'],
  data () {
    return {
      // 筛选栏表单数据
      queryParams: {
        orderNo: '',
        pageNum: 0,
        pageSize: 10,
        peopleName: '',
        title: '',
        status: null,
        startTime: null,
        endTime: null
      },
      getTime: [],
      // 主内容表格加载
      loading: false,
      // 主内容表格数据
      assetListData: [],
      // 分页
      total: 0,
      // 弹出框标题
      title: '',
      open: false,
      isEdit: false,
      isDetail: false,
      isDisabled: false,
      addDialogData: {
      },
      // 主内容表格选中数据数组
      ids: [],
      // 新增时自动获取当前时间
      addTimeNow: null,
      // 仓库列表
      warehouseData: [],
      // 部门列表
      deptData: [],
      // 详情数据
      rowInfo: {},
      processInstanceId: undefined
    }
  },
  created () {
    this.getList()
    this.getWareList()
    this.getListDept()
  },
  methods: {
    /** 获取列表数据 */
    getList() {
      getWareBackByCondition(this.queryParams).then(res => {
        if (res.code === 200) {
          this.assetListData = res.data.records || []
          this.total = res.data.total
        }
      })
    },
    /** 获取仓库列表*/
    getWareList () {
      listWarehouse({ pageSize: 1000 }).then(
        res => {
          this.warehouseData = res.data.records
        }
      )
    },
    /** 获取部门信息*/
    getListDept () {
      listDept({}).then(response => {
        this.deptData = this.handleTree(response.data, 'deptId')
      })
    },
    /** 打印 */
    handlePrint() {
      if (!this.ids.length) {
        return this.$message.warning('请勾选要打印的项')
      }
      this.$modal.loading('正在获取打印数据，请稍候...')
      Promise.all(this.ids.map(item => getWareBackDetailsById(item))).then(res => {
        this.$modal.closeLoading()
        this.$refs.FilePrint.show(res.map(item => item.data))
      })
    },
    /** 导出 */
    handleExport () {
      this.download(
        'biz/wareBack/export/',
        this.ids,
        `资产退库_${this.parseTime(new Date(), '{y}-{m}-{d}')}.xlsx`
      )
    },
    /** 新增 */
    handleAdd() {
      this.open = true
      this.isEdit = true
      this.isDetail = false
      this.isDisabled = false
      this.title = '新建退库'
      this.$nextTick(() => { this.$refs.addDialog.reset() })
      this.reset()
      this.addTimeNow = dayjsFormat(new Date().getTime()).split(' ')[0]
      //
    },
    /** 重置 */
    reset() {
      this.addDialogData = {
        orderNo: '',
        title: '',
        status: '',
        userId: this.$store.state.user.userId,
        userName: this.$store.state.user.realName,
        note: '',
        warehouse: '',
        warehouseId: '',
        backDeptId: this.$store.state.user.deptId,
        backDept: this.$store.state.user.deptName,
        wareBackDetails: []
      }
    },
    /** 刷新 */
    refresh() {
      this.getList()
    },
    /** 重置 */
    resetQuery() {
      this.queryParams = {
        pageNum: 1,
        pageSize: 10,
        peopleName: undefined,
        title: undefined,
        status: undefined
      }
      this.getTime = []
      this.getList()
    },
    /** 搜索 */
    handleQuery() {
      this.queryParams.startTime = this.getTime[0]
      this.queryParams.endTime = this.getTime[1]
      this.getList(this.queryParams)
    },
    /** 主内容表格选中 */
    /** 多选框选中数据 */
    handleSelectionChange (selection) {
      this.ids = selection.map(item => item.id)
    },
    /** 创建时间格式化 */
    dayjsFormat,
    /** 根据状态取定type */
    getStatueType (value) {
      switch (value) {
        case '0':
          return 'info'
        case '1':
          return 'warning'
        case '2':
          return 'primary'
        case '3':
          return 'success'
        case '4':
          return 'danger'
        default:
          return ''
      }
    },
    /** 根据状态显示文本 */
    getStatusName (value) {
      switch (value) {
        case '0':
          return '草稿'
        case '1':
          return '待审批'
        case '2':
          return '审批中'
        case '3':
          return '待退库'
        case '4':
          return '不通过'
        case '5':
          return '已退库'
        default:
          return ''
      }
    },
    /** 获取样式 */
    getStyle (value) {
      switch (value) {
        case '0':
          return { lineHeight: '35.6px', color: '#409EFF' }
        case '1':
          return { lineHeight: '35.6px', color: '#67C23A' }
        case '2':
          return { lineHeight: '35.6px', color: '#22C23A' }
        case '3':
          return { lineHeight: '35.6px', color: '#22C23A' }
        case '4':
          return { lineHeight: '35.6px', color: '#F56C6C' }
        case '5':
          return { lineHeight: '35.6px', color: '#F56C6C' }
        default:
          return ''
      }
    },
    /** 详情 */
    handleDetail(row) {
      this.isEdit = false
      this.isDetail = true
      this.isDisabled = true
      this.title = '退库详情'
      this.open = true
      this.rowInfo = { ...row }
      // 展示审批流程的数据
      this.processInstanceId = row.processInstanceId
      this.$nextTick(() => {
        this.$refs.addDialog.$refs.approvalProcess.processInstanceId = row.processInstanceId
        if (this.processInstanceId) {
          this.$refs.addDialog.$refs.approvalProcess.getList(row.processInstanceId)
        }
      })
      getWareBackDetailsById(row.id).then(res => {
        // 详情数据
        if (res.code === 200) {
          this.addDialogData = res.data || {}
          this.addDialogData.wareBackDetails = this.filterWareBackDetails(res.data.wareBackDetails)
        } else {
          this.$message.error(res.msg)
        }
      })
    },
    /** 编辑 */
    handleEdit(row) {
      this.isEdit = true
      this.open = true
      this.isDetail = false
      this.isDisabled = false
      this.title = '编辑'
      getWareBackDetailsById(row.id).then(res => {
        // 编辑数据
        if (res.code === 200) {
          this.addDialogData = res.data || {}
          this.addDialogData.wareBackDetails = this.filterWareBackDetails(res.data.wareBackDetails)
        } else {
          this.$message.error(res.msg)
        }
      })
    },
    /** 删除 */
    handleDelete(row) {
      this.$modal.confirm('确定删除退库单 "' + row.orderNo + '"？', '确定删除').then(function () {
        return deleteWareBack(row.id)
      }).then((res) => {
        if (res.code === 200) {
          this.getList()
          this.$modal.msgSuccess('删除成功')
        } else {
          this.$message.error(res.msg)
        }
      }).catch(() => { })
    },
    /** 关闭弹窗 */
    closeDialog() {
      this.$nextTick(() => {
        this.$refs.addDialog.reset()
      })
      this.open = false
    },
    /** 弹窗提交 */
    save(index) {
      this.$refs.addDialog.submit(index)
    },
    /** 创建成功 */
    successwork(payload) {
      this.open = false
      if (payload.code === 200) {
        this.$modal.msgSuccess('保存成功')
        this.getList()
      }
    },
    /** 创建失败 */
    failwork(payload) {
      this.$message.error(payload.msg)
      this.open = false
    },
    /** 过滤数组 */
    filterWareBackDetails(arr) {
      const newArr = []
      arr.forEach(item => {
        const obj = {
          assetId: item.assetId,
          code: item.code,
          internalCode: item.internalCode,
          name: item.name,
          brandName: item.brandName,
          dept: item.deptName,
          standard: item.standard,
          unit: item.unit,
          warehouseName: item.warehouseName
        }
        newArr.push(obj)
      })
      return newArr
    }
  }
}
</script>
<style lang="scss" scoped>
.post-box {
  border: none;
  .btn-icon {
    width: 18px;
    height: 18px;
    vertical-align: middle;
  }
  .btn-text {
    vertical-align: middle;
  }
  // 主内容表格
  .main-card {
    // 筛选栏-创建时间
    .date_box {
      position: relative;
      width: fit-content;
      .data_icon {
        position: absolute;
        top: 50%;
        right: 17px;
        z-index: 9;
        color: #c0c4cc;
        font-size: 14px;
        transform: translateY(-50%);
      }
      ::v-deep .el-input__icon.el-range__icon.el-icon-date {
        display: none;
      }
    }
    // 主内容表格
    .asset-collection-table {
      margin-top: 16px;
      .status-tag {
        width: 72px;
      }
    }
  }
  // 新增弹窗

}
.addDig {

  ::v-deep .el-dialog .el-dialog__body {
    padding-top: 0;
    max-height: 850px;
    overflow-y: auto;
  }
  ::v-deep .el-table--border{
    border-bottom:1px solid #EBEEF5;
    }
  }
.dialog-footer {
  display: flex;
  justify-content: space-between;
}
</style>

