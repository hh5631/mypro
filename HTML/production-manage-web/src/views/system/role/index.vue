<template>
  <div class="role-box">
    <tags-view>
      <el-row :gutter="10" class="mb8">
        <el-col :span="1.5">
          <el-button
            v-hasPermi="['system:role:add']"
            type="primary"
            icon="el-icon-plus"
            size="small"
            @click="handleAdd"
          >
            新建角色</el-button>
        </el-col>
        <el-col :span="1.5">
          <el-button
            v-hasPermi="['system:role:remove']"
            icon="el-icon-delete"
            size="small"
            :disabled="multiple"
            @click="handleDelete"
          >批量删除</el-button>
        </el-col>
      </el-row>
    </tags-view>
    <el-card class="main-card">
      <el-form v-show="showSearch" ref="queryForm" :model="queryParams" :inline="true">
        <el-form-item label="角色名称" prop="roleName">
          <el-input
            v-model.trim="queryParams.roleName"
            placeholder="请输入"
            clearable
            size="small"
            maxlength="15"
            style="width: 200px"
            @keyup.enter.native="handleQuery"
          />
        </el-form-item>
        <el-form-item label="角色代码" prop="roleKey">
          <el-input
            v-model="queryParams.roleKey"
            placeholder="请输入"
            clearable
            size="small"
            maxlength="15"
            style="width: 200px"
            @keyup.enter.native="handleQuery"
          />
        </el-form-item>
        <el-form-item label="状态" prop="status">
          <el-select v-model="queryParams.status" placeholder="请选择" clearable size="small" style="width: 200px">
            <el-option
              v-for="dict in dict.type.sys_normal_disable"
              :key="dict.value"
              :label="dict.label"
              :value="dict.value"
            />
          </el-select>
        </el-form-item>
        <!-- <el-form-item label="创建时间">
      <el-date-picker
        v-model="dateRange"
        size="small"
        style="width: 240px"
        value-format="yyyy-MM-dd"
        type="daterange"
        range-separator="-"
        start-placeholder="开始日期"
        end-placeholder="结束日期"
      ></el-date-picker>
    </el-form-item> -->
        <el-form-item>
          <el-button size="small" @click="resetQuery">重置</el-button>
          <el-button type="primary" plain size="small" class="search-btn" @click="handleQuery">搜索</el-button>
        </el-form-item>
      </el-form>
      <el-table ref="roleTable" v-loading="loading" :data="roleList" highlight-current-row height="100%" @selection-change="handleSelectionChange">
        <el-table-column type="selection" width="55" align="left" />
        <!-- <el-table-column label="显示顺序"  prop="roleSort" align="left" /> -->
        <el-table-column label="角色名称" prop="roleName" :show-overflow-tooltip="true" />
        <el-table-column label="角色代码" prop="roleKey" :show-overflow-tooltip="true" />
        <!-- <el-table-column label="权限字符" prop="roleKey" :show-overflow-tooltip="true" width="150" /> -->
        <!-- <el-table-column label="显示顺序" sortable>
        <template slot-scope="scope">
          <div>
            {{ scope.row.roleSort !== null?scope.row.roleSort:'0' }}
          </div>
        </template>
      </el-table-column> -->
        <el-table-column label="状态" align="left">
          <template slot-scope="scope">
            <!-- <el-switch
          v-model="scope.row.status"
          active-value="0"
          inactive-value="1"
          @change="handleStatusChange(scope.row)"
        ></el-switch> -->
            <div class="statePoint">
              <span v-if="scope.row.status == 0" style="color: #3DB954;"><img
                class="point"
                src="../../../assets/images/green@2x.png"
              >正常</span>
              <span v-else style="color:#EF5826"><img class="point" src="../../../assets/images/red@2x.png">停用</span>
            </div>
          </template>
        </el-table-column>
        <!-- <el-table-column label="创建时间" align="center" prop="createTime" width="180">
      <template slot-scope="scope">
        <span>{{ parseTime(scope.row.createTime) }}</span>
      </template>
    </el-table-column> -->
        <el-table-column label="操作" align="left" class-name="small-padding" width="280">
          <template v-if="scope.row.roleId !== 1" slot-scope="scope">
            <el-button
              v-hasPermi="['system:role:edit']"
              size="small"
              type="text"
              style="color:#4686EF"
              @click="handleUpdate(scope.row)"
            >编辑</el-button>
            <el-button
              v-hasPermi="['system:role:edit']"
              size="small"
              type="text"
              style="color:#4686EF"
              @click="handleUserSet(scope.row)"
            >用户设置</el-button>
            <el-button
              v-hasPermi="['system:role:edit']"
              size="small"
              type="text"
              style="color:#4686EF"
              @click="handleAuthSet(scope.row)"
            >权限设置</el-button>
            <el-button
              v-hasPermi="['system:role:remove']"
              size="small"
              type="text"
              style="color:#EF5826"
              @click="handleDelete(scope.row)"
            >删除</el-button>
            <el-dropdown
              v-hasPermi="['system:role:edit']"
              size="mini"
              @command="(command) => handleCommand(command, scope.row)"
            >
              <span class="el-dropdown-link">
                <i class="el-icon-d-arrow-right el-icon--right" />更多
              </span>
              <el-dropdown-menu slot="dropdown">
                <el-dropdown-item v-hasPermi="['system:role:edit']" command="handleDataScope" icon="el-icon-circle-check">
                  数据权限
                </el-dropdown-item>
                <!-- <el-dropdown-item v-hasPermi="['system:role:edit']" command="handleAuthUser" icon="el-icon-user">分配用户
                </el-dropdown-item> -->
              </el-dropdown-menu>
            </el-dropdown>
          </template>
        </el-table-column>
      </el-table>

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

      <!-- 添加或修改角色配置对话框 -->
      <el-dialog :close-on-click-modal="false" :title="title" :visible.sync="open" width="800px" append-to-body>
        <el-form ref="form" :model="form" :rules="rules" label-width="193px" style="margin-right: 111px;">
          <el-form-item label="角色名称" prop="roleName">
            <el-input v-model.trim="form.roleName" maxlength="15" placeholder="请输入" />
          </el-form-item>
          <el-form-item label="角色代码" prop="roleKey">
            <!-- <span slot="label">
          <el-tooltip content="控制器中定义的权限字符，如：@PreAuthorize(`@ss.hasRole('admin')`)" placement="top">
            <i class="el-icon-question"></i>
          </el-tooltip>
          权限字符
        </span> -->
            <el-input v-model.trim="form.roleKey" oninput="value=value.replace(/[\u4E00-\u9FA5]/ig, '')" maxlength="15" placeholder="请输入" />
          </el-form-item>
          <el-form-item label="显示顺序" prop="roleSort">
            <!-- <el-input-number v-model="form.roleSort" controls-position="right" :min="0" /> -->
            <el-input
              v-model.trim="form.roleSort"
              maxlength="5"
              placeholder="请输入"
              type="text"
              onkeyup="if(value==0&&value!=''){value=1}this.value = this.value.replace(/[^0-9]/g, '');"
            />
          </el-form-item>
          <el-form-item label="角色状态">
            <el-radio-group v-model="form.status">
              <el-radio v-for="dict in dict.type.sys_normal_disable" :key="dict.value" :label="dict.value">{{ dict.label
              }}
              </el-radio>
            </el-radio-group>
          </el-form-item>
          <!-- <el-form-item label="菜单权限">
          <el-checkbox v-model="menuExpand" @change="handleCheckedTreeExpand($event, 'menu')">展开/折叠</el-checkbox>
          <el-checkbox v-model="menuNodeAll" @change="handleCheckedTreeNodeAll($event, 'menu')">全选/全不选</el-checkbox>
          <el-checkbox v-model="form.menuCheckStrictly" @change="handleCheckedTreeConnect($event, 'menu')">父子联动</el-checkbox>
          <el-tree
            class="tree-border"
            :data="menuOptions"
            show-checkbox
            ref="menu"
            node-key="id"
            :check-strictly="!form.menuCheckStrictly"
            empty-text="加载中，请稍候"
            :props="defaultProps"
          ></el-tree>
        </el-form-item>
        <el-form-item label="备注">
          <el-input v-model="form.remark" type="textarea" placeholder="请输入内容"></el-input>
        </el-form-item> -->
        </el-form>
        <div slot="footer" class="dialog-footer">
          <el-button size="small" @click="cancel">取 消</el-button>
          <el-button size="small" type="primary" @click="submitForm">保 存</el-button>
        </div>
      </el-dialog>

      <!-- 权限设置对话框 -->
      <el-dialog :close-on-click-modal="false" title="权限设置" :visible.sync="openRole" width="800px" append-to-body>
        <div class="role-content">
          <div class="role-control">
            <el-input v-model.trim="AuthKey" maxlength="15" placeholder="请输入权限名称" size="small" style="width: 300px;">
              <i slot="suffix" class="el-input__icon el-icon-search" />
            </el-input>
            <div>
              <el-button size="small" @click="handleIsExpand()">展开全部</el-button>
              <el-button size="small" @click="handleCheckedTreeExpand(false, 'menu')">收缩全部</el-button>
              <el-button size="small" @click="handleCheckedTreeNodeAll(true, 'menu')">全部选中</el-button>
              <el-button size="small" @click="handleCheckedTreeNodeAll(false, 'menu')">取消全选</el-button>
            </div>
          </div>
          <div>
            <el-tree
              ref="menu"
              class="tree-border"
              :data="menuOptions"
              show-checkbox
              node-key="id"
              :default-expand-all="isExpand"
              :default-expanded-keys="expandedArr1"
              highlight-current
              :filter-node-method="filterNode"
              :check-strictly="!form.menuCheckStrictly"
              empty-text="暂无数据"
              :props="defaultProps"
              @check-change="handleMenuCheckChange"
            />
          </div>
        </div>
        <div slot="footer" class="dialog-footer">
          <el-button size="small" @click="cancel">取 消</el-button>
          <el-button size="small" type="primary" @click="submitAuthSet">保 存</el-button>
        </div>
      </el-dialog>

      <!-- 用户设置对话框 -->
      <el-dialog :close-on-click-modal="false" title="用户设置" :visible.sync="openUser" width="1200px" append-to-body class="role-dialog">
        <div class="user-content">
          <div class="dept">
            <el-input v-model="DeptKey" placeholder="请输入部门名称" size="small">
              <i slot="suffix" class="el-input__icon el-icon-search" />
            </el-input>
            <el-tree
              ref="dept"
              class="tree-border"
              :data="deptOptions"
              show-checkbox
              default-expand-all
              node-key="id"
              highlight-current
              :filter-node-method="filterNode"
              :check-strictly="!form.deptCheckStrictly"
              empty-text="加载中，请稍候"
              :props="defaultProps"
              @check-change="handleCheckChange"
            >
              <span slot-scope="{ node,data }" class="custom-tree-node">
                <span v-if="data.userId" style="color:red;">{{ node.label }}<span
                  v-if="data.posts.length > 0"
                >(</span><span
                  v-for="(item, index) in data.posts"
                  :key="item.postId"
                ><span v-if="index == 0">{{ item.postName }}</span><span v-else>,{{ item.postName
                }}</span></span><span v-if="data.posts.length > 0">)</span></span>
                <span v-else>{{ node.label }}</span>
              </span>
            </el-tree>
          </div>
          <div class="user">
            <div style="height: 56px;line-height: 56px;">已选用户（{{ checkedUserList.length }}）：</div>
            <div>
              <div v-for="(item, index) in userGroup" :key="item.deptId">
                <div class="tag-title">
                  {{ item.deptName }} ({{ item.rows.length }})
                </div>
                <div class="tag-list">
                  <el-tag
                    v-for="(tag, i) in item.rows"
                    :key="tag.userId"
                    closable
                    class="tagClass"
                    @close="deleteTag(tag, index, i)"
                  >
                    {{ tag.userName }}
                  </el-tag>
                </div>
              </div>
            </div>
          </div>
        </div>
        <div slot="footer" class="dialog-footer">
          <el-button size="small" @click="handleCheckedTreeNodeAll1(false, 'dept')">取消已选</el-button>
          <el-button size="small" type="primary" @click="submitUserSet">保 存</el-button>
        </div>
      </el-dialog>

      <!-- 用户设置弹框 -->
      <el-dialog
        title="用户设置"
        :visible.sync="IsShowUser"
        width="1000px"
        append-to-body
        class="role-dialog"
        :close-on-click-modal="false"
        @close="closeLeader"
      >
        <div class="user-select">
          <div class="dept-left">
            <el-input v-model.trim="deptName" style="width:calc(100% - 16px)" maxlength="15" placeholder="请输入部门名称" size="small">
              <i slot="suffix" class="el-input__icon el-icon-search" />
            </el-input>
            <div class="head-container" style="margin-top:16px;height:calc(100% - 44px);overflow:auto">
              <el-tree
                ref="dept"
                :data="deptOptions2"
                :props="defaultProps"
                :expand-on-click-node="false"
                :filter-node-method="filterNode"
                node-key="id"
                :default-expanded-keys="expandedArr"
                highlight-current
                @node-click="handleNodeClick"
                @check-change="handleCheckChange"
              />
            </div>
          </div>
          <div class="user-right">
            <el-input v-model.trim="leaderName" style="width: 320px;" maxlength="15 " placeholder="请输入姓名或工号" size="small" @keyup.enter.native="searchPeople">
              <i slot="suffix" class="el-input__icon el-icon-search" @click="searchPeople" />
            </el-input>
            <el-table
              ref="rowRef"
              v-loading="loading2"
              :row-key="getRowKeys"
              highlight-current-row
              :data="userList2"
              style="margin-top: 10px;"
              @selection-change="handleRoleSelectionChange"
            >
              <el-table-column type="selection" width="50" :reserve-selection="true" align="left" />
              <el-table-column type="index" width="100" label="序号" />
              <el-table-column key="realName" label="姓名" align="left" prop="realName" :show-overflow-tooltip="true" />
              <el-table-column key="jobNumber" label="工号" align="left" prop="jobNumber" />
              <el-table-column key="leader" show-overflow-tooltip label="职位名称" align="left" prop="leader">
                <template slot-scope="scope">
                  <div v-if="scope.row.posts.length > 0">
                    <span v-for="(item, index) in scope.row.posts" :key="item.postId"><span v-if="index == 0">{{
                      item.postName
                    }}</span><span v-else>,{{ item.postName }}</span></span>
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
          <el-button size="small" type="primary" @click="submitUserSet2">保 存</el-button>
        </div>
      </el-dialog>

      <!-- 分配角色数据权限对话框 -->
      <el-dialog :close-on-click-modal="false" :title="title" :visible.sync="openDataScope" width="500px" append-to-body>
        <el-form :model="form" label-width="80px">
          <el-form-item label="角色名称">
            <el-input v-model="form.roleName" :disabled="true" />
          </el-form-item>
          <el-form-item label="权限字符">
            <el-input v-model="form.roleKey" :disabled="true" />
          </el-form-item>
          <!-- 按需修改 -->
          <div v-for="item in form.dataScopes" :key="item.type">
            <el-form-item :label="labelMap[item.type]">
              <el-select v-model="item.dataScope" @change="dataScopeSelectChange($event,item)">
                <el-option v-for="item in dataScopeOptions" :key="item.value" :label="item.label" :value="item.value" />
              </el-select>
            </el-form-item>
            <el-form-item v-show="item.dataScope == '2'" label="数据权限">
              <el-checkbox v-model="item.deptExpand" @change="handleCheckedTreeExpand($event, 'dept',item.type)">展开/折叠</el-checkbox>
              <el-checkbox v-model="item.deptNodeAll" @change="handleCheckedTreeNodeAll($event, 'dept',item)">全选/全不选</el-checkbox>
              <el-checkbox v-model="item.deptCheckStrictly" @change="handleCheckedTreeConnect($event, 'dept',item.type,item)">父子联动
              </el-checkbox>
              <el-tree
                :ref="item.type"
                class="tree-border"
                :data="deptOptions"
                show-checkbox
                default-expand-all
                node-key="id"
                :check-strictly="!item.deptCheckStrictly"
                empty-text="加载中，请稍候"
                :props="defaultProps"
              />
            </el-form-item>
          </div>
        </el-form>
        <div slot="footer" class="dialog-footer">
          <el-button size="small" @click="cancelDataScope">取 消</el-button>
          <el-button size="small" type="primary" @click="submitDataScope">保 存</el-button>
        </div>
      </el-dialog>
    </el-card>
  </div>

</template>

<script>
import { listRole, getRole, delRole, addRole, updateRole, dataScope, changeRoleStatus, allocatedUserList, authUserBatchAll } from '@/api/system/role'
import { treeselect as menuTreeselect, roleMenuTreeselect } from '@/api/system/menu'
import { treeselect as deptTreeselect, roleDeptTreeselect } from '@/api/system/dept'
import TagsView from '@/layout/components/TagsView/index.vue'
import { listUser } from '@/api/system/user'
// import user from '../../../store/modules/user'

export default {
  name: 'Role',
  components: { TagsView },
  dicts: ['sys_normal_disable'],
  data() {
    return {
      // 遮罩层
      loading: true,
      // 选中数组
      ids: [],
      // 非单个禁用
      single: true,
      // 非多个禁用
      multiple: true,
      // 显示搜索条件
      showSearch: true,
      // 当前选中项
      selectedLength: 0,
      // 总条数
      total: 0,
      // 角色表格数据
      roleList: [],
      // 弹出层标题
      title: '',
      // 是否显示弹出层
      open: false,
      // 是否显示弹出层（数据权限）
      openDataScope: false,
      menuExpand: false,
      menuNodeAll: false,
      deptExpand: true,
      deptNodeAll: false,
      // 日期范围
      dateRange: [],
      // 数据范围选项
      dataScopeOptions: [
        {
          value: '1',
          label: '全部数据权限'
        },
        {
          value: '2',
          label: '自定数据权限'
        },
        {
          value: '3',
          label: '本部门数据权限'
        },
        {
          value: '4',
          label: '本部门及以下数据权限'
        },
        {
          value: '5',
          label: '仅本人数据权限'
        }
      ],
      // 菜单列表
      menuOptions: [],
      // 部门列表
      deptOptions: [],
      // 查询参数
      queryParams: {
        pageNum: 1,
        pageSize: 10,
        roleName: undefined,
        roleKey: undefined,
        status: undefined
      },
      // 表单参数
      form: {
        dataScopes: [
          {
            type: 'system',
            dataScope: '',
            deptIds: []
          },
          {
            type: 'wareGet',
            dataScope: '',
            deptIds: []
          },
          {
            type: 'procurementPlanListVO',
            dataScope: '',
            deptIds: []
          }
        ]
      },
      defaultProps: {
        children: 'children',
        label: 'label'
      },
      // 表单校验
      rules: {
        roleName: [
          { required: true, message: '请填写角色名称', trigger: 'blur' }
        ],
        roleKey: [
          { required: true, message: '请填写角色代码', trigger: 'blur' },
          { min: 3, message: '角色代码长度最少为3位', trigger: 'blur' }
        ]
        // roleSort: [
        //   { required: true, message: '角色顺序不能为空', trigger: 'blur' }
        // ]
      },
      // 权限设置弹窗
      openRole: false,
      // 权限搜索关键词
      AuthKey: '',
      // 用户设置弹窗
      openUser: false,
      IsShowUser: false,
      // 部门搜索关键词
      DeptKey: '',
      // 用户设置弹框的角色id
      roleId: '',
      // 所有用户数据
      userList: null,
      // 用户根据部门分组数据
      userMap: null,
      // 选中的复选框
      checkedList: [],
      // 选中的用户
      checkedUserList: [],
      // 选中用户分组
      userGroup: null,
      // 已选中的角色用户
      roleCheckedUser: [],
      // 要保存的用户id列表
      submitCheckedUser: [],
      userListLength: '',
      expandedArr: [],
      expandedArr1: [],
      deptOptions2: undefined,
      deptName: '',
      // 树形查询参数
      queryParams2: {},
      loading2: true,
      leaderName: '',
      userList2: [],
      leaderListRow: [],
      rowInfo: {},
      isExpand: false,
      isStatus: false,
      selectionName: [],
      arr3: [],
      labelMap:{
        'system':'系统数据权限',
        'wareGet':'领用数据权限',
        'typePurchasePlan':'采购计划'
      }
    }
  },
  watch: {
    // 根据名称筛选菜单树
    AuthKey(val) {
      this.$refs.menu.filter(val)
    },
    // 根据名称筛选部门树
    DeptKey(val) {
      this.$refs.dept.filter(val)
    },
    deptName(val) {
      
      this.$refs.dept.filter(val)
    },
    leaderName(val) {
      this.userList2 = this.userList.filter(item => {
        if (item.userId === val || item.realName.indexOf(val) !== -1 || item.jobNumber.indexOf(val) !== -1) {
          return true
        }
      })
    }
  },
  created() {
    this.getList()
    // this.getAllUser()
  },
  methods: {
    /** 查询角色列表 */
    getList() {
      this.loading = true
      listRole(this.addDateRange(this.queryParams, this.dateRange)).then(response => {
        this.roleList = response.rows
        this.userListLength = Number(response.total) + 1
        this.total = response.total
        this.loading = false
        this.$refs.roleTable.setCurrentRow(this.roleList.filter(item => item.roleId === this.rowInfo.roleId)[0])
      }
      )
    },
    /** 查询菜单树结构 */
    getMenuTreeselect() {
      menuTreeselect().then(response => {
        this.menuOptions = response.data.filter(item => item.visible === '0')
      })
    },
    /** 查询部门树结构 */
    getDeptTreeselect() {
      deptTreeselect().then(response => {
        this.deptOptions = response.data
      })
    },
    // 查询所有用户数据
    getAllUser() {
      listUser().then(response => {
        this.userList = response.rows
        this.userMap = {}
        this.userList.forEach(element => {
          element.label = element.userName
          element.id = 'userId-' + element.userId
          this.userMap[element.deptId] = this.userMap[element.deptId] || []
          this.userMap[element.deptId].push(element)
        })
      }
      )
    },
    // 遍历单个节点,插入用户节点
    traverseNode(node) {
      for (const key in this.userMap) {
        if (node.id === Number(key)) {
          if (node.children) {
            node.children.push(...this.userMap[key])
          } else {
            node.children = []
            node.children.push(...this.userMap[key])
          }
        }
      }
    },
    // 递归遍历树
    traverseTree(node) {
      if (!node) {
        return
      }
      this.traverseNode(node)
      if (node.children && node.children.length > 0) {
        var i = 0
        for (i = 0; i < node.children.length; i++) {
          this.traverseTree(node.children[i])
        }
      }
    },
    // 所有菜单节点数据
    getMenuAllCheckedKeys() {
      // 目前被选中的菜单节点
      const checkedKeys = this.$refs.menu.getCheckedKeys()
      // 半选中的菜单节点
      const halfCheckedKeys = this.$refs.menu.getHalfCheckedKeys()
      checkedKeys.unshift.apply(checkedKeys, halfCheckedKeys)
      return checkedKeys
    },
    // 所有部门节点数据
    getDeptAllCheckedKeys(type) {
      // 目前被选中的部门节点
      const checkedKeys = this.$refs[type][0].getCheckedKeys()
      // 半选中的部门节点
      const halfCheckedKeys = this.$refs[type][0].getHalfCheckedKeys()
      checkedKeys.unshift.apply(checkedKeys, halfCheckedKeys)
      return checkedKeys
    },
    /** 根据角色ID查询菜单树结构 */
    getRoleMenuTreeselect(roleId) {
      return roleMenuTreeselect(roleId).then(response => {
        this.menuOptions = response.menus.filter(item => item.visible === '0')
        // response.menus.forEach(item => {
        //   this.expandedArr1.push(item.id)
        // })
        // return response

        this.traverseTree2(response.menus)
        this.menuOptions = response.menus
        response.menus.forEach(item => {
          this.expandedArr1.push(item.id)
        })
        return response
        // return this.traverseTree2(response.menus)
        // 
      })
    },
    // 遍历单个节点,插入用户节点
    fliterNode(node) {
      for (let i = 0; i < node.length; i++) {
        if (node[i].visible === '1') {
          node.splice(i, 1)
          i--
        }
      }
    },
    // 递归遍历树
    traverseTree2(node) {
      if (!node) {
        return
      }
      this.fliterNode(node)
      node.forEach(item => {
        if (item.children && item.children.length > 0) {
          var i = 0
          for (i = 0; i < item.children.length; i++) {
            this.traverseTree2(item.children)
          }
        }
      })
    },
    /** 根据角色ID查询部门树结构 */
    getRoleDeptTreeselect(roleId) {
      return roleDeptTreeselect(roleId).then(response => {
        this.deptOptions = response.depts
        // this.insertUserNode(this.deptOptions)
        this.deptOptions.forEach(item => {
          this.traverseTree(item)
        })
        return response
      })
    },
    // 角色状态修改
    handleStatusChange(row) {
      const text = row.status === '0' ? '启用' : '停用'
      this.$modal.confirm('确认要"' + text + '""' + row.roleName + '"角色吗？').then(function() {
        return changeRoleStatus(row.roleId, row.status)
      }).then(() => {
        this.$modal.msgSuccess(text + '成功')
      }).catch(function() {
        row.status = row.status === '0' ? '1' : '0'
      })
    },
    // 取消按钮
    cancel() {
      this.open = false
      this.openRole = false
      // this.openUser = false
      this.IsShowUser = false
      this.reset()
    },
    // 取消按钮（数据权限）
    cancelDataScope() {
      this.openDataScope = false
      this.reset()
    },
    // 表单重置
    reset() {
      if (this.$refs.menu !== undefined) {
        this.$refs.menu.setCheckedKeys([])
      }
      this.roleCheckedUser = []
      this.menuExpand = false
      this.menuNodeAll = false
      this.deptExpand = true
      this.deptNodeAll = false
      this.form = {
        roleId: undefined,
        roleName: undefined,
        roleKey: undefined,
        roleSort: undefined,
        status: '0',
        // menuIds: [],
        // 按需修改
        dataScopes: [
        ],
        deptIds: [],
        menuCheckStrictly: true,
        deptCheckStrictly: false,
        remark: undefined
      }
      this.resetForm('form')
    },
    /** 搜索按钮操作 */
    handleQuery() {
      this.queryParams.pageNum = 1
      this.getList()
    },
    /** 重置按钮操作 */
    resetQuery() {
      this.dateRange = []
      this.resetForm('queryForm')
      this.handleQuery()
    },
    // 多选框选中数据
    handleSelectionChange(selection) {
      this.selectedLength = selection.length
      this.ids = selection.map(item => item.roleId)
      this.selectionName = selection.map(item => item.roleName)
      this.single = selection.length !== 1
      this.multiple = !selection.length
    },
    // 更多操作触发
    handleCommand(command, row) {
      switch (command) {
        case 'handleDataScope':
          this.handleDataScope(row)
          break
        case 'handleAuthUser':
          this.handleAuthUser(row)
          break
        default:
          break
      }
    },
    // 树权限（展开/折叠）
    handleCheckedTreeExpand(value, type, dataScopeType) {
      if (type === 'menu') {
        const treeList = this.menuOptions
        for (let i = 0; i < treeList.length; i++) {
          this.$refs.menu.store.nodesMap[treeList[i].id].expanded = value
        }
      } else if (type === 'dept') {
        const treeList = this.deptOptions
        // for (let i = 0; i < treeList.length; i++) {
        //   
        //   const domRef = this.$refs[dataScopeType]
        //   domRef.store.nodesMap[treeList[i].id].expanded = value
        // }
        const domRef = this.$refs[dataScopeType][0]
        for (let i = 0; i < domRef.store._getAllNodes().length; i++) {
          domRef.store._getAllNodes()[i].expanded = value
        }
      }
    },
    handleIsExpand() {
      this.isExpand = true
      this.buildData()
    },
    buildData() {
      for (let i = 0; i < this.$refs.menu.store._getAllNodes().length; i++) {
        this.$refs.menu.store._getAllNodes()[i].expanded = this.isExpand
      }
    },
    // 树权限（全选/全不选）
    handleCheckedTreeNodeAll(value, type, item) {
      if (type === 'menu') {
        this.$refs.menu.setCheckedNodes(value ? this.menuOptions : [])
      } else if (type === 'dept') {
        // this.$refs[item.type][0].setCheckedNodes(value ? this.deptOptions : [])
        const domRef = this.$refs[item.type][0]
        
        for (let i = 0; i < domRef.store._getAllNodes().length; i++) {
          domRef.store._getAllNodes()[i].checked = value
        }
      }
    },
    handleCheckedTreeNodeAll1(value) {
      this.$refs.dept.setCheckedNodes([])
    },
    // 树权限（父子联动）
    handleCheckedTreeConnect(value, type, item) {
      if (type === 'menu') {
        this.form.menuCheckStrictly = !!value
      } else if (type === 'dept') {
        item.deptCheckStrictly = !!value
      }
    },
    /** 新增按钮操作 */
    handleAdd() {
      this.reset()
      // this.getMenuTreeselect()
      // this.form.roleSort = this.userListLength
      this.open = true
      this.title = '新建角色'
    },
    /** 修改按钮操作 */
    handleUpdate(row) {
      this.reset()
      this.rowInfo = row
      const roleId = row.roleId || this.ids
      // const roleMenu = this.getRoleMenuTreeselect(roleId)
      /** 修改角色编辑*/
      /** 要将角色编辑拆分成基本编辑和权限设置*/
      getRole(roleId).then(response => {
        // 角色详情显示
        this.form = response.data

        this.open = true
        this.isStatus = response.data.status === '0'
        // this.$nextTick(() => {
        //   roleMenu.then(res => {
        //     
        //     let checkedKeys = res.checkedKeys
        //     checkedKeys.forEach((v) => {
        //         this.$nextTick(()=>{
        //             
        //             this.$refs.menu.setChecked(v, true ,false);
        //         })
        //     })
        //   });
        // });
        this.title = '编辑角色'
      })
    },
    /** 选择角色权限范围触发 */
    dataScopeSelectChange(value, item) {
      
      if (value !== '2') {
        this.$refs[item.type][0].setCheckedKeys([])
        // this.$refs[`${item.type}`].setCheckedKeys([])
      }
    },
    // 回显树结构
    setSelectedTreeData(dataScopes) {
      if (Array.isArray(dataScopes) && dataScopes.length) {
        dataScopes.forEach(dataScope => {
          if (dataScope.deptIds) {
            this.$nextTick(() => {
              const domRef = this.$refs[dataScope.type][0]
              for (let i = 0; i < domRef.store._getAllNodes().length; i++) {
                const node = domRef.store._getAllNodes()[i]
                
                if (dataScope.deptIds.includes(node.data.id)) {
                  node.checked = true
                }
              }
            })
          }
        })
      }
    },
    /** 分配数据权限操作 */
    handleDataScope(row) {
      // 清空原有数据
      this.reset()
      const roleDeptTreeselect = this.getRoleDeptTreeselect(row.roleId)
      getRole(row.roleId).then(response => {
        const defaultScopes = Object.keys(this.labelMap).map(label=>{
          return {
            type:label
          }
        })
        this.form = {
          ...response.data,
          dataScopes: (response.data.dataScopes && response.data.dataScopes.length !== 0 && defaultScopes.map(scope=>{
            const filterScope = response.data.dataScopes.filter(item=>item.type === scope.type)
            return {
              ...scope,
              ...(filterScope && filterScope[0])
            }
          })
        )
          || [
            {
              type: 'system',
              dataScope: '',
              deptIds: [],
              deptExpand: true,
              deptNodeAll: false,
              deptCheckStrictly: false
            },
            {
              type: 'wareGet',
              dataScope: '',
              deptIds: [],
              deptExpand: true,
              deptNodeAll: false,
              deptCheckStrictly: false
            },
            {
              type: 'typePurchasePlan',
              dataScope: '',
              deptIds: [],
              deptExpand: true,
              deptNodeAll: false,
              deptCheckStrictly: false
            }
          ]
        }
        // 回显树结构
        this.setSelectedTreeData(response.data.dataScopes)
        this.openDataScope = true
        // this.$nextTick(() => {
        //   roleDeptTreeselect.then(res => {
        //     // this.$refs.dept.setCheckedKeys(res.checkedKeys)
        //   })
        // })
        this.title = '分配数据权限'
      })
    },
    /** 分配用户操作 */
    handleAuthUser: function(row) {
      const roleId = row.roleId
      this.$router.push('/system/role-auth/user/' + roleId)
    },
    /** 保存按钮 */
    submitForm: function() {
      // this.form.menuIds = []
      this.$refs['form'].validate(valid => {
        if (valid) {
          if (this.form.roleId !== undefined) {
            if (this.form.roleSort === '0') {
              this.form.roleSort = '1'
            }
            // this.form.menuIds = []
            // this.form.menuIds = this.getMenuAllCheckedKeys()
            updateRole(this.form).then(response => {
              if (this.form.status === '1' && this.isStatus === true) {
                this.$confirm('确定停用角色' + '"' + this.form.roleName + '"' + '?', '确定停用', {
                  confirmButtonText: '确定',
                  cancelButtonText: '取消',
                  type: 'warning'
                }).then(() => {
                  this.$modal.msgSuccess('保存成功')
                  this.resetForm('queryForm')
                  this.open = false
                  this.getList()
                }).catch(() => {
                })
              } else {
                this.$modal.msgSuccess('保存成功')
                this.resetForm('queryForm')
                this.open = false
                this.getList()
              }
            })
          } else {
            this.form.menuIds = []
            // this.form.menuIds = this.getMenuAllCheckedKeys()
            if (!this.form.roleSort) {
              this.form.roleSort = this.userListLength
            }
            addRole(this.form).then(response => {
              this.$modal.msgSuccess('保存成功')
              this.resetForm('queryForm')
              this.open = false
              this.getList()
            })
          }
        }
      })
    },
    /** 保存按钮（数据权限） */
    submitDataScope: function() {
      if (this.form.roleId !== undefined) {
        // this.form.deptIds = this.getDeptAllCheckedKeys()
        dataScope({
          ...this.form,
          dataScopes: this.form.dataScopes.map(dataScope => {
            return {
              ...dataScope,
              deptIds: dataScope['dataScope'] !== '2' ? [] : this.getDeptAllCheckedKeys(dataScope.type)
            }
          })
        }).then(response => {
          this.$modal.msgSuccess('保存成功')
          this.openDataScope = false
          this.getList()
        })
      }
    },
    /** 删除按钮操作 */
    handleDelete(row) {
      const roleIds = row.roleId || this.ids
      const roleName = row.roleName || this.selectionName
      this.$modal.confirm('确定删除角色"' + roleName + '"？', '确定删除').then(function() {
        return delRole(roleIds)
      }).then(() => {
        this.getList()
        this.$modal.msgSuccess('删除成功')
      }).catch(() => { })
    },
    /** 导出按钮操作 */
    handleExport() {
      this.download('system/role/export', this.ids, `role_${new Date().getTime()}.xlsx`)
    },
    /** 权限设置*/
    handleAuthSet(row) {
      this.reset()
      const roleId = row.roleId || this.ids
      const roleMenu = this.getRoleMenuTreeselect(roleId)
      getRole(roleId).then(response => {
        this.form = response.data
        this.openRole = true
        this.$nextTick(() => {
          roleMenu.then(res => {
            const checkedKeys = res.checkedKeys
            checkedKeys.forEach((v) => {
              this.$nextTick(() => {
                this.$refs.menu.setChecked(v, true, false)
              })
            })
          })
        })
      })
    },
    /** 权限设置保存*/
    submitAuthSet() {
      if (this.form.roleId !== undefined) {
        this.form.menuIds = this.getMenuAllCheckedKeys()
        updateRole(this.form).then(response => {
          if (response.code === 200) {
            this.$message({
              message: '保存成功',
              type: 'success',
              duration: 1000
            })
          } else {
            this.$message({
              message: '保存失败' + response.msg,
              type: 'error',
              duration: 1000
            })
          }
          this.openRole = false
          this.getList()
        })
      } else {
        this.form.menuIds = this.getMenuAllCheckedKeys()
        addRole(this.form).then(response => {
          if (response.code === 200) {
            this.$message({
              message: '保存成功',
              type: 'success',
              duration: 1000
            })
          } else {
            this.$message({
              message: '保存失败' + response.msg,
              type: 'error',
              duration: 1000
            })
          }
          this.oopenRolepopenRoleen = false
          this.getList()
        })
      }
    },
    /** 筛选节点*/
    filterNode(value, data) {
      if (!value) return true
      return data.label.indexOf(value) !== -1
    },
    /** 用户设置*/
    handleUserSet(row) {
      this.reset()
      this.roleId = row.roleId
      const roleDeptTreeselect = this.getRoleDeptTreeselect(row.roleId)
      allocatedUserList({ roleId: row.roleId }).then(res => {
        // this.openUser = true
        this.IsShowUser = true
        this.$nextTick(() => {
          if (res.rows) {
            res.rows.forEach(row => {
              this.$refs.rowRef.toggleRowSelection(row)
            })
          } else {
            this.$refs.rowRef.clearSelection()
          }
        })

        this.getTreeselect()
        this.getList2()
        res.rows.forEach(item => {
          this.roleCheckedUser.push('userId-' + item.userId)
        })
        roleDeptTreeselect.then(res => {
          this.$refs.dept.setCheckedKeys(this.roleCheckedUser)
        })
      })
    },
    // 节点复选框选中事件
    handleCheckChange(data, checked, indeterminate) {
      // 
      // 所有被选中的节点
      this.checkedList = this.$refs.dept.getCheckedNodes(true, false)
      this.checkedUserList = []
      this.checkedList.forEach((item) => {
        if (item.userName) {
          this.checkedUserList.push(item)
        }
      })
      var newData = []
      this.checkedUserList.forEach(function(v, i) {
        var n
        for (let i = 0; i < newData.length; i++) {
          var t = newData[i]
          if (t.deptId === v.deptId) {
            n = t
            break
          }
        }
        if (!n) {
          n = { deptId: v.deptId, deptName: v.dept.deptName, rows: [] }
          newData.push(n)
        }
        n.rows.push(v)
      })
      this.userGroup = newData
    },
    handleMenuCheckChange(data, checked) {
      const arr = this.readNodes(this.menuOptions)
      const arr2 = []
      const arr3 = []
      if (checked === true) {
        arr.forEach(item => {
          if (item.id === data.id) {
            arr2.push(item)
          }
        })
        arr.forEach(item => {
          if (item.parentId === arr2[0].parentId) {
            arr3.push(item)
            this.arr3 = arr3
          }
        })
        arr3.forEach(item => {
          if (item.label.indexOf('查询') !== -1) {
            this.$refs.menu.setChecked(item, true)
          }
        })
      }
      if (data.label.indexOf('查询') !== -1 && checked === false) {
        this.arr3.forEach(item => {
          this.$refs.menu.setChecked(item, false)
        })
      }
    },
    // 删除用户
    deleteTag(tag, index, i) {
      this.checkedList.forEach((item, index) => {
        if (item.userId === tag.userId) {
          this.checkedList.splice(index, 1)
        }
      })
      this.$refs.dept.setCheckedNodes(this.checkedList)
    },
    // 保存用户设置
    submitUserSet() {
      this.submitCheckedUser = []
      this.checkedUserList.forEach(item => {
        this.submitCheckedUser.push(item.userId)
      })
      const userIds = this.submitCheckedUser.join(',') || ''
      // if (userIds === '') {
      //   this.$modal.msgError('请选择要分配的用户')
      //   return
      // }
      authUserBatchAll({ roleId: this.roleId, userIds: userIds }).then(res => {
        this.$modal.msgSuccess(res.msg)
        if (res.code === 200) {
          // this.openUser = false
          this.IsShowUser = false
        }
      })
    },
    handleRoleSelectionChange(selection) {
      this.submitCheckedUser = []
      selection.forEach(item => {
        this.submitCheckedUser.push(item.userId)
      })
    },
    submitUserSet2() {
      const userIds = this.submitCheckedUser.join(',') || ''
      // if (userIds === '') {
      //   this.$modal.msgError('请选择要分配的用户')
      //   return
      // }
      authUserBatchAll({ roleId: this.roleId, userIds: userIds }).then(res => {
        this.$modal.msgSuccess(res.msg)
        if (res.code === 200) {
          // this.openUser = false
          this.IsShowUser = false
        }
      })
    },
    closeLeader() {
      this.queryParams2 = {}
      this.deptName = ''
      this.leaderName = ''
      this.$refs.rowRef.clearSelection()
    },
    /** 查询部门下拉树结构 */
    getTreeselect() {
      deptTreeselect().then(response => {
        this.deptOptions2 = response.data
        response.data.forEach(item => {
          this.expandedArr.push(item.id)
        })
      })
    },
    handleNodeClick(data) {
      this.queryParams2.deptId = data.id
      this.queryParams2.label = data.label
      this.handleQuery2()
    },
    handleQuery2() {
      this.getList2()
    },
    // 右侧搜索
    searchPeople() {
      this.userList2 = this.userList.filter(item => {
        if (item.realName.indexOf(this.leaderName) !== -1 || item.jobNumber.indexOf(this.leaderName) !== -1) {
          return true
        }
      })
    },
    getList2() {
      this.loading2 = true
      listUser(this.addDateRange(this.queryParams2)).then(response => {
        this.userList = response.rows
        this.userList2 = this.userList
        this.searchPeople()

        // const leadList = this.form.leader?.split(',')
        // this.leaderListRow = this.userList2.filter((v) => leadList?.includes(v.userName))
        // if (this.leaderListRow) {
        //   
        //   this.leaderListRow.forEach(row => {
        //     this.$refs.multipleTable.toggleRowSelection(row)
        //   })
        // } else {
        //   this.$refs.multipleTable.clearSelection()
        // }
        // 
        this.total = response.total
        // this.form.orderNum = this.total + 1
        this.loading2 = false
      }
      )
    },
    getRowKeys(row) {
      return row.userId
    },
    ClearChosen(rows) {
      this.$refs.rowRef.clearSelection()
    },
    readNodes(nodes = [], arr = []) {
      for (const item of nodes) {
        // if (item.parentId === item.id) {
        //   arr.push(item.id)
        // }
        arr.push(item)
        if (item.children && item.children.length) {
          this.readNodes(item.children, arr)
        }
      }
      return arr
    }
  }
}
</script>
<style lang="scss" scoped>
.role-box {
  // .chooseCount {
  //   color: #7d8592;
  //   margin-top: 20px;
  // }
  // .common-pagination {
  //   display: flex;
  //   justify-content: space-between;
  //   align-items: center;
  //   border-top: 1px solid #eaedf4;
  //   margin-top: 6px;
  // }
}
.role-dialog {
  &::v-deep .el-dialog__body {
    padding: 0;
  }
}

.role-content {
  .role-control {
    display: flex;
    justify-content: space-between;

    .dept {
      height: 100%;
      overflow: scroll;
      width: 400px;
      padding: 16px;
      border-right: 1px solid #DDDDDD;
    }

    .user {
      height: 100%;
      overflow: scroll;
      width: 800px;
      padding: 16px;
    }

    .tag-list {
      margin-bottom: 10px;
    }

    .tag-title {
      margin-bottom: 10px;
      width: 100%;
      height: 44px;
      line-height: 44px;
      background-color: #F1F2F5;
      padding: 0 15px;
    }

    .tagClass {
      width: 80px;
      height: 40px;
      line-height: 40px;
      text-align: center;
      background: #FFFFFF;
      color: #333333;
      border-radius: 2px;
      border: 1px solid #DDDDDD;
      margin: 0 5px;
    }

    &::v-deep .el-tag .el-tag__close {
      color: #333333;
    }
  }
}

.statePoint .point {
  width: 8px;
  height: 8px;
  margin: 0 8px 2px 0;
}

// ::v-deep .el-tree-node.is-expanded.is-focusable>.el-tree-node__children{
//   font-weight: bold;
// }
.user-select {
  height: 500px;
  display: flex;
}

.dept-left {
  width: 359px;
  height: 100%;
  padding:16px 0 0 16px;
  border-right: 1px solid #DDDDDD;
}

.user-right {
  width: 650px;
  height: 100%;
  padding: 16px;
  overflow-y: scroll;
}
::v-deep .el-dialog .el-dialog__body{
  max-height: 600px;
  overflow-y: auto;
}
</style>
