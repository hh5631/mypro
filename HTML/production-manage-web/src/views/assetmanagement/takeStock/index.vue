<template>
  <div>
    <tags-view>
      <el-row :gutter="10" class="mb8">
        <!-- <el-col :span="1.5">
          <el-button
            v-hasPermi="['system:user:export']"
            plain
            size="small"
            @click="handleExport"
          ><i
            class="el-icon-printer"
          />打印</el-button>
        </el-col> -->
        <el-col :span="1.5">
          <el-button
            v-hasPermi="['assetManagement:takeStock:export']"
            plain
            size="small"
            @click="handleExport"
          ><i
            style="color: #4686ef"
            class="iconfont icon-daochu"
          />导出</el-button>
        </el-col>
        <el-col :span="1.5">
          <el-button
            v-hasPermi="['assetManagement:takeStock:saveUpdate']"
            type="primary"
            size="small"
            @click="showAddDig(0,{})"
          ><i
            class="iconfont icon-zengjia"
            style="color: #ffffff"
          />新建盘点</el-button>
        </el-col>
        <el-col :span="1.5">
          <el-button
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
    <el-card class="main-card">
      <!-- 页面内容 -->
      <el-row>
        <el-col :span="24">
          <el-form
            ref="queryForm"
            :model="queryParams"
            :inline="true"
            label-width="68px"
            class="searchForm"
          >
            <el-form-item label="盘点单号" prop="checkNo">
              <el-input
                v-model.trim="queryParams.checkNo"
                placeholder="请输入"
                clearable
                size="small"
                style="width: 200px"
                maxlength="15"
                @keyup.enter.native="handleQuery"
              />
            </el-form-item>
            <el-form-item label="盘点人" prop="checkPerson">
              <el-input
                v-model.trim="queryParams.checkPerson"
                placeholder="请输入"
                clearable
                size="small"
                style="width: 200px"
                maxlength="15"
                @keyup.enter.native="handleQuery"
              />
            </el-form-item>

            <el-form-item>
              <el-button size="small" @click="resetQuery">重置</el-button>
              <el-button
                v-hasPermi="['assetManagement:takeStock:list']"
                size="small"
                class="search-btn"
                @click="handleQuery"
              >搜索</el-button>
            </el-form-item>
          </el-form>
        </el-col>
        <!-- <el-col :span="2">
          <el-button
            size="small"
            class="search-btn"
            @click="changeState"
          >设置为可领</el-button>
        </el-col> -->
      </el-row>

      <!-- 表格显示list数据 -->
      <el-table
        ref="tableData"
        v-loading="loading"
        highlight-current-row
        height="550"
        :data="tableData"
        @selection-change="handleSelectionChange"
      >
        <el-table-column type="selection" align="center" />
        <el-table-column
          label="序号"
          sortable
          align="center"
          prop="index"
          type="index"
        />
        <el-table-column label="" align="left" class-name="small-padding" width="200">
          <template slot-scope="scope">
            <el-button
              v-debounce
              size="small"
              type="text"
              :disabled="scope.row.taskStatus === '2'"
              @click.native="handleStart(scope.row,1)"
            >开始盘点</el-button>
            <el-button
              v-debounce
              size="small"
              type="text"
              :class="
                scope.row.taskStatus === '2'?'handleBtn':''
              "
              :disabled="scope.row.taskStatus !== '2'"
              @click.native="handleStart(scope.row,2)"
            >盈亏处理</el-button>
          </template>
        </el-table-column>
        <el-table-column
          label="盘点单号"
          align="left"
          prop="checkNo"
          :show-overflow-tooltip="true"
        />
        <el-table-column
          label="盘点人"
          align="left"
          prop="checkPersonName"
          :show-overflow-tooltip="true"
        />
        <el-table-column
          label="盘点类型"
          align="left"
          prop="checkRange"
          :show-overflow-tooltip="true"
        >
          <template slot-scope="scope">
            <div>
              <span v-if="scope.row.checkRange==='0'">所有</span>
              <span v-if="scope.row.checkRange==='1'">指定范围</span>
            </div>
          </template>
        </el-table-column>
        <el-table-column
          label="开始时间"
          align="center"
          prop="startTime"
          :show-overflow-tooltip="true"
        />
        <el-table-column
          label="盘点进度"
          align="left"
          :show-overflow-tooltip="true"
        >
          <template slot-scope="scope">
            <div>
              {{ scope.row.processedCheckCount+'/'+scope.row.unhandledCheckCount }}
            </div>
          </template>
        </el-table-column>
        <el-table-column
          label="计划时间"
          align="left"
          prop="planStartTime"
          :show-overflow-tooltip="true"
          width="180"
        >
          <template slot-scope="scope">
            <div>
              {{ scope.row.planStartTime }}~{{ scope.row.planEndTime }}
            </div>
          </template>
        </el-table-column>
        <el-table-column label="状态" align="left" prop="taskStatus">
          <template slot-scope="scope">
            <el-tag v-if="scope.row.taskStatus === '0'">未开始</el-tag>
            <el-tag v-if="scope.row.taskStatus === '1'" type="warning">盘点中</el-tag>
            <el-tag v-if="scope.row.taskStatus === '2'" type="success">已完成</el-tag>
          </template>
        </el-table-column>
        <el-table-column
          label="完成时间"
          align="left"
          prop="endTime"
          :show-overflow-tooltip="true"
        />
        <el-table-column label="操作" align="left" class-name="small-padding">
          <template slot-scope="scope">
            <el-button
              v-hasPermi="['assetManagement:takeStock:byCondition']"
              size="small"
              type="text"
              @click="handleDetail(scope.row)"
            >详情</el-button>
            <el-button
              v-if="scope.row.taskStatus === '0'"
              v-hasPermi="['assetManagement:takeStock:saveUpdate']"
              size="small"
              type="text"
              @click="showAddDig(1,scope.row)"
            >编辑</el-button>
          </template>
        </el-table-column>
      </el-table>
      <!-- 分页 -->
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

    </el-card>
    <!-- 新建或编辑位置对话框 -->
    <el-dialog
      :title="addTitle"
      :visible.sync="open"
      width="700px"
      append-to-body
      :close-on-click-modal="false"
      class="selectPlan"
      @close="closeAddDig"
    ><AddInventory ref="addInventory" @cancelAdd="cancelAdd" @submitForm="submitAddForm" />

    </el-dialog>
    <!-- 添加盘盈资产 -->
    <el-dialog
      title="添加盘盈资产"
      :visible.sync="openSurplusAssets"
      width="700px"
      append-to-body
      :close-on-click-modal="false"
      class="selectPlan"
      @close="closeSurplusAssetsDig"
    ><AddSurplusAssets ref="addSurplusAssets" @addsucess="addSucess" />

    </el-dialog>
    <!-- 开始盘点和盈亏处理 -->
    <el-dialog
      :title="diaTitle"
      :visible.sync="openHandle"
      width="1050px"
      append-to-body
      :close-on-click-modal="false"
      class="selectPlan"
      @close="closeHandle"
    ><Handle
       ref="assetsHandle"
       :base-form="baseForm"
       :handle-type="handleType"
       @addProfit="addProfit"
       @editProfit="editProfit"
     />
      <div
        slot="footer"
        class="dialog-footer"
      >
        <el-button @click="closeHandle">关闭</el-button>
      </div>
    </el-dialog>
    <el-dialog
      title="盘点详情"
      :visible.sync="openDetail"
      width="1200px"
      append-to-body
      :close-on-click-modal="false"
      class="selectPlan"
      @close="closeDetailDig"
    ><Detail
       ref="handleDetail"
       @finishTask="finishTask"
     />
      <div
        slot="footer"
        class="dialog-footer"
      > <el-button v-if="detailForm.taskStatus==='2'||detailForm.taskStatus==='0'" @click="finishTask">关闭</el-button>
        <el-button v-if="detailForm.taskStatus!=='2'&&detailForm.taskStatus!=='0'" @click="finishTask">取消</el-button>
        <el-button v-if="detailForm.taskStatus!=='2'&&detailForm.taskStatus!=='0'" type="primary" @click="completeSuccess">完成</el-button>
      </div>
    </el-dialog>
    <el-dialog
      title="提示"
      :visible.sync="warningShow"
      width="30%"
      @close="warningShow = false"
    >
      <span>导出数据超出2000条,是否确定要全部导出?</span>
      <span slot="footer" class="dialog-footer">
        <el-button @click="warningShow = false">取 消</el-button>
        <el-button type="primary" @click="handleExport">确 定</el-button>
      </span>
    </el-dialog>
  </div>
</template>

<script>
import { getCheckTaskList, saveOrUpdateCheckTask, getCheckTaskDetailsById, startTask } from '@/api/assetmanagement/takeStock.js'
import TagsView from '@/layout/components/TagsView/index.vue'
import AddInventory from './component/add.vue'
import AddSurplusAssets from './component/addSurplusAssets.vue'
import Handle from './component/handle.vue'
import '@riophae/vue-treeselect/dist/vue-treeselect.css'
import Detail from './component/detail.vue'
export default {
  name: 'Place',
  components: { TagsView, AddInventory, Handle, AddSurplusAssets, Detail },
  dicts: ['assets_enable_status'],
  data () {
    return {
      currentRow: {},
      diaTitle: '资产盘点',
      warningShow: false,
      // 遮罩层加载效果显示
      loading: false,
      // 右侧头部表单状态
      status: [
        {
          label: '正常',
          value: true
        },
        {
          label: '停用',
          value: false
        }
      ],
      // 右侧头部表单查询参数
      queryParams: {
        pageNum: 1,
        pageSize: 10,
        checkPerson: undefined,
        checkNo: undefined
      },
      // 右侧主体表格显示data
      tableData: [],
      // 右侧主体多选数组id
      ids: [],
      // 右侧主体多选数组不可领用下的id
      arrCheckIds: [],
      // 右侧非单个禁用
      single: true,
      // 右侧非多个禁用
      multiple: true,
      // 右侧主体保存row信息
      rowInfo: {},
      placeNodes: [],
      // 右下分页选中条数
      selectedLength: 0,
      // 右下分页总条数
      total: 0,
      // 弹出层是否显示
      open: false,
      openHandle: false,
      openDetail: false,
      openSurplusAssets: false,
      detailForm: {}, // 详情数据
      handleType: 1, // 1资产盘点2盈亏处理
      // 弹出层标题
      addTitle: '新建盘点',
      // 详情页表单
      baseForm: {
        id: undefined,
        type: '',
        code: '',
        unit: '',
        standard: '',
        brandName: '',
        status: '',
        place: '',
        codeIcon: '',
        price: '',
        rate: ''
        // // 购置金额
        // acquireDate: '',
      }

    }
  },
  computed: {
  },
  watch: {
  },
  created () {
    this.getList()
  },
  methods: {
    /** 顶部停用*/
    stopUse () {
    },
    /** 导出 */
    handleExport () {
      if (!this.ids.length && this.total > 2000 && !this.warningShow) {
        this.warningShow = true
        return
      }
      this.download(
        '/biz/checkTask/export/',
        this.ids,
        `资产盘点_${this.parseTime(new Date(), '{y}-{m}-{d}')}.xlsx`
      )
      this.warningShow = false
    },
    /** 顶部新增位置*/
    async  showAddDig (type, row) {
      this.open = true
      // 显示对话框
      this.$nextTick(() => {
        // 新增弹窗重置表单
        this.$refs.addInventory.resetAddForm()
        this.$refs.addInventory.initData()
        if (type === 0) {
          this.addTitle = '新建盘点'
        } else {
          this.addTitle = '编辑任务'
          this.getCheckTaskDetailsById(row.id, 'edit')
        }
      })
    },
    // 取消新增
    cancelAdd() {
      this.open = false
    },
    /** 左侧关键字过滤树节点 */
    filterNode (value, data) {
      if (!value) return true
      return data.name.indexOf(value) !== -1
    },
    /** 右侧头部搜索按钮操作 */
    handleQuery () {
      this.queryParams.pageNum = 1
      this.getList()
    },
    /** 右侧头部重置按钮操作 */
    resetQuery () {
      this.resetForm('queryForm')
      this.handleQuery()
    },
    /** 刷新 */
    refresh () {
      this.getList()
    },
    /** 查询list结构 */
    getList () {
      this.tableData = []
      this.loading = true
      getCheckTaskList(this.addDateRange(this.queryParams)).then(
        (response) => {
          if (response.code === 200) {
            this.loading = false
            this.tableData = response.data.records
            this.total = response.data.total
            this.$refs.tableData.clearSelection()
          } else {
            this.$message.error(response.msg)
          }
        }
      )
    },
    // 根据id查询明细
    async getCheckTaskDetailsById (id, type) {
      const res = await getCheckTaskDetailsById(id)
      // console.log('res', res)
      if (res.code === 200) {
        if (type === 'edit') {
          this.open = true
          this.$nextTick(() => {
            // 编辑时格式化数据
            this.$refs.addInventory.getFormData(res.data)
          })
        }
        if (type === 'detail') {
          this.openDetail = true
          this.detailForm = res.data
          this.$nextTick(() => {
            this.$refs.handleDetail.getFormData(res.data)
          })
        }
      } else {
        this.$message.error(res.msg)
      }
    },
    // 选中数据
    handleSelectionChange (selection) {
      this.selectedLength = selection.length
      this.ids = []
      this.arrCheckIds = []
      selection.forEach(item => {
        this.ids.push(item.id)
      })
      selection.forEach(item => {
        if (item.status === '1') {
          this.arrCheckIds.push(item.id)
        }
      })
      this.single = selection.length !== 1
      this.multiple = !selection.length
    },
    // 开始盘点
    handleStart (row, type) {
      // 无论是详情还是编辑都打开对话框
      this.currentRow = row
      this.openHandle = true
      this.handleType = type
      this.$nextTick(() => {
        this.$refs.assetsHandle.init(row, 'init')
      })
      if (type === 1) {
        this.diaTitle = '资产盘点'
        if (row.taskStatus === '0') {
          this.startTask(row.id)
        }
      } else {
        this.diaTitle = '盈亏处理'
        this.$nextTick(() => {
          this.$refs.assetsHandle.getHandleStatus(row.checkNo)
        })
      }
    },
    startTask(id) {
      startTask(id).then((res) => {
        if (res.code === 200) {
          // this.$modal.msgSuccess('保存成功')
          this.getList()
        } else {
          this.$message.error(res.msg)
        }
      })
    },
    closeHandle() {
      this.openHandle = false
      this.getList()
    },
    handleDetail(row) {
      this.getCheckTaskDetailsById(row.id, 'detail')
    },
    // 点击完成
    completeSuccess() {
      this.$nextTick(() => {
        this.$refs.handleDetail.completeTask()
      })
    },
    finishTask() {
      this.openDetail = false
      this.getList()
    },
    /** 弹出对话框关闭 */
    closeAddDig () {
      this.open = false
    },
    closeSurplusAssetsDig() {
      this.$refs.addSurplusAssets.resetAddForm()
      this.openHandle = true
    },
    closeDetailDig () { },
    // 展示添加盘盈资产弹框
    addProfit(id) {
      this.openSurplusAssets = true
      this.openHandle = false
      this.$nextTick(() => {
        this.$refs.addSurplusAssets.taskId = id
      })
    },
    // 编辑盘盈资产
    editProfit(data) {
      this.openSurplusAssets = true
      this.openHandle = false
      this.$nextTick(() => {
        this.$refs.addSurplusAssets.getFormData(data)
      })
    },
    /** 新建盘点-最终提交数据方法 */
    submitAddForm(data) {
      // 拼装数据
      const params = {
        id: data.id,
        // 盘点人
        checkPerson: data.peopleId,
        // 盘点范围
        checkRange: data.range,
        // 计划开始时间
        planStartTime: data.dateRange[0],
        // 计划结束时间
        planEndTime: data.dateRange[1],
        // 备注
        remark: data.remark,
        // 盘点范围-指定部门数组
        deptIds: data.dept,
        // 盘点范围-指定位置数组
        placeIds: data.place,
        // 盘点范围-指定类别(资产类别)数组
        goodCodes: data.category,
        // 盘点范围-指定仓库数组
        warehouseIds: data.warehouse
      }
      this.open = false
      // 调接口新建盘点
      saveOrUpdateCheckTask(params).then((res) => {
        if (res.code === 200) {
          this.$modal.msgSuccess('保存成功')
          this.getList()
        } else {
          this.$message.error(res.msg)
        }
      })
    },
    // 添加盘盈资产成功
    addSucess() {
      this.openSurplusAssets = false
      this.$nextTick(() => {
        this.$refs.assetsHandle.init(this.currentRow, 'init')
      })
      this.closeSurplusAssetsDig()
    }

  }
}
</script>

<style  lang="scss" scoped>
.chooseCount {
  color: #7d8592;
  margin-top: 20px;
}
.inputContent {
  display: flex;
  justify-content: space-between;
  border-bottom: 1px solid #eaedf4;
}
.statePoint .point {
  width: 8px;
  height: 8px;
  margin: 0 8px 2px 0;
}
::v-deep .el-tree-node:focus > .el-tree-node__content {
  background-color: #fff;
}
.add-card {
  margin: 20px 0 10px;
}
.head-container {
  ::v-deep .el-tree-node__content {
    padding: 6px;
    height: auto;
  }
  ::v-deep
    .el-tree--highlight-current
    .el-tree-node.is-current
    > .el-tree-node__content {
    background-color: #d7e6ff;
  }
}
.selectPlan {
  ::v-deep .el-dialog__body {
    padding-top: 0;
  }
}
.handleBtn{
  color: #ef5826;
}
</style>
