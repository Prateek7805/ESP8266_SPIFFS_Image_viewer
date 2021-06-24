var debug = false;
imgJSON = ['i-1.jpg','i-2.jpg','i-3.jpg','i-4.jpg','i-5.jpg','i-6.jpeg']
function d(i){
    return document.getElementById(i);
}
function dc(i, j){
    j=j||0;
    return document.getElementsByClassName(i)[j];
}
function da(i, j, f){
    d(i).addEventListener(j, f);
}
function er(m){
    console.log(m);
}
function img(src){
    return "<a href='"+src+"'><img src='"+src+"' class='center img' alt='"+src.substring(0, src.lastIndexOf('.'))+"'></a>";
}
function xr(url, f){
    xhr = new XMLHttpRequest();
    xhr.onreadystatechange = function(){
        if(this.readyState == 4 && this.status == 200){
            f(this);
        }
    }
    xhr.open('GET', url, true);
    xhr.send();
}
function progressBar(w){
  dc('progress').childNodes[1].childNodes[0].style.width = w+'%';
}
document.addEventListener('DOMContentLoaded', function(){
    dc('progress').childNodes[1].style.width = dc('uplabel').offsetWidth + 'px';
    xr('/img', function(self){
        er(self.responseText);
        imgJSON = self.responseText;
        imgJSON = imgJSON.substring(0, imgJSON.length - 2) + ']';
        imgJSON  = JSON.parse(imgJSON);
        er(imgJSON);

        var iL = Math.ceil(imgJSON.length/3);
        for(var i = 0; i<iL; i++){
            var t = "<div class='row'>";
            for(var j = 0; j<3; j++){
                var src = imgJSON[i*3 + j] || 0;
                t += "<div class='c3 px pt'>";
                if(src !== 0){
                    t+=img(src);
                }
                t+="</div>";
            }
            t+="</div>";
            d('img-container').innerHTML += t;
        }
    });
});

d('ufile').addEventListener('change', function(e) {
    var file = this.files[0];
    if(file.name.length <= 60){
        var pBar = dc('progress').childNodes[1];
        var fd = new FormData();
        fd.append('ufile', file);
        
        var xhr = new XMLHttpRequest();
        xhr.open('POST', '/edit?dir=1', true);
        d('ufile').value ='';

        xhr.upload.onprogress = function(e) {
            pBar.style.display = 'table';  
            var percentage = e.lengthComputable ?(e.loaded / e.total) * 100 : 0;
            progressBar(percentage);
        };

        xhr.onreadystatechange = function(){
            if(this.readyState == 4 && this.status == 200){
                pBar.style.display = 'none';
                progressBar('0');
            }
        };
        xhr.send(fd);
    }
    else{
        alert('file name too long');
    }
  });

