<?php
// monta cabeçalho
include APPRAIZ . 'includes/cabecalho.inc';
include APPRAIZ."sca/classes/Visita.class.inc";
$oVisita = new Visita();

print '<br/>';

$db->cria_aba($abacod_tela,$url,'');
monta_titulo( 'Consulta Visitas Pendentes', '&nbsp;' );

if( $_GET['limpar']){
    $oVisita->efetuarSaidaVisitasPendentes( $_POST['visitas'], $_SESSION['usucpf']);
}

?>

<!-- 
<script language="javascript" type="text/javascript" src="./js/ajax.js"></script>

<script language="javascript" type="text/javascript" src="../includes/funcoes.js"></script>
<script language="javascript" type="text/javascript" src="../includes/prototype.js"></script>
<script language="javascript" type="text/javascript" src="../includes/jquery.js"></script>
<script language='javascript' type="text/javascript" src="./js/sca.js"></script>
 -->
<script language='javascript' type='text/javascript' src='./js/sca.js'></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script src="../includes/calendario.js"></script>
<script language="JavaScript" src="/includes/funcoes.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script language="javascript" type="text/javascript" src="../includes/calendario.js"></script>
<script language="javascript" type="text/javascript">

    //função que dispara a pesquisa no banco
    function pesquisar(acao){
        var url = 'sca.php?modulo=principal/visitante/popup/consultarVisitasPendentes&acao=A';
        var formulario=document.getElementById("formulario");
        formulario.action = url;
        formulario.submit();
    }

    function exibirMensagem(msg){
        alert(msg);
    }

    function excluirVisitas(){
        var url = 'sca.php?modulo=principal/visitante/popup/consultarVisitasPendentes&acao=A&limpar=1';
        var formulario=document.getElementById("formularioVisitasPendentes"); // O nome formularioVisitasPendentes, esta sendo definido no metodo $oVisita->listarVisitasPendentes
        formulario.action = url;
        formulario.submit();
    }

    function visualizaFoto( vttid ){
    	alert( vttid );
    }

    function CheckAll(){
    	var formulario=document.getElementById("formularioVisitasPendentes");
    	for (i=0;i<formulario.elements.length;i++){ 
            if(formulario.elements[i].type == "checkbox"){	
            	if(formulario.elements[i].checked==1){
            		formulario.elements[i].checked=0;
                }else{
                	formulario.elements[i].checked=1;
                }    
            }
    	} 
    }
</script>

    <table class="tabela" align="center" cellpadding="2" cellspacing="1" bgcolor="#f5f5f5" width="100%">
        <tr>
            <td>
            
            <form name="formulario" id="formulario" method="post">
                <table class="tabela" align="center" cellpadding="2" cellspacing="1" bgcolor="#f5f5f5" width="100%">
                    <tr>
                        <td class="SubTituloDireita">Data Inicial:</td>
                        <td>
                        <?//=campo_texto('vstdatentrada','','','',60,100,'','','left','',0,'id="vstdatentrada"','',$_REQUEST['vstdatentrada']);?>
                        <?=campo_data2('vstdatentrada',false,true,'','N','Data Inicial','Change', $_REQUEST['vstdatentrada'] );?>
                        <?//=campo_data2('vstdatentrada2',false,true,'Data','S','AAA','', $_REQUEST['vstdatentrada'],'', '', $id='vstdatentrada2' );?>
                        </td>
                    </tr>
                    <tr>
                        <td bgcolor="#CCCCCC" colspan="2" align="center">
                            <input type="button" name="visualizar" value="Consultar" style="cursor:pointer;" onclick="pesquisar('filtrar');">
                            <!-- input type="button" name="vertodos" value="Ver Todos" style="cursor:pointer;" onclick="pesquisar('todos');"-->
                            <input type="button" name="efetuarSaida" value="Efetuar Saída" style="cursor:pointer;" onclick="excluirVisitas();">
                            <input type="button" name="marcarTodos" value="Marcar/Desmarcar Todos" style="cursor:pointer;" onclick="CheckAll();">
                        </td>
                    </tr>
                </table>
            </form>
            <script>
            </script>
    
        <?php $oVisita->listarVisitasPendentes( $_REQUEST['vstdatentrada']); ?>

		</td>
    </tr>
</table>

