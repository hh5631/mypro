<template>
        <div class="bl"></div>
        <div id="in" class="bt">欢迎注册</div>
        <div id="in"><input type="text" id="put"  placeholder="请输入用户名" v-model="uname"></div>
        <div id="in"><input type="text" id="put" placeholder="请输入密码" v-model="passwd"></div>
        <div id="in"><button @click="login">{{ login_btn }}</button>
             <button @click="register">{{ register_btn}}</button>
        </div>
    </template>
    
    <script setup>
    import axios from 'axios'
    import { ref } from 'vue'
    import { useRouter } from 'vue-router'
    
        const router = useRouter() // 在 setup 中获取 router 实例
        const login_btn = ref('返回')
        const register_btn =  ref('注册')
        const uname = ref('')
        const passwd = ref('')
    
        const login =()=>{
          router.push('/login')
        }
        const register =()=>{
          register_btn.value='注册中'
          console.log(uname,passwd)
          axios.get('http://localhost/register',{
            params:{
              username: uname.value,
              password: passwd.value
            }
          }
         ).then((res) => {
          alert(res.data)
        }).catch((err) => {
          alert(err)
        })
        }
     
    </script>
    
    <style>
    .bl{
      height: 40px;
      text-align: center;
    }
    .bt{
      font-size: 20px;
      line-height: 1.5;
      color:rgb(208, 176, 205);
    }
    #in{
      text-align: center;
      margin: 10px 5px 0px 0px;
    }
    
    #put{
      border: 1px solid #b79d75;
      border-radius: 5px;
      
    }
    button {
      margin: 0px 10px 10px 0px;
      background-color: rgb(231, 228, 210);
      border-radius: 8px;
      border: 1px solid #f4f2ef;
    }
    </style>
    