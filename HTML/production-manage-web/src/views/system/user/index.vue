<template>
  <div>
    <tags-view>
      <el-row :gutter="10" class="mb8">
        <!-- <el-col :span="1.5">
              <el-button
                plain
                size="mini"
                @click="setRole"
                v-hasPermi="["system:user:remove"]"
                ><i style="color:#4686EF;" class="iconfont icon-jiaoseshezhi"></i
                >批量设置角色</el-button
              >
            </el-col> -->
        <el-col :span="1.5">
          <el-button
            v-hasPermi="['system:user:import']"
            plain
            size="small"
            @click="downloadDraft"
          ><i style="color:#4686EF;" class="iconfont icon-daoru" />下载模板</el-button>
        </el-col>
        <el-col :span="1.5">
          <el-button
            v-hasPermi="['system:user:export']"
            plain
            size="small"
            @click="handleExport"
          ><i style="color:#4686EF;" class="iconfont icon-daochu" />导出</el-button>
        </el-col>
        <el-col :span="1.5">
          <el-upload
            ref="upload"
            :limit="1"
            accept=".xlsx, .xls"
            :headers="upload.headers"
            :before-upload="beforeUpload"
            :action="upload.url + '?updateSupport=' + upload.updateSupport"
            :disabled="upload.isUploading"
            :on-progress="handleFileUploadProgress"
            :on-success="handleFileSuccess"
            :show-file-list="false"
          >
            <el-button
              v-hasPermi="['system:user:import']"
              plain
              size="small"
            ><i style="color:#4686EF;" class="iconfont icon-daoru" />导入</el-button>
          </el-upload>
        </el-col>
        <el-col :span="1.5">
          <el-button
            v-hasPermi="['system:user:edit']"
            plain
            size="small"
            @click="setDept"
          ><i style="color:#4686EF;" class="iconfont icon-bumenxuanze" />批量设置部门</el-button>
        </el-col>
        <el-col :span="1.5">
          <el-button
            v-hasPermi="['system:user:add']"
            type="primary"
            size="small"
            @click="handleAdd"
          ><i class="iconfont icon-zengjia" style="color: #ffffff" />新建用户</el-button>
        </el-col>
      </el-row>
    </tags-view>
    <el-card class="main-card-left">
      <el-row :gutter="20" style="margin-left:0;margin-right:0">
        <!--部门数据-->
        <el-col
          :span="4"
          :xs="24"
          style="border-right: 1px solid #eaedf4;padding-left:0;padding-right:0"
        >
          <div class="head-container">
            <el-input
              v-model.trim="deptName"
              placeholder="请输入部门名称"
              clearable
              size="small"
              suffix-icon="el-icon-search"
              style="margin-bottom: 20px;margin-right:16px;width:calc(100% - 16px)"
              maxlength="15"
            />
          </div>
          <div class="head-container" style="height:calc(100% - 52px);overflow: scroll;">
            <el-tree
              ref="tree"
              :data="deptOptions"
              :props="defaultProps"
              :expand-on-click-node="false"
              :filter-node-method="filterNode"
              highlight-current
              node-key="id"
              :default-expanded-keys="expandedArr"
              @node-click="handleNodeClick"
            />
          </div>
        </el-col>
        <!--用户数据-->
        <el-col :span="20" :xs="24" class="right-card">
          <el-form
            v-show="showSearch"
            ref="queryForm"
            :model="queryParams"
            :inline="true"
            label-width="68px"
            class="searchForm"
          >
            <el-form-item label="用户名" prop="userName">
              <el-input
                v-model.trim="queryParams.userName"
                placeholder="请输入"
                clearable
                size="small"
                style="width: 200px"
                maxlength="15"
                @keyup.enter.native="handleQuery"
              />
            </el-form-item>
            <el-form-item label="姓名" prop="realName">
              <el-input
                v-model.trim="queryParams.realName"
                placeholder="请输入"
                clearable
                size="small"
                style="width: 200px"
                maxlength="15"
                @keyup.enter.native="handleQuery"
              />
            </el-form-item>
            <el-form-item label="工号" prop="jobNumber">
              <el-input
                v-model.trim="queryParams.jobNumber"
                placeholder="请输入"
                clearable
                size="small"
                maxlength="20"
                style="width: 200px"
                @keyup.enter.native="handleQuery"
              />
            </el-form-item>

            <el-form-item v-if="!isShowMore">
              <el-button size="small" @click="resetQuery">重置</el-button>
              <el-button
                plain
                type="primary"
                size="small"
                class="search-btn"
                @click="handleQuery"
              >搜索</el-button>
            </el-form-item>
            <!-- <el-form-item v-if="isShowMore" label="部门" prop="deptId"> -->
            <!-- <el-input
                    v-model="queryParams.deptId"
                    placeholder="请输入"
                    clearable
                    size="mini"
                    style="width: 200px"
                    @keyup.enter.native="handleQuery"
                  /> -->
            <!-- <treeselect v-model.trim="queryParams.deptId" class="treeselect-main" style="width: 200px;" :options="deptOptions" :show-count="true" no-results-text="暂无数据" placeholder="请输入" /> -->
            <!--  </el-form-item> -->
            <el-form-item v-if="isShowMore" label="职位" prop="postIds">
              <!-- <el-input
                    v-model="queryParams.postName"
                    placeholder="请输入"
                    clearable
                    size="small"
                    style="width: 180px"
                    @keyup.enter.native="handleQuery"
                  /> -->
              <el-select
                v-model="queryParams.postIds"
                filterable
                size="small"
                placeholder="请选择"
                style="width: 200px"
              >
                <el-option
                  v-for="item in postOptions"
                  :key="item.postId"
                  :label="item.postName"
                  :value="item.postId"
                />
              </el-select>
            </el-form-item>
            <el-form-item v-if="isShowMore" label="创建时间">
              <div class="date_box">
                <el-date-picker
                  v-model="dateRange"
                  size="small"
                  style="width: 200px"
                  value-format="yyyy-MM-dd"
                  type="daterange"
                  range-separator="~"
                  start-placeholder="开始日期"
                  end-placeholder="结束日期"
                  :clearable="false"
                /><i class="el-icon-date data_icon" />
              </div>
            </el-form-item>
            <el-form-item v-if="isShowMore">
              <el-button size="small" @click="resetQuery">重置</el-button>
              <el-button
                plain
                type="primary"
                size="small"
                class="search-btn"
                @click="handleQuery"
              >搜索</el-button>
            </el-form-item>
          </el-form>

          <el-table
            ref="multipleTable"
            v-loading="loading"
            :data="userList"
            height="100%"
            highlight-current-row
            class="userTable"
            @selection-change="handleSelectionChange"
          >
            <el-table-column type="selection" align="center" />
            <!-- <el-table-column
              key="sort"
              label="序号"
              prop="sort"
              align="left"
              width="80"
              sortable
            /> -->
            <!-- <el-table-column
              label="显示顺序"
              prop="sort"
              width="100"
            /> -->
            <el-table-column
              key="userName"
              label="用户名"
              align="left"
              prop="userName"
              width="140"
              :show-overflow-tooltip="true"
            />
            <el-table-column
              key="realName"
              label="姓名"
              align="left"
              prop="realName"
              width="180"
              :show-overflow-tooltip="true"
            >
              <template slot-scope="scope">
                <el-popover ref="popoverRef" width="400" trigger="hover" placement="top" @show="showData(scope.row)">
                  <div class="popoverContent">
                    <div class="popoverHead">
                      <i class="iconfont icon-guanbi" @click="closePopover" />
                      <div class="popoverHead_img">
                        <img class="headImage" :src="avatar">
                      </div>
                      <div class="popoverHead_info">
                        <p class="popoverP" style="font-weight: bold;font-size: 18px;">{{ scope.row.userName }}|{{ scope.row.realName }}</p>
                        <div class="popoverHead_jobNum">
                          <img style="width:24px;height:24px;margin-right:10px;" src="../../../assets/images/card@2x.png" alt="">
                          <p class="popoverP" style="font-size:16px">{{ scope.row.jobNumber }}</p>
                        </div>
                      </div>
                    </div>
                    <div style="width:100%;height:1px;background:rgba(0, 0, 0, 0.35);" />
                    <div>
                      <p class="popoverP"><span style="color:#6A7697;">所属部门</span><span style="padding-left:20px;">{{ deptData.deptName }}</span></p>
                      <div v-if="scope.row.posts.length>0" style="display:flex;flex-wrap: wrap">
                        <span v-for="(item,index) in scope.row.posts" :key="index"><p v-if="index==0" class="popoverP"><span style="color:#6A7697;">职位 </span><span style="padding-left:44px;">{{ item && item.postName }}</span></p><p v-else class="popoverP">,{{ item && item.postName }}</p></span>
                      </div>
                      <p v-else class="popoverP">
                        <span style="color:#6A7697;">职位 </span><span style="padding-left:44px;">--</span>
                      </p>
                      <div v-if="scope.row.roles.length>0" style="display:flex;flex-wrap: wrap">
                        <span v-for="(item,index) in scope.row.roles" :key="index">
                          <p v-if="index==0" class="popoverP">
                            <span style="color:#6A7697;">角色 </span>
                            <span style="padding-left:44px;">{{ item && item.roleName }}</span>
                          </p>
                          <p v-else class="popoverP">,{{ item && item.roleName }}</p>
                        </span>
                      </div>
                      <p v-else class="popoverP" style="padding-bottom: 10px;">
                        <span style="color:#6A7697;">角色 </span><span style="padding-left:44px;">--</span>
                      </p>
                      <p class="popoverP"><span style="color:#6A7697;">手机号 </span><span style="padding-left:30px;">{{ scope.row.phonenumber }}</span></p>
                    </div>
                  </div>
                  <div slot="reference" class="name-wrapper">
                    {{ scope.row.realName }}
                  </div>
                </el-popover>
              </template>
            </el-table-column>
            <el-table-column
              key="jobNumber"
              sortable
              label="工号"
              align="left"
              width="180"
              prop="jobNumber"
              :show-overflow-tooltip="true"
            />
            <el-table-column
              key="deptName"
              label="部门"
              align="left"
              prop="dept.deptName"
              :show-overflow-tooltip="true"
            />
            <el-table-column
              key="roles"
              label="角色"
              align="left"
              prop="roles"
              :show-overflow-tooltip="true"
            >
              <template slot-scope="scope">
                <div v-if="scope.row.roles.length>0" class="roleL">
                  <span v-for="(item,index) in scope.row.roles" :key="index">
                    <span v-if="index==0">{{ item && item.roleName }}</span>
                    <span v-else>,{{ item && item.roleName }}</span>
                  </span>
                </div>
                <div v-else>
                  --
                </div>
              </template>
            </el-table-column>
            <el-table-column
              key="posts"
              label="职位"
              align="left"
              prop="posts"
              :show-overflow-tooltip="true"
              width="250"
            >
              <template slot-scope="scope">
                <div v-if="scope.row.posts.length>0" class="roler">
                  <span v-for="(item,index) in scope.row.posts" :key="index"><span v-if="index==0">{{ item && item.postName }}</span><span v-else>,{{ item && item.postName }}</span></span>
                </div>
                <div v-else>
                  --
                </div>
              </template>
            </el-table-column>
            <el-table-column
              label="操作"
              align="left"
              width="250"
              class-name="small-padding"
            >
              <template slot-scope="scope">
                <el-button
                  v-hasPermi="['system:user:query']"
                  size="small"
                  type="text"
                  @click="handleUpdate(scope.row,0)"
                >详情</el-button>
                <el-button
                  v-hasPermi="['system:user:edit']"
                  size="small"
                  type="text"
                  @click="handleUpdate(scope.row,1)"
                >编辑</el-button>
                <el-button
                  v-hasPermi="['system:user:edit']"
                  size="small"
                  type="text"
                  @click="handleResetPwd(scope.row)"
                >重置密码</el-button>
                <el-button
                  v-hasPermi="['system:user:remove']"
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
        </el-col>
      </el-row>

      <!-- 添加或修改用户配置对话框 -->
      <el-dialog
        :title="title"
        :visible.sync="open"
        :width="isEdit?'800px':'600px'"
        append-to-body
        :close-on-click-modal="false"
        @close="closeUser"
      >
        <el-form
          ref="form"
          :model="form"
          :rules="rules"
          label-width="100px"
          style="width: 80%; margin-left: 10%"
          :class="isEdit?'':'editForm'"
          label-position="right"
        >
          <el-form-item
            label="用户名"
            :prop="isEdit?'userName':''"
          >
            <el-input
              v-if="isEdit"
              v-model.trim="form.userName"
              placeholder="请输入"
              maxlength="15"
              :disabled="isDisabledModify || isEdit"
            />
            <span v-else>
              {{ form.userName }}
            </span>
          </el-form-item>
          <el-form-item label="真实姓名" :prop="isEdit?'realName':''">
            <el-input
              v-if="isEdit"
              v-model.trim="form.realName"
              placeholder="请输入"
              maxlength="15"
              :disabled="isDisabledModify"
            />
            <span v-else>
              {{ form.realName }}
            </span>
          </el-form-item>
          <el-form-item label="性别">
            <el-radio-group v-if="isEdit" v-model="form.sex" :disabled="isDisabledModify">
              <el-radio :key="0" label="0">男</el-radio>
              <el-radio :key="1" label="1">女</el-radio>
            </el-radio-group>
            <span v-else>
              {{ form.sex ==='0'?'男':form.sex ==='1'?'女':'-' }}
            </span>
          </el-form-item>
          <el-form-item label="工号" :prop="isEdit?'jobNumber':''">
            <el-input v-if="isEdit" v-model.trim="form.jobNumber" oninput="value=value.replace(/[\u4E00-\u9FA5]/ig, '')" maxlength="20" :disabled="isDisabledModify" placeholder="请输入" />
            <span v-else>
              {{ form.jobNumber }}
            </span>
          </el-form-item>
          <el-form-item label="显示顺序" prop="sort">
            <el-input
              v-if="isEdit"
              v-model.trim="form.sort"
              placeholder="请输入"
              :disabled="isDisabledModify"
              type="text"
              maxlength="5"
              onkeyup="if(value==0&&value!=''){value=1}this.value = this.value.replace(/[^0-9]/g, '');"
            />
            <span v-else>
              {{ form.sort?form.sort:'--' }}
            </span>
          </el-form-item>
          <el-form-item label="联系电话" :prop="isEdit?'phonenumber':''">
            <el-input
              v-if="isEdit"
              v-model="form.phonenumber"
              placeholder="请输入"
              maxlength="11"
              type="number"
              oninput="if(value.length>11)value=value.slice(0,11)"
              :disabled="isDisabledModify"
            />
            <span v-else>
              {{ form.phonenumber }}
            </span>
          </el-form-item>
          <el-form-item label="所属部门" :prop="isEdit?'deptName':''">
            <!-- <el-input
              v-if="isEdit"
              v-model.trim="chooseDep.label"
              placeholder="请选择"
              :disabled="isDisabledModify"
              maxlength="15"
              @focus="showDep"
            >
              <i slot="suffix" class="iconfont icon-bumenxuanze" style="font-size: 20px;" @click="showDep" />
            </el-input> -->
            <div class="my-select-con">
              <template v-if="isEdit">
                <el-select
                  v-model.trim="form.deptName"
                  filterable
                  placeholder="请输入"
                  class="input-width"
                  style="width:508px;"
                  @change="getDeptId"
                >
                  <el-option
                    v-for="item in belongDeptOptions"
                    :key="item.deptId"
                    :label="item.deptName"
                    :value="item.deptId"
                  />
                </el-select>
                <i slot="suffix" class="iconfont icon-bumenxuanze" style="cursor: pointer;" @click="showDep" />
              </template>
              <span v-else>
                {{ form.deptName?form.deptName:'--' }}
              </span>
            </div>
          </el-form-item>
          <el-form-item label="职位">
            <div class="my-select-con">
              <template v-if="isEdit">
                <el-select
                  ref="showPost"
                  v-model="checkedNodes1"
                  :placeholder="isDisabledModify==true?'':'请选择'"
                  style="width: 100%"
                  :disabled="isDisabledModify"
                  multiple
                  value-key="postId"
                  @remove-tag="removePost"
                  @visible-change="handlePost"
                >
                  <el-option
                    v-for="item in postOptions"
                    :key="item.postId"
                    :label="item.postName"
                    :value="item"
                    :disabled="item.status == 1"
                  >{{ item.postName }}</el-option>
                </el-select>
                <i slot="suffix" class="iconfont icon-people" style="font-size: 14px;line-height: 36px;cursor: pointer;" @click.stop="showPostdig" />
              </template>
              <div v-else-if="postsInfo.length>0">
                <span v-for="(item,index) in postsInfo" :key="item.postId"><span v-if="index==0">{{ item.postName }}</span><span v-else>,{{ item.postName }}</span></span>
              </div>
              <div v-else>
                --
              </div>
            </div>
          </el-form-item>
          <el-form-item label="角色">
            <div class="my-select-con">
              <template v-if="isEdit">
                <el-select
                  ref="showRole"
                  v-model="checkedNodes"
                  class="role"
                  :placeholder="isDisabledModify==true?'':'请选择'"
                  style="width: 100%"
                  multiple
                  value-key="roleId"
                  :disabled="isDisabledModify"
                  @remove-tag="removeRole"
                  @visible-change="handleRole"
                >
                  <el-option
                    v-for="item in roleOptions"
                    :key="item.roleId"
                    :label="item.roleName"
                    :value="item"
                  >{{ item.roleName }}</el-option>
                </el-select>
                <i slot="suffix" class="iconfont icon-people" style="font-size: 14px;line-height: 36px;cursor: pointer;" @click.stop="showRole" />
              </template>
              <span v-for="(item,index) in checkedNodes" v-else-if="checkedNodes.length>0" :key="item.roleId"><span v-if="index==0">{{ item.roleName }}</span><span v-else>,{{ item.roleName }}</span>
              </span>
              <span v-else>--</span>
            </div>
          </el-form-item>
          <el-form-item label="是否允许登录">
            <el-radio-group v-if="isEdit" v-model="form.allowLogin" :disabled="isDisabledModify">
              <el-radio :key="1" label="1">是</el-radio>
              <el-radio :key="0" label="0">否</el-radio>
            </el-radio-group>
            <span v-else>{{ form.allowLogin === '1'?'是':'否' }}</span>
          </el-form-item>
          <el-form-item label="账户锁定">
            <el-switch
              v-if="isEdit"
              v-model="form.locked"
              active-value="1"
              inactive-value="0"
              active-color="#409EFF"
            />
            <span v-else>{{ form.locked === '1'?'是':'否' }}</span>
          </el-form-item>
        </el-form>
        <div v-if="isDisabledModify == false" slot="footer" class="dialog-footer">
          <el-button size="small" @click="cancel">取 消</el-button>
          <el-button size="small" type="primary" @click="submitForm">保 存</el-button>
        </div>
      </el-dialog>
      <!-- 角色选择 -->
      <el-dialog
        title="选择角色"
        :visible.sync="show"
        width="800px"
        class="selectRole"
        :close-on-click-modal="false"
        append-to-body
      >
        <selectRole ref="role" :role-options="roleOptions" :checked-nodes="checkedNodes" @checkRole="checkRole" />
      </el-dialog>
      <!-- 职位选择 -->
      <el-dialog
        title="选择职位"
        :visible.sync="showPost"
        width="800px"
        class="selectPost"
        :close-on-click-modal="false"
        append-to-body
      >
        <selectPost ref="post" :post-options="postOptions" :checked-nodes="checkedNodes1" @checkPost="checkPost" />
      </el-dialog>
      <!-- 部门选择 -->
      <el-dialog
        title="部门选择"
        :visible.sync="IsShowDept"
        width="600px"
        class="selectDept"
        :close-on-click-modal="false"
        append-to-body
      >
        <div class="head-search">
          <el-input
            v-model.trim="dialogDeptName"
            placeholder="请输入部门名称"
            clearable
            size="small"
            maxlength="15"
            suffix-icon="el-icon-search"
            style="margin-bottom: 20px"
          />
        </div>
        <div class="head-container" style="height:400px;overflow-y:auto;">
          <el-tree
            ref="deptTree"
            :data="deptOptions"
            :props="defaultProps"
            :expand-on-click-node="false"
            :filter-node-method="filterNode"
            node-key="id"
            :default-expanded-keys="expandedArr"
            highlight-current
            @node-click="handleClickDep"
          />
        </div>
        <div slot="footer" class="dialog-footer">
          <el-button
            size="small"
            @click="IsShowDept=false"
          >取 消</el-button>
          <el-button
            size="small"
            type="primary"
            @click="SaveChoseDept"
          >保 存</el-button>
        </div>
      </el-dialog>
    </el-card>
  </div>

</template>

<script>
import {
  listUser,
  getUser,
  delUser,
  addUser,
  updateUser,
  resetUserPwd
} from '@/api/system/user'
import { getToken } from '@/utils/auth'
import selectRole from './components/selectRole.vue'
import selectPost from './components/selectPost.vue'
import { treeselect, batchUpdateDept, listDept } from '@/api/system/dept'
import TagsView from '@/layout/components/TagsView/index.vue'
/* import Treeselect from '@riophae/vue-treeselect' */
import '@riophae/vue-treeselect/dist/vue-treeselect.css'
import { mapGetters } from 'vuex'
export default {
  name: 'User',
  dicts: ['sys_normal_disable', 'sys_user_sex'],
  /*   components: { Treeselect }, */
  components: { selectRole, selectPost, TagsView },
  data() {
    return {
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
      isDownload: true,
      // 总条数
      total: 0,
      // 用户表格数据
      userList: null,
      // 弹出层标题
      title: '',
      // 部门树选项
      deptOptions: undefined,
      // 是否显示弹出层
      open: false,
      // 部门名称
      deptName: undefined,
      //  弹框部门名称
      dialogDeptName: undefined,
      // 默认密码
      initPassword: undefined,
      // 日期范围
      dateRange: [],
      // 岗位选项
      postOptions: [],
      // 角色选项
      roleOptions: [],
      // 表单参数
      form: {
      },
      defaultProps: {
        children: 'children',
        label: 'label'
      },
      props: {
        // children: 'children',
        label: 'roleName'
      },
      props1: {
        // children: 'children',
        label: 'postName'
      },
      // 用户导入参数
      upload: {
        // 是否显示弹出层（用户导入）
        open: false,
        // 弹出层标题（用户导入）
        title: '',
        // 是否禁用上传
        isUploading: false,
        // 是否更新已经存在的用户数据
        updateSupport: 0,
        // 设置上传的请求头部
        headers: { Authorization: 'Bearer ' + getToken() },
        // 上传的地址
        url: process.env.VUE_APP_BASE_API + '/system/user/importData'
      },
      // 查询参数
      queryParams: {
        pageNum: 1,
        pageSize: 10,
        userName: undefined,
        realName: undefined,
        jobNumber: undefined,
        deptName: undefined,
        postName: undefined
      },
      // 列信息
      columns: [
        { key: 0, label: `用户编号`, visible: true },
        { key: 1, label: `用户名称`, visible: true },
        { key: 2, label: `用户昵称`, visible: true },
        { key: 3, label: `部门`, visible: true },
        { key: 4, label: `手机号码`, visible: true },
        { key: 5, label: `状态`, visible: true },
        { key: 6, label: `创建时间`, visible: true }
      ],
      // 表单校验
      rules: {
        // sort: [{ required: true, message: '请输入', trigger: 'blur' }],
        userName: [
          { required: true, message: '请填写用户名', trigger: 'blur' }
        ],
        realName: [{ required: true, message: '请填写真实姓名', trigger: 'blur' }],
        phonenumber: [
          { required: true, message: '请填写手机号', trigger: 'blur' },
          {
            pattern: /^1[3|4|5|6|7|8|9][0-9]\d{8}$/,
            message: '请输入正确的手机号码',
            trigger: 'blur'
          }
        ],
        jobNumber: [{ required: true, message: '请填写工号', trigger: 'blur' }],
        deptName: [
          { required: true, message: '请选择所属部门', trigger: 'change' }
        ]
      },
      isShowMore: false,
      selectedLength: 0,
      show: false,
      showPost: false,
      IsShowDept: false,
      roleName: '',
      postName: '',
      checked: true,
      checkedNodes: [],
      checkedNodes1: [],
      checkNodes: [],
      checkNodes1: [],
      chooseDep: {},
      preChooseDep: {},
      isDisabledModify: false,
      userLength: 0,
      isEdit: false,
      postsInfo: [],
      selectId: '',
      expandedArr: [],
      rowInfo: {},
      deptData: {},
      // 所属部门下拉选项
      belongDeptOptions: []
    }
  },
  computed: {
    ...mapGetters([
      'avatar'
    ])
  },
  watch: {
    // 根据名称筛选部门树
    deptName(val) {
      this.$refs.tree.filter(val)
    },
    dialogDeptName(val) {
      this.$refs.deptTree.filter(val)
    },
    roleName(val) {
      this.$refs.roletree.filter(val)
    },
    postName(val) {
      this.$refs.posttree.filter(val)
    }
  },
  created() {
    this.getList()
    this.getTreeselect()
    this.getConfigKey('sys.user.initPassword').then((response) => {
      this.initPassword = response.msg
    })
  },
  mounted() {
    getUser().then((response) => {
      this.roleOptions = response.roles.filter(item => item.status !== '1')
      this.postOptions = response.posts
    })
  },
  methods: {
    showData(data) {
      this.deptData = data.dept
    },
    closePopover() {
      document.body.click()
    },
    /** 查询用户列表 */
    getList() {
      this.loading = true
      listUser(this.addDateRange(this.queryParams, this.dateRange)).then(
        (response) => {
          // console.log(response)
          this.userList = response.rows
          this.userList.forEach(item => {
            item.roles = item.roles.filter(item1 => item1 && item1.status === '0')
            item.posts = item.posts.filter(item1 => item1 && item1.status === '0')
          })
          // const arr = []
          // response.rows.forEach(item => {
          //   arr.push(item.sort)
          // })
          // this.userLength = arr.reduce((max, item) => {
          //   return max > item ? max : item
          // })
          this.userLength = response.total + 1
          this.total = response.total
          this.loading = false
          this.$refs.multipleTable.setCurrentRow(this.userList.filter(item => item.userId === this.rowInfo.userId)[0])
        }
      )
      /** 获取所有部门列表 */
      listDept().then(response => {
        this.belongDeptOptions = response.data
      })
    },
    /** 查询部门下拉树结构 */
    getTreeselect() {
      treeselect().then((response) => {
        this.deptOptions = response.data
        response.data.forEach(item => {
          this.expandedArr.push(item.id)
        })
      })
    },
    // 筛选节点
    filterNode(value, data) {
      if (!value) return true
      return data.label.indexOf(value) !== -1
    },

    filterPostNode(value, data) {
      if (!value) return true
      return data.postName.indexOf(value) !== -1
    },
    // 节点单击事件
    handleNodeClick(data) {
      // console.log(data)
      this.queryParams.deptId = data.id
      this.chooseDep.id = data.id
      this.chooseDep.deptName = data.label
      this.handleQuery()
    },
    // 取消按钮
    cancel() {
      this.open = false
      this.reset()
      this.chooseDep = {}
    },
    closeUser() {
      this.chooseDep = {}
    },
    // 表单重置
    reset() {
      this.form = {
        sort: undefined,
        userId: undefined,
        deptId: undefined,
        userName: undefined,
        nickName: undefined,
        password: undefined,
        phonenumber: undefined,
        email: undefined,
        sex: undefined,
        // status: '0',
        allowLogin: '1',
        locked: '0',
        remark: undefined,
        postIds: [],
        roleIds: []
      }
      this.resetForm('form')
    },
    /** 搜索按钮操作 */
    handleQuery() {
      this.queryParams.pageNum = 1
      this.getList()
    },
    /** 重置按钮操作 */
    resetQuery() {
      this.dateRange = []
      this.resetForm('queryForm')
      this.getTreeselect()
      this.queryParams.deptId = undefined
      this.handleQuery()
    },
    // 多选框选中数据
    handleSelectionChange(selection) {
      this.selectedLength = selection.length
      this.ids = selection.map((item) => item.userId)
      this.single = selection.length !== 1
      this.multiple = !selection.length
    },
    /** 新增按钮操作 */
    handleAdd() {
      this.reset()
      this.getTreeselect()
      this.isDisabledModify = false
      getUser().then((response) => {
        this.postOptions = response.posts
        this.roleOptions = response.roles
        // this.roleOptions.forEach(item => {
        //   if (item.status === '1') {
        //     item.disabled = true
        //   }
        // })
        this.roleOptions = this.roleOptions.filter(item => item.status !== '1')
        // this.postOptions.forEach(item => {
        //   if (item.status === '1') {
        //     item.disabled = true
        //   }
        // })
        this.postOptions = this.postOptions.filter(item => item.status !== '1')
        this.open = true
        this.isEdit = true
        this.title = '新建用户'
        // this.form.password = this.initPassword;
        this.checkedNodes = []
        this.checkedNodes1 = []
      })
    },
    /** 修改按钮操作 */
    handleUpdate(row, index) {
      this.reset()
      this.rowInfo = row
      this.chooseDep = {}
      if (index === 1) {
        this.getTreeselect()
      }

      // const arr = []
      const userId = row.userId || this.ids
      getUser(userId).then((response) => {
        // console.log(response)
        this.form = response.data
        this.postOptions = response.posts
        this.roleOptions = response.roles
        // this.roleOptions.forEach(item => {
        //   if (item.status === '1') {
        //     item.disabled = true
        //   }
        // })
        this.roleOptions = this.roleOptions.filter(item => item.status !== '1')
        // this.postOptions.forEach(item => {
        //   if (item.status === '1') {
        //     item.disabled = true
        //   }
        // })
        this.postOptions = this.postOptions.filter(item => item.status !== '1')
        // this.form.postIds = response.postIds[0]
        this.checkedNodes = []
        this.checkedNodes1 = []
        if (response.roleIds) {
          this.form.roleIds = response.roleIds
          // this.roleOptions.forEach(item => {
          //   this.form.roleIds.forEach(item1 => {
          //     if (item.roleId === item1) {
          //       arr.push(item)
          //     }
          //   })
          // })
          this.checkedNodes = response.data.roles.filter(item => item.status === '0')
        }
        if (response.postIds) {
          this.form.postIds = response.postIds
          this.postsInfo = response.data.posts.filter(item => item.status !== '1')
          this.checkedNodes1 = response.data.posts.filter(item => item && item.status !== '1')
        }
        // this.deptOptions.forEach(item=>{
        //   if(item.id == response.deptId){

        //   }
        // })
        if (response.data.dept) {
          this.chooseDep.id = response.data.dept.deptId
          this.chooseDep.label = response.data.dept.deptName
        }

        this.open = true
        // this.form.password = ''
      })
      if (index === 1) {
        this.title = '编辑'
        this.isEdit = true
        this.isDisabledModify = false
        this.selectId = userId
      } else if (index === 0) {
        this.title = '详情'
        this.isEdit = false
        this.isDisabledModify = true
      }
      console.log(this.form, 'form')
    },
    /** 重置密码按钮操作 */
    handleResetPwd(row) {
      resetUserPwd(row.userId).then((response) => {
        this.$modal.msgSuccess('重置密码成功!')
      })
      // this.$prompt('请输入"' + row.userName + '"的新密码', '提示', {
      //   confirmButtonText: '确定',
      //   cancelButtonText: '取消',
      //   closeOnClickModal: false,
      //   inputPattern: /^.{5,20}$/,
      //   inputErrorMessage: '用户密码长度必须介于 5 和 20 之间'
      // })
      //   .then(({ value }) => {
      //     resetUserPwd(row.userId, value).then((response) => {
      //       this.$modal.msgSuccess('修改成功，新密码是：' + value)
      //     })
      //   })
      //   .catch(() => {})
    },
    /** 分配角色操作 */
    handleAuthRole(row) {
      const userId = row.userId
      this.$router.push('/system/user-auth/role/' + userId)
    },
    /** 提交按钮 */
    submitForm() {
      this.form.deptId = this.chooseDep.id
      // let arr = this.form.postIds.length>0?[this.form.postIds]:[]
      // this.form.postIds = arr
      this.form.password = this.initPassword
      // if (this.form.sort === '') {
      //   this.form.sort = this.userLength
      // }

      if (this.form.sex === '' || this.form.sex === undefined) {
        this.form.sex = '2'
      }
      // this.form.roles = []
      // this.form.roleIds = []
      // this.form.roleId = null
      // this.form.roleName = null
      // console.log(this.form)
      this.$refs['form'].validate((valid) => {
        if (valid) {
          if (this.form.userId !== undefined) {
            updateUser(this.form).then((response) => {
              this.$modal.msgSuccess('修改成功')
              this.open = false
              this.getList()
            })
          } else {
            addUser(this.form).then((response) => {
              this.$modal.msgSuccess('保存成功')
              this.open = false
              this.getList()
            })
          }
        }
      })
    },
    /** 删除按钮操作 */
    handleDelete(row) {
      const userIds = row.userId || this.ids
      const userName = row.userName
      this.$modal
        .confirm('确定删除用户"' + userName + '"？', '确定删除')
        .then(function() {
          return delUser(userIds)
        })
        .then(() => {
          this.getList()
          this.$modal.msgSuccess('删除成功')
        })
        .catch(() => {})
    },
    /** 导出按钮操作 */
    handleExport() {
      this.download(
        'system/user/export',
        this.ids,
        `用户_${this.parseTime(new Date(), '{y}-{m}-{d}')}.xlsx`
      )
    },
    /** 导入按钮操作 */
    // handleImport() {
    //   this.upload.title = '用户导入'
    //   this.upload.open = true
    // },
    // 文件上传中处理
    handleFileUploadProgress(event, file, fileList) {
      this.upload.isUploading = true
    },
    beforeUpload(file) {
      const isLt100M = file.size / 1024 / 1024 < 100
      if (!isLt100M) {
        this.$message.error('上传文件大小不能超过 100MB')
      }
      return isLt100M
    },
    // 文件上传成功处理
    handleFileSuccess(response, file, fileList) {
      this.upload.open = false
      this.upload.isUploading = false
      this.$refs.upload.clearFiles()
      this.$alert(
        "<div style='overflow:auto;overflow-x: hidden;max-height: 20vh;padding: 0 20px 0;text-align:left'>" +
        response.msg +
        '</div>',
        response.code === 200 ? '导入成功' : '导入失败',
        { dangerouslyUseHTMLString: true }
      )
      this.getList()
    },
    // 提交上传文件
    submitFileForm() {
      this.$refs.upload.submit()
    },
    //  弹出角色选择框
    showRole() {
      this.$refs.showRole.blur()
      this.show = true
      this.$nextTick(() => {
        this.$refs.role.$refs.roletree.setCheckedNodes(this.checkedNodes)
      })
    },
    showPostdig() {
      this.$refs.showPost.blur()
      this.showPost = true
      this.$nextTick(() => {
        // console.log(this.$refs.post)
        this.$refs.post.$refs.posttree.setCheckedNodes(this.checkedNodes1)
      })
    },
    //  移除角色
    removeRole(e) {
      const arr = this.form.roleIds
      arr.forEach((item, index) => {
        if (item === e.roleId) {
          arr.splice(index, 1)
        }
      })
      this.form.roleIds = arr
    },
    removePost(e) {
      const arr = this.form.postIds
      arr.forEach((item, index) => {
        if (item === e.postId) {
          arr.splice(index, 1)
        }
      })
      this.form.roleIds = arr
    },

    confirmPost() {
      this.checkedNodes1 = this.checkNodes1
      this.checkedNodes1.forEach(item => {
        this.form.postIds.push(item.postId)
      })
      this.form.postIds = Array.from(new Set(this.form.postIds))
      this.showPost = false
    },
    //  保存部门
    SaveChoseDept() {
      if (!this.preChooseDep.id) {
        this.$message.error('请选择部门')
        return
      }
      this.chooseDep = this.preChooseDep
      if (this.ids.length > 0) {
        const data = {
          deptId: this.chooseDep.id,
          userIdList: this.ids
        }
        batchUpdateDept(data).then((response) => {
          if (response.code === 200) {
            this.$notify({
              title: '设置成功',
              type: 'success',
              duration: 1000
            })
            this.getList()
          }
        })
      }
      this.IsShowDept = false
      this.form.deptId = this.chooseDep.id
      this.form.deptName = this.chooseDep.label
    },
    //  显示部门弹框
    showDep() {
      this.IsShowDept = true
      this.$nextTick(() => {
        this.$refs.deptTree.setCurrentKey(this.chooseDep)
      })
    },
    //  点击部门
    handleClickDep(e) {
      this.preChooseDep = e
      console.log(this.preChooseDep)
      // this.chooseDep = eroleName
    },
    //  批量设置用户按钮
    setDept() {
      if (this.selectedLength === 0) {
        this.$message({
          message: '请勾选需要设置的用户',
          type: 'error'
        })
        return
      }
      this.IsShowDept = true
      this.$nextTick(() => {
        this.$refs.deptTree.setCurrentKey(null)
      })
    },
    downloadDraft() {
      this.download('system/user/importTemplate', {
      }, `user_template_${new Date().getTime()}.xlsx`)
    },
    handleRole(e) {
      if (!e) {
        this.checkRole(this.checkedNodes)
      }
    },
    checkRole(data) {
      // console.log(data, 'role')
      this.checkedNodes = data
      const roleNameArr = []
      this.form.roleIds = []
      // if (this.form.roleName) roleNameArr = this.form.roleName.split(',')
      this.checkedNodes.forEach(item => {
        this.form.roleIds.push(item.roleId)
        roleNameArr.push(item.roleName)
      })
      // console.log(data, roleNameArr, 'role')
      this.form.roleName = roleNameArr.join(',')
      this.form.roleIds = Array.from(new Set(this.form.roleIds))
      this.show = false
    },
    handlePost(e) {
      if (!e) {
        this.checkPost(this.checkedNodes1)
      }
    },
    checkPost(data) {
      this.checkedNodes1 = data
      this.form.postIds = []
      const postNameArr = []
      this.checkedNodes1.forEach(item => {
        this.form.postIds.push(item.postId)
        postNameArr.push(item.roleName)
      })
      this.form.postName = postNameArr.join(',')
      this.form.postIds = Array.from(new Set(this.form.postIds))
      this.showPost = false
    },
    getDeptId(data) {
      this.belongDeptOptions.forEach(item => {
        if (data === item.deptId) {
          this.form.deptName = item.deptName
          this.form.deptId = item.deptId
          this.chooseDep.id = item.deptId
          this.chooseDep.deptName = item.deptName
        }
      })
    }

  }
}
</script>
<style lang='scss' scoped>
.inputContent {
  display: flex;
  justify-content: space-between;
  border-bottom: 1px solid #eaedf4;
}
.flod {
  width: 100px;
  text-align: right;
  font-size: 14px;
  color: #333;
}
.flod span {
  cursor: pointer;
}
::v-deep .el-button {
  padding-left: 10px;
  padding-right: 10px;
  i.iconfont {
    font-size: 14px;
    margin-right: 8px;
  }
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
//.role {
//  ::v-deep .el-input__inner {
//    background: url('../../../assets/images/tanchu@2x.png') no-repeat;
//    background-size: 20px 20px;
//    background-position: 98% 7px;
//    box-sizing: border-box;
//    font-size: 14px;
//  }
//  ::v-deep .el-icon-arrow-up:before {
//    content: '';
//  }
//}
.popoverContent{
  font-size: 14px;
  .popoverHead{
    display:flex;
    position:relative;
    .icon-guanbi{
      position:absolute;
      right:0;
    }
    .popoverHead_img{
      width:20%;
      display: flex;
      align-items: center;
      text-align: center;
      .headImage{
        width:56px;
        height:56px;
        border-radius:50%;
      }
    }
    .popoverHead_info{
      width: 80%;
    }
    .popoverHead_jobNum{
      display:flex;
      align-items:center;
    }
  }
  .popoverP{
    margin: 10px 0;
    color:#333333;
  }

}
.dep{
  ::v-deep .el-form-item__label:after{
    content: '*';
    color: #ff4949;
    margin-right: 1px;
  }
}
::v-deep .el-upload-list{
  display: none;
}
.roleContainer{
  height: 500px;
  .roleLeftAndRight{
    display: flex;
    height: 90%;
  }
  .left{
    width: 50%;
  }
  .right{
    width: 50%;
    margin-left: 50px;
  }
}
::v-deep .el-tag.el-tag--info{
  background-color: #E9F2FF;
}
::v-deep .el-range__icon {
  display: none;
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
  ::v-deep .el-range__close-icon {
    position: absolute;
    right: 34px;
    top: 54%;
    transform: translateY(-50%);
  }
}
::v-deep .searchForm .el-form-item{
  margin-bottom: 12px
}
::v-deep .editForm .el-form-item{
  margin-bottom: 10px
}
::v-deep .userTable .el-table__body-wrapper.is-scrolling-left{
  height: 491px;
}
::v-deep .el-message-box{
  height: 500px;
  width: 360px;
}

::v-deep .el-select .el-input .el-select__caret{
  font-size: 20px;
}

.selectPost, .selectRole{
  ::v-deep .el-dialog .el-dialog__body{
    padding: 0;
  }
}
.selectDept{
  ::v-deep .el-dialog .el-dialog__body{
    padding: 0;
  }
  .head-search{
    padding-top: 16px;
    border-bottom: none;
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
  .icon-people {
    top: 7px;
    background: url("../../../assets/images/people.png") no-repeat;
    background-position: center;
    background-size: 100% 100%;
  }
  .icon-bumenxuanze {
    top: 0px
  }
  ::v-deep .el-select {
    width: 100%;
    .el-input__suffix-inner {
      display: none;
    }
  }
}
.roleL{
  overflow: hidden;
  white-space: nowrap;
  text-overflow: ellipsis;
}
.roler{
   overflow: hidden;
  white-space: nowrap;
  text-overflow: ellipsis;
}
</style>
<style>
.head-container .el-tree-node__content {
  padding: 6px;
  height: auto;
}
.left .el-tree-node__content {
  font-size: 16px;
  padding: 5px 0;
  height: auto;
}

/* .el-tree-node:focus > .el-tree-node__content{
    background-color: #D7E6FF;
  } */
</style>

