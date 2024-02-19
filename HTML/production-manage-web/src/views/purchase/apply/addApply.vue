<template>
  <div>
    <tags-view>
      <div class="addShop_head">
        <div class="head_left">
          <!-- <p>新建采购申请页面</p> -->
        </div>
        <div>
          <el-button v-debounce size="small" type="primary" :loading="loadflag" @click="addAndSave()">暂存</el-button>
          <el-button v-debounce size="small" type="primary" :loading="loadflag" @click="addAndSave('1',true)">保存并新建</el-button>
          <el-button v-debounce size="small" type="primary" :loading="loadflag" @click="addAndSave('1')">保存</el-button>
          <el-button size="small" @click="closeApply">返回</el-button>
        </div>
      </div>
    </tags-view>
    <el-card class="add_apply">
      <collapse title="基本信息">
        <baseInfo ref="baseInfo" :base-infos="baseInfos" />
      </collapse>
      <materialDetail ref="materialDetail" :table-datas="tableDatas" :is-detail="'0'" />
    </el-card>

  </div>
</template>

<script>
import collapse from '../plan/component/collapse.vue'
import baseInfo from './component/baseInfo.vue'
import materialDetail from './component/materialDetail.vue'
import TagsView from '../../../layout/components/TagsView/index.vue'
import { saveOrUpdateApply } from '@/api/purchase/index'
// import MaterialDetail from '../plan/component/materialDetail'
export default {
  components: { collapse, baseInfo, materialDetail, TagsView },
  beforeRouteEnter(to, form, next) {
    if (to.name === 'addApply') {
      next(vm => {
        vm.$refs.materialDetail.tableData = []
        vm.$refs.baseInfo.basicForm = {
          title: '', // 标题
          orderNo: '', // 采购单编号
          procurementLeader: '', // 采购负责人
          expectedArrivalDate: '', // 预期到货时间
          supplierId: '', // 供应商
          supplierTelPhone: '', // 供应商电话
          enquiryComments: '', // 采购询价意见
          purchaseDescription: '' // 采购说明
        }
        // 负责人数组
        vm.$refs.baseInfo.leaderArr = []
        vm.$refs.baseInfo.supplierName = []
        vm.$refs.baseInfo.isTitleWarn = false
        vm.$refs.baseInfo.isPeopleWarn = false
        vm.$refs.baseInfo.isSupplierWarn = false
        vm.$refs.baseInfo.isPhoneWarn = false
        // 每次进入页面清除校验
        vm.$refs.baseInfo.clearValidate()
        // vm.$store.dispatch('getNo', 'CG').then((res) => {
        //   vm.$refs.baseInfo.basicForm.orderNo = res.data
        // })
      })
    }
  },
  data() {
    return {
      loadflag: false, // 按钮点击后不可重复点击
      activeNames: ['basicInfo', 'detail'],
      baseInfos: {},
      tableDatas: []

    }
  },
  mounted() {

  },
  methods: {
    addAndSave(status, isNew) {
      this.loadflag = true
      // console.log(this.$refs.baseInfo, 'baseInfo')
      this.$refs.baseInfo.validateForm().then(da => {
        if (!da) {
          this.loadflag = false
          return
        }
        // const isEmpty = this.$refs.materialDetail.tableData.some(item => { return !item.unitPrice || item.unitPrice === '0' })
        // if (isEmpty) {
        //   this.$message.error('请填写单价')
        //   return
        // }
        this.$refs['materialDetail'].$refs['materialForm'].validate((valid) => {
          if (valid) {
            if (this.$refs.materialDetail.tableData.length > 0) {
              const basicForm = this.$refs.baseInfo.basicForm
              const materialDetail = this.$refs.materialDetail.tableData
              const details = []
              const urgencyDegree = this.$refs.materialDetail.urgencyDegree
              materialDetail.map((e) => {
                const obj = {
                // id: e.id, // 新增时不传
                  mainId: e.mainId || this.tableDatas[0]?.mainId,
                  materialId: e.materialId,
                  fileList: e.fileList,
                  orderRelationIds: e.idArr,
                  purchaseQty: e.planQty,
                  remark: e.remark,
                  totalPrice: (e.totalPrice * 100).toFixed(0),
                  unitPrice: (e.unitPrice * 100).toFixed(0)

                }
                details.push(obj)
              })
              // console.log(details, 'details')

              // this.$store.dispatch('getNo', 'CG').then((res) => {
              // this.$refs.baseInfo.basicForm.orderNo = res.data
              const data = {
                ...basicForm,
                details,
                urgencyDegree,
                status: status || ''
              }
              saveOrUpdateApply(data).then(res => {
                if (res.code === 200) {
                  this.$modal.msgSuccess('保存成功')
                  // 保存并新建重置表单数据
                  if (isNew) {
                    // console.log(isNew, 'isnew')
                    this.$refs.baseInfo.clearValidate()
                    this.$refs.baseInfo.basicForm = {}
                    // 清空负责人
                    this.$refs.baseInfo.leaderArr = []
                    // 清空供应商
                    this.$refs.baseInfo.supplierName = []
                    this.$refs.baseInfo.supplierName = []
                    this.$refs.materialDetail.tableData = []
                    this.$refs.materialDetail.urgencyDegree = '0'
                  } else {
                    if (status) {
                      this.$router.push({ path: '/purchase/apply', query: { mark: this.$route.query.mark }})
                    } else {
                      this.$router.push({ path: '/purchase/apply', query: { mark: 2 }})
                    }
                  }
                }
              })
              // })
            } else {
              this.loadflag = false
              this.$message.error('请添加采购明细')
            }
          } else {
            this.loadflag = false
          }
        })
      })

      // 对表单进行校验
    },
    closeApply() {
      // this.$router.go(-1)
      this.$router.push({ path: '/purchase/apply', query: { mark: this.$route.query.mark }})
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
.add_apply{
  height: calc(100vh - 164px);
  overflow-y:auto;
  min-height:0;
margin: 16px;
}
.redDot{
    color: #EF5826;
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
