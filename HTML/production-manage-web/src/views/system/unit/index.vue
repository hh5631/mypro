<template>
  <div>
    <tags-view />
    <div class="app-container">
      <el-card style="min-height:40vh;">
        <div slot="header" class="clearfix">
          <div>
            <i class="iconfont icon-xiangqingxinxi" style="font-size:22px;color:#6A7697" />
            <span>{{ title }}</span>
          </div>
          <div>
            <el-button v-if="isEditLock" size="small" style="float: right;padding:6px 10px" plain @click="changeEdit"><i class="iconfont icon-bianji" style="margin-right: 5px;font-size:18px;color:#87b0f5;" />编辑</el-button>

            <el-button v-else size="small" style="float: right;padding:9px 18px;height:32px;line-height: 12px;width:64px" type="primary" @click="save">保存</el-button>
            <el-button v-if="!isEditLock" size="small" style="float: right;padding:9px 18px;height:32px;line-height: 12px;width:64px;margin-right:20px" plain @click="cancelEdit">取消</el-button>
          </div>
        </div>
        <div>
          <el-form ref="form" :rules="rules" :model="form" label-width="80px">
            <el-row :gutter="60">
              <el-col :offset="1" :span="7">
                <el-form-item prop="name" label="单位名称">
                  <el-tooltip v-if="form.name.length>13" :disabled="!isEditLock" effect="dark" :content="form.name" placement="top">
                    <el-input v-model="form.name" maxlength="60" :disabled="isEditLock" />
                  </el-tooltip>
                  <el-input v-else v-model="form.name" maxlength="60" :disabled="isEditLock" />
                </el-form-item>
              </el-col>
              <el-col style="padding:0;" :span="7">
                <el-form-item prop="account" label="统一社会信用代码" label-width="140px">
                  <el-input v-model="form.account" maxlength="18" :disabled="isEditLock" onkeyup="this.value = this.value.replace(/[^\A-\Z0-9]/g, '');" />
                </el-form-item>
              </el-col>
              <el-col :span="7" style="margin-left: 60px;">
                <el-form-item label="单位电话" prop="phone">
                  <el-input v-model="form.phone" :disabled="isEditLock" maxlength="13" />
                </el-form-item>
              </el-col>
            </el-row>
            <el-row :gutter="60">
              <!-- <el-col :offset="1" :span="8">
              <el-form-item label="帐号">
                <el-input v-model="form.account" :disabled="isEditLock" />
              </el-form-item>
            </el-col> -->
              <el-col :offset="1" :span="7">
                <el-form-item label="单位网址">
                  <el-tooltip v-if="form.network.length>24" :disabled="!isEditLock" effect="dark" :content="form.network" placement="top">
                    <el-input v-model="form.network" maxlength="60" :disabled="isEditLock" />
                  </el-tooltip>
                  <el-input v-else v-model="form.network" maxlength="60" :disabled="isEditLock" />
                </el-form-item>
              </el-col>
              <el-col style="padding:0 0 0 60px;" :span="7">
                <el-form-item label="单位传真" prop="fax">
                  <el-input v-model="form.fax" :disabled="isEditLock" maxlength="13" oninput="this.value = this.value.replace(/[^(-\d\d\d)]/g, '');" />
                </el-form-item>
              </el-col>
              <el-col :span="7" style="margin-left: 60px;">
                <el-form-item label="单位邮编" prop="email">
                  <el-input v-model="form.email" :disabled="isEditLock" maxlength="6" oninput="this.value = this.value.replace(/[^(-\d\d\d)]/g, '');" />
                </el-form-item>
              </el-col>
            </el-row>
            <el-row :gutter="60">
              <el-col :offset="1" :span="14" style="padding-right:0">
                <el-form-item label="单位地址">
                  <el-tooltip v-if="form.address.length>68" :disabled="!isEditLock" effect="dark" :content="form.address" placement="top">
                    <el-input v-model="form.address" maxlength="150" :disabled="isEditLock" />
                  </el-tooltip>
                  <el-input v-else v-model="form.address" maxlength="150" :disabled="isEditLock" />
                </el-form-item>
              </el-col>
              <el-col :span="7" style="margin-left: 60px;">
                <el-form-item label="开户行">
                  <el-tooltip v-if="form.openAccountBank.length>41" :disabled="!isEditLock" effect="dark" :content="form.openAccountBank" placement="top">
                    <el-input v-model="form.openAccountBank" maxlength="60" :disabled="isEditLock" />
                  </el-tooltip>
                  <el-input v-else v-model="form.openAccountBank" maxlength="60" :disabled="isEditLock" />
                </el-form-item>
              </el-col>
            </el-row>
            <el-row>
              <el-col :offset="1" :span="22" style="padding-right: 16px;">
                <el-form-item label="单位简介">
                  <el-input v-if="isReset" v-model="form.info" maxlength="500" :disabled="isEditLock" :resize="isEditLock?'none':''" autosize type="textarea" />
                </el-form-item>
              </el-col>
            </el-row>
          </el-form>
        </div>
      </el-card>
      <el-card v-loading="loading" style="margin-top:16px;min-height:10vh">
        <div slot="header" class="clearfix">
          <div>
            <i class="iconfont icon-xiangqingxinxi" style="font-size:22px;color:#6A7697" />
            <span>附件<!-- （{{ fileListCount }}） --></span>
          </div>

          <div style="float: right;display:flex">
            <el-upload
              ref="upload"
              :limit="1"
              :accept="fileAccept"
              :headers="upload.headers"
              :action="upload.url"
              :disabled="upload.isUploading"
              :before-upload="beforeUpload"
              :on-progress="handleFileUploadProgress"
              :on-success="handleFileSuccess"
              :show-file-list="false"
            >
              <el-button size="small" style="padding:7px" plain @click="handleImport"><i class="iconfont icon-daochu" />上传附件</el-button>
            </el-upload>
            <el-button size="small" style="padding:6px 10px;margin-left:8px;width:100%;" plain @click="batchDownload"><i class="iconfont icon-daoru" style="margin-right: 5px;color:#4686EF;" />批量下载</el-button>
          </div>
        </div>
        <div class="fujian">
          <div v-for="value in fileListfj" :key="value.id" class="fujian-content" :class="{'checkedColor':value.checked}">
            <div class="fujian-left">
              <img v-if="value.type==='doc'||value.type==='docx'" src="@/assets/images/word@2x.png" alt="">
              <img v-if="value.type==='jpg'" src="@/assets/images/jpg@2x.png" alt="">
              <img v-if="value.type==='jpeg'" src="@/assets/images/jpeg@2x.png" alt="">
              <img v-if="value.type==='png'" src="@/assets/images/png@2x.png" alt="">
              <img v-if="value.type==='pdf'" src="@/assets/images/pdf@2x.png" alt="">
              <img v-if="value.type==='xls'||value.type==='xlsx'" src="@/assets/images/excel@2x.png" alt="">
              <img v-if="value.type==='txt'" src="@/assets/images/txt@2x.png" alt="">
              <img v-if="value.type==='html'" src="@/assets/images/html@2x.png" alt="">
              <img v-if="value.type==='csv'" src="@/assets/images/csv@2x.png" alt="">
              <img v-if="value.type==='ppt'" src="@/assets/images/ppt@2x.png" alt="">
              <img v-if="value.type==='pptx'" src="@/assets/images/pptx@2x.png" alt="">
            </div>
            <div class="fujian-right">
              <div class="fujian-title">
                <div class="title">{{ value.name.slice( 0 , value.name.lastIndexOf('_')) }}.{{ value.type }}</div>
                <div style="padding:4px 12px 0 0"><el-checkbox :key="value.id" v-model="value.checked" @change="change(value)" /></div>
              </div>
              <div class="fujian-click">
                <el-button size="small" type="text" @click="filePreview1(value.location)"><i class="iconfont icon-yulan" />预览</el-button>
                <el-button v-if="value.type==='jpg'||value.type==='jpeg'||value.type==='png'" size="small" type="text" @click="downloadIamge(value.location,value.name.slice( 0 , value.name.lastIndexOf('_')),value.type)"><i class="iconfont icon-daoru" />下载</el-button>
                <el-button v-else-if="value.type==='pdf'||value.type==='txt'||value.type==='html'" size="small" type="text" @click="fileLinkToStreamDownload(value.location,value.name.slice( 0 , value.name.lastIndexOf('_')))"><i class="iconfont icon-daoru" />下载</el-button>
                <!-- <el-button v-else-if="value.type==='txt'" type="text" @click="fileLinkToStreamDownload(value.location,value.name.slice( 0 , value.name.lastIndexOf('_')))"><i class="iconfont icon-daoru" />下载</el-button> -->
                <!-- <el-button v-else-if="value.type==='html'" type="text" @click="fileLinkToStreamDownload(value.location,value.name.slice( 0 , value.name.lastIndexOf('_')))"><i class="iconfont icon-daoru" />下载</el-button> -->
                <el-button v-else size="small" type="text" @click="preview(value.location)"><i class="iconfont icon-daoru" />下载</el-button>
                <el-button size="small" type="text" @click="deleteFile(value.id)"><i class="iconfont icon-shanchu" />删除</el-button>
              </div>
            </div>
          </div>
        </div>

      </el-card>
    </div>
    <el-dialog
      title="预览"
      :visible.sync="fileDig"
      append-to-body
      width="1200"
    >
      <filePreview :src="url" />
    </el-dialog>
  </div>

</template>

<script>
import { unitInfo, setUnitInfo, addFile, delFile, getRealPath } from '@/api/system/unit'
import TagsView from '@/layout/components/TagsView/index.vue'
import { getToken } from '@/utils/auth'
import filePreview from '@/components/FilePreview/index'
import FormValidators from '../../../vform/utils/validators'
export default {
  name: 'Unit',
  components: { TagsView, filePreview },
  dicts: ['sys_normal_disable'],
  data() {
    return {
      // 表单参数
      form: {
        name: '',
        phone: '',
        account: '',
        network: '',
        openAccountBank: '',
        fax: '',
        email: '',
        address: '',
        // account: '',
        info: '',
        userId: ''
      },
      fileType: ['.xlsx', '.xls', '.ppt', '.pptx', '.doc', '.docx', '.pdf', '.csv', ' .html', '.png', '.jpg', '.jpeg', '.txt', '.docx', '.xlsx'],
      //  是否编辑
      isEditLock: true,
      isChecked: false,
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
        url: process.env.VUE_APP_BASE_API + '/file/upload'
      },
      rules: {
        name: [{ required: true, message: '请填写单位名称', trigger: 'blur' }],
        account: [
          { required: true, message: '请填写统一社会信用代码', trigger: 'blur' },
          { min: 18, message: '统一社会信用代码为18位', trigger: 'blur' }
        ],
        fax: [
          { min: 7, message: '单位传真位数应不少于7位', trigger: 'blur' }
        ],
        phone: [
          // { min: 7, message: '电话号码位数应不少于7位', trigger: 'blur' },
          { validator: FormValidators.allPhone, message: '请输入正确的电话', trigger: 'blur' }
        ],
        email: [
          { min: 6, message: '单位邮编位数应该为6位', trigger: 'blur' }
        ]
      },
      userId: this.$store.state.user.id,
      // 新上传的那个文件的信息
      fileMessage:
        {
          companyId: '',
          id: '',
          location: '',
          name: '',
          type: ''
        },
      // 文件列表
      fileListfj: [],
      // 文件个数
      fileListCount: 0,
      title: '详情',
      loading: false,
      checkedArr: [],
      isCheckedColor: false,
      isReset: true,
      fileDig: false,
      url: ''
    }
  },
  computed: {
    fileAccept() {
      return this.fileType.map(item => `${item}`).join(',')
    },
    fileTypeAll () {
      return [...this.fileType, ...this.fileType.map(item => item.toUpperCase())]
    }
  },
  created() {
    this.isEditLock = true
    this.getCompanyInfo()
    this.title = '详情'
  },
  mounted() {
    this.getCompanyInfo()
  },
  // activated() {
  //   this.isEditLock = true
  //   this.getCompanyInfo()
  //   this.title = '详情'
  // },
  methods: {
    // 取消编辑
    cancelEdit() {
      this.isEditLock = true
      this.getCompanyInfo() // 取消后重新恢复数据
    },
    change(data) {
      this.fileListfj.splice()
      // if (data.checked === true) {
      //   this.checkedArr.push(data)
      //   // this.isCheckedColor = true
      // } else {
      //   this.checkedArr.forEach((item, index) => {
      //     if (item.checked === false) {
      //       this.checkedArr.splice(index, 1)
      //       // this.isCheckedColor = false
      //     }
      //   })
      // }
    },
    changeEdit() {
      this.isEditLock = false
      this.title = '编辑'
    },
    getCompanyInfo() {
      this.isReset = false
      this.$nextTick(() => {
        this.isReset = true
      })
      unitInfo(this.userId).then(res => {
        this.loading = false
        this.form = res.data
        this.fileMessage.companyId = res.data.id
        this.fileListfj = res.data.companyFileList
        this.fileListfj.forEach(item => {
          item.checked = false
        })

        this.fileListCount = 0
      })
    },
    save() {
      this.$refs['form'].validate((valid) => {
        if (valid) {
          this.form.userId = this.userId
          setUnitInfo(this.form).then((response) => {
            if (response.code === 200) {
              this.$message({
                message: '保存成功',
                type: 'success',
                duration: 1000
              })
              this.isEditLock = true
              this.title = '详情'
              this.isReset = false
              this.$nextTick(() => {
                this.isReset = true
              })
            }
          })
        }
      })
    },
    handleImport() {
      this.upload.title = '上传附件'
      this.upload.open = true
    },
    handleFileUploadProgress(event, file, fileList) {
      this.upload.isUploading = true
    },
    beforeUpload(file) {
      this.loading = true
      return new Promise((resolve, reject) => {
        const fileExtension = file.name.slice(file.name.lastIndexOf('.'))
        const preFileName = file.name.slice(0, file.name.lastIndexOf('.'))
        const isRightName = /^[\u4e00-\u9fa5a-zA-Z0-9]+$/.test(preFileName)// 文件名不支持特殊字符
        const index = this.fileTypeAll.indexOf(fileExtension)
        if (index === -1) {
          this.$message.error('上传文件的格式不符合要求')
          this.loading = false
          reject(false)
        } else if (!isRightName) {
          this.$message.error('文件名称不支持特殊符号字符，请重新编辑文件名')
          this.loading = false
          reject(false)
        } else if ((file.size / 1024 / 1024) > 100) {
          this.$message.error('请上传小于100M的文件')
          this.loading = false
          reject(false)
        } else {
          // 成功
          resolve(true)
        }
      })
    },
    handleFileSuccess(response, file, fileList) {
      this.fileMessage.location = response.data.url
      this.fileMessage.name = response.data.name
      this.fileMessage.type = this.fileMessage.name.substring(this.fileMessage.name.lastIndexOf('.') + 1)
      addFile(this.fileMessage).then((res) => {
        this.getCompanyInfo()
      })
      this.upload.open = false
      this.upload.isUploading = false
      this.$refs.upload.clearFiles()
      // this.$alert("<div style='overflow: auto;overflow-x: hidden;max-height: 70vh;padding: 10px 20px 0;'>" + response.msg + '</div>', '导入结果', { dangerouslyUseHTMLString: true })
      this.$message.success('上传成功')
    },
    submitFileForm() {
      this.$refs.upload.submit()
      // upload(() => {})
      // addFile()
    },
    importTemplate() {
      this.download('system/user/importTemplate', {
      }, `user_template_${new Date().getTime()}.xlsx`)
    },
    deleteFile(id) {
      const obj = { fileIds: [] }
      obj.fileIds.push(id)
      this.$confirm('确定删除该文件?', '确定删除', {
        confirmButtonText: '确定',
        cancelButtonText: '取消',
        type: 'warning'
      }).then(() => {
        this.loading = true
        delFile(obj).then((res) => {
          this.getCompanyInfo()
        })
        this.$message({
          type: 'success',
          message: '删除成功'
        })
      }).catch(() => {
      })
    },
    preview(url) {
      window.open(url)
    },
    batchDownload() {
      //
      // let target = e.target
      // if (target.nodeName === 'I') {
      //   target = e.target.parentNode
      // }
      // target.blur()
      this.checkedArr = this.fileListfj.filter(item => item.checked === true)

      if (this.checkedArr.length === 0) {
        this.$message.error('请选择你要下载的文件')
        return
      }
      // let jpg = []
      // jpg = this.fileListfj.filter(item => item.type === 'jpg' || item.type === 'jpeg' || item.type === 'png')
      //
      this.checkedArr.forEach(item => {
        if (item.type === 'jpg' || item.type === 'jpeg' || item.type === 'png') {
          this.downloadIamge(item.location, item.name.slice(0, item.name.lastIndexOf('_')), item.type)
        } else if (item.type === 'pdf' || item.type === 'txt' || item.type === 'html') {
          this.fileLinkToStreamDownload(item.location, item.name.slice(0, item.name.lastIndexOf('_')))
        } else {
          // this.preview(item.location))
          const iframe = document.createElement('iframe')
          iframe.style.display = 'none'
          iframe.style.height = 0
          iframe.src = item.location
          document.body.appendChild(iframe)
          // 1s之后删除
          setTimeout(() => {
            iframe.remove()
          }, 1000)
        }
      })
      this.$nextTick(() => {
        this.$notify({
          title: '下载完成',
          message: `共${this.checkedArr.length}个文件已下载至本地。`,
          type: 'success',
          duration: 1000
        })
        this.getCompanyInfo()
      })
    },
    downloadIamge(imgsrc, name, type) {
      // 下载图片地址和图片名
      var image = new Image()
      // 解决跨域 Canvas 污染问题
      image.setAttribute('crossOrigin', 'anonymous')
      image.onload = function() {
        var canvas = document.createElement('canvas')
        canvas.width = image.width
        canvas.height = image.height
        var context = canvas.getContext('2d')
        context.drawImage(image, 0, 0, image.width, image.height)
        let url
        if (type === 'png') {
          url = canvas.toDataURL('image/png') // 得到图片的base64编码数据
        } else if (type === 'jpg' || type === 'jpeg') {
          url = canvas.toDataURL('image/jpeg') // 得到图片的base64编码数据
        }
        var a = document.createElement('a') // 生成一个a元素
        var event = new MouseEvent('click') // 创建一个单击事件
        a.download = name || 'photo' // 设置图片名称
        a.href = url // 将生成的URL设置为a.href属性
        a.dispatchEvent(event) // 触发a的单击事件
      }
      image.src = imgsrc
    },
    fileLinkToStreamDownload(url, fileName) {
      const xhr = new XMLHttpRequest()
      xhr.open('get', url, true)
      xhr.setRequestHeader('Content-Type', `application/pdf`)
      xhr.responseType = 'blob'
      const that = this
      xhr.onload = function() {
        if (this.status === 200) {
          // 接受二进制文件流
          var blob = this.response
          that.downloadExportFile(blob, fileName)
        }
      }
      xhr.send()
    },
    // downloadTxt(url, fileName) {
    //   // const strData = JSON.stringify(json)
    //   // const T_URL = window.URL.createObjectURL(new Blob([strData]))
    //   const LINK = document.createElement('a')
    //   LINK.href = url
    //   LINK.setAttribute('download', fileName)
    //   LINK.click()
    //   window.URL.revokeObjectURL(url)
    // },
    downloadExportFile(blob, tagFileName) {
      const downloadElement = document.createElement('a')
      let href = blob
      if (typeof blob === 'string') {
        downloadElement.target = '_blank'
      } else {
        href = window.URL.createObjectURL(blob) // 创建下载的链接
      }
      downloadElement.href = href
      downloadElement.download =
        tagFileName
      // 下载后文件名
      downloadElement.click() // 点击下载
      // document.body.removeChild(downloadElement) // 下载完成移除元素
      if (typeof blob !== 'string') {
        window.URL.revokeObjectURL(href) // 释放掉blob对象
      }
    },
    filePreview1(url) {
      // getRealPath(url).then(res => {
      //   window.open(this.filePreview(res.data))
      // })
      getRealPath(url).then(res => {
        this.url = this.filePreview(res.data)
      })
      this.fileDig = true
      // window.open(url, '_blank')
      //
      // window.open(this.filePreview(url))
      // window.open(`http://127.0.0.1:8012/onlinePreview?url=` + encodeURIComponent(base64Encode(url)))
    }
    // 改downs这个函数就行，downloadIamge（‘图片下载地址’，图片名字）
    // downs(index) {
    //   // 主要是为了名字不重复
    //   var name = new Date().getTime()
    //   this.downloadIamge(this.result[index].newImgUrl, `${name}`)
    // }
  }
}
</script>
<style lang="scss" scoped>
.app-container {
  padding: 16px;
}
.clearfix{
  // height:36px;
  // line-height: 36px;
  display: flex;
  align-items: center;
  justify-content: space-between;
}
.clearfix span {
  font-size: 18px;
  margin-left: 8px;
}
.fujian{
  display:flex;
  flex-wrap: wrap;
}
.fujian .fujian-content{
  display:flex;
  border: 1px solid #DDDDDD;
  border-radius: 2px;width:20vw;
  margin:0 15px 15px 0;
  padding-top: 5px;
}
.fujian .checkedColor{
  border: 1px solid #5395FF;
}
.fujian-left{
  width: 16%;
}
.fujian-right{
  width: 80%;
}
.fujian-left img{
  width: 56px;
  height: 56px;
}
.fujian-right .fujian-title{
  width:100%;
  word-break: break-all;
  display: flex;
  justify-content: space-between;
  font-size: 16px;
  color: #333333;
  .title{
    width: 70%;
    white-space: nowrap;
    overflow: hidden;
    text-overflow: ellipsis;
  }
}
.fujian-right .fujian-click{
  font-size:14px;
}
.fujian-click i{
  margin-right:4px;
}
.el-button--text{
  color: #333;
}
.el-button--text:hover{
  color: #4686EF;
}
::v-deep .el-form-item__label{
  color: #6A7697;
}
.icon-daochu{
  margin-right: 5px;
  color:#4686EF;
}
::v-deep .el-input.is-disabled .el-input__inner{
  color: #333333;
  cursor: default;
}
::v-deep .el-textarea.is-disabled .el-textarea__inner{
  color: #333333;
  cursor: default;
}
::v-deep .el-input__inner{
  height: 40px;
  line-height: 40px;
}
// .info{
//   ::v-deep .el-card{
//   min-height:calc(100vh - 182px);
// }
// }
::v-deep .el-card__header{
  padding: 16px 15px;
}
::v-deep .el-col-offset-1{
  margin-left: 100px;
}
::v-deep .el-checkbox__inner{
  width: 16px;
  height: 16px;
}
::v-deep .el-card__body{
  padding-bottom: 0;
}
</style>
