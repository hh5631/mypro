<template>
  <div class="app-container">
    <tags-view>
      <el-row :gutter="10" class="mb8">
        <el-col :span="1.5">
          <el-col :span="1.5">
            <el-button
              v-hasPermi="['system:dict:export']"
              plain
              size="small"
              @click="handleExport"
            ><i style="color:#4686EF;" class="iconfont icon-daochu" />导出</el-button>
          </el-col>
          <el-button
            v-hasPermi="['system:dict:add']"
            type="primary"
            size="small"
            @click="handleAdd"
          ><i class="iconfont icon-zengjia" style="color: #ffffff" />新增</el-button>
        </el-col>
        <right-toolbar :show-search.sync="showSearch" @queryTable="getList" />
      </el-row>
    </tags-view>
    <el-card class="main-card">
      <el-form v-show="showSearch" ref="queryForm" :model="queryParams" :inline="true" label-width="68px">
        <el-form-item label="字典名称" prop="dictName">
          <el-input
            v-model="queryParams.dictName"
            placeholder="请输入"
            clearable
            size="small"
            style="width: 240px"
            @keyup.enter.native="handleQuery"
          />
        </el-form-item>
        <el-form-item label="字典类型" prop="dictType">
          <el-input
            v-model="queryParams.dictType"
            placeholder="请输入"
            clearable
            size="small"
            style="width: 240px"
            @keyup.enter.native="handleQuery"
          />
        </el-form-item>
        <el-form-item label="状态" prop="status">
          <el-select
            v-model="queryParams.status"
            placeholder="请选择"
            clearable
            size="small"
            style="width: 240px"
          >
            <el-option
              v-for="dict in dict.type.sys_normal_disable"
              :key="dict.value"
              :label="dict.label"
              :value="dict.value"
            />
          </el-select>
        </el-form-item>
        <el-form-item label="创建时间">
          <div class="date_box">
            <el-date-picker
              v-model="dateRange"
              size="small"
              style="width: 200px"
              value-format="yyyy-MM-dd"
              type="daterange"
              range-separator="至"
              start-placeholder="开始日期"
              end-placeholder="结束日期"
              :clearable="false"
            /><i class="el-icon-date data_icon" />
          </div>
        </el-form-item>
        <el-form-item>
          <el-button size="small" @click="resetQuery">重置</el-button>
          <el-button size="small" class="search-btn" @click="handleQuery">搜索</el-button>
        </el-form-item>
      </el-form>

      <el-table v-loading="loading" :data="typeList" height="100%" @selection-change="handleSelectionChange">
        <el-table-column type="selection" width="55" align="left" />
        <!-- <el-table-column label="序号" type="index" width="50" /> -->
        <!-- <el-table-column label="字典编号" align="center" prop="dictId" /> -->
        <el-table-column label="字典名称" align="left" width="400" prop="dictName" :show-overflow-tooltip="true" />
        <el-table-column label="字典类型" align="left" width="360" :show-overflow-tooltip="true">
          <template slot-scope="scope">
            <router-link :to="'/system/dict-data/index/' + scope.row.dictId" class="link-type">
              <span>{{ scope.row.dictType }}</span>
            </router-link>
          </template>
        </el-table-column>
        <el-table-column label="备注" align="left" prop="remark" :show-overflow-tooltip="true" />
        <el-table-column label="创建时间" align="left" prop="createTime" width="240">
          <template slot-scope="scope">
            <span>{{ parseTime(scope.row.createTime) }}</span>
          </template>
        </el-table-column>
        <el-table-column label="状态" align="left" prop="status">
          <template slot-scope="scope">
            <div class="statePoint">
              <span v-if="scope.row.status == 0" style="color: #3DB954;"><img
                class="point"
                src="../../../assets/images/green@2x.png"
              >正常</span>
              <span v-else style="color:#EF5826"><img class="point" src="../../../assets/images/red@2x.png">停用</span>
            </div>
          </template>
        </el-table-column>
        <el-table-column label="操作" align="left" class-name="small-padding">
          <template slot-scope="scope">
            <el-button
              v-hasPermi="['system:dict:edit']"
              size="small"
              type="text"
              @click="handleUpdate(scope.row)"
            >编辑</el-button>
            <el-button
              v-hasPermi="['system:dict:remove']"
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
          v-show="total>0"
          :total="total"
          :page.sync="queryParams.pageNum"
          :limit.sync="queryParams.pageSize"
          @pagination="getList"
        />
      </div>
    </el-card>

    <!-- 添加或修改参数配置对话框 -->
    <el-dialog class="addDig" :title="title" :visible.sync="open" width="800px" append-to-body>
      <el-form ref="form" style="width:80%;margin-left: 10%" :model="form" :rules="rules" label-width="80px">
        <el-form-item label="字典名称" prop="dictName">
          <el-input v-model="form.dictName" placeholder="请输入字典名称" />
        </el-form-item>
        <el-form-item label="字典类型" prop="dictType">
          <el-input v-model="form.dictType" placeholder="请输入字典类型" />
        </el-form-item>
        <el-form-item label="状态" prop="status">
          <el-radio-group v-model="form.status">
            <el-radio
              v-for="dict in dict.type.sys_normal_disable"
              :key="dict.value"
              :label="dict.value"
            >{{ dict.label }}</el-radio>
          </el-radio-group>
        </el-form-item>
        <el-form-item label="备注" prop="remark">
          <el-input v-if="isReset" v-model="form.remark" type="textarea" placeholder="请输入内容" />
        </el-form-item>
      </el-form>
      <div slot="footer" class="dialog-footer">
        <el-button size="small" @click="cancel">取消</el-button>
        <el-button size="small" type="primary" @click="submitForm">保存</el-button>
      </div>
    </el-dialog>
  </div>
</template>

<script>
import TagsView from '@/layout/components/TagsView/index.vue'
import { listType, getType, delType, addType, updateType, refreshCache } from '@/api/system/dict/type'

export default {
  name: 'Dict',
  dicts: ['sys_normal_disable'],
  components: { TagsView },
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
      // 总条数
      total: 0,
      // 字典表格数据
      typeList: [],
      // 弹出层标题
      title: '',
      // 是否显示弹出层
      open: false,
      // 日期范围
      dateRange: [],
      // 查询参数
      queryParams: {
        pageNum: 1,
        pageSize: 10,
        dictName: undefined,
        dictType: undefined,
        status: undefined
      },

      // 表单参数
      form: {},
      // 表单校验
      rules: {
        dictName: [
          { required: true, message: '字典名称不能为空', trigger: 'blur' }
        ],
        dictType: [
          { required: true, message: '字典类型不能为空', trigger: 'blur' }
        ]
      },
      isReset: true
    }
  },
  created() {
    this.getList()
  },
  methods: {
    /** 查询字典类型列表 */
    getList() {
      this.loading = true
      listType(this.addDateRange(this.queryParams, this.dateRange)).then(response => {
        this.typeList = response.rows
        this.total = response.total
        this.loading = false
      }
      )
    },
    // 取消按钮
    cancel() {
      this.open = false
      this.reset()
    },
    // 表单重置
    reset() {
      this.form = {
        dictId: undefined,
        dictName: undefined,
        dictType: undefined,
        status: '0',
        remark: undefined
      }
      this.resetForm('form')
    },
    /** 搜索按钮操作 */
    handleQuery() {
      this.queryParams.pageNum = 1
      this.pageParams = this.queryParams
      this.getList()
    },
    /** 重置按钮操作 */
    resetQuery() {
      this.dateRange = []
      this.resetForm('queryForm')
      this.handleQuery()
    },
    /** 新增按钮操作 */
    handleAdd() {
      this.reset()
      this.open = true
      this.title = '新增字典类型'
    },
    // 多选框选中数据
    handleSelectionChange(selection) {
      this.ids = selection.map(item => item.dictId)
      this.single = selection.length !== 1
      this.multiple = !selection.length
    },
    /** 修改按钮操作 */
    handleUpdate(row) {
      this.reset()
      const dictId = row.dictId || this.ids
      getType(dictId).then(response => {
        this.form = response.data
        this.open = true
        this.title = '修改字典类型'
      })
    },
    /** 提交按钮 */
    submitForm: function() {
      this.$refs['form'].validate(valid => {
        if (valid) {
          if (this.form.dictId !== undefined) {
            updateType(this.form).then(response => {
              this.$modal.msgSuccess('修改成功')
              this.open = false
              this.isReset = false
              this.$nextTick(() => {
                this.isReset = true
              })
              this.getList()
            })
          } else {
            addType(this.form).then(response => {
              this.$modal.msgSuccess('新增成功')
              this.open = false
              this.isReset = false
              this.$nextTick(() => {
                this.isReset = true
              })
              this.getList()
            })
          }
        }
      })
    },
    /** 删除按钮操作 */
    handleDelete(row) {
      const dictIds = row.dictId || this.ids
      this.$modal.confirm('确定删除字典编号为"' + dictIds + '"的数据项？', '确定删除').then(function() {
        return delType(dictIds)
      }).then(() => {
        this.getList()
        this.$modal.msgSuccess('删除成功')
      }).catch(() => {})
    },
    /** 导出按钮操作 */
    handleExport() {
      this.download('system/dict/type/export', this.ids, `type_${new Date().getTime()}.xlsx`)
    },
    /** 刷新缓存按钮操作 */
    handleRefreshCache() {
      refreshCache().then(() => {
        this.$modal.msgSuccess('刷新成功')
      })
    }
  }
}
</script>
<style lang="scss" scoped>
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
    ::v-deep .el-input__icon.el-range__icon.el-icon-date {
      display: none;
    }
  }
  .statePoint .point {
  width: 8px;
  height: 8px;
  margin: 0 8px 2px 0;
}
.dialog-footer{
  ::v-deep .el-button{
    width: 80px;
  }
}
.addDig{
  ::v-deep .el-dialog__body{
    max-height: 543px;
    overflow-y: auto;
  }
}
</style>
