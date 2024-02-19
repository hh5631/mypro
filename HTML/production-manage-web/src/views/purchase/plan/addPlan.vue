<template>
  <div>
    <!-- header -->
    <tags-view>
      <div class="addShop_head">
        <div class="head_left">
          <!-- <p>新建采购计划</p> -->
        </div>
        <div>
          <!-- <el-button size="small" type="primary">暂存</el-button> -->
          <el-button size="small" type="primary" @click="save(1)">保存并新建</el-button>
          <el-button size="small" type="primary" @click="save(2)">保存</el-button>
          <el-button size="small" @click="goBack">返回</el-button>
        </div>
      </div>
    </tags-view>
    <!-- content -->
    <el-card class="main-card">
      <div class="addShop_content">
        <BaseInfo ref="baseInfoRef" :is-title-warn="isTitleWarn" :is-expect-date-warn="isExpectDateWarn" />
        <MaterialDetail ref="materialDetail" :table-datas="tableData" :is-detail="'0'" @submitData="submitData" />
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
import { saveOrUpdatePlan } from '@/api/purchase/index'
// import { Loading } from 'element-ui'
// import ApprovalProcess from './component/approvalProcess.vue'
export default {
  components: { TagsView, BaseInfo, MaterialDetail },
  beforeRouteEnter(to, form, next) {
    if (to.name === 'addPlan') {
      next(vm => {
        vm.$refs.materialDetail.tableData = []
        vm.$refs.baseInfoRef.clearValidate()
        // vm.$store.dispatch('getNo', 'JH').then((res) => {
        //   vm.$refs.baseInfoRef.baseInfo.orderNo = res.data
        // })
      })
    }
  },
  data() {
    return {
      isActive: false,
      activeNames: ['basicInfo', 'detail'],
      // 物料明细数据
      tableData: [],
      // approvalData: [{
      //   code: 'CG45464984',
      //   actionTime: '2022-04-29 11:27:23',
      //   nodeName: '研发部门',
      //   author: '王花户',
      //   post: '管理者',
      //   appResule: '通过',
      //   advice: '需修改XXX',
      //   action: '打印'
      // }, {
      //   code: 'CG45464984',
      //   actionTime: '2022-04-29 11:27:23',
      //   nodeName: '研发部门',
      //   author: '王花户',
      //   post: '管理者',
      //   appResule: '通过',
      //   advice: '需修改XXX',
      //   action: '打印'
      // }, {
      //   code: 'CG45464984',
      //   actionTime: '2022-04-29 11:27:23',
      //   nodeName: '研发部门',
      //   author: '王花户',
      //   post: '管理者',
      //   appResule: '通过',
      //   advice: '需修改XXX',
      //   action: '打印'
      // }, {
      //   code: 'CG45464984',
      //   actionTime: '2022-04-29 11:27:23',
      //   nodeName: '研发部门',
      //   author: '王花户',
      //   post: '管理者',
      //   appResule: '通过',
      //   advice: '需修改XXX',
      //   action: '打印'
      // }],
      // 表单标题是否验证
      isTitleWarn: false,
      // 预期完成事件是否验证
      isExpectDateWarn: false
    }
  },
  methods: {
    /** 监听子组件选择物料明细事件 */
    submitData(data) {
      // data.forEach(item => {
      //   const obj = {
      //     materialId: item.materialId,
      //     expectedCompletionDate: item.expectedCompletionDate,
      //     planQty: item.planQty,
      //     supplierName: item.supplierName,
      //     remark: item.remark,
      //     fileList: item.fileList,
      //     id: item.id
      //   }
      //   this.tableData.push(obj)
      // })
      this.tableData = data
    },
    // saveAndCreate() {
    //   const title = this.$refs.baseInfoRef.baseInfo.title
    //   const expectedCompletionDate = this.$refs.baseInfoRef.baseInfo.expectedCompletionDate
    // },
    /** 提交新建表单 */

    save(index) {
      // 执行
      let loading
      // 暴力获取标题和预期完成时间并判断
      const title = this.$refs.baseInfoRef.baseInfo.title
      // const orderNo = this.$refs.baseInfoRef.baseInfo.orderNo
      const expectedCompletionDate = this.$refs.baseInfoRef.baseInfo.expectedCompletionDate
      if (title === '') {
        this.isTitleWarn = true
        return
      }
      this.isTitleWarn = false
      if (expectedCompletionDate === '') {
        this.isExpectDateWarn = true
        return
      }
      if (this.tableData.length === 0) {
        this.$message.error('至少添加一条采购计划明细')
        return
      }
      if (title !== '' && expectedCompletionDate !== '') {
        // 都不为空 加载动画播放
        loading = this.$loading({
          lock: true,
          text: '正在保存中,请稍等片刻',
          spinner: 'el-icon-loading',
          background: 'rgba(0, 0, 0, 0.7)'
        })
      }
      this.isExpectDateWarn = false
      // 组件提交数据
      const submitData = this.tableData.map(item => {
        return {
          materialId: item.materialId,
          expectedCompletionDate: item.expectedCompletionDate,
          planQty: item.planQty,
          supplierName: item.supplierName,
          remark: item.remark,
          fileList: item.fileList,
          id: item.id
        }
      })

      // this.$store.dispatch('getNo', 'JH').then((res) => {
      // this.$refs.baseInfoRef.baseInfo.orderNo = res.data
      saveOrUpdatePlan({ title: title, orderNo: this.$refs.baseInfoRef.baseInfo.orderNo, expectedCompletionDate: expectedCompletionDate, details: submitData }).then(res => {
        if (res.code === 200) {
          this.$message.success('保存成功')
          this.$refs.baseInfoRef.baseInfo.title = ''
          this.$refs.baseInfoRef.baseInfo.expectedCompletionDate = ''
          this.tableData = []
          this.$refs.baseInfoRef.clearValidate()
          if (index === 2) {
            // this.$router.go(-1)
            this.$router.push({ path: '/purchase/plan', query: { mark: this.$route.query.mark }})
          }
          loading.close()
        } else {
          loading.close()
        }
      })
      // })
      setTimeout(() => {
        loading.close()
      }, 1000)
    },
    /** 返回 */
    goBack() {
      this.isTitleWarn = false
      this.isExpectDateWarn = false
      this.$nextTick(() => {
        this.$refs.baseInfoRef.clearValidate()
      })
      this.$refs.baseInfoRef.baseInfo.title = ''
      this.$refs.baseInfoRef.baseInfo.expectedCompletionDate = ''
      this.tableData = []
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
