f(){
    return 17;
}

g(i){
    return i+1;
}

main(){
    var n;

    if (f == g) { // error: function comparison
        n=10;
    }

    return 0;
}