<template>
  <div>
    <tags-view>
      <el-row :gutter="10" class="mb8">
        <el-col :span="1.5">
          <el-button
            v-hasPermi="['system:user:add']"
            type="primary"
            size="small"
            @click="setDepreciation"
          >设置折旧</el-button>
        </el-col>
        <el-col :span="1.5">
          <el-button
            v-hasPermi="['system:post:export']"
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
            v-hasPermi="['system:post:export']"
            plain
            size="small"
            @click="handleDownloadTemp"
          ><i
            style="color: #4686ef"
            class="iconfont icon-daoru"
          />下载模板</el-button>
        </el-col>
        <el-col :span="1.5">
          <el-upload
            ref="upload"
            :limit="1"
            accept=".xlsx, .xls"
            :headers="upload.headers"
            :before-upload="beforeUpload"
            :action="upload.url + '?updateSupport=' + upload.updateSupport"
            :disabled="upload.isUploading"
            :on-progress="handleFileUploadProgress"
            :on-success="handleFileSuccess"
            :show-file-list="false"
          >
            <el-button
              v-hasPermi="['system:material:import']"
              plain
              size="small"
            ><i
              style="color: #4686ef"
              class="iconfont icon-daoru"
            />导入</el-button>
          </el-upload>
        </el-col>
        <el-col :span="1.5">
          <el-button
            v-hasPermi="['system:user:add']"
            type="primary"
            size="small"
            @click="handleAdd"
          ><i
            class="iconfont icon-zengjia"
            style="color: #ffffff"
          />新增</el-button>
        </el-col>
      </el-row>
    </tags-view>
    <el-card class="main-card-left">
      <el-row :gutter="20">
        <!--位置左侧tree数据-->
        <el-col
          :span="4"
          :xs="24"
          style="border-right: 1px solid #eaedf4; padding-right: 0px"
        >
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
          <!-- 树形组件 -->
          <div class="head-container left-tree">
            <el-tree
              ref="tree"
              current-node-key="0000000"
              :default-expanded-keys="['0000000']"
              :props="defaultProps"
              :data="placeOptions"
              :expand-on-click-node="false"
              :filter-node-method="filterNode"
              highlight-currentclearable
              node-key="code"
              @node-click="handleNodeClick"
            />
          </div>
        </el-col>
        <!--位置右侧list数据-->
        <el-col :span="20" :xs="24" class="right-card">
          <el-form
            ref="queryForm"
            :model="queryParams"
            :inline="true"
            label-width="68px"
            class="searchForm"
          >
            <el-form-item label="物料编码" prop="code">
              <el-input
                v-model.trim="queryParams.code"
                placeholder="请输入"
                clearable
                size="small"
                style="width: 200px"
                maxlength="15"
                @keyup.enter.native="handleQuery"
              />
            </el-form-item>
            <el-form-item label="物料名称" prop="name">
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
            <!-- <el-form-item label="状态" prop="enable">
              <el-select v-model="queryParams.enable" style="width: 200px" placeholder="请选择" clearable size="small">
                <el-option
                  v-for="dict in dict.type.enable_status"
                  :key="dict.value"
                  :label="dict.label"
                  :value="dict.value"
                />
              </el-select>
            </el-form-item> -->
            <el-form-item>
              <el-button size="small" @click="resetQuery">重置</el-button>
              <el-button
                size="small"
                type="primary"
                class="search-btn"
                plain
                @click="handleQuery"
              >搜索</el-button>
            </el-form-item>
          </el-form>
          <!-- 表格显示list数据 -->
          <MaterialTable
            v-loading="loading"
            :material-table="materialTable"
            :total="total"
            :query-params="queryParams"
            @selectOptions="selectOptions"
            @changePage="changePage"
            @changeStatus="changeStatus"
            @update="handleUpdate"
            @handleDelete="handleDelete"
          />
        </el-col>
      </el-row>
      <!-- 新建或编辑对话框 -->
      <el-dialog
        :title="title"
        :visible.sync="open"
        :width="'1000px'"
        append-to-body
        :close-on-click-modal="false"
        destroy-on-close
        @close="closeUser"
      >
        <addMaterial
          ref="addMaterial"
          :is-disabled="isDisabled"
          :is-edit="isEdit"
          :is-add="isAdd"
          :form-data="formData"
          :tree-data="placeOptions"
          :default-props="defaultProps"
          @submitMaterialList="submitMaterialList"
        />
        <div
          v-if="isDisabledModify == false"
          slot="footer"
          class="dialog-footer"
        >
          <el-button size="small" @click="cancel">取 消</el-button>
          <el-button
            size="small"
            type="primary"
            @click="submitForm"
          >保 存</el-button>
        </div>
      </el-dialog>
    </el-card>
    <el-dialog
      :title="setTitle"
      :visible.sync="setDigShow"
      width="800px"
      append-to-body
      :close-on-click-modal="false"
    >
      <SetInvoice ref="set" />
      <div slot="footer" class="dialog-footer">
        <el-button size="small" @click="cancelSetInvoice">取消</el-button>
        <el-button v-debounce size="small" type="primary" @click="setSave">保存</el-button>
      </div>
    </el-dialog>
  </div>
</template>

<script>
import {
  getMaterialList,
  getMaterialById,
  changeMaterialStatus,
  delMaterial
} from '@/api/basicdata/material.js'
import { getFixedAssetTree } from '@/api/basicdata/index.js'
import TagsView from '@/layout/components/TagsView/index.vue'
import addMaterial from './component/addMaterial.vue'
import '@riophae/vue-treeselect/dist/vue-treeselect.css'
import MaterialTable from './component/materialTable.vue'
import { getToken } from '@/utils/auth'
import { fnClone } from '@/utils'
import SetInvoice from './component/set.vue'
import { postBaUpDepInfo } from '@/api/assetChange/index'

export default {
  name: 'Material',
  components: { TagsView, addMaterial, MaterialTable, SetInvoice },
  // dicts: ['enable_status'],
  data() {
    return {
      // 遮罩层加载效果显示
      loading: true,
      // 左侧搜索框关键词名称
      keywordName: '',
      // 左侧树选项data
      placeOptions: [],
      // 左侧树形组件默认参数
      defaultProps: {
        children: 'child',
        label: 'name'
      },
      // 右侧头部表单查询参数
      queryParams: {
        pageNum: 1,
        pageSize: 10,
        name: undefined,
        code: undefined,
        assetCode: null
      },
      // 判断是否是新增
      isAdd: false,
      setDigShow: false,
      setTitle: '',
      selectRow: undefined,
      // 用户导入参数
      upload: {
        // 是否显示弹出层（用户导入）
        open: false,
        // 弹出层标题（用户导入）
        title: '',
        // 是否禁用上传
        isUploading: false,
        // 是否更新已经存在的用户数据
        // updateSupport: 0,
        // 设置上传的请求头部
        headers: { Authorization: 'Bearer ' + getToken() },
        // 上传的地址
        url: process.env.VUE_APP_BASE_API + '/system/material/importData'
      },
      // 右侧主体表格显示data
      materialTable: [],
      // 右下分页总条数
      total: 0,
      // 弹出层是否显示
      open: false,
      // 弹出层标题
      title: '',
      // 弹出框是否可编辑
      isDisabledModify: false,
      // 弹出框新建位置表单校验
      rules: {
        sort: [{ required: true, message: '请输入显示顺序', trigger: 'blur' }],
        name: [{ required: true, message: '请填写类别名称', trigger: 'blur' }],
        code: [{ required: true, message: '请填写类别', trigger: 'blur' }]
      },
      // 新建物料对象
      formData: {},
      isDisabled: false,
      isEdit: false,
      clickObj: {},
      ids: []
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
    this.getTreeselect()
  },
  methods: {
    /** 顶部导出*/
    handleExport() {
      this.download(
        'system/material/exportMaterialByCondition/',
        this.ids,
        `物料库_${this.parseTime(new Date(), '{y}-{m}-{d}')}.xlsx`
      )
      // if (this.ids.length === 0) {
      //   // this.$message.error('请选择需要导出的内容')
      //   // return
      //   this.materialTable.forEach(item => {
      //     this.ids.push(item.id)
      //   })
      // }
      // this.download(
      //   'system/material/exportMaterialByCondition/' + this.ids.toString(),
      //   {},
      //   `material_${this.parseTime(new Date(), '{y}-{m}-{d}')}.xlsx`
      // )
    },
    // 顶部模板下载
    handleDownloadTemp() {
      this.download('system/material/importTemplate', {
      }, `material_template_${new Date().getTime()}.xlsx`)
    },
    // 文件上传中处理
    handleFileUploadProgress(event, file, fileList) {
      this.upload.isUploading = true
    },
    beforeUpload(file) {
      const isLt100M = file.size / 1024 / 1024 < 100
      if (!isLt100M) {
        this.$message.error('上传文件大小不能超过 100MB')
      }
      return isLt100M
    },

    // 文件上传成功处理
    handleFileSuccess(response, file, fileList) {
      this.upload.open = false
      this.upload.isUploading = false
      this.$refs.upload.clearFiles()
      this.$alert(
        "<div style='overflow:auto;overflow-x: hidden;max-height: 20vh;padding: 0 20px 0;text-align:left'>" +
          response.msg +
          '</div>',
        response.code === 200 ? '导入成功' : '导入失败',
        { dangerouslyUseHTMLString: true }
      )
      this.getList()
    },
    // 提交上传文件
    submitFileForm() {
      this.$refs.upload.submit()
    },
    /** 顶部新增位置*/
    handleAdd() {
      // 显示保存取消按钮
      this.isDisabledModify = false
      // 显示对话框
      this.open = true
      // 设置对话框标题
      this.title = '新建物料'
      this.isDisabled = false
      this.isAdd = true
      this.isEdit = true
      // 表单重置
      this.$nextTick(() => {
        this.$refs.addMaterial.reset()
        // this.$refs.addMaterial.check(this.clickObj)
      })
    },
    /** 左侧查询下拉树data */
    getTreeselect() {
      getFixedAssetTree().then((response) => {
        // 树形data赋值
        if (response.code === 200) {
          this.placeOptions = response.data
          fnClone(this.placeOptions)
        } else {
          this.$message.error(response.msg)
        }
      })
    },
    /** 左侧关键字过滤树节点 */
    filterNode(value, data) {
      if (!value) return true
      return data.name.indexOf(value) !== -1
    },
    /** 左侧树状节点单击事件 */
    handleNodeClick(data) {
      //
      this.clickObj = data
      this.queryParams.pageNum = 1
      this.queryParams.assetCode = data.assetCode
      if (data.name === '所有') {
        this.queryParams.assetCode = ''
        this.clickObj.code = ''
      }
      this.getList()
      // getMaterialList(this.queryParams).then(
      //   (response) => {
      //     if (response.code === 200) {
      //       this.materialTable = response.data.records.filter(element => {
      //         if (element.categoryLevel1 === this.clickObj.code || element.categoryLevel2 === this.clickObj.code || element.categoryLevel3 === this.clickObj.code) {
      //           return element
      //         }
      //       })
      //       this.total = response.data.total
      //     } else {
      //       this.$message.error(response.msg)
      //     }
      //   }
      // )
    },
    /** 右侧头部搜索按钮操作 */
    handleQuery() {
      this.queryParams.pageNum = 1
      this.getList()
    },
    /** 右侧头部重置按钮操作 */
    resetQuery() {
      this.keywordName = ''
      this.queryParams.assetCode = ''
      this.resetForm('queryForm')
      this.getTreeselect()
      this.handleQuery()
    },
    /** 右侧list多选*/
    selectOptions(row) {
      this.selectRow = row
      this.ids = []
      row.forEach((item) => {
        this.ids.push(item.id)
      })
    },
    /** 右侧查询list结构 */
    getList() {
      this.loading = true
      getMaterialList(this.queryParams).then((response) => {
        if (response.code === 200) {
          this.loading = false
          this.materialTable = response.data.records
          this.total = response.data.total
        } else {
          this.$message.error(response.msg)
        }
      })
    },
    /** 右侧主体详情和编辑操作 */
    handleUpdate(row, index) {
      const { id } = row
      // 无论是详情还是编辑都打开对话框
      this.open = true
      // 保存当前点击的行对象数据
      if (index === 1) {
        this.isAdd = false
        this.title = '编辑物料'
        this.isDisabled = false
        this.isDisabledModify = false
        this.isEdit = true
        getMaterialById(id).then((res) => {
          if (res.code === 200) {
            this.formData = res.data
            this.$nextTick(() => {
              this.$refs.addMaterial.editMaterial()
            })
          } else {
            this.$message.error(res.msg)
          }
        })
      } else if (index === 0) {
        this.isAdd = false
        this.title = '详情'
        this.isDisabled = true
        this.isDisabledModify = true
        this.isEdit = false
        getMaterialById(id).then((res) => {
          this.formData = res.data
          this.$nextTick(() => {
            this.$refs.addMaterial.editMaterial()
          })
        })
      }
    },
    /** 右侧主体删除操作 */
    handleDelete(row) {
      const { id, name } = row
      this.$modal
        .confirm('确定删除"' + name + '"?', '确定删除')
        .then(function () {
          return delMaterial(id)
        })
        .then((res) => {
          if (res.code === 200) {
            // 如果删除之后当前数据长度为1则this.queryParams页数少1
            if (
              this.materialTable.length === 1 &&
              this.queryParams.pageNum !== 1
            ) {
              this.queryParams.pageNum = this.queryParams.pageNum - 1
            }
            this.getList()
            this.$modal.msgSuccess('删除成功')
          } else {
            this.$message.error(res.msg)
          }
        })
        .catch(() => {})
    },
    /** 弹出对话框关闭 */
    closeUser() {
      this.$nextTick(() => {
        this.$refs.addMaterial.reset()
      })
      this.open = false
      this.clickObj = {}
    },
    /** 弹出框取消按钮 */
    cancel() {
      this.$nextTick(() => {
        this.$refs.addMaterial.reset()
      })
      this.open = false
    },
    /** 弹出框保存按钮 */
    submitForm() {
      this.$nextTick(() => {
        this.$refs.addMaterial.submitForm()
      })
    },
    submitMaterialList(data) {
      if (data.code === 200 && !data.id) {
        this.$message.success('创建成功')
        this.open = false
        this.getList()
      } else if (data.id) {
        this.$message.success('编辑成功')
        this.open = false
        this.getList()
      }
    },
    /** 切换分页 */
    changePage(data) {
      this.queryParams.pageNum = data.pageNum
      this.queryParams.pageSize = data.pageSize
      this.getList()
    },
    /** 切换状态 */
    changeStatus(row) {
      const { id } = row
      changeMaterialStatus(id).then((res) => {
        if (res.code === 200) {
          this.$modal.msgSuccess('修改成功')
          this.getList()
        } else {
          this.$message.error(res.msg)
          this.getList()
        }
      })
    },
    setDepreciation() {
      // debugger
      // 设置折旧方法

      if (this.selectRow) {
        this.setDigShow = true
        this.setTitle = '设置折旧'
      } else if (!this.selectRow) {
        this.$message.error('至少需要选择一行')
      }
    },
    setSave() {
      this.$refs.set.$refs.form.validate((valid) => {
        if (valid) {
          const arrayList = this.$refs.set.depreciation
          const data = {
            depreciationRate: arrayList.depreciationRate / 100,
            depreciationType: arrayList.depreciationType,
            ids: this.ids,
            residualValueRate: arrayList.residualValueRate / 100,
            serviceLife: arrayList.serviceLife
          }
          postBaUpDepInfo(data).then(
            //
            (res) => {
              // console.log('res', res)
              if (res.code === 200) {
                this.setDigShow = false
                this.getList()
                this.$refs.set.$refs.form.resetFields()
              } else {
                this.$message.error('保存失败')
                this.setDigShow = false
              }
            })
        } else if (!valid) {
          // console.log('error submit!!')
          this.$message.error('验证不通过')
          return false
        }
      })
    },
    cancelSetInvoice() {
      this.$nextTick(() => {
        this.$refs.set.depreciation = {}
      })
      this.setDigShow = false
    }
  }
}
</script>

<style  lang="scss" scoped>
.right-col {
  display: flex;
  height: 100%;
  flex-direction: column;
}
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
::v-deep .el-dialog__body {
  padding: 3px 20px;
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
.colTitle {
  height: 36px;
  line-height: 36px;
  padding-left: 10px;
  background: #f1f2f5;
  border-right: 1px solid #dddddd;
  border-bottom: 1px solid #dddddd;
}
.redDot {
  color: #ef5826;
}
.date_box {
  position: relative;
  width: 100%;
  .data_icon {
    position: absolute;
    top: 50%;
    right: 17px;
    z-index: 9;
    color: #c0c4cc;
    font-size: 14px;
    transform: translateY(-50%);
  }
  ::v-deep .el-range__close-icon {
    position: absolute;
    right: 34px;
    top: 54%;
    transform: translateY(-50%);
  }
  ::v-deep .el-date-editor .el-input__inner {
    border: none;
  }
}
::v-deep .inputNoBorder .el-input__inner {
  border: none;
  border-radius: 0;
  border-bottom: 1px solid #dddddd;
}
::v-deep .el-dialog__body {
  line-height: 36px;
}
</style>
