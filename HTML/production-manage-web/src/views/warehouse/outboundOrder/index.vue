<template>
  <div class="post-box">
    <tags-view>
      <el-row :gutter="10" class="mb8">
        <el-col :span="1.5">
          <el-button
            v-hasPermi="['warehousem:outboundOrder:export']"
            size="small"
            @click="handleExport"
          >
            <i class="iconfont icon-daochu" style="font-size: 14px;margin-right: 8px;" />
            <span class="btn-text">导出</span>
          </el-button>
        </el-col>
        <el-col :span="1.5">
          <el-button
            v-hasPermi="['warehousem:outboundOrder:print']"
            size="small"
            @click="handlePrint"
          >
            <i
              class="el-icon-printer"
              style="font-size: 14px; margin-right: 0px"
            />
            <span class="btn-text">打印</span>
          </el-button>
        </el-col>
        <el-col :span="1.5">
          <el-button
            v-hasPermi="['warehousem:outboundOrder:add']"
            type="primary"
            size="small"
            @click="handleAdd"
          >
            <i class="iconfont icon-zengjia" style="font-size: 14px;margin-right: 8px;" />
            <span class="btn-text">新增出库</span>
          </el-button>
        </el-col>
        <el-col :span="1.5">
          <el-button
            size="small"
            @click="refresh"
          >
            <el-tooltip class="item" effect="dark" content="刷新" placement="top-start">
              <i class="iconfont icon-shuaxin no-margin" />
            </el-tooltip>
          </el-button>
        </el-col>
      </el-row>
    </tags-view>
    <el-card class="main-card">
      <el-form v-show="showSearch" ref="queryForm" :model="queryParams" :inline="true">
        <el-form-item label="出库单号" prop="orderNo">
          <el-input
            v-model.trim="queryParams.orderNo"
            placeholder="请输入"
            clearable
            maxlength="15"
            size="small"
            style="width: 200px"
            @keyup.enter.native="handleQuery"
          />
        </el-form-item>
        <el-form-item label="单据名称" prop="orderName">
          <el-input
            v-model.trim="queryParams.orderName"
            placeholder="请输入"
            clearable
            maxlength="15"
            size="small"
            style="width: 200px"
            @keyup.enter.native="handleQuery"
          />
        </el-form-item>
        <el-form-item label="出库类型" prop="outType">
          <el-select v-model="queryParams.outType" style="width: 200px" placeholder="请选择" clearable size="small">
            <el-option
              v-for="dict in dict.type.outbound_order_origin"
              :key="dict.value"
              :label="dict.label"
              :value="dict.value"
            />
          </el-select>
        </el-form-item>
        <!-- <el-form-item label="存放仓库" prop="warehouseId">
          <el-select v-model="queryParams.warehouseId" style="width: 200px" placeholder="请选择" clearable size="small">
            <el-option
              v-for="item in warehouseData"
              :key="item.id"
              :label="item.name"
              :value="item.id"
            />
          </el-select>
        </el-form-item> -->
        <el-form-item label="关联单号" prop="bizOrderNo">
          <el-input
            v-model.trim="queryParams.bizOrderNo"
            placeholder="请输入"
            clearable
            maxlength="15"
            size="small"
            style="width: 200px"
            @keyup.enter.native="handleQuery"
          />
        </el-form-item>
        <el-form-item label="出库状态" prop="warehousingStatus">
          <el-select v-model="queryParams.warehousingStatus" style="width: 200px" placeholder="请选择" clearable size="small">
            <el-option
              v-for="dict in dict.type.outbound_order_status"
              :key="dict.value"
              :label="dict.label"
              :value="dict.value"
            />
          </el-select>
        </el-form-item>
        <el-form-item label="出库时间" prop="dateRange">
          <div class="date_box">
            <el-date-picker
              v-model="queryParams.dateRange"
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
          <el-button v-debounce size="small" @click="resetQuery">重置</el-button>
          <el-button v-debounce v-hasPermi="['warehousem:outboundOrder:list']" type="primary" class="search-btn" plain size="small" @click="handleQuery">搜索</el-button>
        </el-form-item>
      </el-form>
      <!-- show table -->
      <el-table ref="warehouseTable" v-loading="loading" highlight-current-row :data="listData" height="100%" @selection-change="handleSelectionChange">
        <el-table-column type="selection" width="55" align="left" />
        <el-table-column label="序号" type="index" width="55" align="left"><template #default="scope">
          <span>{{ (queryParams.pageNum - 1) * queryParams.pageSize + scope.$index + 1 }}</span>
        </template></el-table-column>
        <el-table-column label="出库单号" align="center" prop="orderNo" :show-overflow-tooltip="true" />
        <el-table-column label="单据名称" align="center" prop="orderName" :show-overflow-tooltip="true" />
        <el-table-column label="出库类型" align="center" prop="outType" :show-overflow-tooltip="true">
          <template slot-scope="scope">
            <span v-if="scope.row.outType==='0'">领用</span>
            <span v-if="scope.row.outType==='1'">调拨</span>
            <span v-if="scope.row.outType==='2'">捐赠</span>
            <span v-if="scope.row.outType==='10'">其他</span>
          </template>
        </el-table-column>
        <!-- <el-table-column label="存放仓库" align="center" prop="warehouseName" :show-overflow-tooltip="true" /> -->
        <el-table-column label="关联单号" align="left" prop="bizNo" :show-overflow-tooltip="true" />
        <el-table-column label="出库状态" align="left" prop="warehousingStatus" :show-overflow-tooltip="true">
          <template slot-scope="scope">
            <el-tag v-if="scope.row.warehousingStatus === '0'">待出库</el-tag>
            <el-tag v-else-if="scope.row.warehousingStatus === '1'" type="success">已出库</el-tag>
            <el-tag v-else type="danger">已废弃</el-tag>
          </template>
        </el-table-column>
        <el-table-column label="出库时间" align="left" prop="warehousingTime" :show-overflow-tooltip="true" />
        <el-table-column label="负责人" align="left" prop="handler" :show-overflow-tooltip="true" />
        <el-table-column label="操作" width="200" align="left" class-name="small-padding">
          <template slot-scope="scope">
            <el-button
              v-if="scope.row.warehousingStatus==='0'"
              v-hasPermi="['warehousem:outboundOrder:confirm']"
              v-debounce
              size="small"
              type="text"
              style="color:#4686EF"
              @click="handleDetail(scope.row,'1')"
            >确认出库</el-button>
            <el-button
              v-debounce
              v-hasPermi="['warehousem:outboundOrder:info']"
              size="small"
              type="text"
              style="color:#4686EF"
              @click="handleDetail(scope.row,'2')"
            >详情</el-button>
            <!-- <el-button
                v-if="scope.row.warehousingStatus=='0'"
                v-hasPermi="['system:post:edit']"
                size="small"
                type="text"
                style="color:#4686EF"
                @click="handleDetail(scope.row,'1')"
              >编辑</el-button> -->
            <el-button
              v-if="scope.row.warehousingStatus==='2'"
              v-debounce
              v-hasPermi="['warehousem:outboundOrder:delete']"
              size="small"
              type="text"
              style="color:#EF5826"
              @click="handleDelete(scope.row)"
            >删除</el-button>
          </template>
        </el-table-column>
      </el-table>
      <!-- 分页 -->
      <div class="common-pagination">
        <!-- <div class="chooseCount">已选{{ selectedLength }}条</div> -->
        <pagination
          v-show="total > 0"
          :total="total"
          :page.sync="queryParams.pageNum"
          :limit.sync="queryParams.pageSize"
          @pagination="getList"
        />
      </div>
    </el-card>
    <!-- 新建或编辑位置对话框 -->
    <el-dialog
      v-if="open"
      :z-index="1000"
      class="dialog-class"
      :title="title"
      :visible.sync="open"
      width="1000px"
      append-to-body
      destroy-on-close
      :close-on-click-modal="false"
      @close="closeDialog"
    >
      <!-- 新增出库组件 -->
      <AddOutboundOrder ref="addForm" :open="open" :is-edit="isEdit" :detail-list="detailList" :log-list="logList" :form-data="formData" :user-data="userData" @getDetailRow="getDetailRow" @addDetail="addDetail" @delDetail="delDetail" />
      <div v-if="isEdit==='0'" slot="footer" class="add-footer">
        <el-button size="small" @click="cancel">取 消</el-button>
        <el-button v-debounce size="small" type="primary" @click="submitAddForm">保存</el-button>
      </div>
      <div v-else slot="footer" class="dialog-footer">
        <div :class="[formData.warehousingStatus==='0'?'colorBlue':formData.warehousingStatus==='1'?'colorGreen':'colorRed']">
          <i class="iconfont icon-xiangqingxinxi" />
          <span v-if="formData.warehousingStatus==='0'">待出库</span>
          <span v-else-if="formData.warehousingStatus==='1'">已出库</span>
          <span v-else>已废弃</span>
        </div>
        <div v-if="isEdit==='1'" class="dialog-footer">
          <el-button size="small" @click="open=false">取消</el-button>
          <el-button v-debounce size="small" plain type="primary" @click="abandon">废弃</el-button>
          <el-button size="small" type="primary" @click="sureAddForm">保存</el-button>
        </div>

        <el-button v-if="isEdit==='2'" size="small" @click="open=false">关闭</el-button>
      </div>
    </el-dialog>
    <!-- 出库明细的详情信息 -->
    <el-dialog
      :z-index="1000"
      class="dialog-class"
      title="明细详情"
      :visible.sync="openDetail"
      width="1000px"
      append-to-body
      :close-on-click-modal="false"
      @close="openDetail=false"
    >
      <!-- 组件 -->
      <OutBoundDetail ref="outBoundDetail" :data-list="assetsList" :row-data="rowData" />
      <div slot="footer">
        <el-button size="small" @click="openDetail=false">关闭</el-button>
      </div>
    </el-dialog>
    <!-- 打印标签 -->
    <el-dialog
      :z-index="1000"
      title="绑卡"
      :visible.sync="bindDig"
      width="1200px"
      append-to-body
      :close-on-click-modal="false"
      class="selectPlan"
      @close="closebindDig"
    >
      <Print ref="selectDetail" :assets-list="assetsList" />
      <div slot="footer" class="dialog-footer" />
    </el-dialog>
    <FilePrint ref="FilePrint" type="outboundOrder" />

    <!-- 确认出库选择资产 -->
    <el-dialog
      :z-index="1000"
      class="dialog-class"
      title="选择资产"
      :visible.sync="selectAsset"
      width="1200px"
      append-to-body
      :close-on-click-modal="false"
      @close="closeSelectAsset"
    >
      <el-row :gutter="40">
        <el-col :span="16">
          <el-table
            style="width: 100%;margin: 10px;"
            border
            :data="assetDetailList"
            highlight-current-row
            max-height="800"
            @current-change="handleSelectionAssetChange"
          >
            <!-- <el-table-column type="selection" /> -->
            <el-table-column label="单选" align="center" width="60">
              <template slot-scope="scope">
                <el-radio v-model="radio" :label="scope.row.materialCode" />
              </template>
            </el-table-column>
            <el-table-column
              prop="materialName"
              label="资产名称"
              align="center"
            />
            <el-table-column
              prop="standard"
              label="规格型号"
              width="180"
              align="center"
            />
            <el-table-column
              prop="warehouseName"
              label="出库仓库"
              width="120"
              align="center"
            />
            <el-table-column
              prop="qty"
              label="出库数量"
              width="120"
              align="center"
            />
            <el-table-column
              prop="selectQty"
              label="已选数量"
              width="120"
              align="center"
            >
              <template slot-scope="scope">
                <a style="margin-left: 10px" @click="showSelectCodeList(scope.row)">{{ getSelectQty(scope.row) }}</a>
              </template>
            </el-table-column>
          </el-table>
        </el-col>
        <el-col class="selectBox" :span="8">
          <div class="selectAssetCode">
            <el-input
              v-model="assetCode"
              placeholder="请扫描或输入资产编码"
              :maxlength="20"
              class="input-class"
              @blur="handleSelectAssetQuery"
            />
            <div class="selectBtn">
              <el-button size="small" @click="resetSelectAssetQuery">重置</el-button>
              <el-button size="small" class="search-btn" @click="handleSelectAssetQuery">搜索</el-button>
            </div>
          </div>
          <el-table
            ref="selectCode"
            style="width: 100%"
            border
            :row-key="getRowKeys"
            :data="assetCodeList"
            highlight-current-row
            max-height="430"
            @selection-change="handleSelectionAssetCodeChange"
          >
            <el-table-column type="selection" :reserve-selection="true" />
            <el-table-column
              prop="internalCode"
              label="资产编码"
              align="center"
            />
            <el-table-column
              prop="acquireDate"
              label="入库时间"
              align="center"
            />
          </el-table>
        </el-col>
      </el-row>
      <div slot="footer">
        <el-button size="small" @click="clearSelectAsset">清空已选</el-button>
        <el-button size="small" @click="sureOut">确认出库</el-button>
      </div>
    </el-dialog>
    <!-- 选中资产列表 -->
    <el-dialog
      :z-index="1000"
      class="dialog-class"
      title="资产列表"
      :visible.sync="showCodeList"
      width="1200px"
      append-to-body
      :close-on-click-modal="false"
      @close="showCodeList = false"
    >
      <el-table
        style="width: 100%"
        border
        :data="codeList"
        highlight-current-row
        max-height="430"
      >
        <el-table-column
          prop="code"
          label="资产编码"
          align="center"
        />
        <el-table-column
          prop="acquireDate"
          label="入库时间"
          align="center"
        />
      </el-table>
    </el-dialog>
  </div>
</template>

<script>
import {
  getOutboundOrderOrder,
  getOutboundOrderById,
  exportOutboundOrder,
  addOutboundOrder,
  deleteOutboundOrderOrderById,
  abandonOutboundOrderOrderById,
  updateOutboundOrder,
  getAssetCardByMaterialCodeAndWarehouseId,
  getLog
} from '@/api/warehouse/index'
import { listWarehouse } from '@/api/basicdata/warehouse'
import TagsView from '@/layout/components/TagsView/index.vue'
import FilePrint from '@/components/FilePrint'
import OutBoundDetail from './component/outBoundDetail.vue'
import AddOutboundOrder from './component/addOutboundOrder.vue'
import { blobValidate } from '@/utils/jri'
import errorCode from '@/utils/errorCode'
import { saveAs } from 'file-saver'
import { Message, Loading } from 'element-ui'
import Print from './component/print.vue'
import { getAssetByOutIdAndMaterialId } from '@/api/warehouse/index'
import { listUser } from '@/api/system/user'
export default {
  name: 'Warehousing',
  components: { TagsView, AddOutboundOrder, OutBoundDetail, Print, FilePrint },
  dicts: ['outbound_order_origin', 'outbound_order_status'],
  data() {
    return {
      bindDig: false,
      assetsList: [], // 选中的出库单绑定的全部资产信息
      // 遮罩层
      loading: true,
      // 选中数组
      ids: [],
      // 非单个禁用
      single: true,
      // 非多个禁用
      multiple: true,
      // 显示搜索条件
      showSearch: true,
      // 选中的数量
      selectedLength: 0,
      // 总条数
      total: 0,
      // 品牌表格数据
      listData: [],
      // 弹出层标题
      title: '',
      // 是否显示弹出层
      open: false,
      // 是否显示弹出层
      openDetail: false,
      // 0是新增1是出库确认2是详情
      isEdit: '0',
      // 排序
      listLength: '',
      // 查询参数
      queryParams: {
        pageNum: 1,
        pageSize: 10,
        orderName: '',
        orderNo: '',
        outType: '',
        warehouseId: '',
        bizOrderNo: '',
        warehousingStatus: '',
        dateRange: []
      },
      // 表单参数
      form: {},
      selectionName: [],
      detailList: [],
      logList: [],
      // 仓库列表
      warehouseData: [],
      formData: {
        id: undefined
      },
      rowData: {},
      selectAsset: false,
      assetCode: '',
      radio: '',
      // 选择具体资产编码列表参数
      assetCodeList: [],
      totalAssetCodeList: [],
      // 选择具体资产列表参数
      assetDetailList: [],
      // 资产编码选中数组
      codes: [],
      showCodeList: false,
      codeList: [],
      enable: true, // 判断数量选择是否正确
      // 用户数据
      userData: []
    }
  },
  created() {
    this.getList()
    this.getUserList()
  },
  methods: {
    // 自动生成出库单号
    autoGeneration() {
      const currentDate = new Date().toLocaleString()
      const year = currentDate.slice(2, 4) // 获取当前年份(2位)
      let month = currentDate.slice(5, 6) // 获取当前月份(0-11,0代表1月)
      if (month.length === 1) {
        month = '0' + month
      }
      const date = currentDate.slice(7, 9) // 获取当前日(1-31)
      const random = this.randomNum()
      this.formData.orderNo = 'CK' + year + month + date + random
    },
    // 获取随机数001-999
    randomNum() {
      var result = Math.floor(Math.random() * 1000)
      if (result < 10) {
        return '00' + result
      } else if (result < 100) {
        return '0' + result
      } else {
        return String(result)
      }
    },
    /** 查询列表 */
    getList() {
      const data =
        {
          pageNum: this.queryParams.pageNum,
          pageSize: this.queryParams.pageSize,
          orderName: this.queryParams.orderName,
          orderNo: this.queryParams.orderNo,
          outType: this.queryParams.outType,
          warehouseId: this.queryParams.warehouseId,
          bizOrderNo: this.queryParams.bizOrderNo,
          warehousingStatus: this.queryParams.warehousingStatus,
          endDate: this.queryParams.dateRange[1],
          startDate: this.queryParams.dateRange[0]
        }
      getOutboundOrderOrder(data).then(response => {
        //
        if (response.code === 200) {
          this.listData = response.data.records
          this.listLength = response.data.total + 1
          this.total = response.data.total
          this.loading = false
        } else {
          this.loading = false
          this.$message.error(response.msg)
        }
      })
      // 获取仓库列表
      listWarehouse({ pageSize: 999 }).then(
        res => {
          this.warehouseData = res.data.records
        }
      )
    },
    /** 头部导出按钮操作 */
    handleExport() {
      // if (this.ids.length === 0) {
      //   // return this.$message.error('请选择需要导出的内容')
      //   this.listData.forEach(item => {
      //     this.ids.push(item.id)
      //   })
      // }
      //  this.ids
      const downloadLoadingInstance = Loading.service({ text: '正在下载数据，请稍候', spinner: 'el-icon-loading', background: 'rgba(0, 0, 0, 0.7)' })
      exportOutboundOrder(this.ids).then(
        async data => {
          const filename = `出库管理_${this.parseTime(new Date(), '{y}-{m}-{d}')}.xlsx`
          const isLogin = await blobValidate(data)
          if (isLogin) {
            const blob = new Blob([data])
            saveAs(blob, filename)
          } else {
            const resText = await data.text()
            const rspObj = JSON.parse(resText)
            const errMsg = errorCode[rspObj.code] || rspObj.msg || errorCode['default']
            Message.error(errMsg)
          }
          downloadLoadingInstance.close()
        }
      )
    },
    /** 打印 */
    handlePrint () {
      if (!this.ids.length) {
        return this.$message.warning('请勾选要打印的项')
      }
      this.$modal.loading('正在获取打印数据，请稍候...')
      Promise.all(this.ids.map(item => getAssetByOutIdAndMaterialId({ outboundOrderId: item }))).then(res => {
        // console.log(res)
        this.$modal.closeLoading()
        this.$refs.FilePrint.show(res.map(item => item.data))
      })
    },
    /** 头部新增按钮操作 */
    handleAdd() {
      this.reset()
      this.formData.outType = '10'
      this.open = true
      this.isEdit = '0'
      this.title = '新增出库'
      // this.autoGeneration()/后端已经自动生成了
      // this.$store.dispatch('getNo', 'CK').then((res) => {
      //   this.formData.orderNo = res.data
      // })
      this.$nextTick(() => {
        this.$refs.addForm.editWarehouse()
      })
    },
    /** 关闭按钮 */
    closeDialog() {
      this.open = false
    },
    showBindDid() {
      this.bindDig = true
    },
    closebindDig() {
      this.bindDig = false
    },
    /** 取消按钮 */
    cancel() {
      this.open = false
      this.reset()
    },
    cancel1() {
      this.openSure = false
      this.reset()
    },
    /** 头部tags-view刷新按钮操作 */
    refresh() {
      this.getList()
    },
    /** 表单重置 */
    reset() {
      this.formData = {
        id: undefined,
        orderNo: '',
        orderName: '',
        outType: '',
        bizNo: '',
        dept: '',
        people: '',
        handlerName: '',
        company: '',
        companyName: '',
        fileList: []
      }
      this.detailList = []
    },
    /** 新增编辑保存按钮 */
    submitAddForm: function () {
      this.formData = this.$refs.addForm.form
      console.log(this.formData, 'formData')

      const arr = []
      this.detailList.map((e) => {
        const obj = {
          location: e.warehouseId,
          locationName: e.warehouseName,
          materialCode: e.materialCode,
          materialId: e.materialId,
          outboundDate: e.outboundDate,
          qty: Number(e.qty)
        }
        arr.push(obj)
      })
      //
      this.$refs['addForm'].$refs.form.validate(valid => {
        // const api = this.isEdit === '0' ? addOutboundOrder : updateOutboundOrder//已经删除编辑功能，在出库确认进行编辑数量
        if (valid) {
          if (this.detailList.length === 0) {
            return this.$message.error('请选择新增明细信息')
          }
          // const noNum = arr.some((e) => {
          //   return !e.qty
          // })
          // if (noNum) {
          //   return this.$message.error('出库数须大于0')
          // }
          // this.$store.dispatch('getNo', 'CK').then((res) => {
          // this.formData.orderNo = res.data
          this.$refs['addForm'].$refs.tableForm.validate(valid => {
            if (valid) {
              const data = {
                // ...this.formData,
                id: this.formData.id,
                orderNo: this.formData.orderNo,
                orderName: this.formData.orderName,
                company: this.formData.company,
                outType: this.formData.outType,
                bizNo: this.formDatabizNo,
                dept: this.formData.dept,
                people: this.formData.people,
                placeId: this.formData.placeId,
                placeName: this.formData.placeName,
                fileList: this.formData.fileList,
                detailList: arr
              }
              addOutboundOrder(data).then(res => {
                if (res.code === 200) {
                  this.$modal.msgSuccess('保存成功')
                  this.open = false
                  this.resetForm('queryForm')
                  this.getList()
                } else {
                  this.$message.error(res.msg)
                }
              })
            }
          })
        }
      })
    },
    // 确认出库
    sureAddForm() {
      // 选择资产明细
      this.selectAsset = true
      this.codes = []
      this.$nextTick(() => {
        this.$refs.selectCode.clearSelection()
      })
      // 默认选中第一个
      this.radio = this.assetDetailList[0].materialCode
      // 请求接口
      const assetCardDtoList = this.assetDetailList.map(item => {
        return {
          materialCode: item.materialCode,
          warehouseId: item.warehouseId
        }
      })

      getAssetCardByMaterialCodeAndWarehouseId(assetCardDtoList).then(res => {
        if (res.code === 200) {
          this.totalAssetCodeList = res.data.flat() || []
          // this.assetCodeList = this.totalAssetCodeList
          this.handleSelectionAssetChange(this.assetDetailList[0])
        }
      })
    },
    /** 选择资产关闭 */
    closeSelectAsset() {
      this.selectAsset = false
      this.radio = ''
      this.assetCode = ''
    },
    /** 已选数量 */
    getSelectQty(row) {
      const arr = []
      Array.from(this.codes).forEach(item => {
        if (item.goodCode === row.materialCode) {
          arr.push(item)
        }
      })
      const num = arr.length
      if (num !== row.qty) {
        this.enable = false
      } else {
        this.enable = true
      }
      return num
    },
    /** 最终确认出库 */
    sureOut() {
      // 判断出库数量是否与已选数量相等
      if (!this.enable) {
        this.$message.error('已选有效资产数量与出库数量不一致')
        return
      }
      const arr = []
      this.assetDetailList.forEach((e) => {
        const codesArr = []
        this.codes.forEach(item => {
          if (item.goodCode === e.materialCode) {
            codesArr.push(item.code)
          }
        })
        const obj = {
          codes: codesArr,
          location: e.id,
          locationName: e.warehouseName,
          materialCode: e.materialCode,
          materialId: e.materialId,
          outboundDate: e.outboundDate,
          qty: Number(e.qty)
        }
        arr.push(obj)
      })
      const data = {
        id: this.formData.id,
        orderNo: this.formData.orderNo,
        orderName: this.formData.orderName,
        company: this.formData.company,
        outType: this.formData.outType,
        bizNo: this.formDatabizNo,
        dept: this.formData.dept,
        people: this.formData.people,
        fileList: this.formData.fileList,
        placeId: this.formData.placeId,
        placeName: this.formData.placeName,
        detailList: arr,
        warehousingStatus: '1'// 更改状态
      }
      updateOutboundOrder(data).then(response => {
        if (response.code === 200) {
          this.$modal.msgSuccess('出库成功')
          // 出库确认后时要弹出打印弹框
          this.getAssetByOutIdAndMaterialId({ outboundOrderId: this.formData.id })
          this.bindDig = true
          this.selectAsset = false
          this.open = false
          this.resetForm('queryForm')
          this.getList()
        } else {
          this.$message.error(response.msg)
        }
      })
    },
    /** 清空已选 */
    clearSelectAsset() {
      this.radio = ''
      this.codes = ''
      this.assetCode = ''
      this.$refs.selectCode.clearSelection()
    },
    /** 搜索按钮操作 */
    handleQuery() {
      this.queryParams.pageNum = 1
      this.getList()
    },
    /** 重置按钮操作 */
    resetQuery() {
      this.resetForm('queryForm')
      this.handleQuery()
    },
    // 多选框选中数据
    handleSelectionChange(selection) {
      this.selectedLength = selection.length
      this.ids = selection.map(item => item.id)
      this.selectionName = selection.map(item => item.name)
      this.single = selection.length !== 1
      this.multiple = !selection.length
    },
    /** 查看按钮操作 */
    handleDetail(row, type) {
      this.open = true
      this.isEdit = type
      if (type === '1') {
        this.title = '出库确认'
      } else if (type === '2') {
        this.title = '出库详情'
        this.getLog(row.id)
      }
      this.detailList = []
      // 查询明细
      getOutboundOrderById(row.id).then(response => {
        this.formData = response.data
        //
        const detailListVo = response.data.detailListVo;
        (detailListVo || []).map((e) => {
          const obj = {
            brandName: e.brand,
            materialCode: e.code,
            materialId: e.id,
            materialName: e.name,
            qty: e.qty,
            selectQty: 0,
            standard: e.standard,
            type: e.type,
            unitName: e.unit,
            id: e.location,
            warehouseId: e.location,
            warehouseName: e.locationName,
            outboundDate: e.outboundDate,
            categoryNameLevel: e.categoryNameLevel4 || e.categoryNameLevel3 || e.categoryNameLevel2 || e.categoryNameLevel1
          }
          this.detailList.push(obj)
        })
        this.assetDetailList = this.detailList

        this.$nextTick(() => {
          this.$refs.addForm.editWarehouse()
        })
      })
    },
    getLog(id) {
      const data = {
        bizOrderId: id,
        bizType: 'type_outbound_order',
        pageNum: 1,
        pageSize: 1000
      }
      getLog(data).then(response => {
        if (response.code === 200) {
          this.logList = response.data.records
          //
        } else {
          this.$message.error(response.msg)
        }
      })
    },
    /** 废弃按钮操作 */
    abandon() {
      const ids = this.formData.id
      const name = this.formData.orderNo
      this.$modal.confirm('确定废弃出库单"' + name + '"？', '确定废弃').then(function() {
        return abandonOutboundOrderOrderById(ids)
      }).then(() => {
        this.getList()
        this.open = false
        this.$modal.msgSuccess('废弃成功')
      }).catch(() => {})
    },

    /** 删除按钮操作 */
    handleDelete(row) {
      const ids = row.id
      const name = row.orderNo
      this.$modal.confirm('确定删除出库单"' + name + '"？', '确定删除').then(function() {
        return deleteOutboundOrderOrderById(ids)
      }).then(() => {
        this.queryParams.pageNum = this.changePageNum(this.total, 1, this.queryParams.pageNum, this.queryParams.pageSize)
        this.getList()
        this.$modal.msgSuccess('删除成功')
      }).catch(() => {})
    },
    /** 出库确认 */
    handleSure() {
      this.openSure = true
      this.title = '出库确认'
    },
    getDetailRow(data) {
      this.openDetail = true
      this.rowData = data
      const queryData = {
        materialCode: this.rowData.materialCode,
        warehouseId: this.rowData.warehouseId,
        outboundOrderId: this.formData.id
      }
      this.getAssetByOutIdAndMaterialId(queryData)
      this.$nextTick(() => {
        this.$refs.outBoundDetail.editWarehouse()
      })
      //
    },
    // 增加明细
    addDetail(data) {
      this.detailList = data
    },
    // 删除明细
    delDetail(row) {
      const index = this.detailList.findIndex(e => {
        return e.id === row.id
      })
      this.detailList.splice(index, 1)
    },
    getAssetByOutIdAndMaterialId(data) {
      // const data = {
      //   materialCode: '', // 不传即查询全部关联资产
      //   // outboundOrderId: this.formData.id,
      //   // materialCode: '3301',
      //   outboundOrderId: id
      // }
      getAssetByOutIdAndMaterialId(data).then((response) => {
        this.assetsList = response.data
        // this.assetsList.map((e) => {
        //   this.$set(e, 'afterPrice', e.price * (1 + e.rate))
        // })
        // console.log(this.assetsList, 'assetsList')
      })
    },
    /** 选择资产 */
    handleSelectionAssetChange(value) {
      const temp = value || ''
      this.radio = value.materialCode || ''
      const arr = []
      this.totalAssetCodeList.forEach(item => {
        if (item.goodCode === temp.materialCode) {
          arr.push(item)
        }
      })
      this.assetCodeList = arr
    },
    /**  扫描或输入资产编码搜索 */
    handleSelectAssetQuery() {
      if (!this.assetCode) {
        this.assetCodeList = this.totalAssetCodeList
        return
      }
      const arr = []
      this.totalAssetCodeList.forEach(item => {
        if (item.internalCode.indexOf(this.assetCode) !== -1) {
          arr.push(item)
        }
      })
      this.assetCodeList = arr
    },
    /** 选择资产重置 */
    resetSelectAssetQuery() {
      this.assetCode = ''
      this.assetCodeList = this.totalAssetCodeList
    },
    /** 资产编码多选选中 */
    handleSelectionAssetCodeChange(selection) {
      this.codes = selection || []
    },
    // 确定唯一的key值
    getRowKeys(row) {
      return row.code
    },
    /** 展示选中的资产编码列表 */
    showSelectCodeList(row) {
      this.showCodeList = true
      const codeListArr = []
      this.codes.forEach(item => {
        if (item.goodCode === row.materialCode) {
          codeListArr.push(item)
        }
      })
      this.codeList = codeListArr
    },
    getUserList () {
      listUser({ pageNum: 1, pageSize: 9999 }).then(response => {
        this.userData = response.rows
      }
      )
    }

  }
}
</script>
<style lang="scss" scoped>
.post-box {
  .btn-icon{
    width: 18px;
    height: 18px;
    vertical-align: middle;
  }
  .btn-text{
    vertical-align: middle;
  }
}
.statePoint .point{
  width:8px;
  height:8px;
  margin:0 8px 2px 0;
}
.select-width{
  ::v-deep .el-input__inner{
    width: 456px;
  }
}
.detail{
  margin: 0 auto;
}
.dialog-class{
  ::v-deep .el-dialog{
    padding: 0 20px;
  }
  ::v-deep .el-dialog__body{
    min-height: 600px;
    max-height: 800px;
    overflow: scroll;
  }
  .add-footer{
  text-align: end;
    background: #fff;
  }
  .dialog-footer{
    display: flex;
    justify-content:space-between;
    background: #fff;
  }
}
.detail-item{
  display: flex;
  .item-label{
    width: 128px;
    height: 16px;
    font-size: 16px;
    font-weight: 400;
    color: #6A7697;
    line-height: 24px;
    margin:0 16px 16px 0 ;
    text-align: end;
  }
}
.add-form{
  display:flex;
  justify-content:space-between
}
.input-width{
  ::v-deep .el-input__inner{
    width: 500px;
  }
}
::v-deep .el-dialog__body{
  padding: 5px;
}
::v-deep .el-dialog__footer{
  padding: 15px 0;
}
.colorBlue{
  font-size: 14px;
  color: #4686EF;
}
.colorGreen{
  font-size: 14px;
  color: #3DB954;
}
.colorRed{
  font-size: 14px;
  color: #EF5826;
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
.selectBox {
  margin-top: 10px;
  padding-left: 0 !important;
  padding-right: 0 !important;
  .selectAssetCode {
    display: flex;
    margin-bottom: 10px;
    justify-content: space-between;
    ::v-deep .el-input--medium .el-input__inner {
      height: 80px;
      line-height: 80px;
    }
    ::v-deep .el-input__inner::placeholder {
      text-align: left;
    }
    .input-class {
      width: 294px;
    }
    .selectBtn {
      display: flex;
      flex-direction: column;
      justify-content: space-around;
      margin-right: 25px;
      .el-button {
        margin-left: 0;
      }
    }
  }
}
::v-deep .el-radio__label {
  display: none;
}
</style>

