<template>
  <div class="post-box">
    <tags-view>
      <el-row :gutter="10" class="mb8">
        <el-col :span="1.5">
          <el-button
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
        <el-form-item label="中文名称" prop="chineseName">
          <el-input
            v-model.trim="queryParams.chineseName"
            placeholder="请输入"
            clearable
            maxlength="15"
            size="small"
            style="width: 200px"
            @keyup.enter.native="handleQuery"
          />
        </el-form-item>
        <el-form-item label="英文名称" prop="englishName">
          <el-input
            v-model.trim="queryParams.englishName"
            placeholder="请输入"
            clearable
            maxlength="15"
            size="small"
            style="width: 200px"
            @keyup.enter.native="handleQuery"
          />
        </el-form-item>
        <!-- <el-form-item label="状态" prop="enable">
          <el-select
            v-model="queryParams.enable"
            style="width: 200px"
            placeholder="请选择"
            clearable
            size="small"
          >
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
          <el-button type="primary" class="search-btn" plain size="small" @click="handleQuery">搜索</el-button>
        </el-form-item>
      </el-form>
      <el-table
        ref="brandTable"
        v-loading="loading"
        highlight-current-row
        :data="brandList"
        :row-style="{ height: '48px' }"
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
          label="中文名称"
          align="left"
          prop="chineseName"
          :show-overflow-tooltip="true"
        />
        <el-table-column
          label="英文名称"
          align="left"
          prop="englishName"
          :show-overflow-tooltip="true"
        />
        <el-table-column
          label="品牌代码"
          align="left"
          prop="code"
          :show-overflow-tooltip="true"
        />
        <el-table-column
          label="常见类目"
          align="left"
          prop="type"
        >
          <template slot-scope="scope">
            <div>
              {{ typeObj[scope.row.type] || "" }}
            </div>
          </template>
        </el-table-column>
        <el-table-column
          label="品牌LOGO"
          align="left"
          prop="icon"
          :show-overflow-tooltip="true"
        >
          <template slot-scope="scope">
            <div style="height: 40px">
              <img
                v-if="scope.row.icon"
                :src="`${picUrl}${scope.row.icon}`"
                alt=""
                width="64"
                height="40"
              >
            </div>
          </template>
        </el-table-column>
        <!-- <el-table-column label="状态" width="380" align="left" prop="enable">
          <template slot-scope="scope">
            <div>
              <el-switch
                v-model="scope.row.enable"
                active-value="0"
                inactive-value="1"
                @change="changeState($event, scope.row)"
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
        title="品牌详情"
        :visible.sync="openDetail"
        width="600px"
        height="627px"
        append-to-body

        class="post-dialog"
        @close="closeDetail"
      >
        <div class="detail">
          <div class="detail-item">
            <div class="item-label">中文名称</div>
            <div class="item-text">{{ rowInfo.chineseName }}</div>
          </div>
          <div class="detail-item">
            <div class="item-label">英文名称</div>
            <div class="item-text">{{ rowInfo.englishName }}</div>
          </div>
          <div class="detail-item">
            <div class="item-label">品牌代码</div>
            <div class="item-text">{{ rowInfo.code }}</div>
          </div>
          <div class="detail-item">
            <div class="item-label">显示顺序</div>
            <div class="item-text">{{ rowInfo.sort }}</div>
          </div>

          <div class="detail-item">
            <div class="item-label">属性</div>
            <div class="item-text">
              {{ rowInfo.attribute === "0" ? "国产" : "进口" }}
            </div>
          </div>
          <div class="detail-item">
            <div class="item-label">常见类目</div>
            <div class="item-text">{{ rowInfo.type }}</div>
          </div>
          <div class="detail-item item-logo">
            <div class="item-label">品牌LOGO</div>
            <img
              v-if="rowInfo.icon"
              :src="`${picUrl}${rowInfo.icon}`"
              alt=""
              width="108px"
              height="88px"
            >
          </div>
          <div class="detail-item">
            <div class="item-label">品牌简称</div>
            <div class="item-text">{{ rowInfo.abbreviation }}</div>
          </div>
          <div class="detail-note">
            <div class="label-note">备注</div>
            <div class="item-note">{{ rowInfo.note }}</div>
          </div>
        </div>
      </el-dialog>
      <!-- 添加或修改岗位对话框 -->
      <el-dialog
        :close-on-click-modal="false"
        :title="title"
        :visible.sync="open"
        width="800px"
        append-to-body
        class="post-dialog"
      >
        <el-form
          ref="form"
          :model="form"
          :rules="rules"
          label-width="193px"
          style="margin-right: 111px"
        >
          <el-form-item label="品牌名称（中文）" prop="chineseName">
            <el-input
              v-model="form.chineseName"
              maxlength="15"
              placeholder="请输入"
              @change="trimFun('chineseName', $event)"
              @blur="$refs.form.validateField('englishName')"
            />
          </el-form-item>
          <el-form-item label="品牌名称（英文）" prop="englishName">
            <el-input
              v-model="form.englishName"
              maxlength="15"
              placeholder="请输入"
              @change="trimFun('englishName', $event)"
              @blur="$refs.form.validateField('chineseName')"
            />
          </el-form-item>
          <el-form-item label="品牌代码" prop="code">
            <el-input
              v-model.trim="form.code"
              maxlength="15"
              placeholder="请输入"
            />
          </el-form-item>
          <el-form-item label="显示顺序" prop="sort">
            <el-input v-model="form.sort" v-trim.positiveInt="form.sort" maxlength="5" type="text" placeholder="请输入" />
          </el-form-item>
          <!-- <el-form-item label="状态">
            <el-radio-group v-model="form.enable">
              <el-radio
                v-for="dict in dict.type.sys_normal_disable"
                :key="dict.value"
                :label="dict.value"
              >{{ dict.label }}</el-radio>
            </el-radio-group>
          </el-form-item> -->
          <el-form-item label="属性">
            <el-radio-group v-model="form.attribute">
              <el-radio
                v-for="dict in dict.type.sys_brand_property"
                :key="dict.value"
                :label="dict.value"
              >{{ dict.label }}</el-radio>
            </el-radio-group>
          </el-form-item>
          <el-form-item label="常见类目">
            <el-select
              v-model="form.type"
              placeholder="请选择"
              class="select-width"
            >
              <el-option
                v-for="dict in dict.type.general_classification"
                :key="dict.value"
                :label="dict.label"
                :value="dict.value"
              />
            </el-select>
          </el-form-item>
          <el-form-item label="品牌LOGO" prop="icon">
            <el-upload
              accept=".png, .jpg, .jpeg"
              class="logo"
              :multiple="false"
              list-type="picture-card"
              :show-file-list="false"
              :headers="headers"
              :action="uploadUrl"
              :before-upload="handleBeforeUpload"
              :on-success="handleUploadSuccess"
              :on-error="handleUploadError"
            >
              <div v-if="form.icon" class="logo-icon">
                <img class="del-icon" src="@/assets/images/close.png" width="16px" height="16px" @click.stop="delLogo">
                <img
                  class="brand-icon"
                  :src="`${picUrl}${form.icon}`"
                  width="108px"
                  height="88px"
                >
              </div>
              <i v-else class="el-icon-plus" />
            </el-upload>
            <div class="upload-limit">
              请上传≤5M的图片支持JPG、JPEG、PNG格式
            </div>
          </el-form-item>
          <el-form-item label="品牌简称" prop="abbreviation">
            <el-input
              v-model.trim="form.abbreviation"
              maxlength="15"
              placeholder="请输入"
            />
          </el-form-item>
          <el-form-item label="备注" prop="note">
            <el-input
              v-model="form.note"
              type="textarea"
              placeholder="请输入"
              maxlength="500"
            />
          </el-form-item>
        </el-form>
        <div slot="footer" class="dialog-footer">
          <el-button size="small" @click="cancel">取 消</el-button>
          <el-button
            v-debounce
            size="small"
            type="primary"
            @click="submitForm"
          >保 存</el-button>
        </div>
      </el-dialog>
    </el-card>
  </div>
</template>

<script>
import { listBrand, addOrUpdate, delBrand, updateState } from '@/api/basicdata/brand'
import TagsView from '@/layout/components/TagsView/index.vue'
import { getToken } from '@/utils/auth'
import { myTrim } from '@/utils/index'
import FormValidators from '@/vform/utils/validators'
export default {
  name: 'Post',
  components: { TagsView },
  dicts: ['sys_normal_disable', 'general_classification', 'sys_brand_property'],
  data () {
    var validators = (rule, value, callback) => {
      const englishName = this.$refs.form.model.englishName
      const chineseName = this.$refs.form.model.chineseName
      if (!chineseName && !englishName) {
        callback(new Error('中文、英文品牌名称至少输入一个'))
      } else {
        callback()
      }
    }
    return {
      uploadUrl: process.env.VUE_APP_BASE_API + '/file/upload', // 上传的图片服务器地址
      headers: {
        Authorization: 'Bearer ' + getToken()
      },
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
      brandList: [],
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
        chineseName: undefined,
        englishName: undefined,
        enable: undefined
      },
      // 表单参数
      form: {
        chineseName: undefined,
        englishName: undefined,
        code: undefined,
        icon: undefined,
        sort: undefined,
        note: undefined,
        type: undefined,
        abbreviation: undefined,
        attribute: ''
      },
      // 品牌列举枚举对象
      typeObj: {
        0: '办公家电',
        1: '信息设备',
        2: '办公家具',
        3: '专用设备'
      },
      // 表单校验
      rules: {
        chineseName: [
          // { required: true, message: '请填写中文名称', trigger: 'blur' },
          // {
          //   validator: (rule, value, callback) => {
          //     if (value && (/^[\u4e00-\u9fa5]+$/.test(value) === false)) {
          //       callback(new Error('请输入中文'))
          //     } else {
          //       // 校验通过
          //       callback()
          //     }
          //   },
          //   trigger: ['blur', 'change']
          // }
          { validator: validators, trigger: 'blur' },
          {
            validator: (rule, value, callback) => {
              if (value && (/^[\u4e00-\u9fa50-9\s]+$/.test(value) === false)) {
                callback(new Error('请输入中文'))
              } else {
                // 校验通过
                callback()
              }
            },
            trigger: ['blur', 'change']
          }

        ],
        englishName: [
          // { required: true, message: '请填写英文名称', trigger: 'blur' },
          // {
          //   validator: (rule, value, callback) => {
          //     if (value && (/^[a-zA-Z]+$/.test(value) === false)) {
          //       callback(new Error('请输入英文'))
          //     } else {
          //       // 校验通过
          //       callback()
          //     }
          //   },
          //   trigger: ['blur', 'change']
          // }
          { validator: validators, trigger: 'blur' },
          {
            validator: (rule, value, callback) => {
              if (value && (/^[a-zA-Z0-9\s]+$/.test(value) === false)) {
                callback(new Error('请输入英文'))
              } else {
                // 校验通过
                callback()
              }
            },
            trigger: ['blur', 'change']
          }
        ],
        code: [
          { required: true, message: '请填写品牌代码', trigger: 'blur' },
          { min: 3, message: '品牌代码长度最少为3位', trigger: 'blur' },
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
        ],
        // enable: [
        //   { required: true, message: '请选择状态', trigger: 'blur' }
        // ]
        // postSort: [
        //   { required: true, message: '岗位顺序不能为空', trigger: 'blur' }
        // ]
        abbreviation: [{ validator: FormValidators.chineseAndLetter, message: '仅支持汉字或者英文' }]
      },
      rowInfo: {},
      isStatus: false,
      selectionName: [],
      picUrl: process.env.VUE_APP_PIC_URL
    }
  },
  created () {
    this.getList()
  },
  methods: {
    // 去首位空格，但可输入中间空格
    trimFun (str, val) {
      this.form[str] = myTrim(val)
    },
    /** 查询列表 */
    getList () {
      this.loading = true
      listBrand(this.queryParams).then(response => {
        if (response.code === 200) {
          this.brandList = response.data.records
          this.listLength = response.data.total + 1
          this.total = response.data.total
          this.loading = false
          this.$refs.brandTable.setCurrentRow(this.brandList.filter(item => item.id === this.rowInfo.id)[0])
        } else {
          this.$message.error(response.msg)
        }
      })
    },
    // 上传前校检格式和大小
    handleBeforeUpload (file) {
      // 校检文件大小
      const isLt = file.size / 1024 / 1024 < 5
      if (!isLt) {
        this.$message.error(`上传文件大小不能超过 5 MB!`)
        return false
      }
      return true
    },
    handleUploadSuccess (res, file) {
      // 如果上传成功
      if (res.code === 200) {
        this.$set(this.form, 'icon', res.data.url)
      } else {
        this.$message.error('图片上传失败')
      }
    },
    handleUploadError () {
      this.$message.error('图片上传失败')
    },
    // 取消按钮
    cancel () {
      this.open = false
      this.reset()
    },
    // 表单重置
    reset () {
      this.form = {
        chineseName: undefined,
        englishName: undefined,
        code: undefined,
        icon: undefined,
        sort: undefined,
        note: undefined,
        type: undefined,
        abbreviation: undefined,
        attribute: ''
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
      this.selectionName = selection.map(item => item.chineseName)
      this.single = selection.length !== 1
      this.multiple = !selection.length
    },
    /** 导出 */
    handleExport () {
      this.download(
        'system/brand/export/',
        this.ids,
        `品牌_${this.parseTime(new Date(), '{y}-{m}-{d}')}.xlsx`
      )
    },
    closeDetail () {
      this.getList()
    },
    /** 查看按钮操作 */
    handleDetail (row) {
      this.openDetail = true
      this.rowInfo = row
      this.rowInfo.type = this.typeObj[this.rowInfo.type]
    },
    /** 新增按钮操作 */
    handleAdd () {
      this.reset()
      this.open = true
      // this.form.postSort = this.listLength
      this.title = '新建品牌'
    },
    /** 修改按钮操作 */
    handleUpdate (row) {
      this.reset()
      this.form.chineseName = row.chineseName
      this.form.englishName = row.englishName
      this.form.icon = row.icon
      this.form.id = row.id
      this.form.sort = row.sort
      this.form.type = row.type
      this.form.note = row.note
      this.form.code = row.code
      this.form.abbreviation = row.abbreviation
      this.form.attribute = row.attribute
      this.open = true
      this.title = '编辑品牌'
      this.rowInfo = row
    },
    /** 提交按钮 */
    submitForm: function () {
      this.$refs['form'].validate(valid => {
        if (valid) {
          addOrUpdate(this.form).then(res => {
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
    },
    /** 删除按钮操作 */
    handleDelete (row) {
      const brandIds = row.id || this.ids
      const brandName = row.chineseName || this.selectionName
      this.$modal.confirm('确定删除品牌"' + brandName + '"？', '确定删除').then(function () {
        return delBrand(brandIds)
      }).then((res) => {
        if (res.code === 200) {
          // 如果删除之后当前数据长度为1则this.queryParams页数少1
          if (this.brandList.length === 1 && this.queryParams.pageNum !== 1) {
            this.queryParams.pageNum = this.queryParams.pageNum - 1
          }
          this.getList()
          this.$modal.msgSuccess('删除成功')
        } else {
          this.$message.error(res.msg)
        }
      }).catch(() => { })
    },
    // switch改变状态
    changeState (el, row) {
      updateState(row.id).then(res => {
        if (res.code === 200) {
          this.$modal.msgSuccess('修改成功')
          this.getList()
        } else {
          this.$message.error(res.msg)
          this.getList()
        }
      })
    },
    delLogo() {
      this.form.icon = ''
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
.item-logo {
  height: 66px !important;
}
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
.detail-note{
  display: flex;
  font-size: 16px;
  font-weight: 400;
  line-height: 24px;
  margin-bottom: 10px;
  height: auto;
  .item-note{
    width: 416px;
    // height:auto;
    max-height: 200px;
    padding: 5px 10px 5px 0;
    overflow-y: scroll;
  }
    .label-note {
      color: #6a7697;
    margin: 0 16px 16px 0;
    width: 128px;
    color: #6a7697;
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
    position: relative;
  }
}
.my-table {
  padding: 8px 16px 0 16px;
}
.upload-limit {
  font-size: 14px;
  font-weight: 400;
  color: #b2b5b9;
}
.logo-icon{
  width:108px;
  height:88px;
  position:absolute;
  .brand-icon{
    position: absolute;
    left: 0;
    top: 0;
  }
  .del-icon{
    width: 16px;
    height:16px;
    position:absolute;
  top: 0;
  right: 0;
  z-index: 200;
  background-color: #333;
  }
}
</style>

