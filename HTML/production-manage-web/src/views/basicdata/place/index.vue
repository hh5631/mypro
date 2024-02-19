<template>
  <div>
    <!-- 头部 -->
    <tags-view>
      <el-row :gutter="10" class="mb8">
        <el-col :span="1.5">
          <el-button
            v-hasPermi="['system:post:export']"
            plain
            size="small"
            @click="handleExport"
          ><i style="color:#4686EF;" class="iconfont icon-daochu" />导出</el-button>
        </el-col>
        <el-col :span="1.5">
          <el-button
            v-hasPermi="['system:post:add']"
            type="primary"
            size="small"
            @click="handleAdd"
          ><i class="iconfont icon-zengjia" style="color: #ffffff" />新增</el-button>
        </el-col>
      </el-row>
    </tags-view>
    <!-- 主体 -->
    <el-card class="main-card-left">
      <!-- 页面内容 -->
      <el-row :gutter="20">
        <el-col :span="24" :xs="24" class="right-card">
          <el-form
            ref="queryForm"
            :model="queryParams"
            :inline="true"
            label-width="68px"
            class="searchForm"
          >
            <el-form-item label="位置代码" prop="placeCode">
              <el-input
                v-model.trim="queryParams.placeCode"
                placeholder="请输入"
                clearable
                size="small"
                style="width: 150px"
                maxlength="15"
                @keyup.enter.native="handleQuery"
              />
            </el-form-item>
            <el-form-item label="使用状态" prop="placeStatus">
              <el-select v-model="queryParams.placeStatus" style="width: 150px" placeholder="请选择" clearable size="small">
                <el-option
                  v-for="item in placeStatus"
                  :key="item.value"
                  :label="item.label"
                  :value="item.value"
                />
              </el-select>
            </el-form-item>
            <!-- <el-form-item label="状态" prop="status">
              <el-select v-model="queryParams.status" style="width: 150px" placeholder="请选择" clearable size="small">
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
          <el-table
            ref="multiplePlaceTable"
            v-loading="loading"
            highlight-current-row
            height="100%"
            row-key="id"
            :expand-on-click-node="false"
            :expand-row-keys="tableExpandedArr"
            :data="placeTable"
            :tree-props="{children: 'children', hasChildren: 'hasChildren'}"
            @selection-change="handleSelectionChange"
          >
            <el-table-column type="selection" align="center" />
            <el-table-column label="序号" sortable align="left" prop="index" type="index" />
            <el-table-column label="位置名称" align="left" prop="placeName" :show-overflow-tooltip="true" />
            <el-table-column label="位置代码" align="left" prop="placeCode" :show-overflow-tooltip="true" />
            <el-table-column label="使用状态" align="left" prop="placeStatus" :show-overflow-tooltip="true">
              <template slot-scope="scope">
                <span>{{ scope.row.placeStatus === '0' ?"在用":"闲置" }}</span>
              </template>
            </el-table-column>
            <!-- <el-table-column label="状态" align="left" prop="status">
              <template slot-scope="scope">
                <div class="statePoint">
                  <el-switch v-model="scope.row.status" active-value="0" inactive-value="1" @change="switchChange(scope.row)" />
                </div>
              </template>
            </el-table-column> -->
            <el-table-column label="操作" align="left" class-name="small-padding">
              <template slot-scope="scope">
                <el-button
                  v-hasPermi="['system:user:query']"
                  size="small"
                  type="text"
                  @click="handleUpdate(scope.row,0)"
                >详情</el-button>
                <el-button
                  v-hasPermi="['system:user:edit']"
                  size="small"
                  type="text"
                  style="color:#4686EF"
                  @click="handleUpdate(scope.row,1)"
                >编辑</el-button>
                <el-button
                  v-hasPermi="['system:user:add']"
                  size="small"
                  type="text"
                  @click="handleAdd(scope.row)"
                >新建</el-button>
                <el-button
                  v-if="scope.row.parentId !== 0"
                  v-hasPermi="['system:user:remove']"
                  size="small"
                  type="text"
                  style="color:#EF5826"
                  @click="handleDelete(scope.row)"
                >删除</el-button>
              </template>
            </el-table-column>
          </el-table>
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
          <el-form-item v-if="isAncestor" label="上级位置" :prop="isEdit?'parentId':''">
            <!-- <el-select v-if="isEdit" v-model="placeForm.parentId" :disabled="isDisabled" style="width: 508px" placeholder="请选择" size="small">
              <el-option
                :key="placeForm.parentId"
                :label="placeForm.parentName"
                hidden
                :value="placeForm.parentId"
              />
              <el-input
                v-model="filterText"
                placeholder="请输入关键词"
                prefix-icon="el-icon-search"
                type="text"
                class="search"
                clearable
                maxlength="15"
              />
              <el-tree
                ref="treeSelect"
                class="filter-tree"
                :data="treeData"
                :props="defaultProps"
                node-key="id"
                :expand-on-click-node="false"
                :check-on-click-node="true"
                default-expand-all
                :filter-node-method="filterNode"
                @node-click="handleNodeClick"
              />
            </el-select> -->
            <el-input
              v-if="isEdit"
              v-model.trim="placeForm.parentName"
              placeholder="请选择"
              :readonly="true"
              :disabled="isDisabled"
              type="text"
              @click.native="showTree"
            />
            <span v-else>
              {{ rowInfo.parentName?rowInfo.parentName:'--' }}
            </span>
          </el-form-item>
          <el-form-item label="位置名称" :prop="isEdit?'placeName':''">
            <el-input
              v-if="isEdit"
              v-model.trim="placeForm.placeName"
              placeholder="请输入"
              :disabled="isDisabledModify"
              type="text"
              maxlength="15"
            />
            <span v-else>
              {{ rowInfo.placeName?rowInfo.placeName:'--' }}
            </span>
          </el-form-item>
          <el-form-item label="位置代码" :prop="isEdit?'placeCode':''">
            <el-input
              v-if="isEdit"
              v-model.trim="placeForm.placeCode"
              placeholder="请输入"
              :disabled="isDisabledModify"
              type="text"
              maxlength="15"
            />
            <span v-else>
              {{ rowInfo.placeCode?rowInfo.placeCode:'--' }}
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
            />
            <span v-else>
              {{ rowInfo.sort?rowInfo.sort:'--' }}
            </span>
          </el-form-item>
          <el-form-item label="使用状态">
            <el-radio-group v-if="isEdit" v-model="placeForm.placeStatus" :disabled="isDisabledModify">
              <el-radio :key="0" label="0">在用</el-radio>
              <el-radio :key="1" label="1">闲置</el-radio>
            </el-radio-group>
            <span v-else>{{ rowInfo.placeStatus === '0'?'在用':'闲置' }}</span>
          </el-form-item>
          <!-- <el-form-item label="状态" prop="status">
            <el-radio-group v-if="isEdit" v-model="placeForm.status">
              <el-radio :key="0" label="0">正常</el-radio>
              <el-radio :key="1" label="1">停用</el-radio>
            </el-radio-group>
            <span v-else>{{ rowInfo.status === '0'?'正常':'停用' }}</span>
          </el-form-item> -->
          <el-form-item label="备注">
            <el-input
              v-if="isEdit"
              v-model="placeForm.remark"
              type="textarea"
              :rows="2"
              maxlength="500"
              show-word-limit
              placeholder="请输入文本"
            />
            <div v-else style="height:300px;overflow:auto;">{{ rowInfo.remark }}</div>
          </el-form-item>
        </el-form>
        <div v-if="isDisabledModify == false" slot="footer" class="dialog-footer">
          <el-button size="small" @click="cancel">取 消</el-button>
          <el-button v-debounce size="small" type="primary" @click="submitForm">保 存</el-button>
        </div>
      </el-dialog>
    </el-card>
    <tree-select v-if="treeShow" :title="placeTitle" :handle-function="getPlaceTreeSelect" @onClose="treeShow = false" @selectNode="handleNodeClick" />
  </div>

</template>
<script>
import { getPlaceList, getPlaceDetail, getPlaceTreeSelect, addPlace, updatePlace, delPlace, changeStatus } from '@/api/basicdata/index'
import TagsView from '@/layout/components/TagsView/index.vue'
import TreeSelect from '@/components/TreeSelect'
import '@riophae/vue-treeselect/dist/vue-treeselect.css'
import FormValidators from '@/vform/utils/validators'
export default {
  name: 'Place',
  dicts: ['enable_status'],
  components: { TagsView, TreeSelect },
  data() {
    return {
      getPlaceTreeSelect,
      treeShow: false,
      // 遮罩层加载效果显示
      loading: true,
      // 上级位置
      treeData: [],
      // 左侧树形组件默认参数
      defaultProps: {
        children: 'children',
        label: 'label'
      },
      // 右侧头部表单使用状态
      placeStatus: [
        {
          label: '在用',
          value: '0'
        },
        {
          label: '闲置',
          value: '1'
        }
      ],
      // 右侧头部表单查询参数
      queryParams: {
        pageNum: 1,
        pageSize: 10,
        placeName: undefined,
        placeCode: undefined,
        placeStatus: undefined,
        status: undefined
      },
      // 右侧主体表格显示data
      placeTable: [],
      // 多选数组，导出
      ids: [],
      // 右侧主体保存row信息
      rowInfo: {},
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
        parentId: undefined,
        placeCode: undefined,
        placeName: undefined,
        placeStatus: undefined,
        remark: undefined,
        status: undefined,
        sort: undefined
      },
      // 弹出框新建位置表单校验
      rules: {
        sort: [
          { required: true, message: '请输入显示顺序', trigger: 'blur' },
          {
            validator: function(rule, value, callback) {
              const reg = /^\+?[1-9]\d*$/
              if (reg.test(value) === false) {
                callback(new Error('请输入正整数'))
              } else {
                callback()
              }
            },
            trigger: ['blur']
          }
        ],
        parentId: [
          { required: true, message: '请选择上级位置', trigger: 'change' }
        ],
        placeName: [
          { required: true, message: '请填写位置名称', trigger: 'blur' },
          { max: 15, message: '位置名称小于等于 15 个字符', trigger: 'blur' },
          { validator: FormValidators.chineseAndLetterAndNumber, message: '必须为字母、汉字或者数字' }
        ],
        placeCode: [
          { required: true, message: '请填写位置代码', trigger: 'blur' },
          { min: 3, max: 15, message: '长度在 3 到 15 个字符', trigger: 'blur' },
          {
            validator: function(rule, value, callback) {
              const reg = /^[A-Za-z0-9]+$/
              if (reg.test(value) === false) {
                callback(new Error('请输入数字或英文'))
              } else {
                // 校验通过
                callback()
              }
            },
            trigger: ['blur', 'change']
          }
        ]
      },
      isAncestor: true,
      filterText: '',
      isDisabled: false,
      tableExpandedArr: [],
      placeTitle: '选择位置'
    }
  },
  computed: {
  },
  watch: {
    filterText(val) {
      this.$refs.treeSelect.filter(val)
    }
  },
  created() {
    this.getList()
    this.getTreeselect()
  },
  methods: {
    /** 顶部导出*/
    handleExport() {
      // if (this.ids.length === 0) {
      //   // this.$message.error('请选择需要导出的内容')
      //   // return
      //   // this.placeTable.forEach(item => {
      //   //   this.ids.push(item.id)
      //   // })
      // } else {

      // }
      this.download(
        'system/place/export/?ids=',
        this.ids,
        `位置管理_${this.parseTime(new Date(), '{y}-{m}-{d}')}.xlsx`
      )
    },
    /** 顶部新增位置*/
    handleAdd(row) {
      // 重置新增表单
      this.reset()
      // 新增时表单控件可编辑
      this.isDisabledModify = false
      // 显示对话框
      this.open = true
      this.isAncestor = true
      // 显示控件元素
      this.isEdit = true
      // 设置对话框标题
      this.title = '新建位置'
      this.isDisabled = false
      if (row.id) {
        this.placeForm.parentId = row.id
        this.placeForm.parentName = row.placeName
        this.placeForm.sort = row.children ? row.children.length + 1 : 1
        this.title = '新建子位置'
        this.isDisabled = true
      }
    },
    /** 右侧头部搜索按钮操作 */
    handleQuery() {
      this.queryParams.pageNum = 1
      this.getList()
    },
    /** 右侧头部重置按钮操作 */
    resetQuery() {
      this.resetForm('queryForm')
      this.getTreeselect()
      this.handleQuery()
    },
    showTree() {
      if (!this.isDisabled) {
        this.treeShow = true
      } else {
        this.treeShow = false
      }
    },
    /** 右侧查询list结构 */
    getList() {
      this.loading = true
      getPlaceList(this.queryParams).then(
        (response) => {
          if (response.code === 200) {
            this.ids = response.data.map(item => {
              return item.id
            })
            this.placeTable = this.handleTree(response.data, 'id')
            this.placeTable.forEach(item => {
              this.tableExpandedArr.push(item.id + '')
            })
            this.total = response.data.total
            this.loading = false
          } else {
            this.$message.error(response.msg)
          }
        }
      )
    },
    /** 左侧查询下拉树data */
    getTreeselect() {
      getPlaceTreeSelect().then((response) => {
        if (response.code === 200) {
          this.treeData = response.data
        }
      })
    },
    /** 右侧主体详情和编辑操作 */
    handleUpdate(row, index) {
      // 无论是详情还是编辑都打开对话框
      this.open = true
      // 保存当前点击的行对象数据
      if (row.parentId === 0) {
        this.isAncestor = false
      } else {
        this.isAncestor = true
      }
      const { id } = row
      if (index === 1) {
        this.title = '编辑位置'
        this.isEdit = true
        this.isDisabledModify = false
        this.placeForm = JSON.parse(JSON.stringify(row))
      } else if (index === 0) {
        this.title = '详情'
        this.isEdit = false
        this.isDisabledModify = true
        // 获取详情
        getPlaceDetail(id).then(
          res => {
            if (res.code === 200) {
              this.rowInfo = res.data
            }
          }
        )
      }
    },
    /** 右侧主体删除操作 */
    handleDelete(row) {
      const { id, placeName } = row
      this.$modal
        .confirm('确定删除"' + placeName + '"?', '确定删除')
        .then(function() {
          return delPlace(id)
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
    reset() {
      this.placeForm = {
        parentId: undefined,
        placeCode: undefined,
        placeName: undefined,
        placeStatus: '0',
        remark: undefined,
        status: '0',
        sort: undefined
      }
      this.resetForm('placeForm')
    },
    /** 弹出框取消按钮 */
    cancel() {
      this.open = false
      this.$refs.placeForm.clearValidate()
      this.reset()
    },
    /** 弹出框保存按钮 */
    submitForm() {
      this.$refs['placeForm'].validate((valid) => {
        if (valid) {
          // 验证通过判断时新增还是修改
          if (this.placeForm.id) {
            updatePlace(this.placeForm).then((response) => {
              if (response.code === 200) {
                this.$modal.msgSuccess('修改成功')
                this.open = false
                this.$refs.placeForm.clearValidate()
                this.getList()
              } else {
                this.$message.error(response.msg)
              }
            })
          } else {
            addPlace(this.placeForm).then((response) => {
              if (response.code === 200) {
                this.$modal.msgSuccess('新增成功')
                this.open = false
                this.$refs.placeForm.clearValidate()
                this.getList()
                this.resetForm('queryForm')
              } else {
                this.$message.error(response.msg)
              }
            })
          }
        }
      })
    },
    /** 弹出对话框关闭 */
    closeUser() {
      // 表单对象置空
      this.open = false
      this.$refs.placeForm.clearValidate()
    },
    /** 改变状态 */
    switchChange(row) {
      const { id } = row
      changeStatus(id).then(res => {
        if (res.code === 200) {
          this.$modal.msgSuccess('修改成功')
        } else {
          this.$message.error(res.msg)
        }
      })
    },
    /** 上级选择 */
    handleNodeClick(data) {
      this.placeForm.parentName = data.label
      this.placeForm.parentId = data.id
      this.filterText = ''
    },
    /** 过滤 */
    filterNode(value, data) {
      if (!value) return true
      return data.label.indexOf(value) !== -1
    },
    /** 选中导出数据 */
    handleSelectionChange(selection) {
      this.ids = []
      selection.forEach(item => {
        this.ids.push(item.id)
      })
    }
  }
}
</script>

<style  lang="scss" scoped>
.statePoint .point{
  width:8px;
  height:8px;
  margin:0 8px 2px 0;
}
::v-deep .el-tree-node:focus > .el-tree-node__content {
  background-color: #fff;
}
.search {
  width: 400px;
  margin-left: 50px;
  padding: 10px 5px;
}
</style>
