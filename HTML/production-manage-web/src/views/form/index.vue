<template>
  <CommonTitleBar v-if="!showFlow">
    <template #titleBtn>
      <el-button
        :disabled="multipleSelection.length == 0"
        type="primary"
        size="small"
        @click="categorizeDialog()"
      >归类</el-button>
      <el-button
        type="primary"
        size="small"
        icon="el-icon-plus"
        @click="handleAddEdit()"
      >新建</el-button>
    </template>
    <el-card class="main-card task-card">
      <div class="app-container">
        <el-row :gutter="20">
          <el-col :span="4" :xs="24" style="border-right: 1px solid #eaedf4">
            <!-- 搜索框 -->
            <div class="head-container">
              <el-input
                v-model.trim="keywordName"
                placeholder="请输入关键字搜索"
                clearable
                size="small"
                suffix-icon="el-icon-search"
                style="margin-bottom: 20px; width: calc(100% - 10px)"
                maxlength="15"
              />
            </div>
            <div class="head-container" style="overflow: auto">
              <el-tree
                ref="tree"
                :current-node-key="currentNodeKey"
                node-key="id"
                :data="cProcessCategoryList"
                :props="defaultProps"
                :expand-on-click-node="false"
                :filter-node-method="filterNode"
                default-expand-all
                highlight-current
                @node-click="handleNodeClick"
              />
            </div>
          </el-col>
          <el-col :span="20" :xs="24" class="right-col">
            <div class="inputContent">
              <div>
                <el-form
                  ref="queryForm"
                  :model="queryParams"
                  :inline="true"
                  label-width="68px"
                >
                  <el-form-item prop="formName">
                    <el-input
                      v-model="queryParams.formName"
                      placeholder="请输入关键字查询"
                      clearable
                      size="small"
                      style="width: 180px"
                      @keyup.enter.native="handleQuery"
                    />
                  </el-form-item>

                  <el-form-item>
                    <el-button size="small" @click="resetQuery">重置</el-button>
                    <el-button
                      plain
                      type="primary"
                      size="small"
                      class="search-btn"
                      @click="handleQuery"
                    >搜索</el-button>
                  </el-form-item>
                </el-form>
              </div>
            </div>

            <el-table
              v-loading="loading"
              :data="formList"
              height="100%"
              @selection-change="handleSelectionChange"
            >
              <el-table-column type="selection" width="55" />
              <el-table-column
                label="序号"
                type="index"
                align="left"
                sortable
              />
              <el-table-column label="表单名称" align="left" prop="formName" />
              <el-table-column label="表单类别" align="left">
                <template slot-scope="scope">{{
                  returnCategory(scope.row.categoryId)
                }}</template>
              </el-table-column>
              <el-table-column
                label="创建人"
                align="left"
                prop="createNickname"
              />
              <el-table-column
                label="创建时间"
                align="left"
                prop="createTime"
              />
              <el-table-column
                label="操作"
                align="left"
                width="250"
                class-name="small-padding"
              >
                <template slot-scope="scope">
                  <el-button
                    size="small"
                    type="text"
                    @click="handleReview(scope.row)"
                  >详情</el-button>
                  <el-button
                    size="small"
                    type="text"
                    @click="handleAddEdit(scope.row, 2)"
                  >编辑</el-button>
                  <el-button
                    size="small"
                    type="text"
                    @click="handleAddEdit(scope.row, 1)"
                  >复制</el-button>
                  <el-button
                    size="small"
                    type="text"
                    style="color: #ef5826"
                    @click="handleDelete(scope.row)"
                  >删除</el-button>
                </template>
              </el-table-column>
            </el-table>
            <div class="common-pagination">
              <pagination
                v-show="total > 0"
                :total="total"
                :page.sync="queryParams.pageNum"
                :limit.sync="queryParams.pageSize"
                @pagination="getList"
              />
            </div>
          </el-col>
        </el-row>
      </div>
      <el-dialog
        title="详情"
        :visible.sync="vformDialog"
        destroy-on-close
        class="xq"
      >
        <collapse title="基本信息" style="font-family: SourceHanSansCN-Medium, SourceHanSansCN;font-weight: 500;color: #6A7697;line-height: 24px;">
          <el-descriptions>
            <el-descriptions-item label="表单名称">{{
              form1.formName
            }}</el-descriptions-item>
            <el-descriptions-item label="表单类型">
              {{ returnCategory(form1.categoryId) }}
            </el-descriptions-item>
            <!-- <el-descriptions-item label="显示顺序">
              {{ form1.orderNum }}
            </el-descriptions-item> -->
          </el-descriptions>
        </collapse>
        <collapse title="表单预览" style="font-family: SourceHanSansCN-Medium, SourceHanSansCN;font-weight: 500;color: #6A7697;line-height: 24px;">
          <v-form-render :form-json="formJson" />
        </collapse>
      </el-dialog>
      <!-- 归类弹窗 -->
      <el-dialog title="归类" :visible.sync="dialogVisible" width="300px">
        <el-select v-model="categoryId" clearable>
          <el-option
            v-for="(item, index) in processCategoryList"
            :key="index"
            :value="item.id"
            :label="item.categoryName"
          />
        </el-select>
        <span slot="footer" class="dialog-footer">
          <el-button
            size="small"
            @click="dialogVisible = false"
          >取 消</el-button>
          <el-button
            size="small"
            type="primary"
            @click="categorizeFun"
          >保 存</el-button>
        </span>
      </el-dialog>
    </el-card>
  </CommonTitleBar>
  <!-- 新建页面 -->
  <CommonTitleBar v-else class="step-bar">
    <template #secondTitle>
      <div class="step-box-second">({{ typeStr }})</div>
      <div class="step-box-left">{{ stepStr[active] }}</div>
    </template>
    <template #titleBtn>
      <el-button size="small" @click="back">返回</el-button>
      <template v-if="active === 1">
        <el-button
          size="small"
          type="primary"
          plain
          @click="active = 0"
        >上一步</el-button>
        <el-button type="primary" size="small" @click="save">保存</el-button>
        <!-- <el-button type="primary" size="small" @click="saveAndEnable">保存并启用</el-button> -->
      </template>
      <el-button
        v-if="active === 0"
        type="primary"
        plain
        size="small"
        @click="stepNext"
      >下一步</el-button>
    </template>
    <div v-show="active == 0" class="main-form">
      <BasicMessage
        slot="form"
        ref="basicmessage"
        :form-name="formName"
        @next="nextStep"
      />
    </div>
    <div v-show="active == 1" class="main-form-active2">
      <v-form-designer slot="section" ref="vforms" />
    </div>
  </CommonTitleBar>
</template>

<script>
// 表单管理页面
import {
  getFormList,
  addForm,
  updateForm,
  delForm,
  judgeFormBindDesign,
  formMove
} from '@/api/process'
import BasicMessage from './component/basicMessage.vue'
import { mapGetters } from 'vuex'
import CommonTitleBar from '../../components/common/common-title-bar.vue'
import collapse from '@/views/basicdata/material/component/collapse'
export default {
  components: { BasicMessage, CommonTitleBar, collapse },
  data () {
    return {
      form1: {},
      showFlow: false,
      currentNodeKey: -1, // 当前值
      clickKey: 0,
      // 左侧搜索框关键词名称
      keywordName: '',
      active: 0,
      formList: [],
      // 遮罩层
      loading: false,
      defaultProps: {
        children: 'children',
        label: 'categoryName'
      },
      // 查询参数
      queryParams: {
        pageNum: 1,
        pageSize: 10,
        formName: '',
        categoryId: ''
      },
      total: 0,
      formJson: {},
      vformDialog: false,
      formName: '',
      multipleSelection: [],
      dialogVisible: false,
      categoryId: '',
      stepStr: ['第一步:填写基本信息', '第二步:表单设计'],
      typeStr: '编辑'
    }
  },
  computed: {
    ...mapGetters(['processCategoryList']),
    cProcessCategoryList () {
      if (this.processCategoryList) {
        return [{ id: -1, categoryName: '全部' }, ...this.processCategoryList]
      }
      return []
    }
  },
  watch: {
    // 左侧根据名称筛选树
    keywordName (val) {
      this.$refs.tree.filter(val)
    }
  },
  // eslint-disable-next-line vue/order-in-components
  provide () {
    return {
      saveForm: this.saveForm
    }
  },
  created () {
    this.getList()
    this.showFlow = false
  },
  // activated() {
  //   this.showFlow = false
  // },
  methods: {
    // 重置
    resetQuery () {
      this.resetForm('queryForm')
      this.queryParams.categoryId = ''
      this.keywordName = ''
      this.$refs.tree.setCurrentKey(-1)
      this.handleQuery()
    },
    saveAndEnable () { },
    save () {
      const content = this.$refs.vforms.saveVFormNew()
      this.saveForm(content)
    },
    back () {
      this.showFlow = false
      this.active = 0
    },
    // 获取右边表单列表
    getList () {
      this.loading = true
      getFormList(this.queryParams).then((da) => {
        this.formList = da.data.records
        this.total = da.data.total
      }).finally(() => {
        this.loading = false
      })
    },
    // 基础表单完成
    nextStep (val) {
      this.active = 1
      this.basicMessage = val
    },
    saveForm (data) {
      const params = JSON.parse(JSON.stringify(this.basicMessage))
      params.formJson = data
      if (params.id) {
        updateForm(params).then(() => {
          this.$message.success('编辑成功')
          this.showFlow = false
          this.getList()
        })
      } else {
        addForm(params).then(() => {
          this.$message.success('新增成功!')
          this.showFlow = false
          this.getList()
        })
      }
    },
    // 下一步
    stepNext () {
      this.$refs.basicmessage.submitForm()
    },
    // 点击保存表单
    handleSubmit () {
      this.$refs.vforms.saveVForm()
    },
    // 筛选节点
    filterNode (value, data) {
      if (!value) return true
      return data.categoryName.indexOf(value) !== -1
    },
    // 节点单击事件
    handleNodeClick(data) {
      this.clickKey = data.id
      if (data.id === -1) {
        this.queryParams.categoryId = ''
      } else if (this.queryParams.categoryId === data.id) {
        this.queryParams.categoryId = ''
        this.$refs.tree.setCurrentKey(-1)
      } else {
        this.queryParams.categoryId = data.id
      }
      this.handleQuery()
    },
    /** 搜索按钮操作 */
    handleQuery () {
      this.queryParams.pageNum = 1
      this.getList()
    },
    /** 新增编辑按钮操作
     * type: undefined 直接新建
     *       1  以此为模板新建
     *       2  编辑
     */
    async handleAddEdit(row, type) {
      this.active = 0
      const params = {
        categoryId: '',
        formName: ''
        // orderNum: ''
      }
      this.formName = ''
      // 如果是编辑则判断是否已经被使用
      if (type === 2) {
        const da = await judgeFormBindDesign(row.id)
        // 判断是否绑定， 决定是否打开dialog
        if (da.data === true) {
          this.$message.warning('已与表单绑定,不能编辑')
          return
        }
      }
      // 是新增则直接打开
      this.showFlow = true
      // 存入当前选项 返回的时候返回当前选项
      if (this.showFlow === true) {
        // 存入currentNodeKey 返回直接拿到
        this.currentNodeKey = this.clickKey
      }
      this.typeStr = '新建'
      // 对于编辑或者以此为模板的新建
      if (type === 1 || type === 2) {
        if (type === 2) {
          // 以此为模板的新建则赋值
          params.id = row.id
          params.formName = row.formName
          this.typeStr = '编辑'
          this.formName = row.formName
        } else {
          params.formName = ''
        }
        params.categoryId = row.categoryId
        // params.orderNum = row.orderNum // 测试说不要+1

        await this.$nextTick()
        if (row.formJson === '{}') {
          this.$refs.vforms.editVForm()
        } else {
          this.$refs.vforms.editVForm(row.formJson)
        }
      } else {
        await this.$nextTick()
        this.$refs.vforms.editVForm()
      }
      this.$refs.basicmessage.form = params
    },
    // 预览
    handleReview (row) {
      this.formJson = JSON.parse(row.formJson)
      this.vformDialog = true
      this.form1 = row
    },
    /** 删除按钮操作 */
    handleDelete (row) {
      this.$confirm('是否确认删除该表单的数据项?', '确定删除', {
        confirmButtonText: '确定',
        cancelButtonText: '取消',
        type: 'warning'
      }).then(() => {
        delForm(row.id).then(() => {
          this.$message.success('删除成功')
          this.queryParams.pageNum = this.changePageNum(this.total, 1, this.queryParams.pageNum, this.queryParams.pageSize)
          this.getList()
        })
      })
        .catch(() => { })
    },
    returnCategory (id) {
      if (!this.processCategoryList || !id) return
      for (var i of this.processCategoryList) {
        if (i.id === id) {
          return i.categoryName
        }
      }
    },
    handleSelectionChange (val) {
      this.multipleSelection = val
    },
    /* 归类弹窗 */
    categorizeDialog () {
      this.categoryId = ''
      this.dialogVisible = true
    },
    /* 归类功能 */
    categorizeFun () {
      this.$confirm('确认修改？', '警告', {
        confirmButtonText: '确定',
        cancelButtonText: '取消',
        type: 'warning'
      }).then(() => {
        const ids = this.multipleSelection.map(m => { return m.id })
        formMove({ categoryId: this.categoryId, ids: ids.join(',') }).then(da => {
          this.dialogVisible = false
          this.getList()
          this.$message.success('归类成功!')
        })
      }
      )
    }

  }
}
</script>
<style lang="scss" scoped>
.step-bar {
  .step-box {
    display: flex;
    justify-content: space-between;
    width: 100%;
  }
  .step-box-left {
    font-size: 16px;
    color: #7d8592;
    margin-left: 16px;
  }
  .main-form {
    width: 1000px;
    margin: 16px auto;
    background-color: #fff;
    height: calc(100vh - 152px);
  }
  .main-form-active2 {
    margin-top: 16px;
    height: calc(100vh - 137px);
  }
}
::v-deep .el-tree-node:focus > .el-tree-node__content {
  background-color: #fff;
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
::v-deep .xq .el-dialog__title {
  font-size: 20px;
  font-family: SourceHanSansCN-Regular, SourceHanSansCN;
  font-weight: 400;
  color: #333333;
  line-height: 27px;
}
</style>
