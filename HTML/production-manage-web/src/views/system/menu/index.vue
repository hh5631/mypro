<template>
  <div>
    <tags-view>
      <el-row :gutter="10" class="mb8">
        <el-col :span="1.5">
          <el-button v-hasPermi="['system:menu:add']" type="primary" icon="el-icon-plus" size="small" @click="handleAdd">
            新建菜单</el-button>
        </el-col>
        <right-toolbar :show-search.sync="showSearch" @queryTable="getList" />
      </el-row>
    </tags-view>
    <el-card class="main-card">
      <el-form v-show="showSearch" ref="queryForm" :model="queryParams" :inline="true">
        <el-form-item label="菜单名称" prop="menuName">
          <el-input
            v-model="queryParams.menuName"
            placeholder="请输入"
            clearable
            size="small"
            style="width: 200px"
            @keyup.enter.native="handleQuery"
          />
        </el-form-item>
        <el-form-item label="菜单类型" prop="menuType">
          <el-select v-model="queryParams.menuType" style="width: 200px" clearable size="small">
            <el-option label="目录" value="M">目录</el-option>
            <el-option label="菜单" value="C">菜单</el-option>
            <el-option label="按钮" value="F">按钮</el-option>
          </el-select>
        </el-form-item>
        <el-form-item label="状态" prop="visible">
          <el-select v-model="queryParams.visible" style="width: 200px" placeholder="请选择" clearable size="small">
            <el-option
              v-for="dict_ in dict.type.sys_show_hide"
              :key="dict_.value"
              :label="dict_.label"
              :value="dict_.value"
            />
          </el-select>
        </el-form-item>
        <el-form-item>
          <el-button size="small" @click="resetQuery">重置</el-button>
          <el-button type="primary" class="search-btn" plain size="small" @click="handleQuery">搜索</el-button>
        </el-form-item>
        <el-form-item class="expand">
          <el-button type="primary" class="search-btn" plain size="small" @click="toggleExpandAll(true)">全部展开</el-button>
          <el-button type="primary" class="search-btn" plain size="small" @click="toggleExpandAll(false)">全部合并</el-button>
        </el-form-item>
      </el-form>

      <el-table
        v-if="refreshTable"
        ref="menuTable"
        v-loading="loading"
        :data="menuList"
        row-key="menuId"
        class="menu-table"
        height="100%"
        highlight-current-row
        :default-expand-all="isExpandAll"
        :expand-row-keys="menuList"
        :tree-props="{children: 'children', hasChildren: 'hasChildren'}"
      >
        <!-- <el-table-column prop="orderNum" label="显示顺序" width="100" :show-overflow-tooltip="true" /> -->
        <el-table-column prop="menuName" label="菜单名称" :show-overflow-tooltip="true" width="300" />
        <!-- <el-table-column prop="icon" label="图标" align="center" width="100">
          <template slot-scope="scope">
            <svg-icon :icon-class="scope.row.icon" />
          </template>
        </el-table-column> -->
        <el-table-column prop="perms" label="权限代码" align="left" :show-overflow-tooltip="true" width="200" />
        <el-table-column
          prop="menuType"
          label="菜单类型"
          align="left"
          width="200"
          :show-overflow-tooltip="true"
        >
          <template slot-scope="scope">
            <div>
              <div v-if="scope.row.menuType === 'M'">目录</div>
              <div v-else-if="scope.row.menuType === 'C'">菜单</div>
              <div v-else-if="scope.row.menuType === 'F'">按钮</div>
            </div>
          </template>
        </el-table-column>
        <el-table-column prop="path" label="路由地址" :show-overflow-tooltip="true" width="300" />
        <!-- <el-table-column prop="component" label="组件路径" :show-overflow-tooltip="true"></el-table-column> -->
        <el-table-column
          prop="visible"
          label="菜单状态"
          width="200"
        >
          <template slot-scope="scope">
            <div>
              <span v-if="scope.row.visible === '0'">显示</span>
              <span v-else>隐藏</span>
            </div>
            <!-- <dict-tag :options="dict.type.sys_normal_disable" :value="scope.row.status"/> -->
          </template>
        </el-table-column>
        <!-- <el-table-column label="创建时间" align="center" prop="createTime">
          <template slot-scope="scope">
            <span>{{ parseTime(scope.row.createTime) }}</span>
          </template>
        </el-table-column> -->
        <el-table-column label="操作" align="left" class-name="small-padding">
          <template slot-scope="scope">
            <el-button
              v-hasPermi="['system:menu:edit']"
              size="small"
              type="text"
              style="color:#4686EF"
              @click="handleUpdate(scope.row)"
            >编辑</el-button>
            <el-button
              v-hasPermi="['system:menu:add']"
              size="small"
              type="text"
              style="color:#4686EF"
              @click="handleAdd(scope.row)"
            >新增</el-button>
            <el-button
              v-hasPermi="['system:menu:remove']"
              size="small"
              type="text"
              style="color:#EF5826"
              @click="handleDelete(scope.row)"
            >删除</el-button>
          </template>
        </el-table-column>
      </el-table>

      <!-- 添加或修改菜单对话框 -->
      <el-dialog :close-on-click-modal="false" :title="title" :visible.sync="open" width="800px" :top="form.menuType==='M'?'15vh':form.menuType==='C'?'10vh':'20vh'" append-to-body class="menu-dialog">
        <el-form ref="form" :model="form" :rules="rules" label-width="193px" style="margin-right: 111px;">
          <el-row>
            <el-col :span="24">
              <el-form-item key="parentId" label="上级菜单">
                <treeselect
                  v-model="form.parentId"
                  :options="menuOptions"
                  :normalizer="normalizer"
                  :show-count="true"
                  placeholder="选择上级菜单"
                  :disabled="isDisabled"
                />
              </el-form-item>
            </el-col>
            <el-col :span="24">
              <el-form-item key="menuName" label="菜单名称" prop="menuName">
                <el-input v-model="form.menuName" maxlength="15" placeholder="请输入" />
              </el-form-item>
            </el-col>
            <el-col :span="24">
              <el-form-item key="menuType" label="菜单类型" prop="menuType">
                <el-radio-group v-model="form.menuType">
                  <el-radio label="M">目录</el-radio>
                  <el-radio label="C">菜单</el-radio>
                  <el-radio label="F">按钮</el-radio>
                </el-radio-group>
              </el-form-item>
            </el-col>
            <el-col v-if="form.menuType != 'F'" key="form.icon" :span="24">
              <el-form-item key="icon" label="菜单图标" prop="icon">
                <el-popover placement="bottom-start" width="460" trigger="click" @show="$refs['iconSelect'].reset()">
                  <IconSelect ref="iconSelect" @selected="selected" />
                  <el-input slot="reference" v-model="form.icon" placeholder="点击选择图标" readonly>
                    <svg-icon
                      v-if="form.icon"
                      slot="prefix"
                      :icon-class="form.icon"
                      class="el-input__icon"
                      style="height: 32px;width: 16px;"
                    />
                    <i v-if="form.icon" slot="suffix" class="el-icon-circle-close" @click.stop="form.icon=''" />
                    <i v-else slot="prefix" class="el-icon-search el-input__icon" />
                  </el-input>
                </el-popover>
              </el-form-item>
            </el-col>
            <el-col :span="24">
              <el-form-item key="perms" prop="perms">
                <!-- 不能输入中文 oninput="value=value.replace(/[\u4E00-\u9FA5]/ig, '')"  -->
                <el-input v-model="form.perms" placeholder="请输入权限标识" oninput="value=value.replace(/[\u4E00-\u9FA5]/ig, '')" maxlength="60" />
                <span slot="label">
                  <el-tooltip content="控制器中定义的权限字符，如：@PreAuthorize(`@ss.hasPermi('system:user:list')`)" placement="top" />
                  权限代码
                </span>
              </el-form-item>
            </el-col>
            <el-col :span="24">
              <el-form-item key="orderNum" label="显示排序" prop="orderNum">
                <!-- <el-input-number v-model="form.orderNum" controls-position="right" :min="0"/> -->
                <el-input v-model="form.orderNum" maxlength="5" type="text" placeholder="请输入" onkeyup="if(value==0&&value!=''){value=1}this.value = this.value.replace(/[^0-9]/g, '');" />
              </el-form-item>
            </el-col>
            <el-col v-if="form.menuType != 'F'" key="form.isFrame" :span="24">
              <el-form-item key="isFrame">
                <span slot="label">
                  <el-tooltip content="选择是外链则路由地址需要以`http(s)://`开头" placement="top" />
                  是否外链
                </span>
                <el-radio-group v-model="form.isFrame">
                  <el-radio label="0">是</el-radio>
                  <el-radio label="1">否</el-radio>
                </el-radio-group>
              </el-form-item>
            </el-col>
            <el-col v-if="form.menuType != 'F'" key="form.path" :span="24">
              <el-form-item key="path" prop="path">
                <span slot="label">
                  <el-tooltip content="访问的路由地址，如：`user`，如外网地址需内链访问则以`http(s)://`开头" placement="top" />
                  路由地址
                </span>
                <el-input v-model="form.path" placeholder="请输入路由地址" />
              </el-form-item>
            </el-col>
            <el-col v-if="form.menuType === 'C'" :key="form.component" :span="24">
              <el-form-item key="component" prop="component">
                <span slot="label">
                  <el-tooltip content="访问的组件路径，如：`system/user/index`，默认在`views`目录下" placement="top" />
                  组件路径
                </span>
                <el-input v-model="form.component" placeholder="请输入组件路径" />
              </el-form-item>
            </el-col>
            <el-col v-if="form.menuType === 'C'" :key="form.query" :span="24">
              <el-form-item key="query">
                <el-input v-model="form.query" placeholder="请输入路由参数" maxlength="255" />
                <span slot="label">
                  <el-tooltip content="访问路由的默认传递参数，如：`{&quot;id&quot;: 1, &quot;name&quot;: &quot;jri&quot;}`" placement="top" />
                  路由参数
                </span>
              </el-form-item>
            </el-col>
            <!-- <el-col :span="12" v-if="form.menuType === 'C'">
              <el-form-item>
                <span slot="label">
                  <el-tooltip content="选择是则会被`keep-alive`缓存，需要匹配组件的`name`和地址保持一致" placement="top">
                  <i class="el-icon-question"></i>
                  </el-tooltip>
                  是否缓存
                </span>
                <el-radio-group v-model="form.isCache">
                  <el-radio label="0">缓存</el-radio>
                  <el-radio label="1">不缓存</el-radio>
                </el-radio-group>
              </el-form-item>
            </el-col> -->
            <el-col v-if="form.menuType !== 'F'" key="form.visible" :span="24">
              <el-form-item key="visible">
                <span slot="label">
                  <!-- <el-tooltip content="选择隐藏则路由将不会出现在侧边栏，但仍然可以访问" placement="top">
                  <i class="el-icon-question"></i>
                  </el-tooltip> -->
                  状态
                </span>
                <el-radio-group v-model="form.visible">
                  <el-radio
                    v-for="dict_ in dict.type.sys_show_hide"
                    :key="dict_.value"
                    :label="dict_.value"
                  >{{ dict_.label }}</el-radio>
                </el-radio-group>
              </el-form-item>
            </el-col>
            <!-- <el-col :span="12" v-if="form.menuType !== 'F'">
              <el-form-item>
                <span slot="label">
                  <el-tooltip content="选择停用则路由将不会出现在侧边栏，也不能被访问" placement="top">
                  <i class="el-icon-question"></i>
                  </el-tooltip>
                  菜单状态
                </span>
                <el-radio-group v-model="form.status">
                  <el-radio
                    v-for="dict in dict.type.sys_normal_disable"
                    :key="dict.value"
                    :label="dict.value"
                  >{{dict.label}}</el-radio>
                </el-radio-group>
              </el-form-item>
            </el-col> -->
          </el-row>
        </el-form>
        <div slot="footer" class="dialog-footer">
          <el-button size="small" @click="cancel">取 消</el-button>
          <el-button size="small" type="primary" @click="submitForm">保 存</el-button>
        </div>
      </el-dialog>
    </el-card>
  </div>

</template>

<script>
import { listMenu, getMenu, delMenu, addMenu, updateMenu } from '@/api/system/menu'
import Treeselect from '@riophae/vue-treeselect'
import '@riophae/vue-treeselect/dist/vue-treeselect.css'
import IconSelect from '@/components/IconSelect'
import TagsView from '@/layout/components/TagsView/index.vue'

export default {
  name: 'Menu',
  dicts: ['sys_show_hide', 'sys_normal_disable'],
  components: { Treeselect, IconSelect, TagsView },
  data() {
    return {
      // 遮罩层
      loading: true,
      // 显示搜索条件
      showSearch: true,
      // 菜单表格树数据
      menuList: [],
      // 菜单树选项
      menuOptions: [],
      // 弹出层标题
      title: '',
      // 是否显示弹出层
      open: false,
      // 是否展开，默认全部折叠
      isExpandAll: false,
      // 重新渲染表格状态
      refreshTable: true,
      // 查询参数
      queryParams: {
        menuName: undefined,
        menuType: undefined,
        visible: undefined
      },
      // 表单参数
      form: {},
      // 表单校验
      rules: {
        menuName: [
          { required: true, message: '请填写菜单名称', trigger: 'blur' }
        ],
        // orderNum: [
        //   { required: true, message: '菜单顺序不能为空', trigger: 'blur' }
        // ],
        path: [
          { required: true, message: '请填写路由地址', trigger: 'blur' }
        ],
        perms: [
          { required: true, message: '请填写权限代码', trigger: 'blur' },
          { min: 3, message: '权限代码长度最少为3位', trigger: 'blur' }
        ]
      },
      isDisabled: false,
      tableExpandedArr: [],
      rowInfo: {}
    }
  },
  created() {
    this.getList()
  },
  methods: {
    // 选择图标
    selected(name) {
      this.form.icon = name
    },
    /** 查询菜单列表 */
    getList() {
      this.loading = true
      listMenu(this.queryParams).then(response => {
        this.menuList = this.handleTree(response.data, 'menuId')
        this.loading = false
        this.menuList.forEach(item => {
          this.tableExpandedArr.push(item.menuId + '')
        })
        this.$refs.menuTable.setCurrentRow(this.menuList.filter(item => item.menuId === this.rowInfo.menuId)[0])
      })
    },
    /** 转换菜单数据结构 */
    normalizer(node) {
      if (node.children && !node.children.length) {
        delete node.children
      }
      return {
        id: node.menuId,
        label: node.menuName,
        children: node.children
      }
    },
    /** 查询菜单下拉树结构 */
    getTreeselect() {
      listMenu().then(response => {
        this.menuOptions = []
        const menu = { menuId: 0, menuName: '主类目', children: [] }
        menu.children = this.handleTree(response.data, 'menuId')
        this.menuOptions.push(menu)
      })
    },
    // 取消按钮
    cancel() {
      this.open = false
      this.reset()
    },
    // 表单重置
    reset() {
      this.form = {
        menuId: undefined,
        parentId: 0,
        menuName: undefined,
        icon: undefined,
        menuType: 'M',
        orderNum: undefined,
        isFrame: '1',
        isCache: '0',
        visible: '0',
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
      this.reset()
      this.getTreeselect()
      if (row != null && row.menuId) {
        this.title = '新建子菜单'
        this.isDisabled = true
        // this.form.orderNum = row.children ? row.children.length + 1 : 1
        this.form.parentId = row.menuId
      } else {
        this.title = '新建菜单'
        this.isDisabled = false
        this.form.parentId = 0
        const arr = []
        this.menuList.forEach(item => {
          if (item.parentId === 0) {
            arr.push(item)
          }
        })
        // this.form.orderNum = arr.length + 1
      }
      this.open = true
    },
    /** 展开/折叠操作 */
    toggleExpandAll(isExpandAll) {
      this.refreshTable = false
      this.isExpandAll = isExpandAll
      this.$nextTick(() => {
        this.refreshTable = true
      })
    },
    /** 修改按钮操作 */
    handleUpdate(row) {
      this.reset()
      this.getTreeselect()
      this.rowInfo = row
      getMenu(row.menuId).then(response => {
        this.form = response.data
        this.open = true
        this.title = '编辑菜单'
      })
    },
    /** 提交按钮 */
    submitForm: function() {
      this.$refs['form'].validate(valid => {
        if (valid) {
          if (this.form.menuId !== undefined) {
            updateMenu(this.form).then(response => {
              this.$modal.msgSuccess('修改成功')
              this.open = false
              this.resetForm('queryForm')
              this.getList()
              this.$store.dispatch('GenerateRoutes').then(accessRoutes => {
              })
            })
          } else {
            addMenu(this.form).then(response => {
              this.$modal.msgSuccess('保存成功')
              this.open = false
              this.resetForm('queryForm')
              this.getList()
              this.$store.dispatch('GenerateRoutes').then(accessRoutes => {
              })
            })
          }
        }
      })
    },
    /** 删除按钮操作 */
    handleDelete(row) {
      this.$modal.confirm('确定删除菜单"' + row.menuName + '"？', '确定删除').then(function() {
        return delMenu(row.menuId)
      }).then(() => {
        this.getList()
        this.$modal.msgSuccess('删除成功')
        this.$store.dispatch('GenerateRoutes').then(accessRoutes => {
        })
      }).catch(() => { })
    }
    // /** 菜单类型筛选*/
    // filterMenu(value, row) {
    //   return row.menuType === value
    // },
    // /** 菜单状态筛选*/
    // filterVisible(value, row) {
    //   return row.visible === value
    // }
  }
}
</script>
<style lang="scss" scoped>
::v-deep .vue-treeselect__control-arrow-container{
  background: url('../../../assets/images/bumen@2x.png') no-repeat 50%;
  background-size:16px;
}
.expand{
  float:right
}
</style>
