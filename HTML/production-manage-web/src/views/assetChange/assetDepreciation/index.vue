<template>
  <div>
    <!-- <top-bar ref="headBar" /> -->
    <tags-view>
      <div class="addShop_head">
        <div class="head_left">
          <!-- <p>采购发票</p> -->
        </div>
        <div class="head_right">

          <el-button size="small" type="primary" @click="setDepreciation">
            <i class="iconfont icon-zengjia" style="color: #ffffff;" />
            设置折旧
          </el-button>
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
        <el-form-item label="资产编码" prop="internalCode">
          <el-input
            v-model.trim="queryParams.internalCode"
            placeholder="请输入"
            clearable
            size="small"
            style="width: 200px"
            maxlength="20"
            @keyup.enter.native="handleQuery"
          />
        </el-form-item>
        <el-form-item label="资产名称" prop="assetName">
          <el-input
            v-model.trim="queryParams.assetName"
            placeholder="请输入"
            clearable
            size="small"
            style="width: 200px"
            maxlength="15"
            @keyup.enter.native="handleQuery"
          />
        </el-form-item>
        <el-form-item label="折旧方法" prop="depreciationType">
          <el-select v-model.trim="queryParams.depreciationType" placeholder="请选择" clearable>
            <el-option
              v-for="item in dict.type.sys_depreciation_type"
              :key="item.value"
              :label="item.label"
              :value="item.value"
            />
          </el-select>
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

      <my-table ref="table" :table-data="tableData" :total="total" :query-params-key="queryParams" @select="select" @nextList="nextList" @sortChange="sortChange" />

    </el-card>
    <el-dialog
      :title="setTitle"
      :visible.sync="setDigShow"
      width="800px"
      append-to-body
      :close-on-click-modal="false"
      @close="closeDig"
    >
      <SetInvoice ref="set" :select-row="selectRow" />
      <div slot="footer" class="dialog-footer">
        <el-button size="small" @click="cancel">取消</el-button>
        <el-button v-debounce size="small" type="primary" @click="setSave">保存</el-button>
      </div>
    </el-dialog>

  </div>
</template>
<script>
import TagsView from '../../../layout/components/TagsView/index.vue'
import MyTable from './component/table.vue'
import SetInvoice from './component/set.vue'
import { getAssetDepreciationList, postSetDepreciation } from '../../../api/assetChange/index'
export default {
  components: { MyTable, TagsView, SetInvoice },
  dicts: ['sys_depreciation_type'],
  data() {
    return {
      queryParams: {
        pageNum: 1,
        pageSize: 10,
        internalCode: null, // 资产编码
        assetName: '', // 资产名称
        depreciationType: '', // 折旧方法
        isChineseField: true,
        fieldName: undefined, // 排序字段名
        type: undefined // 排序类型type = asc升序 =desc 降序
      },
      isShowMore: false,
      total: 0,
      tableData: [],
      ids: [],
      setTitle: '',
      setDigShow: false,
      formData: {},
      selectRow: undefined
    }
  },

  created() {
    this.getList()
    this.$store.dispatch('settings/changeSetting', {
      key: 'showTags',
      value: false
    })
  },
  methods: {

    nextList(queryParams2) {
      console.log(queryParams2, 'queryParams2============')
      this.queryParams.pageNum = queryParams2.pageNum
      this.queryParams.pageSize = queryParams2.pageSize
      this.getList()
    },
    getList() {
      this.loading = true
      const data = {
        pageNum: this.queryParams.pageNum,
        pageSize: this.queryParams.pageSize,
        internalCode: this.queryParams.internalCode,
        assetName: this.queryParams.assetName,
        depreciationType: this.queryParams.depreciationType,
        fieldName: this.queryParams.fieldName,
        type: this.queryParams.type

      }
      getAssetDepreciationList(data).then(
        (res) => {
          if (res.code === 200) {
            this.loading = false
            this.tableData = res.data.records
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
      // console.log(tab, event)
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
      // this.$refs.table.$refs.multipleTable.clearSort()
      this.handleQuery()
    },
    setDepreciation() {
      // debugger
      // 设置折旧方法
      if (!this.selectRow) {
        this.$message.error('请选中一条信息进行设置')
      } else {
        this.setDigShow = true
      }

      this.setTitle = '设置折旧'
    },
    closeDig() {
      // this.$refs.addInvoice.reset()
      this.setDigShow = false
    },
    // 取消按钮
    cancel() {
      // this.$refs.addInvoice.reset()
      this.setDigShow = false
    },
    submitForm() {
      this.$refs.addInvoice.submitForm()
    },

    enterMessage() {
      this.messageDig = true
      this.msgTitle = '消息通知'
    },
    select(val) {
      // 选中的一行
      this.selectRow = val
      console.log('val22==', val)
    },
    setSave() {
      // console.log('执行11', this.$refs)

      const arrayList = this.$refs.set.depreciation
      const data = {
        depreciationRate: arrayList.depreciationRate / 100,
        depreciationType: arrayList.depreciationType,
        id: arrayList.id,
        residualValueRate: arrayList.residualValueRate / 100,
        serviceLife: arrayList.serviceLife
      }
      postSetDepreciation(data).then(
        (res) => {
          if (res.code === 200) {
            this.setDigShow = false
            this.getList()
          } else {
            this.$message.error('保存失败')
            this.setDigShow = false
          }
        })
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
