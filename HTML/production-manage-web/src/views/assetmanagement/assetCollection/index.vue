<template>
  <div class="post-box">
    <!-- header -->
    <tags-view>
      <el-row :gutter="10" type="flex">
        <!-- 导出 -->
        <el-col :span="1.5">
          <el-button
            v-hasPermi="['system:post:add']"
            size="small"
            @click="handleExport"
          >
            <i
              class="iconfont icon-daochu"
              style="font-size: 14px; margin-right: 8px"
            />
            <span class="btn-text">导出</span>
          </el-button>
        </el-col>
        <!-- 新增 -->
        <el-col :span="1.5">
          <el-button
            v-hasPermi="['assetManagement:assetCollection:addOrEdit']"
            type="primary"
            size="small"
            @click="handleAdd"
          >
            <i
              class="iconfont icon-zengjia"
              style="font-size: 14px; margin-right: 8px"
            />
            <span class="btn-text">新增领用</span>
          </el-button>
        </el-col>
        <!-- 刷新 -->
        <el-col :span="1.5">
          <el-button
            v-hasPermi="['system:post:add']"
            size="small"
            @click="refresh"
          >
            <el-tooltip
              class="item"
              effect="dark"
              content="刷新"
              placement="top-start"
            >
              <i class="iconfont icon-shuaxin no-margin" />
            </el-tooltip>
          </el-button>
        </el-col>
      </el-row>
    </tags-view>
    <!-- content -->
    <el-card class="main-card">
      <el-form ref="queryForm" :model="queryParams" :inline="true">
        <el-form-item label="领用单号" prop="oddNum">
          <el-input
            v-model.trim="queryParams.oddNum"
            placeholder="请输入"
            clearable
            maxlength="15"
            size="small"
            style="width: 180px"
            @keyup.enter.native="handleQuery"
          />
        </el-form-item>
        <el-form-item label="领用部门" prop="dept">
          <el-input
            v-model.trim="queryParams.dept"
            placeholder="请输入"
            clearable
            maxlength="15"
            size="small"
            style="width: 180px"
            @keyup.enter.native="handleQuery"
          />
        </el-form-item>
        <el-form-item label="领用人" prop="peopleName">
          <el-input
            v-model.trim="queryParams.peopleName"
            placeholder="请输入"
            clearable
            maxlength="15"
            size="small"
            style="width: 180px"
            @keyup.enter.native="handleQuery"
          />
        </el-form-item>
        <el-form-item label="创建时间">
          <div class="date_box">
            <el-date-picker
              v-model="getTime"
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
        <el-form-item label="状态" prop="status">
          <el-select
            v-model="queryParams.status"
            style="width: 150px"
            placeholder="请选择"
            clearable
            size="small"
          >
            <el-option
              v-for="dict in dict.type.asset_approval_status"
              :key="dict.value"
              :label="dict.label"
              :value="dict.value"
            />
          </el-select>
        </el-form-item>
        <el-form-item>
          <el-button size="small" @click="resetQuery">重置</el-button>
          <el-button
            type="primary"
            plain
            size="small"
            class="search-btn"
            @click="handleQuery"
          >搜索</el-button>
        </el-form-item>
      </el-form>
      <!-- 切换按钮 -->
      <!-- <el-row type="flex" :gutter="24">
        <el-col :span="16">
          <el-radio-group v-model="dataRange" size="small" @change="chooseBtn">
            <el-radio-button
              v-for="(item,index) in radioList"
              :key="index"
              :label="item.label"
            >{{ item.name }}</el-radio-button>
          </el-radio-group>
        </el-col>
      </el-row> -->
      <!-- table -->
      <!-- 展示table -->
      <el-table
        ref="assetTable"
        v-loading="loading"
        class="asset-collection-table"
        highlight-current-row
        :data="assetList"
        height="100%"
        @selection-change="handleSelectionChange"
      >
        <el-table-column type="selection" width="45" align="left" />
        <el-table-column
          label="序号"
          type="index"
          width="55"
          align="left"
        ><template #default="scope">
          <span>{{
            (queryParams.pageNum - 1) * queryParams.pageSize +
              scope.$index +
              1
          }}</span>
        </template></el-table-column>
        <el-table-column label="领用单号" align="left" prop="oddNum" />
        <el-table-column label="领用标题" show-overflow-tooltip align="left" prop="title" />
        <!-- <el-table-column label="资产名称" align="center" prop="assets" :show-overflow-tooltip="true" /> -->
        <!-- <el-table-column label="资产编号" align="center" prop="code" :show-overflow-tooltip="true" /> -->
        <!-- <el-table-column label="领用数量" align="left" prop="num" :show-overflow-tooltip="true" /> -->
        <el-table-column
          label="创建时间"
          align="left"
          prop="createTime"
          :show-overflow-tooltip="true"
        >
          <template slot-scope="scope">
            <span>{{ dayjsFormat(scope.row.createTime) }}</span>
          </template>
        </el-table-column>
        <el-table-column
          label="领用部门"
          align="center"
          prop="deptName"
          :show-overflow-tooltip="true"
        />
        <el-table-column
          label="领用人"
          align="center"
          prop="peopleName"
          :show-overflow-tooltip="true"
        />
        <el-table-column v-if="!show" label="状态" align="left" prop="status">
          <template slot-scope="scope">
            <el-tag :type="getStatueType(scope.row.status)" style="width: 92px;">{{
              dictionary.returnDictLabel(dict.type.asset_approval_status,scope.row.status)
            }}</el-tag>
          </template>
        </el-table-column>
        <el-table-column
          label="操作"
          width="200"
          align="left"
          class-name="small-padding"
        >
          <template slot-scope="scope">
            <el-button
              size="small"
              type="text"
              style="color: #4686ef"
              @click="handleDetail(scope.row)"
            >详情</el-button>
            <el-button
              v-if="scope.row.status === '0'"
              size="small"
              type="text"
              style="color: #4686ef"
              @click="handleUpdate(scope.row)"
            >编辑</el-button>
            <el-button
              v-if="scope.row.status === '0'"
              size="small"
              type="text"
              style="color: #ef5826"
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
    <!-- 新增/编辑弹窗 -->
    <el-dialog
      destroy-on-close
      :title="title"
      :visible.sync="open"
      :width="isEdit ? '1000px' : '1000px'"
      append-to-body
      class="addDig"
      :close-on-click-modal="false"
      @close="closeDialog"
    >
      <!-- 基本信息 -->
      <collapse title="基本信息">
        <el-form
          ref="form"
          :validate-on-rule-change="false"
          :model="form"
          :rules="isEdit ? rules : {}"
          label-width="100px"
        >
          <div>
            <div class="add-form">
              <el-form-item label="领用单号" prop="oddNum">
                <el-input
                  v-model.trim="form.oddNum"
                  size="small"
                  disabled
                  maxlength="12"
                  placeholder="系统自动生成"
                  class="input-width"
                />
              </el-form-item>
              <el-form-item label="领用标题" prop="title">
                <el-tooltip v-if="form.title&&isDisabled" :disabled="!isShowTooltip" class="item" effect="dark" :content="form.title" placement="top-start">
                  <el-input
                    ref="titleInput"
                    v-model="form.title"
                    :disabled="isDisabled"
                    size="small"
                    clearable
                    maxlength="20"
                    placeholder="请输入"
                    class="input-width overInput"
                    @mouseenter.native="visibilityChange($event)"
                  /></el-tooltip>
                <el-input
                  v-else
                  v-model="form.title"
                  :disabled="isDisabled"
                  size="small"
                  clearable
                  maxlength="20"
                  placeholder="请输入"
                  class="input-width overInput"
                />
              </el-form-item>
              <el-form-item label="创建时间" prop="getTime">
                <el-date-picker v-model="form.getTime" type="date" size="small" disabled value-format="yyyy-MM-dd" :placeholder="addTimeNow" class="input-width noIcon2" />
              </el-form-item>
            </div>
            <div class="add-form">
              <el-form-item label="领用人" prop="people">
                <div class="my-select-con">
                  <el-select
                    v-model="form.people"
                    filterable
                    :disabled="isDisabled"
                    :placeholder="isEdit?'请输入':''"
                    class="input-width"
                    @change="peopleChange"
                  >
                    <el-option
                      v-for="item in leaderOptions"
                      :key="item.userId"
                      :label="item.realName"
                      :value="item.userId"
                    />
                  </el-select>
                  <i
                    v-if="isEdit"
                    slot="suffix"
                    class="iconfont icon-people"
                    style="font-size: 14px; line-height: 36px; cursor: pointer"
                    @click.stop="showLeaderDig"
                  />
                </div>
              </el-form-item>
              <el-form-item label="领用部门" prop="dept">
                <el-input
                  v-model="form.deptName"
                  disabled
                  maxlength="15"
                  :placeholder="isEdit?'默认账号所属部门':''"
                  clearable
                  class="input-width"
                />
                <!-- <el-select
                  v-model="form.dept"
                  disabled
                  maxlength="15"
                  :placeholder="isEdit?'默认账号所属部门':''"
                  clearable
                  size="small"
                  class="input-width noIcon2"
                >
                  <el-option
                    v-for="dict in deptData"
                    :key="dict.deptId"
                    :label="dict.deptName"
                    :value="dict.deptId"
                  />
                </el-select> -->
              </el-form-item>
              <el-form-item label="领用单位" style="padding-right: 20px">
                <el-input
                  v-model="form.companyName"
                  disabled
                  maxlength="15"
                  :placeholder="isEdit?'默认账号所属单位':''"
                  clearable
                  size="small"
                  class="input-width"
                />
              </el-form-item>
            </div>
            <div class="add-form">
              <el-form-item label="存放位置" prop="placeName">
                <el-select
                  v-if="isEdit"
                  ref="selectUpResId"
                  v-model="form.placeName"
                  placeholder="请选择"
                  clearable
                  :disabled="isDisabled"
                  size="small"
                  class="input-width"
                >
                  <el-option
                    :key="form.id"
                    hidden
                    :label="form.label"
                    :value="form.id"
                  />
                  <el-tree
                    ref="treeSelect"
                    class="filter-tree"
                    :data="placeData"
                    :props="defaultProps"
                    node-key="id"
                    :expand-on-click-node="false"
                    :check-on-click-node="true"
                    :default-expanded-keys="expandedArr"
                    :filter-node-method="filterNode"
                    @node-click="handleNodeClick"
                  />
                </el-select>

                <el-input v-else v-model="form.placeName" :disabled="isDisabled" class="input-width" />
              </el-form-item></div>
            <el-form-item label="领用说明" prop="note">
              <el-input
                v-model.trim="form.note"
                type="textarea"
                :placeholder="isEdit?'请输入':''"
                :rows="2"
                :disabled="isDisabled"
                maxlength="255"
                show-word-limit
                class="input-width"
              />
            </el-form-item>
          </div>
        </el-form>
      </collapse>
      <!-- 明细信息 -->
      <collapse title="资产明细">
        <div>
          <div class="addOrRemove">
            <el-button v-if="isEdit" size="small" @click="addDetail">
              <i class="iconfont icon-zengjia" style="cursor: pointer" />
              添加明细
            </el-button>
          </div>
          <el-table
            ref="detailTable"
            style="width: 100%"
            border
            :data="tableData"
            highlight-current-row
          >
            <el-table-column
              type="index"
              label="序号"
              width="50"
              align="center"
            />
            <!-- <el-table-column prop="materialNum" label="资产编号" width="120" align="center" /> -->
            <el-table-column
              prop="materialName"
              label="资产名称"
              align="center"
            />
            <el-table-column
              prop="brandName"
              label="品牌"
              align="center"
            />
            <el-table-column
              prop="standard"
              label="规格型号"
              align="center"
            />
            <!-- <el-table-column
              prop="existNum"
              label="闲置数量"
              width="120"
              align="center"
            /> -->
            <el-table-column
              prop="unitName"
              label="计量单位"
              align="center"
            />
            <el-table-column prop="getNum" align="center">
              <template slot="header">
                <span>领用数量</span>
                <span v-if="isEdit" class="star">*</span>
              </template>
              <template slot-scope="scope">
                <el-input
                  v-if="isEdit"
                  v-model="scope.row.getNum"
                  @input="numberFilter.filterQty(scope.row,'getNum',scope.row.existNum)"
                />
                <span v-else>{{ scope.row.getNum }}</span>
              </template>
            </el-table-column>
            <el-table-column
              prop="warehouseName"
              label="存放仓库"
              align="center"
            />
            <!-- <el-table-column
              prop="note"
              label="领用说明"
              align="center"
            >
              <template slot-scope="scope">
                <el-input v-if="isEdit" v-model="scope.row.note" />
                <el-popover
                  v-else
                  placement="top-start"
                  width="200"
                  trigger="hover"
                  :content="scope.row.note"
                >
                  <span slot="reference" class="text-three-lines">{{ scope.row.note }}</span>
                </el-popover>
              </template>
            </el-table-column> -->
            <el-table-column
              v-if="isEdit"
              label="操作"
              width="100"
            >
              <template slot-scope="scope">
                <el-button
                  type="text"
                  size="small"
                  @click.native.prevent="deleteRow(scope.$index, tableData)"
                >删除</el-button>
              </template>
            </el-table-column>
          </el-table>
        </div>
      </collapse>
      <!-- 操作日志 新增/编辑时不显示-->
      <collapse v-if="!isEdit" title="审批流程">
        <!-- <ReviewProcess :id="form.id" :columns1="columns1" :columns2="columns2" /> -->
        <ApprovalProcess :id="form.id" ref="approvalProcess" type="type_ware_get" />
      </collapse>
      <!-- 紧急状态 -->
      <div class="statusContent">
        <span class="spanStatus">紧急状态</span>
        <el-radio-group v-if="isEdit" v-model="form.urgency">
          <el-radio label="0">一般</el-radio>
          <el-radio label="1">紧急</el-radio>
        </el-radio-group>
        <span v-else>{{
          form.urgency === "0" ? "一般" : form.urgency === "1" ? "紧急" : ""
        }}</span>
      </div>
      <!-- footer -->
      <div slot="footer" class="dialog-footer">
        <el-button size="small" @click="closeDialog">关闭</el-button>
        <el-button
          v-if="isEdit"
          size="small"
          @click="save(0)"
        >存草稿</el-button>
        <el-button
          v-if="isEdit"
          v-debounce
          size="small"
          type="primary"
          @click="save(1)"
        >保存</el-button>
      </div>
    </el-dialog>
    <!-- 选择领用人 -->
    <el-dialog
      title="选择领用人"
      :visible.sync="selectLeaderDig"
      width="800px"
      append-to-body
      :close-on-click-modal="false"
      destroy-on-close
      @close="closeLeaderDig"
    >
      <selectLeader ref="selectLeader" @checkedUser="getCheckUser" />
      <div slot="footer" class="dialog-footer">
        <!-- <el-button size="small" @click="clearSelected">清空已选</el-button> -->
        <el-button
          size="small"
          type="primary"
          @click="submitCheckLeader"
        >确定</el-button>
      </div>
    </el-dialog>
    <!-- 选择物品 -->
    <el-dialog
      title="选择明细"
      :visible.sync="openDetail"
      width="1050px"
      append-to-body
      :close-on-click-modal="false"
      @close="closeDetailDig"
    >
      <div class="selectLeader">
        <div class="search">
          <el-input
            v-model.trim="materialKeyWords"
            style="width: 40%"
            maxlength="15"
            placeholder="请输入资产名称或存放仓库"
            clearable
            class="inputSearch"
            @change="handleDetailSearch"
          />
          <el-button size="small" @click="resetDetailQuery">重置</el-button>
          <el-button
            size="small"
            class="search-btn"
            style="margin-left: 16px"
            @click="handleDetailSearch"
          >搜索</el-button>
        </div>
        <div>
          <el-table
            ref="table"
            v-loading="detailLoading"
            highlight-current-row
            :data="detailData"
            style="margin-top: 10px"
            max-height="500"
            :row-key="getRowKeys"
            @selection-change="handleSelectionDetail"
          >
            <el-table-column
              type="selection"
              width="50"
              :reserve-selection="true"
              align="center"
            />
            <!-- <el-table-column key="materialCode" label="资产编码" align="center" prop="materialCode" :show-overflow-tooltip="true" /> -->
            <el-table-column
              key="materialName"
              label="资产名称"
              align="center"
              prop="materialName"
              :show-overflow-tooltip="true"
            />
            <el-table-column
              key="brandName"
              label="品牌"
              align="center"
              prop="brandName"
              :show-overflow-tooltip="true"
            />
            <el-table-column
              key="standard"
              label="规格型号"
              align="center"
              prop="standard"
              :show-overflow-tooltip="true"
            />
            <el-table-column
              key="existNum"
              label="可领用数量"
              align="center"
              prop="existNum"
              :show-overflow-tooltip="true"
            />
            <el-table-column
              key="unitName"
              label="计量单位"
              align="center"
              prop="unitName"
              :show-overflow-tooltip="true"
            />
            <el-table-column
              key="warehouseName"
              label="存放仓库"
              align="center"
              prop="warehouseName"
              :show-overflow-tooltip="true"
            />
          </el-table>
        </div>
      </div>
      <div slot="footer" class="dialog-footer">
        <el-button size="small" @click="closeAddDig">取消</el-button>
        <el-button
          size="small"
          type="primary"
          @click="submitAdd"
        >保存</el-button>
      </div>
    </el-dialog>
  </div>
</template>
<script>
import { getWareGetByCondition, getDetailsById, addOrUpdateAsset, deleteWareGet } from '@/api/assetmanagement/assetCollection.js'
import { getIdleAssetList } from '@/api/warehouse/index'
import { listDept } from '@/api/system/dept'
import { getCompanyInfo, listUser } from '@/api/system/user'
import TagsView from '@/layout/components/TagsView/index.vue'
import collapse from '@/views/basicdata/material/component/collapse'
import selectLeader from '@/views/purchase/apply/component/selectLeader.vue'
import { dayjsFormat } from '@/utils/index'
import { cloneDeep } from 'lodash'
import { getPlaceTreeSelect } from '@/api/basicdata/index'
import { judeInputOver } from '@/utils/jri.js'

// import ReviewProcess from '@/components/ReviewProcess'
import ApprovalProcess from '../approvalProcess/approvalProcess.vue'
export default {
  name: 'Post',
  components: { TagsView, collapse, selectLeader, ApprovalProcess },
  dicts: ['asset_approval_status', 'general_classification'],
  data () {
    return {
      // 加载效果
      loading: true,
      detailLoading: true,
      selectedLength: 0,
      // 分页总条数
      total: 0,
      // 表格数据
      assetList: [],
      dataRange: '1', // 1全部0草稿
      // 查询表单对象
      queryParams: {
        oddNum: '',
        pageNum: 1,
        pageSize: 10,
        dept: '',
        peopleName: '',
        getTime: null,
        status: null
      },
      // 列表选中数据
      ids: [],
      // 领用时间段
      getTime: [],
      radioList: [
        { label: '1', name: '全部' },
        { label: '0', name: '草稿箱' }
      ],
      // 新增弹窗标题
      title: '',
      open: false,
      isEdit: false,
      isDisabled: false,
      isShowTooltip: false,
      // 新增弹窗表单
      form: {
        id: undefined,
        oddNum: '',
        title: '',
        getTime: '',
        company: '',
        dept: '',
        deptName: '',
        people: '',
        note: '',
        urgency: '0',
        peopleName: '',
        companyName: '',
        placeId: '',
        placeName: ''
      },
      // 表单校验
      rules: {
        title: [
          { required: true, message: '请输入标题', trigger: ['blur', 'change'] }
        ],
        company: [
          { required: true, message: '请填写单位', trigger: ['blur', 'change'] }
        ],
        people: [
          { required: true, message: '请选择领用人', trigger: ['blur', 'change'] }
        ], placeName: [
          { required: true, message: '请选择存放位置', trigger: ['blur', 'change'] }
        ]
      },
      // 选择领用人弹窗
      selectLeaderDig: false,
      selectedUserInfo: {},
      tableData: [],
      deptData: [],
      // 选择明细弹框显示隐藏
      openDetail: false,
      show: false,
      materialKeyWords: '',
      detailData: [],
      selectedData: [],
      // 树形组件默认参数
      defaultProps: {
        children: 'children',
        label: 'label'
      },
      expandedArr: [],
      columns1: [
        {
          prop: 'code',
          label: '采购单编号'
        },
        {
          prop: 'endTime',
          label: '操作时间'
        },
        {
          prop: 'taskName',
          label: '节点名称'
        },
        {
          prop: 'assigneeName',
          label: '操作者'
        },
        {
          prop: 'postName',
          label: '职位'
        },
        {
          prop: 'result',
          label: '审批结果'
        },
        {
          prop: 'message',
          label: '处理意见'
        }
      ],
      columns2: [
        {
          prop: 'operationTime',
          label: '操作时间'
        },
        {
          prop: 'operation',
          label: '节点名称'
        },
        {
          prop: 'operator',
          label: '操作者'
        },
        {
          prop: 'position',
          label: '职位'
        },
        {
          prop: 'operation',
          label: '操作'
        }
      ],
      leaderOptions: [],
      processInstanceId: undefined,
      // 新增时自动获取当前时间
      addTimeNow: null,
      placeData: []// 存放位置数组
    }
  },
  watch: {
    open: {
      immediate: false,
      handler: function(val) {
        if (val) {
          // 11
        }
      }
    }
  },
  created () {
    this.getList()
    this.getListDept()
    this.getPlaceList()
    listUser().then((response) => { this.leaderOptions = response.rows || [] })
  },
  methods: {
    /** 获取仓库列表*/
    getPlaceList () {
      getPlaceTreeSelect({}).then(response => {
        response.data.forEach(item => {
          this.expandedArr.push(item.id)
        })
        this.placeData = response.data
      })
    },
    /** 过滤 */
    filterNode(value, data) {
      if (!value) return true
      return data.deptName.indexOf(value) !== -1
    },
    /** 上级选择 */
    handleNodeClick(data) {
      this.form.placeId = data.id
      this.form.placeName = data.label
      this.$refs.selectUpResId.blur()
    },
    /** 获取部门信息 */
    getListDept () {
      listDept({}).then(response => {
        this.deptData = response.data
        console.log(this.deptData)
      })
    },
    numInput (e) {
      if (isNaN(e.getNum)) {
        e.getNum = ''
      } else if (e.getNum > e.num) {
        e.getNum = e.num
      }
    },
    /** 查询列表 */
    getList () {
      this.loading = true
      getWareGetByCondition({
        ...this.queryParams,
        startTime: this.getTime && this.getTime[0],
        endTime: this.getTime && this.getTime[1]
      }).then(res => {
        if (res.code === 200) {
          this.assetList = res.data.records
          this.total = res.data.total
          this.loading = false
        } else {
          this.$message.error(res.msg)
        }
      })
    },
    handleDetailSearch() {
      if (!this.materialKeyWords) {
        this.getIdleAssetList()
        return
      }
      // 前端取值搜索功能
      this.detailData = this.detailDataCopy.filter(item => {
        return item.materialName.includes(this.materialKeyWords) || item.warehouseName.includes(this.materialKeyWords)
      })
    },
    getIdleAssetList () {
      this.detailLoading = true
      // 获取选择物品明细
      getIdleAssetList({ pageNum: 1, pageSize: 100000 }).then(
        (response) => {
          if (response.code === 200) {
            this.detailLoading = false
            this.detailData = response.data.records.filter((e) => {
              return e.existNum !== 0
            })
            // 接受原先被搜索的数据
            this.detailDataCopy = cloneDeep(this.detailData)
          } else {
            this.detailLoading = false
            this.$message.error(response.msg)
          }
        }
      )
    },
    getCompanyInfo (id) {
      // 查询人员单位信息
      getCompanyInfo({ id }).then(
        (res) => {
          if (res.code === 200) {
            this.form.company = res.data.id
            this.form.companyName = res.data.name
          }
        }
      )
    },
    /** 导出 */
    handleExport () {
      this.download(
        'biz/wareGet/export/',
        this.ids,
        `资产领用_${this.parseTime(new Date(), '{y}-{m}-{d}')}.xlsx`
      )
    },
    /** 新增 */
    handleAdd () {
      this.title = '新增领用'
      this.isEdit = true
      this.isDisabled = false
      this.open = true
      this.addTimeNow = dayjsFormat(new Date().getTime()).split(' ')[0]
    },
    /** 刷新 */
    refresh () {
      this.getList()
    },
    /** 搜索按钮操作 */
    handleQuery () {
      if (this.form.status === '0') {
        this.dataRange = '0'
        this.show = false
      }
      this.queryParams.pageNum = 1
      this.getList()
    },
    /** 重置按钮操作 */
    resetQuery() {
      this.resetForm('queryForm')
      this.getTime = []
      this.show = false
      this.dataRange = '1'
      this.handleQuery()
    },
    chooseBtn (label) {
      if (label === '0') {
        this.queryParams.status = label
        this.show = true
      } else {
        this.queryParams.status = ''
        this.show = false
      }
      this.getList()
    },
    /** 多选框选中数据 */
    handleSelectionChange (selection) {
      this.ids = selection.map(item => item.id)
    },
    /** 根据状态取定type */
    getStatueType (value) {
      if (value === '0') {
        return 'info'
      } else if (value === '1') {
        return 'primary'
      } else if (value === '2') {
        return 'primary'
      } else if (value === '3') {
        return 'warning'
      } else if (value === '4') {
        return 'danger'
      } else if (value === '5') {
        return 'success'
      }
    },
    /** 根据状态显示文本 */
    getStatusName (value) {
      switch (value) {
        case '0':
          return '草稿'
        case '1':
          return '待审批'
        case '2':
          return '审批中'
        case '3':
          return '通过'
        case '4':
          return '不通过'
        default:
          return ''
      }
    },
    // 超出输入框展示el-tooltip提示
    visibilityChange(event) {
      const ev = event.target.firstElementChild
      this.isShowTooltip = judeInputOver(ev)
    },
    /** 查看按钮操作 */
    handleDetail (row) {
      this.title = '详情'
      this.open = true
      this.isEdit = false
      this.isDisabled = true

      this.$nextTick(() => {
        this.form = {
          ...row
        }
        const { peopleName, companyName } = row
        this.peopleName = peopleName
        this.companyName = companyName
        // 展示审批流程的数据
        this.processInstanceId = row.processInstanceId
        this.$refs.approvalProcess.processInstanceId = row.processInstanceId
        // this.$refs.approvalProcess.activeChange(0)
        if (this.processInstanceId) {
          this.$refs.approvalProcess.getList(row.processInstanceId)
        }
      })

      this.deptData.forEach(item => {
        if (item.deptId === Number(row.company)) {
          row.company = item.deptName
        }
      })
      getDetailsById(row.id).then(res => {
        if (res.code === 200) {
          this.tableData = res.data
        }
      })
    },
    /** 修改按钮操作 */
    handleUpdate (row) {
      this.title = '编辑'
      this.open = true
      this.isEdit = true
      this.isDisabled = false

      this.$nextTick(() => {
        this.form = { ...row }
      })

      this.deptData.forEach(item => {
        if (item.deptId === Number(row.company)) {
          row.company = item.deptName
        }
      })
      getDetailsById(row.id).then(res => {
        if (res.code === 200) {
          this.tableData = res.data
        }
      })
    },
    /** 删除按钮操作 */
    handleDelete (row) {
      const { id, oddNum } = row
      this.$modal.confirm('确定删除领用单"' + oddNum + '"？', '确定删除').then(function () {
        return deleteWareGet(id)
      }).then((res) => {
        if (res.code === 200) {
          this.queryParams.pageNum = this.changePageNum(this.total, 1, this.queryParams.pageNum, this.queryParams.pageSize)
          this.getList()
          this.$modal.msgSuccess('删除成功')
        } else {
          this.$message.error(res.msg)
        }
      }).catch(() => { })
    },
    /** 重置表单 */
    reset () {
      this.resetForm('form')
      // this.peopleName = ''
      this.form.companyName = ''
      this.form.urgency = '0'
      this.form.id = undefined
    },
    /** 添加明细 */
    addDetail () {
      this.materialKeyWords = ''
      this.openDetail = true
      this.getIdleAssetList()
      this.$nextTick(() => {
        this.$refs.table.clearSelection()
      })
    },
    /** 删除明细 */
    deleteRow (index, rows) {
      rows.splice(index, 1)
    },
    resetFormData () {
      this.reset()
      this.tableData = []
      this.peopleName = ''
      this.companyName = ''
      this.form.placeId = ''
      this.form.placeName = ''
      this.form.deptId = ''
      this.form.deptName = ''
      this.clearPeople()
    },
    /** 取消 */
    closeDialog () {
      this.open = false
      this.tableData = []
      // this.reset()
      this.resetFormData()
    },
    /** 弹窗确认 */
    save (index) {
      if (this.tableData.length === 0) {
        return this.$message.error('请选择领用的资产信息')
      }
      const noNum = this.tableData.some((e) => { return !e.getNum || e.getNum === '0' })
      if (noNum) {
        return this.$message.error('领用数量须大于0')
      }
      try {
        this.tableData.forEach(item => {
          if (item.getNum > item.num) {
            throw new Error('领用数量不能大于闲置数量')
          }
        })
      } catch (error) {
        this.$message.error('领用数量不能大于闲置数量')
        return
      }
      this.$refs['form'].validate(valid => {
        let isSubmit = true
        if (valid) {
          if (index === 0) {
            // 存草稿
            isSubmit = false
          }
          this.form.wareGetAssetDtos = this.tableData.map(item => {
            return {
              code: item.materialCode,
              getNum: item.getNum,
              materialId: item.materialId,
              warehouseId: item.warehouseId,
              note: item.note
            }
          })
          const data = {
            id: this.form.id,
            oddNum: this.form.oddNum,
            title: this.form.title,
            getTime: this.form.getTime,
            company: this.form.company,
            dept: this.form.dept,
            people: this.form.people,
            note: this.form.note,
            urgency: this.form.urgency,
            wareGetAssetDtos: this.form.wareGetAssetDtos,
            placeId: this.form.placeId
          }
          addOrUpdateAsset({ isSubmit }, JSON.stringify(data)).then(
            res => {
              if (res.code === 200) {
                this.$modal.msgSuccess('保存成功')
                this.resetFormData() // 重置表单
                this.open = false
                this.getList()
              } else {
                this.$message.error(res.msg)
              }
            }
          )
        }
      })
    },
    clearPeople () {
      this.form.peopleName = ''
      this.form.people = ''
    },
    /** 显示领用人弹窗 */
    showLeaderDig () {
      this.selectLeaderDig = true
    },
    getCheckUser (items) {
      console.log(items, 5555)
      this.selectedUserInfo = items[0]
    },
    peopleChange (value) {
      const obj = this.leaderOptions.find(item => item.userId === value)
      console.log(obj, 'obj')
      this.form.peopleName = obj.realName
      this.form.people = value
      this.form.dept = obj.deptId
      this.form.deptName = obj.deptName
      this.getCompanyInfo(obj.userId)
    },
    /** 关闭选择选择领用人弹窗 */
    closeLeaderDig () {
      this.selectLeaderDig = false
    },
    /** 清空选择选择领用人弹窗 */
    clearSelected () {
      this.$refs.selectLeader.$refs.rowRef.setCurrentRow()
      this.form.company = ''
      this.form.companyName = ''
      this.form.peopleName = ''
      this.form.people = ''
      this.form.dept = ''
      this.form.deptName = ''
    },
    /** 确认领用人 */
    submitCheckLeader () {
      this.selectLeaderDig = false
      this.form.peopleName = this.selectedUserInfo.realName
      this.form.people = this.selectedUserInfo.userId
      this.form.dept = this.selectedUserInfo.deptId
      // this.form.deptName = this.selectedUserInfo.deptName
      this.getCompanyInfo(this.selectedUserInfo.userId)
    },
    getRowKeys (row) {
      return row.id + Math.random()
    },
    closeDetailDig () {
      this.openDetail = false
    },
    closeAddDig () {
      this.openDetail = false
    },
    /** 搜索物品 */
    search () {
      if (this.materialKeyWords === '') {
        this.getList()
      } else {
        this.detailData = this.detailData.filter(item => { return item.materialName.toString().indexOf(this.materialKeyWords) >= 0 }
        )
      }
    },
    /** 重置搜索 */
    resetDetailQuery () {
      this.materialKeyWords = ''
      this.getIdleAssetList()
    },
    /** 确认选择物品 */
    submitAdd () {
      this.openDetail = false
      // 需要做数据合并
      this.selectedData.forEach(item => {
        const filtered = this.tableData.filter(row => row.id === item.id)[0]
        if (filtered) {
          filtered.num = item.num + filtered.num
        } else {
          this.tableData.push(item)
        }
      })
    },
    /** 选中物品 */
    handleSelectionDetail (val) {
      this.selectedData = val
    },
    dayjsFormat
  }
}
</script>
<style lang="scss" scoped>
.post-box {
  .btn-icon {
    width: 18px;
    height: 18px;
    vertical-align: middle;
  }
  .btn-text {
    vertical-align: middle;
  }
  // .common-pagination {
  //   display: flex;
  //   justify-content: flex-end;
  //   align-items: center;
  //   border-top: 1px solid #eaedf4;
  //   margin-top: 6px;
  //   .chooseCount {
  //     color: #7d8592;
  //     margin-top: 30px;
  //     margin-right: 360px;
  //   }
  // }
}
.statePoint .point {
  width: 8px;
  height: 8px;
  margin: 0 8px 2px 0;
}
.select-width {
  ::v-deep .el-input__inner {
    width: 456px;
  }
}
.noIcon {
  ::v-deep .el-input__icon {
    display: none;
  }
}
.noIcon2 {
  ::v-deep .el-input__icon.el-icon-date {
    display: none;
  }
  ::v-deep .el-input__inner {
    padding-left: 16px;
  }
  ::v-deep .el-input__suffix-inner {
      display: none;
    }
}
.selectInput {
  margin-right: 20px;
}
.detail {
  margin: 0 auto;
}
.detail-item {
  display: flex;
  font-size: 16px;
  font-weight: 400;
  line-height: 24px;
  .item-label {
    width: 128px;
    height: 16px;
    color: #6a7697;
    margin: 0 16px 16px 0;
    text-align: end;
  }
}
.logo {
  ::v-deep .el-upload--picture-card {
    width: 108px;
    height: 88px;
    display: flex;
    justify-content: center;
    align-items: center;
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
.contentTable {
  padding-top: 16px;
}
.add-form {
  display: flex;
  // justify-content:space-around;
  // flex-wrap:wrap;
  ::v-deep .el-form-item__label {
    padding-right: 10px;
  }
}
.input-width {
  ::v-deep .el-input__inner {
    width: 200px;
  }
}
.overInput{
  //设置输入框超出长度隐藏并显示省略号
::v-deep .el-input__inner {
  overflow: hidden;
  white-space: nowrap;
  text-overflow: ellipsis;
}
}
.addOrRemove {
  display: flex;
  justify-content: flex-end;
  margin-bottom: 15px;
  margin-right: 10px;
}
.statusContent {
  padding: 10px;
  .spanStatus {
    margin-right: 15px;
    color: #515a6e;
    font-weight: 500;
  }
}
.selectLeader {
  width: 100%;
  height: 100%;
  padding: 16px;
  overflow-y: scroll;
  .inputSearch{
    margin-right: 16px;
  }
}
.asset-collection-table {
  margin-top: 16px;
}
.star {
  color: #f56c6c;
  margin-left: 10px;
}
.addDig {
  ::v-deep .el-dialog .el-dialog__body {
    padding-top: 0;
    max-height: 542px;
    overflow-y: auto;
    // padding-bottom: 0;
  }
  ::v-deep .el-table--border{
    border-bottom:1px solid #EBEEF5;
  }
}
.my-select-con {
  .icon-people {
    display: inline-block;
    cursor: pointer;
    width: 22px;
    height: 22px;
    position: absolute;
    right: 6px;
    bottom: 0;
    top: 7px;
    background: url("../../../assets/images/people.png") no-repeat;
    background-position: center;
    background-size: 100% 100%;
  }
  ::v-deep .el-select {
    width: 100%;
    .el-input__suffix-inner {
      display: none;
    }
  }
}
.el-tag{
    text-align: center;
    font-size: 14px;
  }
</style>

