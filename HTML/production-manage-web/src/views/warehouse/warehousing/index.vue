<template>
  <div class="post-box">
    <!-- 头部 -->
    <tags-view>
      <el-row :gutter="10" class="mb8">
        <el-col :span="1.5">
          <el-button
            v-hasPermi="['warehousem:warehousingOrder:export']"
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
          <el-button
            v-hasPermi="['warehousem:warehousingOrder:print']"
            size="small"
            @click="handlePrint"
          >
            <i
              class="el-icon-printer"
              style="font-size: 14px; margin-right: 0px"
            />
            <span class="btn-text">打印</span>
          </el-button>
        </el-col>
        <el-col :span="1.5">
          <el-button
            v-hasPermi="['warehousem:warehousingOrder:add']"
            type="primary"
            size="small"
            @click="handleAdd"
          >
            <i
              class="iconfont icon-zengjia"
              style="font-size: 14px; margin-right: 8px"
            />
            <span class="btn-text">新增入库</span>
          </el-button>
        </el-col>
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
    <!-- 主体 -->
    <el-card class="main-card">
      <!-- query form -->
      <div class="querybox">
        <el-form
          ref="queryForm"
          class="queryform"
          :model="queryParams"
          :inline="true"
        >
          <el-form-item label="入库单号" prop="orderNo">
            <el-input
              v-model.trim="queryParams.orderNo"
              placeholder="请输入"
              clearable
              maxlength="15"
              size="small"
              style="width: 200px"
              @keyup.enter.native="handleQuery"
            />
          </el-form-item>
          <el-form-item label="单据名称" prop="orderName">
            <el-input
              v-model.trim="queryParams.orderName"
              placeholder="请输入"
              clearable
              maxlength="15"
              size="small"
              style="width: 200px"
              @keyup.enter.native="handleQuery"
            />
          </el-form-item>
          <el-form-item label="入库类型" prop="origin">
            <el-select
              v-model="queryParams.origin"
              style="width: 200px"
              placeholder="请选择"
              clearable
              size="small"
            >
              <el-option
                v-for="dict in dict.type.warehousing_order_origin"
                :key="dict.value"
                :label="dict.label"
                :value="dict.value"
              />
            </el-select>
          </el-form-item>
          <el-form-item label="入库仓库" prop="warehouseId">
            <el-select
              v-model="queryParams.warehouseId"
              style="width: 200px"
              placeholder="请选择"
              clearable
              size="small"
            >
              <el-option
                v-for="item in warehouseData"
                :key="item.id"
                :label="item.name"
                :value="item.id"
              />
            </el-select>
          </el-form-item>
          <el-form-item label="入库时间">
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

          <el-form-item label="入库状态" prop="status">
            <el-select
              v-model="queryParams.status"
              style="width: 200px"
              placeholder="请选择"
              clearable
              size="small"
            >
              <el-option
                v-for="dict in dict.type.warehousing_order_status"
                :key="dict.value"
                :label="dict.label"
                :value="dict.value"
              />
            </el-select>
          </el-form-item>
          <el-form-item label="关联单号" prop="bizOrderNo">
            <el-input
              v-model.trim="queryParams.bizOrderNo"
              placeholder="请输入"
              clearable
              maxlength="15"
              size="small"
              style="width: 200px"
              @keyup.enter.native="handleQuery"
            />
          </el-form-item>
          <el-form-item>
            <el-button size="small" @click="resetQuery">重置</el-button>
            <el-button
              v-hasPermi="['warehousem:warehousingOrder:list']"
              type="primary"
              plain
              size="small"
              class="search-btn"
              @click="handleQuery"
            >搜索</el-button>
          </el-form-item>
        </el-form>
        <!-- <div class="showOrhide" @click="toggleShow">
          <i :class="['iconfont', isActive ? downClass : upClass]" />
          <span class="showtext">{{ isActive ? "展开" : "收起" }}</span>
        </div> -->
      </div>
      <!-- show table -->
      <el-table
        ref="warehouseTable"
        v-loading="loading"
        highlight-current-row
        :data="listData"
        height="550px"
        @selection-change="handleSelectionChange"
      >
        <el-table-column
          type="selection"
          label="多选"
          width="45"
          align="left"
        />
        <el-table-column label="序号" type="index" width="55" align="left">
          <template #default="scope">
            <span>{{
              (queryParams.pageNum - 1) * queryParams.pageSize +
                scope.$index +
                1
            }}</span>
          </template></el-table-column>
        <el-table-column
          label="入库单号"
          width="150"
          align="left"
          prop="orderNo"
          :show-overflow-tooltip="true"
        />
        <el-table-column
          label="单据名称"
          width="180"
          align="left"
          prop="orderName"
          :show-overflow-tooltip="true"
        />
        <el-table-column
          label="入库类型"
          align="left"
          prop="origin"
          :show-overflow-tooltip="true"
        >
          <template slot-scope="scope">
            <span>{{ getWarehousingType(scope.row.origin) }}</span>
          </template>
        </el-table-column>
        <el-table-column
          label="入库仓库"
          align="left"
          prop="warehouseName"
          :show-overflow-tooltip="true"
        />
        <el-table-column
          label="关联单号"
          width="150"
          align="left"
          prop="bizOrderNo"
          :show-overflow-tooltip="true"
        />
        <el-table-column
          label="入库状态"
          align="left"
          prop="status"
          :show-overflow-tooltip="true"
        >
          <template slot-scope="scope">
            <el-tag :type="getStatueType(scope.row.status)">{{
              getStatusName(scope.row.status)
            }}</el-tag>
          </template>
        </el-table-column>
        <el-table-column
          label="入库时间"
          align="left"
          prop="createWarehousingDate"
          :show-overflow-tooltip="true"
        />
        <el-table-column
          label="负责人"
          align="left"
          prop="chargePerson"
          :show-overflow-tooltip="true"
        />
        <el-table-column
          label="操作"
          width="200"
          align="left"
          class-name="small-padding"
        >
          <template slot-scope="scope">
            <el-button
              v-if="getStatusName(scope.row.status) === '待入库' || getStatusName(scope.row.status) === '部分入库'"
              v-hasPermi="['warehousem:warehousingOrder:confirm']"
              size="small"
              type="text"
              style="color: #4686ef"
              @click="handleWarehousing(scope.row)"
            >验收入库
            </el-button>
            <el-button
              v-hasPermi="['warehousem:warehousingOrder:info']"
              size="small"
              type="text"
              style="color: #4686ef"
              @click="handleDetail(scope.row)"
            >详情</el-button>
            <el-button
              v-if="getStatusName(scope.row.status) === '待入库' || getStatusName(scope.row.status) === '部分入库'"
              v-hasPermi="['warehousem:warehousingOrder:update']"
              size="small"
              type="text"
              style="color: #4686ef"
              @click="handleUpdate(scope.row)"
            >编辑</el-button>
            <el-button
              v-if="getStatusName(scope.row.status) === '已废弃'"
              v-hasPermi="['warehousem:warehousingOrder:delete']"
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
    <!-- 新增/编辑入库对话框 -->
    <template v-if="open">
      <el-dialog
        class="dialog-class"
        :title="title"
        :visible.sync="open"
        :width="isEdit ? '1200px' : '1200px'"
        append-to-body
        :close-on-click-modal="false"
        @close="closeDialog"
      >
        <!-- 新增/编辑/详情入库组件 -->
        <addWarehouseOrder
          ref="addForm"
          :is-edit="isEdit"
          :is-sure="isSure"
          :is-new="isNew"
          :show-log="showLog"
          :is-detail="isDeatil"
          :form-data="formData"
          :table-data="tableData"
          :user-data="userData"
          :warehouse-data="warehouseData"
          @submitSuccess="submitSuccess"
          @warehousingSuccess="warehousingSuccess"
          @sureSuccess="sureSuccess"
          @add="add"
          @addDetail="addDetail"
        />
        <div slot="footer" class="dialog-footer">
          <div>
            <span
              v-if="isDisabledModify == false"
              :style="getStyle(rowInfo.status)"
            ><i class="iconfont icon-xiangqingxinxi" />
              {{ getStatusName(rowInfo.status) }}
            </span>
            <span v-else />
          </div>
          <div>
            <el-button @click="cancel">{{ isDeatil&&!isSure?'关闭':"取 消" }}</el-button>
            <el-button
              v-if="isEdit"
              v-debounce
              @click="discard"
            >废 弃</el-button>
            <el-button
              v-if="(getStatusName(rowInfo.status) === '待入库' || getStatusName(rowInfo.status) === '部分入库') && isSure "
              v-debounce
              type="primary"
              @click="confirmWarehousingOrder(rowInfo)"
            >验收入库</el-button>
            <el-button
              v-if="(getStatusName(rowInfo.status) === '待入库' || getStatusName(rowInfo.status) === '部分入库') && !isDeatil "
              v-debounce
              type="primary"
              @click="confirmWarehousingOrder(rowInfo)"
            >保 存</el-button>
            <el-button
              v-if="isNew"
              v-debounce
              type="primary"
              @click="submitForm"
            >保 存</el-button>
          </div>
        </div>
      </el-dialog>
    </template>

    <!-- 选择明细 -->
    <!-- <el-dialog
      :z-index="1000"
      title="生成资产卡片"
      :visible.sync="assetsDig"
      width="1000px"
      append-to-body
      :close-on-click-modal="false"
      class="selectPlan"
      @close="closeAssetsDig"
    ><AddAssetsCard ref="addAssetsCard" :asstes-data="tableData" />
      <div slot="footer">
        <el-button size="mini" @click="closeAssetsDig">取消</el-button>
        <el-button
          size="mini"
          type="primary"
          :disabled="btnDisabled"
          @click="createAssetCard"
        >保存</el-button>
      </div></el-dialog> -->
    <FilePrint ref="FilePrint" type="warehousing" />
    <!-- 标签打印 -->
    <el-dialog
      :z-index="1000"
      title="打印标签"
      :visible.sync="printDig"
      width="1000px"
      append-to-body
      :close-on-click-modal="false"
      class="selectPlan"
      @close="closePrintDig"
    >
      <printAndBindAssetsCard ref="printAndBindAssetsCard" :bind-card-data="bindCardData" />
    </el-dialog>

  </div>
</template>
<script>
import TagsView from '@/layout/components/TagsView/index.vue'
import { getWarehousingOrder, exportWarehousingOrder, toVoidWarehousingOrder, getWarehousingOrderById, deleteWarehousingOrderById } from '@/api/warehouse/index'
import { listWarehouse } from '@/api/basicdata/warehouse'
import FilePrint from '@/components/FilePrint'
import addWarehouseOrder from './component/addWarehouseOrder.vue'
import { blobValidate } from '@/utils/jri'
import errorCode from '@/utils/errorCode'
import { saveAs } from 'file-saver'
import { Message, Loading } from 'element-ui'
// import AddAssetsCard from './component/addAssetsCard.vue'
import printAndBindAssetsCard from './component/printAndBindAssetsCard.vue'
import { listUser } from '@/api/system/user'
export default {
  name: 'Warehousing',
  components: { TagsView, addWarehouseOrder, printAndBindAssetsCard, FilePrint },
  dicts: ['warehousing_order_origin', 'warehousing_order_status'],
  data () {
    return {
      // 选中数组
      ids: [],
      selectedLength: 0,
      // 遮罩层
      loading: true,
      // 分页总条数
      total: 0,
      // 入库表格展示数据
      listData: [],
      // 日期范围
      dateRange: '',
      // query form绑定数据
      queryParams: {
        chargePerson: undefined,
        bizOrderNo: undefined,
        pageNum: 1,
        pageSize: 10,
        orderNo: undefined,
        orderName: undefined,
        origin: undefined,
        status: undefined,
        warehouseId: undefined,
        type: undefined,
        warehousingStartTime: undefined,
        warehousingEndTime: undefined
      },
      // 仓库列表
      warehouseData: [],
      // 弹出层标题
      title: '',
      // 是否显示弹出层
      open: false,
      // 是否可编辑
      isEdit: false,
      // 是否禁用
      isDeatil: false,
      isDisabledModify: false,
      // 基本信息数据
      formData: {},
      // 入库明细数据
      tableData: [],
      // 点击时row数据暂存
      rowInfo: {},
      // 下面三个决定样式类
      isSuccess: false,
      isWait: false,
      isDanger: false,
      showLog: false,
      isNew: false,
      showOther: false,
      isActive: true,
      downClass: 'icon-down2 ',
      upClass: 'icon-up2',
      // assetsDig: false,
      isSure: false, // 验收入库
      printDig: false, // 标签打印弹窗
      bindCardData: [],
      btnDisabled: false, // 禁用按钮
      userData: []
    }
  },
  computed: {
    btnMsg () {
      return this.isDeatil ? '确认入库' : '保 存'
    }
  },
  created () {
    this.getList()
    this.getWareList()
    this.getUserList()
  },
  methods: {
    /** 获取入库信息列表 */
    getList () {
      this.loading = true
      this.queryParams.warehousingStartTime = this.dateRange[0]
      this.queryParams.warehousingEndTime = this.dateRange[1]
      // 获取入库信息列表
      getWarehousingOrder(this.queryParams).then(response => {
        if (response.code === 200) {
          this.listData = response.data.records
          this.total = response.data.total
          this.loading = false
        } else {
          this.loading = false
          this.$message.error(response.msg)
        }
      })
      // 获取仓库列表
    },
    getWareList () {
      listWarehouse({ pageSize: 999 }).then(
        res => {
          this.warehouseData = res.data.records
        }
      )
    },
    getUserList () {
      listUser({ pageNum: 1, pageSize: 999 }).then(response => {
        this.userData = response.rows
      }
      )
    },
    /** 头部tags-view导出按钮操作 */
    handleExport () {
      const downloadLoadingInstance = Loading.service({ text: '正在下载数据，请稍候', spinner: 'el-icon-loading', background: 'rgba(0, 0, 0, 0.7)' })
      exportWarehousingOrder(this.ids).then(
        async data => {
          const filename = `入库管理_${this.parseTime(new Date(), '{y}-{m}-{d}')}.xlsx`
          const isLogin = await blobValidate(data)
          if (isLogin) {
            const blob = new Blob([data])
            saveAs(blob, filename)
          } else {
            const resText = await data.text()
            const rspObj = JSON.parse(resText)
            const errMsg = errorCode[rspObj.code] || rspObj.msg || errorCode['default']
            Message.error(errMsg)
          }
          downloadLoadingInstance.close()
        }
      )
    },
    /** 打印 */
    handlePrint () {
      if (!this.ids.length) {
        return this.$message.warning('请勾选要打印的项')
      }
      this.$modal.loading('正在获取打印数据，请稍候...')
      Promise.all(this.ids.map(item => getWarehousingOrderById(item))).then(res => {
        this.$modal.closeLoading()
        this.$refs.FilePrint.show(res.map(item => item.data))
      })
    },
    /** 头部tags-view新增按钮操作 */
    handleAdd () {
      this.$nextTick(() => { this.reset() })
      this.rowInfo = {}
      this.open = true
      this.isNew = true
      this.isEdit = false
      this.isSure = false
      this.isDeatil = false
      this.showLog = false
      this.isDisabledModify = true
      this.title = '新增入库'
      // this.$store.dispatch('getNo', 'RK').then((res) => {
      //   this.$refs.addForm.form.orderNo = res.data
      //   this.$refs.addForm.form.createTime = formatDate(Date.now())
      // })
    },
    /** 头部tags-view刷新按钮操作 */
    refresh () {
      this.getList()
    },
    /** 展开隐藏 */
    toggleShow () {
      this.showOther = !this.showOther
      this.isActive = !this.isActive
    },
    /** 搜索按钮操作 */
    handleQuery () {
      this.queryParams.pageNum = 1
      this.getList()
    },
    /** 重置按钮操作 */
    resetQuery () {
      this.resetForm('queryForm')
      this.dateRange = ''
      this.handleQuery()
    },
    /** 多选框选中触发事件 */
    handleSelectionChange (selection) {
      this.selectedLength = selection.length
      this.ids = selection.map(item => item.id)
    },
    /** 根据状态确定入库类型 */
    getWarehousingType (value) {
      switch (value) {
        case '0':
          return '新购'
        case '1':
          return '调拨'
        case '2':
          return '赠予'
        case '3':
          return '退库'
        case '10':
          return '其他入库'
        default:
          return ''
      }
    },
    /** 根据状态取定type */
    getStatueType (value) {
      switch (value) {
        case '0':
          return ''
        case '1':
          return 'success'
        case '2':
          return 'warning'
        default:
          return 'danger'
      }
    },
    /** 根据状态显示文本 */
    getStatusName (value) {
      switch (value) {
        case '0':
          // this.isWait = true
          return '待入库'
        case '1':
          this.isSuccess = true
          return '已入库'
        case '2':
          this.isSuccess = true
          return '部分入库'
        case '99':
          this.isDanger = true
          return '已废弃'
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
        case '99':
          return { lineHeight: '35.6px', color: '#F56C6C' }
        default:
          return ''
      }
    },
    /** 验收入库 */
    handleWarehousing(row) {
      this.isNew = false
      this.isEdit = false
      this.isSure = true
      this.isDeatil = true// 验收入库无法改动，相当于详情
      this.open = true
      this.showLog = true
      this.isDisabledModify = false
      this.title = '验收入库'
      this.isSure = true
      this.rowInfo = row
      getWarehousingOrderById(row.id).then(response => {
        if (response.code === 200) {
          this.formData = response.data
          this.tableData = response.data.detailVOS
          this.$nextTick(() => {
            this.$refs.addForm.editWarehouse()
          })
        }
      })
    },
    /** 详情 */
    handleDetail(row) {
      this.isNew = false
      this.isEdit = false
      this.isSure = false
      this.isDeatil = true
      this.open = true
      this.showLog = true
      this.isDisabledModify = false
      this.title = '详情'
      this.rowInfo = row
      getWarehousingOrderById(row.id).then(response => {
        if (response.code === 200) {
          console.log(response)
          this.formData = response.data
          this.tableData = response.data.detailVOS
          this.$nextTick(() => {
            this.$refs.addForm.editWarehouse()
          })
        }
      })
    },
    /** 编辑 */
    handleUpdate(row) {
      this.isNew = false
      this.isEdit = true
      this.isSure = false
      this.isDeatil = false
      this.rowInfo = row
      this.open = true
      this.isDisabledModify = true
      const id = row.id
      this.title = '编辑入库'
      getWarehousingOrderById(id).then(response => {
        if (response.code === 200) {
          this.formData = response.data
          this.tableData = response.data.detailVOS
          this.$nextTick(() => {
            this.$refs.addForm.editWarehouse('edit')
          })
          this.open = true
        } else {
          this.$message.error(response.msg)
        }
      })
    },
    /** 废弃 */
    discard () {
      const { id } = this.rowInfo
      toVoidWarehousingOrder(id).then(
        res => {
          if (res.code === 200) {
            this.open = false
            this.getList()
          }
        }
      )
    },
    /** 删除 */
    handleDelete (row) {
      const { id, orderNo } = row
      this.$modal.confirm('确定删除入库单"' + orderNo + '"?', '确定删除').then(function () {
        return deleteWarehousingOrderById(id)
      }).then(() => {
        this.queryParams.pageNum = this.changePageNum(this.total, 1, this.queryParams.pageNum, this.queryParams.pageSize)
        this.getList()
        this.$modal.msgSuccess('删除成功')
      }).catch(() => { })
    },
    /** 新建入库时表单重置 */
    reset () {
      this.$nextTick(() => {
        this.$refs.addForm.reset()
        this.$refs.addForm.$refs.form.resetFields()
      })
      this.tableData = []
    },
    // closeAssetsDig () {
    //   this.assetsDig = false
    //   this.tableData = []
    // },
    /** 关闭按钮 */
    closeDialog () {
      this.open = false
      this.rowInfo = {}
    },
    /** 取消按钮 */
    cancel () {
      this.open = false
      this.rowInfo = {}
    },
    // 确认入库
    confirmWarehousingOrder(row) {
      this.$refs.addForm.submit()
    },

    /** 新建入库保存按钮 */
    submitForm () {
      this.$refs.addForm.submit()
      // this.getList()
    },
    /** 查看是否新建成功 */
    submitSuccess (payload) {
      if (payload.code === 200) {
        this.$modal.msgSuccess('新增成功')
        this.open = false
        this.getList()
      } else {
        this.$message.error(payload.msg)
      }
    },
    warehousingSuccess (payload) {
      if (payload.code === 200) {
        this.$modal.msgSuccess('修改成功')
        this.open = false
        // this.assetsDig = false
        this.getList()
      } else {
        this.$message.error(payload.msg)
      }
    },
    /** 入库是否成功 */
    sureSuccess (payload) {
      if (payload.code === 200) {
        this.$modal.msgSuccess('入库成功')
        this.open = false
        this.printDig = true
        this.bindCardData = payload.data || []
        this.getList()
      } else {
        this.$message.error(payload.msg)
      }
    },
    /** 添加明细 */
    add (payload) {
      this.tableData = []
      payload.forEach(item => {
        this.tableData.push(
          {
            materialId: item.materialId,
            materialCategory: item.materialCategory,
            materialCode: item.materialCode,
            brandName: item.brandName,
            materialStandard: item.materialStandard,
            unitName: item.unitName,
            planQty: item.purchaseQty,
            qty: item.qty,
            thisQty: item.thisQty || undefined,
            remark: item.remark,
            materialName: item.materialName
          })
      })
    },
    /** 增加明细*/
    addDetail (data) {
      this.tableData = []
      if (data.length === 0) {
        this.tableData = []
        return
      }
      data.forEach(item => {
        this.tableData.push(
          {
            materialId: item.id,
            materialCategory: item.categoryNameLevel3 ? item.categoryNameLevel3 : item.categoryNameLevel2 ? item.categoryNameLevel2 : item.categoryNameLevel1 ? item.categoryNameLevel1 : item.category,
            materialCode: item.code,
            materialName: item.name,
            brandName: item.brand,
            materialStandard: item.standard,
            unitName: item.unit,
            planQty: undefined,
            qty: 0,
            thisQty: undefined,
            remark: undefined
          })
      })
    },
    /** 生成资产卡片 */
    // createAssetCard () {
    //   // 生成资料卡片过滤数量0
    //   const arr = this.tableData.filter(item => {
    //     return item.thisQty !== 0
    //   })
    //
    //   const data = arr.map((e) => {
    //     return {
    //       id: e.materialId,
    //       num: Number(e.thisQty),
    //       price: Number(Number(e.price).toFixed(2)),
    //       taxAmount: Number(Number(e.taxAmount).toFixed(2)),
    //       warehousingOrderNo: this.formData.orderNo,
    //       warehouseId: this.formData.warehouseId// 入库时只能选择一个仓库
    //     }
    //   })
    //   const noNum = data.some(e => {
    //     return !e.num
    //   })
    //   if (noNum) {
    //     return this.$message.error('生成资产卡片数量须大于0')
    //   }
    //   this.btnDisabled = true
    //   createAssetCard(data).then((response) => {
    //     if (response.code === 200) {
    //       // this.$modal.msgSuccess('保存成功')
    //       this.$modal.msgSuccess('已成功生成资产卡片')
    //       this.assetsDig = false
    //       this.printDig = true
    //       // 获取绑卡页面数据
    //       const data = {
    //         pageNum: 1,
    //         pageSize: 1000000,
    //         warehousingOrderNo: this.formData.orderNo
    //       }
    //       getAssetByWarehousingOrderNo(data).then(res => {
    //         if (res.code === 200) {
    //           this.bindCardData = res.data.records || []
    //         }
    //
    //       })
    //       this.getList()
    //     }
    //   }).finally(() => {
    //     this.btnDisabled = false
    //   })
    // },
    /** 标签打印 */
    closePrintDig () {
      this.printDig = false
    }
  }
}
</script>
<style lang="scss" scoped>
// 主体div
.post-box {
  .btn-icon {
    width: 18px;
    height: 18px;
    vertical-align: middle;
  }
  .btn-text {
    vertical-align: middle;
  }
}
.statePoint .point {
  width: 8px;
  height: 8px;
  margin: 0 8px 2px 0;
}
.select-width {
  ::v-deep .el-input__inner {
    width: 456px;
  }
}
.detail {
  margin: 0 auto;
}
.detail-item {
  display: flex;
  .item-label {
    width: 128px;
    height: 16px;
    font-size: 16px;
    font-weight: 400;
    color: #6a7697;
    line-height: 24px;
    margin: 0 16px 16px 0;
    text-align: end;
  }
}
.add-form {
  display: flex;
  justify-content: space-between;
}
.input-width {
  ::v-deep .el-input__inner {
    width: 500px;
  }
}
::v-deep .el-dialog__body {
  padding:5px 15px;
  min-height: 600px;
  max-height: 800px;
}
// 弹出框底部样式
.dialog-footer {
  display: flex;
  justify-content: space-between;
}
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
.querybox {
  display: flex;
  justify-content: space-between;
  font-size: 14px;
  .queryform {
    width: 100%;
    display: flex;
    flex-wrap: wrap;
  }
  .showOrhide {
    cursor: pointer;
    display: flex;
    width: 50px;
    height: 40px;
    .showtext {
      padding-left: 5px;
    }
  }
}
.dialog-class{
  ::v-deep .el-dialog__body{
    max-height: 446px;
    overflow-y: auto;
  }
}
</style>

