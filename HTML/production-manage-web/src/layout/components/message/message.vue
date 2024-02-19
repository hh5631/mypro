<template>
  <div class="overAll">

    <!-- content -->
    <el-card class="main-card">
      <div class="msgState">
        <div>
          <span>未读</span>
          <span class="unreadNum"> {{ unread }}</span><span> 件</span>
          <span class="read">已读</span>
          <span style="color:#4686EF;"> {{ read }}</span><span> 件</span>
        </div>
        <div>
          <div class="allRead" @click="allRead">全部为已读</div>
        </div>
      </div>
      <!-- search form -->
      <el-form
        ref="queryForm"
        :model="queryParams"
        :inline="true"
        label-width="80"
      />

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
      </el-row>
      <my-table
        ref="myTable"
        :type="1"
        :table-datas="tableDatas"
        :query-params="queryParams"
        :total="total"
        @deleteSuccess="deleteSuccess"
        @getList="getList"
        @getMsgNum="getMsgNum"
        @sortChange="sortChange"
      />
    </el-card>
  </div>
</template>
<script>
import MyTable from './msgTable.vue'
import { getbyCondition, getNumByReadType, putAllRead } from '@/api/assetChange/index'
export default {
  components: { MyTable },
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
        dataRange: 0,
        fieldName: undefined, // 排序字段名
        type: undefined // 排序类型type = asc升序 =desc 降序,
      },
      read: 0,
      unread: 0,
      // 分页总数
      total: 0,
      radioList: [{ label: '0', name: '未读' },
        { label: '1', name: '已读' }],
      checkAll: true,

      // 传给子组件的数据
      tableDatas: [],
      readStatus: 0
    }
  },

  created() {
    this.getMsgNum(0)
    this.getMsgNum(1)
    this.getList()
    this.$store.dispatch('settings/changeSetting', {
      key: 'showTags',
      value: false
    })
  },
  methods: {
    /** 获取列表数据 */
    getList(params) {
      const data = {
        pageNum: params ? params.pageNum : this.queryParams.pageNum,
        pageSize: params ? params.pageSize : this.queryParams.pageSize,
        readStatus: this.readStatus
      }
      getbyCondition(data).then(res => {
        this.tableDatas = res.data.records
        // 如果大于1的页数没有数据了，请求上一页
        if (data.pageNum > 1 && this.tableDatas.length === 0) {
          data.pageNum = data.pageNum - 1
          this.getList(data)
        }

        this.total = res.data.total
      })
    },
    getMsgNum(num) {
      // 已读未读数据查询
      getNumByReadType({ readStatus: num }).then(res => {
        if (res.code === 200 && num === 0) {
          this.unread = res.data

          this.$emit('unread', this.unread)
        } else if (res.code === 200 && num === 1) {
          this.read = res.data
        }
      })
    },
    // 排序
    sortChange(column) {
      // this.queryParams.fieldName = column.prop
      // this.queryParams.type = column.order === 'ascending' ? 'asc' : column.order === 'descending' ? 'desc' : ''
      this.getList()
    },
    /** 监听删除完成重新获取列表 */
    deleteSuccess() {
      this.getList()
    },
    handleClick(tab, event) {

    },

    /** 切换按钮 */
    chooseBtn(label) {
      this.readStatus = label
      this.getList()
    },

    allRead() {
      putAllRead().then(res => {
        if (res.code === 200) {
          this.getMsgNum(0)
          this.getMsgNum(1)
          this.getList()
        }
      })
    }
  }
}
</script>
<style lang="scss" scoped>
  .overAll{
    .main-card{
      height: 630px;
      border: 0px solid;
      ::v-deep .el-card__body{
        padding: 0px;
      }
      .msgState{
        display: flex;
        flex-direction: row; /* 子元素横向排列 */
        justify-content:space-between; /* 相对父元素水平居中 */
        align-items: center;/* 相对父元素上下居中 */
        .unreadNum{
          color:red;
          font-size: 18px;
        }
      }
      .read{
        margin-left: 20px;
      }
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
        margin-bottom: 10px;
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
