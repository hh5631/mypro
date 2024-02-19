<template>
  <div>
    <tags-view>
      <el-row :gutter="10" class="mb8">

        <!-- <el-col :span="1.5">
          <el-button
            plain
            size="small"
            @click="handleExport"
          ><i style="color:#4686EF;" class="iconfont icon-daochu" />导出</el-button>
        </el-col> -->
        <!-- <el-col :span="1.5">
          <el-button
            plain
            size="small"
          ><i style="color:#4686EF;" class="iconfont icon-jichushuju" />导出卡片</el-button>
        </el-col> -->
        <el-col :span="1.5">
          <el-button
            v-debounce
            type="primary"
            size="small"
            :disabled="disPrint"
            @click="handlePrint"
          ><i class="iconfont icon-dayin" style="color: #ffffff" />打印标签</el-button>
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
      <!--位置右侧list数据-->
      <el-form
        ref="queryForm"
        :model="queryParams"
        :inline="true"
        label-width="68px"
        class="searchForm"
      >
        <el-form-item label="资产名称" prop="name">
          <el-input
            v-model.trim="queryParams.name"
            placeholder="请输入"
            clearable
            size="small"
            style="width: 200px"
            maxlength="15"
            @keyup.enter.native="handleQuery"
          />
        </el-form-item>
        <el-form-item label="资产编码" prop="internalCode">
          <el-input
            v-model.trim="queryParams.internalCode"
            placeholder="请输入"
            clearable
            size="small"
            style="width: 200px"
            @keyup.enter.native="handleQuery"
          />
        </el-form-item>
        <el-form-item label="状态" prop="status">
          <el-select v-model="queryParams.status" style="width: 130px" placeholder="请选择" clearable size="small">
            <el-option
              v-for="dict in dict.type.assets_enable_status"
              :key="dict.value"
              :label="dict.label"
              :value="dict.value"
            />
          </el-select>
        </el-form-item>
        <el-form-item label="出库单号" prop="bizNo">
          <el-input
            v-model.trim="queryParams.bizNo"
            placeholder="请输入"
            clearable
            size="small"
            style="width: 200px"
            maxlength="15"
            @keyup.enter.native="handleQuery"
          />
        </el-form-item>
        <el-form-item>
          <el-button size="small" @click="resetQuery">重置</el-button>
          <el-button size="small" class="search-btn" @click="handleQuery">搜索</el-button>
        </el-form-item>
      </el-form>
      <!-- 表格显示list数据 -->
      <el-table ref="tableData" v-loading="loading" highlight-current-row height="550" :data="tableData" @selection-change="handleSelectionChange">
        <el-table-column type="selection" align="center" />
        <el-table-column label="序号" sortable align="left" prop="index" type="index" />
        <el-table-column label="二维码" align="left" prop="codeIcon" :show-overflow-tooltip="true">
          <template slot-scope="scope">
            <div>
              <img :src="scope.row.codeIcon" alt="" width="40px" height="40px">
            </div>
          </template>
        </el-table-column>
        <el-table-column label="RFID编号" align="left" prop="rfidCode" :show-overflow-tooltip="true">
          <template slot-scope="scope">
            <span v-if="scope.row.isBind === '1'">{{ scope.row.rfidCode }}</span>
          </template>
        </el-table-column>
        <el-table-column label="资产编码" align="left" prop="internalCode" width="180" :show-overflow-tooltip="true" />
        <el-table-column label="资产名称" align="left" prop="name" :show-overflow-tooltip="true" />
        <el-table-column label="品牌" align="left" prop="brandName" :show-overflow-tooltip="true" />
        <el-table-column label="规格型号" align="left" prop="standard" :show-overflow-tooltip="true" />
        <el-table-column label="计量单位" align="left" prop="unit" :show-overflow-tooltip="true" />
        <el-table-column label="状态" align="left" prop="status">
          <template slot-scope="scope">
            <!-- <el-tag v-if="scope.row.status==='0'" type="success">闲置</el-tag>
            <el-tag v-if="scope.row.status === '1'">领用中</el-tag>
            <el-tag v-if="scope.row.status === '2'" type="warning">退库中</el-tag>
            <el-tag v-if="scope.row.status === '3'" type="info">使用</el-tag> -->
            <el-tag :type="filterTagType(scope.row.status)">{{ filterStatus(scope.row.status) }}</el-tag>
          </template>
        </el-table-column>
        <el-table-column label="绑定状态" align="left" prop="status">
          <template slot-scope="scope">
            <span v-if="scope.row.isBind==='0'">未绑定</span>
            <span v-if="scope.row.isBind === '1'" class="bind-class">已绑定</span>
          </template>
        </el-table-column>
        <el-table-column label="使用部门" align="left" prop="dept" :show-overflow-tooltip="true" />
        <el-table-column label="使用人" align="left" prop="people" :show-overflow-tooltip="true" />
        <el-table-column label="存放地点" align="left" prop="warehouseName" :show-overflow-tooltip="true" />
        <el-table-column label="操作" align="left" class-name="small-padding" width="130">
          <template slot-scope="scope">
            <el-button
              v-hasPermi="['assetManagement:labelPrint:getMaxRfid']"
              size="small"
              type="text"
              @click="bindCard(scope.row)"
            >{{ scope.row.isBind === '0'?"绑卡":"更新" }}</el-button>

            <!-- <el-button
              v-if="scope.row.isBind === '1'"
              type="text"
              size="small"
              @click="handleUpdate(scope.row)"
            >更新</el-button> -->
            <!-- <el-button
                v-if="scope.row.status ==='0'"
                size="mini"
                type="text"
                style="color:#EF5826"
                @click="handleDelete(scope.row)"
              >删除</el-button> -->
            <el-button
              size="small"
              type="text"
              @click="handleDetail(scope.row)"
            >详情</el-button>
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

      <!-- 详情 -->
      <el-dialog
        title="资产详情"
        :visible.sync="openDetail"
        width="1050px"
        append-to-body
        :close-on-click-modal="false"
        class="selectPlan"
        @close="closeDetailDig"
      ><AssetsDetail ref="assetsDetail" :base-form="baseForm" :use-records="useRecords" /><div slot="footer" class="dialog-footer" /></el-dialog>
      <!-- REID绑定 -->
      <el-dialog
        title="REID绑定"
        :visible.sync="open"
        width="1000px"
        append-to-body
        :close-on-click-modal="false"
        class="selectPlan"
        @close="closeBindDig"
      ><BindingReid ref="addAssetsCard" :table-data="selectedData" /><div slot="footer" class="dialog-footer">
        <el-button size="small" @click="closeBindDig">取消</el-button>
        <el-button size="small" type="primary" @click="batchBindRfId">保存</el-button>
      </div></el-dialog>
      <!-- REID绑定 -->
      <el-dialog
        :title="rfidTitle"
        :visible.sync="openUpdate"
        width="800px"
        append-to-body
        :close-on-click-modal="false"
        @close="closeUpdateDig"
      >
        <el-form :model="updateForm" label-position="right" label-width="150px" class="updateCss">
          <el-form-item label="资产编码" prop="internalCode">
            <el-input v-model.trim="updateForm.internalCode" disabled />
          </el-form-item>
          <el-form-item label="资产名称" prop="name">
            <el-input v-model.trim="updateForm.name" disabled />
          </el-form-item>
          <el-form-item label="品牌" prop="brandName">
            <el-input v-model.trim="updateForm.brandName" disabled />
          </el-form-item>
          <el-form-item label="规格型号" prop="standard">
            <el-input v-model.trim="updateForm.standard" disabled />
          </el-form-item>
          <el-form-item label="使用部门" prop="dept">
            <el-input v-model.trim="updateForm.dept" disabled />
          </el-form-item>
          <el-form-item label="领用人" prop="people">
            <el-input v-model.trim="updateForm.people" disabled />
          </el-form-item>
          <el-form-item label="RFID" prop="rfidCode">
            <el-input v-model.trim="updateForm.rfidCode" @focus="getInputFocus($event)">
              <i slot="suffix" class="el-input__icon el-icon-s-tools" />
            </el-input>
          </el-form-item>
        </el-form>

        <div v-if="checkOrupdate==='1'" slot="footer" class="dialog-footer">
          <el-button size="small" @click="closeUpdateDig">取消</el-button>
          <el-button size="small" type="primary" @click="sureUpdate">确认更新</el-button>
        </div>
        <div v-else slot="footer" class="dialog-footer">
          <el-button size="small" type="primary" @click="sureCheck">确认</el-button>
        </div>
      </el-dialog>
      <!--      &lt;!&ndash; 标签打印 &ndash;&gt;-->
      <!--      <el-dialog-->
      <!--        title="标签打印"-->
      <!--        :visible.sync="openPrint"-->
      <!--        width="1200px"-->
      <!--        append-to-body-->
      <!--        :close-on-click-modal="false"-->
      <!--        @close="closePrintDig"-->
      <!--      >-->
      <!--        <div>-->
      <!--          <el-button size="small" @click="previewlabel">预览标签</el-button>-->
      <!--          <img id="pvimg" name="pvimg" src="data:image/png;base64,/9j/4AAQSkZJRgABAQEBLAEsAAD/2wBDAAgGBgcGBQgHBwcJCQgKDBQNDAsLDBkSEw8UHRofHh0aHBwgJC4nICIsIxwcKDcpLDAxNDQ0Hyc5PTgyPC4zNDL/2wBDAQkJCQwLDBgNDRgyIRwhMjIyMjIyMjIyMjIyMjIyMjIyMjIyMjIyMjIyMjIyMjIyMjIyMjIyMjIyMjIyMjIyMjL/wAARCADsAk8DASIAAhEBAxEB/8QAHwAAAQUBAQEBAQEAAAAAAAAAAAECAwQFBgcICQoL/8QAtRAAAgEDAwIEAwUFBAQAAAF9AQIDAAQRBRIhMUEGE1FhByJxFDKBkaEII0KxwRVS0fAkM2JyggkKFhcYGRolJicoKSo0NTY3ODk6Q0RFRkdISUpTVFVWV1hZWmNkZWZnaGlqc3R1dnd4eXqDhIWGh4iJipKTlJWWl5iZmqKjpKWmp6ipqrKztLW2t7i5usLDxMXGx8jJytLT1NXW19jZ2uHi4+Tl5ufo6erx8vP09fb3+Pn6/8QAHwEAAwEBAQEBAQEBAQAAAAAAAAECAwQFBgcICQoL/8QAtREAAgECBAQDBAcFBAQAAQJ3AAECAxEEBSExBhJBUQdhcRMiMoEIFEKRobHBCSMzUvAVYnLRChYkNOEl8RcYGRomJygpKjU2Nzg5OkNERUZHSElKU1RVVldYWVpjZGVmZ2hpanN0dXZ3eHl6goOEhYaHiImKkpOUlZaXmJmaoqOkpaanqKmqsrO0tba3uLm6wsPExcbHyMnK0tPU1dbX2Nna4uPk5ebn6Onq8vP09fb3+Pn6/9oADAMBAAIRAxEAPwD3+iiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKACiiigAooooAKKKKAP//Z">-->
      <!--        </div>-->
      <!--        <div slot="footer" class="print-footer">-->
      <!--          <div @click="isAllprint=!isAllprint">-->
      <!--            &lt;!&ndash; <i v-if="isAllprint" class="iconfont icon-active module-radio" style="color: #4686EF;" />-->
      <!--              <i v-else class="iconfont icon-active1 module-radio" />-->
      <!--              逐条打印 &ndash;&gt;-->
      <!--          </div>-->
      <!--          <div>-->
      <!--            <el-button size="small" @click="closePrintDig">取消</el-button>-->
      <!--            <el-button size="small" type="primary" @click="print">打印</el-button>-->
      <!--          </div>-->
      <!--        </div>-->
      <!--      </el-dialog>-->
    </el-card>
    <el-dialog
      title="覆盖RFID"
      :visible.sync="openDig"
      width="30%"
      center
      :show-close="false"
    >
      <div class="checkRfid">
        <div style="margin-bottom:16px">绑定RFID：{{ oldRfid }}</div>
        <div>本次核验RFID：{{ updateForm.rfidCode }}</div>
        <div class="checkInfo">核对绑定资产不一致是否覆盖RFID</div>
      </div>

      <span slot="footer" class="dialog-footer">
        <el-button size="small" round @click="openDig = false">取 消</el-button>
        <el-button size="small" type="warning" round @click="sureUpdate">覆盖</el-button>
      </span>
    </el-dialog>
  </div>
</template>

<script>
import { getCanHandleAssetCard, getAssetDetailById, delassetCard, batchUpdateRfId } from '@/api/assetmanagement/assetsParameter'
import TagsView from '@/layout/components/TagsView/index.vue'
import AssetsDetail from '../assetsParameter/component/detail.vue'
import BindingReid from './component/binding.vue'
import '@riophae/vue-treeselect/dist/vue-treeselect.css'
import { printLabelPromise } from './print'
import { bindCardFun } from './bindCard'
import { initialPrintData, handlePrintData } from '@/utils/jri.js'
export default {
  name: 'Place',
  components: { TagsView, AssetsDetail, BindingReid },
  dicts: ['assets_enable_status'],
  data() {
    return {
      // 遮罩层加载效果显示
      loading: true,
      // 左侧搜索框关键词名称
      keywordName: '',
      // 左侧树选项data
      placeOptions: [],
      // 左侧树形组件默认参数
      defaultProps: {
        children: 'child',
        label: 'name'
      },
      // 右侧头部表单状态
      // status: [
      //   {
      //     label: '正常',
      //     value: true
      //   },
      //   {
      //     label: '停用',
      //     value: false
      //   }
      // ],
      // 选择打印标签的模板
      templateValue: '',
      // 逐条打印
      isAllprint: true,
      // 右侧头部表单查询参数
      queryParams: {
        pageNum: 1,
        pageSize: 10,
        name: undefined,
        internalCode: undefined,
        status: undefined,
        bizNo: undefined
      },
      checkOrupdate: '',
      rfidTitle: 'RFID核验',
      updateForm: {
        id: '',
        internalCode: '',
        name: '',
        brandName: '',
        standard: '',
        dept: '',
        people: '',
        rfid: ''
      },
      openDig: false,
      // 使用记录
      useRecords: [],
      // 右侧主体表格显示data
      tableData: [],
      // 右侧主体多选数组
      ids: [],
      // 右侧主体保存row信息
      rowInfo: {},
      // 右下分页选中条数
      selectedLength: 0,
      // 选中的数据
      selectedData: [],
      // 右下分页总条数
      total: 0,
      // 弹出层是否显示
      open: false,
      openDetail: false,
      openUpdate: false,
      // 弹出层标题
      title: '',

      // 详情页表单
      baseForm: {
        id: undefined,
        type: '',
        code: '',
        unit: '',
        standard: '',
        brandName: '',
        status: '',
        place: '',
        // 购置金额
        acquireDate: '',
        codeIcon: ''
      },
      oldRfid: '',
      disPrint: false, // 禁止打印按钮
      printData: initialPrintData
    }
  },
  computed: {
  },
  watch: {
  },
  created() {
    this.getList()
  },
  methods: {
    /** 批量绑定*/
    showBindDig() {
      if (this.selectedData.length === 0) {
        return this.$message.warning('请至少选择一条数据')
      }
      // 重置新增表单
      // this.resetUpdateForm()
      // 显示对话框
      this.open = true
    },
    batchBindRfId() {
      const arr = this.selectedData.map((e) => {
        return {
          id: e.id, rfidCode: e.rfidCode
        }
      })
      this.batchUpdateRfId(arr, 1)
    },
    /** 更新绑定*/
    showUpdateDig(type) {
      if (this.selectedData.length !== 1) {
        return this.$message.warning('请选择一条数据')
      }
      // 重置新增表单
      this.resetUpdateForm()
      this.updateForm = this.selectedData[0]
      this.oldRfid = this.updateForm.rfidCode
      // 显示对话框
      this.openUpdate = true
      this.checkOrupdate = type
      this.rfidTitle = type === '1' ? 'RFID更新' : 'RFID核验'
    },
    // 绑定接口
    batchUpdateRfId(arr, type) {
      const data = arr
      batchUpdateRfId(data).then(
        (response) => {
          if (response.code === 200) {
            if (type === 0) {
              this.$modal.msgSuccess('更新成功')
              this.openUpdate = false
              this.openDig = false
            } else {
              this.open = false
              this.$modal.msgSuccess('绑定成功')
            }
          } else {
            this.$message.error(response.msg)
          }
        }
      )
    },

    // 绑卡
    async bindCard(row) {
      bindCardFun(this, row, this.getList)
    },
    // 打印
    async handlePrint() {
      if (this.selectedData.length === 0) {
        return this.$message.warning('请至少选择一条数据')
      }
      this.printData.labels = handlePrintData(this.selectedData)

      printLabelPromise(this.printData)
      // 测试部分
      // const res = await printStartPromise()
      //
    },
    resetUpdateForm() {
      this.updateForm = {
        id: '',
        internalCode: '',
        name: '',
        brandName: '',
        standard: '',
        dept: '',
        people: '',
        rfid: ''
      }
    },
    /** 右侧头部搜索按钮操作 */
    handleQuery() {
      this.queryParams.pageNum = 1
      this.getList()
    },
    /** 右侧头部重置按钮操作 */
    resetQuery() {
      this.resetForm('queryForm')
      this.handleQuery()
    },
    /** 刷新 */
    refresh() {
      this.getList()
    },
    /** 右侧查询list结构 */
    getList() {
      this.tableData = []
      this.loading = true
      getCanHandleAssetCard(this.addDateRange(this.queryParams)).then(
        (response) => {
          if (response.code === 200) {
            this.loading = false
            this.tableData = response.data.records
            this.total = response.data.total
            this.$refs.tableData.clearSelection()
          } else {
            this.$message.error(response.msg)
          }
        }
      )
    },
    // 右侧主体多选框选中数据
    handleSelectionChange(selection) {
      this.selectedLength = selection.length
      this.ids = []
      this.selectedData = selection
      selection.forEach(item => {
        this.ids.push(item.id)
      })
    },
    /** 右侧主体详情和编辑操作 */
    handleDetail(row) {
      // 无论是详情还是编辑都打开对话框
      this.openDetail = true
      getAssetDetailById({ id: row.id }).then(
        (response) => {
          if (response.code === 200) {
            this.baseForm = response.data.assetCard
            this.baseForm.status = String(this.baseForm.status)
            this.useRecords = response.data.assetOperationVos
            this.$nextTick(() => {
              this.$refs.assetsDetail.getForm()
            })
          } else {
            this.$message.error(response.msg)
          }
        }
      )
    },
    /** 右侧主体删除操作 */
    handleDelete(row) {
      const { id, name } = row
      this.$modal
        .confirm('确定删除"' + name + '"?', '确定删除')
        .then(function() {
          return delassetCard(id)
        })
        .then(() => {
          this.queryParams.pageNum = this.changePageNum(this.total, 1, this.queryParams.pageNum, this.queryParams.pageSize)
          this.getList()
          this.$modal.msgSuccess('删除成功')
        })
        .catch(() => {})
    },
    /** 弹出对话框关闭 */
    closeBindDig() {
      this.open = false
    },
    /** 弹出对话框关闭 */
    closeUpdateDig() {
      this.openUpdate = false
    },
    closePrintDig() {
      this.openPrint = false
    },
    closeDetailDig() { },
    /** 弹出框显示上级位置选项 */
    showPrePlaceDialog() {
      this.isShowPlace = true
    },
    selectTemplate(value) {
      this.templateValue = value
    },
    // 获取焦点并全选
    getInputFocus(event) {
      event.currentTarget.select()
    },
    // 确认更新
    sureUpdate() {
      const arr = []
      arr.push({ id: this.updateForm.id, rfidCode: this.updateForm.rfidCode })
      this.batchUpdateRfId(arr, 0)
    },
    // 核验
    sureCheck() {
      if (this.updateForm.rfidCode === this.oldRfid) {
        this.$message({
          message: '核验一致',
          type: 'success'
        })
        this.openUpdate = false
      } else {
        this.openDig = true
      }
    },

    /** 导出 */
    handleExport() {
      this.download(
        '/biz/assetCard/export/',
        this.ids,
        `资产卡片_${this.parseTime(new Date(), '{y}-{m}-{d}')}.xlsx`
      )
    },
    /** 根据字典返回值显示状态 */
    filterStatus(data) {
      const map = new Map([
        ['0', '闲置'],
        ['1', '不可领用'],
        ['2', '退库中'],
        ['3', '在用'],
        ['4', '变卖'],
        ['5', '报废'],
        ['6', '捐赠'],
        ['7', '盘亏'],
        ['100', '其他']
      ])
      return map.get(data)
    },
    /** 根据字典返回值显示标签样式 */
    filterTagType(data) {
      const map = new Map([
        ['0', 'success'],
        ['1', 'warning'],
        ['2', 'warning'],
        ['3', 'info'],
        ['4', 'warning'],
        ['5', 'warning'],
        ['6', 'warning'],
        ['7', 'warning'],
        ['100', '']
      ])
      return map.get(data)
    }
  }
}
</script>

<style  lang="scss" scoped>
.icon-card{
  background: url('../../../assets/images/exportCard.png') no-repeat;
  background-size: cover;
  width: 24px;
  height: 24px;
  margin-top: 6px;
}
.chooseCount {
  color: #7d8592;
  margin-top: 20px;
}
.inputContent {
  display: flex;
  justify-content: space-between;
  border-bottom: 1px solid #eaedf4;
}
.statePoint .point{
  width:8px;
  height:8px;
  margin:0 8px 2px 0;
}
::v-deep .el-tree-node:focus > .el-tree-node__content {
  background-color: #fff;
}
.add-card{
  margin:20px 0 10px
}
.head-container {
  ::v-deep .el-tree-node__content {
    padding: 6px;
    height: auto;
  }
  ::v-deep
  .el-tree--highlight-current
  .el-tree-node.is-current
  > .el-tree-node__content {
    background-color: #d7e6ff;
  }
}
.updateCss{
  padding:0 50px;
  ::v-deep  .el-input {
    width: 250px;
  }
  ::v-deep .el-icon-s-tools{
    background: url('../../../assets/images/rfid.png') no-repeat;
    background-size: cover;
    width: 24px;
    height: 24px;
    margin-top: 6px;
  }
  ::v-deep .el-icon-s-tools:before{
    content: "";
    font-size: 16px;
  }

}
.info{
  margin-bottom: 30px ;
  .xing{
    color: rgb(255, 0, 0);;
  }
}
.label-size{
  margin: 24px 0 24px;
  color: #000;
}
.select-module{
  display: flex;
  justify-content: space-between;
  text-align: center;
  .module{
    width: 572px;
    height: 355px;
    box-shadow: 0px 2px 8px 0px rgba(0,0,0,0.3000);
    border-radius: 2px;
    border: 1px solid #DDDDDD;
    position: relative;
    &-radio{
      position: absolute;
      width: 16px;
      height: 16px;
      left: 16px;
      top: 16px;

    }
    &-label{
      position: absolute;
      left: 76px;
      top: 54px;
    }
  }
}
.print-footer{
  display: flex;
  justify-content: space-between;
}
.checkRfid{
  font-size: 14px;
  font-weight: 500;
  color: #7D8592
}
.checkInfo{
  color: #EF5826;
  text-align: center;
  margin: 16px 0 0;
}
.bind-class{
  color:#4686EF
}
</style>
