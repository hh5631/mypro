<template>
        <div class="bl">
                <input type="file" id="file-input" ref="fileInput">
              
        </div>
        <br>    
        <div class="bl">
        <button @click="trans">{{ trans_btn }}</button>
                <button @click="downLoad">{{ downLoad_btn }}</button>
        </div>
        
        


</template>

<script setup>
import axios from 'axios';
import { ref } from 'vue'


const trans_btn = ref('上传');
const downLoad_btn =  ref('下载')
const trans =async () => {
        const formData = new FormData();
        const fileInput = document.getElementById('file-input')
        formData.append('file', fileInput.files[0]);

    
        try {
                const response = await axios.post('http://localhost/upload', formData);
                const response2 = await axios.get('http://localhost/showAll');
                response2.data.forEach(element => {
                        console.log(element.fileName+element.filePath);
                })
                alert(response.data);
                
        } catch (error) {
                alert(error);
        }
}
const downLoad =async ()=>{
        try {
        const fileInput = document.getElementById('file-input');
        const response = await axios.get('http://localhost/download', {
            params: {
                filename: fileInput.files[0].name
            },
        });

        const url = window.URL.createObjectURL(new Blob([response.data]));

// 创建一个链接元素
        const link = document.createElement('a');
        link.href = url;
        link.setAttribute('download', fileInput.files[0].name);  // 设置下载文件的名称
        // 触发点击链接操作
        link.click();
    } catch (error) {
        alert('下载文件时出现错误：', error);
    }
}
</script>

<style>


#file-input {
  padding: 10px;
  margin-bottom: 10px;
}


button:hover {
  background-color: #bdc2c6;
}

.bl {
        text-align: center;
}

</style>