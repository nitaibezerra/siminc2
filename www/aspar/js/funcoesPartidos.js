$(function(){
    $("#formulario").submit(function(){
        salvar();
        return false;
    });  
});

function excluir(id){
    swal({
        title: "Atenção",
        text: "Deseja Realmente excluir o registro?",
        type: "warning",
        showCancelButton: true,
        confirmButtonText: "Sim",
        closeOnConfirm: false,
        cancelButtonText: "Não"
        },
        function (isConfirm) {
            if (isConfirm) {
                $.ajax({
                    url: 'aspar.php?modulo=apoio/partidos/index&acao=A',
                    data: {
                        prtid: id, 
                        action: 'excluir'
                    },
                    method: 'post',
                    success: function (result) {
                        result = eval('('+result+')');
                        if (result=='success'){
                            swal({  
                                closeOnConfirm: false, 
                                title: "Sucesso", 
                                type: "success",
                                text: "Registro excluído com sucesso!",   
                                confirmButtonText: "OK" },
                                function(isConfirm){
                                    if (isConfirm){
                                        window.location.href ='aspar.php?modulo=apoio/partidos/index&acao=A';
                                    }
                                });
                            
                        }else{
                            swal({
                                title: "Sucesso",
                                text: "Erro ao excluir o registro! "+result,
                                type: "error",
                                confirmButtonText: "OK"
                                },
                                function () {window.location.href ='aspar.php?modulo=apoio/partidos&acao=A';}
                            );
                        }                        
                    }
                });                
            }
        }
    );

}

function editarPopup( id ){
    $.ajax({
        url: 'aspar.php?modulo=apoio/partidos/index&acao=A',
        data: {
            prtid: id, 
            action: 'carregarPartidos'
        },
        method: 'post',
        success: function (result) {
            result = eval('('+result+')');
            $("#prtdsc").val(result.prtdsc);
            $("#prtid").val(id);
            $('#myModal').modal('show');                       
        }
    });        
}

function salvar(){
    $.ajax({
        url: 'aspar.php?modulo=apoio/partidos/index&acao=A',
        data: {
            prtid: $("#prtid").val(), 
            prtdsc: $("#prtdsc").val(),
            action: 'gravar'
        },
        method: 'post',
        success: function (result) {
            result = eval('('+result+')');
            if (result=='success'){
                swal({  
                    title: "Sucesso",
                    text: "Registro salvo com sucesso!",
                    type: "success",
                    confirmButtonText: "OK" },
                    function(isConfirm){
                        if (isConfirm){
                            window.location.href ='aspar.php?modulo=apoio/partidos/index&acao=A';
                        }
                    });                    
            }else{
                swal({
                    title: "Sucesso",
                    text: "Erro ao salvar o registro! "+result,
                    type: "error",
                    confirmButtonText: "OK"
                    },
                    function () {}
                );
            }            
        }
    });    
}