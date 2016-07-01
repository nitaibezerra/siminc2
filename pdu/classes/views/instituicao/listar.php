<script language="javascript">
    function abrir(id , element){

        $.post(window.location.href , { controller: 'instituicao' , action: 'listarDados' , id: id} , function(html){
            $(element).closest('tr').after(function(){return $('<tr ><td style="width: 5%; background-color: #fff;"></td><td  colspan="4" style="padding: 0px 0px 0px 0px;">' + html + '</td></tr>').hide().toggle(300);});
            $(element).closest('tr').find('td:first').html('<a href="javascript:void(0);" onclick="fechar(\'' + id + '\' , this)"><i class="glyphicon glyphicon-minus"></i></a>');
        });
    }

    function fechar(id , element){
        $($(element).closest('tr').next()[0]).hide( function(){$(this).remove()} );
        $(element).closest('tr').find('td:first').html('<a href="javascript:void(0);" onclick="abrir(\'' + id + '\' , this)"><i class="glyphicon glyphicon-plus"></i></a>');
//            $($(element).closest('tr').next()[0]).remove();
    }
</script>