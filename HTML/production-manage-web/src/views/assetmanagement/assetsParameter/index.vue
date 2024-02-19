<template>
  <div>
    <tags-view>
      <el-row :gutter="10" class="mb8">
        <el-col :span="1.5">
          <el-button
            v-hasPermi="['system:user:export']"
            plain
            size="small"
            @click="handleExport"
          ><i
            style="color: #4686ef"
            class="iconfont icon-daochu"
          />导出</el-button>
        </el-col>
        <el-col :span="1.5">
          <!--          <el-button-->
          <!--            v-hasPermi="['system:user:add']"-->
          <!--            type="primary"-->
          <!--            size="small"-->
          <!--            @click="showAddDig"-->
          <!--          ><i-->
          <!--            class="iconfont icon-zengjia"-->
          <!--            style="color: #ffffff"-->
          <!--          />新建卡片</el-button>-->
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
    <el-card class="main-card">
      <!-- 页面内容 -->
      <el-row>
        <el-col :span="22">
          <el-form
            ref="queryForm"
            :model="queryParams"
            :inline="true"
            label-width="68px"
            class="searchForm"
          >
            <el-form-item label="资产名称" prop="name">
              <el-input
                v-model.trim="queryParams.name"
                placeholder="请输入"
                clearable
                size="small"
                style="width: 200px"
                maxlength="15"
                @keyup.enter.native="handleQuery"
              />
            </el-form-item>
            <el-form-item label="入库单号" prop="warehousingOrderNo">
              <el-input
                v-model.trim="queryParams.warehousingOrderNo"
                placeholder="请输入"
                clearable
                size="small"
                style="width: 200px"
                maxlength="15"
                @keyup.enter.native="handleQuery"
              />
            </el-form-item>
            <el-form-item label="资产编码" prop="internalCode">
              <el-input
                v-model.trim="queryParams.internalCode"
                placeholder="请输入"
                clearable
                size="small"
                style="width: 200px"
                maxlength="20"
                @keyup.enter.native="handleQuery"
              />
            </el-form-item>
            <el-form-item label="资产状态" prop="status">
              <el-select
                v-model="queryParams.status"
                style="width: 130px"
                placeholder="请选择"
                clearable
                size="small"
              >
                <el-option
                  v-for="dict in dict.type.assets_enable_status"
                  :key="dict.value"
                  :label="dict.label"
                  :value="dict.value"
                />
              </el-select>
            </el-form-item>
            <el-form-item>
              <el-button size="small" @click="resetQuery">重置</el-button>
              <el-button
                size="small"
                class="search-btn"
                @click="handleQuery"
              >搜索</el-button>
            </el-form-item>
          </el-form>
        </el-col>
        <!--        <el-col :span="2">-->
        <!--          <el-button-->
        <!--            size="small"-->
        <!--            class="search-btn"-->
        <!--            @click="changeState"-->
        <!--          >设置为可领</el-button>-->
        <!--        </el-col>-->
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
          align="left"
          prop="index"
          type="index"
        />
        <el-table-column
          label="资产编码"
          align="left"
          prop="internalCode"
          :show-overflow-tooltip="true"
        />
        <el-table-column
          label="资产名称"
          align="left"
          prop="name"
          :show-overflow-tooltip="true"
        />
        <el-table-column
          label="品牌"
          align="left"
          prop="brandName"
          :show-overflow-tooltip="true"
        />
        <el-table-column
          label="规格型号"
          align="left"
          prop="standard"
          :show-overflow-tooltip="true"
        />
        <!-- <el-table-column label="闲置数量" align="left" prop="existNum" :show-overflow-tooltip="true" /> -->
        <el-table-column
          label="计量单位"
          align="center"
          prop="unit"
          :show-overflow-tooltip="true"
        />
        <el-table-column
          label="税前单价（元）"
          align="center"
          prop="price"
          :show-overflow-tooltip="true"
        />
        <el-table-column
          label="税额（元）"
          align="center"
          prop="taxAmount"
          :show-overflow-tooltip="true"
        />
        <el-table-column label="状态" align="left" prop="statusLabel">
          <!-- <template slot-scope="scope"> -->
          <!-- <el-tag>{{scope.row.status}}</el-tag> -->
          <!-- <el-tag v-if="scope.row.status === '0'" type="success">闲置</el-tag>
            <el-tag v-if="scope.row.status === '1'">领用中</el-tag>
            <el-tag v-if="scope.row.status === '2'" type="warning"
              >退库中</el-tag
            >
            <el-tag v-if="scope.row.status === '3'" type="info">使用</el-tag> -->
          <!-- </template> -->
        </el-table-column>
        <el-table-column
          label="使用部门"
          align="left"
          prop="dept"
          :show-overflow-tooltip="true"
        />
        <el-table-column
          label="使用人"
          align="left"
          prop="people"
          :show-overflow-tooltip="true"
        />
        <el-table-column
          label="存放仓库"
          align="left"
          prop="warehouseName"
          :show-overflow-tooltip="true"
        />
        <el-table-column label="操作" align="left" class-name="small-padding">
          <template slot-scope="scope">
            <el-button
              size="small"
              type="text"
              @click="handleDetail(scope.row)"
            >详情</el-button>
            <!--            <el-button-->
            <!--              v-if="scope.row.status === '0'"-->
            <!--              size="small"-->
            <!--              type="text"-->
            <!--              style="color: #ef5826"-->
            <!--              @click="handleDelete(scope.row)"-->
            <!--            >删除</el-button>-->
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
      <!-- 新建或编辑位置对话框 -->
      <!-- 选择明细 -->
      <el-dialog
        title="新建卡片"
        :visible.sync="open"
        width="1000px"
        append-to-body
        :close-on-click-modal="false"
        class="selectPlan"
        @close="closeAddDig"
      ><AddAssetsCard ref="addAssetsCard" />
        <div slot="footer" class="dialog-footer">
          <!-- <el-button size="mini" style="float:left" @click="addApply"><i class="iconfont icon-zengjia" />新增供应商</el-button> -->
          <el-button size="small" @click="closeAddDig">取消</el-button>
          <el-button
            v-debounce
            size="small"
            type="primary"
            @click="submitAdd"
          >生成资产卡片</el-button>
        </div></el-dialog>
      <!-- 详情 -->
      <el-dialog
        title="资产详情"
        :visible.sync="openDetail"
        width="1050px"
        append-to-body
        :close-on-click-modal="false"
        class="selectPlan"
        @close="closeDetailDig"
      ><AssetsDetail
         ref="assetsDetail"
         :base-form="baseForm"
         :use-records="useRecords"
       />
        <div
          slot="footer"
          class="dialog-footer"
        /></el-dialog>
    </el-card>
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
// insertOrUpdateFixedAsset,
import { getCanHandleAssetCard, getAssetDetailById, delassetCard, createAssetCard, changeAssetState } from '@/api/assetmanagement/assetsParameter'
import TagsView from '@/layout/components/TagsView/index.vue'
import AddAssetsCard from './component/add.vue'
import AssetsDetail from './component/detail.vue'
import '@riophae/vue-treeselect/dist/vue-treeselect.css'
export default {
  name: 'Place',
  components: { TagsView, AddAssetsCard, AssetsDetail },
  dicts: ['assets_enable_status'],
  data () {
    return {
      warningShow: false,
      // 遮罩层加载效果显示
      loading: true,
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
        name: undefined,
        internalCode: undefined,
        status: undefined,
        warehousingOrderNo: undefined
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
      openDetail: false,
      // 弹出层标题
      title: '',
      // 弹出框新建位置的表单数据
      placeForm: {
        parentCode: '',
        code: '',
        name: '',
        status: '',
        sort: 0,
        unit: ''
      },
      // 弹出框新建位置表单校验
      rules: {
        sort: [{ required: true, message: '请输入显示顺序', trigger: 'blur' }],
        name: [
          { required: true, message: '请填写资产名称', trigger: 'blur' }
        ],
        code: [{ required: true, message: '请填写资产', trigger: 'blur' }]
      },
      // 选择上级位置弹出框是否显示
      isShowPlace: false,
      // 选择上级位置弹出框名称筛选树
      dialogPlaceName: undefined,
      // 选择上级位置的对象
      preChoose: {},
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
      },
      // 使用记录
      useRecords: [],
      // 资产状态
      assetsEnableStatusOptions: []
    }
  },
  computed: {
  },
  watch: {
    // 左侧根据名称筛选树
    keywordName (val) {
      this.$refs.tree.filter(val)
    },
    // 新建选择上级位置筛选树
    dialogPlaceName (val) {
      this.$refs.placeTree.filter(val)
    }
  },
  created () {
    this.getList()
    this.getDicts('assets_enable_status').then(response => {
      this.assetsEnableStatusOptions = response.data
      //
    })
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
        '/biz/assetCard/export/',
        this.ids,
        `资产台账_${this.parseTime(new Date(), '{y}-{m}-{d}')}.xlsx`
      )
      this.warningShow = false
    },
    /** 顶部新增位置*/
    showAddDig () {
      // 重置新增表单
      this.reset()
      // 显示对话框
      this.open = true
      this.$nextTick(() => {
        this.$refs.addAssetsCard.getList()
      })
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
    /** 右侧查询list结构 */
    getList () {
      this.tableData = []
      this.loading = true
      getCanHandleAssetCard(this.addDateRange(this.queryParams)).then(
        (response) => {
          if (response.code === 200) {
            //
            this.loading = false
            this.tableData = response.data.records || []
            this.tableData.map(item => {
              item.ratePrice = item.price * (1 + item.rate)
              const filterStatus = this.assetsEnableStatusOptions.filter(item1 => item1.dictValue === item.status)[0] || ''
              item.statusLabel = filterStatus.dictLabel
            })
            this.total = response.data.total
            this.$refs.tableData.clearSelection()
          } else {
            this.$message.error(response.msg)
          }
        }
      )
    },
    // 右侧主体多选框选中数据
    handleSelectionChange (selection) {
      this.selectedLength = selection.length
      this.ids = []
      this.arrCheckIds = []
      selection.forEach(item => {
        this.ids.push(item.id)
        if (item.status === '1') {
          this.arrCheckIds.push(item.id)
        }
      })
      this.single = selection.length !== 1
      this.multiple = !selection.length
    },
    /** 右侧主体详情和编辑操作 */
    handleDetail (row) {
      // 无论是详情还是编辑都打开对话框
      this.openDetail = true
      getAssetDetailById({ id: row.id }).then(
        (response) => {
          if (response.code === 200) {
            this.baseForm = response.data.assetCard
            this.baseForm.status = String(this.baseForm.status)
            this.useRecords = response.data.assetOperationVos
            this.$nextTick(() => {
              this.$refs.assetsDetail.getForm()
            })
          } else {
            this.$message.error(response.msg)
          }
        }
      )
    },
    /** 右侧主体删除操作 */
    handleDelete (row) {
      const { id, name } = row
      this.$modal
        .confirm('确定删除"' + name + '"?', '确定删除')
        .then(function () {
          return delassetCard(id)
        })
        .then(() => {
          this.queryParams.pageNum = this.changePageNum(this.total, 1, this.queryParams.pageNum, this.queryParams.pageSize)
          this.getList()
          this.$modal.msgSuccess('删除成功')
        })
        .catch(() => { })
    },
    /** 弹出框新建位置表单重置 */
    reset () {
      // this.$nextTick(() => {
      //   this.$refs.addAssetsCard.clearSelected()
      // })
    },
    /** 弹出框保存按钮 */
    submitAdd () {
      const arr = this.$refs.addAssetsCard.selectedData
      if (arr.length === 0) {
        return this.$message.error('请选择要生成资产卡片的物料')
      }
      const data = arr.map((e) => {
        return {
          id: e.id,
          num: Number(e.assetCardNum),
          canGetNum: Number(e.canGetNum),
          warehouseId: e.warehouseId
        }
      })
      const noNum = data.some(e => {
        // return !e.num || !e.canGetNum
        return !e.num
      })
      if (noNum) {
        return this.$message.error('数量不能为空')
      }
      createAssetCard(data).then((response) => {
        if (response.code === 200) {
          this.$modal.msgSuccess('保存成功')
          this.closeAddDig()
          this.getList()
        }
      })
    },
    /** 弹出对话框关闭 */
    closeAddDig () {
      this.open = false
      this.$refs.addAssetsCard.$refs.table.clearSelection()
      this.$refs.addAssetsCard.$refs.table.selectedData = []
    },
    closeDetailDig () { },
    /** 弹出框显示上级位置选项 */
    showPrePlaceDialog () {
      this.isShowPlace = true
    },
    /** 弹出框保存选择的上级位置处理 */
    handleClickPre (data) {
      this.preChoose = data
    },
    /** 弹出框保存选择的上级位置 */
    saveChoosePlace () {
      // 弹出框隐藏
      this.isShowPlace = false
      this.placeForm.parentCode = this.preChoose.parentCode
      if (!this.preChoose.code) {
        this.$message.error('请选择上级')
        return
      }
    },
    // 设置为可领
    async changeState () {
      try {
        // const filterIds = this.ids.filter(item => )
        await changeAssetState([...this.arrCheckIds])
        this.getList()
      } catch (err) {
        this.$message.error(err)
        return
      }
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
</style>
