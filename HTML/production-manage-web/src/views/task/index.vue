<template>
  <CommonTitleBar class="task-box">
    <el-card class="main-card">
      <div class="app-container">
        <el-radio-group v-model="activeName" size="small" class="radio-box">
          <el-radio-button v-for="(page, index) in tabList" :key="index" :label="page" />
        </el-radio-group>
        <component
          :is="componentId"
          v-loading="loading"
          :data="tableData"
          @handle="openHandleDialog"
          @lookProcess="lookProcess"
          @withdraw="withDraw"
          @tovoid="toVoid"
          @reload="getList"
        />
        <div class="common-pagination">
          <pagination
            v-show="total > 0"
            :total="total"
            :page.sync="listQuery.pageNum"
            :limit.sync="listQuery.pageSize"
            @pagination="getList"
          />
        </div>
        <el-dialog
          v-if="dialogFormVisible"
          :title="title"
          width="1200px"
          :visible.sync="dialogFormVisible"
          class="task-dialog"
        >
          <!-- <DetailForm v-if="title === '详情'" /> -->
          <el-tabs v-model="activeTabs" tab-position="top" class="tabs-dialog">
            <el-tab-pane
              v-if="componentId == 'TaskUpComing' && !dialogFormData.taskType"
              label="任务处理"
              name="任务处理"
              style="height: 100%"
            >
              <el-form

                ref="dealForm"
                :model="dealForm"
                label-width="80px"
              >
                <v-form-render
                  ref="nodeForm"
                  :form-json="nodeFormJson"
                />
                <div v-if="!componetType">
                  <el-form-item label="处理意见" prop="message">
                    <el-input v-model="dealForm.message" type="textarea" rows="4" />
                  </el-form-item>
                  <el-form-item v-if="isGateway" class="btn-box">
                    <el-button
                      v-show="isGateway == 0"
                      type="primary"
                      size="small"
                      @click="dealFun('dealForm')"
                    >保存</el-button>
                    <el-button
                      v-show="isGateway == 1"
                      size="small"
                      @click="dealFun('dealForm',0)"
                    >拒绝</el-button>
                    <el-button
                      v-show="isGateway == 1"
                      type="primary"
                      size="small"
                      @click="dealFun('dealForm',1)"
                    >同意</el-button>
                  </el-form-item>
                </div>
              </el-form>
              <div v-if="componetType" class="task-detail">
                <div class="left-class">
                  <div>
                    <!-- 采购明细 -->
                    <purchaseTable v-if="componetType === 'type_po'" :detail-data="detailData" />
                    <!-- 领用明细 -->
                    <assetCollectionTable v-if="componetType==='type_ware_get'" :detail-data="detailData" />
                    <!-- 退库明细 -->
                    <warabackTable v-if="componetType==='type_ware_back'" :detail-data="detailData" />
                    <!-- 处置明细 -->
                    <disposalTable v-if="componetType==='type_asset_handle'" :detail-data="detailData" />
                  </div>
                  <div v-if="componetType === 'type_po'" class="totalDiv">
                    <div class="totalDiv-item1">
                      <div class="total_title">合计(元)</div>
                      <div class="total_count">{{ allTotalPrice||'' }}</div>
                    </div>
                    <div class="totalDiv-item2">
                      <div class="total_title">大写</div>
                      <div class="total_count">{{ allTotalPrice? dealMoney(allTotalPrice):'' }}</div>
                    </div>
                  </div>

                </div>
                <div class="right-class">
                  <el-form
                    ref="dealForm"
                    :model="dealForm"
                    label-width="80px"
                    class="demo-ruleForm"
                    label-position="top"
                  >
                    <!-- <v-form-render
                      ref="nodeForm"
                      :form-json="nodeFormJson"
                    /> -->
                    <div class="form-orderNo">
                      <img src="@/assets/images/order.png" alt="" width="18px" height="20px">
                      <div class="orderNo">{{ dealForm.orderNo }}</div>
                      <!-- <div class="serialNo">(流水号：{{ dealForm.detailNo }})</div> -->
                    </div>
                    <div class="form-item">
                      <div class="item-label">申请部门：</div>
                      <div class="item-text">{{ dealForm.dept }}</div>
                    </div>
                    <div class="form-item">
                      <div class="item-label">申请人：</div>
                      <div class="item-text">{{ dealForm.people }}</div>
                    </div>
                    <div class="form-item">
                      <div class="item-label">申请时间：</div>
                      <div class="item-text">{{ dealForm.time }}</div>
                    </div>
                    <div v-if="componetType==='type_ware_get'" class="form-item">
                      <div class="item-label">领用说明：</div>
                      <div class="item-text">{{ dealForm.note }}</div>
                    </div>
                    <div class="form-item">
                      <div class="item-label">紧急程度：</div>
                      <div class="item-text" :class="{urgency:dealForm.urgency==='1' || dealForm.urgency === 1}">
                        {{ filterUrgency(dealForm) }}</div>
                    </div>
                    <div v-if="componetType==='type_asset_handle'" class="form-item">
                      <div class="item-label">处置类型：</div>
                      <div class="item-text">{{ getDisposalType(dealForm.type) }}</div>
                    </div>
                    <div class="form-divider" />
                    <div class="form-item">
                      <div class="item-label">审批意见：</div>
                    </div>
                    <el-form-item prop="message">
                      <el-input v-model="dealForm.message" maxlength="500" show-word-limit type="textarea" rows="4" />
                    </el-form-item>
                  </el-form>
                  <div class="btn-box1">
                    <el-button
                      v-show="isGateway === 0"
                      type="primary"
                      size="small"
                      @click="dealFun('dealForm')"
                    >保存</el-button>
                    <el-button
                      v-show="isGateway === 1"
                      size="small"
                      plain
                      class="reject"
                      @click="dealFun('dealForm',0)"
                    >拒绝</el-button>
                    <el-button
                      v-show="isGateway === 1"
                      type="primary"
                      size="small"
                      @click="dealFun('dealForm',1)"
                    >同意</el-button>
                  </div>
                </div>
              </div>
            </el-tab-pane>
            <el-tab-pane label="任务流程" name="任务流程" style="height: 100%">
              <process-designer
                ref="processDesigner"
                class="process-detail"
              />
            </el-tab-pane>
            <el-tab-pane label="流转记录" name="流转记录" style="height: 100%">
              <el-timeline>
                <el-timeline-item
                  v-for="(item, index) in flowRecordData"
                  :key="index"
                  :timestamp="item.taskName"
                  placement="top"
                >
                  <div>
                    <p>
                      {{ item.result === '2'?item.startTime: item.endTime }}
                      <el-divider direction="vertical" />
                      审批人:{{ item.assigneeName }}
                    </p>
                    <p>审批结果:{{ item.result === '1' ? "通过" : (item.result === '0')?"拒绝":'待办' }}</p>
                    <p>审批意见:{{ item.message }}</p>
                    <p>节点单据:</p>
                    <div class="node-record-form">
                      <div class="node-record-form-render">
                        <v-form-render
                          :form-json="item.formJson?JSON.parse(item.formJson):{}"
                          :form-data="item.formDataJson?JSON.parse(item.formDataJson):{}"
                        />
                      </div>
                      <div class="node-record-form-after" />
                    </div>
                  </div>
                </el-timeline-item>
              </el-timeline>
            </el-tab-pane>
          </el-tabs>
        </el-dialog>
        <el-dialog title="查看进程" :visible.sync="dialogProcessVisible">
          <process-designer
            ref="processDialogDesigner"
            class="process-detail"
          />
        </el-dialog>
      </div>
    </el-card>
  </CommonTitleBar>

</template>
<script>
import {
  getMyTask,
  getTaskHistory,
  getMyInitiatedTasks,
  getHighLine,
  getFormDetail,
  getFlowRecord,
  withdrawTask,
  toVoidTask,
  updateFlowData,
  dealProcess
} from '@/api/process'
import ProcessDesigner from '../../bpmn/designer/ProcessDesignerPreview.vue'
import TaskUpComing from './components/upcoming.vue'
import TaskInitiated from './components/initiated.vue'
import TaskDone from './components/done.vue'
import ProcessForm from '@/components/Process/Form.vue'
import CommonTitleBar from '../../components/common/common-title-bar.vue'
import DetailForm from './components/detail-form.vue'
import { getApplyDetail } from '@/api/purchase/index'
import { getDetailsById, getDetailById } from '@/api/assetmanagement/assetCollection.js'
import editFile from '@/views/purchase/apply/component/editFile.vue'
import { dealBigMoney } from '@/utils/index.js'
import purchaseTable from './components/purchase-table.vue'
import assetCollectionTable from './components/assetCollection-table.vue'
import warabackTable from './components/waraback-table.vue'
import disposalTable from './components/disposal-table.vue'
import { getWareBackDetailsById } from '@/api/assetmanagement/assetWithdrawal.js'
import { getAssetHandleById } from '@/api/assetmanagement/assetDisposal.js'
export default {
  components: { TaskUpComing, TaskDone, TaskInitiated, ProcessForm, CommonTitleBar, DetailForm, ProcessDesigner, editFile, purchaseTable, assetCollectionTable, warabackTable, disposalTable },
  provide() {
    return {
      setNextKey: this.getNextKey
    }
  },
  beforeRouteEnter(to, from, next) {
    // console.log(to, 'to')
    if (to.name === 'Task') {
      next(vm => {
        vm.getList()
      })
    }
  },
  data() {
    return {
      activeTabs: '任务处理',
      tabList: ['待办任务', '已办任务', '我发起的'],
      activeName: '待办任务',
      listQuery: {
        pageSize: 10,
        pageNum: 1
      },
      tableData: [],
      total: 0,
      componentId: 'TaskUpComing',
      dialogFormData: {},
      dialogFormVisible: false,
      dialogProcessVisible: false,
      formJson: {},
      formData: {},
      nodeFormJson: {},
      xmlString: '',
      dealForm: {
        orderNo: '', // 订单号
        dept: '', // 申请部门
        people: '', // 申请人
        time: '', // 申请时间
        urgency: '', // 紧急程度
        message: '', // 审批意见
        note: ''
      },
      title: '',
      isGateway: '', // 条件流转0否1是
      loading: false,
      flowRecordData: [], // 流转记录信息
      componetType: '', // 业务流程表单名称
      bizOrderNo: '', // 业务单号
      detailData: [],
      allTotalPrice: [], // 总合计,
      fileDig: false,
      fileList: []
    }
  },
  watch: {
    activeTabs(nv) {
      if (nv === '任务流程' && this.componentId === 'TaskUpComing' && !this.dialogFormData.taskType) {
        setTimeout(() => {
          this.$refs.processDesigner.refit()
        }, 200)
      }
    },
    activeName() {
      this.tableData = []
      this.total = 0
      this.listQuery.pageNum = 1
      this.getList()
    },
    'detailData': {
      deep: true,
      handler: function (newVal, oldVal) {
        let num = 0
        newVal.map((e) => {
          num += Number(e.totalPrice)
        })
        this.allTotalPrice = num
      }
    }
  },
  created() {
    this.getList()
  },
  destroyed() {
    this.dialogFormVisible = false
    this.dialogProcessVisible = false
  },
  // deactivated() {
  //   this.dialogFormVisible = false
  //   this.dialogProcessVisible = false
  // },
  methods: {
    getList() {
      switch (this.activeName) {
        case '待办任务':
          this.title = '办理业务'
          this.componentId = 'TaskUpComing'
          this.getMyTask()
          break
        case '已办任务':
          this.title = '详情'
          this.componentId = 'TaskDone'
          this.getTaskHistory()
          break
        default:
          this.title = '详情'
          this.componentId = 'TaskInitiated'
          this.getMyInitiatedTasks()
          break
      }
    },
    // 待办任务列表
    getMyTask() {
      this.loading = true
      getMyTask(this.listQuery)
        .then((da) => {
          this.tableData = da.data.records
          this.total = da.data.total
        })
        .finally(() => {
          this.loading = false
        })
    },
    // 已完成任务查询
    getTaskHistory() {
      this.loading = true
      getTaskHistory(this.listQuery)
        .then((da) => {
          this.tableData = da.data.records
          this.total = da.data.total
        })
        .finally(() => {
          this.loading = false
        })
    },
    // 我发起的列表
    getMyInitiatedTasks() {
      this.loading = true
      getMyInitiatedTasks(this.listQuery)
        .then((da) => {
          this.tableData = da.data.records
          this.total = da.data.total
        })
        .finally(() => {
          this.loading = false
        })
    },
    getDetail(id) {
      const api = this.getComponentType(this.componetType)
      api(id).then(res => {
        if (res.code === 200) {
          // 采购
          if (this.componetType === 'type_po') {
            // 采购申请
            this.dealForm.orderNo = res.data.orderNo
            this.dealForm.urgency = res.data.urgencyDegree
            this.dealForm.time = res.data.createTime
            this.dealForm.people = res.data.creator
            this.dealForm.note = res.data.note
            this.dealForm.dept = res.data.procurementDepartment

            this.detailData = res.data.detailVOS.map((e) => {
              const obj = {}
              obj.id = e.id
              obj.mainId = e.mainId
              obj.materialId = e.materialId
              obj.orderNoArr = e.relationPlanNoList
              obj.idArr = e.orderRelationIds
              obj.detailNo = e.detailNo
              obj.materialCategory = e.material.categoryNameLevel4 || e.material.categoryNameLevel3 || e.material.categoryNameLevel2 || e.material.categoryNameLevel1
              obj.materialName = e.material.name
              obj.brandName = e.material.brand
              obj.materialStandard = e.material.standard
              obj.unitName = e.material.unit
              obj.planQty = e.purchaseQty
              obj.unitPrice = e.unitPrice / 100
              obj.totalPrice = e.totalPrice / 100
              obj.remark = e.remark
              obj.fileList = e.fileList
              return obj
            })
          }
          // 领用明细
          if (this.componetType === 'type_ware_get') {
            // 退库业务
            getDetailById(id).then((res) => {
              if (res.code === 200) {
                this.dealForm.orderNo = res.data.oddNum
                this.dealForm.urgency = res.data.urgency
                this.dealForm.time = res.data.getTime
                this.dealForm.people = res.data.peopleName
                this.dealForm.dept = res.data.deptName
                this.dealForm.note = res.data.note
              }
            })
            // 领用业务
            this.detailData = res.data
          }
          // 退库明细
          if (this.componetType === 'type_ware_back') {
            this.dealForm.orderNo = res.data.orderNo
            this.dealForm.time = res.data.createTime
            this.dealForm.people = res.data.userName
            this.dealForm.dept = res.data.backDept
            this.detailData = res.data.wareBackDetails || []
          }
          // 处置
          if (this.componetType === 'type_asset_handle') {
            this.dealForm.orderNo = res.data.orderNo
            this.dealForm.urgency = res.data.urgency
            this.dealForm.time = res.data.handleDate
            this.dealForm.people = res.data.applicantName
            this.dealForm.type = res.data.type
            this.dealForm.note = res.data.note
            this.dealForm.dept = res.data.applicantDept
            this.detailData = res.data.handleDetailList || []
          }
        }
      })
    },
    // 办理业务弹窗
    async openHandleDialog(data) {
      this.isGateway = ''
      this.detailData = []
      this.nodeFormJson = {}
      this.dealForm = {
        orderNo: '', // 订单号
        dept: '', // 申请部门
        people: '', // 申请人
        time: '', // 申请时间
        urgency: '', // 紧急程度
        message: '' // 审批意见
      }
      this.dialogFormVisible = true
      if (this.componentId === 'TaskUpComing' && !data.taskType) {
        this.activeTabs = '任务处理'
      } else {
        this.activeTabs = '任务流程'
      }
      //
      // 判断是否是业务表单
      if (data.type > 0) {
        this.componetType = data.bizType
        this.bizOrderNo = data.businessKey// 'type_po'采购业务type_ware_get领用业务
        this.getDetail(this.bizOrderNo)
      } else {
        this.componetType = ''
        // 获取表单详情
        const da = await getFormDetail(data.formId)
        this.formJson = JSON.parse(da.data.formJson || '{}')
        this.formData = JSON.parse(data.formDataJson || '{}')
        if (!data.taskType) {
          await this.$nextTick()
          this.$refs.vFormRef && this.$refs.vFormRef.disableForm()
        }
        // 获取节点表单详情
        if (data.nodeFormId) {
          const da = await getFormDetail(data.nodeFormId)
          this.nodeFormJson = JSON.parse(da.data.formJson || '')
        }
      }
      this.dialogFormData = this.$deepClone(data)
      // 获取流转记录
      // if (this.componentId === 'TaskInitiated') {
      const da = await getFlowRecord(data.processInstanceId)
      this.flowRecordData = da.data.records.filter(item => {
        return item
      })
      // }
      const res = await getHighLine({ instanceId: data.processInstanceId })
      this.$refs.processDesigner.initDiagram(data, res.data, !(this.componentId === 'TaskUpComing' && !this.dialogFormData.taskType))
      //
    },
    beforeDialogClose(done) {
      done()
    },
    // 查看进程弹窗
    async lookProcess(data) {
      this.dialogProcessVisible = true
      const da = await getHighLine({ instanceId: data.processInstanceId })
      this.$refs.processDialogDesigner.initDiagram(data, da.data)
    },
    // 处理任务
    dealFun(formName, result) {
      this.$refs.nodeForm.getFormData().then(formData => {
        const data = {
          taskId: this.dialogFormData.taskId,
          result: result === 0 ? 0 : 1,
          nodeFormId: this.dialogFormData.nodeFormId,
          nodeFormDataJson: JSON.stringify(formData)
        }
        this.$modal.loading('保存中，请稍后...')
        dealProcess({ ...data, ...this.dealForm }).then((da) => {
          this.$message.success(da.msg || '处理成功')
          this.getList()
        }).finally(() => {
          this.dialogFormVisible = false
          this.$modal.closeLoading()
        }).catch(() => {
          this.$modal.closeLoading()
        })
      })
    },
    // 重新保存
    reStart() {
      this.$refs.vFormRef
        .getFormData()
        .then((formData) => {
          //
          this.$confirm('确认重新保存吗?', '警告', {
            confirmButtonText: '确定',
            cancelButtonText: '取消',
            type: 'warning'
          }).then(() => {
            var data = {
              id: this.dialogFormData.launchDataId,
              formDataJson: JSON.stringify(formData)
            }
            this.$modal.loading('保存中，请稍后...')
            updateFlowData(this.dialogFormData.taskId, data)
              .then((da) => {
                this.$message.success(da.msg)
                this.getList()
              }).finally(() => {
                this.dialogFormVisible = false
                this.$modal.closeLoading()
              })
              .catch(function() {
                this.$modal.closeLoading()
              })
          })
        })
        .catch((error) => {
          // Form Validation failed
          this.$message.error(error)
        })

      // .then(() => {
      //   this.getList();
      //   this.msgSuccess("转换成功");
      // })
      // .catch(function () {});
    },
    // 获取下一节点
    getNextKey(val) {
      if (val.indexOf('Gateway') > -1) {
        this.isGateway = 1
      } else {
        this.isGateway = 0
      }
      // console.log('this.isGateway', this.isGateway)
    },
    // 撤回任务
    withDraw(val) {
      const _that = this
      this.$confirm('确认撤回该任务?', '提示', {
        confirmButtonText: '确定',
        cancelButtonText: '取消',
        type: 'warning'
      }).then(() => {
        withdrawTask(val.processInstanceId).then((da) => {
          _that.$message.success('撤回成功')
          this.getList()
        })
      })
    },
    // 作废任务
    toVoid(val) {
      const _that = this
      this.$confirm('确认作废该任务?', '提示', {
        confirmButtonText: '确定',
        cancelButtonText: '取消',
        type: 'warning'
      }).then(() => {
        toVoidTask(val.id).then((da) => {
          _that.$message.success('作废成功')
          this.getList()
        })
      })
    },
    dealMoney(n) {
      return dealBigMoney(n)
    },
    /** 获取办理业务单类型 */
    getComponentType(type) {
      switch (type) {
        case 'type_po':
          return getApplyDetail
        case 'type_ware_get':
          return getDetailsById
        case 'type_ware_back':
          return getWareBackDetailsById
        case 'type_asset_handle':
          return getAssetHandleById
      }
    },
    /** 根据状态显示文本值 */
    getDisposalType(value) {
      const map = new Map([
        ['0', '变卖'],
        ['1', '报废'],
        ['2', '捐赠'],
        ['4', '盘亏'],
        ['100', '其它']
      ])
      return map.get(value)
    },
    filterUrgency(data) {
      if (data.urgency === 0 || data.urgency === '0') {
        return '一般'
      }
      if (data.urgency === 1 || data.urgency === '1') {
        return '紧急'
      }
    }

  }
}
</script>
<style lang="scss" scoped>
.task-box {
  .radio-box {
    margin-bottom: 16px;
  }
  .tabs-dialog {
    min-height: 490px;
    .el-form {
      border: 0;
    }
    .btn-box {
      text-align: right;
    }
  }
  .process-detail {
    height: 440px;
  }
}
.fix-card {
  .app-container {
    display: flex;
    height: 100%;
    flex-direction: column;
  }
}

.node-record-form{
  width: 50%;
  position: relative;
  &-render{
    // border: 1px solid red;
    position: relative;

  }
  &-after{
    position: absolute;
    top: 0;
    left: 0;
    content: '';
    background: rgba(255,255,255,0);
    width: 100%;
    height: 100%;
  }
}
.task-detail{
  display: flex;
  justify-content: space-between;
}
.task-dialog{
  ::v-deep .el-dialog__body{
    padding-bottom: 0px;
  }
}
.left-class{
  width:755px
}
.demo-ruleForm{
  padding: 0 20px 12px ;
}
.right-class{
  width: 390px;
  height: 500px;
  border: 1px solid #EAEDF4;
  border-top: none;
  position: relative;
  overflow: scroll;
  .btn-box1{
    width: 400px;
    text-align: right;
    border-top: 1px solid #EAEDF4;
    height: 56px;
    padding: 12px 20px;
    position: fixed;
    // margin-top: 20px;
    bottom: 0;
    background: #fff;
    z-index: 1;
  }
}
.reject{
  border-color: #EF5826;
  color: #EF5826;
}
.form-orderNo{
  display: flex;
  align-items: center;
  margin-bottom: 20px;
  .orderNo{
    font-size: 16px;
    font-family: Arial-BoldMT, Arial;
    font-weight: normal;
    color: #000000;
    line-height: 18px;
    margin: 0 8px;
  }
  .serialNo{
    font-size: 14px;
    font-family: SourceHanSansCN-Regular, SourceHanSansCN;
    font-weight: 400;
    color: #B2B5B9;
    line-height: 19px;
  }
}
.form-divider{
  width: 100%;
  height: 1px;
  background: #EAEDF4;
  margin-bottom: 20px;
}
.form-item{
  display: flex;
  align-items: center;
  margin-bottom: 20px;
  .item-label{
    font-size: 14px;
    font-family: SourceHanSansCN-Regular, SourceHanSansCN;
    font-weight: 400;
    color: #6A7697;
    line-height: 21px;
  }
  .item-text{
    font-size: 14px;
    font-family: SourceHanSansCN-Regular, SourceHanSansCN;
    font-weight: 400;
    color: #000000;
    line-height: 21px;
  }
  .urgency{
    color: #EF5826;
  }
}
.totalDiv {
  width: 100%;
  border: 1px solid #ddd;
  border-top: none;
  min-height: 48px;
  display: flex;
  justify-content: space-between;
  font-size: 14px;
  &-item1{
    width: 40%;
    display: flex;
  }
  &-item2{
    width: 60%;
    display: flex;
  }

  .total_title {
    width: 170px;
    background: #f1f2f5;
    height: 100%;
    text-align: center;
    line-height: 53px;
    border-right: 1px solid #ddd;
  }
  .total_unit {
    height: 100%;
    text-align: center;
    line-height: 53px;
    padding: 0 10px;
  }
  .total_count,
  .total_right {
    height: 100%;
    text-align: center;
    line-height: 53px;
    padding-left: 20px;
  }
}
</style>
