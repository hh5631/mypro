<template>
  <div class="post-box">
    <!-- header -->
    <tags-view>
      <el-row :gutter="10" type="flex">
        <!-- 打印 -->
        <el-col :span="1.5">
          <el-button
            v-hasPermi="['assetManagement:assetDisposal:print']"
            size="small"
            @click="handlePrint"
          >
            <i
              class="el-icon-printer"
              style="font-size: 14px;"
            />
            <span class="btn-text">打印</span>
          </el-button>
        </el-col>
        <!-- 导出 -->
        <el-col :span="1.5">
          <el-button
            v-hasPermi="['assetManagement:assetDisposal:export']"
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
        <!-- 新建 -->
        <el-col :span="1.5">
          <el-button
            v-hasPermi="['assetManagement:assetDisposal:addOrUpdate']"
            type="primary"
            size="small"
            @click="handleAdd"
          >
            <i
              class="iconfont icon-zengjia"
              style="font-size: 14px; margin-right: 8px"
            />
            <span class="btn-text">新建处置</span>
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
      <!-- 搜索栏 -->
      <searchForm @search="handleQuery" @reset="resetQuery" />
      <!-- 展示table -->
      <showTable
        :loading="loading"
        :asset-list="assetList"
        @selectChange="handleSelectionChange"
        @detail="handleDetail"
        @edit="handleEdit"
        @deleted="handleDelete"
      />
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
    <!-- 新建/编辑/详情 弹窗 -->
    <addOrEditDialog
      ref="addOrEditDialog"
      :open="open"
      :title="title"
      :is-edit="isEdit"
      :is-disabled="isDisabled"
      :add-time-now="addTimeNow"
      @closedialog="handleCloseDialog"
      @success="handleSuccess"
    />
    <!-- 打印预览 -->
    <FilePrint ref="FilePrint" type="assetDisposalOrder" />
  </div>
</template>
<script>
import searchForm from './components/searchForm.vue'
import showTable from './components/showTable.vue'
import addOrEditDialog from './components/addOrEditDialog.vue'
import TagsView from '@/layout/components/TagsView/index.vue'
import FilePrint from '@/components/FilePrint'
import { getAssetHandleList, getAssetHandleById, deleteAssetHandle } from '@/api/assetmanagement/assetDisposal.js'
import { dayjsFormat } from '@/utils/index'
export default {
  name: 'Post',
  components: { TagsView, searchForm, showTable, addOrEditDialog, FilePrint },
  data () {
    return {
      // 已选数量
      // selectedLength: 0,
      // 加载效果
      loading: true,
      // 分页总条数
      total: 0,
      // 表格数据
      assetList: [],
      // 页面初始查询表单对象
      queryParams: {
        pageNum: 0,
        pageSize: 10,
        getTime: []
      },
      // 处置时间段
      getTime: [],
      // 展示列表选中数据条数
      ids: [],
      // 新增弹窗标题
      title: '',
      open: false,
      isEdit: false,
      isDisabled: false,
      // 选择明细弹框显示隐藏
      openDetail: false,
      show: false,
      processInstanceId: undefined,
      // 新增时自动获取当前时间
      addTimeNow: undefined
    }
  },
  created () {
    this.getList()
  },
  methods: {
    /** 查询列表 */
    getList () {
      this.loading = true
      getAssetHandleList({
        ...this.queryParams,
        startTime: this.queryParams.getTime[0],
        endTime: this.queryParams.getTime[1]
      }).then(res => {
        if (res.code === 200) {
          this.assetList = res.data.records
          this.total = res.data.total
          this.loading = false
        } else {
          this.$message.error(res.msg)
        }
      })
    },
    /** 导出 */
    handleExport () {
      this.download(
        'biz/assetHandle/export',
        this.ids,
        `资产处置_${this.parseTime(new Date(), '{y}-{m}-{d}')}.xlsx`
      )
    },
    /** 新增 */
    handleAdd () {
      this.open = true
      this.title = '新建处置'
      this.isEdit = true
      this.isDisabled = false
      this.addTimeNow = dayjsFormat(new Date().getTime()).split(' ')[0]
      // 清空表单
      this.$nextTick(() => {
        this.$refs.addOrEditDialog.reset()
      })
    },
    /** 刷新 */
    refresh () {
      this.getList()
    },
    /** 搜索按钮操作 */
    handleQuery (payload) {
      this.queryParams = { ...payload }
      this.getList()
    },
    /** 重置按钮操作 */
    resetQuery(payload) {
      this.handleQuery(payload)
    },
    /** 列表选中数据导出 */
    handleSelectionChange(payload) {
      this.ids = payload.map(item => item.id)
    },
    /** 查看按钮操作 */
    handleDetail (payload) {
      this.open = true
      this.title = '详情'
      this.isEdit = false
      this.isDisabled = true
      this.$nextTick(() => {
        // 展示审批流程的数据
        this.processInstanceId = payload.processInstanceId
        this.$refs.addOrEditDialog.$refs.approvalProcess.processInstanceId = payload.processInstanceId
        if (this.processInstanceId) {
          this.$refs.addOrEditDialog.$refs.approvalProcess.getList(payload.processInstanceId)
        }
      })
    },
    /** 编辑按钮操作 */
    handleEdit (row) {
      this.open = true
      this.title = '编辑'
      this.isEdit = true
      this.isDisabled = false
    },
    /** 删除按钮操作 */
    handleDelete (payload) {
      const { id, orderNo } = payload
      this.$modal.confirm('确定删除处置单"' + orderNo + '"？', '确定删除').then(function () {
        return deleteAssetHandle(id)
      }).then((res) => {
        if (res.code === 200) {
          this.queryParams.pageNum = this.changePageNum(this.total, 1, this.queryParams.pageNum, this.queryParams.pageSize)
          this.getList()
          this.$modal.msgSuccess('删除成功')
        } else {
          this.$message.error(res.msg)
        }
      }).catch(() => { })
    },
    /** 关闭弹窗 */
    handleCloseDialog(payload) {
      if (payload) {
        this.open = false
      }
    },
    /** 新建或修改成功关闭弹窗 */
    handleSuccess(payload) {
      if (payload.code === 200) {
        this.open = false
        this.getList()
      }
    },
    /** 打印 */
    handlePrint () {
      if (!this.ids.length) {
        return this.$message.warning('请勾选要打印的项')
      }
      this.$modal.loading('正在获取打印数据，请稍候...')
      Promise.all(this.ids.map(item => getAssetHandleById(item))).then(res => {
        console.log('res', res)
        this.$modal.closeLoading()
        this.$refs.FilePrint.show(res.map(item => item.data))
      })
    }
  }
}
</script>
<style lang="scss" scoped>
.post-box {
  .btn-icon {
    width: 18px;
    height: 18px;
    vertical-align: middle;
  }
  .btn-text {
    vertical-align: middle;
  }
  // .common-pagination {
  //   display: flex;
  //   justify-content: flex-end;
  //   align-items: center;
  //   border-top: 1px solid #eaedf4;
  //   margin-top: 6px;
  //   .chooseCount {
  //     color: #7d8592;
  //     margin-top: 30px;
  //     margin-right: 360px;
  //   }
  // }
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
.noIcon {
  ::v-deep .el-input__icon {
    display: none;
  }
}
.selectInput {
  margin-right: 20px;
}
.detail {
  margin: 0 auto;
}
.detail-item {
  display: flex;
  font-size: 16px;
  font-weight: 400;
  line-height: 24px;
  .item-label {
    width: 128px;
    height: 16px;
    color: #6a7697;
    margin: 0 16px 16px 0;
    text-align: end;
  }
}
.logo {
  ::v-deep .el-upload--picture-card {
    width: 108px;
    height: 88px;
    display: flex;
    justify-content: center;
    align-items: center;
  }
}
.contentTable {
  padding-top: 16px;
}

.selectLeader {
  width: 100%;
  height: 100%;
  padding: 16px;
  overflow-y: scroll;
}
</style>
