<link href="/includes/JQuery/jquery-ui-1.8.4.custom/css/jquery-ui.css" rel="stylesheet" type="text/css"/>
<script src="/includes/JQuery/jquery-1.7.2.min.js"></script>
<script src="/includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-ui-1.8.4.custom.min.js"></script> 

<?php
//teste svn
monta_titulo('Lista Agência / Escola' , 'Escolha o Escola ao qual deseja vincular agência');

$imgAgenciaSemAcao   = "'<img style=\"border: none;\" src=\"/imagens/pju_agencia.png\" alt=\"Agência\" title=\"Agência\" />'";
$imgAgenciaComAcao   = "'<img style=\"border: none; cursor:pointer;\" src=\"/imagens/pju_agencia.png\" alt=\"Agência\" title=\"Agência\" onclick=\"modalVinculaAgencia('||ent.entid||')\"  />'";
$imgAgenciaVinculada = "'<img style=\"border: none;\" src=\"/imagens/pju_vinculado.png\" alt=\"Agência Vinculada\" title=\"Agência Vinculada\" />'";

$paramentros['imgAgenciaVinculada']  = $imgAgenciaVinculada; 
$paramentros['imgAgenciaComAcao'] 	 = $imgAgenciaComAcao;

$retorno = retornaNomeDaAgenciaCadastrada( $paramentros );
                
$cabecalho	 = array('Ações', 'Escola','Estado','Município','Endereço do Escola', 'Cep', 'Agência');// plm.apcid ='".$_SESSION['projovemcampo']['apcid']."' ";
$tamanho	 = array("5%","5%","10%","10%","40%","10%","20%");
$alinhamento = array("center","center","center","center","left","center","left");
$db->monta_lista( $retorno, $cabecalho, 20, 5, 'N', 'center', $par2, "", $tamanho, $alinhamento );
?>

<div id="modalAgencia">
&nbsp;


</div>
<script type="text/javascript">
    function modalVinculaAgencia(entid)
    {
            var params = {
                            entid : entid, 
            };

            $.get( "projovemcampo.php?modulo=principal/monitoramento/associarAgencia&acao=A", params, function(resposta){
                    $( "#modalAgencia" ).html(resposta);
                    $( "#modalAgencia" ).dialog({
                        draggable:true,
                        resizable:true,
                     close: function(){
                       
                        document.location.reload(true);
                     } 
                    });
            });
    };
</script>