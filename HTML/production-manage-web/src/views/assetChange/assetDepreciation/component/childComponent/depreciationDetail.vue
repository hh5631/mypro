<template>
  <div class="overAll">
    <!-- content -->
    <el-card class="main-card">
      <!-- search form -->
      <el-form
        ref="queryForm"
        :model="queryParams"
        :inline="true"
        label-width="80"
      />
      <DetailTable
        ref="myTable"
        :type="1"
        :table-datas="tableDatas"
        :query-params="queryParams"
        :total="total"
        @nextList="nextList"
        @sortChange="sortChange"
      />
    </el-card>
  </div>
</template>
<script>
import DetailTable from './detailTable.vue'
import { getAssDepHisList } from '@/api/assetChange/index'
export default {
  components: { DetailTable },
  props: {
    idKey: {
      type: Number,
      default: 0
    }
  },
  data() {
    return {
      queryParams: {
        pageNum: 1,
        pageSize: 10
      },
      // 分页总数
      total: 0,
      checkAll: true,
      // 传给子组件的数据
      tableDatas: [],
      readStatus: 0
    }
  },

  watch: {
    idKey(val) {
      this.tableDatas = []
      this.getList()
    }
  },

  created() {
    this.getList()
    // this.$store.dispatch('settings/changeSetting', {
    //   key: 'showTags',
    //   value: false
    // })
  },
  methods: {
    /** 获取列表数据 */
    getList(params) {
      const data = {
        depreciationId: this.idKey,
        pageNum: this.queryParams.pageNum,
        pageSize: this.queryParams.pageSize
      }
      getAssDepHisList(data).then(res => {
        this.tableDatas = res.data.records
        this.total = res.data.total
      })
    },
    nextList(queryParams) {
      this.queryParams.pageNum = queryParams.pageNum
      this.queryParams.pageSize = queryParams.pageSize
      this.getList()
    },
    // 排序
    sortChange(column) {
      // this.queryParams.fieldName = column.prop
      // this.queryParams.type = column.order === 'ascending' ? 'asc' : column.order === 'descending' ? 'desc' : ''
      this.getList()
    },

    handleClick(tab, event) {

    }

  }
}
</script>
<style lang="scss" scoped>

  .overAll{

    .main-card{
      height: 530px;
      border: 0px solid;
      margin: 0px;

      ::v-deep .el-form{
      border: 0px solid ;
      margin: 0px;
      }
      ::v-deep.el-card__body{
        padding: 0;
      }
    }
    .headline{
      display: flex;
      flex-direction: row; /* 子元素横向排列 */
      justify-content:space-between; /* 相对父元素水平居中 */
      align-items: center;/* 相对父元素上下居中 */
      .allRead{
        background-color:#4686EF;
        border-radius: 10px;
        color: #FFFFFF;
        width: 80px;
        height: 30px;
        display: flex;
        justify-content: center; /* 相对父元素水平居中 */
        align-items: center;
        cursor: pointer;
      }
    }
  }
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
