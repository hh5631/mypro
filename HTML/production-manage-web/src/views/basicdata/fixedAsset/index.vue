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
          ><i style="color:#4686ef;" class="iconfont icon-daochu" />导出
          </el-button>
        </el-col>
        <!-- <el-col :span="1.5">
          <el-button
            v-hasPermi="['system:user:add']"
            type="primary"
            size="small"
            @click="handleAdd"
          ><i class="iconfont icon-zengjia" style="color: #ffffff" />新增
          </el-button>
        </el-col> -->
      </el-row>
    </tags-view>
    <el-card class="main-card-left">
      <!-- 页面内容 -->
      <el-row :gutter="20">
        <!--位置左侧tree数据-->
        <el-col
          :span="4"
          :xs="24"
          style="border-right: 1px solid #eaedf4;padding-right: 0px;"
        >
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
          <!-- 树形组件 -->
          <div class="head-container" style="height: 70vh;overflow: auto;">
            <el-tree
              ref="tree"
              :current-node-key="currentNodeKey"
              :default-expanded-keys="[currentNodeKey]"
              :data="placeOptions"
              :props="defaultProps"
              :expand-on-click-node="false"
              :filter-node-method="filterNode"
              highlight-current
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
            <el-form-item label="类别名称" prop="name">
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
            <el-form-item label="类别代码" prop="code">
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
            <!-- <el-form-item label="状态" prop="enable">
              <el-select v-model="queryParams.enable" style="width: 130px" placeholder="请选择" clearable size="small">
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
              <el-button size="small" type="primary" class="search-btn" plain @click="handleQuery">搜索</el-button>
            </el-form-item>
          </el-form>
          <!-- 表格显示list数据 -->
          <el-table ref="multiplePlaceTable" v-loading="loading" highlight-current-row height="100%" :data="placeTable" @selection-change="handleSelectionChange">
            <el-table-column type="selection" align="center" />
            <el-table-column label="序号" sortable align="left" prop="index" type="index" />
            <el-table-column label="类别名称" align="left" prop="name" :show-overflow-tooltip="true" />
            <el-table-column label="类别代码" align="left" prop="code" :show-overflow-tooltip="true" />
            <!-- <el-table-column label="状态" align="left" prop="enable">
              <template slot-scope="scope">
                <div class="statePoint">
                  <el-switch v-model="scope.row.enable" active-value="0" inactive-value="1" @change="switchChange(scope.row)" />
                </div>
              </template>
            </el-table-column> -->
            <!-- <el-table-column label="操作" align="left" width="180" class-name="small-padding">
              <template slot-scope="scope">
                <el-button
                  v-hasPermi="['system:user:query']"
                  size="small"
                  type="text"
                  @click="handleUpdate(scope.row,0)"
                >详情
                </el-button>
                <el-button
                  v-hasPermi="['system:user:edit']"
                  size="small"
                  type="text"
                  style="color:#4686ef"
                  @click="handleUpdate(scope.row,1)"
                >编辑
                </el-button>
                <el-button
                  v-hasPermi="['system:user:remove']"
                  size="small"
                  type="text"
                  style="color:#ef5826"
                  @click="handleDelete(scope.row)"
                >删除
                </el-button>
              </template>
            </el-table-column> -->
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
        </el-col>
      </el-row>
      <!-- 新建或编辑位置对话框 -->
      <el-dialog
        :title="title"
        :visible.sync="open"
        :width="isEdit?'800px':'600px'"
        append-to-body
        :close-on-click-modal="false"
        @close="closeUser"
      >
        <el-form
          ref="placeForm"
          :model="placeForm"
          :rules="rules"
          label-width="100px"
          style="width: 80%; margin-left: 10%"
          :class="isEdit?'':'editForm'"
          label-position="right"
        >
          <el-form-item label="上级类别" :prop="isEdit?'parentName':''">
            <el-input v-if="isEdit" v-model="placeForm.parentName" placeholder="请选择" maxlength="20" @focus="showPrePlaceDialog">
              <i slot="suffix" class="iconfont icon-bumenxuanze" style="cursor: pointer;" @click="showPrePlaceDialog" />
            </el-input>
            <span v-else>
              {{ rowInfo.parentName ? rowInfo.parentName : '--' }}
            </span>
          </el-form-item>
          <el-form-item label="名称类别" :prop="isEdit?'name':''">
            <el-input
              v-if="isEdit"
              v-model.trim="placeForm.name"
              placeholder="请输入"
              :disabled="isDisabledModify"
              type="text"
              maxlength="15"
            />
            <span v-else>
              {{ rowInfo.name ? rowInfo.name : '--' }}
            </span>
          </el-form-item>
          <el-form-item label="类别代码" :prop="isEdit?'code':''">
            <el-input
              v-if="isEdit"
              v-model.trim="placeForm.code"
              placeholder="请输入"
              :disabled="isDisabledModify"
              type="text"
              maxlength="15"
            />
            <span v-else>
              {{ rowInfo.code ? rowInfo.code : '--' }}
            </span>
          </el-form-item>
          <el-form-item label="显示顺序" :prop="isEdit?'sort':''">
            <el-input
              v-if="isEdit"
              v-model.trim="placeForm.sort"
              placeholder="请输入"
              :disabled="isDisabledModify"
              type="text"
              maxlength="5"
              onkeyup="if(value==0&&value!=''){value=1}this.value = this.value.replace(/[^0-9]/g, '');"
            />
            <span v-else>
              {{ rowInfo.sort ? rowInfo.sort : '--' }}
            </span>
          </el-form-item>
          <!-- <el-form-item label="状态">
            <el-radio-group v-if="isEdit" v-model="placeForm.enable" :disabled="isDisabledModify">
              <el-radio :key="'0'" :label="'0'">正常</el-radio>
              <el-radio :key="'1'" :label="'1'">停用</el-radio>
            </el-radio-group>
            <span v-else>{{ rowInfo.enable === '0' ? '正常' : '停用' }}</span>
          </el-form-item> -->
          <el-form-item label="备注">
            <el-input
              v-if="isEdit"
              v-model.trim="placeForm.description"
              placeholder="请输入文本"
              :disabled="isDisabledModify"
              type="textarea"
              :rows="5"
            />
            <span v-else>
              {{ rowInfo.description }}
            </span>
          </el-form-item>
        </el-form>
        <div v-if="isDisabledModify == false" slot="footer" class="dialog-footer">
          <el-button size="small" @click="cancel">取 消</el-button>
          <el-button size="small" type="primary" @click="submitForm">保 存</el-button>
        </div>
      </el-dialog>
      <!-- 上级选择 -->
      <el-dialog
        title="上级资产类别选择"
        :visible.sync="isShowPlace"
        width="600px"
        :close-on-click-modal="false"
        append-to-body
      >
        <div class="head-container">
          <el-input
            v-model.trim="dialogPlaceName"
            placeholder="请输入关键字"
            clearable
            size="small"
            maxlength="15"
            suffix-icon="el-icon-search"
            style="margin-bottom: 20px"
          />
        </div>
        <div class="head-container" style="height:400px;overflow-y:auto;">
          <el-tree
            ref="placeTree"
            :data="addPlaceOptions"
            :props="defaultProps"
            :expand-on-click-node="false"
            :filter-node-method="filterPreNode"
            node-key="id"
            highlight-current
            @node-click="handleClickPre"
          />
        </div>
        <div slot="footer" class="dialog-footer">
          <el-button
            size="small"
            @click="isShowPlace=false"
          >取 消
          </el-button>
          <el-button
            size="small"
            type="primary"
            @click="saveChoosePlace"
          >保 存
          </el-button>
        </div>
      </el-dialog>
    </el-card>
  </div>
</template>

<script>
import {
  getFixedAssetList,
  getFixedAssetByCode,
  changeFixedAssetStatus,
  insertOrUpdateFixedAsset,
  delFixedAsset,
  getTree
} from '@/api/basicdata/index'
// import FormValidators from '../../../vform/utils/validators'
import TagsView from '@/layout/components/TagsView/index.vue'
import '@riophae/vue-treeselect/dist/vue-treeselect.css'

export default {
  name: 'Place',
  components: { TagsView },
  dicts: ['enable_status'],
  data () {
    return {
      currentNodeKey: '0000000',
      // 遮罩层加载效果显示
      loading: true,
      // 左侧搜索框关键词名称
      keywordName: '',
      // 左侧树选项data
      placeOptions: [],
      // 左侧树选项是否渲染
      placeOptionsFlog: true,
      // 新增树上级类别选项
      addPlaceOptions: [],
      // 左侧树形组件默认参数
      defaultProps: {
        children: 'child',
        label: 'name'
      },
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
        code: undefined,
        parentCode: '',
        enable: undefined
      },
      // 右侧主体表格显示data
      placeTable: [],
      // 右侧主体多选数组
      ids: [],
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
      // 弹出层标题
      title: '',
      // 弹出框是否可编辑
      isDisabledModify: false,
      // 弹出框是否是编辑状态
      isEdit: false,
      // 弹出框新建位置的表单数据
      placeForm: {
        parentName: '',
        parentCode: '',
        code: '',
        name: '',
        enable: '0',
        sort: 0
      },
      // 弹出框新建位置表单校验
      rules: {
        sort: [{ required: true, message: '请输入显示顺序', trigger: 'blur' }],
        parentName: [
          { required: true, message: '请选择上级类别', trigger: 'blur' }
        ],
        name: [
          { required: true, message: '请填写名称', trigger: 'blur' },
          { max: 15, message: '类别名称', trigger: 'blur' }
        ],
        code: [
          { required: true, message: '请填写类别', trigger: 'blur' },
          { min: 3, max: 15, message: '类别代码-长度3-15位', trigger: 'blur' }
        ]
      },
      // 选择上级位置弹出框是否显示
      isShowPlace: false,
      // 选择上级位置弹出框名称筛选树
      dialogPlaceName: undefined,
      // 选择上级位置的对象
      preChoose: {},
      clickInfo: {},
      // 新增时的 parentCode 存储
      addParentCode: null
    }
  },
  computed: {},
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

  async created () {
    await this.getList()

    await this.getTreeselect()
  },
  methods: {
    /** 顶部导出*/
    handleExport () {
      this.download(
        'system/fixedAsset/export/',
        this.ids,
        `资产类别_${this.parseTime(new Date(), '{y}-{m}-{d}')}.xlsx`
      )
      // if (this.ids.length === 0) {
      //   // this.$message.error('请选择需要导出的内容')
      //   // return
      //   this.placeTable.forEach(item => {
      //     this.ids.push(item.id)
      //   })
      // }
      //
      // this.download(
      //   'system/fixedAsset/export/' + this.ids.toString(),
      //   {},
      //   `fixedAsset_${this.parseTime(new Date(), '{y}-{m}-{d}')}.xlsx`
      // )
    },
    /** 顶部新增位置*/
    handleAdd () {
      // 重置新增表单
      this.reset()
      this.getTreeselect()
      // 新增时表单控件可编辑
      this.isDisabledModify = false
      // 显示对话框
      if (this.clickInfo.code) {
        this.placeForm.parentName = this.clickInfo.name
      }
      this.open = true
      // 显示控件元素
      this.isEdit = true
      // 设置对话框标题
      this.title = '新建资产类别'
      // 给新增的类别赋予 parentCode 参数
      this.$set(this.placeForm, 'parentCode', this.addParentCode)
    },
    /** 左侧查询下拉树data */
    async getTreeselect () {
      await getTree().then((response) => {
        // 树形data赋值
        //
        // const arrList = []
        // for (let i = 0; i < 6; i++) {
        //   arrList.push({ name: assetInfo[i].name, code: '', child: [] })
        // }

        // response.data.forEach(item => {
        //   const index = item.code.substr(0, 1) - 1
        //   arrList[index].child.push(item)
        // })
        if (this.placeOptionsFlog) {
          this.placeOptions = response.data
        } else {
          this.addPlaceOptions = response.data
        }
      })
      this.placeOptionsFlog = false
    },
    /** 左侧关键字过滤树节点 */
    filterNode (value, data) {
      if (!value) return true
      return data.name.indexOf(value) !== -1
    },
    /** 关键字过滤树节点 */
    filterPreNode (value, data) {
      if (!value) return true
      return data.name.indexOf(value) !== -1
    },
    /** 左侧树状节点单击事件 */
    handleNodeClick (data) {
      this.queryParams.parentCode = data.code
      if (data.name === '所有') {
        this.queryParams.parentCode = ''
      }
      this.addParentCode = data.code || null
      this.clickInfo = data
      this.handleQuery()
    },
    /** 右侧头部搜索按钮操作 */
    handleQuery () {
      this.queryParams.pageNum = 1
      this.getList()
    },
    /** 右侧头部重置按钮操作 */
    resetQuery () {
      this.resetForm('queryForm')
      this.queryParams.parentCode = ''
      this.placeOptionsFlog = true
      this.getTreeselect()
      this.handleQuery()
      this.keywordName = ''
    },
    /** 右侧查询list结构 */
    getList () {
      this.loading = true
      getFixedAssetList(this.addDateRange(this.queryParams)).then(
        (response) => {
          if (response.code === 200) {
            this.loading = false
            this.placeTable = response.data.records
            this.total = response.data.total
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
      selection.forEach(item => {
        this.ids.push(item.code)
      })
      this.single = selection.length !== 1
      this.multiple = !selection.length
    },
    /** 右侧主体详情和编辑操作 */
    handleUpdate (row, index) {
      const { code } = row
      // 无论是详情还是编辑都打开对话框
      this.open = true
      if (index === 1) {
        this.title = '编辑'
        this.isEdit = true
        this.isDisabledModify = false
        getFixedAssetByCode(code).then(res => {
          this.placeForm = res.data
        })
      } else if (index === 0) {
        this.title = '详情'
        this.isEdit = false
        this.isDisabledModify = true
        getFixedAssetByCode(code).then(res => {
          this.rowInfo = res.data
        })
      }
    },
    /** 右侧主体删除操作 */
    handleDelete (row) {
      const { code, name } = row
      this.$modal
        .confirm('确定删除"' + name + '"?', '确定删除')
        .then(function () {
          return delFixedAsset(code)
        })
        .then(() => {
          // 如果删除之后当前数据长度为1则this.queryParams页数少1
          if (this.placeTable.length === 1 && this.queryParams.pageNum !== 1) {
            this.queryParams.pageNum = this.queryParams.pageNum - 1
          }
          this.getList()
          this.$modal.msgSuccess('删除成功')
        })
        .catch(() => {})
    },
    /** 弹出框新建位置表单重置 */
    reset () {
      this.placeForm = {
        sort: '',
        name: '',
        enable: '0',
        code: '',
        parentCode: ''
      }
      this.resetForm('placeForm')
    },
    /** 弹出框取消按钮 */
    cancel () {
      this.open = false
      this.reset()
      this.getList()
      this.clickInfo = {}
    },
    /** 弹出框保存按钮 */
    submitForm () {
      this.$refs['placeForm'].validate((valid) => {
        if (valid) {
          // 验证通过判断时新增还是修改
          if (this.placeForm.code !== undefined) {
            insertOrUpdateFixedAsset(this.placeForm).then((response) => {
              this.$modal.msgSuccess('修改成功')
              this.open = false
              this.resetForm('queryForm')
              this.getList()
            })
          } else {
            insertOrUpdateFixedAsset(this.placeForm).then((response) => {
              this.$modal.msgSuccess('保存成功')
              this.open = false
              this.resetForm('queryForm')
              // 如果当前页和选择的父级类别是同一个，就刷新页面，否则不刷新
              if (this.addParentCode === this.preChoose.code) {
                this.getList()
                this.placeOptionsFlog = true
                this.getTreeselect()
              }
            })
          }
        }
      })
    },
    /** 弹出对话框关闭 */
    closeUser () {
      // 表单对象置空
      this.placeForm = {}
      this.preChoose = {}
      this.clickInfo = {}
    },
    /** 弹出框显示上级选项 */
    showPrePlaceDialog () {
      this.getTreeselect()
      this.isShowPlace = true
    },
    /** 弹出框保存选择的上级处理 */
    handleClickPre (data) {
      this.preChoose = data
    },
    /** 弹出框保存选择的上级 */
    saveChoosePlace () {
      // 弹出框隐藏
      this.isShowPlace = false

      // if (this.preChoose.parentCode === null) {
      //   this.placeForm.parentCode = this.preChoose.code
      // }
      // this.placeForm.parentCode = this.preChoose.code
      // this.placeForm.parentName = this.preChoose.name
      this.$set(this.placeForm, 'parentCode', this.preChoose.code)
      this.$set(this.placeForm, 'parentName', this.preChoose.name)
      if (!this.placeForm.parentName) {
        this.$message.error('请选择上级')
        return
      }
      this.$nextTick(() => {
        this.$refs['placeForm'].validateField('parentName')
      })
    },
    /** 切换状态 */
    switchChange (row) {
      const { code } = row
      changeFixedAssetStatus(code).then(res => {
        if (res.code === 200) {
          this.$modal.msgSuccess('修改成功')
          this.getList()
        } else {
          this.$message.error(res.msg)
          this.getList()
        }
      })
    }
  }
}
</script>

<style lang="scss" scoped>
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
