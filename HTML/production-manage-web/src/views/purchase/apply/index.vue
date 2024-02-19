<template>
  <div>
    <!-- <top-bar ref="headBar" /> -->
    <tags-view>
      <div class="addShop_head">
        <div class="head_left">
          <!-- <p>采购计划</p> -->
        </div>
        <div class="head_right">
          <el-button v-debounce size="small" @click="handlePrint"><i class="el-icon-printer" />打印单据</el-button>
          <el-button v-debounce size="small" type="primary" @click="addApply"><i class="el-icon-plus" />新建申请</el-button>
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
        <el-form-item label="状态" prop="status">
          <el-select v-model="queryParams.status" placeholder="请选择">
            <el-option
              v-for="item in dict.type.procurement_order_status"
              :key="item.value"
              :label="item.label"
              :value="item.value"
            />
          </el-select>
        </el-form-item>
        <el-form-item label="标题" prop="title">
          <el-input
            v-model.trim="queryParams.title"
            placeholder="请输入"
            clearable
            size="small"
            style="width: 200px"
            maxlength="15"
            @keyup.enter.native="handleQuery"
          />
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
        <el-form-item label="采购负责人" prop="procurementLeader">
          <el-input
            v-model.trim="queryParams.procurementLeader"
            placeholder="请输入"
            clearable
            size="small"
            style="width: 200px"
            maxlength="15"
            @keyup.enter.native="handleQuery"
          />
        </el-form-item>
        <el-form-item label="采购单编号" prop="orderNo">
          <el-input
            v-model.trim="queryParams.orderNo"
            placeholder="请输入"
            clearable
            size="small"
            style="width: 200px"
            maxlength="15"
            @keyup.enter.native="handleQuery"
          />
        </el-form-item>
        <el-form-item label="计划编号" prop="orderNumber">
          <el-input
            v-model.trim="queryParams.orderNumber"
            placeholder="请输入"
            clearable
            size="small"
            style="width: 200px"
            maxlength="15"
            @keyup.enter.native="handleQuery"
          />
        </el-form-item>
        <el-form-item>
          <el-button v-debounce size="small" @click="resetQuery">重置</el-button>
          <el-button
            v-debounce
            v-hasPermi="['procurement:order:list']"
            size="small"
            type="primary"
            plain
            class="search-btn"
            @click="handleQuery"
          >搜索</el-button>
        </el-form-item>
      </el-form>
      <el-radio-group v-model="queryParams.dataRange" size="small" @change="chooseBtn">
        <el-radio-button
          v-for="(item,index) in radioList"
          :key="index"
          :label="item.label"
        >{{ item.name }}</el-radio-button>
      </el-radio-group>
      <my-table
        ref="myTable"
        :type="2"
        :table-datas="tableDatas"
        :total="total"
        :query-params="queryParams"
        :mark-key="Number(queryParams.dataRange)"
        :checked-colum="checkedColum"
        @deleteSuccess="deleteSuccess"
        @getList="getList"
        @sortChange="sortChange"
        @handleSelectionChange="handleSelectionChange"
      />
    </el-card>
    <FilePrint ref="FilePrint" type="purchaseApply" />
  </div>
</template>
<script>
import TagsView from '@/layout/components/TagsView/index.vue'
import MyTable from './component/table.vue'
import { getApplyList, getApplyDetail } from '@/api/purchase/index'
import FilePrint from '@/components/FilePrint'
export default {
  components: { MyTable, TagsView, FilePrint },
  dicts: ['procurement_order_status'],
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
        dataRange: 0,
        pageNum: 1,
        pageSize: 10,
        title: undefined,
        orderNo: undefined,
        procurementLeader: undefined,
        orderNumber: undefined,
        fieldName: undefined, // 排序字段名
        type: undefined // 排序类型type = asc升序 =desc 降序
      },
      // 选中数组
      ids: [],
      dateRange: [],
      tableDatas: [],
      total: 0,
      radioList: [{ label: '0', name: '所有申请' },
        { label: '1', name: '我发起的' },
        { label: '2', name: '草稿' }],
      // { label: '审批中', name: '审批中' },
      // { label: '通过', name: '通过' },
      // { label: '驳回', name: '驳回' },
      // { label: '不通过', name: '不通过' },
      // { label: '废弃', name: '废弃' }],
      checkAll: true,
      checkedColum: ['采购单编号', '标题', '采购负责人', '申请时间', '状态'],
      tableColum: ['采购单编号', '申请标题', '采购负责人', '申请时间', '供应商'],
      isIndeterminate: false,
      options: [{
        value: 1,
        label: '黄金糕'
      }, {
        value: 2,
        label: '双皮奶'
      }, {
        value: 3,
        label: '蚵仔煎'
      }, {
        value: 4,
        label: '龙须面'
      }, {
        value: 5,
        label: '北京烤鸭'
      }]
    }
  },

  watch: {
    '$route.query.mark': {
      handler(val) {
        if (val) {
          this.queryParams.dataRange = Number(val)
        }
      },
      immediate: true
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
    getList() {
      const data = {
        ...this.queryParams,
        startDate: this.dateRange[0],
        endDate: this.dateRange[1]
      }
      getApplyList(data).then(res => {
        this.tableDatas = res.data.records
        this.total = res.data.total
      })
    },
    handleClick(tab, event) {
      // console.log(tab, event)
    },
    sortChange(column) {
      this.queryParams.fieldName = column.prop
      this.queryParams.type = column.order === 'ascending' ? 'asc' : column.order === 'descending' ? 'desc' : ''
      this.getList()
    },
    handleSelectionChange(selection) {
      // console.log(selection, '选中的数组')
      this.ids = selection.map(item => item.id)
    },
    /** 打印 */
    handlePrint() {
      // console.log('点击打印')
      if (!this.ids.length) {
        return this.$message.warning('请勾选要打印的项')
      }
      this.$modal.loading('正在获取打印数据，请稍候...')
      Promise.all(this.ids.map(item => getApplyDetail(item))).then(res => {
        // console.log(res)
        this.$modal.closeLoading()
        this.$refs.FilePrint.show(res.map(item => item.data))
      })
    },
    chooseBtn(label) {
      this.queryParams.dataRange = Number(label)
      this.getList()
    },
    handleQuery() {
      this.getList()
    },
    resetQuery() {
      this.queryParams = {
        dataRange: 0,
        pageNum: 1,
        pageSize: 10,
        title: undefined,
        orderNo: undefined,
        procurementLeader: undefined,
        orderNumber: undefined,
        fieldName: undefined, // 排序字段名
        type: undefined // 排序类型type = asc升序 =desc 降序
      }

      this.$refs.myTable.$refs.multipleTable.clearSort()
      this.handleQuery()
    },
    handleCheckAllChange(val) {
      this.checkedColum = val ? ['计划编号', '标题', '预期完成时间', '资产名称', '创建人', '创建时间'] : []
      this.isIndeterminate = false
    },
    handleCheckedCitiesChange(value) {
      const checkedCount = value.length
      this.checkAll = checkedCount === this.tableColum.length
      this.isIndeterminate = checkedCount > 0 && checkedCount < this.tableColum.length
    },
    addApply() {
      // { path: '/applyDetail', query: { id: row.id, isDetail: 0, mark: this.markNum }
      this.$router.push({ path: '/addApply', query: { isDetail: 0, mark: this.queryParams.dataRange }})
    },
    deleteSuccess() {
      setTimeout(() => {
        this.getList()
      }, 1000)
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
.el-icon-printer{
  padding:1px 10px 1px 0;
}
</style>
