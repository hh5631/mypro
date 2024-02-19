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
          ><i
            style="color: #4686ef"
            class="iconfont icon-daochu"
          />导出</el-button>
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
    <el-card class="main-card">
      <el-form ref="queryForm" :model="queryParams" :inline="true">
        <el-form-item label="供应商名称" prop="name">
          <el-input
            v-model.trim="queryParams.name"
            placeholder="请输入"
            maxlength="15"
            clearable
            size="small"
            style="width: 200px"
            @keyup.enter.native="handleQuery"
          />
        </el-form-item>
        <el-form-item label="供应商代码" prop="code">
          <el-input
            v-model.trim="queryParams.code"
            placeholder="请输入"
            maxlength="15"
            clearable
            size="small"
            style="width: 200px"
            @keyup.enter.native="handleQuery"
          />
        </el-form-item>
        <!-- <el-form-item label="状态" prop="enable">
          <el-select
            v-model="queryParams.enable"
            style="width: 200px"
            placeholder="请选择"
            clearable
            size="small"
          >
            <el-option
              v-for="dict in dict.type.sys_normal_disable"
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

      <el-table
        ref="supplierTable"
        v-loading="loading"
        highlight-current-row
        height="100%"
        :data="supplierTable"
        @selection-change="handleSelectionChange"
      >
        <el-table-column type="selection" width="55" align="left" />
        <el-table-column label="序号" type="index" width="70" align="left" />
        <el-table-column
          label="供应商名称"
          align="left"
          prop="name"
          :show-overflow-tooltip="true"
        />
        <el-table-column
          label="供应商代码"
          align="left"
          prop="code"
          :show-overflow-tooltip="true"
        />
        <el-table-column
          label="统一社会信用代码"
          align="left"
          prop="account"
          :show-overflow-tooltip="true"
        />
        <el-table-column
          label="经营范围"
          align="left"
          prop="businessScope"
          :show-overflow-tooltip="true"
        />
        <!-- <el-table-column label="状态" align="center" prop="enable">
          <template slot-scope="scope">
            <div>
              <el-switch
                v-model="scope.row.enable"
                active-value="0"
                inactive-value="1"
                @change="changeState($event, scope.row)"
              />
            </div>
          </template>
        </el-table-column> -->
        <!-- <el-table-column label="创建时间" align="center" prop="createTime" width="180">
          <template slot-scope="scope">
            <span>{{ parseTime(scope.row.createTime) }}</span>
          </template>
        </el-table-column> -->
        <el-table-column
          label="操作"
          width="200"
          align="left"
          class-name="small-padding"
        >
          <template slot-scope="scope">
            <el-button
              size="small"
              type="text"
              @click="handleUpdate(scope.row, 0)"
            >详情</el-button>
            <el-button
              size="small"
              type="text"
              style="color: #4686ef"
              @click="handleUpdate(scope.row, 1)"
            >编辑</el-button>
            <el-button
              size="small"
              type="text"
              style="color: #ef5826"
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
    </el-card>
    <template v-if="addOrEditDig">
      <el-dialog
        :title="title"
        :visible.sync="addOrEditDig"
        width="1200px"
        append-to-body
        :close-on-click-modal="false"
        @close="closeDig"
      >
        <addSupplier
          ref="addSupplier"
          :form-data="formData"
          @successSubmit="submitSuccess"
        />
        <div v-if="isDisabled === false" slot="footer" class="dialog-footer">
          <el-button size="small" @click="cancel">取消</el-button>
          <el-button
            size="small"
            type="primary"
            @click="submitForm"
          >保存</el-button>
        </div>
      </el-dialog>
    </template>
  </div>
</template>

<script>
import { getSupplierList, updateSupplierState, delSupplier, getSupplier } from '@/api/basicdata/index'
import addSupplier from '../../purchase/apply/component/addSupplier.vue'
import TagsView from '@/layout/components/TagsView/index.vue'
export default {
  components: { TagsView, addSupplier },
  dicts: ['company_type', 'sys_normal_disable'],
  data () {
    return {
      queryParams: {
        name: undefined,
        code: undefined,
        enable: undefined,
        pageNum: 1,
        pageSize: 10
      },
      supplierTable: [],
      total: 0,
      loading: false,
      // selectedLength: 0,
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
      ids: [],
      title: '',
      addOrEditDig: false,
      formData: {},
      isDisabled: false
    }
  },
  created () {
    this.getList()
  },
  methods: {
    getList () {
      this.loading = true
      getSupplierList(this.addDateRange(this.queryParams)).then(
        (response) => {
          this.loading = false
          this.supplierTable = response.data.records
          //
          this.total = response.data.total
        }
      )
    },
    handleUpdate (row, index) {
      if (index === 1) {
        this.title = '编辑供应商'
        this.isDisabled = false
        getSupplier(row.id).then((res) => {
          this.formData = res.data
          this.addOrEditDig = true
          this.$nextTick(() => {
            this.$refs.addSupplier.edit()
          })
        })
      } else {
        this.title = '详情'
        this.isDisabled = true
        getSupplier(row.id).then((res) => {
          this.formData = res.data
          this.addOrEditDig = true
          this.$nextTick(() => {
            this.$refs.addSupplier.getDetail()
          })
        })
      }
    },
    handleDelete (row) {
      const id = row.id || this.ids
      const name = row.name
      this.$modal
        .confirm('确定删除供应商"' + name + '"？', '确定删除')
        .then(function () {
          return delSupplier(id)
        })
        .then(() => {
          // 如果删除之后当前数据长度为1则this.queryParams页数少1
          if (this.supplierTable.length === 1 && this.queryParams.pageNum !== 1) {
            this.queryParams.pageNum = this.queryParams.pageNum - 1
          }
          this.getList()
          this.$modal.msgSuccess('删除成功')
        })
        .catch(() => { })
    },
    handleQuery () {
      this.queryParams.pageNum = 1
      this.getList()
    },
    resetQuery () {
      this.resetForm('queryForm')
      this.handleQuery()
    },
    // 多选框选中数据
    handleSelectionChange (selection) {
      this.ids = []
      selection.forEach(item => {
        this.ids.push(item.id)
      })
    },
    // stopUse() {
    //   if (this.ids.length !== 1) {
    //     this.$message.error('请选择一个供应商')
    //     return
    //   }
    //   updateSupplierState(this.ids[0], false).then((res) => {
    //     if (res.code === 200) {
    //       this.$message.success('停用成功')
    //       this.getList()
    //     }
    //   })
    // },
    handleExport () {
      this.download(
        'system/supplier/export/',
        this.ids,
        `供应商_${this.parseTime(new Date(), '{y}-{m}-{d}')}.xlsx`
      )
      // const idss = []
      // if (this.ids.length === 0) {
      //   // this.supplierTable.forEach(item => {
      //   //   idss.push(item.id)
      //   // })
      //   // this.download(
      //   //   'system/supplier/export/' + idss.toString(),
      //   //   { },
      //   //   `supplier_${this.parseTime(new Date(), '{y}-{m}-{d}')}.xlsx`
      //   // )
      //   // this.$message.error('请选择需要导出的内容')
      //   // return
      //   this.supplierTable.forEach(item => {
      //     this.ids.push(item.id)
      //   })
      // }
      // this.download(
      //   'system/supplier/export/' + this.ids.toString(),
      //   {},
      //   `supplier_${this.parseTime(new Date(), '{y}-{m}-{d}')}.xlsx`
      // )
    },
    handleAdd () {
      this.addOrEditDig = true
      this.$nextTick(() => {
        this.$refs.addSupplier.reset()
      })
      this.title = '新建供应商'
    },
    // 取消按钮
    cancel () {
      this.$refs.addSupplier.reset()
      this.addOrEditDig = false
    },
    closeDig () {
      this.isDisabled = false
      this.$refs.addSupplier.reset()
      this.$nextTick(() => {
        this.addOrEditDig = false
      })
    },
    submitForm () {
      this.$refs.addSupplier.submitForm()
    },
    submitSuccess (data) {
      if (data.code === 200 && !data.id) {
        this.$message.success('创建成功')
        this.addOrEditDig = false
        this.getList()
      } else if (data.id) {
        this.$message.success('编辑成功')
        this.addOrEditDig = false
        this.getList()
      }
    },
    // switch改变状态
    changeState (el, row) {
      updateSupplierState(row.id).then((res) => {
        if (res.code === 200) {
          this.$message.success('修改成功')
          this.getList()
        }
      })
    },
    getStatus (status) {
      return status === '1'
    }
  }
}
</script>

<style lang="scss" scoped>
.statePoint .point {
  width: 8px;
  height: 8px;
  margin: 0 8px 2px 0;
}
::v-deep .el-dialog .el-dialog__body {
  padding-top: 0;
  max-height: 542px;
  overflow-y: auto;
  // padding-bottom: 0;
}
</style>

