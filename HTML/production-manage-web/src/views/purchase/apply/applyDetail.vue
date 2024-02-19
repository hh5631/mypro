<template>
  <div>
    <tags-view>
      <div v-show="this.$route.name==='applyDetail'" class="addShop_head">
        <div class="head_left">
          <span>{{ isDetail==='1'?'采购申请-详情':'采购申请-编辑' }}</span><span class="head-time">创建时间：{{ baseInfos.createTime }}</span>
        </div>
        <div>
          <el-button v-if="isDetail==='0'" v-debounce size="small" type="primary" @click.native="addAndSave()">暂存</el-button>
          <el-button v-if="isDetail==='0'" v-debounce size="small" type="primary" @click.native="addAndSave('1',true)">保存并新建</el-button>
          <el-button v-if="isDetail==='0'" v-debounce size="small" type="primary" @click.native="addAndSave('1')">保存</el-button>
          <el-button v-debounce size="small" @click.native="closeApply">返回</el-button>
        </div>
      </div>
    </tags-view>
    <el-card class="add_apply">
      <collapse title="基本信息">
        <baseInfo ref="baseInfo" :base-infos="baseInfos" :is-detail="isDetail" />
      </collapse>
      <materialDetail ref="materialDetail" :table-datas="tableDatas" :is-detail="isDetail" :mark="mark" style="padding-bottom: 8px;" />
      <collapse v-if="isDetail==='1'" title="审批流程">
        <ApprovalProcess :id="baseInfos.id" ref="approvalProcess" />
      </collapse>
    </el-card>

  </div>
</template>

<script>
import collapse from '../plan/component/collapse.vue'
import baseInfo from './component/baseInfo.vue'
import materialDetail from './component/materialDetail.vue'
import TagsView from '../../../layout/components/TagsView/index.vue'
import { saveOrUpdateApply, getApplyDetail } from '@/api/purchase/index'
import ApprovalProcess from './component/approvalProcess.vue'
export default {
  components: { collapse, baseInfo, materialDetail, TagsView, ApprovalProcess },
  beforeRouteEnter(to, form, next) {
    if (to.name === 'applyDetail') {
      next(vm => {
        vm.getDetail()
      })
    }
  },
  data() {
    return {
      isDetail: '0',
      activeNames: ['basicInfo', 'detail'],
      baseInfos: {},
      tableDatas: [],
      mark: ''
    }
  },
  watch: {
    '$route.query': {
      handler(val) {
        this.isDetail = val.isDetail || '0'
        this.mark = val.mark || '0'
      },
      immediate: true
    }
  },
  mounted() {
  },
  methods: {
    getDetail(id) {
      getApplyDetail(this.$route.query.id || id).then(res => {
        if (res.code === 200) {
          this.baseInfos = res.data
          this.tableDatas = res.data.detailVOS
          this.$refs.materialDetail.urgencyDegree = res.data.urgencyDegree
          if (this.isDetail === '1') {
            this.$refs.approvalProcess.processInstanceId = res.data.processInstanceId
            this.$refs.approvalProcess.getList()
            // this.$refs.approvalProcess.activeChange(0)
          }
        }
      })
    },
    async addAndSave(status, isNew) {
      const da = await this.$refs.baseInfo.validateForm()
      if (!da) {
        return
      }
      const basicForm = this.$refs.baseInfo.basicForm
      const materialDetail = this.$refs.materialDetail.tableData
      const details = []
      const urgencyDegree = this.$refs.materialDetail.urgencyDegree
      materialDetail.map((e) => {
        const obj = {
          // id: e.id || '',    //后端让删掉这个id
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
      const data = {
        ...basicForm,
        details,
        delDetailIdList: this.$refs.materialDetail.delIds,
        urgencyDegree,
        status: status || ''
      }
      saveOrUpdateApply(data).then(res => {
        if (res.code === 200) {
          this.$modal.msgSuccess('保存成功')
          // 保存并新建重置表单数据
          if (isNew) {
            this.$router.push({ path: '/addApply' })
          } else if (!status) {
            // 暂存返回列表时要定位在草稿栏
            this.$router.push({ path: '/purchase/apply', query: { mark: 2 }})
          } else {
            this.$router.go(-1)
          }
        }
      })
    },
    closeApply() {
      // this.$router.go(-1)
      // this.$router.push({ path: '/purchase/apply' })
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
  .head-time{
    font-size: 16px;
    font-weight: 400;
    color: #7D8592;
    line-height: 24px;
    margin-left: 20px;
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
