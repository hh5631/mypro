<template>
  <div class="navbar">
    <!-- <hamburger id="hamburger-container" :is-active="sidebar.opened" class="hamburger-container" @toggleClick="toggleSideBar" /> -->

    <!-- <top-nav id="topmenu-container" class="topmenu-container" v-if="topNav"/> -->

    <div class="right-menu">
      <template v-if="device!=='mobile'">
        <!-- <search id="header-search" class="right-menu-item" /> -->

        <!-- <screenfull id="screenfull" class="right-menu-item hover-effect" /> -->

        <!-- <el-tooltip content="布局大小" effect="dark" placement="bottom">
          <size-select id="size-select" class="right-menu-item hover-effect" />
        </el-tooltip> -->

      </template>
      <div class="avatar-wrapper">

        <el-badge :value="unreads" :hidden="unreads===0" class="iconNum" :max="99">
          <i class="el-icon-bell" @click="enterMessage" />
        </el-badge>
        <img :src="avatar" class="user-avatar">
        <span class="name">{{ name }} -</span>
        <!-- <div class="line" /> -->
        <span class="job">{{ postName.posts.length>0?postName.posts[0].postName:'无职位' }}</span>
        <div class="line" />
        <!-- <i class="iconfont icon-zhuye" />
        <div style="position: relative;">
          <i class="iconfont icon-xinxi" />
          <div
            style="width: 20px;height: 20px;background: #FF4D4F;border-radius: 10px;border: 1px solid #FFFFFF;position: absolute;top: 4px;left: 22px;font-size:12px;display:flex;
justify-content: center;align-items:center;color:#fff;"
          >
            55
          </div>
        </div>
        <i class="iconfont icon-yuyan" />
        <i class="iconfont icon-xitongshezhi" /> -->
        <i class="iconfont icon-tuichu" style="margin:0 10px;" @click="logout" />
      </div>
      <!-- <el-dropdown class="avatar-container right-menu-item hover-effect" trigger="click">
        <div class="avatar-wrapper">
          <img :src="avatar" class="user-avatar">
          <i class="el-icon-caret-bottom" />
        </div>
        <el-dropdown-menu slot="dropdown">
          <router-link to="/user/profile">
            <el-dropdown-item>个人中心</el-dropdown-item>
          </router-link>
          <el-dropdown-item @click.native="setting = true">
            <span>布局设置</span>
          </el-dropdown-item>
          <el-dropdown-item divided @click.native="logout">
            <span>退出登录</span>
          </el-dropdown-item>
        </el-dropdown-menu>
      </el-dropdown> -->
    </div>
    <el-dialog
      :title="msgTitle"
      :visible.sync="messageDig"
      width="1000px"
      class="showAllDialog"
    >
      <Message ref="addInvoice" class="message" @unread="unread" />
    </el-dialog>
  </div>
</template>

<script>
import { mapGetters } from 'vuex'
import Message from './message/message.vue'
import { getNumByReadType } from '@/api/assetChange/index'
import eventBus from '@/utils/bus.js'
// import TopNav from '@/components/TopNav'
// import Hamburger from '@/components/Hamburger'
// import Screenfull from '@/components/Screenfull'
// import SizeSelect from '@/components/SizeSelect'
// import Search from '@/components/HeaderSearch'

export default {
  components: {
    // TopNav,
    // Hamburger,
    // Screenfull,
    // SizeSelect,
    // Search
    Message
  },

  data() {
    return {
      name: this.$store.state.user.name,
      postName: this.$store.state.user,
      messageDig: false,
      msgTitle: '',
      unreads: null,
      wsInstance: ''
    }
  },
  computed: {
    ...mapGetters([
      'sidebar',
      'avatar',
      'device'
    ]),
    setting: {
      get() {
        return this.$store.state.settings.showSettings
      },
      set(val) {
        this.$store.dispatch('settings/changeSetting', {
          key: 'showSettings',
          value: val
        })
      }
    },
    topNav: {
      get() {
        return this.$store.state.settings.topNav
      }
    }
  },
  watch: {
    // 监听不直接使用父级数据
    tableDatas(val) {
      this.tableData = val
      // this.total = val.length
    }

  },
  created() {
    this.getMsgNum()
    eventBus.$on('getMsgCount', (payload) => {
      this.unreads = payload
    })
    this.wsInstance = this.$store.state.user.wsInstance
  },

  // beforeDestroy() {
  //   eventBus.$off('getAssetHandleDetail')
  // },
  methods: {
    toggleSideBar() {
      this.$store.dispatch('app/toggleSideBar')
    },
    enterMessage() {
      this.messageDig = true
      this.msgTitle = '消息通知'
    },
    async logout() {
      this.$store.dispatch('LogOut').then(() => {
        // location.href = '/index'
        this.$router.push('/login')
        // 断开websocket通信
        if (this.wsInstance) {
          this.wsInstance.close()
        }
      })
    },
    getMsgNum() {
      // 已读未读数据查询
      getNumByReadType({ readStatus: 0 }).then(res => {
        if (res.code === 200) {
          this.unreads = res.data
        }
      })
    },
    unread(val) {
      this.unreads = val
    }

  }
}
</script>

<style lang="scss" scoped>
.navbar {
  height: 56px;
  overflow: hidden;
  position: relative;
  background: #fff;
  // box-shadow: 0 1px 4px rgba(0, 21, 41, 0.08);
  border-bottom: 1px solid #DDDDDD;

.iconNum{
  margin-right: 30px;
  margin-top:12px ;
}
::v-deep .el-dialog__body{
  padding: 0px;
}
  .hamburger-container {
    line-height: 46px;
    height: 100%;
    float: left;
    cursor: pointer;
    transition: background 0.3s;
    -webkit-tap-highlight-color: transparent;

    &:hover {
      background: rgba(0, 0, 0, 0.025);
    }
  }

  .topmenu-container {
    position: absolute;
    left: 50px;
  }

  .errLog-container {
    display: inline-block;
    vertical-align: top;
  }

  .right-menu {
    float: right;
    height: 100%;
    line-height: 50px;

    &:focus {
      outline: none;
    }

    .right-menu-item {
      display: inline-block;
      padding: 0 8px;
      height: 100%;
      font-size: 18px;
      color: #5a5e66;
      vertical-align: text-bottom;

      &.hover-effect {
        cursor: pointer;
        transition: background 0.3s;

        &:hover {
          background: rgba(0, 0, 0, 0.025);
        }
      }
    }

    .avatar-wrapper {
      display: flex;
      align-items: center;
      .user-avatar{
        width:32px;
        height:32px;
        border-radius:50%;
      }
      .name{
        padding:0 0 0 10px;
        font-size: 16px;
      }
      .job{
        font-size: 16px;
        padding:0 10px;
      }
      .line{
        border-right:1px solid #DDDDDD;
        width:1px;
        height:30px;
      }
      i{
        margin-left:10px;
        font-size: 24px;
      }
    }
  }
  i {
    cursor: pointer;
  }
}
</style>
