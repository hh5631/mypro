<template>
  <div class="post-box">
    <!-- 头部 -->
    <tags-view>
      <el-row :gutter="10" class="mb8">
        <el-col :span="1.5">
          <el-button v-hasPermi="[' warehousem:outboundOrder:print']" size="small" @click="handlePrint">
            <i
              class="iconfont icon-dayin"
              style="font-size: 14px; margin-right: 8px"
            />
            <span class="btn-text">打印</span>
          </el-button>
        </el-col>
        <el-col :span="1.5">
          <el-button
            v-hasPermi="['warehousem:outboundOrder:export']"
            size="small"
            @click="handleExport"
          >
            <i
              class="iconfont icon-daochu"
              style="font-size: 14px; margin-right: 8px"
            />
            <span class="btn-text">导出</span>
          </el-button>
        </el-col>
        <el-col :span="1.5">
          <el-button size="small" @click="refresh">
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
    <!-- 主体 -->
    <el-card class="main-card">
      <!-- 查找 -->
      <el-form
        ref="queryForm"
        :model="queryParams"
        :inline="true"
        label-width="80"
      >
        <el-form-item label="退库单号" prop="orderNo">
          <el-input
            v-model.trim="queryParams.orderNo"
            placeholder="请输入"
            clearable
            size="small"
            style="width: 200px"
            maxlength="15"
            @keyup.enter.native="handleQuery"
          />
        </el-form-item>
        <el-form-item label="退库标题" prop="title">
          <el-input
            v-model.trim="queryParams.title"
            placeholder="请输入"
            clearable
            size="small"
            style="width: 200px"
            maxlength="15"
            @keyup.enter.native="handleQuery"
          />
        </el-form-item>
        <el-form-item label="退库状态" prop="status">
          <el-select
            v-model="queryParams.status"
            clearable
            placeholder="请选择"
          >
            <el-option
              v-for="dict in dict.type.withdrawingInfo_type"
              :key="dict.value"
              :label="dict.label"
              :value="dict.value"
            />
          </el-select>
        </el-form-item>
        <el-form-item label="退库时间">
          <div class="date_box">
            <el-date-picker
              v-model="dateRange"
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
            v-hasPermi="['warehousem:outboundOrder:list']"
            size="small"
            class="search-btn"
            type="primary"
            @click="handleQuery"
          >搜索</el-button>
        </el-form-item>
      </el-form>
      <!-- 列表 -->
      <el-table
        :data="listData"
        highlight-current-row
        style="width: 100%"
        height="550px"
        @selection-change="handleSelectionChange"
      >
        <el-table-column
          type="selection"
          label="多选"
          width="45"
          align="center"
        />
        <el-table-column label="序号" type="index" width="55" align="center" />
        <el-table-column
          prop="orderNo"
          label="退库单号"
          width="150"
          align="center"
        />
        <el-table-column
          prop="title"
          label="退库标题"
          width="180"
          align="center"
        />
        <el-table-column prop="createTime" label="创建时间" align="center" />
        <el-table-column prop="userName" label="退库人" align="center" />
        <el-table-column prop="backDept" label="接收部门" align="center" />
        <el-table-column
          prop="warehouse"
          label="存放位置"
          width="150"
          align="left"
          :show-overflow-tooltip="true"
        />
        <el-table-column prop="status" label="退库状态" align="center">
          <template slot-scope="scope">
            <el-tag :type="getStatueType(scope.row.status)">{{
              getStatusName(scope.row.status)
            }}</el-tag>
          </template>
        </el-table-column>
        <el-table-column prop="updateTime" label="退库时间" align="center" />
        <el-table-column
          prop="userName"
          label="负责人"
          align="center"
        />
        <el-table-column label="操作" width="200" align="center">
          <template slot-scope="scope">
            <el-button
              v-if="getStatusName(scope.row.status) !== '已退库'"
              v-hasPermi="['warehousem:outboundOrder:confirm']"
              type="text"
              size="small"
              style="color: #4686ef"
              @click="handleEnsureWareBack(scope.row)"
            >确认退库
            </el-button>
            <el-button
              v-hasPermi="['warehousem:outboundOrder:info']"
              type="text"
              size="small"
              style="color: #4686ef"
              @click="handleDetail(scope.row)"
            >详情
            </el-button>
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
      ref="referenceDialog"
      destroy-on-close
      :title="title"
      :visible.sync="open"
      width="1200px"
      append-to-body
      class="addDig"
      :close-on-click-modal="false"
      @close="closeDialog"
    >
      <ensureDialog
        ref="ensureDialog"
        :is-edit="isEdit"
        :is-detail="isDetail"
        :is-disabled="isDisabled"
        :warehouse-data="warehouseData"
        :dept-data="deptData"
        :log-list="logList"
        :data="ensureDialogData"
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
            @click="save"
          >确认退库</el-button>
        </div>
      </div>
    </el-dialog>
    <FilePrint ref="FilePrint" type="warehousingInfo" />
  </div>
</template>

<script>
import TagsView from '@/layout/components/TagsView/index.vue'
import { getWareBackDetailsById } from '@/api/assetmanagement/assetWithdrawal.js'
import { getLog } from '@/api/warehouse/index'
import ensureDialog from './component/ensureDialog.vue'
import { listWarehouse } from '@/api/basicdata/warehouse'
import { getWareBackByAccount } from '@/api/warehouse/index.js'
import { listDept } from '@/api/system/dept'
import FilePrint from '@/components/FilePrint'
export default {
  name: 'WithdrawingInfo',
  components: { TagsView, ensureDialog, FilePrint },
  dicts: ['withdrawingInfo_type'],
  data () {
    return {
      // 分页总条数
      total: 0,
      // 入库表格展示数据
      listData: [],
      // 查找绑定数据
      queryParams: {
        pageNum: 1,
        pageSize: 10,
        orderNo: undefined,
        title: undefined,
        status: undefined,
        startTime: undefined,
        endTime: undefined
      },
      // 搜索时间
      dateRange: [],
      open: false,
      isDetail: false,
      isEdit: false,
      isDisabled: false,
      title: '',
      // 仓库列表
      warehouseData: [],
      ensureDialogData: {},
      rowInfo: {},
      // 部门列表
      deptData: [],
      logList: [],
      ids: []
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
      getWareBackByAccount(this.queryParams).then(res => {
        if (res.code === 200) {
          this.listData = res.data.records
          this.total = res.data.total
        }
      })
    },
    /** 获取仓库列表*/
    getWareList () {
      listWarehouse({ pageNum: 1, pageSize: 100000 }).then(
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
        'biz/wareBack/exportAccount',
        this.ids,
        `退库台账_${this.parseTime(new Date(), '{y}-{m}-{d}')}.xlsx`
      )
    },
    /** 刷新 */
    refresh() {
      this.getList()
    },
    /** 搜索 */
    handleQuery() {
      this.queryParams.startTime = this.dateRange[0]
      this.queryParams.endTime = this.dateRange[1]
      this.getList(this.queryParams)
    },
    /** 重置 */
    resetQuery() {
      this.queryParams = {
        pageNum: 1,
        pageSize: 10,
        orderNo: undefined,
        title: undefined,
        status: undefined
      }
      this.dateRange = []
      this.getList()
    },
    /** 列表选中 */
    handleSelectionChange(selection) {
      this.ids = selection.map(item => item.id)
    },
    /** 根据状态取定type */
    getStatueType (value) {
      switch (value) {
        case '5':
          return 'success'
        case '3':
          return 'warning'
        default:
          return ''
      }
    },
    /** 根据状态显示文本 */
    getStatusName (value) {
      switch (value) {
        case '3':
          return '待退库'
        case '5':
          return '已退库'
        default:
          return ''
      }
    },
    /** 获取样式 */
    getStyle (value) {
      switch (value) {
        case '3':
          return { lineHeight: '35.6px', color: '#409EFF' }
        case '5':
          return { lineHeight: '35.6px', color: '#67C23A' }
        default:
          return ''
      }
    },
    /** 确认退库操作 */
    handleEnsureWareBack(row) {
      this.open = true
      this.isEdit = true
      this.isDetail = false
      this.title = '确认退库'
      getWareBackDetailsById(row.id).then(res => {
        // 详情数据
        if (res.code === 200) {
          this.ensureDialogData = res.data || {}
        } else {
          this.$message.error(res.msg)
        }
      })
    },
    /** 详情 */
    handleDetail(row) {
      this.open = true
      this.isEdit = false
      this.isDetail = true
      this.title = '退库详情'
      this.isDisabled = true
      this.rowInfo = row
      this.getLog(row.id)
      getWareBackDetailsById(row.id).then(res => {
        // 详情数据
        if (res.code === 200) {
          this.ensureDialogData = res.data || {}
        } else {
          this.$message.error(res.msg)
        }
      })
    },
    /** 最终弹窗确认退库按钮 */
    save() {
      this.$refs.ensureDialog.submit()
    },
    /** 取消退库 */
    closeDialog() {
      this.open = false
    },
    /** 操作日志 */
    getLog(id) {
      const data = {
        bizOrderId: id,
        bizType: 'type_ware_back',
        pageNum: 1,
        pageSize: 1000
      }
      getLog(data).then(response => {
        if (response.code === 200) {
          this.logList = response.data.records
        } else {
          this.$message.error(response.msg)
        }
      })
    },
    /** 退库成功 */
    successwork() {
      this.open = false
      this.getList()
    },
    /** 退库失败 */
    failwork(payload) {
      this.open = false
      this.$message.error(payload.msg)
    }
  }
}
</script>
<style lang="scss" scoped>
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
.dialog-footer {
  display: flex;
  justify-content: space-between;
}

</style>
