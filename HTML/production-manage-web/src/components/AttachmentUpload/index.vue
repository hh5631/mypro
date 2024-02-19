<template>
  <div>
    <div class="upload-box">
      <el-upload
        v-if="!readonly"
        ref="upload"
        multiple
        :accept="fileAccept"
        :headers="headers"
        :action="uploadFileUrl + '?updateSupport=' + updateSupport"
        :disabled="isUploading"
        :before-upload="beforeUpload"
        :on-progress="handleFileUploadProgress"
        :on-success="handleFileSuccess"
        :show-file-list="false"
      >
        <el-button style="padding:7px" plain size="small"><i class="iconfont icon-zengjia upload-icon" />添加附件</el-button>
      </el-upload>
      <!-- 提示信息 -->
      <span v-if="!readonly" class="file-tip">{{ fileTip }}</span>
    </div>
    <div class="file-list-body">
      <div v-for="(item,index) in fileList" :key="index" class="file-list">
        <!-- 单行显示文件名 -->
        <!-- <span class="file-name" @click="previewFile(item.location)">{{ item.name }}</span> -->
        <!-- 缩略图显示 -->
        <!-- <img
          :src="filterUrl(item)"
          width="108px"
          height="88px"
          @click="previewFile(item.location)"
        > -->
        <img v-if="item.type==='image/png'" key="png" class="img-show" :src="`${picUrl}${item.location}`" @click="previewFile(item.location)">
        <img v-if="item.type==='image/jpg'" key="jpg" class="img-show" :src="`${picUrl}${item.location}`" @click="previewFile(item.location)">
        <img v-if="item.type==='image/jpeg'" key="jpeg" class="img-show" :src="`${picUrl}${item.location}`" @click="previewFile(item.location)">
        <img v-if="item.type==='application/pdf'" key="pdf" class="img-show" src="@/assets/images/pdf@2x.png" @click="previewFile(item.location)">
        <img v-if="item.type==='application/msword'" key="doc" class="img-show" src="@/assets/images/word@2x.png" @click="previewFile(item.location)">
        <img v-if="item.type==='application/vnd.ms-excel'" key="xls" class="img-show" src="@/assets/images/excel@2x.png" @click="previewFile(item.location)">
        <img v-if="item.type==='application/vnd.openxmlformats-officedocument.spreadsheetml.sheet'" key="sheet" class="img-show" src="@/assets/images/excel@2x.png" @click="previewFile(item.location)">
        <img v-if="item.type==='application/vnd.openxmlformats-officedocument.wordprocessingml.document'" key="document" class="img-show" src="@/assets/images/word@2x.png" @click="previewFile(item.location)">
        <i v-if="!readonly" class="delete-icon el-icon-delete" @click="deleteFile(index)" />
      </div>
    </div>
    <!-- 上传资源预览 -->
    <el-dialog
      title="预览"
      :visible.sync="previewShow"
      append-to-body
      width="1200"
    >
      <file-preview :src="previewUrl" />
    </el-dialog>
  </div>
</template>

<script>
import { getToken } from '@/utils/auth'
import { getRealPath } from '@/api/system/unit'
import FilePreview from '@/components/FilePreview/index'
import { deepClone } from '@/utils'

export default {
  name: 'AttachmentUpload',
  components: {
    FilePreview
  },
  // 修改组件的双向绑定
  model: {
    prop: 'fileList',
    event: 'change' // 代表自定义的事件名称
  },
  props: {
    fileList: {
      type: Array,
      required: true,
      default: () => []
    },
    // 大小限制(MB)
    fileSize: {
      type: Number,
      default: 5
    },
    // 文件类型, 例如['png', 'jpg', 'jpeg']
    fileType: {
      type: Array,
      default: () => ['png', 'jpg', 'jpeg', 'pdf', 'doc', 'docx', 'xls', 'xlsx']// 'rar', 'zip'压缩文件暂不支持
    },
    // 是否更新已经存在的用户数据
    updateSupport: {
      type: Number,
      default: 0
    },
    // 是否只读浏览模式
    readonly: {
      type: Boolean,
      default: false
    }
  },
  data () {
    return {
      headers: { Authorization: 'Bearer ' + getToken() },
      uploadFileUrl: process.env.VUE_APP_BASE_API + '/file/upload',
      isUploading: false, // 是否禁用上传
      previewUrl: '',
      previewShow: false,
      picUrl: process.env.VUE_APP_PIC_URL,
      // pdf
      isPdf: false
    }
  },
  computed: {
    // 接收上传资源类型
    fileAccept() {
      return this.fileType.map(item => `.${item}`).join(',')
    },
    // 文件提示
    fileTip () {
      return `支持文件格式：${this.fileType.join('、')}，单个文件不能超过${this.fileSize}M。`
    },
    fileTypeAll () {
      return [...this.fileType, ...this.fileType.map(item => item.toUpperCase())]
    }
  },
  methods: {
    /** 上传文件之前的钩子
     * 参数 file 就是点击上传的文件，若返回 false 或者返回 Promise 且被 reject，则停止上传。
    */
    beforeUpload(file) {
      return new Promise((resolve, reject) => {
        // 获取文件扩展名
        const fileExtension = file.name.slice(file.name.lastIndexOf('.') + 1)
        const preFileName = file.name.slice(0, file.name.lastIndexOf('.'))
        // 文件名不支持特殊字符
        const isRightName = /^[\u4e00-\u9fa5a-zA-Z0-9]+$/.test(preFileName)
        const index = this.fileTypeAll.indexOf(fileExtension)
        if (index === -1) {
          this.$message.error('上传文件的格式不符合要求')
          reject(false)
        } else if (!isRightName) {
          this.$message.error('文件名称不支持特殊符号字符，请重新编辑文件名')
          reject(false)
        } else if ((file.size / 1024 / 1024) > this.fileSize) {
          this.$message.error(`请上传小于${this.fileSize}M的文件`)
          reject(false)
        } else {
          // 成功
          resolve(true)
        }
      })
    },
    /** 文件上传时 */
    handleFileUploadProgress() {
      this.isUploading = true
    },
    /** 文件上传成功时 */
    handleFileSuccess(response, file) {
      if (response.code === 200) {
        const obj = {
          name: response.data.name,
          location: response.data.url,
          type: file.raw.type
        }
        const fileList = deepClone(this.fileList)
        fileList.push(obj)
        console.log(fileList)
        this.$emit('change', fileList)

        this.$message.success('上传成功')
      } else {
        this.$message.error(response.msg)
      }
      this.isUploading = false
    },
    /** 点击上传文件时获取预览地址 */
    previewFile(url) {
      getRealPath(url).then(res => {
        this.previewUrl = this.filePreview(res.data)
        this.previewShow = true
      })
    },
    /**  删除文件 */
    deleteFile(index) {
      const fileList = deepClone(this.fileList)
      fileList.splice(index, 1)
      this.$emit('change', fileList)
    },
    /** 根据返回类型，图片显示缩略图，文件显示默认图标 */
    filterUrl(item) {
      const map = new Map([
        ['image/png', `${this.picUrl}${item.location}`],
        ['application/pdf', `@/assets/images/profile.jpg`],
        ['application/vnd.openxmlformats-officedocument.wordprocessingml.document', '@/assets/404_images/404.png']
      ])
      return map.get(item.type)
    }
  }
}
</script>

<style scoped lang="scss">
.upload{
  display: flex;
  align-items: center;
}
.upload-box {
  display: flex;
  align-items: center;
}
.upload-icon {
  color: #4686EF;
}
.file-tip{
  color: #B2B5B9;
  font-size: 14px;
  padding-left: 8px;
}
.file-list{
  display: flex;
  align-items: center;
  margin-top: 10px;
  padding: 0 10px;
  cursor: pointer;
  font-size: 14px;
  &:hover{
    color: #4686EF;
  }
  .delete-icon {
    margin-top: -72px;
    margin-left: -15px;
    color: #fff;
    background-color: #333;
  }
}
// .file-name {
//   flex: 1;
// }
.file-list-body {
  display: flex;
  flex-wrap: wrap;
}
.attachment-icon {
  color: #4686EF;
  margin-right: 8px;
  font-size: 20px;
}
.select-icon{
  cursor: pointer;
}
.img-show{
  width: 108px;
  height: 88px
}

</style>
