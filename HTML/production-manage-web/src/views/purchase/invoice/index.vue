<template>
  <div>
    <!-- <top-bar ref="headBar" /> -->
    <tags-view>
      <div class="addShop_head">
        <div class="head_left">
          <!-- <p>采购发票</p> -->
        </div>
        <div class="head_right">
          <el-button size="small" @click="handleExport"><i class="iconfont icon-daochu" />导出</el-button>
          <el-button size="small" type="primary" @click="handleAdd"><i class="iconfont icon-zengjia" style="color: #ffffff;" />新建发票</el-button>
        </div>
      </div>
    </tags-view>
    <el-card class="main-card">
      <el-form
        ref="queryForm"
        :model="queryParams"
        :inline="true"
        label-width="80"
      >
        <el-form-item label="发票号码" prop="invoiceNo">
          <el-input
            v-model.trim="queryParams.invoiceNo"
            placeholder="请输入"
            clearable
            size="small"
            style="width: 200px"
            maxlength="15"
            @keyup.enter.native="handleQuery"
          />
        </el-form-item>
        <el-form-item label="发票类型" prop="invoiceType">
          <el-select v-model.trim="queryParams.invoiceType" placeholder="请选择">
            <el-option
              v-for="item in dict.type.invoice_type"
              :key="item.value"
              :label="item.label"
              :value="item.value"
            />
          </el-select>
        </el-form-item>
        <el-form-item v-if="isShowMore" label="供应商" prop="supplierName">
          <el-input
            v-model.trim="queryParams.supplierName"
            placeholder="请输入"
            clearable
            :disabled="true"
            size="small"
            style="width: 200px"
            maxlength="15"
            @keyup.enter.native="handleQuery"
          />
        </el-form-item>
        <el-form-item v-if="isShowMore" label="开票日期" prop="dateRange">
          <div class="date_box">
            <el-date-picker
              v-model="queryParams.dateRange"
              size="small"
              style="width: 200px"
              value-format="yyyy-MM-dd"
              type="daterange"
              range-separator="至"
              start-placeholder="开始日期"
              end-placeholder="结束日期"
              :clearable="false"
              @change="getDate"
            /><i class="el-icon-date data_icon" />
          </div>
        </el-form-item>
        <el-form-item>
          <el-button v-debounce size="small" @click="resetQuery">重置</el-button>
          <el-button
            size="small"
            type="primary"
            plain
            class="search-btn"
            @click="handleQuery"
          >搜索</el-button>
        </el-form-item>
      </el-form>

      <my-table ref="table" :table-data="tableData" :total="total" @delete="handleDelete" @update="handleUpdate" @select="getIds" @nextList="nextList" @sortChange="sortChange" />

    </el-card>
    <el-dialog
      :title="title"
      :visible.sync="addOrEditDig"
      width="1200px"
      append-to-body
      :close-on-click-modal="false"
      @close="closeDig"
    >
      <addInvoice ref="addInvoice" :is-disabled="isDisabled" :form-data="formData" @successSubmit="submitSuccess" />
      <div v-if="!isDisabled" slot="footer" class="dialog-footer">
        <el-button size="small" @click="cancel">取消</el-button>
        <el-button v-debounce size="small" type="primary" @click="submitForm">保存</el-button>
      </div>
    </el-dialog>
  </div>
</template>
<script>
import TagsView from '../../../layout/components/TagsView/index.vue'
import MyTable from './component/table.vue'
import AddInvoice from './component/add.vue'
import { getInvoiceList, getInvoiceDetail, delInvoice } from '../../../api/purchase/invoice'
import { formatPrice } from '@/utils/index'
export default {
  components: { AddInvoice, MyTable, TagsView },
  dicts: ['invoice_type'],
  beforeRouteLeave(to, from, next) {
    this.$store.dispatch('settings/changeSetting', {
      key: 'showTags',
      value: true
    })
    next()
  },
  data() {
    return {
      queryParams: {
        pageNum: 1,
        pageSize: 10,
        supplierName: '',
        invoiceType: '',
        invoiceNo: '',
        dateRange: [],
        fieldName: undefined, // 排序字段名
        type: undefined // 排序类型type = asc升序 =desc 降序
      },

      isShowMore: false,
      total: 0,
      tableData: [],
      ids: [],
      title: '',
      addOrEditDig: false,
      formData: {},
      isDisabled: false
    }
  },
  // activated() {
  //   this.$store.dispatch('settings/changeSetting', {
  //     key: 'showTags',
  //     value: false
  //   })
  //   // const ele = document.getElementById('tags-title-name')
  //   // ele.parentNode.replaceChild(this.$refs.headBar.$el, ele)
  // },
  created() {
    this.getList()
    this.$store.dispatch('settings/changeSetting', {
      key: 'showTags',
      value: false
    })
  },
  methods: {
    getDate(val) {

    },
    nextList(queryParams) {
      this.queryParams.pageNum = queryParams.pageNum
      this.queryParams.pageSize = queryParams.pageSize
      this.getList()
    },
    getList() {
      this.loading = true
      const data = {
        pageNum: this.queryParams.pageNum,
        pageSize: this.queryParams.pageSize,
        supplierName: this.queryParams.supplierName,
        invoiceType: this.queryParams.invoiceType,
        invoiceNo: this.queryParams.invoiceNo,
        startDate: this.queryParams.dateRange[0],
        endDate: this.queryParams.dateRange[1],
        fieldName: this.queryParams.fieldName,
        type: this.queryParams.type
      }
      getInvoiceList(data).then(
        (res) => {
          if (res.code === 200) {
            this.loading = false
            this.tableData = res.data.records
            this.tableData.map((e) => {
              e.invoicePrice = e.invoiceAmount
              e.invoiceAmount = e.invoiceAmount ? formatPrice((e.invoiceAmount / 100).toFixed(2)) : ''
              e.invoiceTaxAmount = e.invoiceTaxAmount ? formatPrice((e.invoiceTaxAmount / 100).toFixed(2)) : ''
              e.contractAmount = e.contractAmount ? formatPrice((e.contractAmount / 100).toFixed(2)) : ''
            })
            this.total = res.data.total
          } else {
            this.$message.error(res.msg)
          }
        }
      )
    },
    sortChange(column) {
      this.queryParams.fieldName = column.prop
      this.queryParams.type = column.order === 'ascending' ? 'asc' : column.order === 'descending' ? 'desc' : ''
      this.getList()
    },
    handleClick(tab, event) {

    },
    handleQuery() {
      this.queryParams.pageNum = 1
      this.getList()
    },
    resetQuery() {
      // this.resetForm('queryForm')
      this.queryParams = {
        pageNum: 1,
        pageSize: 10,
        supplierName: '',
        invoiceType: '',
        invoiceNo: '',
        dateRange: [],
        fieldName: undefined, // 排序字段名
        type: undefined // 排序类型type = asc升序 =desc 降序
      }
      this.$refs.table.$refs.multipleTable.clearSort()
      this.handleQuery()
    },
    handleAdd() {
      this.isDisabled = false
      this.addOrEditDig = true
      this.$nextTick(() => {
        this.$refs.addInvoice.reset()
      })
      this.title = '新建发票'
    },
    closeDig() {
      this.$refs.addInvoice.reset()
      this.addOrEditDig = false
    },
    // 取消按钮
    cancel() {
      this.$refs.addInvoice.reset()
      this.addOrEditDig = false
    },
    submitForm() {
      this.$refs.addInvoice.submitForm()
    },
    submitSuccess(data) {
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
    // 编辑或者详情
    handleUpdate(row, index) {
      if (index === 1) {
        this.title = '编辑采购发票'
        this.isDisabled = false
        getInvoiceDetail(row.id).then((res) => {
          if (res.code === 200) {
            this.formData = res.data
            this.formData.invoiceAmount = this.formData.invoiceAmount ? formatPrice(Number(this.formData.invoiceAmount / 100).toFixed(2)) : ''
            this.formData.invoiceTaxAmount = this.formData.invoiceTaxAmount ? formatPrice(Number(this.formData.invoiceTaxAmount / 100).toFixed(2)) : ''
            this.formData.contractAmount = this.formData.contractAmount ? formatPrice(Number(this.formData.contractAmount / 100).toFixed(2)) : ''
            this.addOrEditDig = true
            this.$nextTick(() => {
              this.$refs.addInvoice.getDetail()
            })
          } else { this.$message.error(res.msg) }
        })
      } else {
        this.title = '采购发票详情'
        this.isDisabled = true
        getInvoiceDetail(row.id).then((res) => {
          if (res.code === 200) {
            this.formData = res.data
            this.formData.invoiceAmount = this.formData.invoiceAmount ? formatPrice(Number(this.formData.invoiceAmount / 100).toFixed(2)) : ''
            this.formData.invoiceTaxAmount = this.formData.invoiceTaxAmount ? formatPrice(Number(this.formData.invoiceTaxAmount / 100).toFixed(2)) : ''
            this.formData.contractAmount = this.formData.contractAmount ? formatPrice(Number(this.formData.contractAmount / 100).toFixed(2)) : ''
            this.addOrEditDig = true
            this.$nextTick(() => {
              this.$refs.addInvoice.getDetail()
            })
          } else { this.$message.error(res.msg) }
        })
      }
    },
    // 删除
    handleDelete(row) {
      const id = row.id || this.ids
      const name = row.invoiceNo
      this.$modal
        .confirm('确定删除采购发票"' + name + '"？', '确定删除')
        .then(function() {
          return delInvoice(id)
        })
        .then((res) => {
          if (res.code === 200) {
            this.queryParams.pageNum = this.changePageNum(this.total, 1, this.queryParams.pageNum, this.queryParams.pageSize)
            this.getList()
            this.$modal.msgSuccess('删除成功')
          } else {
            this.$message.error(res.msg)
          }
        })
        .catch(() => {})
    },
    getIds(ids) {
      this.ids = ids
    },
    /** 导出 */
    handleExport() {
      this.download(
        '/biz/procurement/invoice/export/',
        this.ids,
        `采购发票_${this.parseTime(new Date(), '{y}-{m}-{d}')}.xlsx`
      )
    }

  }
}
</script>
<style lang="scss" scoped>
.addShop_head{
  height: 64px;
  display: flex;
  align-items: center;
  justify-content: space-between;
  .head_left{
    font-size: 20px;
    font-weight: 500;
    color: #333333;
    font-family: SourceHanSansCN-Medium, SourceHanSansCN;
  }
}
.inputContent {
  display: flex;
  justify-content: space-between;
  border-bottom: 1px solid #eaedf4;
}
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

::v-deep .el-radio-button:first-child .el-radio-button__inner{
  border-radius: 2px 0 0 2px;
}
::v-deep .el-radio-button:last-child .el-radio-button__inner{
  border-radius: 0 2px 2px 0;
}
.my-table{
  padding: 0 16px;
}
.table-tool{
  display: flex;
  height: 100%;
  justify-content: flex-end;
  margin-right: 11px;
  .items{
    margin: auto 5px;
    width: 32px;
    height: 32px;
    background: #FFFFFF;
    border-radius: 4px;
    border: 1px solid #DDDDDD;
    display: flex;
    cursor: pointer;
    img{
      margin: auto;
    }
  }

}
.date_box {
  position: relative;
  width: fit-content;
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
.my-dropdown{
  margin: auto 0;
}
.my-dropdown-menu{
  padding:6px 16px;
  ::v-deep .el-checkbox{
    line-height: 36px;
  }
}
.my-checkbox-group{
  display: flex;
  flex-flow: column;
}
</style>
