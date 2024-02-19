<template>
  <div>
    <tags-view>
      <el-row :gutter="10" class="mb8">
        <el-col :span="1.5">
          <el-button
            v-hasPermi="['system:dept:add']"
            type="primary"
            icon="el-icon-plus"
            size="small"
            @click="handleAdd"
          >新建部门</el-button>
        </el-col>
      </el-row>
    </tags-view>
    <el-card class="main-card">
      <el-form v-show="showSearch" ref="queryForm" :model="queryParams" :inline="true">
        <el-form-item label="部门名称" prop="deptName">
          <el-input
            v-model.trim="queryParams.deptName"
            placeholder="请输入"
            maxlength="15"
            clearable
            size="small"
            style="width: 200px"
            @keyup.enter.native="handleQuery"
          />
        </el-form-item>
        <el-form-item label="部门负责人" prop="leader">
          <el-input
            v-model.trim="queryParams.leader"
            placeholder="请输入"
            maxlength="15"
            clearable
            size="small"
            style="width: 200px"
            @keyup.enter.native="handleQuery"
          />
        </el-form-item>
        <el-form-item>
          <el-button size="small" @click="resetQuery">重置</el-button>
          <el-button type="primary" plain size="small" class="search-btn" @click="handleQuery">搜索</el-button>
        </el-form-item>
      </el-form>
      <el-table
        v-if="refreshTable"
        ref="deptTable"
        v-loading="loading"
        :data="deptList"
        row-key="deptId"
        height="100%"
        highlight-current-row
        :expand-row-keys="tableExpandedArr"
        :tree-props="{children: 'children', hasChildren: 'hasChildren'}"
      >
        <!-- <el-table-column prop="orderNum" width="100" label="显示顺序" align="left" /> -->
        <el-table-column prop="deptName" label="部门名称" min-width="260" :show-overflow-tooltip="true" align="left" />
        <!-- <el-table-column prop="orderNum" label="排序" width="200"></el-table-column> -->
        <el-table-column prop="code" label="部门代码" width="200" :show-overflow-tooltip="true" align="left" />
        <el-table-column prop="leaderName" label="部门负责人" width="200" :show-overflow-tooltip="true" align="left" />
        <el-table-column prop="phone" label="部门电话" width="200" :show-overflow-tooltip="true" align="left" />
        <!-- <el-table-column prop="status" label="状态" width="100">
          <template slot-scope="scope">
            <dict-tag :options="dict.type.sys_normal_disable" :value="scope.row.status"/>
          </template>
        </el-table-column> -->
        <el-table-column label="创建时间" prop="createTime" width="300" align="left">
          <template slot-scope="scope">
            <span>{{ parseTime(scope.row.createTime) }}</span>
          </template>
        </el-table-column>
        <el-table-column label="操作" class-name="small-padding" width="250" align="left">
          <template slot-scope="scope">
            <el-button
              v-hasPermi="['system:dept:query']"
              size="small"
              type="text"
              @click="CheckInfo(scope.row)"
            >详情</el-button>
            <el-button
              v-hasPermi="['system:dept:edit']"
              size="small"
              type="text"
              @click="handleUpdate(scope.row)"
            >编辑</el-button>
            <el-button
              v-hasPermi="['system:dept:add']"
              size="small"
              type="text"
              @click="handleAdd(scope.row)"
            >新建</el-button>
            <el-button
              v-if="scope.row.parentId != 0"
              v-hasPermi="['system:dept:remove']"
              size="small"
              type="text"
              style="color: #EF5826;"
              @click="handleDelete(scope.row)"
            >删除</el-button>
          </template>
        </el-table-column>
      </el-table>

      <!-- 添加或修改部门对话框 -->
      <el-dialog :title="title" :visible.sync="open" :close-on-click-modal="false" width="800px" append-to-body class="open-dialog">
        <el-form ref="form" :model="form" :rules="rules" label-width="193px" style="padding-right: 111px;">
          <el-row>
            <el-col v-if="form.parentId !== 0" :span="24">
              <el-form-item v-if="dialogName==='新建部门'" label="上级部门" prop="parentName">
                <!-- <el-input v-model="form.parentName" placeholder="请选择" maxlength="20" @focus="showDept">
                  <i slot="suffix" class="iconfont icon-bumenxuanze" style="cursor: pointer;" @click="showDept" />
                </el-input> -->
                <div class="my-select-con">
                  <el-select
                    v-model="form.parentName"
                    filterable
                    placeholder="请输入"
                    class="input-width"
                    style="width:456px;"
                    @change="getParentId"
                  >
                    <el-option
                      v-for="item in parentDeptOptions"
                      :key="item.deptId"
                      :label="item.deptName"
                      :value="item.deptId"
                    />
                  </el-select>
                  <i slot="suffix" class="iconfont icon-bumenxuanze" style="cursor: pointer;" @click="showDept" />
                </div>
              </el-form-item>
              <el-form-item v-else label="上级部门" prop="parentName">
                <el-input v-model="form.parentName" placeholder="请输入" maxlength="20" disabled />
              </el-form-item>
            </el-col>
          </el-row>
          <el-row>
            <el-col :span="24">
              <el-form-item label="部门名称" prop="deptName">
                <el-input v-model.trim="form.deptName" maxlength="15" placeholder="请输入" />
              </el-form-item>
            </el-col>
          </el-row>
          <el-row>
            <el-col :span="24">
              <el-form-item label="部门代码" prop="code">
                <!-- oninput="this.value = this.value.replace(/[\*]/g,'');" -->
                <el-input v-model.trim="form.code" oninput="value=value.replace(/[\u4E00-\u9FA5]/ig, '')" maxlength="15" placeholder="请输入" />
              </el-form-item>
            </el-col>
          </el-row>
          <el-row v-if="isNew" key="code_prefix">
            <el-col :span="24">
              <el-form-item label="资产编码前缀" prop="code_prefix">
                <el-input v-model.trim="form.code_prefix" oninput="value=value.replace(/[\u4E00-\u9FA5]/ig, '')" maxlength="15" placeholder="请输入" />
              </el-form-item>
            </el-col>
          </el-row>
          <el-row>
            <el-col :span="24">
              <el-form-item label="显示顺序" prop="orderNum">
                <el-input v-model="form.orderNum" type="text" maxlength="5" onkeyup="if(value==0&&value!=''){value=1}this.value = this.value.replace(/[^0-9]/g, '');" placeholder="请输入" />
              </el-form-item>
            </el-col>
          </el-row>
          <el-row>
            <el-col :span="24">
              <el-form-item label="部门负责人" prop="leader">
                <div class="my-select-con">
                  <el-select
                    v-model="form.leader"
                    multiple
                    filterable
                    placeholder="请输入"
                    class="input-width"
                  >
                    <el-option
                      v-for="item in leaderOptions"
                      :key="item.userId"
                      :label="item.realName"
                      :value="item.userName"
                    />
                  </el-select>
                  <i slot="suffix" class="iconfont icon-people" style="font-size: 14px;line-height: 36px;cursor: pointer;" @click.stop="showLeader" />
                </div>
              </el-form-item>
            </el-col>
          </el-row>
          <el-row>
            <el-col :span="24">
              <el-form-item label="部门电话" prop="phone">
                <el-input v-model="form.phone" oninput="value=value.replace(/[\u4E00-\u9FA5]/ig, '')" placeholder="请输入" maxlength="14" />
              </el-form-item>
            </el-col>
          </el-row>
          <el-row>
            <el-col :span="24">
              <el-form-item label="部门传真" prop="fax">
                <el-input v-model="form.fax" placeholder="请输入" maxlength="13" onkeyup="this.value = this.value.replace(/[^(-\d\d\d)]/g, '');" />
              </el-form-item>
            </el-col>
          </el-row>
          <el-row>
            <el-col :span="24">
              <el-form-item label="部门简介" prop="description">
                <el-input v-if="isReset" v-model="form.description" type="textarea" maxlength="100" placeholder="请输入" />
              </el-form-item>
            </el-col>
          </el-row>

          <!-- <el-row>
            <el-col :span="12">
              <el-form-item label="邮箱" prop="email">
                <el-input v-model="form.email" placeholder="请输入邮箱" maxlength="50" />
              </el-form-item>
            </el-col>
            <el-col :span="12">
              <el-form-item label="部门状态">
                <el-radio-group v-model="form.status">
                  <el-radio
                    v-for="dict in dict.type.sys_normal_disable"
                    :key="dict.value"
                    :label="dict.value"
                  >{{dict.label}}</el-radio>
                </el-radio-group>
              </el-form-item>
            </el-col>
          </el-row> -->
        </el-form>
        <div slot="footer" class="dialog-footer">
          <el-button size="small" @click="cancel">取 消</el-button>
          <el-button size="small" type="primary" @click="submitForm">保 存</el-button>
        </div>
      </el-dialog>
      <!-- 详情弹窗 -->
      <el-dialog title="详情" :close-on-click-modal="false" :visible.sync="checkView" width="600px" append-to-body class="open-dialog">
        <el-form ref="checkInfo" :model="form" label-width="193px" style="padding-right: 111px;">
          <el-row>
            <el-col v-if="form.parentId !== 0" :span="24">
              <el-form-item label="上级部门" prop="parentName">
                <span>{{ checkInfo.parentName?checkInfo.parentName:'--' }}</span>
              </el-form-item>
            </el-col>
          </el-row>
          <el-row>
            <el-col :span="24">
              <el-form-item label="部门名称" prop="deptName">
                <span>{{ checkInfo.deptName?checkInfo.deptName:'--' }}</span>
              </el-form-item>
            </el-col>
          </el-row>
          <el-row>
            <el-col :span="24">
              <el-form-item label="部门代码" prop="code">
                <!-- oninput="this.value = this.value.replace(/[\*]/g,'');" -->
                <span>{{ checkInfo.code?checkInfo.code:'--' }}</span>
              </el-form-item>
            </el-col>
          </el-row>
          <el-row>
            <el-col :span="24">
              <el-form-item label="显示顺序" prop="orderNum">
                <span>{{ checkInfo.orderNum?checkInfo.orderNum:'--' }}</span>
              </el-form-item>
            </el-col>
          </el-row>
          <el-row>
            <el-col :span="24">
              <el-form-item label="部门负责人">
                <span>{{ checkInfo.leaderName?checkInfo.leaderName:'--' }}</span>
              </el-form-item>
            </el-col>
          </el-row>
          <el-row>
            <el-col :span="24">
              <el-form-item label="部门电话" prop="phone">
                <span>{{ checkInfo.phone?checkInfo.phone:'--' }}</span>
              </el-form-item>
            </el-col>
          </el-row>
          <el-row>
            <el-col :span="24">
              <el-form-item label="部门传真" prop="fax">
                <span>{{ checkInfo.fax?checkInfo.fax:'--' }}</span>
              </el-form-item>
            </el-col>
          </el-row>
          <el-row>
            <el-col :span="24">
              <el-form-item label="部门简介" prop="description">
                <span>{{ checkInfo.description?checkInfo.description:'--' }}</span>
              </el-form-item>
            </el-col>
          </el-row>
        </el-form>
      </el-dialog>

      <!-- 展示选择负责人弹框 -->
      <el-dialog title="选择负责人" :close-on-click-modal="false" :visible.sync="IsShowLeader" width="1000px" append-to-body class="leader-dialog" @close="closeLeader">
        <div class="leader-select">
          <div class="dept-left">
            <el-input v-model.trim="deptName" maxlength="15" placeholder="请输入部门名称" size="mini">
              <i slot="suffix" class="iconfont icon-sousuo" @click="deptSearch" />
            </el-input>
            <div class="head-container">
              <el-tree
                ref="tree"
                :data="deptOptions2"
                :props="defaultProps"
                :expand-on-click-node="false"
                :filter-node-method="filterNode"
                node-key="id"
                :default-expanded-keys="expandedArr"
                highlight-current
                @node-click="handleNodeClick"
              />
            </div>
          </div>
          <div class="leader-right">
            <el-input v-model.trim="leaderName" maxlength="15 " placeholder="请输入姓名或工号" size="mini">
              <i slot="suffix" class="iconfont icon-sousuo" @click="deptSearch" />
            </el-input>
            <el-table ref="multipleTable" v-loading="loading2" :row-key="getRowKeys" highlight-current-row :data="userList2" style="margin-top: 10px;" @selection-change="handleSelectionChange">
              <el-table-column type="selection" width="50" :reserve-selection="true" align="center" />
              <el-table-column type="index" width="100" label="序号" />
              <el-table-column key="userName" label="姓名" align="center" prop="realName" :show-overflow-tooltip="true" />
              <el-table-column key="jobNumber" label="工号" align="center" prop="jobNumber" />
              <el-table-column key="posts" show-overflow-tooltip label="职位名称" align="center" prop="posts">
                <template slot-scope="scope">
                  <div v-if="scope.row.posts.length>0">
                    <span v-for="(item,index) in scope.row.posts" :key="item.postId"><span v-if="index==0">{{ item.postName }}</span><span v-else>,{{ item.postName }}</span></span>
                  </div>
                  <div v-else>
                    --
                  </div>
                </template>
              </el-table-column>
            </el-table>
          </div>
        </div>
        <div slot="footer" class="dialog-footer">
          <el-button size="small" @click="ClearChosen">清空已选</el-button>
          <el-button size="small" type="primary" @click="SaveChose">保  存</el-button>
        </div>
      </el-dialog>

      <!-- 展示选择部门弹框 -->
      <el-dialog title="选择部门" :close-on-click-modal="false" :visible.sync="IsShowDept" width="600px" append-to-body class="leader-dialog" @close="closeDept">
        <div class="dept-select">
          <el-input v-model.trim="deptName" placeholder="请输入部门名称" maxlength="15" size="mini" class="search-department">
            <i slot="suffix" class="iconfont icon-sousuo" @click="deptSearch" />
          </el-input>
          <div class="head-container select-dept-tree">
            <el-tree
              ref="tree"
              :data="deptOptions2"
              :props="defaultProps"
              :expand-on-click-node="false"
              :filter-node-method="filterNode"
              node-key="id"
              :default-expanded-keys="expandedArr"
              highlight-current
              @node-click="handleNodeClick"
            />
          </div>
        </div>
        <div slot="footer" class="dialog-footer">
          <el-button size="small" @click="IsShowDept = false">取 消</el-button>
          <el-button size="small" type="primary" @click="SaveChoseDept">保 存</el-button>
        </div>
      </el-dialog>
    </el-card>
  </div>

</template>

<script>
import { listDept, getDept, delDept, addDept, updateDept, listDeptExcludeChild, treeselect } from '@/api/system/dept'
import { listUser } from '@/api/system/user'
// import Treeselect from '@riophae/vue-treeselect'
import '@riophae/vue-treeselect/dist/vue-treeselect.css'
import TagsView from '@/layout/components/TagsView/index.vue'
// import secondVue from '../../../components/Crontab/second.vue'

export default {
  name: 'Dept',
  dicts: ['sys_normal_disable'],
  components: { TagsView },
  data() {
    return {
      // 遮罩层
      loading: true,
      // 显示搜索条件
      showSearch: true,
      // 表格树数据
      deptList: [],
      // 部门树选项
      deptOptions: [],
      // 弹出层标题
      title: '',
      // 是否显示弹出层
      open: false,
      // 是否展开，默认全部展开
      isExpandAll: false,
      // 重新渲染表格状态
      refreshTable: true,
      // 查询参数
      queryParams: {
        deptName: undefined,
        // status: undefined
        leader: undefined, // 部门领导
        phone: undefined // 部门电话
      },
      // 表单参数
      form: {},
      // 表单校验
      rules: {
        parentName: [
          { required: true, message: '请填写上级部门', trigger: 'submit' }
        ],
        deptName: [
          { required: true, message: '请填写部门名称', trigger: 'blur' }
        ],
        // orderNum: [
        //   { required: true, message: '显示排序不能为空', trigger: 'blur' }
        // ],
        // leader: [
        //   { required: true, message: '请填写部门负责人', trigger: 'blur' }
        // ],
        code: [
          { required: true, message: '请填写部门代码', trigger: 'blur' },
          { min: 3, message: '部门代码长度最少为3位', trigger: 'blur' }
        ],
        code_prefix: [
          { required: true, message: '请填写资产编码前缀', trigger: 'blur' }
        ],
        email: [
          {
            type: 'email',
            message: "'请输入正确的邮箱地址",
            trigger: ['blur', 'change']
          }
        ],
        phone: [
          {
            min: 7,
            max: 14,
            message: '请输入正确的部门电话',
            trigger: 'blur'
          }
        ],
        fax: [
          { min: 7, message: '单位传真位数应不少于7位', trigger: 'blur' }
        ]
      },
      // 部门负责人弹框选择
      IsShowLeader: false,
      // 部门关键词搜索
      deptName: '',
      leaderName: '',
      // 部门树选项
      deptOptions2: undefined,
      defaultProps: {
        children: 'children',
        label: 'label'
      },
      // 树形查询参数
      queryParams2: {

      },
      // 用户表格数据
      userList: null,
      userList2: [],
      loading2: true,
      // 选中部门负责人数组
      ids: [],
      IsShowDept: false,
      expandedArr: [],
      dialogName: '',
      // 部门弹窗选中行数据
      leaderListRow: [],
      // 部门表格中的树形结构默认展开
      tableExpandedArr: [],
      rowInfo: {},
      checkInfo: {},
      checkView: false,
      // 部门负责人下拉列表
      leaderOptions: [],
      isReset: true,
      // 上级部门选择数据
      parentDeptOptions: [],
      // 部门负责人真名
      realNameArr: [],
      isNew: false
    }
  },
  watch: {
    // 根据名称筛选部门树
    deptName(val) {
      this.$refs.tree.filter(val)
    },

    leaderName(val) {
      this.userList2 = this.userList.filter(item => {
        if (item.userId === val || item.realName.indexOf(val) !== -1 || item.userName.indexOf(val) !== -1 || item.jobNumber?.indexOf(val) !== -1) {
          return true
        }
      })
    }

  },
  created() {
    this.getList()
    // 获取全部
    listUser({ pageNum: 1, pageSize: 10000 }).then((response) => { this.leaderOptions = response.rows || [] })
  },
  methods: {
    /** 查询部门列表 */
    getList() {
      this.loading = true
      listDept(this.queryParams).then(response => {
        this.parentDeptOptions = response.data
        if (!this.queryParams.deptName && !this.queryParams.leader) {
          this.deptList = this.handleTree(response.data, 'deptId')
        } else {
          this.deptList = response.data
        }
        this.loading = false
        // 遍历找到根节点，设置默认展开
        this.deptList.forEach(item => {
          this.tableExpandedArr.push(item.deptId + '')
        })
        // if (!this.deptList.filter(item => item.deptId === this.rowInfo.deptId)[0]) {
        //   this.deptList.forEach(item1 => {
        //     if (item1.children?.filter(item2 => item2.deptId === this.rowInfo.deptId)[0]) {
        //       this.$refs.deptTable.setCurrentRow(this.deptList.children.filter(item2 => item2.deptId === this.rowInfo.deptId)[0])
        //
        //       return
        //     }
        //   })
        // } else {
        this.$refs.deptTable.setCurrentRow(this.deptList.filter(item => item.deptId === this.rowInfo.deptId)[0])
        // }
      })
    },
    /** 转换部门数据结构 */
    normalizer(node) {
      if (node.children && !node.children.length) {
        delete node.children
      }
      return {
        id: node.deptId,
        label: node.deptName,
        children: node.children
      }
    },
    // 取消按钮
    cancel() {
      this.open = false
      this.reset()
    },
    // 表单重置
    reset() {
      this.form = {
        deptId: undefined,
        parentId: undefined,
        parentName: undefined,
        deptName: undefined,
        orderNum: undefined,
        leader: undefined,
        phone: undefined,
        email: undefined,
        code: undefined,
        fax: undefined,
        description: undefined,
        status: '0'
      }
      this.resetForm('form')
    },
    /** 搜索按钮操作 */
    handleQuery() {
      this.getList()
    },
    /** 重置按钮操作 */
    resetQuery() {
      this.resetForm('queryForm')
      this.handleQuery()
    },
    /** 新增按钮操作 */
    handleAdd(row) {
      this.isNew = true
      this.reset()
      if (row.deptName) {
        this.form.parentId = row.deptId
        this.form.parentName = row.deptName
        // this.form.orderNum = row.children ? row.children.length + 1 : 1
        this.form.orderNum = undefined
        this.dialogName = '新建子部门'
        this.title = '新建子部门'
      } else {
        this.dialogName = '新建部门'
        this.title = '新建部门'
      }
      this.open = true
      listDept().then(response => {
        this.deptOptions = this.handleTree(response.data, 'deptId')
      })
    },
    /** 展开/折叠操作 */
    toggleExpandAll() {
      this.refreshTable = false
      this.isExpandAll = !this.isExpandAll
      this.$nextTick(() => {
        this.refreshTable = true
      })
    },
    /** 修改按钮操作 */
    handleUpdate(row) {
      this.isNew = false
      this.reset()
      this.rowInfo = row
      getDept(row.deptId).then(response => {
        this.form = response.data
        // 修改部门负责人显示
        this.form.leader = this.form.leader ? this.form.leader.split(',') : []
        this.open = true
        this.title = '编辑 '
      })
      listDeptExcludeChild(row.deptId).then(response => {
        this.deptOptions = this.handleTree(response.data, 'deptId')
      })
    },
    CheckInfo(row) {
      this.reset()
      this.checkInfo = row
      this.checkView = true
    },
    /** 提交按钮 */
    submitForm: function() {
      this.$refs['form'].validate(valid => {
        if (valid) {
          const params = { ...this.form }
          if (Array.isArray(params.leader)) {
            params.leader = params.leader.join(',')
          }
          if (Array.isArray(params.leaderName)) {
            params.leaderName = params.leaderName.join(',')
          }
          if (this.form.deptId !== undefined) {
            updateDept(params).then(response => {
              this.$modal.msgSuccess('修改成功')
              this.open = false
              this.isReset = false
              this.$nextTick(() => {
                this.isReset = true
              })
              this.resetForm('queryForm')
              this.getList()
            })
          } else {
            addDept(params).then(response => {
              this.$modal.msgSuccess('保存成功')
              this.open = false
              this.isReset = false
              this.$nextTick(() => {
                this.isReset = true
              })
              this.resetForm('queryForm')
              this.getList()
            })
          }
        }
      })
    },
    /** 删除按钮操作 */
    handleDelete(row) {
      this.$modal.confirm('确定删除部门"' + row.deptName + '"？', '确定删除').then(function() {
        return delDept(row.deptId)
      }).then(() => {
        this.getList()
        this.$modal.msgSuccess('删除成功')
      }).catch(() => {})
    },
    /** 打开负责人选择弹框 */
    showLeader() {
      this.getTreeselect()
      this.getList2()
      this.IsShowLeader = true
    },
    /** 部门搜索 */
    deptSearch() {
    },
    /** 查询部门下拉树结构 */
    getTreeselect() {
      treeselect().then(response => {
        this.deptOptions2 = response.data
        response.data.forEach(item => {
          this.expandedArr.push(item.id)
        })
      })
    },
    // 筛选节点
    filterNode(value, data) {
      if (!value) return true
      return data.label.indexOf(value) !== -1
    },
    // 节点单击事件
    handleNodeClick(data) {
      this.queryParams2.deptId = data.id
      this.queryParams2.label = data.label
      this.deptOptions2.forEach(item => {
        if (item.id === this.queryParams2.deptId) {
          if (!item.children) {
            this.form.orderNum = 1
          } else {
            this.form.orderNum = item.children.length + 1
          }
        }
      })
      this.handleQuery2()
    },
    handleQuery2() {
      this.getList2()
    },
    /** 获取弹窗列表并根据下拉框选中值设置表格选中值状态 */
    getList2() {
      this.loading2 = true
      listUser(this.addDateRange(this.queryParams2)).then(response => {
        this.userList = response.rows
        // 筛选右边搜索框的值
        this.userList2 = this.userList.filter(item => {
          if (item.userId === this.leaderName || item.realName.indexOf(this.leaderName) !== -1 || item.jobNumber?.indexOf(this.leaderName) !== -1) {
            return true
          }
        }) || []
        const leadList = this.form.leader // 部门负责人
        // 获取输入框已选值设置弹窗对于值选中状态
        this.leaderListRow = this.userList2.filter((v) => leadList?.includes(v.userName))
        if (this.leaderListRow) {
          this.leaderListRow.forEach(row => {
            // 设置表格选中行状态
            this.$refs.multipleTable.toggleRowSelection(row)
          })
        } else {
          // 清空用户的选择
          this.$refs.multipleTable.clearSelection()
        }
        this.total = response.total
        this.loading2 = false
      }
      )
    },
    /** 部门负责人表格多选框选中数据*/
    handleSelectionChange(selection) {
      // 返回一个对象数组 ids
      this.ids = selection.map(item => {
        return {
          userName: item.userName,
          realName: item.realName
        }
      })
    },
    ClearChosen(rows) {
      this.$refs.multipleTable.clearSelection()
    },
    /** 保存选中负责人弹窗中选中的值 */
    SaveChose() {
      if (this.ids.length > 5) {
        this.$message.error('最多选择五位负责人')
        return
      }
      this.form.leader = this.ids.map(item => item.userName)
      this.form.leaderName = this.ids.map(item => item.realName)
      this.IsShowLeader = false
      this.$refs.form.validateField('leader')
    },
    closeLeader() {
      this.queryParams2 = {}
      this.deptName = ''
      this.leaderName = ''
      this.$refs.multipleTable.clearSelection()
    },
    /** 部门树弹框 */
    showDept() {
      this.getTreeselect()
      this.IsShowDept = true
    },
    SaveChoseDept() {
      this.IsShowDept = false
      this.form.parentName = this.queryParams2.label
      this.form.parentId = this.queryParams2.deptId
      this.$refs.form.validateField('parentName')
    },
    closeDept() {
      this.queryParams2 = {}
      this.deptName = ''
    },
    getRowKeys(row) {
      return row.userId
    },
    getParentId(data) {
      this.parentDeptOptions.forEach(item => {
        if (data === item.deptId) {
          this.form.parentName = item.deptName
          this.form.parentId = item.deptId
        }
      })
    }
  }
}
</script>
<style scoped lang="scss">
.search-department {
  ::v-deep .el-input__inner{
      height:32px;
      padding-top:2px;
      margin-bottom: 8px
  }
  ::v-deep .el-input__suffix{
        margin-top:2px
      }
}
.select-dept-tree {
  flex: 1;
  overflow: auto;
  padding-bottom: 8px;
}
.icon-sousuo:before{
  content: "\e725";
  line-height: 28px;
  font-size: 22px;
  cursor: pointer;
}
.icon-renyuanxuanze:before {
    content: "\e720";
    font-size: 22px;
}
.leader-select{
  height: 500px;
  display: flex;
}
.dept-left{
  width: 359px;
  height: 100%;
  padding: 16px;
  border-right: 1px solid #DDDDDD;
  overflow-y: scroll;
}
.leader-right{
  width: 650px;
  height: 100%;
  padding: 16px;
  overflow-y: scroll;
}
.dept-select{
  height: 600px;
  display: flex;
  flex-direction: column;
  padding: 16px;
}
.leader-dialog{
  &::v-deep .el-dialog__body{
    padding: 0;
  }
}
::v-deep .el-tree-node__content{
  height: auto;
  padding: 4px 0 ;
}
::v-deep .el-textarea__inner{
  font-family: none;
}
::v-deep .el-input.is-disabled .el-input__inner {
    background-color: #F5F7FA;
    border-color: #dfe4ed;
    color: #C0C4CC;
    cursor: default;
}
::v-deep .el-input.is-disabled .el-input__inner{
  color: #333333;
  cursor: default;
}
.my-select-con{
  .iconfont {
    display: inline-block;
    cursor: pointer;
    width: 22px;
    height: 22px;
    position: absolute;
    right: 6px;
    bottom: 0;
  }
  .icon-people {
    top: 7px;
    background: url("../../../assets/images/people.png") no-repeat;
    background-position: center;
    background-size: 100% 100%;
  }
  .icon-bumenxuanze {
    top:0;
  }
  ::v-deep .el-select {
    width: 100%;
    .el-input__suffix-inner {
      display: none;
    }
  }
}
</style>
