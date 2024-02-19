<template>
  <div>
    <tags-view>
      <div class="addShop_head">
        <div v-if="show" class="head_left">
          <p>采购计划-详情</p>
          <p class="create_time">创建时间：{{ baseInfo.createTime || '' }}</p>
        </div>
        <div>
          <!-- <el-button size="small" type="primary">暂存</el-button> -->
          <!-- <el-button size="small" type="primary">保存并新建</el-button>
          <el-button size="small" type="primary">保存</el-button> -->
          <!-- <el-button size="small" @click="routerBack">返回</el-button> -->
          <el-button size="small" @click="routerBack">返回</el-button>
        </div>
      </div>
    </tags-view>
    <div class="p-d-card-box">
      <el-card class="add_plan">
        <div class="addShop_content">
          <BaseInfo :is-detail="this.$route.query.isDetail" :base-infos="baseInfo" />
          <MaterialDetail :is-detail="this.$route.query.isDetail" :table-datas="tableData" />

        </div>
      </el-card>
      <!-- 审批流程！！！！！！！！ -->
      <!-- <ApprovalProcess :id="baseInfo.id" /> -->
      <el-card class="add_plan">
        <collapse title="操作日志">
          <div>
            <el-table ref="logTableData" border :data="logTableData" class="table-class" highlight-current-row max-height="300">
              <el-table-column prop="operationTime" label="操作时间" align="center" />
              <el-table-column prop="operation" label="节点名称" align="center" />
              <el-table-column prop="operator" label="操作者" align="center" />
              <el-table-column prop="position" label="职位" align="center" />
              <el-table-column prop="operation" label="操作" align="center" />
            </el-table>
          </div>
        </collapse>
      </el-card>
    </div>

  </div>
</template>

<script>
import collapse from './component/collapse.vue'
// import addDetail from './component/addDetail.vue'
import { getDetail } from '@/api/purchase/index'
import { getLog } from '@/api/warehouse/index'
import TagsView from '../../../layout/components/TagsView/index.vue'
import BaseInfo from './component/baseInfo.vue'
import MaterialDetail from './component/materialDetail'
// import ApprovalProcess from './component/approvalProcess.vue'
export default {
  components: { TagsView, BaseInfo, MaterialDetail, collapse },
  data() {
    return {
      isActive: false,
      activeNames: ['basicInfo', 'detail'],
      baseInfo: {},
      tableData: [],
      show: true,
      logTableData: []
    }
  },
  watch: {
    $route: {
      handler(newValue, oldValue) {
        if (newValue.name === 'planDetail') { this.getDetail(); this.show = true }
      }
    }
  },
  created() {
    this.getDetail()
  },
  methods: {
    /** 根据路由参数获取详情 */
    getDetail() {
      const { id } = this.$route.query
      // 获取详情
      getDetail(id).then(
        res => {
          if (res.code === 200) {
            this.baseInfo = res.data

            this.tableData = res.data.detailVOs.map(item => {
              const spliceLength2 = item.materialCategory.lastIndexOf('/')
              var cate = item.materialCategory.slice(spliceLength2 + 1)
              return {
                ...item, materialCategory: cate
              }
            })
            this.getLogList(id)
          }
        }
      )
    },
    /** 获取操作日志 */
    getLogList(id) {
      const data = {
        bizOrderId: id,
        bizType: 'type_purchase_plan',
        pageNum: 1,
        pageSize: 10000
      }
      getLog(data).then(response => {
        if (response.code === 200) {
          this.logTableData = response.data.records
        } else {
          this.$message.error(response.msg)
        }
      })
    },
    /** 返回 */
    routerBack() {
      this.show = false
      // this.$router.go(-1)
      this.$router.push({ path: '/purchase/plan', query: { mark: this.$route.query.mark }})
    }
  }
}
</script>

<style scoped lang="scss">
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
    display: flex;
    .create_time{
      font-size: 16px;
      height: 64px;
      line-height: 70px;
      font-family: Source Han Sans CN-Normal, Source Han Sans CN;
      color: #7D8592;
      margin-left: 16px;
    }
  }
}
.p-d-card-box {
  margin: 16px;
  display: flex;
  flex-direction: column;
  gap: 16px;
  height: calc(100vh - 152px);
}

.add_plan{
  flex: 1;
  overflow-y: auto;
  min-height: 0
}
::v-deep .el-date-editor.el-input{
  width: 100%;
}
::v-deep .el-input--small .el-input__inner{
  height: 36px;
}
::v-deep .el-input__icon.el-icon-date{
  display: none;
}
::v-deep .el-input--prefix .el-input__inner{
  padding-left:15px;
}

</style>
