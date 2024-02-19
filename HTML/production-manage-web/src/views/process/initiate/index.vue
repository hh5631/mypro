<template>
  <CommonTitleBar>
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
                style="margin-bottom: 20px;width: calc(100% - 10px);"
                maxlength="15"
              />
            </div>
            <div class="head-container">
              <el-tree
                ref="tree"
                :current-node-key="currentNodeKey"
                node-key="id"
                :data="cProcessCategoryList"
                :props="defaultProps"
                :filter-node-method="filterNode"
                :expand-on-click-node="false"
                default-expand-all
                highlight-current
                @node-click="handleNodeClick"
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
                  <el-form-item prop="processName">
                    <el-input
                      v-model="queryParams.processName"
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
            <!-- table表格 -->
            <el-table v-loading="loading" :data="list" height="100%">
              <el-table-column type="index" align="left" />
              <el-table-column
                label="流程名称"
                align="left"
                prop="processName"
              />
              <el-table-column
                label="流程类别"
                align="left"
                prop="categoryName"
              />
              <el-table-column
                label="创建时间"
                align="left"
                prop="createTime"
              />
              <el-table-column align="left" label="操作" width="200">
                <template slot-scope="scope">
                  <el-button
                    size="small"
                    type="text"
                    @click="startDialog(scope.row)"
                  >发起任务
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
      <!-- 发起流程弹框 -->
      <el-dialog
        title="发起流程"
        :visible.sync="dialogFormVisible"
        destroy-on-close
        width="80%"
      >
        <v-form-render
          ref="vFormRef"
          :form-json="formJson"
          :data-json="dataJson"
        />
        <div slot="footer" class="dialog-footer">
          <el-button size="small" @click="cancel()">取 消</el-button>
          <el-button
            type="primary"
            size="small"
            @click="start('vFormRef')"
          >保 存</el-button>
        </div>
      </el-dialog>
    </el-card>
  </CommonTitleBar>

</template>

<script>
import { getFlowLaunchList, getFormDetail } from '@/api/process'
import { startProcess } from '@/utils/process'
import { mapGetters } from 'vuex'
import CommonTitleBar from '../../../components/common/common-title-bar.vue'
export default {
  components: { CommonTitleBar },
  data() {
    return {
      keywordName: '',
      currentNodeKey: -1,
      list: [],
      total: 0,
      queryParams: {
        pageNum: 1,
        pageSize: 10,
        processName: null,
        categoryId: ''
      },
      dialogFormVisible: false,
      formJson: {},
      dataJson: {},
      loading: false,
      defaultProps: {
        children: 'children',
        label: 'categoryName'
      },
      uploadUrl: process.env.VUE_APP_BASE_API + '/file/upload' // 上传的图片服务器地址
    }
  },
  computed: {
    ...mapGetters(['processCategoryList']),
    cProcessCategoryList() {
      if (this.processCategoryList) {
        return [{ id: -1, categoryName: '全部' }, ...this.processCategoryList]
      }
      return []
    }
  },
  watch: {
    // 左侧根据名称筛选树
    keywordName(val) {
      this.$refs.tree.filter(val)
    }
  },
  created() {
    this.getList()
  },
  methods: {
    /** 重置按钮操作 */
    resetQuery () {
      this.resetForm('queryForm')
      this.queryParams.categoryId = ''
      this.keywordName = ''
      this.$refs.tree.setCurrentKey(-1)
      this.handleQuery()
    },
    filterNode(value, data) {
      if (!value) return true
      return data.categoryName.indexOf(value) !== -1
    },
    // 获取列表数据
    getList() {
      this.loading = true
      getFlowLaunchList(this.queryParams)
        .then((res) => {
          this.list = res.data.records
          this.total = res.data.total
        })
        .finally(() => {
          this.loading = false
        })
    },
    // 节点单击事件
    handleNodeClick(data) {
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
    handleQuery() {
      this.queryParams.pageNum = 1
      this.getList()
    },
    startDialog(row) {
      this.processKey = row.processKey
      this.formId = row.formId
      this.bindId = row.id
      this.dialogFormVisible = true
      getFormDetail(row.formId).then((da) => {
        this.formJson = JSON.parse(da.data.formJson) || {}
        // 添加默认上传地址
        this.formJson.widgetList.forEach(item => {
          if (item.type === 'picture-upload' || item.type === 'file-upload') {
            if (!item.options.uploadURL) {
              item.options.uploadURL = this.uploadUrl
            }
          }
        })
        this.$refs.vFormRef.setFormJson(this.formJson)
      })
    },
    start(ref) {
      startProcess(this, ref, {
        processKey: this.processKey,
        formId: this.formId,
        bindId: this.bindId
      }).then(() => {
        this.dialogFormVisible = false
      })
    },
    handleSizeChange(val) {
      this.queryParams.pageNum = val
      this.getList()
    },
    handleCurrentChange(val) {
      this.queryParams.pageSize = val
      this.getList()
    },
    cancel() {
      this.dialogFormVisible = false
    }
  }
}
</script>

<style lang="scss" scoped>
// .layui-badge {
//   width: auto;
//   height: 60px;
// }
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

</style>
