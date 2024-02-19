<template>
  <div>
    <!-- <top-bar ref="headBar" /> -->
    <tags-view>
      <div class="addShop_head">
        <div class="head_left">
          <!-- <p>采购计划</p> -->
        </div>
        <div class="head_right">
          <el-button size="small" type="primary" @click="addPlan"><i class="iconfont icon-zengjia" style="font-size: 14px;margin-right: 8px;" />新建计划</el-button>
        </div>
      </div>
    </tags-view>
    <!-- content -->
    <el-card class="main-card">
      <!-- search form -->
      <el-form
        ref="queryForm"
        :model="queryParams"
        :inline="true"
        label-width="80"
      >
        <el-form-item label="状态" prop="state">
          <el-select v-model="queryParams.status" placeholder="请选择" size="small">
            <el-option
              v-for="item in dict.type.procurement_plan_status"
              :key="item.value"
              :label="item.label"
              :value="item.value"
            />
          </el-select>
        </el-form-item>
        <el-form-item label="计划编号" prop="orderNo">
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
        <el-form-item label="创建人" prop="creator">
          <el-input
            v-model.trim="queryParams.creator"
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
      <!-- change button -->
      <el-row type="flex" :gutter="24">
        <el-col :span="16">
          <el-radio-group v-model="queryParams.dataRange" size="small" @change="chooseBtn">
            <el-radio-button
              v-for="(item,index) in radioList"
              :key="index"
              :label="item.label"
            >{{ item.name }}</el-radio-button>
          </el-radio-group>
        </el-col>
        <!-- <el-col :span="8">
          <div class="table-tool">
            <el-dropdown class="my-dropdown">
              <div class="items">
                <img src="@/assets/images/filter.png" alt="">
              </div>
              <el-dropdown-menu slot="dropdown" class="my-dropdown-menu">
                <el-checkbox v-model="checkAll" :indeterminate="isIndeterminate" @change="handleCheckAllChange">全选</el-checkbox>
                <el-checkbox-group v-model="checkedColum" class="my-checkbox-group" @change="handleCheckedCitiesChange">
                  <el-checkbox v-for="t in tableColum" :key="t" :label="t">{{ t }}</el-checkbox>
                </el-checkbox-group>
              </el-dropdown-menu>
            </el-dropdown>
            <div class="items">
              <img src="@/assets/images/refresh.png" alt="">
            </div>
          </div>
        </el-col> -->
      </el-row>
      <!-- table展示页 -->
      <my-table ref="myTable" :type="1" :table-datas="tableDatas" :query-params="queryParams" :mark-key="Number(queryParams.dataRange)" :total="total" :checked-colum="checkedColum" @deleteSuccess="deleteSuccess" @getList="getList" @sortChange="sortChange" />
    </el-card>
  </div>
</template>
<script>
import TagsView from '../../../layout/components/TagsView/index.vue'
import MyTable from './component/table.vue'
import { getList } from '@/api/purchase/index'
export default {
  components: { MyTable, TagsView },
  dicts: ['procurement_plan_status'],
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
        status: undefined,
        creator: undefined,
        orderNo: undefined,
        title: undefined,
        dataRange: 0,
        fieldName: undefined, // 排序字段名
        type: undefined // 排序类型type = asc升序 =desc 降序
      },
      dateRange: [],
      // 分页总数
      total: 0,
      radioList: [{ label: '0', name: '所有计划' },
        { label: '1', name: '我发起的' }],
      // { label: '审批中', name: '审批中' },
      // { label: '通过', name: '通过' },
      // { label: '驳回', name: '驳回' },
      // { label: '不通过', name: '不通过' },
      // { label: '废弃', name: '废弃' }],
      checkAll: true,
      // 表格列名
      checkedColum: ['计划编号', '标题', '预期完成时间', '资产名称', '创建人', '创建时间', '状态'],
      tableColum: ['采购单编号', '申请标题', '采购负责人', '申请时间', '供应商'],
      isIndeterminate: false,
      // 传给子组件的数据
      tableDatas: []
    }
  },
  // activated() {
  //   this.getList()
  //   this.$store.dispatch('settings/changeSetting', {
  //     key: 'showTags',
  //     value: false
  //   })
  //   // const ele = document.getElementById('tags-title-name')
  //   // ele.parentNode.replaceChild(this.$refs.headBar.$el, ele)
  // },
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
    /** 获取采购计划列表数据 */
    getList() {
      getList(this.datesRange(this.queryParams, this.dateRange)).then(res => {
        this.tableDatas = res.data.records
        this.total = res.data.total
      })
    },
    // 排序
    sortChange(column) {
      this.queryParams.fieldName = column.prop
      this.queryParams.type = column.order === 'ascending' ? 'asc' : column.order === 'descending' ? 'desc' : ''
      this.getList()
    },
    /** 监听删除完成重新获取列表 */
    deleteSuccess() {
      this.getList()
    },
    handleClick(tab, event) {

    },
    /** 搜索相关 */
    handleQuery() {
      this.getList()
    },
    /** 切换按钮 */
    chooseBtn(label) {
      this.queryParams.dataRange = label
      this.getList()
    },
    /** 重置 */
    resetQuery() {
      this.queryParams = {
        pageNum: 1,
        pageSize: 10,
        status: undefined,
        creator: undefined,
        orderNo: undefined,
        title: undefined,
        dataRange: 0,
        fieldName: undefined, // 排序字段名
        type: undefined // 排序类型type = asc升序 =desc 降序
      }
      this.dateRange = []
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
    /** 新建采购计划跳转路由 */
    addPlan() {
      this.$router.push({ path: '/addPlan', query: { isDetail: 0, mark: this.queryParams.dataRange }})
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
.head-search{
  padding: 16px 16px 0 16px;
  border-bottom: 1px solid #EAEDF4;
  ::v-deep .el-form-item{
    margin-bottom: 16px;
  }
}
// ::v-deep .el-radio-group{
//   margin-bottom: 8px;
// }
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
