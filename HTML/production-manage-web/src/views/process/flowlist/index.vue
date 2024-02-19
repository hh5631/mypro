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
          <!---->
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
            <div class="head-container">
              <el-tree
                ref="tree"
                :current-node-key="currentNodeKey"
                node-key="id"
                :filter-node-method="filterNode"
                :data="cProcessCategoryList"
                :props="defaultProps"
                :expand-on-click-node="false"
                default-expand-all
                highlight-current
                @node-click="handleNodeClick"
                @current-change="currentChange"
              />
            </div>
          </el-col>
          <!---->
          <el-col :span="20" :xs="24" class="right-col">
            <div class="inputContent">
              <div>
                <el-form
                  ref="queryForm"
                  :model="queryParams"
                  :inline="true"
                  label-width="68px"
                >
                  <el-form-item label="流程名称" prop="name">
                    <el-input
                      v-model="queryParams.name"
                      placeholder="请输入"
                      clearable
                      size="small"
                      :show-overflow-tooltip="true"
                      style="width: 180px"
                      maxlength="15"
                      @keyup.enter.native="handleQuery"
                    />
                  </el-form-item>
                  <el-form-item label="创建人" prop="createNickname">
                    <el-input
                      v-model="queryParams.createNickname"
                      placeholder="请输入"
                      clearable
                      size="small"
                      style="width: 180px"
                      maxlength="15"
                      @keyup.enter.native="handleQuery"
                    />
                  </el-form-item>
                  <el-form-item label="流程类型" prop="type">
                    <el-select
                      v-model="queryParams.type"
                      filterable
                      size="small"
                      placeholder="请选择"
                      style="width: 180px"
                      clearable
                    >
                      <el-option
                        v-for="item in typeData"
                        :key="item.value"
                        :label="item.label"
                        :value="item.value"
                      />
                    </el-select>
                  </el-form-item>

                  <el-form-item>
                    <el-button size="small" @click="resetQuery">重置</el-button>
                    <el-button
                      plain
                      type="primary"
                      size="small"
                      class="search-btn"
                      @click="handleQuery"
                    >搜索
                    </el-button>
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
              <el-table-column
                label="流程名称"
                align="left"
                prop="processName"
                :show-overflow-tooltip="true"
              />
              <el-table-column label="版本号" align="left" prop="version" />
              <el-table-column label="流程类型" align="left">
                <template slot-scope="scope">
                  {{ scope.row.type === "0" ? "自定义流程" : "业务流程" }}
                </template>
              </el-table-column>
              <el-table-column label="流程类别" align="left">
                <template slot-scope="scope">
                  {{ returnCategory(scope.row.categoryId) }}
                </template>
              </el-table-column>
              <el-table-column
                label="创建人"
                align="left"
                prop="createNickname"
              />
              <el-table-column
                label="创建时间"
                align="left"
                prop="deploymentTime"
              />
              <el-table-column
                v-if="type === 1"
                :key="bizName"
                label="业务名称"
                align="left"
                prop="bizName"
              />
              <el-table-column
                v-if="type === 1"
                :key="bizType"
                label="业务类型"
                align="left"
                prop="bizType"
              />
              <el-table-column
                label="启用/停用"
                align="left"
                prop="suspendState"
              >
                <template slot-scope="scope">
                  <el-switch
                    v-model="scope.row.suspendState"
                    :active-value="1"
                    :inactive-value="2"
                    @change="suspendOrActiveApply(scope.row)"
                  />
                </template>
              </el-table-column>
              <el-table-column
                label="操作"
                align="left"
                width="240"
                class-name="small-padding"
              >
                <template slot-scope="scope">
                  <el-button
                    size="small"
                    type="text"
                    @click="handlePreview(scope.row)"
                  >详情
                  </el-button>
                  <el-button
                    size="small"
                    type="text"
                    @click="handleAddEdit(scope.row, 2)"
                  >编辑
                  </el-button>
                  <!--     <el-button-->
                  <!--     size="small"-->
                  <!--     type="text"-->
                  <!--     @click="suspendOrActiveApply(scope.row)"-->
                  <!--     >{{ scope.row.suspendState == 1 ? "挂起" : "激活" }}-->
                  <!--     </el-button>-->
                  <el-button
                    size="small"
                    type="text"
                    @click="handleAddEdit(scope.row, 1)"
                  >复制
                  </el-button>
                  <el-button
                    size="small"
                    type="text"
                    style="color: #ef5826"
                    @click="handleDelete(scope.row)"
                  >删除
                  </el-button>
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
      <flow-preview ref="flowpreview" />
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
        <el-button
          type="primary"
          size="small"
          :disabled="saveDisabled"
          @click="handleSubmit"
        >保存</el-button>
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
      <flow-basic-message
        v-show="active == 0"
        slot="form"
        ref="basicMessage"
        @finish="finishBasicMessage"
      />
    </div>
    <div v-show="active == 1" class="main-form-active2">
      <flow-designer
        v-show="active == 1"
        slot="section"
        ref="flowdesigner"
        :process-name="processName"
        :form-json="formJson"
        @bpmn-module-change="getModuleLength"
      />
    </div>
  </CommonTitleBar>
</template>

<script>
// 流程列表页面
import {
  getFlowList,
  addFlow,
  deleteFlow,
  activeFlow,
  processMove
} from '@/api/process'
import flowBasicMessage from '../component/flowBasicMessage'
import FlowDesigner from '../component/design'
import FlowPreview from '../component/preview.vue'
import { mapGetters } from 'vuex'
import CommonTitleBar from '../../../components/common/common-title-bar.vue'
// import { selectLikeCountByProcessName } from '@/api/process'
export default {
  components: { flowBasicMessage, FlowDesigner, FlowPreview, CommonTitleBar },
  data () {
    return {
      showFlow: false,
      keywordName: '',
      currentNodeKey: -1,
      active: 0,
      // componentName: 'basicMessage',
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
        name: undefined,
        createNickname: undefined,
        categoryId: '',
        type: undefined
      },
      type: undefined,
      typeData: [{
        value: 0,
        label: '自定义流程'
      },
      {
        value: 1,
        label: '业务流程'
      }],
      total: 0,
      formJson: {}, // 发起表单
      vformDialog: false,
      basicMessage: {},
      processName: '', // 流程名称
      multipleSelection: [],
      dialogVisible: false,
      categoryId: '',
      tabList: ['自定义流程', '业务流程'],
      stepStr: ['第一步:填写基本信息', '第二步:流程设计'],
      typeStr: '新建',
      saveDisabled: false,
      node: undefined
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
  created () {
    this.getList()
    // console.log(this.processCategoryList)
    // console.log("...mapGetters(['processCategoryList'])", this.cProcessCategoryList)
  },
  activated () {
    this.showFlow = false
  },
  // activated() {
  //   this.showFlow = false
  // },
  methods: {
    filterNode (value, data) {
      if (!value) return true
      return data.categoryName.indexOf(value) !== -1
    },
    async getList () {
      this.loading = true
      try {
        const res = await getFlowList(this.queryParams)
        this.formList = res.data.records
        this.total = res.data.total
        //
        this.type = this.queryParams.type ? this.queryParams.type : null
      } finally {
        this.loading = false
      }
    },
    // 基础信息完成
    finishBasicMessage (val, formJson) {
      this.active = 1
      this.basicMessage = { ...this.basicMessage, ...val }
      this.processName = val.processName
      this.formJson = formJson
      // 设置bpmn流程名称
      const keyElement = window.bpmnInstances.elementRegistry.find(
        (el) => el.type === 'bpmn:Process'
      )
      window.bpmnInstances.modeling.updateProperties(keyElement, {
        name: val.processName
      })
      // console.log('keyElement', keyElement)
      // console.log('window.bpmnInstances', window.bpmnInstances)
    },
    // 新增保存
    async saveBpmn (stringBPMN) {
      const params = JSON.parse(JSON.stringify(this.basicMessage))
      const keyElement = window.bpmnInstances.elementRegistry.find(
        (el) => el.type === 'bpmn:Process'
      )
      params.processKey = keyElement.id
      // eslint-disable-next-line no-undef
      params.base64StringBPMN = this.$Base64.encode(stringBPMN)
      await addFlow(params)
      this.$message.success(
        this.typeStr === '编辑' ? '流程编辑成功' : '流程新建成功'
      )
      this.showFlow = false
      this.getList()
    },
    // 上一步
    stepAhead () {
      this.active = 0
    },
    // 下一步
    stepNext () {
      this.$refs.basicMessage.submitForm()
    },
    // 点击保存流程
    async handleSubmit () {
      const res = await this.$refs.flowdesigner.saveMyBpmn()
      this.saveBpmn(res)
    },
    // 节点单击事件
    handleNodeClick (data) {
      this.node = data.id
      if (data.id === -1) {
        this.queryParams.categoryId = ''
      } else if (this.queryParams.categoryId === data.id) {
        this.queryParams.categoryId = ''
        this.$refs.tree.setCurrentKey(-1)// 设置选择节点
      } else {
        this.queryParams.categoryId = data.id
      }
      this.handleQuery()
    },

    currentChange(val) {
      // console.log('执行2', val)
      // setCurrentKey(val.id)
    },
    /** 搜索按钮操作 */
    handleQuery () {
      this.queryParams.pageNum = 1
      this.getList()
    },
    /** 重置按钮操作 */
    resetQuery () {
      this.keywordName = ''
      this.resetForm('queryForm')
      this.handleQuery()
    },
    back () {
      this.showFlow = false
      this.active = 0
      // this.$refs.tree.setCurrentKey(this.node)
      this.currentNodeKey = this.node
    },
    /** 新增编辑按钮操作 */
    async handleAddEdit (row, type) {
      this.typeStr = type === 2 ? '编辑' : '新建'
      this.active = 0
      this.showFlow = true
      await this.$nextTick()
      // type =1新增或复制
      if (row) {
        row.bpmnType = type
      }

      if ((type === 1 || type === 2) && row) {
        // 复制或者编辑获取基础信息
        this.$refs.basicMessage.init(row, type)
      }

      // if (type === 2) {
      // 获取详情信息
      this.$refs.flowdesigner.initDiagram(row, type)
      // }
    },
    /* 预览 */
    handlePreview (row) {
      // this.$refs.flowpreview.initDiagram()
      this.$refs.flowpreview.initDiagram(row)
    },
    /** 删除按钮操作 */
    handleDelete (row) {
      this.$modal
        .confirm('是否确认删除该表单的数据项？', '确定删除')
        .then(() => {
          deleteFlow(row.deploymentId).then((da) => {
            this.$message.success(da.msg || '删除成功')
            if (this.formList.length === 1 && this.queryParams.pageNum > 1) {
              this.queryParams.pageNum -= 1
            }
            this.getList()
          })
        })
        .catch(() => { })
    },
    /* 激活挂起 */
    suspendOrActiveApply (row) {
      // var suspendOrActive = row.suspendState === 2 ? '激活' : '挂起'
      // this.$confirm('确认要' + suspendOrActive + '流程定义吗?', '警告', {
      //   confirmButtonText: '确定',
      //   cancelButtonText: '取消',
      //   type: 'warning'
      // }).then(() => {
      //   this.$modal.loading(`${suspendOrActive}中，请稍候...`)
      //   activeFlow(row.id)
      //     .then((da) => {
      //       this.$message.success(da.msg || '操作成功')
      //       this.getList()
      //     })
      //     .finally(this.$modal.closeLoading())
      //     .catch(function() {
      //     })
      // })
      var suspendOrActive = row.suspendState === 1 ? '启用' : '停用'
      this.$modal.loading(`${suspendOrActive}中，请稍候...`)
      activeFlow(row.id)
        .then((da) => {
          this.$message.success(da.msg || `${suspendOrActive}成功`)
          this.getList()
        })
        .finally(this.$modal.closeLoading())
        .catch(function () { })
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
        const ids = this.multipleSelection.map((m) => {
          return m.bindId
        })
        processMove({ categoryId: this.categoryId, ids: ids.join(',') }).then(
          (da) => {
            this.dialogVisible = false
            this.getList()
            this.$message.success(da.msg || '操作成功')
          }
        )
      })
    },
    // bpmn图形修改
    getModuleLength (length) {
      if (length > 1) {
        this.saveDisabled = false
      } else {
        this.saveDisabled = true
      }
    }
  }
}
</script>
<style lang="scss" scoped>
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
.step-bar {
  .main-form {
    width: 1000px;
    margin: 16px auto;
    background-color: #fff;
    height: calc(100vh - 152px);
  }
  .main-form-active2 {
    margin: 16px;
    height: calc(100vh - 152px);
  }
  .step-box-left {
    font-size: 16px;
    color: #7d8592;
    margin-left: 16px;
  }
}
</style>
