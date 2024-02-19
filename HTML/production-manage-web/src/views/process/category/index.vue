<template>
  <CommonTitleBar>
    <template #titleBtn>
      <el-button
        type="primary"
        icon="el-icon-plus"
        size="small"
        @click="addEditCategory(null)"
      >新增
      </el-button>
    </template>
    <el-card class="main-card task-card">
      <div class="app-container">
        <el-form
          ref="queryForm"
          :model="queryParams"
          :inline="true"
          label-width="80"
        >
          <el-form-item label="类别名称" prop="categoryName">
            <el-input
              v-model.trim="queryParams.categoryName"
              placeholder="请输入"
              clearable
              size="small"
              style="width: 200px"
              maxlength="15"
              @keyup.enter.native="handleQuery"
            />
          </el-form-item>
          <el-form-item>
            <el-button size="small" @click="resetQuery">重置</el-button>
            <el-button
              size="small"
              type="primary"
              plain
              class="search-btn"
              @click="handleQuery"
            >搜索</el-button>
          </el-form-item>
        </el-form>
        <!-- table表格 -->
        <el-table v-loading="tableLoading" :data="list" style="width: 100%;margin-top: 8px;" height="100%">
          <el-table-column label="序号" align="left" type="index" />
          <el-table-column label="类别名称" align="left" prop="categoryName" />
          <el-table-column label="创建人" align="left" prop="creator" />
          <el-table-column label="创建时间" align="left" prop="createTime" />
          <el-table-column label="备注" align="left" prop="remark" width="300" :show-overflow-tooltip="true">
            <template slot-scope="scope">
              <span>{{ scope.row.remark }}</span>
            </template>
          </el-table-column>
          <el-table-column align="left" label="操作" width="200">
            <template slot-scope="scope">
              <el-button
                size="small"
                type="text"
                @click="addEditCategory(scope.row)"
              >编辑
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
        <add-edit ref="addedit" @reload="getList" />
      </div>
    </el-card>
  </CommonTitleBar>

</template>

<script>
import { delProcessCategory } from '@/api/process'
import { getProcessCategoryList } from '@/api/process'
import AddEdit from './component/addEdit.vue'
// import { mapGetters } from 'vuex'
import CommonTitleBar from '../../../components/common/common-title-bar.vue'
export default {
  components: {
    AddEdit,
    CommonTitleBar
  },
  data() {
    return {
      tableLoading: false,
      queryParams: {
        pageNum: 1,
        pageSize: 10
      },
      list: [],
      total: 0
    }
  },
  // computed: {
  //   ...mapGetters({ list: 'processCategoryList' })
  // },
  created() {
    this.getList()
  },
  methods: {
    // 获取列表数据
    async getList() {
      const res = await getProcessCategoryList(this.queryParams)
      this.$store.commit('flow/SET_PC_LIST')
      this.list = res.data.records
      this.total = res.data.total || 0
    },
    // 新增编辑
    addEditCategory(val) {
      this.$refs.addedit.openDialog(val)
    },
    /** 搜索相关 */
    handleQuery() {
      this.queryParams.pageNum = 1
      this.getList()
    },
    /** 重置 */
    resetQuery() {
      this.resetForm('queryForm')
      this.handleQuery()
    },
    resetForm(refName) {
      if (this.$refs[refName]) {
        this.$refs[refName].resetFields()
      }
    },
    /** 删除按钮操作 */
    handleDelete(row) {
      this.$confirm('此操作将永久删除该信息, 是否继续?', '确定删除', {
        confirmButtonText: '确定',
        cancelButtonText: '取消',
        type: 'warning'
      }).then(() => {
        delProcessCategory(row.id)
          .then(res => {
            if (res.code === 200) {
              this.$message.success('删除成功')
              this.queryParams.pageNum = this.changePageNum(this.total, 1, this.queryParams.pageNum, this.queryParams.pageSize)
              this.getList()
            } else {
              this.$message.error(res.msg)
            }
          })
      })
    }
  }
}
</script>
<style lang="scss" scoped>
.ellipsis-info{
  width: 280px;
  overflow: hidden;
  text-overflow: ellipsis;
  white-space:nowrap;
}
</style>
