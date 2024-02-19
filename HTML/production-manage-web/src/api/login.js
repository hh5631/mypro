import request from '@/utils/request'

// 登录方法
export function login(username, password, code, uuid, browser) {
  return request({
    url: '/auth/login',
    headers: {
      isToken: false
    },
    method: 'post',
    data: { username, password, code, uuid, browser }
  })
}

// 注册方法
export function register(data) {
  return request({
    url: '/auth/register',
    headers: {
      isToken: false
    },
    method: 'post',
    data: data
  })
}

// 刷新方法
export function refreshToken() {
  return request({
    url: '/auth/refresh',
    method: 'post'
  })
}

// 获取用户详细信息
export function getInfo() {
  return request({
    url: '/system/user/getInfo',
    method: 'get'
  })
}

// 退出方法
export function logout() {
  return request({
    url: '/auth/logout',
    method: 'delete'
  })
}

// 获取验证码
export function getCodeImg() {
  return request({
    url: '/code',
    headers: {
      isToken: false
    },
    method: 'get',
    timeout: 20000
  })
}

// 检查用户与手机号是否对应
export function checkUser(query) {
  return request({
    url: '/system/user/checkUser',
    method: 'get',
    params: query
  })
}

// 用户密码重置
export function resetPassword(data) {
  return request({
    url: '/system/user/resetPassword',
    method: 'post',
    data: data
  })
}
//  发送验证码
export function sendSmsCode(phone) {
  return request({
    url: 'auth/sms/sendSmsCode',
    method: 'post',
    data: phone
  })
}
//  检查验证码
export function checkSmsCode(data) {
  return request({
    url: 'auth/sms/checkSmsCode',
    method: 'post',
    data: data
  })
}

