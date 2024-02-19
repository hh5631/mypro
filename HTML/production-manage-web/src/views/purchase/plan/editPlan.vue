<template>
  <!-- 应该和addPlan用同一个页面即可 -->
  <div>
    <tags-view>
      <div class="addShop_head">
        <div v-if="this.$route.name==='editPlan'" class="head_left">
          <span>{{ this.$route.query.isDetail==='1'?'采购计划-详情':'采购计划-编辑' }}</span>
          <span class="create_time">创建时间：{{ createTime }}</span>
        </div>
        <div>
          <!-- <el-button size="small" type="primary">暂存</el-button> -->
          <!-- <el-button size="small" type="primary">保存并新建</el-button> -->
          <el-button v-if="this.$route.query.isDetail === '0'" size="small" type="primary" @click="submit">保存</el-button>
          <el-button size="small" @click="goBack">返回</el-button>
        </div>
      </div>
    </tags-view>
    <el-card v-loading="loading" class="main-card">
      <div class="addShop_content">
        <BaseInfo ref="baseInfoRef" :base-infos="baseInfo" :is-detail="this.$route.query.isDetail" />
        <MaterialDetail :table-datas="tableData" @submitData="submitData" />
      </div>
    </el-card>
    <!-- 审批流程！！！！！！！！ -->
    <!-- <ApprovalProcess :table-data="approvalData" /> -->
  </div>
</template>

<script>
// import collapse from './component/collapse.vue'
// import addDetail from './component/addDetail.vue'
import TagsView from '../../../layout/components/TagsView/index.vue'
import BaseInfo from './component/baseInfo.vue'
import MaterialDetail from './component/materialDetail'
import { getDetail, saveOrUpdatePlan } from '@/api/purchase/index'
// import ApprovalProcess from './component/approvalProcess.vue'
export default {
  components: { TagsView, BaseInfo, MaterialDetail },
  beforeRouteEnter(to, form, next) {
    if (to.name === 'editPlan') {
      next(vm => {
        vm.$refs.baseInfoRef.clearValidate()
      })
    }
  },
  data() {
    return {
      isActive: false,
      activeNames: ['basicInfo', 'detail'],
      tableData: [],
      submitFormData: [],
      approvalData: [{
        code: 'CG45464984',
        actionTime: '2022-04-29 11:27:23',
        nodeName: '研发部门',
        author: '王花户',
        post: '管理者',
        appResule: '通过',
        advice: '需修改XXX',
        action: '打印'
      }, {
        code: 'CG45464984',
        actionTime: '2022-04-29 11:27:23',
        nodeName: '研发部门',
        author: '王花户',
        post: '管理者',
        appResule: '通过',
        advice: '需修改XXX',
        action: '打印'
      }, {
        code: 'CG45464984',
        actionTime: '2022-04-29 11:27:23',
        nodeName: '研发部门',
        author: '王花户',
        post: '管理者',
        appResule: '通过',
        advice: '需修改XXX',
        action: '打印'
      }, {
        code: 'CG45464984',
        actionTime: '2022-04-29 11:27:23',
        nodeName: '研发部门',
        author: '王花户',
        post: '管理者',
        appResule: '通过',
        advice: '需修改XXX',
        action: '打印'
      }],
      createTime: '',
      baseInfo: {},
      id: this.$route.query.id,
      delDetailIdList: [],
      loading: false
    }
  },
  created() {
    this.getDetail()
  },
  // activated() {
  //   this.getDetail()
  // },
  methods: {
    getDetail() {
      getDetail(this.$route.query.id).then(res => {
        this.createTime = res.data.createTime
        this.baseInfo = {
          title: res.data.title,
          orderNo: res.data.orderNo,
          expectedCompletionDate: res.data.expectedCompletionDate
        }
        this.tableData = res.data.detailVOs

        this.tableData.forEach((item, index) => {
          // 资产类别
          const res = item.materialCategory.lastIndexOf('/')
          item.materialCategory = item.materialCategory.substring(res + 1, item.materialCategory.length)
          const obj = {
            materialId: item.materialId,
            expectedCompletionDate: item.expectedCompletionDate,
            planQty: item.planQty,
            supplierName: item.supplierName,
            remark: item.remark,
            fileList: item.fileList,
            id: item.id,
            materialCategory: item.materialCategory
          }

          this.submitFormData.push(obj)
        })
      })
    },
    submitData(data, deleteIds) {
      this.submitFormData = data
      if (deleteIds?.length > 0) {
        this.delDetailIdList = deleteIds
      }
    },
    submit() {
      if (this.submitFormData.length === 0) {
        this.$message.error('至少添加一条采购计划明细')
        return
      }
      this.loading = true
      const title = this.$refs.baseInfoRef.baseInfo.title
      const expectedCompletionDate = this.$refs.baseInfoRef.baseInfo.expectedCompletionDate
      saveOrUpdatePlan({ title: title, expectedCompletionDate: expectedCompletionDate, id: Number(this.$route.query.id), details: this.submitFormData, delDetailIdList: this.delDetailIdList }).then(res => {
        if (res.code === 200) {
          this.loading = false
          this.$message.success('保存成功')
          // this.$router.go(-1)
          this.$router.push({ path: '/purchase/plan', query: { mark: this.$route.query.mark }})
        }
      })
      this.loading = false
    },
    goBack() {
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
.add_plan{
  height: calc(100vh - 164px);
  overflow-y:auto;
  min-height:0;
  margin:16px
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
