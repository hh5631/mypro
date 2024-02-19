<template>
  <div class="post-box">
    <tags-view>
      <el-row :gutter="10" class="mb8">
        <el-col :span="1.5">
          <el-button
            v-hasPermi="['system:user:export']"
            plain
            size="small"
            @click="handleExport"
          ><i
            style="color: #4686ef"
            class="iconfont icon-daochu"
          />导出</el-button>
        </el-col>
        <el-col :span="1.5">
          <el-button
            v-hasPermi="['system:user:add']"
            type="primary"
            size="small"
            @click="handleAdd"
          ><i
            class="iconfont icon-zengjia"
            style="color: #ffffff"
          />新增</el-button>
        </el-col>
        <!-- <el-col :span="1.5">
          <el-button
            v-hasPermi="['system:post:remove']"
            icon="el-icon-delete"
            size="small"
            :disabled="multiple"
            @click="handleDelete"
          >批量删除</el-button>
        </el-col> -->
      </el-row>
    </tags-view>
    <el-card class="main-card">
      <el-form
        v-show="showSearch"
        ref="queryForm"
        :model="queryParams"
        :inline="true"
      >
        <el-form-item label="仓库名称" prop="name">
          <el-input
            v-model.trim="queryParams.name"
            placeholder="请输入"
            clearable
            maxlength="15"
            size="small"
            style="width: 200px"
            @keyup.enter.native="handleQuery"
          />
        </el-form-item>
        <el-form-item label="仓库代码" prop="code">
          <el-input
            v-model.trim="queryParams.code"
            placeholder="请输入"
            clearable
            maxlength="15"
            size="small"
            style="width: 200px"
            @keyup.enter.native="handleQuery"
          />
        </el-form-item>
        <el-form-item label="使用状态" prop="status">
          <el-select
            v-model="queryParams.status"
            style="width: 200px"
            placeholder="请选择"
            clearable
            size="small"
          >
            <el-option
              v-for="dict in dict.type.operation_status"
              :key="dict.value"
              :label="dict.label"
              :value="dict.value"
            />
          </el-select>
        </el-form-item>
        <!-- <el-form-item label="状态" prop="enable">
          <el-select v-model="queryParams.enable" style="width: 200px" placeholder="请选择" clearable size="small">
            <el-option
              v-for="dict in dict.type.sys_normal_disable"
              :key="dict.value"
              :label="dict.label"
              :value="dict.value"
            />
          </el-select>
        </el-form-item> -->
        <el-form-item>
          <el-button size="small" @click="resetQuery">重置</el-button>
          <el-button
            type="primary"
            class="search-btn"
            plain
            size="small"
            @click="handleQuery"
          >搜索</el-button>
        </el-form-item>
      </el-form>
      <el-table
        ref="warehouseTable"
        v-loading="loading"
        highlight-current-row
        :data="listData"
        height="100%"
        @selection-change="handleSelectionChange"
      >
        <el-table-column type="selection" width="55" align="left" />
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
        <el-table-column
          label="仓库名称"
          align="left"
          prop="name"
          :show-overflow-tooltip="true"
        />
        <el-table-column
          label="仓库代码"
          align="left"
          prop="code"
          :show-overflow-tooltip="true"
        />
        <el-table-column label="使用状态" align="left" prop="status">
          <template slot-scope="scope">
            <div class="statePoint">
              <span
                v-if="scope.row.status === '0'"
                style="color: #3db954"
              >在用</span>
              <span v-else style="color: #ef5826">闲置</span>
            </div>
          </template>
        </el-table-column>
        <el-table-column label="仓库用途" align="left" prop="purpose">
          <template slot-scope="scope">
            <div>
              <span v-if="scope.row.purpose === '0'">资产仓</span>
              <span v-if="scope.row.purpose === '1'">物资仓</span>
              <span v-if="scope.row.purpose === '2'">成品仓</span>
              <span v-if="scope.row.purpose === '3'">原料仓</span>
              <span v-if="scope.row.purpose === '4'">辅料仓</span>
            </div>
          </template>
        </el-table-column>
        <el-table-column label="仓库类型" align="left" prop="type" />
        <!-- <el-table-column label="状态" align="left" prop="enable">
          <template slot-scope="scope">
            <div>
              <el-switch
                v-model="scope.row.enable"
                active-value="0"
                inactive-value="1"
                @change="changeState($event,scope.row)"
              />
            </div>
          </template>
        </el-table-column> -->
        <!-- <el-table-column label="创建时间" align="center" prop="createTime" width="180">
          <template slot-scope="scope">
            <span>{{ parseTime(scope.row.createTime) }}</span>
          </template>
        </el-table-column> -->
        <el-table-column
          label="操作"
          width="200"
          align="left"
          class-name="small-padding"
        >
          <template slot-scope="scope">
            <el-button
              v-hasPermi="['system:post:edit']"
              size="small"
              type="text"
              style="color: #4686ef"
              @click="handleDetail(scope.row)"
            >详情</el-button>
            <el-button
              v-hasPermi="['system:post:edit']"
              size="small"
              type="text"
              style="color: #4686ef"
              @click="handleUpdate(scope.row)"
            >编辑</el-button>
            <el-button
              v-if="scope.row.status === '1'"
              v-hasPermi="['system:post:remove']"
              size="small"
              type="text"
              style="color: #ef5826"
              @click="handleDelete(scope.row)"
            >删除</el-button>
          </template>
        </el-table-column>
      </el-table>
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
      <!-- 品牌详情弹框内容 -->
      <el-dialog
        :close-on-click-modal="false"
        title="仓库详情"
        :visible.sync="openDetail"
        width="600px"
        append-to-body
        class="post-dialog"
      >
        <div class="detail">
          <div class="detail-item">
            <div class="item-label">仓库名称</div>
            <div class="item-text">{{ form.name }}</div>
          </div>
          <div class="detail-item">
            <div class="item-label">显示顺序</div>
            <div class="item-text">{{ form.sort }}</div>
          </div>
          <div class="detail-item">
            <div class="item-label">仓库类型</div>
            <div class="item-text">
              <span v-if="form.type === '0'">普通</span>
              <span v-if="form.type === '1'">冷库</span>
              <span v-if="form.type === '2'">特种</span>
            </div>
          </div>
          <div class="detail-item">
            <div class="item-label">联系电话</div>
            <div class="item-text">{{ form.contactNumber }}</div>
          </div>
          <div class="detail-item">
            <div class="item-label">产权单位</div>
            <div class="item-text">{{ form.propertyRightUnit }}</div>
          </div>
          <div class="detail-item">
            <div class="item-label">所属城市</div>
            <div class="item-text">{{ form.city }}</div>
          </div>
          <!-- <div class="detail-item">
            <div class="item-label">使用状态</div>
            <div class="item-text">
              {{ form.status === "0" ? "在用" : "闲置" }}
            </div>
          </div> -->
          <div class="detail-item">
            <div class="item-label">仓库代码</div>
            <div class="item-text">{{ form.code }}</div>
          </div>
          <div class="detail-item">
            <div class="item-label">仓库用途</div>
            <div class="item-text">
              {{ dictionary.returnDictLabel(dict.type.warehouse_classification,form.purpose) }}
              <!-- <span v-if="form.purpose === '1'">资产仓</span>
              <span v-if="form.purpose === '2'">物资仓</span>
              <span v-if="form.purpose === '3'">成品仓</span>
              <span v-if="form.purpose === '4'">原料仓</span>
              <span v-if="form.purpose === '5'">辅料仓</span> -->
            </div>
          </div>
          <div class="detail-item">
            <div class="item-label">管理员</div>
            <div class="item-text">{{ form.storeMan }}</div>
          </div>
          <div class="detail-item">
            <div class="item-label">使用单位</div>
            <div class="item-text">{{ form.userUnit }}</div>
          </div>
          <div class="detail-item">
            <div class="item-label">仓库地址</div>
            <div class="item-text">{{ form.address }}</div>
          </div>
          <div class="detail-item">
            <div class="item-label">占地面积</div>
            <div class="item-text">{{ form.coveredArea }}</div>
          </div>
          <!-- <div class="detail-item">
            <div class="item-label">状态</div>
            <div class="item-text">
              {{ form.enable === "0" ? "正常" : "停用" }}
            </div>
          </div> -->
          <div class="detail-item">
            <div class="item-label">备注</div>
            <div
              class="item-text"
              style="width: 400px; height: 50px; overflow-y: scroll"
            >
              {{ form.remark }}
            </div>
          </div>
        </div>
      </el-dialog>
      <!-- 添加或修改岗位对话框 -->
      <el-dialog
        :close-on-click-modal="false"
        :title="title"
        :visible.sync="open"
        width="1200px"
        append-to-body
        class="post-dialog"
      >
        <div class="add-body">
          <el-form
            ref="form"
            :model="form"
            :rules="rules"
            label-width="300px"
            label-position="top"
          >
            <div class="add-form">
              <div>
                <el-form-item label="仓库名称" prop="name">
                  <el-input
                    v-model="form.name"
                    maxlength="15"
                    placeholder="请输入"
                    class="input-width"
                    @change="trimFun('name', $event)"
                  />
                </el-form-item>
                <el-form-item label="显示顺序" prop="sort">
                  <el-input
                    v-model.number="form.sort"
                    maxlength="5"
                    placeholder="请输入"
                  />
                </el-form-item>
                <el-form-item label="仓库类型" prop="type">
                  <el-select
                    v-model="form.type"
                    style="width: 500px"
                    placeholder="请选择"
                    clearable
                    size="small"
                  >
                    <el-option
                      v-for="dict in dict.type.warehouse_type"
                      :key="dict.value"
                      :label="dict.label"
                      :value="dict.value"
                    />
                  </el-select>
                </el-form-item>
                <el-form-item label="联系电话" prop="contactNumber">
                  <el-input
                    v-model="form.contactNumber"
                    class="input-width"
                    placeholder="请输入"
                    @change="trimFun('contactNumber', $event)"
                  />
                </el-form-item>
                <el-form-item label="产权单位" prop="propertyRightUnit">
                  <el-input
                    v-model="form.propertyRightUnit"
                    class="input-width"
                    placeholder="请输入"
                    maxlength="50"
                    @change="trimFun('propertyRightUnit', $event)"
                  />
                </el-form-item>
                <el-form-item label="所属城市" prop="city">
                  <el-input
                    v-model="form.city"
                    class="input-width"
                    placeholder="请输入"
                    maxlength="15"
                    @change="trimFun('city', $event)"
                  />
                </el-form-item>
                <!-- <el-form-item label="状态" prop="status">
                  <el-radio-group v-model="form.enable">
                    <el-radio
                      v-for="dict in dict.type.sys_normal_disable"
                      :key="dict.value"
                      :label="dict.value"
                    >{{ dict.label }}</el-radio>
                  </el-radio-group>

                </el-form-item> -->
                <!-- <el-form-item label="使用状态" prop="status">
                  <el-radio-group v-model="form.status">
                    <el-radio
                      v-for="dict in dict.type.operation_status"
                      :key="dict.value"
                      :label="dict.value"
                    >{{ dict.label }}</el-radio>
                  </el-radio-group>
                </el-form-item> -->
              </div>
              <div>
                <el-form-item label="仓库代码" prop="code">
                  <el-input
                    v-model="form.code"
                    class="input-width"
                    maxlength="15"
                    placeholder="请输入"
                    @change="trimFun('code', $event)"
                  />
                </el-form-item>
                <el-form-item label="仓库用途" prop="purpose">
                  <el-select
                    v-model="form.purpose"
                    style="width: 500px"
                    placeholder="请选择"
                    clearable
                    size="small"
                    @change="formValidate()"
                  >
                    <el-option
                      v-for="dict in dict.type.warehouse_classification"
                      :key="dict.value"
                      :label="dict.label"
                      :value="dict.value"
                    />
                  </el-select>
                </el-form-item>
                <el-form-item label="管理员" prop="storeMan">
                  <!-- <el-input
                    v-model="form.storeMan"
                    readonly
                    placeholder="请选择"
                    class="input-width"
                    @change="trimFun('storeMan', $event)"
                  >
                    <i
                      slot="suffix"
                      class="iconfont icon-jiaoseshezhi"
                      style="line-height: 36px; cursor: pointer"
                      @click="showLeaderDig"
                    />
                  </el-input> -->
                  <div class="my-select-con">
                    <el-select
                      v-model="form.storeMan"
                      filterable
                      placeholder="请输入"
                      class="input-width"
                    >
                      <el-option
                        v-for="item in userOptions"
                        :key="item.userId"
                        :label="item.realName"
                        :value="item.realName"
                      />
                    </el-select>
                    <i slot="suffix" class="iconfont icon-jiaoseshezhi" style="font-size: 14px;line-height: 36px;cursor: pointer;" @click.stop="showLeaderDig" />
                  </div>
                </el-form-item>
                <el-form-item label="使用单位" prop="userUnit">
                  <el-input
                    v-model="form.userUnit"
                    class="input-width"
                    maxlength="50"
                    placeholder="请输入"
                    @change="trimFun('userUnit', $event)"
                  />
                </el-form-item>
                <el-form-item label="仓库地址" prop="address">
                  <el-input
                    v-model="form.address"
                    class="input-width"
                    maxlength="50"
                    type="text"
                    placeholder="请输入"
                    @change="trimFun('address', $event)"
                  />
                </el-form-item>
                <el-form-item label="占地面积" prop="coveredArea">
                  <el-input
                    v-model.number="form.coveredArea"
                    class="input-width"
                    maxlength="11"
                    placeholder="请输入"
                    oninput="value=value.replace(' ','')"
                  />
                </el-form-item>
              </div>
            </div>

            <el-form-item label="备注" prop="remark">
              <el-input
                v-model="form.remark"
                type="textarea"
                placeholder="请输入"
                maxlength="500"
                @change="trimFun('remark', $event)"
              />
            </el-form-item>
          </el-form>
        </div>

        <div slot="footer" class="dialog-footer">
          <el-button size="small" @click="cancel">取 消</el-button>
          <el-button
            size="small"
            type="primary"
            @click="submitForm"
          >保 存</el-button>
        </div>
      </el-dialog>
    </el-card>
    <!-- 选择管理员 -->
    <el-dialog
      title="选择管理员"
      :visible.sync="selectLeaderDig"
      width="800px"
      append-to-body
      :close-on-click-modal="false"
      class="selectPlan"
      @close="closeLeaderDig"
    ><selectLeader ref="selectLeader" @checkedUser="getCheckUser" />
      <div slot="footer" class="dialog-footer">
        <el-button size="small" @click="clearSelected">清空已选</el-button>
        <el-button
          size="small"
          type="primary"
          @click="submitCheckLeader"
        >保存</el-button>
      </div></el-dialog>
  </div>
</template>

<script>
import { listWarehouse, getWarehouse, saveOrUpdate, delWarehouse, warehouseStop } from '@/api/basicdata/warehouse'
import selectLeader from '@/views/purchase/apply/component/selectLeader.vue'
import TagsView from '@/layout/components/TagsView/index.vue'
import { myTrim } from '@/utils/index'
import FormValidators from '../../../vform/utils/validators'
import { listUser } from '@/api/system/user'
export default {
  name: 'Warehouse',
  components: { TagsView, selectLeader },
  dicts: ['sys_normal_disable', 'warehouse_type', 'storekeeper', 'warehouse_classification', 'operation_status'],
  data () {
    return {
      selectLeaderDig: false,
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
      // 是否显示详情弹出层
      openDetail: false,
      listLength: '',
      // 查询参数
      queryParams: {
        pageNum: 1,
        pageSize: 10,
        code: undefined,
        name: undefined,
        status: undefined,
        enable: undefined
      },
      // 表单参数
      form: {},
      // 表单校验
      rules: {
        name: [
          { required: true, message: '请填写仓库名称', trigger: 'blur' },
          {
            validator: function (rule, value, callback) {
              if (/^[\u4e00-\u9fa5a-zA-Z]+$/.test(value) === false) {
                callback(new Error('请输入汉字或英文'))
              } else {
                // 校验通过
                callback()
              }
            },
            trigger: ['blur', 'change']
          }
        ],
        sort: [
          { required: true, message: '显示顺序不能为空', trigger: 'blur' },
          { type: 'number', message: '显示顺序必须为大于1的数字' },
          {
            validator: function (rule, value, callback) {
              if (value < 1) {
                callback(new Error('显示顺序必须为大于1的数字'))
              } else {
                // 校验通过
                callback()
              }
            },
            trigger: ['blur', 'change']
          }
        ],
        contactNumber: [
          // {  message: '联系电话长度在11个字符', trigger: 'blur' }
          { validator: FormValidators.allPhone, message: '请输入正确的号码', trigger: 'blur' }
        ],
        code: [
          { required: true, message: '请填写仓库代码', trigger: 'blur' },
          { min: 3, message: '仓库代码长度最少为3位', trigger: 'blur' },
          {
            validator: function (rule, value, callback) {
              if (/^[a-zA-Z0-9]+$/.test(value) === false) {
                callback(new Error('请输入数字或英文'))
              } else {
                // 校验通过
                callback()
              }
            },
            trigger: ['blur', 'change']
          }
          // {
          //   validator: function(rule, value, callback) {
          //     judgeExist(value).then(res => {
          //       if (res.data) {
          //         callback(new Error('仓库代码不能重复'))
          //       } else {
          //         callback()
          //       }
          //     })
          //   },
          //   trigger: 'blur'
          // }
        ],
        purpose: [
          { required: true, message: '请选择仓库用途', trigger: ['blur', 'change'] }
        ],
        storeMan: [{ required: true, message: '请选择管理员', trigger: ['blur', 'change'] }],
        userUnit: [
          { required: true, message: '请填写使用单位', trigger: 'blur' },
          {
            validator: function (rule, value, callback) {
              if (/^[\u4e00-\u9fa5]+$/.test(value) === false) {
                callback(new Error('请输入汉字'))
              } else {
                // 校验通过
                callback()
              }
            },
            trigger: ['blur', 'change']
          }
        ],
        coveredArea: [
          // { type: 'number', message: '占地面积必须为大于1的数字' },
          {
            validator: function (rule, value, callback) {
              if (value === 0) {
                callback(new Error('占地面积必须为大于1的数字'))
              } else if ((value && value < 1)) {
                callback(new Error('占地面积必须为大于1的数字'))
              } else {
                // 校验通过
                callback()
              }
            },
            trigger: ['blur', 'change']
          }
        ],
        address: [
          { required: true, message: '请填写仓库地址', trigger: ['blur', 'change'] }
        ],
        city: [{
          validator: function (rule, value, callback) {
            if (/^[\u4e00-\u9fa5a-zA-Z]+$/.test(value) === false) {
              callback(new Error('请输入汉字或英文'))
            } else {
              // 校验通过
              callback()
            }
          },
          trigger: ['blur', 'change']
        }],
        // status: [{ required: true, message: '请选择使用状态', trigger: 'blur' }],
        // enable: [{ required: true, message: '请选择状态', trigger: 'blur' }],
        remark: [

        ]
      },
      rowInfo: {},
      isStatus: false,
      selectionName: [],
      warehouseType: [], // 仓库类型
      peopleInfo: {},
      // 所有用户
      userOptions: {}
    }
  },
  async created () {
    await this.getDicts('warehouse_type').then(response => {
      this.warehouseType = response.data
    })
    await this.getList()
  },
  methods: {
    /** 查询列表 */
    getList () {
      this.loading = true
      listWarehouse(this.queryParams).then(response => {
        if (response.code === 200) {
          this.listData = response.data.records || []
          this.listLength = response.data.total + 1
          this.total = response.data.total
          this.loading = false
          this.$refs.warehouseTable.setCurrentRow(this.listData.filter(item => item.id === this.rowInfo.id)[0])
          this.listData.map(item => {
            const filtered = this.warehouseType.filter(item1 => item1.dictValue === item.type)
            if (filtered[0]) {
              item.type = filtered[0].dictLabel
            }
          })
        } else {
          this.loading = false
          this.$message.error(response.msg)
        }
      })
      // 获取所有用户
      listUser().then(res => {
        this.userOptions = res.rows
      })
    },
    formValidate() {
      // 不知为何此处要校验表单，有Bug指出，先注释
      // this.$refs['form'].validate()
    },
    trimFun (str, val) {
      this.form[str] = myTrim(val)
    },
    // 取消按钮
    cancel () {
      this.open = false
      this.reset()
    },
    // 表单重置
    reset () {
      this.form = {
        id: undefined,
        name: undefined,
        sort: undefined, // 显示顺序
        type: undefined,
        contactNumber: undefined,
        propertyRightUnit: undefined,
        city: undefined,
        // status: '0',
        code: undefined,
        purpose: undefined,
        storeMan: undefined,
        userUnit: undefined,
        address: undefined,
        coveredArea: undefined, // 占地面积
        // enable: '0',
        remark: undefined
      }
      this.resetForm('form')
    },
    /** 搜索按钮操作 */
    handleQuery () {
      this.queryParams.pageNum = 1
      this.getList()
    },
    /** 重置按钮操作 */
    resetQuery () {
      this.resetForm('queryForm')
      this.handleQuery()
    },
    // 多选框选中数据
    handleSelectionChange (selection) {
      this.selectedLength = selection.length
      this.ids = selection.map(item => item.id)
      this.selectionName = selection.map(item => item.name)
      this.single = selection.length !== 1
      this.multiple = !selection.length
    },
    /** 查看按钮操作 */
    handleDetail (row) {
      getWarehouse(row.id).then(response => {
        this.form = response.data
        this.openDetail = true
      })
    },
    /** 新增按钮操作 */
    handleAdd () {
      this.reset()
      this.open = true
      this.form.sort = this.listLength
      this.title = '新建仓库配置'
    },
    /** 修改按钮操作 */
    handleUpdate (row) {
      this.reset()
      this.rowInfo = row
      const id = row.id || this.ids
      getWarehouse(id).then(response => {
        if (response.code === 200) {
          this.form = response.data
          this.isStatus = response.data.status === '0'
          this.open = true
          this.title = '编辑仓库配置'
        } else {
          this.$message.error(response.msg)
        }
      })
    },
    /** 提交按钮 */
    submitForm: function () {
      this.$refs['form'].validate(valid => {
        if (valid) {
          saveOrUpdate(this.form).then(res => {
            if (res.code === 200) {
              this.$modal.msgSuccess('保存成功')
              this.open = false
              this.resetForm('queryForm')
              this.getList()
            } else {
              this.$message.error(res.msg)
            }
          })
          // if (this.form.postId !== undefined) {
          //   saveOrUpdate(this.form).then(response => {
          //     if (this.form.status === '1' && this.isStatus === true) {
          //       this.$confirm('确认停用仓库' + '"' + this.form.postName + '"' + '?', '确认停用', {
          //         confirmButtonText: '确定',
          //         cancelButtonText: '取消',
          //         type: 'warning'
          //       }).then(() => {
          //         this.$modal.msgSuccess('修改成功')
          //         this.open = false
          //         this.resetForm('queryForm')
          //         this.getList()
          //       }).catch(() => {
          //       })
          //     } else {
          //       this.$modal.msgSuccess('修改成功')
          //       this.open = false
          //       this.resetForm('queryForm')
          //       this.getList()
          //     }
          //   })
          // } else {
          //   saveOrUpdate(this.form).then(response => {
          //     this.$modal.msgSuccess('保存成功')
          //     this.open = false
          //     this.resetForm('queryForm')
          //     this.getList()
          //   })
          // }
        }
      })
    },
    /** 删除按钮操作 */
    handleDelete (row) {
      const warehouseIds = row.id || this.ids
      const warehouseName = row.name || this.selectionName
      this.$modal.confirm('确定删除仓库"' + warehouseName + '"？', '确定删除').then(function () {
        return delWarehouse(warehouseIds)
      }).then(() => {
        // 如果删除之后当前数据长度为1则this.queryParams页数少1
        if (this.listData.length === 1 && this.queryParams.pageNum !== 1) {
          this.queryParams.pageNum = this.queryParams.pageNum - 1
        }
        this.getList()
        this.$modal.msgSuccess('删除成功')
      }).catch(() => { })
    },
    /** 导出按钮操作 */
    handleExport () {
      this.download(
        'system/warehouse/export/',
        this.ids,
        `仓库_${this.parseTime(new Date(), '{y}-{m}-{d}')}.xlsx`
      )
      // const idss = []
      // if (this.ids.length === 0) {
      //   // this.listData.forEach(item => {
      //   //   idss.push(item.id)
      //   // })
      //   // this.download(
      //   //   'system/warehouse/export/' + idss.toString(),
      //   //   { },
      //   //   `warehouse_${this.parseTime(new Date(), '{y}-{m}-{d}')}.xlsx`
      //   // )
      //   // this.$message.error('请选择需要导出的内容')
      //   // return
      //   this.listData.forEach(item => {
      //     this.ids.push(item.id)
      //   })
      // }
      // this.download(
      //   'system/warehouse/export/' + this.ids.toString(),
      //   { },
      //   `warehouse_${this.parseTime(new Date(), '{y}-{m}-{d}')}.xlsx`
      // )
    },
    // switch改变状态
    changeState (el, row) {
      warehouseStop(row.id, el).then((res) => {
        if (res.code === 200) {
          this.$message.success('修改成功')
          this.getList()
        }
      })
    },
    /** 选择管理员 */
    closeLeaderDig () {
      this.selectLeaderDig = false
      this.$refs.selectLeader.$refs.rowRef.setCurrentRow('')
      this.$refs.selectLeader.$refs.dept.setCurrentKey()
    },
    getCheckUser (items) {
      // this.form.storeMan = items[0].realName
      this.peopleInfo = items
    },
    showLeaderDig () {
      this.selectLeaderDig = true
    },
    clearSelected () {
      this.$refs.selectLeader.$refs.rowRef.setCurrentRow('')
      this.$refs.selectLeader.$refs.dept.setCurrentKey()
    },
    submitCheckLeader () {
      this.form.storeMan = this.peopleInfo[0].realName
      this.selectLeaderDig = false
    }
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
  // .chooseCount {
  //   color: #7d8592;
  //   margin-top: 20px;
  // }
  // .common-pagination {
  //   display: flex;
  //   justify-content: space-between;
  //   align-items: center;
  //   border-top: 1px solid #eaedf4;
  //   margin-top: 6px;
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
.detail {
  margin: 0 auto;
}
// .detail-item{
//   display: flex;
//   .item-label{
// width: 128px;
// height: 16px;
// font-size: 16px;
// font-weight: 400;
// color: #6A7697;
// line-height: 24px;
// margin:0 16px 16px 0 ;
// text-align: end;
//   }
//   .item-text{
//     width: 500px;
//   }
// }
.detail-item {
  display: flex;
  font-size: 16px;
  font-weight: 400;
  line-height: 24px;
  margin-bottom: 30px;
  height: 22px;
  .item-label {
    width: 128px;
    height: 16px;
    color: #6a7697;
    margin: 0 16px 16px 0;
    text-align: end;
  }
  .item-text {
    width: 416px;
  }
}
.add-form {
  display: flex;
  justify-content: space-between;
}
.input-width {
  ::v-deep .el-input__inner {
    width: 500px;
  }
}
.add-body {
  // 必须有高度 overflow 为自动
  overflow: auto;
  height: 680px;
  padding: 0px 30px 11px 27px;

  // 滚动条的样式,宽高分别对应横竖滚动条的尺寸
  &::-webkit-scrollbar {
    width: 3px;
  }

  // 滚动条里面默认的小方块,自定义样式
  &::-webkit-scrollbar-thumb {
    background: #8798af;
    border-radius: 2px;
  }

  // 滚动条里面的轨道
  &::-webkit-scrollbar-track {
    background: transparent;
  }
}
.my-select-con{
  .iconfont {
    display: inline-block;
    cursor: pointer;
    width: 22px;
    height: 22px;
    position: absolute;
    right: 6px;
    bottom: 0;
  }
  .icon-jiaoseshezhi {
    top:0;
  }
  ::v-deep .el-select {
    width: 100%;
    .el-input__suffix-inner {
      display: none;
    }
  }
}
</style>

