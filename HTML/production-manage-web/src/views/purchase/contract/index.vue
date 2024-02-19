<template>
  <div>
    <tags-view>
      <el-row :gutter="10" class="mb8">
        <!-- <el-col :span="1.5">
          <el-button
            v-hasPermi="['system:user:export']"
            plain
            size="small"
            @click="handleExport"
          ><i style="color:#4686EF;" class="iconfont icon-daochu" />导出</el-button>
        </el-col> -->
        <el-col :span="1.5">
          <el-button
            v-hasPermi="['procurement:contract:addOrEdit']"
            type="primary"
            size="small"
            @click="handleAdd"
          ><i class="iconfont icon-zengjia" style="color: #ffffff" />新增合同</el-button>
        </el-col>
      </el-row>
    </tags-view>
    <el-card class="main-card">
      <!-- 头部内容 -->
      <el-form
        ref="queryForm"
        :model="queryParams"
        :inline="true"
        label-width="80"
      >
        <el-form-item label="合同编号" prop="contractNo">
          <el-input
            v-model.trim="queryParams.contractNo"
            placeholder="请输入"
            clearable
            size="small"
            style="width: 200px"
            maxlength="15"
            @keyup.enter.native="handleQuery"
          />
        </el-form-item>
        <el-form-item label="合同名称" prop="contractName">
          <el-input
            v-model.trim="queryParams.contractName"
            placeholder="请输入"
            clearable
            size="small"
            style="width: 200px"
            maxlength="15"
            @keyup.enter.native="handleQuery"
          />
        </el-form-item>
        <el-form-item v-if="isShowMore">
          <el-button
            v-hasPermi="['system:user:export']"
            plain
            size="small"
            @click="handleExport"
          ><i style="color:#4686EF;" class="iconfont icon-daochu" />导出</el-button>
        </el-form-item>
        <el-form-item v-if="isShowMore">
          <el-button
            v-hasPermi="['system:user:add']"
            type="primary"
            size="small"
            @click="handleAdd"
          ><i class="iconfont icon-zengjia" style="color: #ffffff" />新增</el-button>
        </el-form-item>
        <el-form-item>
          <el-button size="small" @click="resetQuery">重置</el-button>
          <el-button v-hasPermi="['procurement:contract:list']" size="small" type="primary" class="search-btn" plain @click="handleQuery">搜索</el-button>
        </el-form-item>
      </el-form>
      <!-- 表格显示list数据 -->
      <el-table ref="contractTable" v-loading="loading" highlight-current-row height="100%" :data="contractTable" @selection-change="handleSelectionChange" @sort-change="sortChange">
        <el-table-column type="selection" align="center" />
        <el-table-column label="序号" align="left" prop="index" type="index" />
        <el-table-column label="合同编号" sortable="custom" align="left" prop="contractNo" :show-overflow-tooltip="true" />
        <el-table-column label="合同名称" align="left" prop="contractName" :show-overflow-tooltip="true" />
        <el-table-column label="供应商" align="left" prop="supplierName" :show-overflow-tooltip="true" />
        <el-table-column label="采购单号" align="left" prop="poNo" :show-overflow-tooltip="true" />
        <el-table-column label="合同金额(元)" sortable="custom" align="left" prop="contractAmount" :show-overflow-tooltip="true" :formatter="formatterAmount" />
        <el-table-column label="合同状态" align="left" prop="contractStatus">
          <template slot-scope="scope">
            <div class="statePoint">
              <span v-if="scope.row.contractStatus === '4'" style="color: #3DB954;">已完成</span>
              <span v-else-if="scope.row.contractStatus === '3'" style="color:#EF5826">生效中</span>
              <span v-else-if="scope.row.contractStatus === '0'" style="color:#EF5826">审核中</span>
              <span v-else-if="scope.row.contractStatus === '2'" style="color:#EF5826">签订中</span>
              <span v-else-if="scope.row.contractStatus === '1'" style="color:#909399">已驳回</span>
            </div>
          </template>
        </el-table-column>
        <el-table-column label="操作" align="left" class-name="small-padding">
          <template slot-scope="scope">
            <el-button
              v-hasPermi="['procurement:contract:query']"
              size="small"
              type="text"
              @click="handleUpdate(scope.row,0)"
            >详情</el-button>
            <el-button
              v-hasPermi="['procurement:contract:addOrEdit']"
              size="small"
              type="text"
              style="color:#4686EF"
              @click="handleUpdate(scope.row,1)"
            >编辑</el-button>
            <el-button
              v-hasPermi="['procurement:contract:remove']"
              size="small"
              type="text"
              style="color:#EF5826"
              @click="handleDelete(scope.row)"
            >删除</el-button>
          </template>
        </el-table-column>
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
      <!-- 新建采购合同弹窗 -->
      <el-dialog
        :title="title"
        :visible.sync="open"
        :width="isEdit?'1000px':'900px'"
        append-to-body
        class="addDig"
        :close-on-click-modal="false"
        @close="closeUser"
      >
        <addContract ref="addContract" :is-disabled="isDisabled" :is-edit="isEdit" :form-data="formData" :supplier-options="supplierOptions" @submitContractList="submitContractList" />
        <div v-if="isDisabledModify == false" slot="footer" class="dialog-footer">
          <el-button size="small" @click="cancel">取 消</el-button>
          <el-button size="small" type="primary" @click="submitForm">保 存</el-button>
        </div>
      </el-dialog>

    </el-card>
  </div>
</template>
<script>
import { getContractList, getContractDetail, checkContract, delContract } from '@/api/purchase/index'
import { getSupplierList } from '@/api/basicdata/index'
import { formatPrice } from '@/utils/index'
import { getInfo } from '@/api/login'
import TagsView from '@/layout/components/TagsView/index.vue'
import addContract from './component/addContract.vue'
export default {
  components: { TagsView, addContract },
  data() {
    return {
      // 遮罩层加载效果显示
      loading: false,
      isDisabled: false,
      // 默认查询条件
      queryParams: {
        contractName: undefined,
        contractNo: undefined,
        pageNum: 1,
        pageSize: 10,
        fieldName: undefined, // 排序字段名
        type: undefined // 排序类型type = asc升序 =desc 降序
      },
      // 右下分页选中条数
      selectedLength: 0,
      // 右下分页总条数
      total: 0,
      isShowMore: false,
      // 表格数据
      contractTable: [],
      // 新增合同基本信息对象
      formData: {
        contractNo: '',
        contractStatus: '0',
        // 付款条件数组对象
        details: [],
        // 附件列表
        fileList: []
      },
      // 供应商选择项
      supplierOptions: [],
      // 供应商查询条件
      supplierQueryParams: {
        name: undefined,
        code: undefined,
        enable: undefined,
        pageNum: 1,
        pageSize: 10
      },
      form: {},
      // 弹框标题
      title: '',
      open: false,
      isEdit: true,
      isDisabledModify: false,
      materialData: {}
    }
  },
  watch: {
    contractTable: {
      handler() {
        this.$refs.contractTable.bodyWrapper.scrollTop = 0
      },
      deep: true
    }
  },
  created() {
    this.getList()
    this.getSupplierList()
    this.getCurrentUser()
  },
  methods: {
    /** 获取供应商列表*/
    getSupplierList() {
      getSupplierList(this.addDateRange(this.supplierQueryParams)).then(
        (response) => {
          this.supplierOptions = response.data.records
        }
      )
    },
    /** 获取当前用户身份*/
    getCurrentUser() {
      getInfo().then((res) => {
        this.formData.createBy = this.$store.state.user.name
      })
    },
    /** 获取合同列表*/
    getList() {
      this.loading = true
      getContractList(this.queryParams).then(
        (response) => {
          if (response.code === 200) {
            this.contractTable = response.data.records
            this.total = response.data.total
            this.loading = false
          } else {
            this.$message.error(response.msg)
          }
        }
      )
    },
    // 排序
    sortChange(column) {
      if (column.prop === 'contractNo') {
        this.queryParams.fieldName = 'contract.contract_no'
      }
      if (column.prop === 'contractAmount') {
        this.queryParams.fieldName = 'contract.contract_amount'
      }
      // this.queryParams.fieldName = column.prop
      this.queryParams.type = column.order === 'ascending' ? 'asc' : column.order === 'descending' ? 'desc' : ''
      this.getList()
    },
    /** 格式化金额*/
    formatterAmount(row) {
      return formatPrice(row.contractAmount / 100)
    },
    handleAdd() {
      this.open = true
      this.title = '新建合同'
      this.isDisabled = false
      this.isDisabledModify = false
      this.isEdit = true
      // 表单重置
      this.$nextTick(() => {
        this.$refs.addContract.reset()
      })
    },
    handleExport() {},
    handleQuery() {
      this.queryParams.pageNum = 1
      this.getList()
    },
    resetQuery() {
      // this.resetForm('queryForm')
      this.queryParams = {
        contractName: '',
        contractNo: '',
        pageNum: 1,
        pageSize: 10,
        fieldName: undefined, // 排序字段名
        type: undefined // 排序类型type = asc升序 =desc 降序
      }
      this.$refs.contractTable.clearSort()
      this.handleQuery()
    },
    handleSelectionChange() {},
    /** 详情和编辑采购合同*/
    handleUpdate(row, index) {
      const { id } = row
      // 无论是详情还是编辑都打开对话框
      this.open = true
      // 保存当前点击的行对象数据
      if (index === 1) {
        this.title = '编辑合同'
        this.isEdit = true
        this.isDisabled = false
        this.isDisabledModify = false
        getContractDetail(id).then((res) => {
          if (res.code === 200) {
            // console.log(res)
            this.formData = res.data
            this.formData.contractAmount = formatPrice(this.formData.contractAmount / 100)
            this.$nextTick(() => {
              this.$refs.addContract.editContract()
            })
          } else {
            this.$message.error(res.msg)
          }
        })
      } else if (index === 0) {
        this.title = '详情'
        this.isEdit = false
        this.isDisabled = true
        this.isDisabledModify = true
        getContractDetail(id).then((res) => {
          if (res.code === 200) {
            this.formData = res.data
            this.formData.contractAmount = formatPrice(this.formData.contractAmount / 100)
            this.$nextTick(() => {
              this.$refs.addContract.editContract()
            })
          } else {
            this.$message.error(res.msg)
          }
        })
      }
    },
    /** 删除采购合同*/
    handleDelete(row) {
      const { id, contractName } = row
      this.$modal
        .confirm('确定删除"' + contractName + '"?', '确定删除')
        .then(function() {
          return delContract(id)
        })
        .then(() => {
          this.queryParams.pageNum = this.changePageNum(this.total, 1, this.queryParams.pageNum, this.queryParams.pageSize)
          this.getList()
          this.$modal.msgSuccess('删除成功')
        })
        .catch(() => {})
    },
    closeUser() {
      this.open = false
      this.getList()
    },
    cancel() {
      this.open = false
      this.getList()
    },
    /** 新建合同提交前查看合同号是否重复*/
    checkExit(value) {
      checkContract(value).then(
        res => {
          if (res.data === true) {
            this.$message.error('合同编号已经存在请重新输入')
          }
        }
      )
    },
    /** 新建合同提交*/
    submitForm() {
      this.$nextTick(() => {
        this.$refs.addContract.submitForm()
      })
    },
    submitContractList(data) {
      if (data.code === 200 && !data.id) {
        this.$message.success('创建成功')
        this.open = false
        this.getList()
      } else if (data.id) {
        this.$message.success('编辑成功')
        this.open = false
        this.getList()
      }
    }
  }
}
</script>
<style lang="scss" scoped>
.flod {
  width: 100px;
  text-align: right;
  font-size: 14px;
  color: #333;
  margin-right: 20px;
  margin-top: 20px;
}
.flod span {
  cursor: pointer;
}
.page{
  display: flex;
  justify-content:space-between;
}
.chooseCount {
  color: #7d8592;
  margin-top: 20px;
  margin-left: 20px;
}
.colTitle{
  height:36px;
  line-height:36px;
  padding-left:10px;
  background:#F1F2F5;
  border-right:1px solid #DDDDDD;
  border-bottom:1px solid #DDDDDD;
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
  ::v-deep .el-date-editor .el-input__inner{
    border: none;
  }
}
::v-deep .inputNoBorder .el-input__inner{
  border: none;
  border-radius: 0;
  border-bottom: 1px solid #DDDDDD;
}
::v-deep .el-dialog__body{
  line-height: 36px;
  padding: 3px 20px;
}
.uploadcontent{
  width: 760px;
  display: flex;
  justify-content: flex-start;
  margin-bottom: 30px;
  ::v-deep .el-upload__tip{
    margin-left: 50px;
    margin-top: 15px;
  }

}
.addDig {

::v-deep .el-dialog .el-dialog__body {
  padding-top: 0;
  max-height: 850px;
  overflow-y: auto;
}
::v-deep .el-table--border{
  border-bottom:1px solid #EBEEF5;
}
}
</style>
