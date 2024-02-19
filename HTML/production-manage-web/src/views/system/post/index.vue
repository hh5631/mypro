<template>
  <div class="post-box">
    <tags-view>
      <el-row :gutter="10" class="mb8">
        <el-col :span="1.5">
          <el-button
            v-hasPermi="['system:post:add']"
            type="primary"
            icon="el-icon-plus"
            size="small"
            @click="handleAdd"
          >新建职位</el-button>
        </el-col>
        <el-col :span="1.5">
          <el-button
            v-hasPermi="['system:post:remove']"
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
        <el-form-item label="职位名称" prop="postName">
          <el-input
            v-model.trim="queryParams.postName"
            placeholder="请输入"
            clearable
            maxlength="15"
            size="small"
            style="width: 200px"
            @keyup.enter.native="handleQuery"
          />
        </el-form-item>
        <el-form-item label="职位代码" prop="postCode">
          <el-input
            v-model.trim="queryParams.postCode"
            placeholder="请输入"
            clearable
            maxlength="15"
            size="small"
            style="width: 200px"
            @keyup.enter.native="handleQuery"
          />
        </el-form-item>
        <el-form-item label="状态" prop="status">
          <el-select v-model="queryParams.status" style="width: 200px" placeholder="请选择" clearable size="small">
            <el-option
              v-for="dict in dict.type.sys_normal_disable"
              :key="dict.value"
              :label="dict.label"
              :value="dict.value"
            />
          </el-select>
        </el-form-item>
        <el-form-item>
          <el-button size="small" @click="resetQuery">重置</el-button>
          <el-button type="primary" class="search-btn" plain size="small" @click="handleQuery">搜索</el-button>
        </el-form-item>
      </el-form>
      <el-table ref="postTable" v-loading="loading" highlight-current-row :data="postList" height="100%" @selection-change="handleSelectionChange">
        <el-table-column type="selection" width="55" align="left" />
        <!-- <el-table-column label="显示顺序" prop="postSort" width="100" align="left" /> -->
        <el-table-column label="职位名称" align="left" prop="postName" :show-overflow-tooltip="true" />
        <el-table-column label="职位代码" width="390" align="left" prop="postCode" :show-overflow-tooltip="true" />
        <!-- <el-table-column label="岗位编码" align="center" prop="postCode" /> -->
        <!-- <el-table-column label="显示排序" align="center" prop="postSort" /> -->
        <el-table-column label="状态" width="380" align="left" prop="status">
          <template slot-scope="scope">
            <div class="statePoint">
              <span v-if="scope.row.status == 0" style="color: #3DB954;"><img class="point" src="../../../assets/images/green@2x.png">正常</span>
              <span v-else style="color:#EF5826"><img class="point" src="../../../assets/images/red@2x.png">停用</span>
            </div>
          </template>
        </el-table-column>
        <!-- <el-table-column label="创建时间" align="center" prop="createTime" width="180">
          <template slot-scope="scope">
            <span>{{ parseTime(scope.row.createTime) }}</span>
          </template>
        </el-table-column> -->
        <el-table-column label="操作" width="140" align="left" class-name="small-padding">
          <template slot-scope="scope">
            <el-button
              v-hasPermi="['system:post:edit']"
              size="small"
              type="text"
              style="color:#4686EF"
              @click="handleUpdate(scope.row)"
            >编辑</el-button>
            <el-button
              v-hasPermi="['system:post:remove']"
              size="small"
              type="text"
              style="color:#EF5826"
              @click="handleDelete(scope.row)"
            >删除</el-button>
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

      <!-- 添加或修改岗位对话框 -->
      <el-dialog :close-on-click-modal="false" :title="title" :visible.sync="open" width="800px" append-to-body class="post-dialogs">
        <el-form ref="form" :model="form" :rules="rules" label-width="193px" style="margin-right: 111px;">
          <el-form-item label="职位名称" prop="postName">
            <el-input v-model.trim="form.postName" maxlength="15" placeholder="请输入" />
          </el-form-item>
          <el-form-item label="职位代码" prop="postCode">
            <el-input v-model.trim="form.postCode" oninput="value=value.replace(/[\u4E00-\u9FA5]/ig, '')" maxlength="15" placeholder="请输入" />
          </el-form-item>
          <el-form-item label="显示顺序" prop="postSort">
            <el-input v-model="form.postSort" maxlength="5" type="text" placeholder="请输入" onkeyup="if(value==0&&value!=''){value=1}this.value = this.value.replace(/[^0-9]/g, '');" />
          </el-form-item>
          <el-form-item label="职位状态" class="post-status" prop="status">
            <el-radio-group v-model="form.status">
              <el-radio
                v-for="dict in dict.type.sys_normal_disable"
                :key="dict.value"
                :label="dict.value"
              >{{ dict.label }}</el-radio>
            </el-radio-group>
          </el-form-item>
          <!-- <el-form-item label="备注" prop="remark">
            <el-input v-model="form.remark" type="textarea" placeholder="请输入" />
          </el-form-item> -->
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
import { listPost, getPost, delPost, addPost, updatePost } from '@/api/system/post'
import TagsView from '@/layout/components/TagsView/index.vue'
export default {
  name: 'Post',
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
      // 选中的数量
      selectedLength: 0,
      // 总条数
      total: 0,
      // 岗位表格数据
      postList: [],
      // 弹出层标题
      title: '',
      // 是否显示弹出层
      open: false,
      listLength: '',
      // 查询参数
      queryParams: {
        pageNum: 1,
        pageSize: 10,
        postCode: undefined,
        postName: undefined,
        status: undefined
      },
      // 表单参数
      form: {},
      // 表单校验
      rules: {
        postName: [
          { required: true, message: '请填写职位名称', trigger: 'blur' }
        ],
        postCode: [
          { required: true, message: '请填写职位代码', trigger: 'blur' },
          { min: 3, message: '职位代码长度最少为3位', trigger: 'blur' }
        ]
        // postSort: [
        //   { required: true, message: '岗位顺序不能为空', trigger: 'blur' }
        // ]
      },
      rowInfo: {},
      isStatus: false,
      selectionName: []
    }
  },
  created() {
    this.getList()
  },
  methods: {
    /** 查询岗位列表 */
    getList() {
      this.loading = true
      listPost(this.queryParams).then(response => {
        this.postList = response.rows
        this.listLength = response.total + 1
        this.total = response.total
        this.loading = false
        this.$refs.postTable.setCurrentRow(this.postList.filter(item => item.postId === this.rowInfo.postId)[0])
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
        postId: undefined,
        postCode: undefined,
        postName: undefined,
        postSort: undefined,
        status: '0',
        remark: undefined
      }
      this.resetForm('form')
    },
    /** 搜索按钮操作 */
    handleQuery() {
      this.queryParams.pageNum = 1
      this.pageParams = { ...this.queryParams }
      this.getList()
    },
    /** 重置按钮操作 */
    resetQuery() {
      this.resetForm('queryForm')
      this.queryParams.pageNum = 1
      this.handleQuery()
    },
    // 多选框选中数据
    handleSelectionChange(selection) {
      this.selectedLength = selection.length
      this.ids = selection.map(item => item.postId)
      this.selectionName = selection.map(item => item.postName)
      this.single = selection.length !== 1
      this.multiple = !selection.length
    },
    /** 新增按钮操作 */
    handleAdd() {
      this.reset()
      this.open = true
      // this.form.postSort = this.listLength
      this.title = '新建职位'
    },
    /** 修改按钮操作 */
    handleUpdate(row) {
      this.reset()
      this.rowInfo = row
      const postId = row.postId || this.ids
      getPost(postId).then(response => {
        this.form = response.data
        this.isStatus = response.data.status === '0'
        this.open = true
        this.title = '编辑'
      })
    },
    /** 提交按钮 */
    submitForm: function() {
      this.$refs['form'].validate(valid => {
        if (valid) {
          if (this.form.postId !== undefined) {
            if (this.form.postSort === '0') {
              this.form.postSort = '1'
            }
            updatePost(this.form).then(response => {
              if (this.form.status === '1' && this.isStatus === true) {
                this.$confirm('确认停用职位' + '"' + this.form.postName + '"' + '?', '确认停用', {
                  confirmButtonText: '确定',
                  cancelButtonText: '取消',
                  type: 'warning'
                }).then(() => {
                  this.$modal.msgSuccess('修改成功')
                  this.open = false
                  this.resetForm('queryForm')
                  this.getList()
                }).catch(() => {
                })
              } else {
                this.$modal.msgSuccess('修改成功')
                this.open = false
                this.resetForm('queryForm')
                this.getList()
              }
            })
          } else {
            addPost(this.form).then(response => {
              this.$modal.msgSuccess('保存成功')
              this.open = false
              this.resetForm('queryForm')
              this.getList()
            })
          }
        }
      })
    },
    /** 删除按钮操作 */
    handleDelete(row) {
      const postIds = row.postId || this.ids
      const postName = row.postName || this.selectionName
      this.$modal.confirm('确定删除职位"' + postName + '"？', '确定删除').then(function() {
        return delPost(postIds)
      }).then(() => {
        this.getList()
        this.$modal.msgSuccess('删除成功')
      }).catch(() => {})
    },
    /** 导出按钮操作 */
    handleExport() {
      this.download('system/post/export', this.ids, `post_${new Date().getTime()}.xlsx`)
    }
  }
}
</script>
<style lang="scss" scoped>
.post-box {
  .statePoint .point{
    width:8px;
    height:8px;
    margin:0 8px 2px 0;
  }
}
.post-dialogs{
    ::v-deep .el-dialog .el-dialog__body{
      padding-top: 24px;
      padding-bottom: 2px;
    }
  }
  .post-status{
    ::v-deep .el-form-item{
    margin-bottom: 0;
  }
  }

</style>
